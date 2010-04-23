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
 *  Behavior.cpp
 *  Virage
 *
 *  Created by Henry Bernard on 20/02/09.
 *  Copyright 2009 BlueYeti.Fr. All rights reserved.
 *
 
 unsigned int addBox(int boxBeginPos, int boxLength, unsigned int motherId, int maxSceneWidth, int minBound, int maxBound);
 
 et
 
 void changeBoxBounds(unsigned int boxId, int minBound, int maxBound);
 
 
*/
#include "Behavior.h" 
#include "TimeBox.h"
#include "Relation.h"
#include "SequenceOT.h"
#include "Sequencer.h"

Behavior::Behavior(SequenceOT* _concerned, bool _flexible, int flex_start, int flex_stop){
  m_flexible=_flexible;
  m_flexStart = flex_start;
  m_flexStop = flex_stop;
  m_colour = Colours::black;
  m_concerned = _concerned;
  setWantsKeyboardFocus(false);
}

Behavior::~Behavior(){
  deleteAllChildren();
}

void Behavior::setFlexGroup(bool isFlex, int Fs, int Fe, int /*parentWidth*/) {
  setFlexible(isFlex);//retourne si flexible
  setFlexStart(Fs);
  setFlexStop(Fe);
}

/*set*/
void Behavior::setFlexible(bool _flexible){

  //si le composant parent est une boite ou une relation, avec un trigger à la fin, on reste en flexible whatever
  if(m_concerned->isTriggered(2)) {
    m_flexible = true;
    m_concerned->refresh();
  }
  else {
    m_flexible = _flexible;
  }
}


int Behavior::getDuration(){

  return 1;
}

void Behavior::setDuration(int /*ms*/){


}

void Behavior::setFlexStart(int _flexStart){  

  m_flexStart = _flexStart; 
  m_concerned->resized();
  //	if(dynamic_cast<TimeBox*> (m_concerned))
  //		m_concerned->getParentSequencer()->getEngines()->changeBoxBounds(m_concerned->getId(), m_flexStart, m_flexStop != NO_STOP ? m_flexStop : NO_STOP );
}

void Behavior::setFlexStop(int _flexStop){
	
  m_flexStop = _flexStop;
  m_concerned->resized();
		
  //	if(dynamic_cast<TimeBox*> (m_concerned))
  //		m_concerned->getParentSequencer()->getEngines()->changeBoxBounds(m_concerned->getId(), m_flexStart, m_flexStop != NO_STOP ? m_flexStop : NO_STOP );

}

void Behavior::setColour(Colour _b) {  m_colour = _b;  }
/*get*/
bool Behavior::isFlexible(){return m_flexible; }
int Behavior::getFlexStart(){ return m_flexStart;}
int Behavior::getFlexStop() { return m_flexStop;}

void Behavior::paint(Graphics &g){
  g.setColour(m_colour);
	
  int start = m_concerned->getParentSequencer()->getZoomer()->convertTimeToPix(m_flexStart);
  int stop =  m_concerned->getParentSequencer()->getZoomer()->convertTimeToPix(m_flexStop);
	
  if(!m_flexible)	{
    g.drawLine(0, getHeight()-behaviourInfoThickness, getWidth(), getHeight()-behaviourInfoThickness, behaviourInfoThickness);
  }	
  else{
    g.drawLine(0, getHeight()-behaviourInfoThickness, start, getHeight()-behaviourInfoThickness, behaviourInfoThickness);
    const float data[] = { 2.0f, 2.0f};
    g.fillRect(Rectangle(start-behaviourInfoThickness, getHeight()-behaviourInfoThickness*2,  2*behaviourInfoThickness, behaviourInfoThickness*2));
    g.drawDashedLine(start, getHeight()-behaviourInfoThickness, getWidth(), getHeight()-behaviourInfoThickness, data, 2, behaviourInfoThickness);
		
    //crochet au dessus des pointillés
    if(m_flexStop != NO_STOP) {
      //horizontales
      g.drawLine(start-1, getHeight()-3*behaviourInfoThickness, stop, getHeight()-3*behaviourInfoThickness, 2);
      //verticales
      g.drawLine(start, getHeight()-3*behaviourInfoThickness, start-1, getHeight(), 2);
      g.drawLine(stop-1/*epaisseur*/, getHeight()-3*behaviourInfoThickness, stop-1, getHeight(), 2);
    }
  }
}

void Behavior::mouseDown(const MouseEvent &e){
  //getParentComponent()->mouseDown(e.getEventRelativeTo(getParentComponent()));
  if(dynamic_cast<TimeBox*>(m_concerned)) ((TimeBox*)m_concerned)->mouseDown(e);
  toFront(true);
  //permet au séquenceur (en back-ground) de récupérer les sélections de relations
}

void Behavior::focusGained (FocusChangeType /*cause*/)
{
  //	if(dynamic_cast<TimeBox*>(m_concerned)) ((TimeBox*)m_concerned)->focusGained(focusChangedDirectly);
}

void Behavior::focusLost (FocusChangeType /*cause*/)
{
  //	if(dynamic_cast<TimeBox*>(m_concerned)) ((TimeBox*)m_concerned)->focusLost(focusChangedDirectly);
}

void Behavior::changeListenerCallback(void* /*objectThatHasChanged*/){
  /*if(objectThatHasChanged == m_concerned)*/ toFront(true);
}
