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
#include "Trigger.h"
#include "Sequencer.h"

//==============================================================================
Trigger::Trigger (unsigned int _id, Sequencer* _parent, TimeBox* _controlledBox, bool _triggedToBegin, int x, int y)
:
SequenceOT(_parent, _id)
{
    //[Constructor] You can add your own custom stuff here..
	m_connected = false;
	m_controlledBox = _controlledBox;
	m_anchoringBoxStart = _triggedToBegin;
	m_colour = s_COLOUR_UNSELECTION;
	m_muted = false;
	m_askDeletion = false;
	mControlJack = new ControlJack(this);
	mControlJack->setVisible(false);

	m_bubbleInfo = new BubbleMessageComponent();
	m_bubbleInfo->setAlwaysOnTop (true);
	m_bubbleInfo->addToDesktop (0);
	m_bubbleInfo->setAllowedPlacement(2);
	
	m_execState = READY_STATE;
	
	setSize (s_TRIGRAIL_HEIGHT, s_TRIGRAIL_HEIGHT+10);
	setAlwaysOnTop(true);
	setTopLeftPosition(x, y);
	setWantsKeyboardFocus(true);
	//addChangeListener(getParentSequencer()->getMainWindow()->getPropertyFrame());
	listenToMeSequencer();
	
}

Trigger::~Trigger()
{
	//getParentSequencer()->getMainWindow()->getPropertyFrame()->setContent(0,true);
		
   	m_bubbleInfo->removeFromDesktop();
	if (mControlJack) {
	  deleteAndZero( mControlJack);
	}
	if (m_bubbleInfo) {
	  deleteAndZero( m_bubbleInfo);
	}
	
	getParentSequencer()->removeChildComponent(this->getControlJack());
	getParentSequencer()->removeChildComponent(this);
	
	//linked ?
	if(getLinkedBox() != 0) getLinkedBox()->deleteTriggerId(m_id);
	
	//delete
	getParentSequencer()->getEngines()->removeTriggerPoint(m_id);
	getParentSequencer()->getTrigMap()->erase(m_id); //je supprime le couple (id, boite) du registre
	getParentSequencer()->sendSynchronousChangeMessage(this);	
	
	deleteAllChildren();
}


Behavior* Trigger::getDurationBehavior() {
	
	return 0;
}


void Trigger::listenToMeSequencer(){
	addComponentListener(getParentSequencer());
	addChangeListener(getParentSequencer()->getPropertyFrame());
}
//================================== VIRAGE =====================================
void Trigger::init(){
	m_execState = READY_STATE;
	m_colour = Colours::white;
	repaint();
}

void Trigger::burn(){

	m_execState = TRIGGERED_STATE;
	m_colour = Colours::red;
	repaint();
}

bool Trigger::isTriggered(unsigned int _ctrlPtId) {
	return false;
}

void Trigger::setLinkedBox(TimeBox* _box, bool _anchoringStart, bool loadedTrig) {
	
	if(getLinkedBox() !=0 ) getLinkedBox()->deleteTrigger(getLinkedCtrlPoint()==1);
	
	m_controlledBox = _box;
	m_anchoringBoxStart = _anchoringStart;
	m_connected = true;
	mControlJack->setVisible(true);
	
	if(loadedTrig == false)
		if(_anchoringStart)	setMinuitMessage(String("/") + _box->getName() + String("/start"));
		else				setMinuitMessage(String("/") + _box->getName() + String("/end"));
	else
		setMinuitMessage(getParentSequencer()->getEngines()->getTriggerPointMessage(this->getId()).data());
	
	if(_anchoringStart)
		setTopLeftPosition(_box->getX() - getWidth()/2, 0);
	else
		setTopLeftPosition(_box->getRight()  - getWidth()/2, 0);
	
	/*pour finir on informe la boite qui pourra notamment déplacer le trigger*/
	_box->setNewTrigger(m_id, _anchoringStart);
		
	sendSynchronousChangeMessage(this);
}

TimeBox* Trigger::getLinkedBox() {
	return m_controlledBox;
}


void Trigger::disconnect() {	
	m_connected = false;
	sendSynchronousChangeMessage(this);
}


E_ControlPoint Trigger::getLinkedCtrlPoint() {
	if( m_anchoringBoxStart) return LEFT;
	else return RIGHT;
}

bool Trigger::isAnchoringStart() {
	return m_anchoringBoxStart;
}

//=================================== JUCE ======================================
void Trigger::paint(Graphics& g)
{	
	g.drawArrow(getWidth()/2, 0, getWidth()/2, s_TRIGRAIL_HEIGHT+10, 3, 10, 10);
	g.fillRoundedRectangle(0,0,getWidth(), s_TRIGRAIL_HEIGHT,4);
		
	if(m_muted) {
		
		g.drawLine(0,0,getWidth(),s_TRIGRAIL_HEIGHT);
		g.drawLine(0,s_TRIGRAIL_HEIGHT,getWidth(),0);
		
		g.setColour(Colours::red);
		g.drawLine(0,0,getWidth(),getWidth(),3);
		return;
	}
	
		 
	//symbole
	//g.setFont(s_SYMBOL1_FONT);
	//couleur
	g.setColour(m_colour); 
	Path path;
	int padding = 4;
	
	if(getLinkedBox()!=0 && getLinkedBox()->getY() < (getY()+s_TRIGRAIL_HEIGHT)) 
	{
		/*triangle vers haut*/
		path.startNewSubPath(padding, s_TRIGRAIL_HEIGHT-padding);
		path.lineTo(getWidth()/2, padding);
		path.lineTo( getWidth()-padding, s_TRIGRAIL_HEIGHT- padding);
		//path.lineTo(padding, s_TRIGRAIL_HEIGHT - padding);
		path.closeSubPath();
	}
	else{
		
		/*triangle vers bas */
		/*path.startNewSubPath(padding, padding);
		path.lineTo(getWidth()-padding, padding);
		path.lineTo(getWidth()/2, s_TRIGRAIL_HEIGHT - padding);
		//path.lineTo(padding, padding);
		path.closeSubPath();
		*/
		
		/*triangle vers sens de lecture */
		path.startNewSubPath(padding, padding);
		path.lineTo(getWidth()-padding, s_TRIGRAIL_HEIGHT/2);
		path.lineTo(padding, s_TRIGRAIL_HEIGHT - padding);
		//path.lineTo(padding, padding);
		path.closeSubPath();
	}
	g.fillPath(path, AffineTransform::identity);
	
	//selection
	if(getParentSequencer()->getSelectionSet()->isSelected(this)) g.fillAll(s_COLOUR_SELECTION.withAlpha(0.5f));
}

void Trigger::moved()
{
    mControlJack->resized();
/*prevent from verticzal moves*/	setTopLeftPosition(getX(), getParentSequencer()->getViewedPositionY());
	sendSynchronousChangeMessage(this);
	repaint();
}

void Trigger::mouseDown (const MouseEvent& e)
{
	//if(getParentSequencer()->getEngines()->isRunning()) return;
	getParentSequencer()->getSelectionSet()->addToSelectionBasedOnModifiers(this, e.mods);
	toFront(true);
	getParentSequencer()->repaint();
	if(e.y >= s_TRIGRAIL_HEIGHT) getParentComponent()->mouseDown(e.getEventRelativeTo(getParentComponent()));
}

E_TriggerState Trigger::getState(){
	return m_execState;
}

void Trigger::mouseDrag (const MouseEvent& e)
{
    if(getParentSequencer()->getEngines()->isRunning()) return;
	getParentComponent()->mouseDrag(e.getEventRelativeTo(getParentComponent()));
}

void Trigger::mouseUp (const MouseEvent& e)
{
	if(getParentSequencer()->getEngines()->isRunning() && contains(e.x, e.y)  && m_execState == WAITED_STATE) 
		getParentSequencer()->getEngines()->simulateNetworkMessageReception( (string)getMinuitMessage() );
	else 
		getParentComponent()->mouseUp(e.getEventRelativeTo(getParentComponent()));
}

void Trigger::mouseMove (const MouseEvent& e) {
	
	if(getParentSequencer()->getEngines()->isRunning()) return;
	if(isConnected()) m_bubbleInfo->showAt(this, getMinuitMessage(), 2000, true, false);
	else getParentComponent()->mouseMove(e.getEventRelativeTo(getParentComponent()));
}

void Trigger::mouseDoubleClick  (const MouseEvent& /*e*/)
{
	if(getParentSequencer()->getEngines()->isRunning()) return;
	m_muted = !m_muted;
	setMuted(m_muted);
}


void Trigger::setMuted(bool _m){	
	getParentSequencer()->getEngines()->setTriggerPointMutingState(m_id, _m);
	m_muted = _m;
	repaint();
}

bool Trigger::isMuted(){
	return m_muted;
}



int Trigger::getPlugX(){
	return (getX() + getWidth()/2);	
}

int Trigger::getPlugY(){
	return (getY() + getHeight());
}


bool Trigger::isAskingDeletion() {
	return m_askDeletion;
}

ControlJack* Trigger::getControlJack(){
	return mControlJack;
}

bool Trigger::isDeletionAsked() {
	return m_askDeletion;
}

void Trigger::linkHasBroken(){
	getLinkedBox()->deleteTrigger(m_anchoringBoxStart);
	m_connected = false;
	m_controlledBox = 0;
	mControlJack->setVisible(false);
}

void Trigger::waitedTriggerPointCallBack(bool _isWaited){
	
	m_execState = _isWaited ? WAITED_STATE : TRIGGERED_STATE;
	
	switch(m_execState) {
			
		case READY_STATE:
			m_colour = Colours::white;
			break;
		case WAITED_STATE:
			m_colour = Colours::orange;	
			break;
		case TRIGGERED_STATE:
			m_colour = Colours::red;
			break;
		default:
			break;
	}
}

bool Trigger::isConnected() {
	return m_connected;
}

Colour Trigger::getColour(){
	return m_colour;
}

void Trigger::setColour(Colour newColour){
	m_colour = newColour;
}

void Trigger::setOtColour(Colour newColour){
	m_colour = newColour;
	sendSynchronousChangeMessage(m_parentSequencer);//map
	repaint();
}

void Trigger::setMinuitMessage(String _minuitMessage){
	m_minuitMessage = _minuitMessage;
	getParentSequencer()->getEngines()->setTriggerPointMessage(m_id, (string)m_minuitMessage);
	sendSynchronousChangeMessage(this);
}

String Trigger::getMinuitMessage(){
	return  getParentSequencer()->getEngines()->getTriggerPointMessage(this->getId()).data();
}
