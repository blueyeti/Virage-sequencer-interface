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
#include "Sequencer.h"
#include "AutomationCurve.h"

//==============================================================================
TimeBoxContentComponent::TimeBoxContentComponent(Anchor* _containerL, Anchor* _containerR, TimeBox* _parent)
{

	m_parent = _parent;

	//Messages button
	addAndMakeVisible(messageStart = new MessageButton (this, T("Entering statements")));
	messageStart->setButtonText (T(""));
    messageStart->addButtonListener (this);

	addAndMakeVisible(messageEnd = new MessageButton (this,T("Leaving statements")));
    messageEnd->setButtonText (T(""));
    messageEnd->addButtonListener(this);

	//Mute buttons
	addAndMakeVisible(muteStart = new TextButton (T("M"), T("Mute start shot")));
    muteStart->addButtonListener(this);
	muteStart->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnBottom| Button::ConnectedOnTop);
    //muteStart->setColour (Colours::darkred);
	muteStart->setToggleState(false, false);
	muteStart->setClickingTogglesState(true);

	addAndMakeVisible(muteEnd = new TextButton (T("M"), T("Mute end shot")));
    muteEnd->addButtonListener(this);
	muteEnd->setConnectedEdges(Button::ConnectedOnRight | Button::ConnectedOnBottom| Button::ConnectedOnTop);
    //muteEnd->setColour (Colours::darkred);
	muteEnd->setToggleState(false, false);
	muteEnd->setClickingTogglesState(true);

	//automation
	addAndMakeVisible(curveComponent = new AutomationCurve(getTimeBoxParent()->getId(), m_parent->getParentSequencer()->getEngines(),
			AutomationCurve::DEFAULT_START_VALUE,AutomationCurve::DEFAULT_END_VALUE));

	(automationStretch = new BoxButton (T("s"), T("Automation Stretch")));
    automationStretch->addButtonListener(this);
    automationStretch->setColour (Colours::black);
    automationStretch->setToggleState(false, false);
	automationStretch->setClickingTogglesState(true);

	(automationEditor = new BoxButton (T("e"), T("Automation Editor")));
    automationEditor->addButtonListener(this);
    automationEditor->setColour (Colours::black);

	//automation
	(relationLocker = new BoxButton (T("L"), T("Relation locker"), true));
    relationLocker->addButtonListener(this);
    relationLocker->setColour ( Colours::black);
    relationLocker->setToggleState(false, false);
	relationLocker->setClickingTogglesState(true);


	//Leds
	(ledStart = new Label(T(""), T("")));
	ledStart->setColour(Label::backgroundColourId , s_COLOUR_UNSELECTION);

	(ledEnd   = new Label(T(""),T("")));
	ledEnd->setColour(Label::backgroundColourId , s_COLOUR_UNSELECTION);

	//options
	setWantsKeyboardFocus(false);

	//divers
	container1 = _containerL;
	container2 = _containerR;

	addAndMakeVisible(container1);
	addAndMakeVisible(container2);

	m_first_CP_done = false;
	m_last_CP_done = false;
	m_contentIsMuted = false;
}

TimeBoxContentComponent::~TimeBoxContentComponent(){
	deleteAllChildren();
}


bool TimeBoxContentComponent::keyPressed	(const KeyPress & 	key){return false;}

Anchor* TimeBoxContentComponent::getAnchorLeft(){return container1;}

Anchor* TimeBoxContentComponent::getAnchorRight(){return container2;}

AutomationCurve* TimeBoxContentComponent::getAutomationCurve(){return curveComponent;}

TimeBox* TimeBoxContentComponent::getTimeBoxParent(){
	return m_parent;
}

void TimeBoxContentComponent::mouseDown(const MouseEvent &e){
	getParentComponent()->mouseDown(e.getEventRelativeTo(getParentComponent()));
}

void TimeBoxContentComponent::mouseUp(const MouseEvent &e){
	getParentComponent()->mouseUp(e.getEventRelativeTo(getParentComponent()));
}

void TimeBoxContentComponent::mouseDoubleClick(const MouseEvent &e){
	if(getTimeBoxParent()->isInterpolated()) getTimeBoxParent()->editAutomation();
}

//==============================================================================
void TimeBoxContentComponent::paint(Graphics& g) {
	g.fillAll (Colours::darkgrey);

	//buttonbar
	int buttonBarHeight = ((TimeBox*)getParentComponent())->getButtonBarHeight();

	g.setColour(Colours::white);

	if(!m_contentIsMuted) {
		if(m_last_CP_done) {

			// finished = hachures
			g.setColour(Colours::grey.withAlpha(0.5f));
			g.fillRect(0, 0, getWidth(), getHeight()-buttonBarHeight );
			g.setColour(Colours::white);
			g.setFont(s_GUIFX_FONT);

			g.setColour(Colours::white.withAlpha(0.2f));

			/*for(int i = 0; i < getWidth() + 4000; i+=15){

				Path hashure;
				hashure.startNewSubPath(i, 0);
				hashure.lineTo(i+13, 0);
				hashure.lineTo(i+3-4000, 4000);
				hashure.lineTo(i-10-4000, 4000);
				hashure.closeSubPath();
				g.fillPath(hashure);
			}
			*/
			//	unsigned int diametre = std::max(30, std::min(getWidth(),getHeight() -  buttonBarHeight)-20);//20=offeset
			//	g.drawLine(getWidth()/2 - diametre/2*sqrt(2)/2, (getHeight() -  buttonBarHeight)/2 - diametre/2*sqrt(2)/2, getWidth()/2 + diametre/2*sqrt(2)/2, getHeight()/2 - buttonBarHeight/2 + diametre/2*sqrt(2)/2, 8);
			//	g.drawLine(getWidth()/2 - diametre/2*sqrt(2)/2, (getHeight() -  buttonBarHeight)/2 + diametre/2*sqrt(2)/2, getWidth()/2 + diametre/2*sqrt(2)/2, getHeight()/2 - buttonBarHeight/2 - diametre/2*sqrt(2)/2, 8);
		}
	}

	else {

		/* MUTED" */
		g.setColour(Colours::darkred.withAlpha(0.5f));
		g.fillRect(0, 0, getWidth(), getHeight()-buttonBarHeight );
		g.setColour(Colours::white.withAlpha(0.5f));
		unsigned int diametre = std::max(30, std::min(getWidth(),getHeight() -  buttonBarHeight)-20);//20=offeset
		g.drawEllipse(getWidth()/2 - diametre/2, getHeight()/2 - buttonBarHeight/2- diametre/2, diametre, diametre, 8);
		g.drawLine(getWidth()/2 - diametre/2*sqrt(2)/2, (getHeight() -  buttonBarHeight)/2 - diametre/2*sqrt(2)/2, getWidth()/2 + diametre/2*sqrt(2)/2, getHeight()/2 - buttonBarHeight/2 + diametre/2*sqrt(2)/2, 8);
	}

	/*selection*/
	if(((TimeBox*)getParentComponent())->getParentSequencer()->getSelectionSet()->isSelected(((TimeBox*)getParentComponent()))) g.fillAll(s_COLOUR_SELECTION.withAlpha(0.3f));
}

void
TimeBoxContentComponent::controlPointDone(unsigned int controlPointIndex) {

	m_first_CP_done = (controlPointIndex == 1);
	m_last_CP_done = ( controlPointIndex == 2);
}

void TimeBoxContentComponent::resized() {
	messageStart->setBounds (0, 0, s_BOX_MESSAGEBUTTON_WIDTH, getHeight());
	messageEnd->setBounds (getWidth()-s_BOX_MESSAGEBUTTON_WIDTH, 0, s_BOX_MESSAGEBUTTON_WIDTH, getHeight());


	curveComponent->setVisible(((TimeBox*)getParentComponent())->isInterpolated());
	curveComponent->setBounds(messageStart->getRight(), 0, getWidth()-messageStart->getWidth()-messageEnd->getWidth(), getHeight());


	muteStart->		setBounds (0,										(getHeight()-s_BOX_MUTE_HEIGHT) , (s_BOX_MESSAGEBUTTON_WIDTH), (s_BOX_MUTE_HEIGHT) );
  	muteEnd->		setBounds (getWidth()-s_BOX_MESSAGEBUTTON_WIDTH,	(getHeight()-s_BOX_MUTE_HEIGHT) , (s_BOX_MESSAGEBUTTON_WIDTH), (s_BOX_MUTE_HEIGHT) );

	//button
	/*trivial*/relationLocker->setBounds	(s_BOX_MESSAGEBUTTON_WIDTH, (getHeight()-s_BOX_MUTE_HEIGHT*2), s_BOX_MUTE_HEIGHT, (s_BOX_MUTE_HEIGHT) );
	/*trivial*/automationStretch->setBounds	(s_BOX_MESSAGEBUTTON_WIDTH + relationLocker->getWidth(), (getHeight()-s_BOX_MUTE_HEIGHT*2), s_BOX_MUTE_HEIGHT, (s_BOX_MUTE_HEIGHT) );
	/*trivial*/automationEditor ->setBounds	(s_BOX_MESSAGEBUTTON_WIDTH + 2*automationStretch->getWidth(), (getHeight()-s_BOX_MUTE_HEIGHT*2), s_BOX_MUTE_HEIGHT, (s_BOX_MUTE_HEIGHT) );

	//led end/start
	ledStart->	setBounds(0, 0, muteStart->getWidth()/2, muteStart->getWidth()/2);
	ledEnd->	setBounds(getWidth()-20, 0, muteStart->getWidth()/2, muteStart->getWidth()/2);

	//Anchors - INSIDE
	float yAnchorCenter = (getHeight())/2;
	if(container1 != 0) container1->setBounds(0, yAnchorCenter-s_ANCHOR_HEIGHT/2, s_ANCHOR_WIDTH, s_ANCHOR_HEIGHT);
	if(container2 != 0) container2->setBounds(getWidth()-s_ANCHOR_WIDTH, yAnchorCenter-s_ANCHOR_HEIGHT/2, s_ANCHOR_WIDTH, s_ANCHOR_HEIGHT);

	//CURVE REMOTE
	//if(!getParentSequencer()->getEngines()->isRunning() && ((TimeBox*)getParentComponent())->isInterpolated()) adsr->setBounds(0,0,getWidth(),std::min(getHeight()/4, 30));
}

void
TimeBoxContentComponent::buttonClicked (Button* buttonThatWasClicked) {
	((TimeBox*)getParentComponent())->buttonClicked(NULL);

	if(buttonThatWasClicked == messageStart) ((TimeBox*)getParentComponent())->showMessagesEditor(true);
	if(buttonThatWasClicked == messageEnd) ((TimeBox*)getParentComponent())->showMessagesEditor(false);
	if(buttonThatWasClicked == muteStart)
	{
		((TimeBox*)getParentComponent())-> getParentSequencer()->getEngines()->setCtrlPointMutingState(((TimeBox*)getParentComponent())->getId(), CPT_L , muteStart->getToggleState());
		messageStart->setMuted(muteStart->getToggleState());
	}
	if(buttonThatWasClicked == muteEnd)
	{
		((TimeBox*)getParentComponent())->getParentSequencer()->getEngines()->setCtrlPointMutingState(((TimeBox*)getParentComponent())->getId(), CPT_R, muteEnd->getToggleState() );
		messageEnd->setMuted(muteEnd->getToggleState());
	}

	if(buttonThatWasClicked == automationStretch) return;
	if(buttonThatWasClicked == automationEditor) return;
	if(buttonThatWasClicked == relationLocker) return;
}

Sequencer*
TimeBoxContentComponent::getParentSequencer() {
	return m_parent->getParentSequencer();
}

void TimeBoxContentComponent::setMessageStartButtonVisible(bool _visible) {
	messageStart->setFilled(_visible);
}

void TimeBoxContentComponent::setMessageEndButtonVisible(bool _visible) {
	messageEnd->setFilled(_visible);
}

void TimeBoxContentComponent::init() {
	m_last_CP_done = false;
	m_first_CP_done = false;
}

void TimeBoxContentComponent::muteAll(bool _mute) {

	muteStart->setToggleState(_mute, true);
	muteEnd->setToggleState(_mute, true);
}

void TimeBoxContentComponent::setMutedContent(bool _m) {
	m_contentIsMuted = _m;
	repaint();
}
