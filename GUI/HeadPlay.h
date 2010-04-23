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
#ifndef _HEADPLAY_H_
#define _HEADPLAY_H_

#include "includes.h"

#define WIDTH 10

class HeadPlay : public Component {

private:
  int m_timePos;
  Sequencer* m_parent;
  Engines* m_engines;

public:
  HeadPlay(Sequencer* _parent) : m_parent(_parent) {
    setAlwaysOnTop(true);
    setWantsKeyboardFocus(false);
    setSize(WIDTH, m_parent->getHeight() );
    addComponentListener(_parent);
    m_engines = m_parent->getEngines();
  }

  ~HeadPlay() {}

  void paint(Graphics& g) {

    //rectangle tête
    g.setColour(Colours::darkgrey);
    if(isMouseOver()) g.setColour(Colours::lightgrey);
    g.fillRect(0, s_TRIGRAIL_HEIGHT,getWidth(),s_TIMERAIL_HEIGHT - s_TRIGRAIL_HEIGHT);
    g.setColour(Colours::black);
    g.drawRect(0, s_TRIGRAIL_HEIGHT, getWidth(), s_TIMERAIL_HEIGHT - s_TRIGRAIL_HEIGHT, 1);

    //ligne
    g.drawLine(getWidth()/2-0.75f, s_TIMERAIL_HEIGHT+1, getWidth()/2+0.75f, getHeight(), 1.5f);
  }

  void mouseUp(const MouseEvent& e) {

    if(!e.mouseWasClicked() || !e.mods.isPopupMenu()) return;
    PopupMenu m;
    m.addItem (1, "RESET");

    const int result = m.show();

    if (result == 1) {
      setPixPosition(0);
      setTimePos(0);
    }
  }

  void mouseDrag(const MouseEvent& e){

    setTopLeftPosition(e.getEventRelativeTo(getParentComponent()).x - getWidth()/2, getY());
    setTimePos(((Sequencer*)m_parent)->getZoomer()->convertPixToTime(getPixPos()));
    m_parent->repaint();
  }

  int getTimePos() {
    return m_timePos;
  }

  void setTimePos(int _t){
    m_timePos = _t;
  }

  void setPixPosition(int _x) {
    setTopLeftPosition(_x - getWidth()/2, m_parent->getViewedPositionY());
  }

  int getPixPos(){
    return (getX() + getWidth()/2);
  }

  void mouseExit(const MouseEvent& /*e*/){
    repaint();
  }

  void mouseEnter(const MouseEvent& /*e*/){
    repaint();
  }

  void moved() {
    if (getX() < 0) {
      setPixPosition(0);
      setTimePos(0);
    }
  	m_engines->setGotoValue(m_parent->getZoomer()->convertPixToTime(getPixPos()));
  }

  bool keyPressed(const KeyPress & key){

    if(key==KeyPress::rightKey) {
      setTopLeftPosition(getX()+1, getY());
      setTimePos( ((Sequencer*)m_parent)->getZoomer()->convertPixToTime(getPixPos()) );
      return true;
    }
    if(key==KeyPress::leftKey) {
      setTopLeftPosition(getX()-1, getY());
      setTimePos( ((Sequencer*)m_parent)->getZoomer()->convertPixToTime(getPixPos()) );
      return true;
    }
    if(key==KeyPress(KeyPress::rightKey, ModifierKeys::shiftModifier, 0)) {
      setTimePos(getTimePos()+10/*ms*/);
      setPixPosition( ((Sequencer*)m_parent)->getZoomer()->convertTimeToPix(getTimePos()) );
      return true;
    }
    if(key==KeyPress(KeyPress::leftKey, ModifierKeys::shiftModifier, 0)) {
      setTimePos(getTimePos()-10/*ms*/);
      setPixPosition( ((Sequencer*)m_parent)->getZoomer()->convertTimeToPix(getTimePos()) );
      return true;
    }

    return false;
  }

};
#endif
