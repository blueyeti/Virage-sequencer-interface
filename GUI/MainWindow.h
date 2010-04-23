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
#include "includes.h"
#include "MainMenuBar.h"
#include "Sequencer.h"
#include "SequencerFrame.h"
#include "PropertyFrame.h"
#include "MainComponent.h"
#include "SafeComponentGetter.h"


#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

/*!
 * \class MainWindow
 *
 */

class MainWindow  : public DocumentWindow, public ChangeListener, public ChangeBroadcaster
{
private:
	MainComponent* m_contentComp;
	MainMenuBar* menu;
	ApplicationCommandManager* m_cmdManager;
	SafeComponentGetter* m_safeComponentGetter;
	
public:
	MainWindow(ApplicationCommandManager* _commandManager);
	~MainWindow();
	
	/*DocumentWindow*/
	void closeButtonPressed();
	//bool keyPressed	( const KeyPress & 	key);
	void changeListenerCallback(void* objectThatHasChanged);
	
	/*MainWindow*/
	SafeComponentGetter* getSafeComponentGetter();
	NetComponent* getNamespacePanel();
	//MainMenuBar* getMainMenuBar();
	ApplicationCommandManager* getApplicationCommandManager();
	Sequencer* getConduite();
	void conduiteChanged();
	const BorderSize getContentComponentBorder();
	SequencerFrame* getSequencerFrame();
	PropertyFrame* getPropertyFrame();
	TransportBar* getTransportBar();
	void refreshComment();
	bool doBeforeQuit();
	void showCommentFrame(bool x);
	
	/*TreeMap Explorer added code*/
	void manageStretchableLayout(bool isTreeMapUsed);
};

#endif
