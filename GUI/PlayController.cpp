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
 *  Player.h
 *  Virage
 *
 *  Created by Henry Bernard and Luc Vercellin on 18/05/09.
 *  Copyright 2009 BlueYeti. All rights reserved.
 *
 */
#include "PlayController.h"
#include "Sequencer.h"
#include "MainWindow.h"

PlayController::PlayController(Sequencer* _parent){
	m_parent = _parent;
	m_loop = false;
}

PlayController::~PlayController(){}

bool PlayController::play()
{
	m_parent->letsGoForReading();
	m_parent->startTimer(FPS);
	if (!(m_parent->getEngines()->isRunning()) && !(m_parent->getEngines()->isPaused())) {
		m_parent->getEngines()->play();
	}
	m_parent->sendSynchronousChangeMessage(m_parent);
	m_parent->showCommentFrame(true);

	/*burn trigger passed*/
	m_parent->burnTrigBefore(m_parent->getEngines()->getGotoValue());

	return true;
}

bool PlayController::pause(bool paused) {
	if (m_parent->getEngines()->isRunning()) {
		if (paused != m_parent->getEngines()->isPaused()) {
			m_parent->getEngines()->pause(paused);
		}
	}
}

bool PlayController::stop()
{
	m_parent->getEngines()->stop();
	m_parent->stopTimer();
	m_parent->reinitAfterReading();
	m_parent->sendSynchronousChangeMessage(m_parent);
	m_parent->showCommentFrame(false);

	return false;
}

void PlayController::setMustLoop(bool _wantLoop){

	m_loop = _wantLoop;
	m_parent->getTransportBar()->getLoopButton()->setToggleState(_wantLoop, false);
}

bool PlayController::mustLoop(){

	return m_loop;
}
