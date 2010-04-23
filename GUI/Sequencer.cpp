/*
 Copyright: 
 BlueYeti (http://www.blueyeti.fr)
 
 Adviser(s): 
 Jean-Michel Couturier (jmc_at_blueyeti_dot_fr)
 
 This software is a computer program whose purpose is to propose
 a library for interactive scores edition and execution.
 
 This software is governed by the CeCILL license under French law and
 abiding by the rules of distribution of free software.  You can  use, 
 modify and/ or redistribute the software under the terms of the CeCILL
 license as circulated by CEA, CNRS and INRIA at the following URL
 "http://www.cecill.info". 
 
 As a counterpart to the access to the source code and  rights to copy,
 modify and redistribute granted by the license, users are provided only
 with a limited warranty  and the software's author,  the holder of the
 economic rights,  and the successive licensors  have only  limited
 liability. 
 
 In this respect, the user's attention is drawn to the risks associated
 with loading,  using,  modifying and/or developing or reproducing the
 software by the user in light of its specific status of free software,
 that may mean  that it is complicated to manipulate,  and  that  also
 therefore means  that it is reserved for developers  and  experienced
 professionals having in-depth computer knowledge. Users are therefore
 encouraged to load and test the software's suitability as regards their
 requirements in conditions enabling the security of their systems and/or 
 data to be ensured and,  more generally, to use and operate it in the 
 same conditions as regards security.
 
 The fact that you are presently reading this means that you have had
 knowledge of the CeCILL license and that you accept its terms.
 */
/*
 *  Sequencer.cpp
 *  Virage
 *
 *  Created by Henry Bernard and Luc Vercellin on 02/10/08.
 *
 */

#include "Sequencer.h"
#include "SequencerFrame.h"
#include "MainWindow.h"
#include "HeadPlay.h"

typedef map<unsigned int,TimeBox*> BoxMap;
typedef map<unsigned int,Relation*> RelMap;
typedef map<unsigned int,Trigger*> TrgMap;

const float Sequencer::MS_2_PIX_RATIO = 10; // 10ms/pixel
const float Sequencer::BPIX_2_SPIX_RATIO = 1.0f; // facteur 2 d'affichage

//==============================================================================
Sequencer::Sequencer(SequencerFrame * _parent,  unsigned int _maxDuration, int _height) {
	
	//Prevent NSAutoreleaseNoPool memory leaks (plateforme-specific : mac os)
#if JUCE_MAC || JUCE_IPHONE
#define AUTO_RELEASE_POOL const ScopedAutoReleasePool pool
#else
#define AUTO_RELEASE_POOL //empty
#endif
	
	isResizingByMouseOT = true;
	isZooming = false;
	
	m_spaceBar_temporisator = 0;
	m_magnetised = true;
	
	m_otManager = new OtManager(this);
	
	//horizontal
	m_SCENE_MAXLENGTH_MS = _maxDuration;
	//vertical
	m_SCENE_ROOTHEIGHT = _height;
	
	setSize(_maxDuration/MS_2_PIX_RATIO, _height);
	
	/*controllers zoom + Play */
	m_zoomer = new ZoomManager(MS_2_PIX_RATIO, BPIX_2_SPIX_RATIO);
	m_controller = new PlayController(this);
	m_frameParent = _parent;
	//generation
	setFocusContainer(true);//met les ->tab en inner-boucle
	//lasso
	m_electedItemSet = new SelectedItemSet<Component*>();
	_allOtSelected = false;
	
	_dragBoxId=NO_ID;
	_dropBoxId=NO_ID;
	_dragTrigId=NO_ID;
	_dropTrigId=NO_ID;
	_boxMap = new BoxMap;
	_relMap = new RelMap;
	_trigMap = new TrgMap;
	//drag&drop & draw box
	_anchoringStart = false;
	_anchoringEnd = false;
	_somethingIsBeingDraggedOver = false;
	_isDrawingBox = false;
	_isSelecting = false;
	_isDrawingBus = false;
	_isRelBeingDragged = false;
	
	//red circle
	addChildComponent(_facilityCircle = new Facility(), 0);
	_facilityCircle->setAlwaysOnTop(true);
	
	//CSP Engine
	m_engines = new Engines(_maxDuration);
	m_engines->addCrossingCtrlPointCallback(&controlPointCrossedCallBack);
    m_engines->addCrossingTrgPointCallback(&triggeredTPCallBack);
	m_engines->addExecutionFinishedCallback(&executionFinishedCallBack);
	m_engines->addEnginesNetworkUpdateCallback(&transportBarCallBack);
	
	m_relControlPointMother = 0;
	m_relControlPointDaughter = 0;
	
	//MouseMode
	m_mouseMode = DRAWBOX;/*  cf enum dans 'includes.h' */
	//tete de lecture
	addAndMakeVisible(m_headPlay = new HeadPlay(this));
	m_headPlay->setTimePos(0);
	m_headPlay->setPixPosition( m_zoomer->convertTimeToPix(m_headPlay->getTimePos()+2) );
	
	tempLink = new TemporaryLink(this);
	previewRect = new TemporaryBox(this);
	
	m_safeUpdater = new SafeUpdater(this);
	
	m_wantsStop = true;
	m_areJacksVisible = true;
	
	m_cursorFactory = new CursorFactory();
	refreshCursor();
}

Sequencer::~Sequencer() {
	
	//   clearAllOt();
	
	if(previewRect) {
		deleteAndZero( previewRect );
	}
	if(tempLink) {
		deleteAndZero( tempLink );
	}
	if (  m_electedItemSet ) {
		deleteAndZero( m_electedItemSet );
	}
	if ( m_otManager ) {
		deleteAndZero( m_otManager );
	}
	if ( m_engines ) {
		deleteAndZero( m_engines );
	}
	if ( _boxMap ) {
		deleteAndZero(_boxMap );
	}
	if ( _relMap ) {
		deleteAndZero(_relMap );
	}
	if ( _trigMap ) {
		deleteAndZero(_trigMap );
	}
	if ( m_controller ) {
		deleteAndZero( m_controller );
	}
	if ( m_zoomer ) {
		deleteAndZero( m_zoomer );
	}
	if ( m_headPlay ) {
		deleteAndZero( m_headPlay );
	}
	deleteAllChildren();
}

//==============================================================================
OtManager* Sequencer::getOtManager() {
	return m_otManager;
}

ZoomManager* Sequencer::getZoomer() {
	return m_zoomer;
}

bool Sequencer::wantsStopWhatever() {
	
	return m_wantsStop;
}

void Sequencer::setAskForStop(bool _shouldStopEvenIfLoop) {
	
	m_wantsStop = _shouldStopEvenIfLoop;
}

void Sequencer::refreshComment() {
	
	getMainWindow()->getSafeComponentGetter()->getMiddleFrame()->refreshComment();
}

TransportBar* Sequencer::getTransportBar() {
	return getMainWindow()->getTransportBar();
}

void Sequencer::setMessaging(bool _isMessaging, E_Messaging _inOrOut) {
	getMainWindow()->getTransportBar()->turnMinuitActivity(_isMessaging, _inOrOut);
}

int Sequencer::getDuration() { return m_SCENE_MAXLENGTH_MS; }

int Sequencer::getRootHeight() { return m_SCENE_ROOTHEIGHT; }

void Sequencer::showComments(unsigned int boxId) {
	if (boxId != NO_ID) {
		TimeBox *currentBox = NULL;
		if ((currentBox = getBox(boxId)) != NULL) {
			getMainWindow()->getSafeComponentGetter()->getMiddleFrame()->getCommPanel()->appendCurrentComment(currentBox->getTitle() + ": " + currentBox->getComment());
			
			//----------------------------------------------------------------------------
			//available comments :
			int min1 = INT_MAX;
			int min2 = INT_MAX;
			int tmpDist = 0;
			TimeBox* nextBox1 = 0;
			TimeBox* nextBox2 = 0;
			
			//look for the first next nearest box to display it comment
			BoxMap::iterator it = _boxMap->begin();
			while( it != _boxMap->end()){
				
				tmpDist = it->second->getPlugStartX() - currentBox->getPlugStartX();
				if(it->second != currentBox && tmpDist >= 0 && tmpDist < min1){
					
					min1 = tmpDist;
					nextBox1 = it->second;
				}
				it++;
			}
			//look for the second next nearest box to display it comment
			it = _boxMap->begin();
			while( it != _boxMap->end()){
				
				tmpDist = it->second->getPlugStartX() - currentBox->getPlugStartX();
				if(it->second != currentBox && it->second != nextBox1 && tmpDist >= 0 && tmpDist < min2){
					
					min2 = std::min(min2,tmpDist);
					nextBox2 = it->second;
				}
				it++;
			}
			
			
			/*ATTENTION AUX DOUBLONS*/
			if(nextBox1 != 0 && nextBox1->getTriggersMap()->empty()){
				//	nextBox1->showComment();
				if(nextBox1->getComment().length() != 0)
				{
					getMainWindow()->getSafeComponentGetter()->getMiddleFrame()->getCommPanel()->appendAvailableComment(nextBox1->getTitle() + ": " + nextBox1->getComment());
				}
			}
			if(nextBox2 != 0 && nextBox2->getTriggersMap()->empty()){
				//	nextBox2->showCommentFrame();
				if(nextBox1->getComment().length() != 0)
				{
					getMainWindow()->getSafeComponentGetter()->getMiddleFrame()->getCommPanel()->appendAvailableComment(nextBox2->getTitle() + ": " + nextBox2->getComment());
				}
			}
		}
	}
}

void Sequencer::appendTriggeredBoxComment(String _comment) {
	
	getMainWindow()->getSafeComponentGetter()->getMiddleFrame()->getCommPanel()->appendAvailableComment(_comment);
}

void Sequencer::reinitAfterReading() {
	
	m_headPlay->setVisible(true);
	
	BoxMap::iterator it = _boxMap->begin();
	while( it != _boxMap->end()){
		it->second->raz();
		it++;
	}
	TrgMap::iterator it2 = _trigMap->begin();
	while( it2 != _trigMap->end()){
		it2->second->init();
		it2++;
	}
	
	sendSynchronousChangeMessage(this);//	setChrono(m_headPlay->getTimePos());
	
	repaint();
}

void Sequencer::removeComment(unsigned int boxId) {
	if (boxId != NO_ID) {
		TimeBox *box;
		if ((box = getBox(boxId)) != NULL) {
			String name,comment;
			comment = box->getComment();
			if (! comment.isEmpty()) {
				name = box->getName();
				if (! name.isEmpty()) {
					getMainWindow()->getSafeComponentGetter()->getMiddleFrame()->getCommPanel()->removeFromCurrentComment(name + ": " + comment);
					getMainWindow()->getSafeComponentGetter()->getMiddleFrame()->getCommPanel()->removeFromAvailableComment(name + ": " + comment);
				}
			}
		}
	}
}

void Sequencer::removeComment(String comment) {
	getMainWindow()->getSafeComponentGetter()->getMiddleFrame()->getCommPanel()->removeFromCurrentComment(comment);
	getMainWindow()->getSafeComponentGetter()->getMiddleFrame()->getCommPanel()->removeFromAvailableComment(comment);
}

void Sequencer::letsGoForReading() {
	
	m_headPlay->setVisible(false);
	
	BoxMap::iterator it = _boxMap->begin();
	while( it != _boxMap->end()){
		it->second->init();//rajoute la progressbar
		it++;
	}
	
	getMainWindow()->getSafeComponentGetter()->getMiddleFrame()->getCommPanel()->reset();
}

void Sequencer::concatComments() {
	String concatenedComments;
	BoxMap::iterator it = _boxMap->begin();
	while( it != _boxMap->end()){
		if(it->second->getComment() != 0){
			concatenedComments += it->second->getName() + ": " + it->second->getComment() + "\n";
		}
		it++;
	}
	getMainWindow()->getSafeComponentGetter()->getMiddleFrame()->getCommPanel()->fillComments(concatenedComments);
}

void Sequencer::setParentWindow(MainWindow* _window) {
	
	m_ParentWindow = _window;
	addChangeListener(m_ParentWindow);/* simplement pour demander à la fenetre principale de rajouter une "*" à coté du nom du fichier */
}

void Sequencer::paint(Graphics& g) {
	
	/*background*/
	g.fillAll (s_BGCOLOUR);
	
	/*trigger bar*/
	g.drawLine(0, getViewedPositionY() +s_TRIGRAIL_HEIGHT, getWidth(), getViewedPositionY() +s_TRIGRAIL_HEIGHT);
	if (isMagnetised()) {	
		for (int i=0; i< getHeight(); i+= S_MAGNETISM_DELTA_H) {
			g.drawLine(0, s_TIMERAIL_HEIGHT +i, getWidth(), s_TIMERAIL_HEIGHT+i);
		}
	}
	g.setColour(Colours::white);
	g.fillRect(0, getViewedPositionY() +0, getWidth(), s_TIMERAIL_HEIGHT);//rect blanc
	g.setColour(s_COLOUR_SELECTION_GREEN.withAlpha(0.3f));
	g.fillRect(0, getViewedPositionY() +0, getWidth(), s_TRIGRAIL_HEIGHT-1);//rect vert
	
	/*lines*/
	g.setColour(Colours::darkgrey);
	g.setOpacity(0.2f);
	int value = 1;
	bool ok = false;
	while (!ok) {
		if((value * 1000 / getZoomer()->getRatio()) < s_MIN_XSCALE) {
			value *= s_TIME_LEGEND_MULTIPLE;
		}
		else {
			ok = true;
		}
	}
	
	//ligne séparation entra TIME LINE et TRIG LINE ZONE BLANCHE
	g.drawLine(0, getViewedPositionY() + s_TRIGRAIL_HEIGHT, getWidth(),
			   getViewedPositionY() + s_TRIGRAIL_HEIGHT, 1);
	
	for(int i = 0; i < getZoomer()->convertPixToTime(getWidth()); i += 1000/*1sec*/ * value ){
		int x = getZoomer()->convertTimeToPix(i);
		//high lines
		g.drawLine(x, getViewedPositionY() + s_TRIGRAIL_HEIGHT, x, getViewedPositionY()+getHeight());
		//légende - rail
		g.drawText(String(i/1000), x+1, getViewedPositionY() + 4 + s_TRIGRAIL_HEIGHT, 50, s_TIMERAIL_HEIGHT - s_TRIGRAIL_HEIGHT, Justification::left, true);
	}
	
	/*repaint next Trig*/
	if(nextTrig() != 0) nextTrig()->setColour(s_COLOUR_SELECTION_GREEN);
	
#ifdef VDBG
	/*durées des relations*/
	g.setColour(Colours::red);
	g.drawText(String("Goto timepos : ") + String(m_headPlay->getTimePos()), 0, -20, 1000, 200, Justification::left, false);
	g.drawText(String("Goto pixpos : ") + String(m_headPlay->getPixPos()),   0, -40, 1000, 200, Justification::left, false);
	g.drawText(String("Num selected : ") + String( m_electedItemSet->getNumSelected() ),   0, -80, 1000, 200, Justification::left, false);
	
#endif
}

void Sequencer::timerCallback() {
	getMainWindow()->getTransportBar()->setChrono(m_engines->getCurrentExecutionTime());
	getMainWindow()->getTransportBar()->updateAbsoluteChrono();
}

//===============================================================================================================================================

void
Sequencer::mouseDown(const MouseEvent &e) {
	
	if(/*m_engines->isRunning() || */e.mods.isPopupMenu()) {
		if(!e.mouseWasClicked()) return;
		PopupMenu m;
		m.addItem (1, "RESET");
		
		const int result = m.show();
		
		if (result == 1) {
			resetHeadPlay();
		}
		return;
	}
	/*SI EN MODE EDITION :*/
	
	//if(!e.mods.isAnyModifierKeyDown() ) deselectAll();
	
	if(!m_engines->isRunning() && getHitAnchorBoxId(e.x, e.y) != NO_ID) {
		
		_isRelBeingDragged = true;
		
		_dragBoxId = getHitAnchorBoxId(e.x, e.y);
		TimeBox* dragBox = getBox(_dragBoxId);
		tempLink = new TemporaryLink(this);
		tempLink->setBoxOrigin(dragBox, _anchoringStart?1:2);
		tempLink->setBounds(0,0,getWidth(),getHeight());
		tempLink->setDownX(e.getMouseDownX());
		tempLink->setDownY(e.getMouseDownY());
		
		//CSP validation de la relation
		if(_anchoringStart) {
			m_relControlPointMother = BEGIN_CONTROL_POINT_INDEX;
			tempLink->setZone(dragBox->getPlugStartX(), dragBox->getPlugY(),  e.x, e.y );
			tempLink->setDownX(dragBox->getPlugStartX());
			tempLink->setDownY(dragBox->getPlugY());
		}
		else {
			m_relControlPointMother = m_engines->getBoxLastCtrlPointIndex(_dragBoxId);
			tempLink->setZone(dragBox->getPlugEndX(),   dragBox->getPlugY(),  e.x, e.y );
			tempLink->setDownX(dragBox->getPlugEndX());
			tempLink->setDownY(dragBox->getPlugY());
		}
		
		addAndMakeVisible(tempLink);
		desactivateFacility();
		
		return;
	}
	else if(!m_engines->isRunning() && getHitTriggerId(e.x, e.y) != NO_ID) {
		
		_isRelBeingDragged = true;
		_dragTrigId = getHitTriggerId(e.x, e.y);
		
		tempLink = new TemporaryLink(this, false);
		tempLink->setTriggerOrigin(getTrigger(_dragTrigId));
		tempLink->setBounds(0,0,getWidth(),getHeight());
		tempLink->setDownX(getTrigger(_dragTrigId)->getWidth()/2 + getTrigger(_dragTrigId)->getX());
		tempLink->setDownY(getTrigger(_dragTrigId)->getHeight()  + getTrigger(_dragTrigId)->getY());
		tempLink->setZone(tempLink->getDownX(), tempLink->getDownY(),  e.x, e.y );
		
		addAndMakeVisible(tempLink);
		desactivateFacility();
		return;
	}
	else {
		switch (getMouseMode()) {
			case DRAWBOX:
				if(!m_engines->isRunning()) _isDrawingBox = true;
				previewRect->setDownX(e.x);
				previewRect->setDownY(e.y);
				break;
			case SELECTION:
				_isSelecting = true;
				addChildComponent (&lassoComp);
				lassoComp.beginLasso (e, this);
				lasso_xi = e.x; lasso_yi = e.y;
				break;
			default:
				break;
		}
	}
}

void
Sequencer::mouseDrag(const MouseEvent &e) {
	
	/*SI EN MODE EDITION*/
	//si on dessine déjà une relation et qu'on est pas encore arrivé sur un ancre : on rafraichit sur (x,y) de la souris
	if(!m_engines->isRunning() && _isRelBeingDragged){
		
		desactivateFacility();
		tempLink->setZone(tempLink->getDownX(), tempLink->getDownY(),  e.x, e.y );
		int distance=0;
		_dropBoxId=0;
		_dropTrigId=0;
		/*rappel : DIST_MIN_CONNEXION défini*/
		int min = 32768;//distance minimale entre le pointeur souris et l'OT accrochable le plus proche
		int closest_box_id =0;//id du plus proche SequenceOT - on ne tiens pas compte des relations
		int closest_trig_id =0;//id du plus proche SequenceOT - on ne tiens pas compte des relations
		BoxMap::iterator itBox = _boxMap->begin();
		while(itBox!=_boxMap->end()) {
			TimeBox* box = itBox->second;
			if(box->getId() != _dragBoxId || _dragTrigId!=0){
				distance = /*sqrt*/( (e.x - box->getPlugStartX())*(e.x - box->getPlugStartX()) + (e.y - box->getPlugY())*(e.y - box->getPlugY()));//hypothénuse
				if(distance < min && distance < DIST_MIN_CONNEXION*DIST_MIN_CONNEXION) {min = distance; closest_box_id = itBox->first; _anchoringEnd=true;}
				distance = /*sqrt*/( (e.x - box->getPlugEndX())*(e.x - box->getPlugEndX())   + (e.y - box->getPlugY())*(e.y - box->getPlugY()));//hypothénuse
				if(distance < min && distance < DIST_MIN_CONNEXION*DIST_MIN_CONNEXION) {min = distance; closest_box_id = itBox->first; _anchoringEnd=false;}
			}
			itBox++;
		}
		
		if(_dragTrigId == 0){
			TrgMap::iterator itTrig = _trigMap->begin();
			while(itTrig!=_trigMap->end()) {
				Trigger* trig = itTrig->second;
				distance = /*sqrt*/( (e.x - trig->getPlugX())*(e.x - trig->getPlugX()) + (e.y - trig->getPlugY())*(e.y - trig->getPlugY()));//hypothénuse^2
				if(distance < min && distance < DIST_MIN_CONNEXION*DIST_MIN_CONNEXION) {min = distance; closest_trig_id = itTrig->first;}
				itTrig++;
			}
		}
		/**/
		if ( closest_trig_id > 0) {
			
			Trigger* trig = getTrigger(closest_trig_id);
			activateFacility(trig);
			_dropTrigId = closest_trig_id; _dropBoxId=0;
			
			/*VFeedBack sur le temp_link*/
			tempLink->blink(true);
		}
		
		else if (closest_box_id>0) {
			
			TimeBox* box = getBox(closest_box_id);
			activateFacility(box, _anchoringEnd);
			_dropBoxId = closest_box_id;
			
			tempLink->blink(true);
		}
		else {
			desactivateFacility();
			
			tempLink->blink(false);
		}
	}
	else if(!m_engines->isRunning() && _isDrawingBox) {
		
		previewRect->setZone(previewRect->getDownX(), previewRect->getDownY(), e.x, e.y);
		addAndMakeVisible(previewRect);
	}
	else if(_isSelecting) {
		
		_isDrawingBox = false;
		lassoComp.dragLasso(e);
	}
	
	repaint();
}

void
Sequencer::mouseUp(const MouseEvent &e) {
	
	desactivateFacility();
	
	/*SI EN MODE EXECUTION*/
	if(m_engines->isRunning()) return;
	/*GOTO CURSOR - HEAD PLAY*/
	if( e.y > getViewedPositionY() + s_TRIGRAIL_HEIGHT && e.y <= getViewedPositionY() + s_TIMERAIL_HEIGHT){
		
		m_headPlay->setPixPosition(e.x);
		m_headPlay->setTimePos( m_zoomer->convertPixToTime(e.x) );
		
	}
	
	//END CONNECTION ?
	if(_isRelBeingDragged){
		if (tempLink) {
			deleteAndZero(tempLink);
		}
		_isRelBeingDragged=false;
		if(_dropBoxId>0) {
			if(_dragBoxId > 0){//de la boite vers la boite
				if(_anchoringEnd) {
					m_relControlPointDaughter = BEGIN_CONTROL_POINT_INDEX;
				}
				else {
					m_relControlPointDaughter = END_CONTROL_POINT_INDEX;
				}
				getOtManager()->addRelation(_dragBoxId, _dropBoxId, m_relControlPointMother, m_relControlPointDaughter);
			}
			else if( _dragTrigId > 0 ) {// du trigger vers la boite
				
				Trigger* dragTrigger = getTrigger(_dragTrigId);
				TimeBox* dropBox = getBox(_dropBoxId);
				/*[GUI]*//*on avertit le trigger (et on le "set minuit message")*/
				dragTrigger->setLinkedBox(dropBox, _anchoringEnd);
				/*[ENGINE] on connecte le trigger*/
				if(_anchoringEnd) {
					m_engines->assignCtrlPointToTriggerPoint(_dragTrigId, _dropBoxId, BEGIN_CONTROL_POINT_INDEX);
				}
				else {
					m_engines->assignCtrlPointToTriggerPoint(_dragTrigId, _dropBoxId, END_CONTROL_POINT_INDEX);
				}
			}
		}
		else if(_dropTrigId>0) {//de la boite vers le trigger
			
			Trigger* dropTrigger = getTrigger(_dropTrigId);
			TimeBox* dragBox = getBox(_dragBoxId);
			/*[GUI]*/
			/*on avertit le trigger (et on le "set minuit mesage")*/
			dropTrigger->setLinkedBox(dragBox, _anchoringStart);
			/*[ENGINE] on connecte le trigger*/
			if(_anchoringStart)	{
				m_engines->assignCtrlPointToTriggerPoint(_dropTrigId, _dragBoxId, BEGIN_CONTROL_POINT_INDEX);
			}
			else	{
				m_engines->assignCtrlPointToTriggerPoint(_dropTrigId, _dragBoxId, END_CONTROL_POINT_INDEX);
			}
		}
		
		/*reinit*/
		_dragBoxId=0;
		_dropBoxId=0;
		_dragTrigId=0;
		_dropTrigId=0;
		return;
	}
	//END SELECTION ?
	else if(_isSelecting) {
		_isSelecting = false;
		lassoComp.endLasso();
		lasso_xf = e.x; lasso_yf = e.y;
		return;
	}
	//END DRAWING ?
	else if(_isDrawingBox ) {
		_isDrawingBox = false;
		if((getZoomer()->convertPixToTime(abs(e.getDistanceFromDragStartX()))>100) && (abs(e.getDistanceFromDragStartY()) > 30))
		{
			//aire de tracé :
			int length = (abs(e.x-e.getMouseDownX()));
			int height = (abs(e.y-e.getMouseDownY()));
			
			int cc = (getOtManager()->getBoxCounter());
			getOtManager()->setBoxCounter(++cc);
			TimeBox* box = getOtManager()->addBox(String("object.") + String(getOtManager()->getBoxCounter()), false, std::max(0,(std::min(e.x,e.getMouseDownX()))), std::max(0, (std::min(e.getMouseDownY(),e.y))), length, height);
			
			//selection - ingérable dans le composant (besoin du MouseEvent)
			getSelectionSet()->addToSelectionBasedOnModifiers(box, e.mods);
		}
		previewRect->setZone(0,0,0,0);
		previewRect->setVisible(false);
		repaint();
		
		return;
	}
}

void
Sequencer::mouseMove(const MouseEvent &e) {
	
	refreshCursor();
	if(e.y < s_TIMERAIL_HEIGHT)
		setMouseCursor(MouseCursor::MouseCursor(MouseCursor::NormalCursor));
	if(e.y < s_TRIGRAIL_HEIGHT)
		setMouseCursor(MouseCursor::MouseCursor(MouseCursor::PointingHandCursor));
	
	
	/*SI EN MODE EDITION*/
	if(m_engines->isRunning()) return;
	
	unsigned int id  = getHitAnchorBoxId(e.x, e.y);
	unsigned int idT = getHitTriggerId(e.x, e.y);
	if ( id > 0 )
	{
		TimeBox* overMousedBox = getBox(id);
		activateFacility(overMousedBox, _anchoringStart);
		return;
	}
	if ( idT > 0)
	{
		Trigger* overTrig = getTrigger(idT);
		activateFacility(overTrig);
		return;
	}
	
	desactivateFacility();
}

void
Sequencer::mouseDoubleClick(const MouseEvent& e) {
	
	if(e.mods.isPopupMenu()) return;
	if(m_engines->isRunning()) return;
	if(e.y < getViewedPositionY() + s_TRIGRAIL_HEIGHT) getOtManager()->addTrigger(e.x - s_TRIGRAIL_HEIGHT/2);
}

//==============================================================================

MainWindow*
Sequencer::getMainWindow() {
	return m_ParentWindow;
}

void
Sequencer::showCommentFrame(bool x) {
	m_ParentWindow->showCommentFrame(x);
}

unsigned int
Sequencer::getHitAnchorBoxId(int x, int y, unsigned int _minusId) {
	
	unsigned int id = NO_ID;
	
	BoxMap::iterator it  = _boxMap->begin();
	while (it != _boxMap->end())
	{
		TimeBox* item = it->second;
		
		int xn = x;
		int yn = y;
		relativePositionToOtherComponent(item->getAnchorLeft(), xn, yn);
		
		if(item->getAnchorLeft()->contains(xn, yn)) {
			_anchoringStart = true ;
			id = it->first;
			break;
		}
		
		int xm = x;
		int ym = y;
		relativePositionToOtherComponent(item->getAnchorRight(), xm, ym);
		
		if(item->getAnchorRight()->contains(xm, ym)) {
			_anchoringStart = false ;
			id = it->first;
			break;
		}
		
		it++;
	}
	return id;
}

unsigned int
Sequencer::getHitTriggerId(int x, int y) {
	
	unsigned int id = 0;
	TrgMap::iterator it = _trigMap->begin();
	
	while( it != _trigMap->end() ) {
		
		relativePositionToOtherComponent(it->second, x, y);
		if(it->second->contains(x,y-s_TRIGRAIL_HEIGHT )) {
			return it->first;
		}
		x += it->second->getX(); y += it->second->getY();
		it++;
	}
	return id;
}

//==============================================================================
void Sequencer::activateFacility(TimeBox* _bx, bool side_factor) {
	if(!_facilityCircle->isVisible())	{
		
		if(side_factor) _facilityCircle->setCenter(_bx->getPlugStartX(), _bx->getPlugY());
		else _facilityCircle->setCenter(_bx->getPlugEndX(), _bx->getPlugY());
		_facilityCircle->setSide(side_factor);
		_facilityCircle->setSize(s_FACILITY_AREA, s_FACILITY_AREA);
		_facilityCircle->setVisible(true);
	}
}

void Sequencer::activateFacility(Trigger* _tx) {
	if(!_facilityCircle->isVisible())	{
		
		_facilityCircle->setCenter(_tx->getPlugX(), _tx->getY() + _tx->getHeight());
		_facilityCircle->setSize(s_FACILITY_AREA, s_FACILITY_AREA);
		_facilityCircle->setVisible(true);
	}
}

void Sequencer::desactivateFacility() {
	if(_facilityCircle->isVisible() )
		_facilityCircle->fadeOutComponent(80/*ms*/, 0, 0, 0.7f/*ZOOM OUT*/);
}

Component* Sequencer::getFacilityCircle() {
	return (Component*)_facilityCircle;
}

//==============================================================================
void Sequencer::setViewedPositionX(int x) {
	visibleX=x;
}

void Sequencer::setViewedPositionY(int y) {
	visibleY=y;
}

int Sequencer::getViewedPositionX() {
	return visibleX;
}

int Sequencer::getViewedPositionY() {
	return visibleY;
}

//============ KEY LISTENER ========================================================
bool Sequencer::keyPressed	(const KeyPress & 	key) {
	
	if(!m_engines->isRunning()){
		//si on presse la touche Suppr
		if(key==KeyPress::deleteKey || key==KeyPress::backspaceKey) {
			if(_allOtSelected) {
				clearAllOt();
			}
			else if( getSelectionSet()->getNumSelected() > 0 ) {
				
				for(int i=0; i < getSelectionSet()->getNumSelected(); ++i) {
					Component* selectedObject = m_electedItemSet->getSelectedItem(i);
					if(getSelectionSet()->getNumSelected() != 0 &&   dynamic_cast<Relation*> (selectedObject) ) {
						getSelectionSet()->deselect(selectedObject);
						getOtManager()->removeRelation(((Relation*)selectedObject)->getId());
						i--;
					}
				}
				
				for(int i=0; i < getSelectionSet()->getNumSelected(); ++i) {
					Component* selectedObject = m_electedItemSet->getSelectedItem(i);
					if(getSelectionSet()->getNumSelected() != 0 &&   dynamic_cast<TimeBox*> (selectedObject) ) {
						getSelectionSet()->deselect(selectedObject);
						getOtManager()->removeBox(((TimeBox*)selectedObject)->getId());
						i--;
					}
				}
				
				for(int i=0; i < getSelectionSet()->getNumSelected(); ++i) {
					Component* selectedObject = m_electedItemSet->getSelectedItem(i);
					if(getSelectionSet()->getNumSelected() != 0 &&  dynamic_cast<Trigger*> (selectedObject) ) {
						getSelectionSet()->deselect(selectedObject);
						getOtManager()->removeTrigger( ( (Trigger*)selectedObject )->getId() );
						i--;
					}
				}
			}
			deselectAll();
						
			return true;
		}
		if(key == KeyPress(KeyPress (T('a'), ModifierKeys::commandModifier, 0))) {
			selectAll();
			return true;
		}
		if(key == KeyPress(KeyPress (T('d'), ModifierKeys::commandModifier, 0))) {
			keyPressed(KeyPress(KeyPress (T('c'), ModifierKeys::commandModifier, 0)));
			keyPressed(KeyPress(KeyPress (T('v'), ModifierKeys::commandModifier, 0)));
			return true;
		}
		if(key == KeyPress(KeyPress (T('c'), ModifierKeys::commandModifier, 0))) {
			m_copiedItemSet.deselectAll();
			for(int i=0; i < getSelectionSet()->getNumSelected(); ++i) {
				
				Component* selectedObject = (getSelectionSet()->getSelectedItem(i));
				m_copiedItemSet.addToSelection(selectedObject);
			}
			return true;
		}
		if(key == KeyPress(KeyPress (T('v'), ModifierKeys::commandModifier, 0))) {
			duplicateBufferedSelection();
			return true;
		}
		if(key == KeyPress::rightKey) {
			{
				simulateNextTrig();
				return true;
			}
		}
	}
	if(key == KeyPress(KeyPress (T('z')))) {
		setJacksVisible(false);return true;
	}
	if(key == KeyPress(KeyPress (T('a')))) {
		setJacksVisible(true);return true;
	}
	return false;
}

void Sequencer::setJacksVisible( bool x ) {
	
	m_areJacksVisible = x;
	
	TrgMap::iterator it = _trigMap->begin();
	while(it != _trigMap->end()) {
		it->second->getControlJack()->setVisible(x);
		it++;
	}
}

bool Sequencer::areJacksVisible() {
	
	return m_areJacksVisible;
}

void Sequencer::modifierKeysChanged(const ModifierKeys& modifiers) {
	
	//if(modifiers.getRawFlags() != 4 /*alt*/ ) return;
	getMainWindow()->getTransportBar()->getCrayonButton()->setToggleState(! modifiers.isAltDown(), true);
}

void Sequencer::simulateNextTrig() {
	
	if(nextTrig() != 0) m_engines->simulateNetworkMessageReception( (string)(nextTrig()->getMinuitMessage()) );
}

void Sequencer::burnTrigBefore(int _fromMs) {
	
	TrgMap::iterator it = _trigMap->begin();
	while ( it != _trigMap->end() ) {
		
		Trigger* trig = it->second;
		if(trig->isConnected() && getZoomer()->convertPixToTime(trig->getX()) < _fromMs ){
			
			trig->burn();
		}
		
		it++;
	}
}

Trigger* Sequencer::nextTrig() {
	
	Trigger* trigToTrig = 0;
	int minT = INT_MAX;
	TrgMap::iterator it = _trigMap->begin();
	while(it != _trigMap->end()) {
		
		Trigger* trigger = it->second;
		if( trigger->getX() < minT && trigger->getState() ==  WAITED_STATE) {
			
			minT = trigger->getX();
			trigToTrig = trigger;
		}
		//trigger=0;
		it++;
	}
	return trigToTrig;
}

void Sequencer::duplicateBufferedSelection() {
	
	/*getSelectionSet()->*/ deselectAll()   ;
	
	/* First, boxes */
	for(int i=0; i < m_copiedItemSet.getNumSelected(); ++i) {
		
		Component* selectedObject = m_copiedItemSet.getSelectedItem(i);
		
		if( dynamic_cast<TimeBox*> (selectedObject) ) {
			TimeBox* copiedBox = dynamic_cast<TimeBox*> (selectedObject);
			//getSelectionSet()->deselect(copiedBox);
			getOtManager()-> duplicateBox(copiedBox);
		}
	}
	/* then, constraints */
	for(int i=0; i < m_copiedItemSet.getNumSelected(); ++i) {
		
		Component* selectedObject = m_copiedItemSet.getSelectedItem(i);
		if( dynamic_cast<Relation*> (selectedObject) ) {
			Relation* copiedRel = dynamic_cast<Relation*> (selectedObject);
			
			unsigned int newMotherBoxId ;
			unsigned int newDaughterBoxId ;
			newMotherBoxId = copiedRel->getMotherId() + deltaCopiedId;
			newDaughterBoxId = copiedRel->getDaughterId() + deltaCopiedId;
			
			int motherCP = (getBox(copiedRel->getMotherId())->isRelStart(copiedRel->getId()))?CPT_L:CPT_R;
			int daughterCP = (getBox(copiedRel->getDaughterId())->isRelStart(copiedRel->getId()))?CPT_L:CPT_R;
			bool isFlexible = copiedRel->getDurationBehavior()->isFlexible();
			getOtManager()->addRelation(newMotherBoxId, newDaughterBoxId, motherCP, daughterCP, 0, isFlexible);
		}
	}
	/* finally, triggers. */
	for(int i=0; i < m_copiedItemSet.getNumSelected(); ++i) {
		
		Component* selectedObject = m_copiedItemSet.getSelectedItem(i);
		if( dynamic_cast<Trigger*> (selectedObject) ) {
			Trigger* copiedTrig = dynamic_cast<Trigger*> (selectedObject);
			
			if(copiedTrig->isConnected()) {
				unsigned int boxId = copiedTrig->getLinkedBox()->getId() + deltaCopiedId;
				TimeBox* newTimeBox = getBox(boxId);
				int newX = (copiedTrig->getLinkedCtrlPoint()==1)?newTimeBox->getPlugStartX():newTimeBox->getPlugEndX();
				Trigger* newTrigger = getOtManager()-> addTrigger(newX, copiedTrig->getColour());
				newTrigger->setLinkedBox(newTimeBox, copiedTrig->isAnchoringStart());
				
				/*[ENGINE] on connecte le trigger*/
				if(copiedTrig->isAnchoringStart()) {
					m_engines->assignCtrlPointToTriggerPoint(newTrigger->getId(), boxId, BEGIN_CONTROL_POINT_INDEX);
				}
				else	{
					m_engines->assignCtrlPointToTriggerPoint(newTrigger->getId(), boxId, END_CONTROL_POINT_INDEX);
				}
			}
			else {
				getOtManager()->addTrigger(copiedTrig->getX() + s_COPY_DELTA, copiedTrig->getColour());
			}
		}
	}
}

/**********************************************************************************/
/********************* CHILDREN MOVEMENTS *****************************************/
/**********************************************************************************/

void Sequencer::refresh() {
	
	int previousWidth = getWidth();
	
	m_headPlay->setPixPosition(m_zoomer->convertTimeToPix(m_headPlay->getTimePos()));
	
	/*vertical*/
	int newHeight = getZoomer()->convertRootPixToPix(m_SCENE_ROOTHEIGHT);
	/*horizontal*/
	int newWidth = getZoomer()->convertTimeToPix(m_SCENE_MAXLENGTH_MS);
	/*resize de this/sequencer*/
	setSize( newWidth, m_SCENE_ROOTHEIGHT/*newHeight*/);
	/*resize des boites*/
	map<unsigned int , TimeBox*>::iterator it;
	for (it = _boxMap->begin() ; it != _boxMap->end() ; it++) {
	    TimeBox* zoomedBox = it->second;
	    int id = zoomedBox->getId();
	    zoomedBox->setBounds(getZoomer()->convertTimeToPix(m_engines->getBoxBeginTime(id)),
							 zoomedBox->getY(), // getZoomer()->convertRootPixToPix(zoomedBox->getRootY()),
							 getZoomer()->convertTimeToPix(m_engines->getBoxEndTime(id) - m_engines->getBoxBeginTime(id)),
							 zoomedBox->getHeight() //getZoomer()->convertRootPixToPix(zoomedBox->getRootH())
							 );
	}
	/*resize des triggers*/
	float ratioWidth = 1.0f * newWidth / previousWidth; /*pour les triggers, on recalcule un ratio horizontal purment en pixel, sorte de zoom trivial horizontal*/
	map<unsigned int , Trigger*>::iterator iter = _trigMap->begin();
	while(iter  !=  _trigMap->end()) {
	    Trigger* zoomedTrig = iter->second;
	    if(!zoomedTrig->isConnected())
			zoomedTrig->setTopLeftPosition( (zoomedTrig->getX() * ratioWidth), zoomedTrig->getY() );
	    iter++ ;
	}
}

void Sequencer::componentMovedOrResized (Component& component, bool wasMoved, bool wasResized) {
	
	desactivateFacility();
	
	if(dynamic_cast<TimeBox*>(&component)) {
		TimeBox* movedBox = dynamic_cast<TimeBox*>(&component);
		
		performChildMoves(movedBox, wasMoved, wasResized);
	}
	
	/*to map e.g.*/
	sendSynchronousChangeMessage(this);/*to sequencer frame*/
	
	repaint();
}

PropertyFrame* Sequencer::getPropertyFrame() {
	
	return getMainWindow()->getPropertyFrame();
}

void Sequencer::changeListenerCallback(void * objectThatHasChanged) {
	
	repaint();
}

void Sequencer::componentChildrenChanged(Component &component) {
	
	//	if(&component!=NULL) getMainWindow()->getPropertyFrame()->refreshAll();//fait bugger le clearAllOt
}

void Sequencer::componentBroughtToFront (Component &component) {
	
	if(dynamic_cast<TimeBox*> (&component)){
		TimeBox* _ot = dynamic_cast<TimeBox*> (&component);
		std::map<unsigned int, bool>::iterator itRelRegister = _ot->getRelationMap()->begin();
		while ( itRelRegister != _ot->getRelationMap()->end() ){
			
			getRelation(itRelRegister->first)->toFront(false);
			itRelRegister++;
		}
		_ot->repaint();
	}
	
	if(Relation* _ot = dynamic_cast<Relation*> (&component)){
		
		if(_ot->getMotherId()!=0) getBox(_ot->getMotherId())->toFront(false);
		if(_ot->getDaughterId()!=0) getBox(_ot->getDaughterId())->toFront(false);
	}
	
	if(dynamic_cast<SequenceOT*> (&component)) {
		
		//selection gérée dans chaque objet
		
		getMainWindow()->getPropertyFrame()->setConcerned(&component);
		
		if(dynamic_cast<TimeBox*> (&component)) {
			
			getMainWindow()->getSafeComponentGetter()->getAutomationScope()->setConcerned( ((TimeBox*)&component)->getTimeBoxContentComponent()->getAutomationCurve(),
																						  ((TimeBox*)&component)->getName(),
																						  &component);
		}
		else /*Relation, Trigger*/ getMainWindow()->getSafeComponentGetter()->getAutomationScope()->setConcerned(0);
	}
	repaint();
}

int Sequencer::getHeadPlayPos() {
	return m_headPlay->getPixPos();
}

void Sequencer::setHeadPlayPos(unsigned int newPixPos) {
	m_headPlay->setPixPosition(newPixPos);
}

void Sequencer::resetHeadPlay() {
	
	m_headPlay->setPixPosition(0);
	m_headPlay->setTimePos(0);
}

void Sequencer::setMagnetised(bool _m) {
	
	m_magnetised = _m ;
}

void Sequencer::moveSelectionFrom(TimeBox* _movedBox) {
	
	int deltaX = _movedBox->getMovedDX();
	int deltaY = _movedBox->getMovedDY();
	
	/*on desactive temporrmt le magnetisme*/ m_magnetised = false;
	
	for(int index=0; index < m_electedItemSet->getNumSelected(); ++index) {
		
		SequenceOT* box = (SequenceOT*)(m_electedItemSet->getSelectedItem(index));
		
		if(dynamic_cast<Trigger*>(m_electedItemSet->getSelectedItem(index))) {
			
			Trigger* trig = (Trigger*)(m_electedItemSet->getSelectedItem(index));
			
			if(!trig->isConnected()) {
				const int newX = trig->getX() + deltaX;
				trig->setTopLeftPosition(newX, trig->getY());
			}
		}
		else 	if(box != _movedBox && !dynamic_cast<Relation*> (box)/*on ne gère pas les relations ici*/) {
			
			const int newX = box->getX() + deltaX;
			const int newY = box->getY() + deltaY;
			box->setTopLeftPosition(newX, newY);
		}
		
	}
	/*on reactive le magnetisme*/	m_magnetised = true;
}

bool Sequencer::isMagnetised() {
	return m_magnetised;
}

bool Sequencer::performChildMoves(TimeBox* _movedBox, bool wasMoved, bool wasResized) {
	
	unsigned int id_moved = _movedBox->getId();
	
	/*init*/
	if (id_moved != NO_ID) {
		TimeBox* movedBox = _movedBox;
		
		/**
		 * Gestion des boites deplacees car selection multiple
		 */
		if(movedBox->isMouseOverOrDragging()/*Eviter le test sur les boites asservies*/
		   && m_electedItemSet->getNumSelected() > 0)/*au moins 2 elements*/
		{
			moveSelectionFrom(_movedBox);
		}
		
		/**
		 * Gestion des boites deplacees par contraintes
		 */
		vector<unsigned int> movedBoxes;
		
		bool isMoveValid;
		if (isResizingByMouseOT && !isZooming) {
			if (wasMoved) {
//				cout << "move" << endl;
//				cout << "deb : "<< getZoomer()->convertPixToTime(movedBox->getX()) << endl;
//				cout << "fin : "<< getZoomer()->convertPixToTime(movedBox->getX()) + getEngines()->getBoxDuration(id_moved) << endl;
//				cout << "duree : "<< getZoomer()->convertPixToTime(movedBox->getX()) + getEngines()->getBoxDuration(id_moved) - getZoomer()->convertPixToTime(movedBox->getX()) << endl;
				
				isMoveValid = m_engines->performBoxEditing(id_moved, getZoomer()->convertPixToTime(movedBox->getX()),
																	 getZoomer()->convertPixToTime(movedBox->getX()) +
																		getEngines()->getBoxDuration(id_moved), &movedBoxes);
				movedBox->refresh();
			}
			if (wasResized) {
//				cout << "resize" << endl;
//				cout << "deb : "<< getEngines()->getBoxBeginTime(id_moved) << endl;
//				cout << "fin : "<< getZoomer()->convertPixToTime(movedBox->getRight()) << endl;
//				cout << "duree : "<< getZoomer()->convertPixToTime(movedBox->getRight()) - getEngines()->getBoxBeginTime(id_moved) << endl;
				
				isMoveValid = m_engines->performBoxEditing(id_moved, getEngines()->getBoxBeginTime(id_moved),	
																	 getZoomer()->convertPixToTime(movedBox->getRight()), &movedBoxes);
				movedBox->refresh();
			}
		}
		else /*if (!isResizedByMouse) */ {
//			cout << "change by prop window" << endl;
			isMoveValid = m_engines->performBoxEditing(id_moved, getEngines()->getBoxBeginTime(id_moved),
																 getEngines()->getBoxEndTime(id_moved), &movedBoxes);
			isResizingByMouseOT = true;
		}
		
		if (isMoveValid) {
			vector<unsigned int>::iterator it;
			for (it = movedBoxes.begin() ; it < movedBoxes.end() ; it++) {
				unsigned int id = *it;
				if (id != NO_ID) {
					TimeBox* constrainedBox = getBox(id);
					
					if(id != id_moved) {
						constrainedBox->setBounds(getZoomer()->convertTimeToPix(m_engines->getBoxBeginTime(id)),
												  constrainedBox->getY(),
												  getZoomer()->convertTimeToPix(m_engines->getBoxEndTime(id) - m_engines->getBoxBeginTime(id)), constrainedBox->getHeight());
					}
					
					//RESIZE DES RELATIONS - on parcourt le repertoire de relations de la boite mouvante
					map<unsigned int,bool> *boxRelations = movedBox->getRelationMap();
					map<unsigned int,bool>::iterator relIt = boxRelations->begin();
					for (relIt = boxRelations->begin() ; relIt != boxRelations->end() ; relIt++) {
						unsigned int relID = relIt->first;
						Relation* movedRelation = getRelation(relID);
						//on verifie quelle est la parente de la boite sur cette relation (mere pour connection lourde, fille pour connection legere)
						if(movedBox->isRelStart(relID)) {
							movedRelation->setAnchorsPositions(movedBox->getPlugStartX(),
															   movedBox->getPlugY(), movedBox->isMotherOfRel(relID));
						}
						else {
							movedRelation->setAnchorsPositions(movedBox->getPlugEndX(),
															   movedBox->getPlugY(), movedBox->isMotherOfRel(relID));
						}
					}
					
					//RESIZE DES TRIGGER
					map<unsigned int , bool> *triggerToMoveMap = movedBox->getTriggersMap();
					map<unsigned int , bool>::iterator itt;
					for (itt = triggerToMoveMap->begin() ; itt != triggerToMoveMap->end() ; itt++) {
						unsigned int movedTriggerId = itt->first;
						Trigger* movedTrigger = getTrigger(movedTriggerId);
						if(movedTrigger->isConnected()) {
							if(itt->second) {
								movedTrigger->setTopLeftPosition(movedTrigger->getLinkedBox()->getX() -
																 movedTrigger->getWidth()/2., movedTrigger->getY());
							}
							else {
								movedTrigger->setTopLeftPosition(movedTrigger->getLinkedBox()->getRight() -
																 movedTrigger->getWidth()/2, movedTrigger->getY());
							}
						}
					}
				}
			}
		}
	}
	
	//on reactive temporairement le magnetisme
	m_magnetised = true;
	
	//END ASK CSP
	//redondant//sendSynchronousChangeMessage(this);
	return true;
}

TimeBox* Sequencer::getBox(unsigned int ID) {
	BoxMap::iterator it;
	if ((it = _boxMap->find(ID)) != _boxMap->end()) {
		return it->second;
	}
	return NULL;
}

Relation* Sequencer::getRelation(unsigned int _id) {
	
	return _relMap->find(_id)->second;
}

Trigger* Sequencer::getTrigger(unsigned int _id) {
	return _trigMap->find(_id)->second;
}

/*********************************************************************************************************************************/

PlayController* Sequencer::getController() {
	return m_controller;
}

Engines* Sequencer::getEngines() {
	return m_engines;
}

TimeBox* Sequencer::getBoxToNetExplore() {
	
	TimeBox* firstPlanBox;
	
	if( getSelectionSet()->getNumSelected() > 0 ) {
		
		for(int i=0; i < getSelectionSet()->getNumSelected(); ++i) {
			
			Component* selectedObject = m_electedItemSet->getSelectedItem(i);
			if(getSelectionSet()->getNumSelected() != 0 &&   dynamic_cast<TimeBox*> (selectedObject) ) {
				
				firstPlanBox = (TimeBox*) selectedObject;
				return firstPlanBox;
			}
		}
	}
	
	return 0;
}

/*********************************************************************************************************************************/

void Sequencer::setMouseMode ( int _mode) {
	m_mouseMode = _mode;
	refreshCursor();
}

void Sequencer::refreshCursor() {
	switch (m_mouseMode) {
		case DRAWBOX:
			setMouseCursor( m_cursorFactory->getPenCursor()); //MouseCursor::MouseCursor(MouseCursor::NormalCursor));
			break;
		case SELECTION:
			setMouseCursor(MouseCursor::MouseCursor(MouseCursor::CrosshairCursor));
			break;
		default:
			break;
	}
}

int Sequencer::getMouseMode() {
	return m_mouseMode;
}

/*********************************************************************************************************************************/

bool Sequencer::isOneOtSelected() {
	
	bool one = (getSelectionSet()->getNumSelected() == 1);
	return one;
}

bool Sequencer::isAllOtSelected() {return _allOtSelected;}

void Sequencer::findLassoItemsInArea (Array <Component*>& results, int x, int y, int w, int h) {
	const Rectangle lasso (x, y, w, h);
	for (int i = 0; i < getNumChildComponents(); ++i)
	{
		Component* const e = dynamic_cast <Component*> (getChildComponent (i));
		if (e != 0 && e->getBounds().intersects (lasso) && (dynamic_cast<TimeBox*>(e) ||  dynamic_cast<Trigger*>(e))) {results.add (e);}
		if (e != 0 && (dynamic_cast<Relation*>(e))) {
			if( /*les 4 bords du lasso*/
			   ((Relation*)e)->getWirePath()->intersectsLine((float)x, (float)y, (float)(x+w), (float)y)  ||  ((Relation*)e)->getWirePath()->intersectsLine((float)x, (float)y, (float)x, (float)y+h) ||  ((Relation*)e)->getWirePath()->intersectsLine((float)x+w, (float)y, (float)x+w, (float)y+h)
			   ||  ((Relation*)e)->getWirePath()->intersectsLine((float)x, (float)y+h, (float)x+w, (float)y+h) ||  ((Relation*)e)->getWirePath()->intersectsLine((float)x, (float)y, (float)x+w, (float)y+h) ||  ((Relation*)e)->getWirePath()->intersectsLine((float)x, (float)y+h, (float)x+w, (float)y+h) )
			{
				results.add (e);
			}
		}
	}
	repaint();
}

SelectedItemSet <Component*>& Sequencer::getLassoSelection() {
	deselectAll();
	Array <Component*> results;
	findLassoItemsInArea(results, std::min(lasso_xi, lasso_xf), std::min(lasso_yi, lasso_yf), abs(lasso_xf-lasso_xi), abs(lasso_yf-lasso_yi));
	m_electedItemSet = new SelectedItemSet<Component*>(results);
	return *m_electedItemSet;
}

SelectedItemSet<Component*> *
Sequencer::getSelectionSet() {
	return m_electedItemSet;
}

void Sequencer::deselectAll() {
	_allOtSelected = false;
	m_electedItemSet->deselectAll();
}

void Sequencer::selectAll() {
	_allOtSelected = true;
	Array <Component*> results;
	BoxMap::iterator itb = _boxMap->begin();
	while(itb != _boxMap->end()) {
		results.add((*itb).second);
		itb++;
	}
	RelMap::iterator itr = _relMap->begin();
	while(itr != _relMap->end()) {
		results.add((*itr).second);
		itr++;
	}
	TrgMap::iterator itt = _trigMap->begin();
	while(itt != _trigMap->end()) {
		results.add((*itt).second);
		itt++;
	}
	m_electedItemSet = new SelectedItemSet<Component*>(results);
	repaint();
}

void Sequencer::clearAllOt() {
	desactivateFacility();
	deselectAll();
	
	//raz annexes
	getMainWindow()->getPropertyFrame()->setConcerned(NULL);
	getMainWindow()->getSafeComponentGetter()->getAutomationScope()->setConcerned(NULL);
	
	//clear
	getOtManager()->removeAllRelation();
	getOtManager()->removeAllBox();
	getOtManager()->removeAllTrigger();
}

BoxMap *
Sequencer::getBoxMap() {
	return _boxMap;
}

RelMap *
Sequencer::getRelMap() {
	return _relMap;
}

TrgMap *
Sequencer::getTrigMap() {
	return _trigMap;
}

SafeUpdater *
Sequencer::getSafeUpdater() {
	return m_safeUpdater;
}

void Sequencer::load(const std::string &filename) {
	m_engines->load(filename,&controlPointCrossedCallBack,&triggeredTPCallBack);
}

void Sequencer::addExecutionFinishedCallback() {
	m_engines->addExecutionFinishedCallback(&executionFinishedCallBack);
}


/*****************************************************/
/****************JUCE CALLBACKS***********************/
//Implements the callback actions to execute these in a Juce MessageQueue

class ControlPointCrossedCallBack : public CallbackMessage {
	unsigned int m_boxId;
	unsigned int m_controlPointIndex;
	
	public :
    ControlPointCrossedCallBack(unsigned int boxId, unsigned int controlPointIndex) : m_boxId(boxId), m_controlPointIndex(controlPointIndex) {
    }
	
    ~ControlPointCrossedCallBack() throw() {
    }
	
    void messageCallback() {
		
		Sequencer *seq = Sequencer::getInstance();
		if (!seq) return;
		
		seq->getSafeUpdater()->templateReceptionB(m_controlPointIndex, m_boxId);
    }
};

class TriggeredTPCallBack : public CallbackMessage {
	bool m_isWaited;
	unsigned int m_triggerId;
	
	public :
    TriggeredTPCallBack(bool isWaited, unsigned int triggerId) : m_isWaited(isWaited), m_triggerId(triggerId) {
    }
	
	~TriggeredTPCallBack() throw() {
	}
	
    void messageCallback() {
		
		Sequencer *seq = Sequencer::getInstance();
		if (!seq) return;
		
		seq->getSafeUpdater()->templateReceptionT(m_isWaited ? 1 : 0, m_triggerId);
	}
};

class ExecutionFinishedCallBack : public CallbackMessage {

	public :
    ExecutionFinishedCallBack() {
    }
	
    ~ExecutionFinishedCallBack() throw() {
    }
	
    void messageCallback() {
		
		Sequencer *seq = Sequencer::getInstance();
		if (!seq) return;
		
		seq->getSafeUpdater()->templateFinished();
		
    }
};

class TransportCommandsCallBack : public CallbackMessage {
	unsigned int m_type;
	const std::string& m_message;
	const std::string& m_arg;
	
	public :
    TransportCommandsCallBack(unsigned int type, const std::string& message, const std::string& arg) : m_type(type), m_message(message), m_arg(arg) {
    }
	
	~TransportCommandsCallBack() throw(){
	}
	
    void messageCallback() {
		
		Sequencer *seq = Sequencer::getInstance();
		if (!seq) return;
		
		TransportBar *tBar = seq->getTransportBar();
		
		switch (m_type) {
			case UNKNOWN_MESSAGE :
				if (m_message == "/Transport/Next") {
					tBar->next();
				}
				break;
			case TRIGGER_MESSAGE :
				break;
			case ENGINES_PLAY :
				tBar->play(true);
				break;
			case ENGINES_PAUSE_MODIFICATION :
				tBar->pause(true);
				break;
			case ENGINES_STOP :
				tBar->stop();
				break;
			case ENGINES_SPEED_MODIFICATION :
				tBar->setSliderValue(seq->getEngines()->getExecutionSpeedFactor());
				break;
			case ENGINES_REWIND :
				tBar->rewind();
				break;
			case ENGINES_GOTO_MODIFICATION:
				seq->setHeadPlayPos(seq->getZoomer()->convertTimeToPix(seq->getEngines()->getGotoValue()));
				break;
		}
	}
};


/*******************************************************/
/****************ENGINE CALLBACKS***********************/

void controlPointCrossedCallBack(unsigned int boxId, unsigned int controlPointIndex) {

	AUTO_RELEASE_POOL;
	CallbackMessage* message = new ControlPointCrossedCallBack(boxId, controlPointIndex);
	message->post();

}

void triggeredTPCallBack(bool isWaited, unsigned int triggerId, unsigned int boxId, unsigned int controlPointIndex, std::string triggerMessage) {
	
	AUTO_RELEASE_POOL;
	CallbackMessage* message = new TriggeredTPCallBack(isWaited, triggerId);
	message->post();

}

void executionFinishedCallBack() {
	
	AUTO_RELEASE_POOL;
	CallbackMessage* message = new ExecutionFinishedCallBack();
	message->post();

}

void transportBarCallBack(unsigned int type, std::string mes, std::string arg) {

	AUTO_RELEASE_POOL;
	CallbackMessage* message = new TransportCommandsCallBack(type, mes, arg);
	message->post();
	
}
