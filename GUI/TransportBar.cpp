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
#include "TransportBar.h"
#include "SequencerFrame.h"
#include "MainComponent.h"
#include "AccelerationSlider.h"
#include "PlayController.h"
#include "Engines.hpp"
#include "Sequencer.h"

//==============================================================================
TransportBar::TransportBar (MainComponent* _parent)
    : Component (T("TransportBar")),
    _stepLineStartX(0),_stepLineStartY(0),_stepLineWidth(10),
    m_stopButton (0), m_playButton (0), m_rwdButton (0), m_nextButton (0), m_loopButton (0),
    m_chronoLabel (0),
	  m_oscActivityLabelIn (0), m_oscActivityLabelOut (0), m_messageLedIn (0), m_messageLedOut(0),
	  m_frameParent(_parent), wantsLedOn(false), m_ledPlug(MESSAGE_IN)
{
	setWantsKeyboardFocus(false);

	_sequencer = m_frameParent->getMiddleFrame()->getConduite();
	_engines = _sequencer->getEngines();
	_controller = _sequencer->getController();


	addAndMakeVisible (m_stopButton = new TransportButton(T("")));
	m_stopButton->setButtonText (T("STOP"));
	m_stopButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	m_stopButton->addButtonListener (this);

	addAndMakeVisible (m_pauseButton = new TransportButton(T("")));
	m_pauseButton->setButtonText (T("PAUSE"));
	m_pauseButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	m_pauseButton->setClickingTogglesState(true);
	m_pauseButton->addButtonListener (this);

	addAndMakeVisible (_slider = new AccelerationSlider(T("Slider")));
	_slider->setRange(AccelerationSlider::START_VALUE, AccelerationSlider::END_VALUE, AccelerationSlider::STEP_TOLERANCE);
	// Skew factor for logarithmic repartition on the slider
	// letting '1' value at center : obtained experimentally
	_slider->setSkewFactor(AccelerationSlider::SKEW_FACTOR);
	_slider->setValue(AccelerationSlider::DEFAULT_VALUE);
	//_slider->setSliderStyle(Slider::RotaryHorizontalDrag);
//	_slider->setColour(Slider::rotarySliderFillColourId,Colours::white);
//	_slider->setColour(Slider::rotarySliderOutlineColourId,Colours::black);
	_slider->setColour(Slider::backgroundColourId,Colours::black);
  _slider->setColour(Slider::textBoxTextColourId,Colours::white);
  _slider->setColour(Slider::textBoxBackgroundColourId,Colours::black);
  _slider->setColour(Slider::textBoxHighlightColourId,Colours::white);
  _slider->setColour(Slider::textBoxOutlineColourId,Colours::grey);

	//_slider->applyFontToAllText();

	addAndMakeVisible (crayonButton = new penIcon());
	crayonButton->addButtonListener (this);
	crayonButton->setToggleState(true, true);
	crayonButton->setClickingTogglesState(true);
	crayonButton->setWantsKeyboardFocus(false);

	addAndMakeVisible (m_playButton = new TransportButton (T("play")));
	m_playButton->setButtonText (T("PLAY"));
	m_playButton->setToggleState(false, false);
	m_playButton->setClickingTogglesState(true);
	m_playButton->addButtonListener (this);

	addAndMakeVisible (m_rwdButton = new TransportButton (T("")));
	m_rwdButton->setButtonText (T("|<"));
	m_rwdButton->addButtonListener (this);

	addAndMakeVisible (m_nextButton = new TransportButton (T("")));
	m_nextButton->setButtonText (T("NEXT"));
	m_nextButton->addButtonListener (this);
	m_nextButton->setEnabled(true);

	addAndMakeVisible (m_loopButton = new TransportButton (T("")));
	m_loopButton->setButtonText (T("LOOP"));
	m_loopButton->setToggleState(false, true);
	m_loopButton->setClickingTogglesState(true);
	m_loopButton->addButtonListener (this);

	//********//
	addAndMakeVisible (m_chronoLabel = new Label (T("Chrono"), T("00\'00\'\'000 \n")));
	m_chronoLabel->setFont (Font (String(/*"ds-digi"*/"Helvetica Neue"), 20.0000f, Font::plain));
	m_chronoLabel->setJustificationType (Justification::centredLeft);
	m_chronoLabel->setEditable (false, false, false);
	m_chronoLabel->setColour (Label::textColourId,  Colours::white);
	m_chronoLabel->setWantsKeyboardFocus(false);

	m_absChronoTimeStart = 0;
	addAndMakeVisible (m_absChronoLabel = new Label (T("abs Chrono"), T("00\'00\'\'000 \n")));
	m_absChronoLabel->setFont (Font (String(/*"ds-digi"*/"Helvetica Neue"), 35.0000f, Font::plain));
	m_absChronoLabel->setJustificationType (Justification::centredLeft);
	m_absChronoLabel->setEditable(false, false, false);
	m_absChronoLabel->setColour (Label::textColourId,  Colours::white);
	m_absChronoLabel->setWantsKeyboardFocus(false);

	addAndMakeVisible (m_oscActivityLabelIn = new Label (T("in"), T("IN")));
	m_oscActivityLabelIn->setFont (Font (15.0000f, Font::plain));
	m_oscActivityLabelIn->setJustificationType (Justification::centredLeft);
	m_oscActivityLabelIn->setEditable (false, true, false);
	m_oscActivityLabelIn->setColour (Label::textColourId, Colours::white);
	m_oscActivityLabelIn->setColour (TextEditor::textColourId, Colours::white);
	m_oscActivityLabelIn->setColour (TextEditor::backgroundColourId, Colour (0x0));
	m_oscActivityLabelIn->setEditable (false, true, false);
	m_oscActivityLabelIn->setWantsKeyboardFocus(false);

	addAndMakeVisible (m_speedLabel = new Label (T("speed"), T("SPEED\nTEST")));
	m_speedLabel->setFont (Font (15.0000f, Font::plain));
	m_speedLabel->setJustificationType (Justification::centredBottom);
	m_speedLabel->setEditable(false, true, false);
	m_speedLabel->setColour(Label::textColourId, Colours::grey);
	m_speedLabel->setColour(TextEditor::textColourId, Colours::grey);
	m_speedLabel->setColour(TextEditor::backgroundColourId, Colours::black);
	m_speedLabel->setEditable (false, true, false);
	m_speedLabel->setWantsKeyboardFocus(false);

	addAndMakeVisible (m_oscActivityLabelOut = new Label (T("out"),T("OUT")));
	m_oscActivityLabelOut->setFont (Font (15.0000f, Font::plain));
	m_oscActivityLabelOut->setJustificationType (Justification::centredLeft);
	m_oscActivityLabelOut->setEditable (false, true, false);
	m_oscActivityLabelOut->setColour (Label::textColourId, Colours::white);
	m_oscActivityLabelOut->setColour (TextEditor::textColourId, Colours::white);
	m_oscActivityLabelOut->setColour (TextEditor::backgroundColourId, Colour (0x0));
	m_oscActivityLabelOut->setEditable (false, true, false);
	m_oscActivityLabelOut->setWantsKeyboardFocus(false);

	addAndMakeVisible (m_messageLedIn = new Label(T(""), T("")));
	m_messageLedIn->setName (T("new component"));
	m_messageLedIn->setColour(Label::backgroundColourId, s_COLOUR_UNSELECTION);
	m_messageLedIn->setVisible(false);
	m_messageLedIn->setWantsKeyboardFocus(false);

	addAndMakeVisible (m_messageLedOut = new Label(T(""), T("")));
	m_messageLedOut->setName (T("new component"));
	m_messageLedOut->setColour(Label::backgroundColourId, s_COLOUR_UNSELECTION);
	m_messageLedOut->setVisible(false);
	m_messageLedOut->setWantsKeyboardFocus(false);

	addAndMakeVisible (flipButton = new BoxButton(T("")));
	flipButton->setButtonText(String("Cue"));
	flipButton->setWantsKeyboardFocus(false);
	flipButton->setToggleState(false, true);
	flipButton->setClickingTogglesState(true);

	//size
	setSize (1000, 100);

	//
	previous_ms = -1;
}

TransportBar::~TransportBar(){
	deleteAllChildren();
}

//==============================================================================
void TransportBar::paint (Graphics& g)
{
	static const int STEP_WIDTH = 5;
	static const int TEXT_WIDTH = 30;
	static const int TEXT_HEIGHT = 4;
	static const int LINE_WIDTH = 2;
	g.fillAll (Colours::black);//s_BGCOLOUR);
  g.setColour(Colours::grey);
  g.drawLine (_stepLineStartX,_stepLineStartY,_stepLineStartX + _stepLineWidth,_stepLineStartY,LINE_WIDTH);
  g.setColour(Colours::white);
  static const std::vector<double> STEPS = _slider->getSteps();
/*  if (STEP_MAP.empty()) {
  	STEP_MAP[0] = 0;
  	STEP_MAP[0.12] = 0.2;
  	STEP_MAP[0.26] = 0.33;
  	STEP_MAP[0.36]	= 0.5;
  	STEP_MAP[0.42] = 0.66;
  	STEP_MAP[0.51] = 1;
  	STEP_MAP[0.69] = 2;
  	STEP_MAP[0.82] = 3;
  	STEP_MAP[0.92] = 4;
  	STEP_MAP[1] = 5;
  }*/

  const int TEXT_TOP = _stepLineStartY + LINE_WIDTH;
  std::vector<double>::const_iterator it;
  for (it = STEPS.begin() ; it != STEPS.end() ; ++it) {
  	//g.fillEllipse(_stepLineStartX + it->first * _stepLineWidth - STEP_WIDTH / 2, _stepLineStartY - STEP_WIDTH / 2.,STEP_WIDTH,STEP_WIDTH);
  	float posValue = _slider->getPositionOfValue(*it);
  	g.fillEllipse(_stepLineStartX + posValue - 2*STEP_WIDTH, _stepLineStartY - STEP_WIDTH / 2.,STEP_WIDTH,STEP_WIDTH);
  	if (*it == 0.33) {
    	g.drawText(String("1"),_stepLineStartX + posValue - 2*STEP_WIDTH - TEXT_WIDTH / 2,TEXT_TOP + TEXT_HEIGHT,TEXT_WIDTH,TEXT_HEIGHT,Justification::centred,false);
    	g.drawText(String("/"),_stepLineStartX + posValue - 2*STEP_WIDTH - TEXT_WIDTH / 3,TEXT_TOP + TEXT_HEIGHT * 1.5,TEXT_WIDTH,TEXT_HEIGHT,Justification::centred,false);
    	g.drawText(String("3"),_stepLineStartX + posValue - 2*STEP_WIDTH - TEXT_WIDTH / 6,TEXT_TOP + TEXT_HEIGHT * 3,TEXT_WIDTH,TEXT_HEIGHT,Justification::centred,false);
  	}
  	else if (*it == 0.66) {
    	g.drawText(String("2"),_stepLineStartX + posValue - 2*STEP_WIDTH - TEXT_WIDTH / 2,TEXT_TOP + TEXT_HEIGHT ,TEXT_WIDTH,TEXT_HEIGHT,Justification::centred,false);
    	g.drawText(String("/"),_stepLineStartX + posValue - 2*STEP_WIDTH - TEXT_WIDTH / 3,TEXT_TOP + TEXT_HEIGHT * 1.5,TEXT_WIDTH,TEXT_HEIGHT,Justification::centred,false);
    	g.drawText(String("3"),_stepLineStartX + posValue - 2*STEP_WIDTH - TEXT_WIDTH / 5,TEXT_TOP + TEXT_HEIGHT * 3,TEXT_WIDTH,TEXT_HEIGHT,Justification::centred,false);
  	}
  	else if (*it == 0.5) {
    	g.drawText(String("1"),_stepLineStartX + posValue - 2*STEP_WIDTH - TEXT_WIDTH / 2,TEXT_TOP + TEXT_HEIGHT ,TEXT_WIDTH,TEXT_HEIGHT,Justification::centred,false);
    	g.drawText(String("/"),_stepLineStartX + posValue - 2*STEP_WIDTH - TEXT_WIDTH / 3,TEXT_TOP + TEXT_HEIGHT * 1.5,TEXT_WIDTH,TEXT_HEIGHT,Justification::centred,false);
    	g.drawText(String("2"),_stepLineStartX + posValue - 2*STEP_WIDTH - TEXT_WIDTH / 5,TEXT_TOP + TEXT_HEIGHT * 3,TEXT_WIDTH,TEXT_HEIGHT,Justification::centred,false);
  	}
  	else if (*it == 0.2) {
    	g.drawText(String("1"),_stepLineStartX + posValue - 2*STEP_WIDTH - TEXT_WIDTH / 2,TEXT_TOP + TEXT_HEIGHT ,TEXT_WIDTH,TEXT_HEIGHT,Justification::centred,false);
    	g.drawText(String("/"),_stepLineStartX + posValue - 2*STEP_WIDTH - TEXT_WIDTH / 3,TEXT_TOP + TEXT_HEIGHT * 1.5,TEXT_WIDTH,TEXT_HEIGHT,Justification::centred,false);
    	g.drawText(String("5"),_stepLineStartX + posValue - 2*STEP_WIDTH - TEXT_WIDTH / 5,TEXT_TOP + TEXT_HEIGHT * 3,TEXT_WIDTH,TEXT_HEIGHT,Justification::centred,false);
  	}
  	else {
  		g.drawText(String(*it),_stepLineStartX + posValue - 2*STEP_WIDTH - TEXT_WIDTH / 2,TEXT_TOP + TEXT_HEIGHT *1.5,TEXT_WIDTH,TEXT_HEIGHT,Justification::centred,false);
  	}
  }
}

void TransportBar::resized()
{
	static const int H_SEPARATION = 5;
	static const int H_SPACE = 20;
	static const int V_SPACE = 5;
	static const int V_BIG_SPACE = 10;
	const int HEIGHT = getHeight() - 2 * V_SPACE;
	static const int CHRONO_WIDTH = 150;
	static const int OSC_WIDTH = 25;
	static const int OSC_HEIGHT = 12;
	static const int LED_WIDTH = 50;
	static const int LED_HEIGHT = 12;
	static const int BUTTON_BIG_WIDTH = 150;
	static const int BUTTON_WIDTH = 80;
	static const int BUTTON_SMALL_WIDTH = 50;
	static const int SLIDER_WIDTH = 400;
	static const int SLIDER_HEIGHT = 13;
	static const int SPEED_WIDTH = 50;
	static const int SPEED_HEIGHT = HEIGHT;//15
	static const int CRAYON_BUTTON_SIZE = 20;

	int hPos = 10;

	m_absChronoLabel->setBounds(hPos,V_SPACE,CHRONO_WIDTH,HEIGHT);
	hPos += CHRONO_WIDTH + H_SPACE;

	/*leds in out */
	m_oscActivityLabelIn->setBounds(hPos, V_SPACE, OSC_WIDTH, OSC_HEIGHT);
	m_oscActivityLabelOut->setBounds(hPos, V_SPACE + OSC_HEIGHT + V_SPACE, OSC_WIDTH, OSC_HEIGHT);
	hPos += OSC_WIDTH + H_SEPARATION;
	m_messageLedIn->setBounds(hPos, V_SPACE,  LED_WIDTH, LED_HEIGHT);
	m_messageLedOut->setBounds(hPos, V_SPACE + LED_HEIGHT + V_SPACE,  LED_WIDTH, LED_HEIGHT);
	hPos += LED_WIDTH + H_SPACE;

	m_rwdButton->setBounds(hPos, V_SPACE, BUTTON_WIDTH, HEIGHT);
	hPos += BUTTON_WIDTH + H_SPACE;

	m_stopButton->setBounds (hPos, V_SPACE, BUTTON_WIDTH, HEIGHT);
	hPos += BUTTON_WIDTH + H_SEPARATION;
		m_playButton->setBounds (hPos, V_SPACE, BUTTON_BIG_WIDTH,  HEIGHT);
	hPos += BUTTON_BIG_WIDTH + H_SEPARATION;
	m_pauseButton->setBounds(hPos, V_SPACE, BUTTON_SMALL_WIDTH, HEIGHT);
	hPos += BUTTON_SMALL_WIDTH + H_SPACE;

	m_nextButton->setBounds(hPos, V_SPACE, BUTTON_WIDTH, HEIGHT);
	hPos += BUTTON_WIDTH + H_SPACE;

	m_loopButton->setBounds(hPos, V_SPACE, BUTTON_SMALL_WIDTH, HEIGHT);
	hPos += BUTTON_SMALL_WIDTH + H_SPACE;

	_slider->setBounds(hPos, V_SPACE, SLIDER_WIDTH, SLIDER_HEIGHT);

	_stepLineWidth = _slider->getPositionOfValue(AccelerationSlider::END_VALUE) - _slider->getPositionOfValue(AccelerationSlider::START_VALUE);
	_stepLineStartX = hPos + _slider->getPositionOfValue(AccelerationSlider::START_VALUE);
	_stepLineStartY = V_SPACE + SLIDER_HEIGHT + 3; //V_SPACE + HEIGHT + V_SPACE - 3;

	_slider->setTextBoxStyle(Slider::TextBoxRight, false, BUTTON_WIDTH, HEIGHT);
	//_slider->setTextBoxIsEditable(false);
  _slider->setColour(Slider::backgroundColourId,Colours::black);
	_slider->setColour(Slider::textBoxTextColourId,Colours::black);
	_slider->setColour(Slider::textBoxBackgroundColourId,Colours::lightgrey);
	_slider->setColour(Slider::textBoxHighlightColourId,Colours::white);
	_slider->setColour(Slider::textBoxOutlineColourId,Colours::lightgrey);

	hPos += SLIDER_WIDTH + H_SPACE;

	m_chronoLabel->setBounds(hPos - BUTTON_WIDTH - H_SPACE,V_SPACE + OSC_HEIGHT + 7,CHRONO_WIDTH,OSC_HEIGHT);

  //m_speedLabel->setBounds(hPos + SLIDER_WIDTH - 1.5 * SPEED_WIDTH , HEIGHT - 4, SPEED_WIDTH, SPEED_HEIGHT);

	crayonButton->setBounds(hPos, V_BIG_SPACE, CRAYON_BUTTON_SIZE, CRAYON_BUTTON_SIZE);

	hPos += CRAYON_BUTTON_SIZE + H_SPACE;

  //m_speedLabel->setBounds(hPos, 0, SPEED_WIDTH, SPEED_HEIGHT);

  //hPos += SPEED_WIDTHBUTTON_SIZE + H_SPACE;

  //flipButton->setBounds(hPos, V_SPACE, BUTTON_SMALL_WIDTH, HEIGHT);


}


TransportButton* TransportBar::getLoopButton(){
	return m_loopButton;
}

void TransportBar::changListenerCallback(void * objectThatHasChanged){

}

TransportButton* TransportBar::getStopButton(){

	return m_stopButton;
}
TransportButton* TransportBar::getNextButton(){

	return m_nextButton;
}

TransportButton* TransportBar::getPlayButton(){

	return m_playButton;
}

void TransportBar::buttonClicked (Button* buttonThatWasClicked)
{
	if (buttonThatWasClicked == m_rwdButton) {
		rewind();
	}
	else if (buttonThatWasClicked == m_loopButton) {
		_controller->setMustLoop(m_loopButton->getToggleState());
	}
	else if (buttonThatWasClicked == m_stopButton) {
		stop();
	}
	else if (buttonThatWasClicked == m_playButton) {
		play();
	}
	else if (buttonThatWasClicked == m_nextButton) {
		next();
	}
	else if (buttonThatWasClicked == crayonButton)    {
		if(crayonButton->getToggleState())
			_sequencer->setMouseMode(DRAWBOX);
		else
			_sequencer->setMouseMode(SELECTION);
	}
	else if (buttonThatWasClicked == m_pauseButton) {
		pause();
	}
}

void TransportBar::play(bool networkRequest) {
	bool isPlaying = _engines->isRunning();
	bool isPaused = _engines->isPaused();
	bool isNowPlaying = false;

	if (isPlaying && !isPaused) {
		isNowPlaying = true;
	}
	else if (!isPlaying && !isPaused){ // not playing
		isNowPlaying = true;
	}

	m_playButton->setToggleState(isNowPlaying, false);
	if (isNowPlaying) {
		_controller->play();
		initAbsoluteChrono();
	}
}

bool TransportBar::isPaused() {
	return _engines->isPaused();
}

bool TransportBar::isPlaying() {
	return _engines->isRunning();
}

void TransportBar::pause(bool networkRequest)
{
	if (networkRequest) {
		_controller->pause(isPaused());
		m_pauseButton->setToggleState(isPaused(),false);
	}
	else {
		_controller->pause(!isPaused());
		m_pauseButton->setToggleState(isPaused(),false);
	}
}

void TransportBar::stop()
{
	_sequencer->setAskForStop(true);

	_controller->stop();

	m_playButton->setToggleState(false, false);
	m_pauseButton->setToggleState(false, false);
	if (_controller->mustLoop()) {
		play();
	}

}

void TransportBar::next()
{
	if( _engines->isRunning() ) {
		_sequencer->simulateNextTrig();
	}
}

void TransportBar::rewind()
{
	_sequencer->resetHeadPlay();
	stop();
}

void TransportBar::setChrono(unsigned int _ms) {

	unsigned int min = (unsigned int)((float)_ms/60/1000); // from ms to min
	unsigned int sec = (unsigned int)((float)_ms/1000) - min*60;
	unsigned int mSec = _ms - (min*60000 + sec*1000);

	String zeroMin = String("");
	String zeroSec = String("");
	String zeroMs = String("");

	if(min<10) {
		zeroMin = String("0");
	}
	if(sec<10) {
		zeroSec = String("0");
	}
	if(mSec<100) {
		zeroMs = String("0");
	}
	if(mSec<10) {
		zeroMs = String("00");
	}

	m_chronoLabel->setText(String(zeroMin + String(min) + String("'") + zeroSec + String(sec) + String("''") + zeroMs + String(mSec)), false/*broadcast change*/);
}

void TransportBar::initAbsoluteChrono() {
	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);

	//std::cout << "Start value" << tv.tv_sec * 1000000LL << std::endl;
	m_absChronoTimeStart = (tv.tv_sec * 1000000LL + tv.tv_usec)/1000LL;
}

void TransportBar::updateAbsoluteChrono()
{
  struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);

	long long currentTime = (tv.tv_sec * 1000000LL + tv.tv_usec)/1000LL;
	long long timeElapsed = currentTime - m_absChronoTimeStart;
  //std::cout << currentTime << std::endl;
	unsigned int min = timeElapsed/60000.;
	unsigned int sec = timeElapsed/1000. - min*60.;
	unsigned int mSec = timeElapsed - (min*60000 + sec*1000);

	String zeroMin = String("");
	String zeroSec = String("");
	String zeroMs = String("");

	if(min<10) {
		zeroMin = String("0");
	}
	if(sec<10) {
		zeroSec = String("0");
	}
	if(mSec<100) {
		zeroMs = String("0");
	}
	if(mSec<10) {
		zeroMs = String("00");
	}

	m_absChronoLabel->setText(String(zeroMin + String(min) + String("'") + zeroSec + String(sec) + String("''") + zeroMs + String(mSec)), false/*broadcast change*/);
}

void TransportBar::turnMinuitActivity(bool _on, E_Messaging _inOrOut) {

	wantsLedOn = _on;
	m_ledPlug = _inOrOut;


	if( m_ledPlug == MESSAGE_IN ) {
		m_messageLedIn->setVisible(wantsLedOn);
		return;
	}

	if( m_ledPlug == MESSAGE_OUT ) {
		m_messageLedOut->setVisible(wantsLedOn);
		return;
	}
}

Component* TransportBar::getLedIn() {
	return m_messageLedIn;
}

Component* TransportBar::getLedOut() {
	return m_messageLedOut;
}

void TransportBar::refresh (){
	switch (m_ledPlug) {
		case MESSAGE_IN:
			m_messageLedIn->setVisible(true);
			if( !wantsLedOn ) m_messageLedIn->setVisible(false);//fadeOutComponent(100/*ms*/, 0, 0, 0);
			break;
		case MESSAGE_OUT:
			m_messageLedOut->setVisible(true);
			if( !wantsLedOn ) m_messageLedOut->setVisible(false);//fadeOutComponent(100/*ms*/, 0, 0, 0);
			break;
		default:
			break;
	}
}

penIcon* TransportBar::getCrayonButton() {
	return crayonButton;
}

MainComponent* TransportBar::getMainComponent() {
	return m_frameParent;
}

double TransportBar::getSliderValue() {
	return _slider->getValue();
}

void TransportBar::setSliderValue(double value) {
	_slider->setValue(value,true,true);
}
