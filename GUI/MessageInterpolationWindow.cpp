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
 *  MessageInterpolationWindow.cpp
 *  Virage
 *
 *  Created by Laurent Garnier on 22/04/09.
 *  Copyright 2009 __BlueYeti__. All rights reserved.
 *
 */

#include "MessageInterpolationWindow.h"
#include "TimeBox.h"
#include "InterpolationContentComponent.h"

MessageInterpolationWindow::MessageInterpolationWindow(TimeBox* _concerned) : DocumentWindow ("", s_COLOUR_BOX, DocumentWindow::closeButton | DocumentWindow::minimiseButton, true)
{
	m_concerned = _concerned;

	m_contentComponent = new InterpolationContentComponent();
	setContentComponent(m_contentComponent, true, false);

	setVisible(false);
	setSize(800, 250);
	setTopLeftPosition(200, 200);
	setResizable(true,true);
	setResizeLimits(750, 50, 800, 800);
	setName(_concerned->getName() + " Interpolation");
	setTitleBarTextCentred (true);
	setAlwaysOnTop(true);

}

MessageInterpolationWindow::~MessageInterpolationWindow(){
	setContentComponent(0, true);
}

void MessageInterpolationWindow::init(vector<string> _interpolableMessages, vector<string> _startValues, vector<string> _endValues){
	m_contentComponent->initialise(_interpolableMessages, _startValues, _endValues);
}

TimeBox* MessageInterpolationWindow::getConcerned(){
	return m_concerned;
}

void MessageInterpolationWindow::updateMessagesEditor(int controlPointIndex, vector<string> _messages){
	m_concerned->updateMessagesEditor(controlPointIndex, _messages);
	m_concerned->updateMessagesEditor(controlPointIndex, _messages);
}

void MessageInterpolationWindow::sendInterpolatedMessages(vector<string> addresses, vector<bool> redondances, int frequency){
	getConcerned()->sendInterpolatedMessages(addresses, redondances, frequency);
}

vector<string> MessageInterpolationWindow::getInterpolatedMessages(){
	return getConcerned()->getInterpolatedMessages();
}

int MessageInterpolationWindow::getInterpolationFrequency(){
	return m_contentComponent->getInterpolationFrequency();
}

void MessageInterpolationWindow::setAllInterpolated(bool _areAllInterpolated){
	if(_areAllInterpolated) {
		m_contentComponent->selectAll();
	}
	else
		m_contentComponent->deselectAll();
}

void MessageInterpolationWindow::activeInterpolation(){
	m_contentComponent->sendInterpolatedMessages();	
}

void MessageInterpolationWindow::setTitle(String _title){
	setName(_title + " Interpolation");
}

void MessageInterpolationWindow::setTitleColour(Colour _colour){
	setBackgroundColour(_colour);
}

void MessageInterpolationWindow::closeButtonPressed(){
	setVisible(false);
	getConcerned()->setMessageEditorsEnabled(true);
}
