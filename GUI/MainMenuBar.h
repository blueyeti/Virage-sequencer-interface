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
 *  MainMenuBar.h
 *  Virage
 *
 *  Created by Henry Bernard and Luc Vercellin on 18/09/08. Copyright 2008 by Blue Yeti. All rights reserved.
 *
 */

#include "includes.h"
#include "Sequencer.h"
#include "Relation.h"

class MainWindow;

#ifndef _MAINMENUBAR_H_
#define _MAINMENUBAR_H_
/*!
 * \class MainMenuBar
 *
 * MainMenuBar.class manages the main menu-bar lists and commands for the main window.
 * MainMenuBar must be instanciated one time for the whole application.
 * Normally in MainWindow.class.
 * Thus, in MacOS mode : it stands on the screen top bar, in Window mode : it stands on the main window top bar.
 * 
 * MainMenuBar is derived from juce::MenuBarModel (which creates menu items) and juce::ApplicationCommandTarget (which attaches actions to menu)
 *
 */
class MainMenuBar : 
public MenuBarModel,
public ApplicationCommandTarget,
public FileBasedDocument,
public UndoManager
{

private:
	Sequencer* m_conduiteToSave;
	MainWindow* m_mainwindow;
	File lastDocumentOpened;
	
	friend class MainWindow;
	
public:
	MainMenuBar();
	~MainMenuBar();
	
	void setConduiteToSave(Sequencer* _conduite);
	void setMainWindowToBroadcast(MainWindow* _mainWindow);
	void changed(Sequencer *conduite);
	
	
protected:
	
	ApplicationCommandManager* iScoreCommandManager;
	
	TooltipWindow *toolTipWindow;
	
	/*!
	 * All of menu items
	 */
	enum CommandIDs {
		Nouveau = 0x1031,
		Ouvrir = 0x1000,
		Recent = 0x1001,
		Fermer = 0x1002,
		Enregistrer = 0x1011,
		EnregistrerSous = 0x1012,
		Infos = 0x1030,
		
		Undo = 0x2000,
		Couper = 0x2001,
		Copier = 0x2002,
		Coller = 0x2003,
		Effacer = 0x2004,
		Dupliquer = 0x2005,
		SelectionnerTout = 0x2006,
		
		ModesAffichage = 0x3000,
		Zoom = 0x3001,
		BoiteAOutils = 0x3002,
		Palettes = 0x3003,
		DeplierExplorer = 0x3004,
		
		Objet = 0x4000,
		Contraintes = 0x4001,
		Grouper = 0x4002,
		Degrouper = 0x4003,
		Encapsuler = 0x4004,
		Verouiller = 0x4005,
		ToutDeverouiller= 0x4006,
		
		CaptureParametres = 0x5000,
		GestionPlans = 0x5001,
		
		Namespace = 0x6005,
		Sequence = 0x6006,
		Comments = 0x6007,
		ToutAfficher = 0x6008,
		Inspecteur = 0x6009,
		
		SearchHelp = 0x7000,
		Help = 0x7001,
		
		Play = 0x8000,
		Stop = 0x8001, 
		Loop = 0x8002, 
		Next = 0x8003,
		Pause = 0x8004,
		
		Preferences = 0x0099,
		About = 0x0098
    };
	
	//From Menu...
	/*!
	 * JUCE
	 * This method must return a list of the names of the menus.
	 */
	const StringArray getMenuBarNames();
	/*!
	 * JUCE
	 * This is called when a menu item has been clicked on.
	 */
	void menuItemSelected (int menuItemID, int topLevelMenuIndex);
	
	//const PopupMenu MainMenuBar::getMenuForIndex (int menuIndex, const String& menuName);
	
	
	//...do it :
	/*JUCE*/ApplicationCommandTarget* getNextCommandTarget();
	/*JUCE*/void getAllCommands (Array <CommandID>& commands);
	/*JUCE*/void getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result);
	/*JUCE*/bool perform (const InvocationInfo& info);
	
	//fonction affectée aux éléments du menu
	void ouvrir();
	void nouveau();
	bool fermer();/*FALSE si Cancel*/
	void importer();
	void exporter();	
	
	//NE PAS CHANGER !!! OVERWRITE de fileBasedDocument
	/*JUCE*/const String loadDocument(const File & file);
	/*JUCE*/const String saveDocument(const File & file);
	/*JUCE*/const String getDocumentTitle();
	/*JUCE*/const File getLastDocumentOpened();
	/*JUCE*/void setLastDocumentOpened(const File & file);
	
};

#endif