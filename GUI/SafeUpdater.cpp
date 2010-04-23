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
 *  Safeupdater.cpp
 *  Virage
 *
 *  Created by Henry Bernard and Luc Vercellin on 23/06/09.
 *  Copyright 2009 BlueYeti. All rights reserved.
 *
 */

#include "SafeUpdater.h"
#include "Sequencer.h"
#include "MainWindow.h"


/******* MESSAGE LISTENER ******/

SafeUpdater::SafeUpdater(Sequencer* _parent){
	
	m_parent = _parent;
}

SafeUpdater::~SafeUpdater(){}

void SafeUpdater::handleMessage (const Message &message){
	switch (message.intParameter1) {
		case 9991/*BOX*/:
			templateReceptionB(message.intParameter2, message.intParameter3) ;
			break;
		case 9992/*TRIGGER*/:
			templateReceptionT(message.intParameter2, message.intParameter3) ;
			break;
		case 9990/*FINISHED*/:
			templateFinished() ;
			break;
		default:
			break;
	}
}


void SafeUpdater::templateReceptionB(int controlPointIndex, int boxId) {
	
	//unpaint fonctions
	m_parent->getBox(boxId)->controlPointDone(controlPointIndex);
	
	switch (controlPointIndex) {
		case 1:
			m_parent->showComments(boxId);			break;
		case 2:
			m_parent->removeComment(boxId);			break;
		default:
			break;
	}

	//paint fonctions
	m_parent->getMainWindow()->getTransportBar()->getLedOut()->setVisible(true);
	m_parent->getBox(boxId)->getProgressBar()->setVisible(m_parent->getBox(boxId)->isTimerRunning());
	m_parent->refreshComment();
	m_parent->repaint();
	m_parent->getMainWindow()->getTransportBar()->getLedOut()->fadeOutComponent(1,0,0,0);
	
	//look for connected relations to change their color
	std::map<unsigned int, bool> *relations = m_parent->getBox(boxId)->getRelationMap();
	std::map<unsigned int, bool>::iterator it = relations->begin();
	while (it != relations->end()) {
		if (controlPointIndex == 2) {
			if (!it->second)
				m_parent->getRelation(it->first)->setOtColour(s_COLOUR_ORANGE);
			else
				m_parent->getRelation(it->first)->setOtColour(Colours::black);
		}
		if (controlPointIndex == 1) {
			if (it->second)
				m_parent->getRelation(it->first)->setOtColour(s_COLOUR_ORANGE);
			else
				m_parent->getRelation(it->first)->setOtColour(Colours::black);
		}
		
		it++;
	}

}

void SafeUpdater::templateReceptionT(int isWaited, int triggerId) {
	
	//unpaint fonctions
	m_parent->getTrigger(triggerId)->waitedTriggerPointCallBack(isWaited==1);
	TimeBox* box = m_parent->getTrigger((unsigned int)triggerId)->getLinkedBox();
	
	if (m_parent->getTrigger(triggerId)->getLinkedCtrlPoint() == LEFT/*    &&       (box->getComment() != String::empty)*/) {
		String comment = String(box->getTitle() + ": " + box->getComment());
		if (isWaited > 0)
			m_parent->appendTriggeredBoxComment(comment);
		else
			m_parent->removeComment(comment);
		box = 0;
	}
	
	//paint fonctions
	m_parent->getMainWindow()->getTransportBar()->getLedIn()->setVisible(true);
	m_parent->refreshComment();//ca plante ici
	m_parent->repaint();
	m_parent->getMainWindow()->getTransportBar()->getLedIn()->fadeOutComponent(1, 0, 0, 0);
}

void SafeUpdater::templateFinished() {

	m_parent->getMainWindow()->getTransportBar()->stop();

	m_parent->repaint();

}
