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
 *  SequencerMap.cpp
 *  Virage
 *
 *  Created by Henry Bernard and Luc Vercellin on 20/04/09.
 *  Copyright 2009 BlueYeti.Fr. All rights reserved.
 *
 */

#include "SequencerMap.h"
#include "Sequencer.h"
#include "TimeBox.h"


SequencerMap::SequencerMap(Sequencer* _sequencer) {

	m_sequencer = _sequencer;
	addAndMakeVisible(m_viseur = new Viseur(m_sequencer));
}

SequencerMap::~SequencerMap(){
  if (m_viseur) {
    deleteAndZero( m_viseur );
  }
}

void
SequencerMap::paint(Graphics& g) {

	g.fillAll(Colours::lightgrey);
	map<unsigned int, TimeBox*> boxList = *(m_sequencer->getBoxMap());
	map<unsigned int, TimeBox*>::iterator it = boxList.begin();
	while(it != boxList.end()) {

		TimeBox* box = (it->second);
		float x = box->getX() * 1.0f * getWidth() / m_sequencer->getWidth();
		float y = box->getY() * 1.0f * getHeight() / m_sequencer->getHeight();
		float w = 1.0f * box->getWidth() * getWidth() / m_sequencer->getWidth();
		float h = 1.0f * box->getHeight() * getHeight() / m_sequencer->getHeight();

		g.setColour(Colours::black);
		g.setOpacity(0.5);
		g.drawRect((int)x,(int)y,(int)w,(int)h,1);
		g.setColour(box->getTitleColour());
		g.fillRect((int)x+1,(int)y+1,(int)w-2,(int)h-2);
		it++;
	}

	/*headplay - goto*/
	g.setColour(Colours::red);
	int hx =  1.0f * m_sequencer->getHeadPlayPos() / m_sequencer->getWidth() * getWidth();
	g.drawLine(hx, 0, hx, getHeight(), 1);
}

void SequencerMap::resized() {
	m_viseur->setBounds(m_viseur->getX(), 0, m_viseur->getWidth(), getHeight() );//a revoir
}

Viseur* SequencerMap::getViseur(){
	return m_viseur;
};

void SequencerMap::mouseDown(const MouseEvent& e) {

	//passe l'evenement au viseur pour qu'il puisse ensuite zoomer
	m_viseur->mouseDown(e.getEventRelativeTo(m_viseur));
	m_viseur->setTopLeftPosition( e.x - m_viseur->getWidth()/2, 0);
}


void SequencerMap::mouseWheelMove(const MouseEvent& /*e*/, float wheelIncrementX, float /*wheelIncrementY*/)
{
	getViseur()->setTopLeftPosition(getViseur()->getX() - signof( wheelIncrementX),
									getViseur()->getY() );//- signof( wheelIncrementY));
}
