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
#include "Relation.h"
#include "Sequencer.h"
#include "MainWindow.h"

/*
 *  Created by Henry Bernard and Luc Vercellin on 02/10/08.
 *  Copyright 2008 by Blue Yeti. All rights reserved.
 */

//===================================================================================================================================================
/*!
 * \class Relation
 * Relation.class creates and represents a time-logical relation between 2 boxes,
 * the current implementation is set mainly for graphic user interface,
 * to tell the engine to specify a logic time-relation and symbols graphically.
 * There is no -by the current step- any dynamic activity inside.
 */

#define COIN 5
#define THICKNESS 2
#define HIT_TOLERANCE 3

Relation::Relation(Sequencer* _parentSequencer, unsigned int _id, int _xi, int _yi, int _xo, int _yo, int _MotherId = -1, int _DaughterId = -2, bool isFlexible) :
SequenceOT(_parentSequencer, _id),
m_isToFront(false),
previousY(0),
m_isResized(false),
m_askDeletion(false)
{

	setSize(0,0);

	//duration behavior
	m_dBehavior = new Behavior(this, isFlexible);
	m_dBehavior->setColour(s_COLOUR_UNSELECTION);

	setAnchorsPositions(_xi,_yi,_xo,_yo);

	m_MotherId = _MotherId;
	m_DaughterId = _DaughterId;

	setWantsKeyboardFocus(true);
	setAlwaysOnTop(true);
	listenToMeSequencer();
	
	m_colour = Colours::black;
}

Relation::~Relation(){

	deleteAllChildren();
}


void Relation::listenToMeSequencer(){

	addChangeListener(getParentSequencer()->getPropertyFrame());
	addComponentListener(getParentSequencer());
}

void Relation::paint(Graphics& g) {

	if (getParentSequencer()->getSelectionSet()->isSelected(this)) g.setColour (s_COLOUR_UNSELECTION.withMultipliedBrightness(isMouseOver() ? 10.f : 1.0f));
	else g.setColour (isMouseOver() ? Colours::darkgrey :  Colours::black);
	
	if (getParentSequencer()->getEngines()->isRunning())
		g.setColour(m_colour);
	
	g.fillPath (wirePath);

	//afficher duree
	if(/*m_isResized && */!getDurationBehavior()->isFlexible() && abs(xf-xi) > 50) {
		/*rect*/
		//g.drawRoundedRectangle((xi+xf)/2 - 30,yf-15,60,15,3/*an*/, 1);
		
		/*val+s*/
		//String timeString(/*1.0f*/getParentSequencer()->getZoomer()->convertPixToTime(abs(xi-xf)) / 1000.);
		String timeString(getDuration() / 1000.);
		
		if (timeString.length() >= 4) {
			g.drawText(timeString.substring(0,4)/* + String("s")*/, (xi+xf)/2 - 20, yf-11, 40, 10, Justification::left, false);
		}
		else {
			g.drawText(timeString.substring(0)/* + String("s")*/, (xi+xf)/2 - 20, yf-11, 40, 10, Justification::left, false);
		}
	}
}

void Relation::refresh() {
	setZone(xi, yi, xf, yf);
}

void Relation::setZone(int _xi, int _yi, int _xf, int _yf) {

	if(_xi>=0) xi =_xi;
	if(_yi>=0) yi = _yi;
	if(_xf>=0) xf = _xf;
	if(_yf>=0) yf = _yf;
	int signe = signof(xf-xi);
	int signeY = signof(yf-yi);

	int newX = xi+relThickness/2;

	wirePath.clear();
	/*verticale*/
	wirePath.addLineSegment(newX, yi, newX, yf-signeY*COIN, relThickness);


	if(getDurationBehavior()->isFlexible()) {
		/*minimum rigide*/
		if(getDurationBehavior()->getFlexStart() != NO_START) wirePath.addLineSegment(newX+signe*COIN, yf, newX + signe*( m_parentSequencer->getZoomer()->convertTimeToPix(getDurationBehavior()->getFlexStart()) + signe*COIN), yf, relThickness);

		/*zone flexible*/
		int flexStart = newX + signe* (m_parentSequencer->getZoomer()->convertTimeToPix( getDurationBehavior()->getFlexStart() ) );
		int flexStop = newX + signe * (m_parentSequencer->getZoomer()->convertTimeToPix( getDurationBehavior()->getFlexStop() ) );

		if((getDurationBehavior()->getFlexStart() + getDurationBehavior()->getFlexStop()) == NO_STOP) {
			for(int i = std::min(xi+signe*COIN,xf); i< std::max(xi+signe*COIN,xf); i+= behaviourInfoThickness*2) {
				wirePath.addEllipse(i, yf-2, relThickness, relThickness);
			}
		}
		else {
			for(int i = std::min(flexStart, flexStop) ; i < std::max(flexStart, flexStop) ; i += behaviourInfoThickness*2) {
					wirePath.addEllipse(i, yf-2, relThickness, relThickness);
			}
		}

		//crochet flexibilit
		if(m_parentSequencer->getZoomer()->convertTimeToPix(getDurationBehavior()->getFlexStop() ) != NO_STOP){
			//petite verticale gauche
			wirePath.addLineSegment(flexStart, yf,
									flexStart, yf+behaviourInfoThickness,
									relThickness);
			//petite verticale droite
			wirePath.addLineSegment(flexStop, yf,
									flexStop, yf+behaviourInfoThickness,
									relThickness);
			//horizontale
			wirePath.addLineSegment(flexStart, yf+behaviourInfoThickness,
									flexStop,  yf+behaviourInfoThickness,
									relThickness);
		}


	}
	else {
		wirePath.addLineSegment(newX + signe*COIN, yf, xf + relThickness/2, yf, relThickness);
	}

	//coins arrondis
	wirePath.addLineSegment(xi, yf-signeY*(COIN+relThickness/2), xi+signe*(COIN+relThickness/2), yf, relThickness);

	//boules, remplacs par des losanges
	wirePath.addEllipse(xf-s_ANCHOR_HEIGHT/3, yf-s_ANCHOR_HEIGHT/3,s_ANCHOR_HEIGHT/3*2,s_ANCHOR_HEIGHT/3*2);
	wirePath.addEllipse(newX-s_ANCHOR_HEIGHT/3, yi-s_ANCHOR_HEIGHT/3,s_ANCHOR_HEIGHT/3*2,s_ANCHOR_HEIGHT/3*2);

	sendSynchronousChangeMessage(this);
	m_isResized = true;
	repaint();
}

Colour Relation::getColour() {

	return s_COLOUR_SELECTION;
}

bool Relation::hitTest (int x, int y)
{
	int collisionDetectionTolerence = HIT_TOLERANCE;

	if( wirePath.contains((float)x, (float)y)  ) return true;

	/*marge de facilit*/
	for(int i=-collisionDetectionTolerence; i < collisionDetectionTolerence; i++) {

		for(int j=-collisionDetectionTolerence; j<collisionDetectionTolerence; j++) {

			if( wirePath.contains((float)(x+i), (float)(y+j))) return true;
		}
	}
	return false;
}

Path* Relation::getWirePath(){
	return &wirePath;
}

void Relation::setAnchorsPositions(int _x, int _y, int _xx, int _yy){

	if(_x>=0) xi = std::max(0,_x );
	if(_y>=0) yi = std::max(0,_y );
	if(_xx>=0) xf = std::max(0,_xx);
	if(_yy>=0) yf = std::max(0,_yy);

	setZone(xi, yi, xf, yf);

	resized();
}
void Relation::setAnchorsPositions(int _x, int _y, bool _motherBox){

	if(_motherBox) {
		if(_x>=0) xi = std::max(0,_x );
		if(_y>=0) yi = std::max(0,_y );
	}
	else {
		if(_x>=0) xf = std::max(0,_x);
		if(_y>=0) yf = std::max(0,_y);
	}
	setZone(xi, yi, xf, yf);

	resized();
}

void Relation::resized(){

		if(m_dBehavior->isFlexible() && m_parentSequencer->getZoomer()->convertTimeToPix( m_dBehavior->getFlexStop())!=NO_STOP)
			m_dBehavior->setBounds(getX(), yf-behavior_thickness, m_parentSequencer->getZoomer()->convertTimeToPix(m_dBehavior->getFlexStop()), behavior_thickness);
		else
			m_dBehavior->setBounds(getX(), yf-behavior_thickness, getWidth(), behavior_thickness);

	setZone(xi, yi, xf, yf);
	repaint();
	(getParentSequencer()->getMainWindow()->getPropertyFrame())->refreshAll();
}

bool Relation::isTriggered(unsigned int _ctrlPtId){

	if(getParentSequencer()->getBoxMap()->find( getDaughterId() )->second->isTriggered(_ctrlPtId))
		return true;
	else
		return false;

}
/******************************************************************************/
//Relation Components

Behavior* Relation::getDurationBehavior(){
	return m_dBehavior;
};

void Relation::focusLost(FocusChangeType /*cause*/) {

	getParentSequencer()->repaint();
	m_isToFront = false;
}

void Relation::mouseDown(const MouseEvent &e) {

	getParentSequencer()->getSelectionSet()->addToSelectionBasedOnModifiers(this, e.mods );
	m_isResized = false;
	repaint();

	getParentSequencer()->mouseDown(e.getEventRelativeTo(getParentSequencer()));
	toFront(true);
}

void Relation::mouseUp(const MouseEvent &e) {
	previousY = 0;
	getParentSequencer()->mouseUp(e.getEventRelativeTo(getParentSequencer()));
}

void Relation::mouseMove(const MouseEvent &e) {

	getParentSequencer()->mouseMove(e.getEventRelativeTo(getParentSequencer()));
	m_isResized = false;
	repaint();
}

void Relation::mouseExit(const MouseEvent &/*e*/) {

	m_isResized = false;
	repaint();
}

void Relation::mouseDrag(const MouseEvent &e) {
	
	getParentSequencer()->mouseDrag(e.getEventRelativeTo(getParentSequencer()));

}



int Relation:: getBeginValue(){


	int b1 = getParentSequencer()->getBox(getMotherId())->isRelStart(m_id) ? getParentSequencer()->getEngines()->getBoxBeginTime(getMotherId()) :  getParentSequencer()->getEngines()->getBoxEndTime(getMotherId());
	int b2 = getParentSequencer()->getBox(getDaughterId())->isRelStart(m_id) ? getParentSequencer()->getEngines()->getBoxBeginTime(getDaughterId()) :  getParentSequencer()->getEngines()->getBoxEndTime(getDaughterId());

	return std::min(b1,b2);//ms

}

int Relation:: getEndValue(){


	int b1 = getParentSequencer()->getBox(getMotherId())->isRelStart(m_id) ? getParentSequencer()->getEngines()->getBoxBeginTime(getMotherId()) :  getParentSequencer()->getEngines()->getBoxEndTime(getMotherId());
	int b2 = getParentSequencer()->getBox(getDaughterId())->isRelStart(m_id) ? getParentSequencer()->getEngines()->getBoxBeginTime(getDaughterId()) :  getParentSequencer()->getEngines()->getBoxEndTime(getDaughterId());

	return std::max(b1,b2);//ms

}

int  Relation::getDuration(){

	int b1 = getParentSequencer()->getBox(getMotherId())->isRelStart(getId()) ? getParentSequencer()->getEngines()->getBoxBeginTime(getMotherId()) :  getParentSequencer()->getEngines()->getBoxEndTime(getMotherId());
	int b2 = getParentSequencer()->getBox(getDaughterId())->isRelStart(getId()) ? getParentSequencer()->getEngines()->getBoxBeginTime(getDaughterId()) :  getParentSequencer()->getEngines()->getBoxEndTime(getDaughterId());

	return abs(b1-b2);//ms
}

unsigned int Relation::getMotherId(){

	return m_MotherId;
}

unsigned int Relation::getDaughterId(){

	return m_DaughterId;
}

void Relation::setMotherId(unsigned int _MotherId){

	m_MotherId=_MotherId;
}

void Relation::setDaughterId(unsigned int _DaughterId){

	m_DaughterId=_DaughterId;
}


/******************************************************************************/
//Graphical datas

int Relation::getThickness(){

	return s_WIRE_THICKNESS;
}

void Relation::setOtColour(Colour _newColour) {
	m_dBehavior->setColour(_newColour);
	m_colour = _newColour;
	repaint();
}

//Component OverWriting

/* ultra dangereux */
int Relation::getZoneX() { return std::min(xi,xf);}
int Relation::getZoneY() { return std::min(yi,yf);}
int Relation::getZoneRight() { return std::max(xi,xf);}
int Relation::getZoneBottom() { return std::max(yi,yf);}
int Relation::getZoneHeight() {return abs(yi-yf);}
int Relation::getZoneWidth() {return abs(xi-xf);}
//end

void Relation::setMuted(bool /*_m*/){return;}
bool Relation::isMuted(){return false;}
