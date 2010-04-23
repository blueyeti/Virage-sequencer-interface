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
 *  MainWindow.cpp
 *  Virage
 *
 *  Created by Henry Bernard and Luc Vercellin on 21/09/08.
 *  Copyright 2008 by Blue Yeti. All rights reserved.
 */

#include "MainWindow.h"

using namespace std;

MainWindow::MainWindow(ApplicationCommandManager* _commandManager)
: DocumentWindow (s_DEFAULT_PROJECT_NAME, Colours::black, DocumentWindow::allButtons, true)
{

	setContentComponent(m_contentComp = new MainComponent(_commandManager));
	setVisible(true);
	setResizable(true, true);

	m_safeComponentGetter = new SafeComponentGetter(m_contentComp);
	m_contentComp->setSCG(m_safeComponentGetter);

	setMenuBar(m_contentComp/*MenuBarModel*/);

	#if JUCE_MAC
		setMenuBar(0);
		MenuBarModel::setMacMainMenu((MenuBarModel*)m_contentComp);//MAC only : barre de menu OSX-native
	#endif

	
	/*CommandManager*/
	m_cmdManager = _commandManager;
	m_cmdManager->setFirstCommandTarget(m_contentComp);
	addKeyListener (_commandManager->getKeyMappings());

	/*MenuBar*/
	m_contentComp->setApplicationCommandManagerToWatch(m_cmdManager); // tells our menu bar model that it should watch this command manager for changes, and send change messages accordingly.
	menu = ((MainMenuBar*)m_contentComp);
	menu->setConduiteToSave(getConduite());
	menu->setMainWindowToBroadcast(this);
	getConduite()->setParentWindow(this);
	
}

MainWindow::~MainWindow(){
//	setMenuBar(0);
//	setContentComponent(0, true);
//	deleteAndZero(m_cmdManager);
//	if (menu) {
//		delete menu;
//	}
}

SafeComponentGetter* MainWindow::getSafeComponentGetter(){
	return m_safeComponentGetter;
}

void MainWindow::closeButtonPressed(){
	menu->fermer();
	setVisible(false);
	sendSynchronousChangeMessage(this);
}

ApplicationCommandManager* MainWindow::getApplicationCommandManager(){
	return m_cmdManager;
}

void MainWindow::changeListenerCallback(void* objectThatHasChanged){
	if(getConduite() == objectThatHasChanged) {
		conduiteChanged();

		if(getConduite()->getEngines()->isRunning()) {
			return;
		}

		getSafeComponentGetter()->getTransportBar()->setChrono(getConduite()->getZoomer()->convertPixToTime(getConduite()->getHeadPlayPos()));
	}
}

Sequencer* MainWindow::getConduite(){ 
	return ((MainComponent*)getContentComponent())->getMiddleFrame()->getSequencerFrame()->getConduite();
}

void MainWindow::conduiteChanged() { 
	menu->changed(getConduite());  
}

bool MainWindow::doBeforeQuit(){
	return menu->fermer();
}

void MainWindow::showCommentFrame(bool x){
	if(x) {
		getSafeComponentGetter()->getMiddleFrame()->getCommentButton()->triggerClick();
	} 
	else {
		getSafeComponentGetter()->getMiddleFrame()->getOtPropButton()->triggerClick();
	}
}

void MainWindow::manageStretchableLayout(bool isTreeMapUsed) {
	m_contentComp->getMiddleFrame()->manageStretchableLayout(isTreeMapUsed);
}

NetComponent* MainWindow::getNamespacePanel(){
	return m_contentComp->getMiddleFrame()->getNamespacePanel();
}

void MainWindow::refreshComment() {	m_contentComp->getMiddleFrame()->refreshComment(); }

PropertyFrame* MainWindow::getPropertyFrame() {	return m_contentComp->getMiddleFrame()->getPropertyFrame(); }

SequencerFrame* MainWindow::getSequencerFrame() { return m_contentComp->getMiddleFrame()->getSequencerFrame(); }

TransportBar* MainWindow::getTransportBar() { return m_contentComp->getTransportBar(); }

const BorderSize MainWindow::getContentComponentBorder() { return BorderSize (getTitleBarHeight(), 0, 18, 0); }

