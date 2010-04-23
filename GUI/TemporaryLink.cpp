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
 *  TemporaryLink.cpp
 *  Virage
 *
 *  Created by Henry Bernard on 20/02/09.
 *  Copyright 2009 BlueYeti.Fr. All rights reserved.
 *
 */ 
#include "Sequencer.h"
#include "TimeBox.h"
#include "Trigger.h"

#include "TemporaryLink.h"
#include <iostream>

TemporaryLink::TemporaryLink(Sequencer* _parentSequencer, bool trigFromBox) : m_colour(s_COLOUR_UNSELECTION) {
	
	m_trigFromBox = trigFromBox;
	
	setAlwaysOnTop(true);
	setInterceptsMouseClicks(false, false);
	
	m_parentSequencer = _parentSequencer;
	m_originBox = 0;
	m_originTrigger = 0;
	m_cporigin = 0;
};

TemporaryLink::~TemporaryLink(){
};

void TemporaryLink::paint(Graphics& g) {
	
	//pointeur
	g.setColour(m_colour);
	g.fillPath(linePath);
	
	//flèches de connection
	map<unsigned int, TimeBox*>::iterator it = m_parentSequencer->getBoxMap()->begin();
	while ( it != m_parentSequencer->getBoxMap()->end()) {
		
		TimeBox* oneBox = it->second;
		if(m_originBox != 0) {	
			std::map<unsigned int, unsigned int> connectedMap1 = m_originBox->getConnectedBoxesMap(getCPOrigin(), 1);
			std::map<unsigned int, unsigned int> connectedMap2 = m_originBox->getConnectedBoxesMap(getCPOrigin(), 2);
			
			/*on sort d'abord la boite d'origine*/
			if(oneBox != m_originBox/*boite depuis laquelle le connector est tiré*/) {
				
				unsigned int boxMapId = oneBox->getId();
				//on sort les boite déjà connecté
				if( connectedMap2.find(boxMapId)->second != 2 ) {
					int y = oneBox->getPlugY();
					int x2 = oneBox->getPlugEndX();
					g.setColour(Colours::black); g.fillEllipse(x2+1, y-10, 20, 20);
					g.setColour(Colours::white); g.drawArrow(x2+17, y, x2+2, y, 4, 8, 5);
				}
				if( connectedMap1.find(boxMapId)->second != 1 ) {
					int y = oneBox->getPlugY();
					int x1 = oneBox->getPlugStartX();
					g.setColour(Colours::black); g.fillEllipse(x1-19, y-10, 20, 20);
					g.setColour(Colours::white); g.drawArrow(x1-15, y, x1, y, 4, 8, 5);
				}
			}
		}
		it++;
	}
	
	if(m_originTrigger != 0){
	
		map<unsigned int, TimeBox*>::iterator it = m_parentSequencer->getBoxMap()->begin();
		while ( it != m_parentSequencer->getBoxMap()->end()) {
			
			TimeBox* oneBox = it->second;
			
			//if( le CP de la boite n'est pas deja relie a un trigger ) {
			int y = oneBox->getPlugY();
			int x1 = oneBox->getPlugStartX();
			
			g.setColour(Colours::black.withAlpha(0.75f));
			g.fillEllipse(x1-20, y-10, 20, 20);
			g.setColour(Colours::white.withAlpha(0.75f));
			g.drawArrow(x1-13, y, x1, y, 4, 8, 5);
			int x2 = oneBox->getPlugEndX();
			g.setColour(Colours::black.withAlpha(0.75f));
			g.fillEllipse(x2+1, y-10, 20, 20);
			g.setColour(Colours::white.withAlpha(0.75f));
			g.drawArrow(x2+17, y, x2+2, y, 4, 8, 5);
			//}
			
			it++;
		}
	}	
	
	
	//text "after/before"
	if( ! m_trigFromBox) return; 
	bool beforeConstraints = (xf >= xi);
	g.setColour(m_colour);
	g.fillRoundedRectangle(xf + (beforeConstraints? 5 : -70), yf, 65, 25, 3);
	g.setColour(m_colour.contrasting(1.0f));
	g.drawText( beforeConstraints ? String("before") : String("after"), xf+ (beforeConstraints ? 5 : -70), yf, 65, 25, Justification::centred, false); 
	
}

void TemporaryLink::setZone(int _xi, int _yi, int _xf, int _yf) {
	
	linePath.clear();
	linePath.addEllipse(_xi-10, _yi-10, 20, 20);
	linePath.addArrow ((float)_xi,(float) _yi, (float)_xf, (float)_yf, 4, 12, 15);
	
	xf = _xf;
	yf = _yf;
}

int TemporaryLink::getDownX() { return xi;}
int TemporaryLink::getDownY() { return yi;}


void TemporaryLink::setDownX(int _x){xi = _x;};
void TemporaryLink::setDownY(int _y){yi = _y;};



TimeBox* TemporaryLink::getBoxOrigin(){
	return m_originBox;
};

unsigned int TemporaryLink::getCPOrigin(){
	return m_cporigin;
};

void TemporaryLink::setBoxOrigin(TimeBox* _box, unsigned int _ctrlPtId){
	m_originBox = _box;
	m_originTrigger = 0;
	m_cporigin = _ctrlPtId;
}

void TemporaryLink::mouseDrag(const MouseEvent &e) {
	
		
	//getParentComponent()->mouseDown(e.getEventRelativeTo(getParentComponent()));
	
}
Trigger* TemporaryLink::getTriggerOrigin(){
	return m_originTrigger;
};

void TemporaryLink::setTriggerOrigin(Trigger* _trigger){
	m_originTrigger = _trigger;
	m_originBox = 0;
	
}


void TemporaryLink::blink(bool _do) {

	/*Actually, it is highlighting rather than blinking*/
	
	m_colour = s_COLOUR_UNSELECTION.withMultipliedBrightness( _do ? 90.f : 1.0f);


}