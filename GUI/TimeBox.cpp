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
 *  TimeBoxes.cpp
 *  Virage
 *
 *  Created by Henry Bernard and Luc Vercellin on 02/10/08.
 *  Copyright 2008 by Blue Yeti. All rights reserved.
 *
 */

#include "TimeBox.h"
#include "MainWindow.h"
#include "NetComponent.h"

#define ALPHA_BOX 0.5f

//=========================================================================================================================
TimeBox::TimeBox(Sequencer* _parentSequencer, unsigned int index, String etiquette, bool _isFlexible, int _x, int _y, int _width, int _height) :
Box (etiquette, _x, _y, _width, _height),
progressVal(0.0),
m_counter_move(0),
m_latency_move(3),
isShiftDown(false),
_lastUpdate(0)
{
	
	m_startMessagesEditorWindow = new MessageTextEditorWindow(this, 1);
	m_endMessagesEditorWindow = new MessageTextEditorWindow(this, 2);
	m_interpolationWindow = new MessageInterpolationWindow(this);
	
	m_isMuted = false;
	
	copyRange = 0;
	
	m_parentSequencer = _parentSequencer;
	m_id = index;
	
	//attributs<-arguments
	idDuplicatingWithAlt=false;
	m_dBehavior = new Behavior(this, _isFlexible);
	(m_anchorLeft = new Anchor());
	(m_anchorRight = new Anchor());

	addChildComponent(m_progressBar = new ProgressBar(progressVal));
	m_progressBar->setColour(ProgressBar::foregroundColourId, s_COLOUR_PASCAL);
	m_progressBar->setSize(_width,_height);

	//flexible properties - addAndMakeVisible-d in Sequencer.class
	m_contentComponent = new TimeBoxContentComponent(m_anchorLeft, m_anchorRight, this);
	setContentComponent(m_contentComponent);

	//Registres
	m_trigRegister = new std::map<unsigned int, bool>;
	m_motherOfRel = new std::map<unsigned int, bool>;
	
	m_first_CP_done = false;
	m_last_CP_done = false;
	m_resultOfMouseDownSelectMethod = false;
	coeff_de_copie = 0;
	
	//Relation
	m_relRegister = new map<unsigned int, bool>;
	
	m_contentComponent->setMessageStartButtonVisible((getOscMessageOutStart() != String::empty  )	);
	m_contentComponent->setMessageEndButtonVisible((getOscMessageOutStop() != String::empty  )	);
	
	m_getX_after_moved = getX();
	m_getY_after_moved = getY();
	m_getX_before_moved = 0;
	m_getY_before_moved = 0;
	
	//base statique
	
	unzoomed_y = getY();
	unzoomed_height = getHeight();
	
	listenToMeSequencer();
	
	addChangeListener(m_dBehavior);
	
	m_originName = getName();
	
	m_duration = getParentSequencer()->getZoomer()->convertPixToTime(getWidth());
	
	m_comment = String();
}

TimeBox::TimeBox(unsigned int _id, TimeBox* _timeBox) :
Box (String(""), _timeBox->getX() + s_COPY_DELTA,_timeBox->getY() + s_COPY_DELTA, _timeBox->getWidth(), _timeBox->getHeight()),progressVal (0.0),m_counter_move(0),m_latency_move(3), isShiftDown(false), _lastUpdate(0)
{
	
	m_startMessagesEditorWindow = new MessageTextEditorWindow(this, 1);
	m_endMessagesEditorWindow = new MessageTextEditorWindow(this, 2);
	m_interpolationWindow = new MessageInterpolationWindow(this);
	
	m_originName = _timeBox->m_originName;
	m_comment = _timeBox->m_comment;
	
	copyRange = _timeBox->copyRange + 1;
	setName(m_originName + String ("_(") + String(copyRange) + String(")") );/*le "_" devant le numéro de copy CAR pas d'espace dans protocole OSC ou anyone*/
	
	m_parentSequencer = _timeBox->getParentSequencer();
	m_id = _id;
	
	m_minimised = _timeBox->isMinimised();
	
	//General
	idDuplicatingWithAlt=false;
	//flexible properties - addAndMakeVisible-d in Sequencer.class
	m_dBehavior = new Behavior(this, _timeBox->getDurationBehavior()->isFlexible());
	m_anchorLeft = new Anchor();
	m_anchorRight = new Anchor();
	
	addChildComponent(m_progressBar = new ProgressBar(progressVal));
	m_progressBar->setColour(ProgressBar::foregroundColourId, s_COLOUR_PASCAL);
	m_progressBar->setSize(_timeBox->getWidth(),_timeBox->getHeight());
	
	m_contentComponent = new TimeBoxContentComponent(m_anchorLeft, m_anchorRight, this);
	setContentComponent(m_contentComponent);
	
	m_resultOfMouseDownSelectMethod = false;
	
	//Registres (!) PAS DE COPIE DES REGISTRES !!!!!!
	m_trigRegister = new std::map<unsigned int, bool>;
	m_motherOfRel = new std::map<unsigned int, bool>;
	m_relRegister = new map<unsigned int, bool>;
	
	//Propriétés
	//addChangeListener(getParentSequencer()->getMainWindow()->getPropertyFrame());
	//addChangeListener(getParentSequencer()->getMainWindow()->getNamespacePanel());

	m_first_CP_done = false;
	m_last_CP_done = false;
	
	m_minimised = _timeBox->isMinimised();
	
	//sélection multiple
	m_getX_after_moved = getX();
	m_getY_after_moved = getY();
	m_getX_before_moved = 0;
	m_getY_before_moved = 0;
	
	m_comment = _timeBox->getComment();
	
	//base statique
	unzoomed_y = getY();
	unzoomed_height = getHeight();
	
	//plus :
	setTitleColour(_timeBox->getTitleColour());
	
	coeff_de_copie = ++(_timeBox->coeff_de_copie);
	
	listenToMeSequencer();
	
	//minuit
	setOscMessageOutStart(_timeBox->getOscMessageOutStart());
	setOscMessageOutEnd(_timeBox->getOscMessageOutStop());

	updateMessagesEditor(1, _timeBox->getOscMessageOutStart());
	updateMessagesEditor(2, _timeBox->getOscMessageOutStop());

	initInterpolationWindow();
		
	setMuted(_timeBox->isMuted());
	
	addChangeListener(m_dBehavior);
	
	setInterpolated(_timeBox->m_isInterpolated);
	m_contentComponent->getAutomationCurve()->setPoints(_timeBox->getTimeBoxContentComponent()->getAutomationCurve()->getPoints());
	
	m_duration = getParentSequencer()->getZoomer()->convertPixToTime(getWidth());
	
	getParentSequencer()->getMainWindow()->getPropertyFrame()->refreshAll();
}

TimeBox::~TimeBox() {
	
	if(m_progressBar) {
		deleteAndZero( m_progressBar);
	}
	if (m_anchorLeft) {
		deleteAndZero(m_anchorLeft);
	}
	if (m_anchorRight) {
		deleteAndZero( m_anchorRight);
	}
	if (m_trigRegister) {
		deleteAndZero(m_trigRegister);
	}
	if (m_motherOfRel) {
		deleteAndZero(m_motherOfRel);
	}
	if (m_relRegister) {
		deleteAndZero(m_relRegister);
	}
	if (m_startMessagesEditorWindow) {
		deleteAndZero(m_startMessagesEditorWindow);
	}
	if (m_endMessagesEditorWindow) {
		deleteAndZero(m_endMessagesEditorWindow);
	}
	if (m_interpolationWindow) {
		deleteAndZero(m_interpolationWindow);
	}
	if (m_dBehavior) {
		deleteAndZero(m_dBehavior);
	}
}

TimeBoxContentComponent* TimeBox::getTimeBoxContentComponent() {
	return m_contentComponent;
}

float  TimeBox::getRootY() {
	return unzoomed_y;
}

float  TimeBox::getRootH() {
	return unzoomed_height;
}

void TimeBox::setVisible(bool shouldBeVisible) {
	m_dBehavior->setVisible(shouldBeVisible);
	ResizableBox::setVisible(shouldBeVisible);
}

/*MUTING*/
void TimeBox::setMuted(bool _muted) {
	
	m_isMuted = _muted;
	
	//annexed sub-mute : left , right, content :
	m_contentComponent->muteAll(_muted);//simple dessin, pas d'effet sur les boutons
	muteContent->setToggleState(_muted, true);
	
	//si setMuted pas appelé par le boutton
	m_muteButton->setToggleState(m_isMuted, false);
	m_muteButton->setColour(m_isMuted ? Colours::red : Colours::darkred);
	
	map<unsigned int, bool>::iterator it = m_trigRegister->begin();
	while(it != m_trigRegister->end()){
		
		getParentSequencer()->getTrigger(it->first)->setMuted(_muted);
		it++;
	}
	/*depuis autre que boutton mute qui lui appelle repaint*/	repaint();
	
	getParentSequencer()->getEngines()->setBoxMutingState(m_id, m_isMuted);
	
	/*5 nouvelles fonctions sont dis
	 
	 void setProcessMutingState(unsigned int boxId, bool mute);
	 bool getBoxContentMuteState(unsigned int boxId);
	 
	 void setBoxMutingState(unsigned int boxId, bool mute);
	 
	 SetBoxMuteState passe TOUTE la boite (controlPoint et contenu)
	 dans l'état mute passé en paramètre (true pour silencieux).
	 SetBoxContentMuteState passe juste le contenu (pour le moment
	 l'interpolation) en mode muté ou non (selon le paramètre).
	 setControlPointMuteState passe le controlPoint en mode muté
	 ou non (selon le paramètre).
	 
	 Il y a ensuite deux accesseurs : un pour l'état mute des
	 controlPoint, un pour l'état mute du contenu.*/
	
}

bool TimeBox::isMuted() {
	return m_isMuted ;
}

/*CALLBACK*/
void
TimeBox::controlPointDone(unsigned int controlPointIndex) {
	
	m_first_CP_done = (controlPointIndex == CPT_L);
	m_last_CP_done = (controlPointIndex == CPT_R);
	
	if(!isTimerRunning() && m_first_CP_done) {
		
		if(getParentSequencer()->getHeadPlayPos() > 0)
			m_starting_global_time = getParentSequencer()->getZoomer()->convertPixToTime(getX());
		else
			m_starting_global_time = (getParentSequencer()->getEngines()->getCurrentExecutionTime());
		
		startTimer(200);//5HZ
		
		m_waiting->setVisible(false);

		m_progressBar->setVisible(true);
	}
	else if(isTimerRunning() &&  m_last_CP_done) {
		stopTimer();
	}
	
	m_contentComponent->controlPointDone(controlPointIndex);
}

ProgressBar* TimeBox::getProgressBar() {
	
	return m_progressBar;
}

void TimeBox::timerCallback() {
	if(!(getParentSequencer()->getEngines()->isRunning())) {
		return;
	}
	
	//nouvelle version basée sur l'incrémentation du timer moteur
	if(getDurationBehavior()->isFlexible()) {
		progressVal = 1.1;
		return;
	}
	progressVal = getParentSequencer()->getEngines()->getProcessProgression(m_id);
}

unsigned int TimeBox::getId() {
	return m_id;
}

unsigned int TimeBox::getTimeInMs() {
	return getParentSequencer()->getEngines()->getBoxDuration(m_id);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//=========================================================================================================================

Anchor* TimeBox::getAnchorLeft() {
	return m_contentComponent->getAnchorLeft() ;
}

Anchor* TimeBox::getAnchorRight() {
	return m_contentComponent->getAnchorRight() ;
}

bool TimeBox::hitAnchor(int /*x*/, int /*y*/) {
	
	if(getAnchorLeft()->isMouseOverOrDragging()) return true;
	if(getAnchorRight()->isMouseOverOrDragging())	return true;
	//	if(contains(x, y)) return true;
	return false;
}

int TimeBox::getPlugStartX() {return (getX() +   getAnchorLeft()->getX() - s_WIRE_THICKNESS/2);}

int TimeBox::getPlugY() {return (getY()  + getContentComponent()->getY() +  getAnchorLeft()->getY() + s_ANCHOR_HEIGHT/2 );}

int TimeBox::getPlugEndX() {return getX() + getAnchorRight()->getRight() ;}

/*Relations*/
void TimeBox::setNewRelation(unsigned int id_rel, bool start, bool mother) {
	(*m_relRegister)[id_rel] = start;
	(*m_motherOfRel)[id_rel] = mother;
	if(start) getAnchorLeft()->setAttached(start);
	else getAnchorRight()->setAttached(!start);
	return;
}

bool TimeBox::isRelStart(unsigned int id) {return m_relRegister->find(id)->second;}

bool TimeBox::isMotherOfRel(unsigned int id) {return m_motherOfRel->find(id)->second;}

bool TimeBox::isConnectedByRel(int id) {
	return ( m_relRegister->erase(id)) ;
}

void TimeBox::deleteRelationId(int id) {
	
	if (id != NO_ID) {
		m_motherOfRel->erase(id);
		m_relRegister->erase(id);
		
		/* MAJ couleurs des ancres */
		bool somethingStart = false;
		bool somethingEnd   = false;
		map<unsigned int, bool>::iterator it = m_relRegister->begin();
		while(it != m_relRegister->end()) {
			
			somethingEnd   = ! it->second;
			somethingStart = it->second;
			it++;
		};
		getAnchorLeft()->setAttached(somethingStart);
		getAnchorRight()->setAttached(somethingEnd);
	}
}

map<unsigned int, bool>* TimeBox::getRelationMap() { 
	return m_relRegister;
}

map<unsigned int, unsigned int> TimeBox::getConnectedBoxesMap(unsigned int _ctrlPtIdFrom, unsigned int _ctrlPtIdTo) {
	map<unsigned int, unsigned int> myMap;
	
	map<unsigned int, bool>::iterator it = m_relRegister->begin();
	while (it != m_relRegister->end() ){
		
		Relation* _relation = getParentSequencer()->getRelMap()->find(it->first)->second;
		
		if(_ctrlPtIdFrom == 1/*on demande une relation sur le début*/ && it->second/*on a bien une relation connectée au début == 1*/) {
			unsigned int _connectedBoxId = 0;
			if(m_id != _relation->getDaughterId()) _connectedBoxId = _relation->getDaughterId();
			if(m_id != _relation->getMotherId())   _connectedBoxId = _relation->getMotherId();
			
			bool isAnchoredStart = getParentSequencer()->getBoxMap()->find(_connectedBoxId)->second->getRelationMap()->find(it->first)->second ;//on récupère le boolean du controlpoint de la boite voisine connectée
			if(_connectedBoxId>0) {
				if(_ctrlPtIdTo == 1 && isAnchoredStart)
					myMap[_connectedBoxId] = 1;
				else if (_ctrlPtIdTo == 2 && !isAnchoredStart)
					myMap[_connectedBoxId] = 2;
			}
		}
		else if(_ctrlPtIdFrom == 2/*on demande une relation sur la fin*/ && !(it->second)/*la relaiton en cours est sur la fin == 2*/) {
			unsigned int _connectedBoxId = 0;
			if(m_id != _relation->getDaughterId()) _connectedBoxId = _relation->getDaughterId();
			if(m_id != _relation->getMotherId())   _connectedBoxId = _relation->getMotherId();
			
			bool isAnchoredStart = getParentSequencer()->getBoxMap()->find(_connectedBoxId)->second->getRelationMap()->find(it->first)->second ;//on récupère le boolean du controlpoint de la boite voisine connectée
			if(_connectedBoxId>0) {
				if(_ctrlPtIdTo == 1 && isAnchoredStart)
					myMap[_connectedBoxId] = 1;
				else if (_ctrlPtIdTo == 2 && !isAnchoredStart)
					myMap[_connectedBoxId] = 2;
			}
		}
		
		it++;
	}
	
	return myMap;
}

/*Trigger Points*/
void TimeBox::setNewTrigger(unsigned int id_rel, bool connectedToStart) {
	
	//	setDropShadowEnabled(true);
	
	(*m_trigRegister)[id_rel] = (connectedToStart);
	
	if(connectedToStart) {
		map<unsigned int, bool>::iterator it = m_relRegister->begin();
		while(it != m_relRegister->end()){
			
			//si la relation est connecté au début de la boite de même que le trigger, elle devient flexible
			if(it->second) {
				getParentSequencer()->getRelMap()->find(it->first)->second->getDurationBehavior()->setFlexible(true);
				getParentSequencer()->getMainWindow()->getPropertyFrame()->refreshAll();
				getParentSequencer()->getRelMap()->find(it->first)->second->refresh();
			}
			it++;
		}
	}
	else {
		map<unsigned int, bool>::iterator it = m_relRegister->begin();
		getDurationBehavior()->setFlexible(true);
	}
	return;
}

void TimeBox::deleteTrigger(bool connectedToStart) {
	
	if(connectedToStart){
		map<unsigned int, bool>::iterator it = m_relRegister->begin();
		while(it != m_relRegister->end()){
			
			//si la relation est connecté au début de la boite, elle devient rigide
			if(it->second) {
				getParentSequencer()->getRelMap()->find(it->first)->second->getDurationBehavior()->setFlexible(false);
				getParentSequencer()->getMainWindow()->getPropertyFrame()->refreshAll();
				getParentSequencer()->getRelMap()->find(it->first)->second->refresh();
			}
			it++;
		}
	}
	else{
		map<unsigned int, bool>::iterator it = m_relRegister->begin();
		while(it != m_relRegister->end()){
			
			//si la relation est connecté a la fin de la boite, elle devient rigide
			if(!it->second) {
				getParentSequencer()->getRelMap()->find(it->first)->second->getDurationBehavior()->setFlexible(false);
				(getParentSequencer()->getMainWindow()->getPropertyFrame())->refreshAll();
				getParentSequencer()->getRelMap()->find(it->first)->second->refresh();
			}
			it++;
		}
		
		getDurationBehavior()->setFlexible(false);
		
	}
	
	if( getTriggersMap()->size() == 0) setDropShadowEnabled(false);
}

bool TimeBox::isTriggered(unsigned int _ctrlPtId) {
	
	map<unsigned int, bool>::iterator it = m_trigRegister->begin();
	while(it != m_trigRegister->end()){
		
		if(it->second && _ctrlPtId==1) return true;
		else if(!it->second && _ctrlPtId==2) return true;
		
		it++;
	}
	return false;
}

void TimeBox::setTitleColour(Colour _titleColour, float _alpha ) {
	Box::setTitleColour(_titleColour, _alpha = 1.0f);
	getParentSequencer()->sendSynchronousChangeMessage(getParentSequencer());
}

void TimeBox::deleteTriggerId(int id) {
	if (id != NO_ID) {
		m_trigRegister->erase(id);
	}
}

map<unsigned int, bool>* TimeBox::getTriggersMap() {
	return m_trigRegister;
}

Behavior* TimeBox::getDurationBehavior() {
	return m_dBehavior;
}

bool TimeBox::containsMessage(String _message, unsigned int _cp) {
	std::vector<std::string>* messages = new std::vector<std::string>;
	switch (_cp) {
			
		case CPT_L:
			messages = getOscMessageOutStartVector();
			for (std::vector<std::string>::iterator i = messages->begin(); i != messages->end(); i++) {
				std::string address = (*i).substr(0, (*i).find(" "));
				if ((std::string)_message == address) {
					return true;
				}
			}
			return false;
			break;
			
		case CPT_R:
			messages = getOscMessageOutStopVector();
			for (std::vector<std::string>::iterator i = messages->begin(); i != messages->end(); i++) {
				std::string address = (*i).substr(0, (*i).find(" "));
				if ((std::string)_message == address) {
					return true;
				}
			}
			return false;
			break;
			
		default:
			return false;
			break;
	}
	delete messages;
	
}

bool TimeBox::containsString(String _message, unsigned int _cp) { 
	std::vector<std::string>* messages = new std::vector<std::string>;
	switch (_cp) {
			
		case CPT_L:
			messages = getOscMessageOutStartVector();
			for (std::vector<std::string>::iterator i = messages->begin(); i != messages->end(); i++) {
				std::string address = (*i).substr(0, (*i).find(" "));
				if (address.find((std::string)_message) != std::string::npos) {
					return true;
				}
			}
			return false;
			break;
			
		case CPT_R:
			messages = getOscMessageOutStopVector();
			for (std::vector<std::string>::iterator i = messages->begin(); i != messages->end(); i++) {
				std::string address = (*i).substr(0, (*i).find(" "));
				if (address.find((std::string)_message) != std::string::npos) {
					return true;
				}
			}
			return false;
			break;
			
		default:
			return false;
			break;
	}
	delete messages;
}

void TimeBox::mouseDown(const MouseEvent &e) {

	Box::mouseDown(e);

	//selection ?
	getParentSequencer()->getSelectionSet()->addToSelectionBasedOnModifiers(this, e.mods);
	
	getParentSequencer()->repaint();
	(getParentSequencer()->getMainWindow()->getPropertyFrame())->refreshAll();
	getParentSequencer()->getMainWindow()->getNamespacePanel()->setEditedBox(this);

}

void TimeBox::mouseUp(const MouseEvent &e) {
	
	setMouseCursor(MouseCursor::MouseCursor(MouseCursor::NormalCursor));
	
	if(idDuplicatingWithAlt && e.mods.isAltDown()) {
		TimeBox* altedBox = getParentSequencer()->getOtManager()->duplicateBox(this);
		altedBox->setTopLeftPosition(e.getEventRelativeTo(getParentSequencer()).x/*position de la souris dans le parent*/ - e.getMouseDownX()/*pour tenir compte de l'offset de selection*/, e.getEventRelativeTo(getParentSequencer()).y - e.getMouseDownY());
		idDuplicatingWithAlt = false;
	}
	
	Box::mouseUp(e);
	
	if(!e.mods.isAltDown()) {
		getParentSequencer()->getSelectionSet()->addToSelectionOnMouseUp(this, e.mods, hasBeenMoved, m_resultOfMouseDownSelectMethod);
	}
	hasBeenMoved = false;
	getParentSequencer()->repaint();
	(getParentSequencer()->getMainWindow()->getPropertyFrame())->refreshAll();
}

void TimeBox::mouseDrag(const MouseEvent &e) {
	if(!e.mods.isAltDown()) {
		ResizableBox::mouseDrag(e);
	}
	else {
		idDuplicatingWithAlt = true;
		setMouseCursor(MouseCursor::MouseCursor(MouseCursor::CopyingCursor));
	}
}

void TimeBox::mouseEnter(const MouseEvent &e) {
	if (m_comment != String()) {
		m_bubbleComment->showAt(this, getComment(), 5000, true, false);
	}
}

void TimeBox::editAutomation() {
	getParentSequencer()->getMainWindow()->getSafeComponentGetter()->getAutomationScope()->setConcerned(m_contentComponent->getAutomationCurve(), getName(), this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// JUCE
void TimeBox::moved() {
	m_counter_move++;
	if(m_counter_move > m_latency_move) m_counter_move=0;
	m_getX_before_moved = m_getX_after_moved;
	m_getY_before_moved = m_getY_after_moved;
	Box::moved();
	hasBeenMoved = true;
	resized();//behavior
	sendSynchronousChangeMessage(this);
	if(m_counter_move == 0) {
		m_getX_after_moved = getX();
		m_getY_after_moved = getY();
	}
	//MAJ base statique
	unzoomed_y = (unsigned int)(1.0f * getY()/getParentSequencer()->getZoomer()->getVerticalRatio());
//	getParentSequencer()->isMoved = false;

}

int TimeBox::getMovedDX() {
	int x_delta = (m_getX_after_moved - m_getX_before_moved);
	return x_delta;
}

int TimeBox::getMovedDY() {
	int y_delta = (m_getY_after_moved - m_getY_before_moved);
	return y_delta;
}

void TimeBox::paint(Graphics &g) {
	
	//g.fillAll(Colours::white.withAlpha(0.0f));
	
	//BG
	g.setColour(Colours::lightgrey.withAlpha(ALPHA_BOX));
	g.fillRect(0,0,getWidth(),getHeight());
	
	if(m_last_CP_done) {
		g.setColour(Colours::black);
		g.setOpacity(0.2f);
		g.fillRect(0,0,getWidth(),getHeight());
	}
	else {
		g.setColour(getTitleColour());
		g.setOpacity(getTitleAlpha());
		g.fillRect(Rectangle::Rectangle(0,0,getWidth(), m_titleBarHeight));
	}
	/*poignées resizing*/
	g.setColour(Colours::black);
	int padding = 2;//padding du dessin "poignée de resize"
	int delta_entre_lignes = 4;
	int cote = 0;
	
	int lineQty = std::min(2/*max lines qty*/, (int)(getWidth()/2/4-1));
	
	for(int i=0; i<lineQty; ++i) {
		cote += delta_entre_lignes;
		g.drawLine(padding, getHeight()-cote-padding, padding+cote, getHeight()-padding, 1);
		g.drawLine(getWidth()-padding, getHeight()-cote-padding , getWidth()-(padding+cote), getHeight()-padding, 1);
	}
	
	//texte titre
	g.setColour(getTitleColour().contrasting(1.0f));
	g.setFont(Font(String("Helvetica Neue"), m_titleBarHeight, Font::plain));
	g.drawText(getName(), titleEditor->getX(), titleEditor->getY(), titleEditor->getWidth(), titleEditor->getHeight(), Justification::left, false );
	
	//selection
	if(!getParentSequencer()->getEngines()->isRunning() && getParentSequencer()->getSelectionSet()->isSelected(this)) {
		/*ON REMPLI TOUT SAUF LA BARRE DE TITRE*/
		g.setColour(s_COLOUR_SELECTION.withAlpha(0.3f));
		g.fillRect(0, m_titleBarHeight, getWidth(), getHeight());
	}
}

void TimeBox::raz() {
	m_contentComponent->init();
	stopTimer();
	m_first_CP_done = false;
	m_last_CP_done = false;
	
	m_waiting->setVisible(false);
	m_progressBar->setVisible(false);
	m_anchorLeft->setVisible(true);
	m_anchorRight->setVisible(true);
	m_bubbleComment->setVisible(false);
	
	setInterceptsMouseClicks(true, true);
	repaint();
}

void TimeBox::init() {
	raz();
	progressVal = 0.0;
	m_waiting->setVisible(true);
	m_progressBar->setVisible(true);
	m_anchorLeft->setVisible(false);
	m_anchorRight->setVisible(false);
	setInterceptsMouseClicks(false, false);
}

void TimeBox::resized() {
	Box::resized();
	
//	if(getWidth() < s_BOX_MIN_WIDTH)
//		setSize(s_BOX_MIN_WIDTH, getHeight());
	
	if(getX() < 0 )
		setTopLeftPosition(0, getY());
	if(getY() < 0 )
		setTopLeftPosition(getX(), 0);
	if(getRight() > getParentSequencer()->getWidth())
		setTopLeftPosition(getParentSequencer()->getWidth() - getWidth(), getY());
	if(getHeight() > getParentSequencer()->getHeight())
		setSize(getWidth(), getParentSequencer()->getHeight());
	
	if((unsigned int)getHeight() < m_titleBarHeight + m_footerBarHeight + m_buttonBarHeight)
		setSize(getWidth(), m_titleBarHeight + m_footerBarHeight + m_buttonBarHeight);
	
	if(m_minimised && (unsigned int)getHeight() > m_originalBoxHeight)
		m_minimised = false;
	
	if(titleEditor != NULL)
		titleEditor->setBounds(s_BOX_TITLE_LEFT, 0, getWidth()-s_BOX_TITLE_RIGHT-s_BOX_TITLE_LEFT*2, m_titleBarHeight);
	
	m_progressBar->setBounds(0, getContentComponent()->getBottom(), getWidth(), getHeight()-getContentComponent()->getBottom());
	m_waiting->setBounds(0, getContentComponent()->getBottom(), getWidth(), getHeight()-getContentComponent()->getBottom());
	
	//Behavior - OUTSIDE
	m_dBehavior->resized();
	if(!m_dBehavior->isFlexible() || m_dBehavior->getFlexStop()==0) m_dBehavior->setBounds(getX(), getY()-behavior_thickness, getWidth(), behavior_thickness);
	else m_dBehavior->setBounds(getX(), getY()-behavior_thickness, m_parentSequencer->getZoomer()->convertTimeToPix(m_dBehavior->getFlexStop()), behavior_thickness);
	
	//sequencer
	hasBeenMoved = true;
	
	//MAJ base statique
	unzoomed_height = getParentSequencer()->getZoomer()->convertPixToRootPix(getHeight());
	unzoomed_y = getParentSequencer()->getZoomer()->convertPixToRootPix(getY());
	
	int deltaBh = 5;
	if(getParentSequencer()->isMagnetised()){
		for(int i=0; i< getParentSequencer()->getHeight(); i+=S_MAGNETISM_DELTA_H) {
			if((unsigned int)getY() < (s_TIMERAIL_HEIGHT * 2 + i)/*au dessous*/  && (unsigned int)getY() >= s_TIMERAIL_HEIGHT + i - s_TIMERAIL_HEIGHT/*au dessus*/) {
				setTopLeftPosition(getX(), s_TIMERAIL_HEIGHT + i + deltaBh);
			}
		}
	}
	
	if(isShiftDown) {
		setSize(getWidth(), m_previous_height);
	}
	
	m_previous_height = getHeight();

	refresh();
}

void TimeBox::refresh() {
	(getParentSequencer()->getMainWindow()->getPropertyFrame())->refreshAll();
}

void TimeBox::modifierKeysChanged(const ModifierKeys& modifiers) {
	
	isShiftDown = (modifiers.isShiftDown());
}

bool TimeBox::keyPressed(const KeyPress & 	key) {
	
	SequenceOT::keyPressed(key);
	
	/* fleche de deplacement */
	if(getParentSequencer()->getEngines()->isRunning()) return false;
	
	if(key==KeyPress::rightKey) {
		setTopLeftPosition(getX()+1, getY());
		return true;
	}
	if(key==KeyPress::leftKey) {
		setTopLeftPosition(getX()-1, getY());
		return true;
	}
	if(key==KeyPress::upKey) {
		setTopLeftPosition(getX(), getY()-1);
		return true;
	}
	if(key==KeyPress::downKey) {
		setTopLeftPosition(getX(), getY()+1);
		return true;
	}
	if(key==KeyPress(KeyPress::rightKey, ModifierKeys::shiftModifier, 0)) {
		setTopLeftPosition(getX()+20, getY());
		return true;
	}
	if(key==KeyPress(KeyPress::leftKey, ModifierKeys::shiftModifier, 0)) {
		setTopLeftPosition(getX()-20, getY());
		return true;
	}
	if(key==KeyPress(KeyPress::upKey, ModifierKeys::shiftModifier, 0)) {
		setTopLeftPosition(getX(), getY()-20);
		return true;
	}
	if(key==KeyPress(KeyPress::downKey, ModifierKeys::shiftModifier, 0)) {
		setTopLeftPosition(getX(), getY()+20);
		return true;
	}
	if(key == KeyPress(KeyPress::returnKey) && getParentSequencer()->getSelectionSet()->isSelected(this)) {
		titleEditor->setVisible(true);
		titleEditor->setCaretVisible(true);
		titleEditor->toFront(true);
		return true;
	}
	
	if(key == KeyPress(KeyPress (T('-'), ModifierKeys::commandModifier, 0))) {
		expandButtonPressed();
		return true;
	}
	
	return false;
}

void TimeBox::listenToMeSequencer() {
	
	addChangeListener(getParentSequencer()->getMainWindow()->getPropertyFrame());
	addComponentListener(getParentSequencer());
}

void TimeBox::buttonClicked(Button* buttonThatWasClicked) {
	
	getParentSequencer()->getSelectionSet()->selectOnly(this);
	
	//if(buttonThatWasClicked == m_commentButton) m_bubbleComment->showAt(m_commentButton, getComment(), 5000, true, false);
	
	if(buttonThatWasClicked == muteContent)
	{
		getParentSequencer()->getEngines()->setProcessMutingState( m_id, muteContent->getToggleState() );
		m_contentComponent->setMutedContent( muteContent->getToggleState() );
	}
	
	
	Box::buttonClicked(buttonThatWasClicked);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Control point
void TimeBox::setOscMessageOutStart(String _message) {
	istringstream iss( (string)_message );
	vector<string> messagesToSend;
	std::string tmp;
	
	//separate the message String to send a vector containing the different messages
	while(std::getline(iss, tmp, '\n')){
		if(tmp != "") messagesToSend.push_back(tmp);
	}
	
	getParentSequencer()->getEngines()->setCtrlPointMessagesToSend(m_id, BEGIN_CONTROL_POINT_INDEX, messagesToSend);
	
	((TimeBoxContentComponent*)getContentComponent())->setMessageStartButtonVisible( (messagesToSend.size() > 0) );
	
	sendSynchronousChangeMessage(this);
}

void TimeBox::setOscMessageOutEnd(String _message) {
	istringstream iss( (string)_message );
	vector<string> messagesToSend;
	std::string tmp;
	
	while(std::getline(iss, tmp, '\n')){
		if(tmp != "") messagesToSend.push_back(tmp);
	}
	
	getParentSequencer()->getEngines()->setCtrlPointMessagesToSend(m_id, END_CONTROL_POINT_INDEX, messagesToSend);
	((TimeBoxContentComponent*)getContentComponent())->setMessageEndButtonVisible( (messagesToSend.size() > 0) );
	
	sendSynchronousChangeMessage(this);
}

String TimeBox::getOscMessageOutStart() {
	vector<string> *messages = new vector<string>;
	String s;
	getParentSequencer()->getEngines()->getCtrlPointMessagesToSend(m_id, 1, messages);
	if(!messages->empty())
	{
		for(unsigned int i = 0; i < messages->size(); i++){
			s += messages->at(i).data();
			s += "\n";
		}
	}
	if (messages) {
		deleteAndZero(messages);
	}
	return s;
}

String TimeBox::getOscMessageOutStop() {
	vector<string> * messages = new vector<string>;
	String s;
	getParentSequencer()->getEngines()->getCtrlPointMessagesToSend(m_id, 2, messages);
	if(!messages->empty())
	{
		for(unsigned int i = 0; i < messages->size(); i++){
			s += messages->at(i).data();
			s += "\n";
		}
	}
	if (messages) {
		deleteAndZero( messages);
	}
	return s;
}

vector<std::string>*
TimeBox::getOscMessageOutStartVector() {
	vector<string> *messages = new vector<string>;
	
	getParentSequencer()->getEngines()->getCtrlPointMessagesToSend(m_id, 1, messages);
	
	return messages;
}

vector<std::string>*
TimeBox::getOscMessageOutStopVector() {
	vector<string> *messages = new vector<string>;
	
	getParentSequencer()->getEngines()->getCtrlPointMessagesToSend(m_id, 2, messages);
	
	return messages;
}


/* Interpolation */

void TimeBox::sendInterpolatedMessages(vector<string> messages, vector<bool> /*redondances*/, int frequency) {
	if (messages.size() == 0) {
		return;
	}
	
	m_isInterpolated = true;
	muteContent->setVisible(true);
	setAllInterpolated(true);
	
	vector<string> result;
	vector<string>::iterator msgIt;
	for (msgIt = messages.begin(); msgIt != messages.end(); ++msgIt) {
		result.clear();
		getParentSequencer()->getEngines()->getCurveArgTypes(*msgIt, &result);
		string address = result[0];
		getParentSequencer()->getEngines()->addCurve(m_id, address);
		getParentSequencer()->getEngines()->setCurveSampleRate(m_id, address, frequency);
	}
	
	//bool shouldBeInterpolated = (messages.size() != 0);
	//setInterpolated(shouldBeInterpolated);
	
	sendSynchronousChangeMessage(this);
	getContentComponent()->resized();
	repaint();
	(getParentSequencer()->getMainWindow()->getPropertyFrame())->refreshAll();

}

vector<string> TimeBox::getInterpolatedMessages() {
	return getParentSequencer()->getEngines()->getCurvesAddress(m_id);
}

void TimeBox::setInterpolated(bool _isInterpolated) {
	
	m_isInterpolated = _isInterpolated;
	muteContent->setVisible(_isInterpolated);
	setAllInterpolated(_isInterpolated);
	
	if (!_isInterpolated) {
		getParentSequencer()->getEngines()->clearCurves(m_id);
	}
	else {
		m_interpolationWindow->activeInterpolation();
	}
	
	getContentComponent()->resized();
	
	repaint();
}

bool TimeBox::isInterpolated() {
	return m_isInterpolated;
}

void TimeBox::initInterpolationWindow() {
	vector<string>* startMessages = new vector<string>;
	vector<string>* endMessages = new vector<string>;
	vector<string> startAddresses, endAddresses, startValues, endValues, interpolableMessages;
	
	getParentSequencer()->getEngines()->getCtrlPointMessagesToSend(m_id, BEGIN_CONTROL_POINT_INDEX, startMessages);
	getParentSequencer()->getEngines()->getCtrlPointMessagesToSend(m_id, END_CONTROL_POINT_INDEX, endMessages);
	
	//only take the addresses of the messages to compare them
	for(unsigned int i = 0; i < startMessages->size(); i++){
		size_t foundSpace = startMessages->at(i).find(" ");
		if (foundSpace != string::npos && foundSpace > 0) {
			startAddresses.push_back( startMessages->at(i).substr(0,foundSpace));
		}
	}
	
	for(unsigned int i = 0; i < endMessages->size(); i++){
		size_t foundSpace = endMessages->at(i).find(" ");
		if (foundSpace != string::npos && foundSpace > 0) {
			endAddresses.push_back( endMessages->at(i).substr(0,foundSpace));
		}
	}
	
	//compare start vs end addresses to keep only the messages at once in and out
	for(unsigned int i = 0; i < startAddresses.size(); i++) {
		for(unsigned int j = 0; j < endAddresses.size(); j++) {
			if(startAddresses.at(i).compare(endAddresses.at(j)) == 0){
				interpolableMessages.push_back(startAddresses.at(i));
				startValues.push_back(startMessages->at(i).substr(startMessages->at(i).find(" ")+1, startMessages->at(i).length()));
				endValues.push_back(endMessages->at(j).substr(endMessages->at(j).find(" ")+1, endMessages->at(j).length()));
			}
		}
	}
	
	//init the interpolation choice window with interpolable messages
	m_interpolationWindow->init(interpolableMessages, startValues, endValues);
	
	if (startMessages) {
		deleteAndZero(startMessages);
	}
	if (endMessages) {
		deleteAndZero(endMessages);
	}
}

void TimeBox::showInterpolationWindow() {
	//disable messages text editor to prevent conflicts
	setMessageEditorsEnabled(false);
	
	m_interpolationWindow->setVisible(true);
}

void TimeBox::setAllInterpolated(bool _areAllInterpolated) {
	m_interpolationWindow->setAllInterpolated(_areAllInterpolated);
}


void TimeBox::titleChanged(String _oldTitle) {
	//updateTriggersMessage(_oldTitle);
	sendSynchronousChangeMessage(this);
}

void TimeBox::setTitleText(String _oldTitle, String _newTitle) {
	setName(_newTitle);
	updateTriggers(_newTitle);
	setMessagesEditorTitle(_newTitle);
	titleChanged(_oldTitle);
	repaint();
	(getParentSequencer()->getMainWindow()->getPropertyFrame())->refreshAll();
	editAutomation();
	
	titleEditor->setText(_newTitle);
}

String TimeBox::getTitle() {
	if (!getName().isEmpty()) {
		return getName();
	}
	return String();
}

//void TimeBox::updateTriggersMessage(String _boxOldTitle) {
//	std::cout << "updateTriggersMessage : " << (string)_boxOldTitle << std::endl;
//	map<unsigned int, bool>::iterator i  = m_trigRegister->begin();
//	while ( i != m_trigRegister->end() ) {
//		if (getParentSequencer()->getTrigMap()->find(i->first)->second->getMinuitMessage().startsWith("/" + _boxOldTitle))
//		{
//			String newMessage = getParentSequencer()->getTrigMap()->find(i->first)->second->getMinuitMessage().replace(_boxOldTitle, getName());
//			getParentSequencer()->getTrigMap()->find(i->first)->second->setMinuitMessage(newMessage);
//		}
//		i++;
//	}
//}

void TimeBox::updateTriggers(String _s) {
	map<unsigned int, bool>::iterator it =	getTriggersMap()->begin();
	while(it != getTriggersMap()->end() ) {
		
		if(it->second) /*connecté start*/
			getParentSequencer()->getTrigger(it->first)->setMinuitMessage("/"+_s+"/start");
		else
			getParentSequencer()->getTrigger(it->first)->setMinuitMessage("/"+_s+"/end");
		
		it++;
	}
}

void TimeBox::setComment(String _comment) {
	m_comment = _comment;
	(getParentSequencer()->getMainWindow()->getPropertyFrame())->refreshAll();
	
	getParentSequencer()->getEngines()->setBoxOptionalArgument(m_id, "comment", (std::string)_comment);
}

String TimeBox::getComment() {
	if (! m_comment.isEmpty()) {
		return m_comment;
	}
	else {
		return String();
	}
}

void TimeBox::showComment() {
	if(m_comment != String()) {
		m_bubbleComment->showAt(m_commentButton, m_comment, getParentSequencer()->getZoomer()->convertPixToTime(getWidth()), false, false);
	}
}


/*** PROPERTY WINDOWS ***/

void TimeBox::showMessagesEditor(bool _startMessagesWanted) {
	if(_startMessagesWanted){
		
		if(m_startMessagesEditorWindow->getTextEditor()->isEmpty()) updateMessagesEditor(1, getOscMessageOutStart());
		m_startMessagesEditorWindow->setVisible(!m_startMessagesEditorWindow->isVisible());
	}
	else{
		
		if(m_endMessagesEditorWindow->getTextEditor()->isEmpty()) updateMessagesEditor(2, getOscMessageOutStop());
		m_endMessagesEditorWindow->setVisible(!m_endMessagesEditorWindow->isVisible());
	}
}

void TimeBox::setMessageEditorsEnabled(bool _isEnabled) {
	m_startMessagesEditorWindow->setEnabled(_isEnabled);
	m_endMessagesEditorWindow->setEnabled(_isEnabled);
}

//Updates messages editor with minuit commands which has just been assigned and displays it
void TimeBox::updateMessagesEditor(int _controlPointIndex, vector<string> _messages) {
	String messages;
	for(unsigned int i = 0; i < _messages.size(); i++) {
		messages += (String)_messages.at(i).data() + (String)"\n";
	}
	
	if(_controlPointIndex == 1)
		m_startMessagesEditorWindow->updateOscMessages(messages);
	else
		m_endMessagesEditorWindow->updateOscMessages(messages);
}

void TimeBox::updateMessagesEditor(int _controlPointIndex, String _messages) {
	if(_controlPointIndex == 1)
		m_startMessagesEditorWindow->updateOscMessages(_messages);
	else
		m_endMessagesEditorWindow->updateOscMessages(_messages);
}

void TimeBox::setMessagesEditorTitle(String _title) {
	m_startMessagesEditorWindow->setTitle(_title);
	m_endMessagesEditorWindow->setTitle(_title);
	m_interpolationWindow->setTitle(_title);
}

void TimeBox::setMessagesEditorColour(Colour _colour) {
	m_startMessagesEditorWindow->setTitleColour(_colour);
	m_endMessagesEditorWindow->setTitleColour(_colour);
	m_interpolationWindow->setTitleColour(_colour);
}

/*TEXT EDITOR*/
void TimeBox::textEditorReturnKeyPressed (TextEditor& editor) {
	Box::textEditorReturnKeyPressed(editor);
	
	String titleWithoutSpace = editor.getText().replace(String(" "), String("_"));
	setName(titleWithoutSpace);
	updateTriggers(titleWithoutSpace);
	setTitleText(String(),titleWithoutSpace);
}

void TimeBox::textEditorFocusLost (TextEditor& editor) {
	Box::textEditorFocusLost(editor);

	String titleWithoutSpace = editor.getText().replace(String(" "), String("_"));
	setName(titleWithoutSpace);
	updateTriggers(titleWithoutSpace);
	setTitleText(String(),titleWithoutSpace);
}
