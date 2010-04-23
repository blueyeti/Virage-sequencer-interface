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
 *  AutomationScope.cpp
 *  Virage
 *
 *  Created by Henry on 02/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

/*** !!! PERCENT --> POUR MILLE POUR UNE MEILLEUR RESOL ***/

#include "AutomationScope.h"
#include "EditorFrame.h"
#include "TimeBox.h"
#include "CursorFactory.h"

AutomationScope::AutomationScope(int _id, Engines* _en, int startValue, int endValue)
:
AutomationCurve(_id, _en, startValue, endValue),
m_concerned(NULL),
m_editionTool(DOT_MAKER),

previous_x_line(0),
previous_y_line(0),

m_isMovingLine(false),
m_isMovingPoint(false),
m_concernedName(String()),

xpc_start_point_covering(-1)
{
	//m_editablePoints =  new std::map<int, EditablePoint*>;
	m_epCoveredBuffer = new std::map<int, EditablePoint*>;
	//setMouseCursor(MouseCursor::MouseCursor(MouseCursor::PointingHandCursor));
	setWantsKeyboardFocus(false);
}

AutomationScope::~AutomationScope(){

	//deleteAndZero(m_editablePoints);	//deleteAndZero(m_epCoveredBuffer);	//deleteAndZero(m_mvlepr);	//deleteAndZero(m_mvlepl);
}

void AutomationScope::paint(Graphics& g) {
	g.fillAll(Colours::lightgrey);

	if(m_concerned==NULL) return;
	AutomationCurve::paint(g);

	#ifdef VDBG
		g.setColour(Colours::red);
		g.drawText(String("info: ")+String(m_cout), 20,-5,1000,200, Justification::left, false);
		g.drawText(String("XPC start covering: ")+String(xpc_start_point_covering), 20,-55,1000,200, Justification::left, false);
		g.drawText(String("rcvr buffer: ")+String(m_epCoveredBuffer->empty()  ?"vide":"contient..." ), 20,-75,1000,200, Justification::left, false);
	#endif
}

/**
 *add a point, and a component to move this point, and update component register
 */
void AutomationScope::addAPoint(int xClick, int yClick) {

	if(m_concerned == NULL) {
		return;
	}

	/**BEGIN - conversion en valeur relative*/
	int xpc = convertXToRelative(xClick);
	int ypc = convertYToRelative(yClick);
	/**END - conversion en valeur relative*/

	addAPCPoint(xpc, ypc);
}


void AutomationScope::addAPCPoint(int xpc, int ypc, bool sendUpdateMessage) {

	if(m_concerned == NULL) return;

	/**BEING - replacement of points if doubled bloom*/
	delAPoint(xpc, ypc);
	/**END - replacement */

//	std::cout << "AutomationScope::addAPCPoint : adding point [" << xpc << "," << ypc << "]" << std::endl;
	points[xpc]=ypc;

	EditablePoint* ep;
	addAndMakeVisible(ep =  new EditablePoint(this, xpc, ypc));
	ep->setBounds(convertXToAbsolute(xpc)-POINT_SIZE/2, convertYToAbsolute(ypc)-POINT_SIZE/2, POINT_SIZE, POINT_SIZE);
	m_editablePoints->insert(pair<int,EditablePoint*>(xpc,ep) );

	if(sendUpdateMessage) {
		updated();
	}
	repaint();//repaint this
	refreshConcerned();//repaint concerned automation box
}

/**
 *del a point, the component, and update component register
 *
 *this method is called by the change listener when mouse up or del-key on a component
 *this is not directly called in the Automationscope pure class
 */
void AutomationScope::delAPoint(int xpc, int /*ypc*/, bool sendUpdateMessage) {

	EditablePoint* epToDel = m_editablePoints->find(xpc)->second;

	if(epToDel == NULL) return;

	epToDel->setVisible(false);//n'a de sens que dans le cas des lignes... où on ne déplace pas un points editable, mais on supprime_old -> crée_new
	m_editablePoints->erase(xpc);
	points.erase(xpc);


	if(sendUpdateMessage)
		updated();
	repaint();
	refreshConcerned();
}

/***************************** /BEGIN/ MOUSE MANAGEMENT ***********************************/
void AutomationScope::mouseDown(const MouseEvent& e) {

	/*click droit*/
	if(e.mods.isPopupMenu()){

		//vector<string> addresses = m_engines->getCurvesAddress(m_id_relative);

		PopupMenu m;
		m.addItem(1, "Reset Automation");
		//m.addItem(2, "Copy (inactive)",false);

//		PopupMenu subMenu;
//		vector<string>::iterator addrIt;
//		unsigned int subMenuItemID = 2;
//		for (addrIt = addresses.begin(); addrIt != addresses.end() ; ++addrIt) {
//			subMenu.addItem(subMenuItemID,String((*addrIt).c_str()));
//			subMenuItemID++;
//		}
//
//		m.addSubMenu("Update extrema",subMenu,true,NULL,true);

		const int result = m.show();

		if (result == 1){
			/*BEGIN delete all points*/
			reset();
		}
		else if (result >= 2) {
//			cout << endl << "UPDATING EXTREMA ..." << addresses.size() << endl;

//			if (! addresses.empty()) {
//				string address = addresses.at(result - 2);
//				std::vector<float> values;
//
//				if(m_engines->getCurveValues(m_id_relative, address, 0, &values)) {
//					setStartValue(values.front());
//					setEndValue(values.back());
//					m_currentAddress = String(address.c_str());
//					setExtInfo(m_concernedName,NULL);
//				}
//			}
		}
	}
	refreshConcerned();
}
void AutomationScope::reset(){
			points.clear();
	points[0] = AutomationCurve::DEFAULT_START_VALUE;
	points[s_SCALE_LENGTH] = AutomationCurve::DEFAULT_END_VALUE;
	//m_editablePoints->clear();
	deleteAllChildren();
	updated();
	/*END delete all points*/
	repaint();
}


void AutomationScope::clear()
{
	points.clear();
	//m_editablePoints->clear();
	deleteAllChildren();
	/*END delete all points*/
	repaint();
}

void AutomationScope::mouseDrag(const MouseEvent& e)
{
//	if(m_editionTool == PEN) {
//		//addAPoint(e.x, e.y);
//		return;
//	}
//
///** BEGIN - déplacement d'une ligne **/
//	if(! m_isMovingLine) {
//
//		if(previous_x_line==0)
//			previous_x_line = e.getMouseDownX();
//		if(previous_y_line==0)
//			previous_y_line = e.getMouseDownY();
//	}
//
//	moveALine(e.x, e.y,
//			  e.x - previous_x_line, e.y - previous_y_line,
//			  e.mods.isShiftDown() );
//
//	/*MAJ REF*/
//	previous_x_line = e.x;
//	previous_y_line = e.y;
//	m_isMovingLine=true;
//
///** END - déplacement d'une ligne */
}


void AutomationScope::moveALine(int xClick, int yClick, int deltaX, int deltaY, bool pure_vertical){

	/** BEGIN DETERMINATION DES POINTS GAUCHE-DROITE AU DEPART SEULEMENT */
	/* on ne le fait qu'une fois !!! sinon glissements*/
	if(! m_isMovingLine) {
		/********détermination du point de droite*/
		int min = 320000/*MAX*/;
		map<int, EditablePoint*>::iterator it = m_editablePoints->begin();
		while( it != m_editablePoints->end() ){

			if(/*xClick > it->second->getX() &&*/ abs(xClick - it->second->getX()) < min   && (xClick < it->second->getX()) ){

				m_mvlepr = it->second ;
				min = (xClick - it->second->getX()) ;
			}
			++it;
		}

		/******détermination du point de gauche*/
		min = 32000;/*MAX*/
		it = m_editablePoints->begin();
		while( it != m_editablePoints->end() ){

			if(/*xClick > it->second->getX() &&*/ abs(xClick - it->second->getX()) < min && (it->second != m_mvlepr)   && (xClick > it->second->getX())  ){

				m_mvlepl = it->second;
				min = (xClick - it->second->getX()) ;
			}
			++it;
		}
	}
	/** END DETERMINATION DES PTS GAUCHE DROITE AU DEPART SEULEMENT */

	/*check...*/
	if(m_mvlepr == NULL) return;
	if(yClick>=getHeight() || yClick<=0 || (m_mvlepr->getY()<=0 && deltaY<0) || (m_mvlepr->getY()>=getHeight()-POINT_SIZE/2 && deltaY>0)) return;//si limites haut bas atteintes
	if(m_mvlepr->isAnExtrema()) deltaX = 0; //si points extrem, mouvement sur Y uniquement

	/*check...*/
	if(m_mvlepl == NULL) return;
	if(yClick>=getHeight() || yClick<=0 || (m_mvlepl->getY()<=0 && deltaY<0) || (m_mvlepl->getY()>=getHeight()-POINT_SIZE/2 && deltaY>0)) return;//on stoppe si on atteint les bords
	if(m_mvlepl->isAnExtrema())  deltaX = 0; //si points extrem, mouvement sur Y uniquement

	/** BEGIN - est on bien sur la ligne ?*/ /* A NE VERIFIER QU A MOUSE DOWN !!! */
	/*
	 if(! m_isMovingLine){
		Path linePath;
		float tolerence = 9.0f;
		linePath.addLineSegment(m_mvlepl->getX(), m_mvlepl->getY(), m_mvlepr->getX(), m_mvlepr->getY(), LINE_THICKNESS);
		if(! linePath.contains(e.x, e.y, tolerence)) return;
	 }
	 */
	/** END - est on bien sur la ligne ? */

	/*donc...*/
	m_mvlepr->setTopLeftPosition(pure_vertical ? m_mvlepr->getX() : m_mvlepr->getX() + deltaX, m_mvlepr->getY() + deltaY);
	m_mvlepl->setTopLeftPosition(pure_vertical ? m_mvlepl->getX() : m_mvlepl->getX() + deltaX, m_mvlepl->getY() + deltaY);
	editablePointHasMoved(m_mvlepr);
	editablePointHasMoved(m_mvlepl);

	/*callback*/
	lineHasMoved();//covering
}

void AutomationScope::mouseUp(const MouseEvent& e) {

		previous_x_line = 0;
		previous_y_line = 0;
		m_mvlepr = NULL;
		m_mvlepl = NULL;
		m_isMovingLine = false;
		m_isMovingPoint = false;
		if(m_mvlepl != NULL) m_mvlepl->setIsMoving(false);
		if(m_mvlepr !=NULL) m_mvlepr-> setIsMoving(false);

		if(! e.mouseWasClicked() || e.mods.isPopupMenu() ) {
			return;
		}
		else {
//			addAPoint(e.x, e.y);
		}
}
/***************************** /END/ MOUSE MANAGEMENT ***********************************/



/***************************** /BEGIN/ CONCERNED MANAGEMENT ***********************************/
void AutomationScope::setConcerned(AutomationCurve* _concerned, String _concernedName, Component* _OTConcerned) {


	if(_concerned==NULL) {

		setExtInfo(String("..."), NULL);
//		((EditorFrame*)m_parent)->setScopeTitleColour(Colours::white);
		clear();
		m_concerned = NULL;
		m_id_relative = 0;
		return;
	}

	m_concerned = _concerned;
	//m_concerned->getTimeBoxContentContener()->getTimeBoxParent()->addChangeListener(this);
	TimeBox* box = NULL;

	setExtInfo(_concernedName, _OTConcerned);
	m_concernedName = _concernedName;
	/*about parent editor*/
	box = ((TimeBox*)(_concerned->getParentComponent()->getParentComponent()));
	//	((EditorFrame*)m_parent)->setScopeTitleColour(box->getTitleColour());
	m_id_relative = box->getId();
	m_engines = box->getParentSequencer()->getEngines();

	/*prelimin*/
	//m_editablePoints->clear();
	deleteAllChildren();

	/*points*/
	setPoints(_concerned->getPoints());
//	map<int, int>::iterator it = points.begin();
//	while(it != points.end()){
//
//		EditablePoint* ep = new EditablePoint(this, it->first, it->second);
//		m_editablePoints->insert(pair<int, EditablePoint*>(it->first, ep));
//		addAndMakeVisible(m_editablePoints->find(it->first)->second);
//		m_editablePoints->find(it->first)->second->setTopLeftPosition( convertXToAbsolute(it->first)-POINT_SIZE/2, convertYToAbsolute(it->second)-POINT_SIZE/2);
//		it++;
//	}
	/*end*/

	repaint();
}

AutomationCurve* AutomationScope::getConcerned(){
	return m_concerned;
}

void AutomationScope::removeConcerned(){

	m_concerned=NULL;
	m_currentAddress = String();
	m_concernedName = String();
	setExtInfo(m_concernedName,NULL);
	((EditorFrame*)getParentComponent())->setScopeTitleName(String::empty);
	repaint();
}

void AutomationScope::setEditionTool(E_AutomationToolMode _newTool){
//
//	m_editionTool = _newTool;
//
//	CursorFactory CF;
//	if(m_editionTool == PEN)		setMouseCursor((CF.getPenCursor()));
//	if(m_editionTool == DOT_MAKER)	setMouseCursor(MouseCursor::MouseCursor(MouseCursor::PointingHandCursor));
}

void AutomationScope::refreshConcerned(){
	if(m_concerned == NULL)
		return;
	m_concerned->setPoints(points);//rpt-included
}

void AutomationScope::setExtInfo(String _concernedName, Component* /*_OTConcerned*/){
//	if (! m_currentAddress.isEmpty()) {
//		((EditorFrame*)getParentComponent())->setScopeTitleName(_concernedName + " : " + m_currentAddress);
//	}
//	else {
		((EditorFrame*)getParentComponent())->setScopeTitleName(_concernedName);
//	}
}

/***************************** /END/ CONCERNED MANAGEMENT ***********************************/




/***************************** /BEGIN/ CHANGES MANAGEMENT ***********************************/
void AutomationScope::changeListenerCallback(void* objectThatHasChanged){

	if(m_concerned == NULL)
		return;
	Component* e = (Component*)objectThatHasChanged;

	/*** Ecouter les mouvements des points et des lignes, les appliquer au m_concerned ***/
	if(! dynamic_cast<EditablePoint*> (e)) return;

	EditablePoint* ep = (EditablePoint*)objectThatHasChanged;


	/*delete EP*/
	if(ep->askDeletetion() && (! ep->isAnExtrema()) ) {

		delAPoint(ep->getXPC(), ep->getYPC());
	}
	/*BEGIN - an editable points has finished to move */
	else {
		editablePointHasMoved(ep);
	}
	/*END - an editable points has moved */


	repaint();//repaint this
	refreshConcerned();//repaint concerned automation box
}
/***************************** /END/ CHANGES MANAGEMENT ***********************************/

void AutomationScope::editablePointHasMoved(EditablePoint* _epRef){

	/*conversion de la nvelle position en valeur relative*/
	int xpc = convertXToRelative(_epRef->getX() + POINT_SIZE/2);
	int ypc = convertYToRelative(_epRef->getY() + POINT_SIZE/2);

	/**pour les lignes exclusibvement*/
	delAPoint(_epRef->getXPC() , _epRef->getYPC(), false );

	/** create nu - after deplacement - replacement & deletion managed in addPoint*/
	//addAPCPoint(xpc, ypc, false);

	/*update EditablePoint relative coordinate*/
    _epRef->setXYPC(xpc, ypc);

	/** if covering */
	holdAPoint(_epRef);


	if(! _epRef->isMoving())
	updated( );

	repaint();
	refreshConcerned();
}


void AutomationScope::holdAPoint(EditablePoint* _epRef){ //BOTH delete passed point, and hold in a buffer

	/* MAJ xpc_start_point_covering ~ON/OFF */
	if(! m_isMovingLine) {
		if(_epRef->isMoving() && xpc_start_point_covering < 0/*means undefined*/)	xpc_start_point_covering = _epRef->getXPC();
		else if(!_epRef->isMoving() && xpc_start_point_covering >= 0/*means undefined*/)	xpc_start_point_covering = -1;
	}


	/**stock covered*/
	/*MAJ*/
	/**on supprime les points de m_editablePoints et on les stocke dans m_epCoveredBuffer*/
	if( !m_isMovingLine && (! _epRef->isMoving() || xpc_start_point_covering == -1) ) {

		/* on supprime du réel, les points du buffer qui ont été abandonné*/
		map<int, EditablePoint*>::iterator it = m_epCoveredBuffer->begin();
		while(it != m_epCoveredBuffer->end()){

			if(m_editablePoints->find(it->first)->second != NULL)   m_editablePoints->erase(m_editablePoints->find(it->first));
			it++;
		}
		m_epCoveredBuffer->clear();
		return;
	}

	int xpc = _epRef->getXPC();

	map<int, EditablePoint*>::iterator it = m_editablePoints->begin();
	while(it != m_editablePoints->end()) {

		EditablePoint* epScanned = it->second;
		int xpc_scanned = epScanned->getXPC();

		if(  epScanned->getXPC() != xpc /* previously : epScanned != _epRef, mais mieux car évite la redondance avec delAPoint() */) {
			/*stock covered*/
			if(   xpc_scanned <= std::max(xpc, xpc_start_point_covering) && xpc_scanned >= std::min(xpc, xpc_start_point_covering) ) //si on est  entre le point qui bouge et le point de départ (x_start_covering)
			{
				/*on stocke dans le buffer*/
				m_epCoveredBuffer->insert ( pair<int,EditablePoint*>(xpc_scanned,epScanned) );

				/*on supprime dans le réel*/
				//comme delAPoint mais sans la suppresion de m_editablePoint.map
				(m_editablePoints->find(xpc_scanned)->second)->setVisible(false);
				points.erase(xpc_scanned);
			}
			/*recover*/
			/*sinon si le point est ailleurs, on regarde s'il ne s'agit pas d'un point à rétablir...*/
			else if(m_epCoveredBuffer->find(xpc_scanned)->second != NULL) {

				/*... auquel cas, on ajoute le point dans réel*/
				//comme delAPoint mais sans l'insert dans m_editablePoint.map
				m_editablePoints->find(xpc_scanned)->second->setVisible(true);
				points[xpc_scanned]=m_editablePoints->find(xpc_scanned)->second->getYPC() ;

				/*supprime du buffer*/
				m_epCoveredBuffer->erase(m_epCoveredBuffer->find(epScanned->getXPC()));
			}
		}
		it++;
	}


	repaint();
	refreshConcerned();
}




void AutomationScope::lineHasMoved(){

	int xL = m_mvlepl->getXPC();
	int xR = m_mvlepr->getXPC();

	/**stock covered*/
	map<int, EditablePoint*>::iterator it = m_editablePoints->begin();
	while(it != m_editablePoints->end()) {

		EditablePoint* epScanned = it->second;
		int xpc_scanned = epScanned->getXPC();

		if(  xpc_scanned != xL && xpc_scanned != xR) {

			/*stock covered*/
			if( xpc_scanned > xL && xpc_scanned < xR ) //si on est  entre le point qui bouge et le point de départ (x_start_covering)
			{
				/*on stocke dans le buffer*/
				m_epCoveredBuffer->insert ( pair<int,EditablePoint*>(xpc_scanned,epScanned) );

				/*on supprime dans le réel*/
				(m_editablePoints->find(xpc_scanned)->second)->setVisible(false);
				points.erase(xpc_scanned);
			}
			/*recover*/
			/*sinon si le point est ailleurs, on regarde s'il ne s'agit pas d'un point à rétablir...*/
			else if(m_epCoveredBuffer->find(xpc_scanned)->second != NULL) {

				/*... auquel cas, on ajoute le point dans réel*/
				//comme delAPoint mais sans l'insert dans m_editablePoint.map
				m_editablePoints->find(xpc_scanned)->second->setVisible(true);
				points[xpc_scanned]=m_editablePoints->find(xpc_scanned)->second->getYPC() ;

				/*supprime du buffer*/
				m_epCoveredBuffer->erase(m_epCoveredBuffer->find(epScanned->getXPC()));
			}
		}
		it++;
	}

	repaint();
	refreshConcerned();
}

int AutomationScope::convertXToRelative(int _xa) {

	int xpc = (s_SCALE_LENGTH *1.0f * _xa / getWidth());
	if(xpc < 0) xpc = 0;
	if(xpc > s_SCALE_LENGTH) xpc = s_SCALE_LENGTH;
	return xpc;
}

int AutomationScope::convertYToRelative(int _ya) {

	int ypc = (s_SCALE_LENGTH *1.0f * _ya / getHeight());
	if(ypc < 0) ypc = 0;
	if(ypc > s_SCALE_LENGTH) ypc = s_SCALE_LENGTH;
	return ypc;
}

int AutomationScope::convertXToAbsolute(int _xr) {

	int xa = (getWidth() *1.0f * _xr / s_SCALE_LENGTH);
	return xa;
}

int AutomationScope::convertYToAbsolute(int _yr) {

	int ya = (getHeight() *1.0f * _yr / s_SCALE_LENGTH);
	return ya;
}


void AutomationScope::refreshEditablePointsPositions(){

//	map<int, EditablePoint*>::iterator it = m_editablePoints->begin();
//	EditablePoint* ep;
//	while ( it != m_editablePoints->end() ) {
//
//		ep = it->second;
//		ep->setTopLeftPosition(convertXToAbsolute(ep->getXPC())-POINT_SIZE/2, convertYToAbsolute(ep->getYPC())-POINT_SIZE/2);
//
//		it++;
//	}
}

void AutomationScope::resized(){

	AutomationCurve::resized();
	refreshEditablePointsPositions();
}
