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
/***************************** VERSION 0.1 **************************

 including Juce 1.46 version 739
 iScore library
 -> download directly on www.plateforme-virage.org

 */


#include "includes.h"
#include "AllWindows.h"
#include "AppleRemote.h"
#include "MySplashScreen.h"

/*!
 @mainpage Virage
 @author Henry Bernard, Luc Vercellin and Laurent Garnier (<A HREF="http://www.henrybernard.net" TARGET="_blank">www.henrybernard.net</A>)\n
 @author (c) Blue Yeti All rights reserved (<A HREF="http://www.blueyeti.fr" TARGET="_blank">www.blueyeti.fr</A>)\n
 @
 @author made in collaboration with iScore library developers, including Raphaël Marczak.
 */


//==============================================================================
/** This is the application object that is started up when Juce starts. It handles
 the initialisation and shutdown of the whole application.
 */
class JUCEHelloWorldApplication : public JUCEApplication, public ChangeBroadcaster, public ChangeListener
{
	/* Important! NEVER embed objects directly inside your JUCEApplication class! Use
	 ONLY pointers to objects, which you should create during the initialise() method
	 (NOT in the constructor!) and delete in the shutdown() method (NOT in the
	 destructor!)

	 This is because the application object gets created before Juce has been properly
	 initialised, so any embedded objects would also get constructed too soon.
	 */

	MainWindow* VirageWindow;

	ApplicationCommandManager* virageCommandManager;


public:
	//==============================================================================
	JUCEHelloWorldApplication()
	: VirageWindow (0)
	{
		// NEVER do anything in here that could involve any Juce function being called
		// - leave all your startup tasks until the initialise() method.
	}

	~JUCEHelloWorldApplication()
	{
		// NOTHING HERE ! NICHTS HIER !
	}

	//==============================================================================
	void initialise (const String& /*commandLine*/)
	{
#ifndef VDBG
		// MySplashScreen panelIntro; panelIntro.show(String("VIRAGE Beta"),  640/*px*/,  480/*px*/, 1500/*ms*/, true/*shadow*/,/*removeOnMouseClick*/false);

		// AlertWindow m_about(String("Virage Sequencer 0.3"),String("info sur cetter release..."),AlertWindow::InfoIcon, 0);
		// m_about.showNativeDialogBox(String("Virage Sequencer 0.3"),  String("TRANSPORT\n [barre espace] : PLAY puis [GO]\n\nTRIGGER : double click dans barre verte au dessus de la timeline\n\nGOTO\nclick dans légende timeline\n\nM : mute an object"),  false);
#endif

		MouseCursor::showWaitCursor();
		virageCommandManager = new ApplicationCommandManager();

		Rectangle screenRect = Desktop::getInstance().getMainMonitorArea();

		// just create the main window...
		/* en dernier */
		VirageWindow = new MainWindow(virageCommandManager);
		VirageWindow->addChangeListener(this);//quitter sur close

		virageCommandManager->registerAllCommandsForTarget(dynamic_cast<ApplicationCommandTarget*>(VirageWindow->getContentComponent()));//s'assure de récupérer les actions sur la barre de menu (t donc d'afficher les menus déroulants)

		/*adpate la taille de la fenetre à l'écran*/VirageWindow->setBounds (10, 50, screenRect.getWidth()-20, screenRect.getHeight()-50);
		VirageWindow->setUsingNativeTitleBar(true);

		/*  ..and now return, which will fall into to the main event
		 dispatch loop, and this will run until something calls
		 JUCEAppliction::quit().
		 In this case, JUCEAppliction::quit() will be called by the
		 hello world window being clicked.
		 */
	}

	/** Called when the operating system is trying to close the application.

	 The default implementation of this method is to call quit(), but it may
	 be overloaded to ignore the request or do some other special behaviour
	 instead. For example, you might want to offer the user the chance to save
	 their changes before quitting, and give them the chance to cancel.

	 If you want to send a quit signal to your app, this is the correct method
	 to call, because it means that requests that come from the system get handled
	 in the same way as those from your own application code. So e.g. you'd
	 call this method from a "quit" item on a menu bar.
	 */
	void changeListenerCallback(void* objectThatHasChanged) {//quitter sur close de VirageWindow
		if(objectThatHasChanged != VirageWindow) return;//normalement n'arrive jms
		//	systemRequestedQuit();

	}
	void systemRequestedQuit(){

		if (VirageWindow->doBeforeQuit()) {
			quit();
		}
	}

	void shutdown()
	{
		// clear up
		if (VirageWindow != 0) {
			delete VirageWindow;
		}
	}

	//==============================================================================
	const String getApplicationName()
	{
		return T("Virage");
	}

	const String getApplicationVersion()
	{
		return T("0.3");
	}

	bool moreThanOneInstanceAllowed()
	{
		return true;
	}

	void anotherInstanceStarted (const String& /*commandLine*/)
	{
	}
};


//==============================================================================
// This macro creates the application's main() function..
START_JUCE_APPLICATION (JUCEHelloWorldApplication)
