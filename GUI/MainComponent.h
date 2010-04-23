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
#include "MiddleFrame.h"
#include "TransportBar.h"
#include "BottomFrame.h"
#include "SafeComponentGetter.h"

#ifndef _MAINCOMPONENT_H_
#define _MAINCOMPONENT_H_

#define WIDTH_RESIZEBAR 5
#define HEIGHT_TRANSPORT 42
#define MIN_MIDDLE_FRAME_HEIGHT 300
#define MIN_BOTTOM_FRAME_HEIGHT 50
#define MAX 32700



class MainComponent  : public Component, public MainMenuBar, public ComponentListener {
	//==============================================================================
private:
	/*MainMenuBar*/
	ApplicationCommandManager* m_commandManager;

	/*MainComponent*/
	TransportBar* m_transportBar;
	MiddleFrame* m_middleFrame;
	BottomFrame* m_bottomFrame;

	SafeComponentGetter* m_safeComponentGetter;
	friend class SafeComponentGetter;

public:
	StretchableLayoutManager* horizontalLayout;
	StretchableLayoutResizerBar* horizontalStretcher;

	//==============================================================================
	MainComponent (ApplicationCommandManager* _commandManager)	{
		m_commandManager = _commandManager;
		
		horizontalLayout = new StretchableLayoutManager;
		horizontalLayout->setItemLayout (0, HEIGHT_TRANSPORT, HEIGHT_TRANSPORT, HEIGHT_TRANSPORT);//transport
		horizontalLayout->setItemLayout (1, MIN_MIDDLE_FRAME_HEIGHT, MAX, 1000);//middle frame
		//horizontalLayout->setItemLayout (3, MIN_BOTTOM_FRAME_HEIGHT, MAX, 50);//bottom frame

		addAndMakeVisible( horizontalStretcher = new StretchableLayoutResizerBar(horizontalLayout, 2, false/*vertical*/) );
		/*
		 StretchableLayoutResizerBar (StretchableLayoutManager* const layoutToUse,
		 const int itemIndexInLayout,
		 const bool isBarVertical)
		 */
		/*ordre importe*/
		addAndMakeVisible(m_middleFrame = new MiddleFrame());
		addAndMakeVisible(m_bottomFrame = new BottomFrame());
		addAndMakeVisible(m_transportBar = new TransportBar(this));
				
		/*écouteur des composants - pour les changements de taille / minimising */
		m_middleFrame->addComponentListener(this);
		m_bottomFrame->addComponentListener(this);

	}

	virtual ~MainComponent(){
		deleteAllChildren();
		if (horizontalLayout) {
			deleteAndZero(horizontalLayout);
		}
		if (m_commandManager) {
			deleteAndZero(m_commandManager);
		}
	}

	TransportBar* getTransportBar(){return m_transportBar;}

	void setSCG(SafeComponentGetter* _scg){
		m_safeComponentGetter = _scg;
	}

	SafeComponentGetter* getSCG() {
		return m_safeComponentGetter;
	}

	void resized() {

		m_transportBar->setBounds( 0, 0, getWidth(), HEIGHT_TRANSPORT );
		// make a list of two of our child components that we want to reposition
		Component* comps[] = { m_transportBar, m_middleFrame, horizontalStretcher, m_bottomFrame};
		horizontalLayout->layOutComponents (comps, 4/*num component*/, 0, 0, getWidth(), getHeight(), true, true);
		/*
		 void layOutComponents (Component** const components,
		 int numComponents,
		 int x, int y, int width, int height,
		 const bool vertically,
		 const bool resizeOtherDimension);*/
	}

	void componentMovedOrResized (Component& /*component*/, bool /*wasMoved*/, bool /*wasResized*/){

		horizontalLayout->setItemPosition(3, m_bottomFrame->getHeight() );
	}


	void paint(Graphics& g){g.fillAll (Colours::black);}

	MiddleFrame* getMiddleFrame(){return m_middleFrame;}
	BottomFrame* getBottomFrame(){return m_bottomFrame;}

	/*
	 * Manages the pop'actions when menu-bar items are clicked
	 * Previously defined by the mother-class MainMenuBar, we redefine it here because we have to use the MainWindow commandManager.
	 */
	const PopupMenu getMenuForIndex (int menuIndex, const String& /*menuName*/)
	{
		PopupMenu menu;
		ApplicationCommandManager* const acm = m_commandManager;


		if (menuIndex==0){

			menu.addCommandItem (acm,Nouveau,"Nouveau");
			menu.addCommandItem (acm,Ouvrir,"Ouvrir");
//			menu.addCommandItem (acm,Fermer,"Fermer");
			menu.addSeparator();
			menu.addCommandItem (acm, Enregistrer,"Enregistrer");
			menu.addCommandItem(acm, EnregistrerSous, "Enregistrer sous...");
			menu.addSeparator();
			menu.addCommandItem(acm, Play,"Lecture & GO");
			menu.addCommandItem(acm, Pause, "Pause");
			menu.addCommandItem(acm, Stop,"Arret");
			menu.addCommandItem(acm, Loop, "Loop mode");
			menu.addCommandItem(acm, Next, "Go [Next]");
//			menu.addSeparator();
//			menu.addCommandItem(acm, Infos, "Informations sur le fichier");
		}
		else if (menuIndex == 1) {
//			menu.addCommandItem (acm,Undo,"Undo/Redo");
			menu.addSeparator();
//			menu.addCommandItem (acm,Couper,"Couper");
			menu.addCommandItem (acm,Copier,"Copier");
			menu.addCommandItem (acm,Coller,"Coller");
			menu.addCommandItem (acm,Effacer,"Effacer");
			menu.addCommandItem (acm,Dupliquer,"Dupliquer");
			menu.addCommandItem (acm,SelectionnerTout,"Selectionner tout");
		}
//		else if (menuIndex == 2) {
//			menu.addCommandItem (acm,ModesAffichage,"Modes d'affichage");
//			menu.addCommandItem (acm,Zoom,"Zoom");
//			menu.addCommandItem (acm,BoiteAOutils,"Boite a outils");
//			menu.addCommandItem (acm,Palettes,"Palettes");
//		}
//		else if (menuIndex == -3) {
//			menu.addCommandItem (acm,Objet,"Inserer un objet...");
//			menu.addCommandItem(acm, Contraintes, "Definir une contrainte...");
//			menu.addSeparator();
//			menu.addCommandItem(acm, Grouper, "Grouper");
//			menu.addCommandItem(acm, Degrouper, "Degrouper");
//			menu.addSeparator();
//			menu.addCommandItem(acm, Verouiller, "Verouiller");
//			menu.addCommandItem(acm, ToutDeverouiller, "Tout deverouiller");
//		}
//		else if (menuIndex == -4) {
//			menu.addCommandItem (acm,CaptureParametres,"Capturer les parametres");
//			menu.addCommandItem(acm, GestionPlans, "Gestion des plans");
//		}
//		else if (menuIndex == 3) {
//			menu.addCommandItem(acm, ToutAfficher, "Tout afficher");
//			menu.addSeparator();
//			menu.addCommandItem(acm, Inspecteur, "Inspecteur");
//		}
		else if (menuIndex == 2) {



#if ! JUCE_MAC
			menu.addCommandItem (acm,SearchHelp,"Rechercher...");
#endif
//			menu.addCommandItem (acm,Help,"Aide");
//			menu.addSeparator();
			// add the about and prefrences to the file menu on non-Mac platforms
			menu.addCommandItem (acm, About, "About Virage");
		}
		else {
			//#if JUCE_MAC
//			menu.addCommandItem (acm, Preferences, "Preferences");
			menu.addCommandItem (acm, About, "About Virage");
			//#endif
		}

		return menu;
	}
};

#endif
