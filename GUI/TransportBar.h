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

#ifndef _TransportBar_H_
#define _TransportBar_H_

#include "includes.h"
#include "penIcon.h"
#include "BoxButton.h"

#include <sys/time.h>
#include <unistd.h>

class AccelerationSlider;
class MainComponent;
class SequencerFrame;
class Engines;
class PlayController;
class Sequencer;

class TransportButton : public TextButton {

 public:
 TransportButton(const String &buttonName, const String &/*toolTip*/=String::empty) : TextButton(buttonName) {
    setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
    setWantsKeyboardFocus(false);
    setColour(TextButton::buttonColourId, Colours::darkgrey.withAlpha(0.0f));
    setColour(TextButton::textColourId, Colours::black.withAlpha(0.0f));
  }

  ~TransportButton() {
  }

  /*   const Font getFont() { */

  /*     return  Font(String("Guifx v2 Transports Labeled"), 25, Font::plain); */
  /*   } */

  void paint(Graphics& g) {

    TextButton::paint(g);

    Colour bgColour = Colours::grey;
    if(isMouseOver()) {
      bgColour = Colours::lightgrey;
    }
    if(isDown() || getToggleState()) {
      bgColour = Colours::white;
    }
    if(!isEnabled()) {
      bgColour = (Colours::darkgrey.withAlpha(0.3f) );
    }

    int padding = 1;
    g.setColour(bgColour);
    g.fillRoundedRectangle(0+padding,0+padding,getWidth()-padding*2, getHeight()-padding*2, 4);
    g.setColour(bgColour.contrasting ());
    g.drawText(getButtonText(), 0, 0, getWidth(), getHeight(),  Justification::centred, false);

  }

  void click() {repaint();}

};

//==============================================================================

class TransportBar  : public Component, public ButtonListener
{
 public:
  TransportBar (MainComponent* _parent);
  ~TransportBar();

  void setChrono(unsigned int _ms);
  void initAbsoluteChrono();
  void updateAbsoluteChrono();
  void paint (Graphics& g);
  void resized();
  void buttonClicked (Button* buttonThatWasClicked);
  void turnMinuitActivity(bool _on, E_Messaging _inOrOut);
  penIcon* getCrayonButton();
  void refresh();

  void play(bool networkRequest = false);
  void pause(bool networkRequest = false);
  bool isPaused();
  bool isPlaying();
  void stop();
  void next();
  void rewind();

  Component* getLedIn();
  Component* getLedOut();

  TransportButton* getLoopButton();
  TransportButton* getStopButton();
  TransportButton* getPlayButton();
  TransportButton* getNextButton();

  /*ChangeListener*/
  void changListenerCallback(void * objectThatHasChanged);

  MainComponent *getMainComponent();

  double getSliderValue();
  void setSliderValue(double value);

 private:
  int previous_ms;
  MainComponent* m_frameParent;
  bool wantsLedOn;
  E_Messaging/*enum*/ m_ledPlug;
  double _stepLineStartX,_stepLineStartY,_stepLineWidth;

 protected:
  TransportButton* m_stopButton;
  TransportButton* m_playButton;
  TransportButton* m_pauseButton;
  TransportButton* m_rwdButton;
  TransportButton* m_nextButton;
  TransportButton* m_loopButton;
  Label* m_chronoLabel;
  Label* m_absChronoLabel;
  long long m_absChronoTimeStart;
  Label* m_oscActivityLabelIn;
  Label* m_oscActivityLabelOut;
  Label* m_messageLedIn;
  Label* m_messageLedOut;
  Label* m_speedLabel;

  BoxButton* flipButton;

  penIcon* crayonButton;
  AccelerationSlider* _slider;
  Sequencer *_sequencer;
  Engines* _engines;
  PlayController* _controller;

  TransportBar (const TransportBar&);
  const TransportBar& operator= (const TransportBar&);
};


#endif
