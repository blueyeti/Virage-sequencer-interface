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
 *  TemporaryBox.cpp
 *  Virage
 *
 *  Created by Henry Bernard on 20/02/09.
 *  Copyright 2009 BlueYeti.Fr. All rights reserved.
 *
 */ 
#include "Sequencer.h"
#include "TemporaryBox.h"

TemporaryBox::TemporaryBox(Sequencer* _parentSequencer) : m_xi(0),m_yi(0),m_xf(0),m_yf(0){
	
	setAlwaysOnTop(true);
	setInterceptsMouseClicks(false, false);
	
	m_parentSequencer = _parentSequencer;
	setBounds(0, 0, _parentSequencer->getWidth(), _parentSequencer->getHeight());
}

TemporaryBox::~TemporaryBox(){
}

void TemporaryBox::paint(Graphics& g) {
	
	g.setColour(Colour::Colour(0.8f,0.3f,0.8f,0.5f));
	g.fillRoundedRectangle(std::min(m_xi, m_xf), std::min(m_yi, m_yf), std::max(m_xi, m_xf)-std::min(m_xi, m_xf), std::max(m_yi, m_yf)-std::min(m_yi, m_yf), 5);
	g.setColour(Colours::white);
	g.drawLine(m_xi, m_yi, m_xf, m_yf, 1);
	g.drawLine(m_xi, m_yf, m_xf, m_yi, 1);
}

void TemporaryBox::setZone(int _xi, int _yi, int _xf, int _yf) {
	
	m_xi = _xi;
	m_yi = _yi;
	m_xf = _xf;
	m_yf = _yf;
}

int TemporaryBox::getDownX() { return m_xi;}
int TemporaryBox::getDownY() { return m_yi;}


void TemporaryBox::setDownX(int _x){m_xi = _x;};
void TemporaryBox::setDownY(int _y){m_yi = _y;};
