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
 *  MainMenuBar.cpp
 *  Virage
 *
 *  Created by Henry Bernard and Luc Vercellin on 18/09/08.
 *  Copyright 2008 by Blue Yeti. All rights reserved.
 *
 */

#include "MainMenuBar.h"
#include "MainWindow.h"
#include <fstream>
#include <map>
#include <stdlib.h>


using namespace std;


MainMenuBar::MainMenuBar() :
FileBasedDocument(String(".virage"), String("*.virage"), String("Choose a Virage file to open..."), String("Choose a Virage file to save as...")),
UndoManager() {
	
	m_conduiteToSave = 0;
	m_mainwindow = 0;
	lastDocumentOpened = File::nonexistent;
}

MainMenuBar::~MainMenuBar() {
}


// Concrétisation de l'héritage de ApplicationCommandTarget
//==============================================================================
// The following methods implement the ApplicationCommandTarget interface, allowing
// this window to publish a set of actions it can perform, and which can be mapped
// onto menus, keypresses, etc.


ApplicationCommandTarget* MainMenuBar::getNextCommandTarget()
{
	// this will return the next parent component that is an ApplicationCommandTarget (in this
	// case, there probably isn't one, but it's best to use this method in your own apps).
	return findFirstTargetParentComponent();
}

void MainMenuBar::getAllCommands (Array <CommandID>& commands)//domaine des possibles
{
	// this returns the set of all commands that this target can perform..
	const CommandID ids[] = {
		
		/*RACINE POUR MAC*/
		About,
//		Preferences,
		
		/*SEQUENCE - Fichier*/
		Nouveau, Ouvrir, 
//		Recent, Fermer,
		Enregistrer, EnregistrerSous,
		
		/*EXECUTION*/
		Play, Stop, Pause, Loop, Next,
		
		/*EDITION*/
//		Undo, 
		Couper, Copier, Coller, Effacer,
		Dupliquer, SelectionnerTout,
		
		/*PRESENTATION*/
//		ModesAffichage,
//		Zoom,
//		BoiteAOutils,
//		Palettes,
		DeplierExplorer,
		
		/*INSERTION*/
//		Objet,Contraintes,Grouper,Degrouper,Verouiller,ToutDeverouiller,
		
		/*MODIFIER*/
//		CaptureParametres,
//		GestionPlans,
		
		/*FENETRES*/
//		Comments,
//		Namespace,
//		Sequence,
//		ToutAfficher,Inspecteur,
		
		/*Help*/
		SearchHelp,Help,
		
		/*???*/
//		Encapsuler
		
	};
	commands.addArray (ids, numElementsInArray (ids));
}

// This method is used when something needs to find out the details about one of the commands
// that this object can perform..
void MainMenuBar::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
	const String demosCategory (T("test"));
	
	switch (commandID)
    {
		case About:
			result.setInfo(T("About"),T("About"), demosCategory, 0);
			break;
			//		case Preferences:
			//			result.setInfo(T("Preferences"),T("Preferences"), demosCategory, 0);
			//			result.addDefaultKeypress (T(','), ModifierKeys::commandModifier);
			//			result.setActive(false);
			//			break;
		case Nouveau:
			result.setInfo (T("Nouveau"), T("Nouveau"), demosCategory, 0);
			result.addDefaultKeypress (T('n'), ModifierKeys::commandModifier);
			break;
		case Ouvrir:
			result.setInfo (T("Ouvrir"), T("Ouvrir"), demosCategory, 0);
			result.addDefaultKeypress (T('o'), ModifierKeys::commandModifier);
			break;
//		case Recent:
//			result.setInfo (T("Recent"), T("Recent"), demosCategory, 0);
//			result.addDefaultKeypress (T('0'), ModifierKeys::commandModifier);
//			result.setActive(false);break;
//		case Fermer:
//			result.setInfo (T("Fermer"), T("Fermer"), demosCategory, 0);
//			result.addDefaultKeypress (T('W'), ModifierKeys::commandModifier );
//			result.setActive(m_mainwindow->isVisible());
//			break;
		case Couper:
			result.setInfo (T("Couper"), T("Couper"), demosCategory, 0);
			result.addDefaultKeypress (T('x'), ModifierKeys::commandModifier);
			result.setActive(false);
			break;
		case Copier:
			result.setInfo (T("Copier"), T("Copier"), demosCategory, 0);
			result.addDefaultKeypress (T('c'), ModifierKeys::commandModifier);
			result.setActive(m_mainwindow->isVisible());	
			break;
		case Coller:
			result.setInfo (T("Coller"), T("Coller"), demosCategory, 0);
			result.addDefaultKeypress (T('v'), ModifierKeys::commandModifier);
			result.setActive(m_mainwindow->isVisible());
			break;
		case Enregistrer:
			result.setInfo (T("Enregistrer"), T("Enregistrer"), demosCategory, 0);
			result.addDefaultKeypress (T('s'), ModifierKeys::commandModifier);
			result.setActive(m_mainwindow->isVisible());
			break;
		case EnregistrerSous:
			result.setInfo (T("Enregistrer Sous"), T("Enregistrer Sous"), demosCategory, 0);
			result.addDefaultKeypress (T('s'), ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
			result.setActive(m_mainwindow->isVisible());
			break;
//		case Undo:
//			result.setInfo (T("empecher jassert3"), T("empecher jassert3"), demosCategory, 0);
//			result.addDefaultKeypress (T('z'), ModifierKeys::commandModifier);
//			result.setActive(false);
//			break;
		case Effacer:
			result.setInfo (T("Effacer"), T("Effacer"), demosCategory, 0);
			result.addDefaultKeypress (KeyPress::deleteKey, 0);
			result.setActive(m_mainwindow->isVisible());
			break;
		case Dupliquer:
			result.setInfo (T("Dupliquer"), T("Dupliquer"), demosCategory, 0);
			result.addDefaultKeypress (T('d'), ModifierKeys::commandModifier);
			result.setActive(m_mainwindow->isVisible());
			break;
		case SelectionnerTout:
			result.setInfo (T("SelectionnerTout"), T("SelectionnerTout"), demosCategory, 0);
			result.addDefaultKeypress (T('a'), ModifierKeys::commandModifier);
			result.setActive(m_mainwindow->isVisible());
			break;
//		case ModesAffichage:
//			result.setInfo (T("empecher jassert9"), T("empecher jassert9"), demosCategory, 0);
//			result.addDefaultKeypress (T('0'), 0);
//			result.setActive(false);
//			break;
//		case Zoom:
//			result.setInfo (T("empecher jassert11"), T("empecher jassert11"), demosCategory, 0);
//			result.addDefaultKeypress (T('0'), 0);
//			result.setActive(false);
//			break;
//		case Objet:
//			result.setInfo (T("empecher jassert12"), T("empecher jassert12"), demosCategory, 0);
//			result.addDefaultKeypress (T('0'), 0);
//			result.setActive(false);
//			break;
//		case Contraintes:
//			result.setInfo (T("empecher jassert13"), T("empecher jassert13"), demosCategory, 0);
//			result.addDefaultKeypress (T('0'), 0);
//			result.setActive(false);
//			break;
//		case Grouper:
//			result.setInfo (T("empecher jassert14"), T("empecher jassert14"), demosCategory, 0);
//			result.addDefaultKeypress (T('0'), 0);
//			result.setActive(false);
//			break;
//		case Degrouper:
//			result.setInfo (T("empecher jassert15"), T("empecher jassert15"), demosCategory, 0);
//			result.addDefaultKeypress (T('0'), 0);
//			result.setActive(false);
//			break;
//		case Encapsuler:
//			result.setInfo (T("empecher jassert16"), T("empecher jassert16"), demosCategory, 0);
//			result.addDefaultKeypress (T('0'), 0);
//			result.setActive(false);
//			break;
//		case Verouiller:
//			result.setInfo (T("empecher jassert17"), T("empecher jassert17"), demosCategory, 0);
//			result.addDefaultKeypress (T('0'), 0);
//			result.setActive(false);
//			break;
//		case ToutDeverouiller:
//			result.setInfo (T("empecher jassert18"), T("empecher jassert18"), demosCategory, 0);
//			result.addDefaultKeypress (T('0'), 0);
//			result.setActive(false);
//			break;
		case SearchHelp:
			result.setInfo (T("SearchHelp"), T("SearchHelp"), demosCategory, 0);
			result.addDefaultKeypress (T('0'), 0);
			result.setActive(false);
			break;
		case Help:
			result.setInfo (T("Help"), T("Help"), demosCategory, 0);
			result.addDefaultKeypress (T('0'), 0);
			result.setActive(false);
			break;
//		case CaptureParametres:
//			result.setInfo (T("empecher jassert23"), T("empecher jassert23"), demosCategory, 0);
//			result.addDefaultKeypress (T('0'), 0);
//			result.setActive(false);
//			break;
//		case GestionPlans:
//			result.setInfo (T("empecher jasst"), T("empecher jrt"), demosCategory, 0);
//			result.addDefaultKeypress (T('0'), 0);
//			result.setActive(false);
//			break;
//		case BoiteAOutils:
//			result.setInfo (T("empechr jassert"), T("empechssert"), demosCategory, 0);
//			result.addDefaultKeypress (T('0'), 0);
//			result.setActive(false);
//			break;
//		case Palettes:
//			result.setInfo (T("emper jassert"), T("emassert"), demosCategory, 0);
//			result.addDefaultKeypress (T('0'), 0);
//			result.setActive(false);
//			break;
		case ToutAfficher:
			result.setInfo (T("Tout afficher"), T("Tout afficher"), demosCategory, 0);
			result.addDefaultKeypress (T('r'), ModifierKeys::commandModifier);
			break;
		case DeplierExplorer:
			result.setInfo (T("Deplier Explorer"), T("Deplier Explorer"), demosCategory, 0);
			result.addDefaultKeypress (T('<'), ModifierKeys::commandModifier);
			break;
//		case Inspecteur:
//			result.setInfo (T("Inspecteur"), T("Inspecteur"), demosCategory, 0);
//			result.setActive(false);
//			break;
		case Play:
			result.setInfo (T("Play"), T("Play"), demosCategory, 0);
			result.addDefaultKeypress (KeyPress::spaceKey, 0);
			result.setActive(m_mainwindow->isVisible());	
			break;
		case Stop:
			result.setInfo (T("Stop"), T("Stop"), demosCategory, 0);
			result.addDefaultKeypress (KeyPress::returnKey, 0);
			result.setActive(m_mainwindow->isVisible());
			break;
		case Pause:
			result.setInfo (T("Pause"), T("Pause"), demosCategory, 0);
			result.addDefaultKeypress (T('p'), 0);
			result.setActive(m_mainwindow->isVisible());
			break;
		case Loop:
			result.setInfo (T("loop mode"), T("loop mode"), demosCategory, 0);
			result.setTicked ( m_conduiteToSave->getController()->mustLoop() );
			result.setActive(m_mainwindow->isVisible());	
			result.addDefaultKeypress (T('l'), ModifierKeys::commandModifier);
			break;
		case Next:
			result.setInfo (T("Next"), T("Next"), demosCategory, 0);
			result.setActive(m_mainwindow->isVisible());
			result.addDefaultKeypress (T('g'),0);
			break;
		default:
			result.setActive(false);
			break;
    }
}

bool MainMenuBar::perform (const InvocationInfo& info)//action...
{
	AlertWindow m_about(String("Virage Sequencer 0.3"),s_About_Text,AlertWindow::InfoIcon, 0);
	
	switch (info.commandID)
    {
		case About:
			m_about.showNativeDialogBox(String("Virage Sequencer 0.3"),  s_About_Text,  false);
			//m_about.setBounds (Desktop::getInstance().getMainMonitorArea().getWidth()/4, Desktop::getInstance().getMainMonitorArea().getHeight()/4, Desktop::getInstance().getMainMonitorArea().getWidth()/2, Desktop::getInstance().getMainMonitorArea().getHeight()/2);
			break;
		case Nouveau:
			nouveau();
			break;
		case Ouvrir:
			ouvrir();
			break;
		case EnregistrerSous:
			saveAsInteractive(true);
			break;
		case Enregistrer:
			save(true,true);
			break;
		case Fermer:
			fermer();
			break;
		case Dupliquer:
			m_mainwindow->getConduite()->keyPressed(KeyPress (T('d'), ModifierKeys::commandModifier, 0));
			break;
		case Copier :
			m_mainwindow->getConduite()->keyPressed(KeyPress (T('c'), ModifierKeys::commandModifier, 0));
			break;
		case Coller:
			m_mainwindow->getConduite()->keyPressed(KeyPress (T('v'), ModifierKeys::commandModifier, 0));
			break;
		case SelectionnerTout:
			m_conduiteToSave->selectAll();
			break;
		case Effacer:
			m_conduiteToSave->keyPressed(KeyPress::deleteKey);
			break;
		case Loop:
			m_conduiteToSave->getTransportBar()->getLoopButton()->triggerClick();
			break;
		case Undo:
			undo();
			break;
		case Play:
			if(!m_conduiteToSave->getEngines()->isRunning())
				m_conduiteToSave->getTransportBar()->getPlayButton()->triggerClick();
			else
				m_conduiteToSave->getTransportBar()->getNextButton()->triggerClick();
			break;
		case Stop:
			if(m_conduiteToSave->getEngines()->isRunning()) m_conduiteToSave->getTransportBar()->getStopButton()->triggerClick();
			break;
		case Pause:
			m_conduiteToSave->getTransportBar()->pause(false);
			break;
		case Next:
			if(m_conduiteToSave->getEngines()->isRunning())
				m_conduiteToSave->getTransportBar()->getNextButton()->triggerClick();
			break;
		case Comments:
			break;
		case Namespace:
			m_mainwindow->getNamespacePanel()->setVisible(!m_mainwindow->getNamespacePanel()->isVisible());
			m_mainwindow->getNamespacePanel()->toFront(true);
			break;
		case Sequence:
			m_mainwindow->setVisible(!m_mainwindow->isVisible());
			m_mainwindow->toFront(true);
			break;
		case Inspecteur:
			m_mainwindow->getPropertyFrame()->setVisible(!m_mainwindow->getPropertyFrame()->isVisible());
			m_mainwindow->getContentComponent()->resized();// repaint();
			break;
		case ToutAfficher:
			m_mainwindow->setVisible(true);
			m_mainwindow->toFront(true);
			m_mainwindow->getNamespacePanel()->setVisible(true);
			m_mainwindow->getNamespacePanel()->toFront(true);
			break;
		case DeplierExplorer:
			m_mainwindow->getSafeComponentGetter()->getMiddleFrame()->manageStretchableLayout(true);
			
		default:
			break;
	}
	
	return true;
}


// Concrétisation de l'héritage de MenuBarModel
//==============================================================================

const StringArray MainMenuBar::getMenuBarNames() {
	const tchar* const names[] = { T("Projet"), T("Edition"), /*T("Presentation"), T("Insertion"), T("Modifier"),T("Fenetres"),*/T("Help"), 0 };
	return StringArray ((const tchar**) names);
}

void MainMenuBar::menuItemSelected (int /*menuItemID*/, int /*topLevelMenuIndex*/) {
	// all our menu items are invoked automatically as commands, so no need to do
	// anything in this callback
}

//======== MENU FONCTIONS ====================================================

void MainMenuBar::ouvrir() {
	
	m_mainwindow->getSafeComponentGetter()->getPropertyFrame()->setContentComponent(m_mainwindow->getSafeComponentGetter()->getNetPanel());//avoid property panel refreshing bug
	
	fermer();
	
	loadFromUserSpecifiedFile(true);
	
	m_mainwindow->setVisible(true);
}

void MainMenuBar::nouveau() {
	
	m_mainwindow->getSafeComponentGetter()->getPropertyFrame()->setContentComponent(m_mainwindow->getSafeComponentGetter()->getNetPanel());//avoid property panel refreshing bug
	
	fermer();
	
	m_mainwindow->setVisible(true);
	
}

bool MainMenuBar::fermer() {
	
	if(saveIfNeededAndUserAgrees() == userCancelledSave) return false;
	
	clearUndoHistory ();
	
	m_conduiteToSave->clearAllOt();
	
	m_conduiteToSave->getEngines()->reset();
	m_conduiteToSave->getEngines()->addCrossingCtrlPointCallback(&controlPointCrossedCallBack);
    m_conduiteToSave->getEngines()->addCrossingTrgPointCallback(&triggeredTPCallBack);
	m_conduiteToSave->getEngines()->addExecutionFinishedCallback(&executionFinishedCallBack);
	m_conduiteToSave->getEngines()->addEnginesNetworkUpdateCallback(&transportBarCallBack);
	
	setChangedFlag(false);
	m_mainwindow->setName(s_DEFAULT_PROJECT_NAME);
	lastDocumentOpened = String::empty;
	
	setFile(File::nonexistent);
	m_mainwindow->setName(getDocumentTitle());
	m_conduiteToSave->getOtManager()->setBoxCounter(0);
	m_mainwindow->setVisible(true);
	
	return true;
}

/*************************** FileBasedDocument overwritted-must methods *******************/

const String MainMenuBar::loadDocument(const File & file) {
	MouseCursor::showWaitCursor();
	//init (as nouveau)
	m_conduiteToSave->clearAllOt();
	
	//title in the main window
	m_mainwindow->setName(file.getFileName());
	
	/********* XML PARSER *********/
	File* openedFile = new File(file);
	if (openedFile->getFileExtension() != String(".virage")) {
		return String();
	}
	
	XmlDocument openedDocument(*openedFile);
	
	XmlElement* projectElement = openedDocument.getDocumentElement();
	if (projectElement == 0)
	{
		String error = openedDocument.getLastParseError();
		return error;
	}
	else
	{
		// ECO
		//pour les callbacks
		//s_SequencerPrincipal_address = m_conduiteToSave;
		//load du .iscore
		
//		std::string filenameUTF8((String( file.getFullPathName() + String(".simone") )).toUTF8());
		std::string filenameUTF8((String( file.getFullPathName() + String(".iscore") )).toUTF8());

		m_conduiteToSave->load(filenameUTF8);/*including reset*/
		m_conduiteToSave->addExecutionFinishedCallback();
		
		// check we're looking at the right kind of document..
		if(projectElement->hasTagName(T("VIRAGE")))
		{
			m_conduiteToSave->clearAllOt();
			
			// now we'll iterate its sub-elements looking for 'giraffe' elements...
			forEachXmlChildElement (*projectElement, e) {
				if (e->hasTagName (T("CONDUITE")))
				{
					
					float hzoom = e->getIntAttribute(T("hzoom"));
					float vzoom = e->getIntAttribute(T("vzoom"));
					m_conduiteToSave->getZoomer()->setHorizontalRatio(hzoom);
					m_conduiteToSave->getZoomer()->setVerticalRatio(vzoom);
				}
				
				if (e->hasTagName (T("DEVICEPARK")))
				{
					XmlElement* deviceParkElement = projectElement->getChildByName(T("DEVICEPARK"));
					
					forEachXmlChildElement (*deviceParkElement, f)
					{
						if (f->hasTagName (T("DEVICE")))
						{
							//GUI
							String deviceName		= f->getStringAttribute (T("name"));
							String deviceIp			= f->getStringAttribute (T("ip"));
							String devicePort		= f->getStringAttribute (T("port"));
							String deviceProtocole  = f->getStringAttribute (T("protocole"));
							
							m_conduiteToSave->getMainWindow()->getNamespacePanel()->addDevice((string)deviceName, (string)deviceProtocole, (string)deviceIp, (string)devicePort);
						}
					}
					
				}
				
				if (e->hasTagName (T("BOXPARK")))
				{
					// found a BX park
					XmlElement* boxParkElement = projectElement->getChildByName(T("BOXPARK"));
					m_conduiteToSave->getOtManager()-> setBoxCounter(e->getIntAttribute(T("box-counter")));//box_counter
					
					forEachXmlChildElement (*boxParkElement, f)
					{
						if (f->hasTagName (T("BOX")))
						{
							//GUI
							Colour boxColour = Colour::fromString(f->getStringAttribute(T("colour")));
							bool isFlexible = (f->getBoolAttribute (T("flexibility")) == 1);
							String boxName  = f->getStringAttribute (T("name"));
							int boxHeight   = f->getIntAttribute (T("rootHeight"));
							int boxY   = f->getIntAttribute (T("y"));
							int boxId   = f->getIntAttribute (T("id"));
							int boxX   = m_conduiteToSave->getZoomer()->convertTimeToPix(m_conduiteToSave->getEngines()->getBoxBeginTime(boxId));
							int boxWidth =   m_conduiteToSave->getZoomer()->convertTimeToPix(m_conduiteToSave->getEngines()->getBoxEndTime(boxId) - m_conduiteToSave->getEngines()->getBoxBeginTime(boxId));
							String comment = f->getStringAttribute (T("comment"));
							
							m_conduiteToSave->getOtManager()->addBox(boxName, isFlexible, boxX, boxY, boxWidth, boxHeight, boxId);
							(m_conduiteToSave->getBox(boxId))->setTitleColour(boxColour);
							(m_conduiteToSave->getBox(boxId))->setComment(comment);
							(m_conduiteToSave->getBox(boxId))->updateMessagesEditor(1, (m_conduiteToSave->getBox(boxId))->getOscMessageOutStart());
							(m_conduiteToSave->getBox(boxId))->updateMessagesEditor(2, (m_conduiteToSave->getBox(boxId))->getOscMessageOutStop());
							(m_conduiteToSave->getBox(boxId))->initInterpolationWindow();
							(m_conduiteToSave->getBox(boxId))->setMuted(f->getIntAttribute(T("muted"))==1);
						}
					}
					
				}
				
				if (e->hasTagName (T("RELPARK")))
				{
					// found a RT park
					XmlElement* relParkElement = projectElement->getChildByName(T("RELPARK"));
					
					forEachXmlChildElement (*relParkElement, g)
					{
						if (g->hasTagName (T("REL")))
						{
							unsigned int relId = g->getIntAttribute(T("id"));
							unsigned int motherBoxId = g->getIntAttribute(T("motherBoxId"));
							unsigned int daughterBoxId = g->getIntAttribute(T("daughterBoxId"));
							bool isFlexible = g->getBoolAttribute (T("flexibility"));
							
							//Ajout relation à la GUI
							unsigned int m_relControlPointMother = m_conduiteToSave->getEngines()->getRelationFirstCtrlPointIndex(relId);
							unsigned int m_relControlPointDaughter = m_conduiteToSave->getEngines()->getRelationSecondCtrlPointIndex(relId);
							
							m_conduiteToSave->getOtManager()->addRelation(motherBoxId, daughterBoxId, m_relControlPointMother, m_relControlPointDaughter, relId, isFlexible);
						}
					}
				}
				
				if (e->hasTagName (T("TRIGPARK")))
				{
					// found a RT park
					XmlElement* trigParkElement = projectElement->getChildByName(T("TRIGPARK"));
					
					forEachXmlChildElement (*trigParkElement, g)
					{
						if (g->hasTagName (T("TRIG")))
						{
							unsigned int id = g->getIntAttribute(T("id"));
							unsigned int x = g->getIntAttribute(T("x"));
							Colour _couleur = Colour::fromString(g->getStringAttribute(T("colour")));
							
							
							/*[GUI]*/
							unsigned int relatedBoxId = m_conduiteToSave->getEngines()->getTriggerPointRelatedBoxId(id);
							
							if( relatedBoxId > 0) {
								
								bool start;
								
								if ( m_conduiteToSave->getEngines()->getTriggerPointRelatedCtrlPointIndex(id) == BEGIN_CONTROL_POINT_INDEX) {
									start = true;
								}
								else {
									start = false;
								}
								m_conduiteToSave->getOtManager()->addTrigger(x, _couleur, m_conduiteToSave->getBox(relatedBoxId), start, id, true);
							}
							else {
								m_conduiteToSave->getOtManager()->addTrigger(x, _couleur, 0, true, id, true);
							}
						}
					}
				}
				
				/* mise a jour de l'affichage */
				m_conduiteToSave->refresh();
			}
		}
	}
	
	if (projectElement) {
		deleteAndZero( projectElement );
	}
	if (openedFile) {
		deleteAndZero( openedFile );
	}
	
	
	MouseCursor::hideWaitCursor();
	return String::empty;
}

const String MainMenuBar::saveDocument(const File & file) {
	
	MouseCursor::showWaitCursor();
	m_mainwindow->setName(getFile().getFileName());
	
	/********* XML PARSER *********/
	File* savedFile;
	
	if(file.getFileExtension() == String(".virage")) {
		savedFile = new File(file.getFullPathName());
	}else {
		savedFile = new File(file.getFullPathName() + String(".virage"));
	};
	
	jassert(savedFile->getFileExtension() == String(".virage"));
	
	
	if (savedFile == 0)
	{
		return String("Error, impossible to save.");
	}
	else
	{
//		std::string filenameUTF8((String( file.getFullPathName() + String(".simone") )).toUTF8());
		std::string filenameUTF8((String( file.getFullPathName() + String(".iscore") )).toUTF8());

		// ECO
		m_conduiteToSave->getEngines()->store(filenameUTF8);
		
		//<VIRAGE>
		XmlElement projet ("VIRAGE");
		projet.setAttribute(T("version"), T("0.3"));
		
		//<CONDUITE>
		XmlElement* conduite = new XmlElement(T("CONDUITE"));
		projet.addChildElement(conduite);
		conduite->setAttribute(T("duration"), m_conduiteToSave->getDuration());
		conduite->setAttribute(T("height"), m_conduiteToSave->getHeight());
		conduite->setAttribute(T("hzoom"), (float)m_conduiteToSave->getZoomer()->getRatio());
		conduite->setAttribute(T("vzoom"), (float)m_conduiteToSave->getZoomer()->getVerticalRatio());
		//</CONDUITE>
		
		//<DEVICEPARK>
		XmlElement* devicePark = new XmlElement(T("DEVICEPARK"));
		projet.addChildElement(devicePark);
		
		for(unsigned int i=0; i< m_conduiteToSave->getMainWindow()->getNamespacePanel()->getDeviceList().size(); i++)
		{
			XmlElement* device = new XmlElement ("DEVICE");
			
			device->setAttribute (T("name"), String(m_conduiteToSave->getMainWindow()->getNamespacePanel()->getDeviceList().at(i)->name));
			device->setAttribute (T("protocole"), String(m_conduiteToSave->getMainWindow()->getNamespacePanel()->getDeviceList().at(i)->protocole));
			device->setAttribute (T("ip"), String(m_conduiteToSave->getMainWindow()->getNamespacePanel()->getDeviceList().at(i)->ip));
			device->setAttribute (T("port"), String(m_conduiteToSave->getMainWindow()->getNamespacePanel()->getDeviceList().at(i)->port));
			
			devicePark->addChildElement (device);
		}
		//</DEVICEPARK>
		
		//<BOXPARK>
		XmlElement* boxPark = new XmlElement(T("BOXPARK"));
		projet.addChildElement(boxPark);
		boxPark->setAttribute(T("box-counter"), m_conduiteToSave->getOtManager()->getBoxCounter());
		
		std::map<unsigned int, TimeBox*>			boxFromConduite = *(m_conduiteToSave->getBoxMap());
		std::map<unsigned int, TimeBox*>::iterator	itBox = boxFromConduite.begin();
		while( itBox != boxFromConduite.end() )
		{
			//<BOX>
			// create an inner element...
			XmlElement* box = new XmlElement ("BOX");
			TimeBox* savedBox = itBox->second;
			box->setAttribute (T("id"), String(savedBox->getId()));
			box->setAttribute (T("name"), savedBox->getName());
			String y = String(savedBox->getRootY());
			box->setAttribute (T("y"), y);
			String h = String(savedBox->getRootH());
			box->setAttribute (T("rootHeight"), h);
			box->setAttribute (T("flexibility"), savedBox->getDurationBehavior()->isFlexible());
			box->setAttribute (T("comment"), savedBox->getComment());
			box->setAttribute (T("colour"), savedBox->getTitleColour().toString());
			box->setAttribute (T("muted"), savedBox->isMuted());
			box->setAttribute (T("minimised"), savedBox->isMinimised());
			//...and add our new element to the parent node
			boxPark->addChildElement (box);
			//</BOX>
			itBox++;
		}
		//</BOXPARK>
		
		//<RELPARK>
		XmlElement* relPark = new XmlElement(T("RELPARK"));
		projet.addChildElement(relPark);
		//<REL>
		std::map<unsigned int, Relation*>				relFromConduite = *(m_conduiteToSave->getRelMap());
		std::map<unsigned int, Relation*>::iterator		itRel = relFromConduite.begin();
		while( itRel != relFromConduite.end() )
		{
			// create an inner element...
			XmlElement* rel = new XmlElement ("REL");
			rel->setAttribute (T("id"), (int)(itRel->first));
			rel->setAttribute (T("motherBoxId"), (int)(itRel->second->getMotherId()));
			rel->setAttribute (T("daughterBoxId"), (int)(itRel->second->getDaughterId()));
			rel->setAttribute (T("flexibility"), (bool)(itRel->second->getDurationBehavior()->isFlexible()));
			
			//...and add our new element to the parent node
			relPark->addChildElement (rel);
			itRel++;
		}
		//</REL>
		//</RELPARK>
		
		//<TRIGPARK>
		XmlElement* trigPark = new XmlElement(T("TRIGPARK"));
		projet.addChildElement(trigPark);
		//<TRIG>
		std::map<unsigned int, Trigger*>				trigFromConduite = *(m_conduiteToSave->getTrigMap());
		std::map<unsigned int, Trigger*>::iterator		itTrig = trigFromConduite.begin();
		while( itTrig != trigFromConduite.end() )
		{
			// create an inner element...
			XmlElement* trig = new XmlElement ("TRIG");
			trig->setAttribute (T("id"), (int)(itTrig->first));
			trig->setAttribute (T("message"), itTrig->second->getMinuitMessage());
			trig->setAttribute (T("x"), (int)(itTrig->second->getX()));
			trig->setAttribute (T("colour"), itTrig->second->getColour().toString());
			
			//...and add our new element to the parent node
			trigPark->addChildElement (trig);
			
			itTrig++;
		}
		//</TRIG>
		//</TRIGPARK>
		
		//</VIRAGE>
		
		// now we can turn the whole thing into a text document..
		const String savedDocument = projet.createDocument (String::empty);
		
		//écriture du fichier sur le disque
		/*flag value	opening mode
		 app	(append) Set the stream's position indicator to the end of the stream before each output operation.
		 ate	(at end) Set the stream's position indicator to the end of the stream on opening.
		 binary	(binary) Consider stream as binary rather than text.
		 in	(input) Allow input operations on the stream.
		 out	(output) Allow output operations on the stream.
		 trunc	(truncate) Any current content is discarded, assuming a length of zero on opening.*/
		
		std::ofstream myXmlDocument(savedFile->getFullPathName().toUTF8(), ios::trunc);
		myXmlDocument <<  (string)savedDocument;
		myXmlDocument.close();
		
		//Get the 2 generated files and move them in a directory which is named with the project name : uses stdlib C functions 
		String dir = file.getFullPathName();
		
		int pos = dir.indexOf(2, String("."));
		if (pos != -1) {
			dir = dir.substring(0, pos);
		}
		String file1 = dir + ".virage";
//		String file2 = dir + ".virage.simone";
		String file2 = dir + ".virage.iscore";
		
		if (system("test -d " + dir) && system("test -d ../" + dir)) {
			system("mkdir " + dir);
		}
		system("mv " + file1 + " " + dir);
		system("mv " + file2 + " " + dir);
		
		//delete conduite;
		//delete boxPark;
		//delete relPark;
	}
	
	deleteAndZero( savedFile );
	
	MouseCursor::hideWaitCursor();
	return String::empty;
}

const String MainMenuBar::getDocumentTitle() {
	if (getFile().exists()) return getFile().getFileName();
	else return s_DEFAULT_PROJECT_NAME;
}

void MainMenuBar::setLastDocumentOpened(const File & file) {
	lastDocumentOpened = file;
}

const File MainMenuBar::getLastDocumentOpened() {
	return lastDocumentOpened;
}

void MainMenuBar::changed(Sequencer* conduite) {
	FileBasedDocument::changed();
	m_mainwindow->setName(getDocumentTitle() + String("*"));
}

/*************************** end FileBasedDocument overwritted-must methods *******************/

void MainMenuBar::setConduiteToSave(Sequencer* _conduite) {
	m_conduiteToSave = _conduite;
}

void MainMenuBar::setMainWindowToBroadcast(MainWindow* _mainWindow) {
	m_mainwindow = _mainWindow;
}

juce_UseDebuggingNewOperator
