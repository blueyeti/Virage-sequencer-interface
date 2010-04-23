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
 *  MiddleFrame.h
 *  Virage
 *
 *  Created by Henry on 02/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MIDDLEFRAME_H_
#define _MIDDLEFRAME_H_

#define WIDTH_RESIZEBAR 5

#include "includes.h"
#include "MainMenuBar.h"
#include "Sequencer.h"
#include "SequencerFrame.h"
#include "PropertyFrame.h"
#include "CommWindow.h"
#include "TransportBar.h"

//toolbar button
#include "OpenCloseButton.h"
#include "PropSequencerButton.h"
#include "PropOTButton.h"
#include "NamespaceBrowserButton.h"
#include "CommButton.h"

//property panels
#include "SequenceOtPropertyPanel.h"
#include "SequencePropertyPanel.h"
#include "NetComponent.h"
#include "CommComponent.h"

class MiddleFrame : public Component, public ButtonListener {
	//==============================================================================
private:
	
	friend class SafeComponentGetter;
	
	MainComponent* m_parent;
	
	/*gauche*/
	Toolbar* m_toolbar;
	
	/*milieu*/
	PropertyFrame*  m_propertyFrame;
	
	/*droite*/
	SequencerFrame* m_sequenceFrame;
	Component* mainFrameComponent;/*par default, la sequence, mais peut passer en cue*/
	float m_FRAME_BORDER_X_PERCENT;//%age entre 0 et 1
	
	//==========TOOOLBAR FACTORY========================================================
	class LeftToolbarItemFactory   : public ToolbarItemFactory
		{
		public:
			LeftToolbarItemFactory(MiddleFrame* _parent) { m_parent = _parent;}
			~LeftToolbarItemFactory() {}
			
			enum DemoToolbarItemIds
			{
				//arrow_open           = 1,
				//edit_prop_seq         = 2,
				edit_prop_ots         = 3,
				edit_namespace         = 4,
				edit_comm         = 5
			};
			
			void getAllToolbarItemIds (Array <int>& ids)
			{
				//ids.add (arrow_open);
				//ids.add (edit_prop_seq);
				ids.add (edit_prop_ots);
				ids.add (edit_namespace);
				ids.add (edit_comm);
			}
			
			void getDefaultItemSet (Array <int>& ids)
			{
				//ids.add (arrow_open);
				//ids.add (edit_prop_seq);
				ids.add (edit_prop_ots);
				ids.add (edit_namespace);
				ids.add (edit_comm);
			}
			
			ToolbarItemComponent* createItem (const int itemId)
			{
				switch (itemId)
				{
					//case arrow_open:	return m_parent->m_openCloseButton;
					//wcase edit_prop_seq: return m_parent->m_editSeqButton;
					case edit_prop_ots: return m_parent->m_editOTButton;
					case edit_namespace: return m_parent->m_editNamespaceButton;
					case edit_comm: return m_parent->m_editCommButton;
					default:		break;
				}
				return 0;
			}
			
		private:
			MiddleFrame* m_parent;
		};
	LeftToolbarItemFactory* factory;
	
	bool m_isNetExplorerUnfolded;
	
public:
	
	OpenCloseButton* m_openCloseButton;
	PropOTButton* m_editOTButton;
	PropSequencerButton* m_editSeqButton;
	NamespaceBrowserButton* m_editNamespaceButton;
	CommButton* m_editCommButton;
	
	/*property frame*/
	NetComponent* m_namespacePanel;
	SequenceOtPropertyPanel* m_otPanel;
	SequencePropertyPanel* m_sequencePanel;
	CommComponent* m_commPanel;
	
	StretchableLayoutManager* verticalLayout;
	StretchableLayoutResizerBar* verticalStretcher;
	
	//==============================================================================
	MiddleFrame ()	{

		m_openCloseButton = new OpenCloseButton();
		m_openCloseButton->addButtonListener(this);
		m_openCloseButton->setToggleState(true, false);

		m_editOTButton = new PropOTButton();
		m_editOTButton->addButtonListener(this);
		m_editOTButton->setToggleState(true, false);

		m_editSeqButton = new PropSequencerButton();
		m_editSeqButton->addButtonListener(this);

		m_editNamespaceButton = new NamespaceBrowserButton();
		m_editNamespaceButton->addButtonListener(this);

		m_editCommButton = new CommButton();
		m_editCommButton->addButtonListener(this);

		m_otPanel = new SequenceOtPropertyPanel();
		m_sequencePanel = new SequencePropertyPanel();
		m_commPanel = new CommComponent();

		m_FRAME_BORDER_X_PERCENT = s_FRAME_BORDER_X_PERCENT;

		verticalLayout = new StretchableLayoutManager;
		/*pan*/verticalLayout->setItemLayout (0, s_MAINWINDOW_PADDING_LEFT, s_MAINWINDOW_PADDING_LEFT, s_MAINWINDOW_PADDING_LEFT);//fixed size
		///*pan*/verticalLayout->setItemLayout (1, 200, 1300, 200);
		/*PropertyFrameAdjust(percents)*/verticalLayout->setItemLayout (1, -0.15, -0.85, -0.2);
		verticalLayout->setItemLayout		 (2, WIDTH_RESIZEBAR, WIDTH_RESIZEBAR, WIDTH_RESIZEBAR);//fixed size - resizer bar
		///*seq*/verticalLayout->setItemLayout (3, 100, 32700, 800);
		/*SequencerFrameAdjust(percents)*/verticalLayout->setItemLayout (3, -0.12, -0.88, -0.80);
		addAndMakeVisible( verticalStretcher = new StretchableLayoutResizerBar(verticalLayout, 2, true) );
		/*
		 StretchableLayoutResizerBar (StretchableLayoutManager* const layoutToUse,
		 const int itemIndexInLayout,
		 const bool isBarVertical)
		 */

		m_sequenceFrame = new SequencerFrame();
		addAndMakeVisible(mainFrameComponent = m_sequenceFrame);

		m_namespacePanel = new NetComponent(getConduite()->getEngines());
		m_namespacePanel->setMainWindow((MainWindow*)getParentComponent());

		addAndMakeVisible(m_propertyFrame = new PropertyFrame((Component*)m_parent));

		addAndMakeVisible(m_toolbar = new Toolbar());

		m_toolbar->setVertical(true);
		factory = new LeftToolbarItemFactory(this);
		m_toolbar->addDefaultItems(*factory);
		m_toolbar->setColour(Toolbar::backgroundColourId, Colours::black);
		m_toolbar->setColour(Toolbar::buttonMouseOverBackgroundColourId, Colours::black);
		m_toolbar->setColour(Toolbar::buttonMouseDownBackgroundColourId, Colours::black);
		
		m_propertyFrame->setContentComponent(m_otPanel);
		m_sequencePanel->setConcerned( m_sequenceFrame->getConduite() );
		
		m_isNetExplorerUnfolded = false;
	}
	
	~MiddleFrame(){ //deleteAllChildren();
	}
	
	SequencerFrame* getSequencerFrame() { return m_sequenceFrame;}
	
	Component* getMainFrame() {return mainFrameComponent;}
	
	void setMainFrameContent(Component* _newContent) { mainFrameComponent = _newContent; /*resized();*/}
	
	PropertyFrame* getPropertyFrame() { return m_propertyFrame;}

	void resized() {
		
		// make a list of two of our child components that we want to reposition
		Component* comps[] = {m_toolbar, m_propertyFrame, verticalStretcher, mainFrameComponent};
		verticalLayout->layOutComponents (comps, 4/*num component*/,
										  0, 0,
										  getWidth(), getHeight(),
										  false, true);
		/*
		 void layOutComponents (Component** const components,
		 int numComponents,
		 int x, int y, int width, int height,
		 const bool vertically,
		 const bool resizeOtherDimension);*/
	}
	
	CommButton* getCommentButton(){ return m_editCommButton;}
	
	PropOTButton* getOtPropButton(){return m_editOTButton;}
	
	void paint(Graphics& g){
		g.fillAll (Colours::black);
	}
	
	void refreshComment(){
		getCommPanel()->refresh();
	}
	
	Sequencer* getConduite(){
		return m_sequenceFrame->getConduite();
	}
	
	void buttonClicked (Button* buttonThatWasClicked){
		
		if(buttonThatWasClicked == m_openCloseButton) {
			m_propertyFrame->setSize(m_openCloseButton->isDown() ? m_propertyFrame->getMinWidth() : m_propertyFrame->getOriginalWidth(), getHeight());
			resized();
		}
		if(buttonThatWasClicked == m_editOTButton) {
			m_propertyFrame->setContentComponent( m_otPanel );
			m_editOTButton->setToggleState(true, false);
			m_editSeqButton->setToggleState(false, false);
			m_editNamespaceButton->setToggleState(false, false);
			m_editCommButton->setToggleState(false, false);
			m_openCloseButton->setToggleState(true, true);
		}
		if(buttonThatWasClicked == m_editSeqButton) {
			m_propertyFrame->setContentComponent( m_sequencePanel );
			m_editSeqButton->setToggleState(true, false);
			m_editOTButton->setToggleState(false, false);
			m_editNamespaceButton->setToggleState(false, false);
			m_editCommButton->setToggleState(false, false);
			m_openCloseButton->setToggleState(true, true);
		}
		if(buttonThatWasClicked == m_editNamespaceButton) {
			m_propertyFrame->setContentComponent( m_namespacePanel );
			m_editNamespaceButton->setToggleState(true, false);
			m_editSeqButton->setToggleState(false, false);
			m_editOTButton->setToggleState(false, false);
			m_editCommButton->setToggleState(false, false);
			m_openCloseButton->setToggleState(true, true);
		}
		if(buttonThatWasClicked == m_editCommButton) {
			m_propertyFrame->setContentComponent( m_commPanel );
			m_editCommButton->setToggleState(true, false);
			m_editSeqButton->setToggleState(false, false);
			m_editOTButton->setToggleState(false, false);
			m_openCloseButton->setToggleState(true, true);
			m_editNamespaceButton->setToggleState(false, false);
			
		}
	}
	
	NetComponent* getNamespacePanel() {
		return m_namespacePanel;
	}
	
	CommComponent* getCommPanel() {
		return m_commPanel;
	}
	
	void manageStretchableLayout(bool istreeMapUsed) {
		if (!m_isNetExplorerUnfolded) {
			verticalLayout->setItemPosition(2, 1400);
			m_isNetExplorerUnfolded = true;
		} else {
			verticalLayout->setItemPosition(2, 300);
			m_isNetExplorerUnfolded = false;
		}
		resized();
	}
};

#endif
