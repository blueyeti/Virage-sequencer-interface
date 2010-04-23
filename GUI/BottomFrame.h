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
 *  BottomFrame.h
 *  Virage
 *
 *  Created by Henry on 04/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _BOTTOMFRAME_H_
#define _BOTTOMFRAME_H_

#define MIN_HEIGHT 20

#include "includes.h"
#include "EditorFrame.h"
#include "OpenCloseButton.h"

class BottomFrame : public Component,public ButtonListener {

private:
	int m_originalHeight;
	friend class SafeComponentGetter;
	MainComponent* m_parent;
	/*left toolbar*/
	Toolbar* m_toolbar;
	/*main*/
	EditorFrame* m_editorFrame;
	
	class LeftToolbarItemFactory   : public ToolbarItemFactory
		{
		public:
			LeftToolbarItemFactory(BottomFrame* _parent) { m_parent = _parent;}
			~LeftToolbarItemFactory() {}
			
			enum DemoToolbarItemIds
			{
				arrow_open = 1
			};
			
			void getAllToolbarItemIds (Array <int>& ids)
			{
				ids.add (arrow_open);
			}
			
			void getDefaultItemSet (Array <int>& ids)
			{
				ids.add (arrow_open);
			}
			
			ToolbarItemComponent* createItem (const int itemId)
			{
				switch (itemId)
				{
					case arrow_open:	return m_parent->m_openCloseButton;
					default:		break;
				}				
				return 0;
			}
			
		private:
			BottomFrame* m_parent;
		};
	LeftToolbarItemFactory* factory;
	
	
public:
	OpenCloseButton* m_openCloseButton;	
	StretchableLayoutManager* verticalLayout;
	StretchableLayoutResizerBar* verticalStretcher;
	
	/********************************************************************/
	BottomFrame() {
		
		addAndMakeVisible(m_editorFrame = new EditorFrame());
		setWantsKeyboardFocus(false);
		
		m_openCloseButton = new OpenCloseButton();
		m_openCloseButton->addButtonListener(this);
		m_openCloseButton->setToggleState(true, false);
		
		addAndMakeVisible(m_toolbar = new Toolbar());
		m_toolbar->setVertical(true);
		factory = new LeftToolbarItemFactory(this);
		m_toolbar->addDefaultItems(*factory);
		m_toolbar->setColour(Toolbar::backgroundColourId, Colours::black);
		m_toolbar->setColour(Toolbar::buttonMouseOverBackgroundColourId, Colours::black);
		m_toolbar->setColour(Toolbar::buttonMouseDownBackgroundColourId, Colours::black);
	}
	
	~BottomFrame(){		deleteAllChildren();    }
	
	void paint(Graphics& g) {
		g.fillAll(Colours::black);
	}
	
	void buttonClicked (Button* buttonThatWasClicked){
		
		if(buttonThatWasClicked == m_openCloseButton) {
			
			setSize(getWidth(), !m_openCloseButton->getToggleState() ? MIN_HEIGHT : 100);//100 : m_originalHeight
			
			resized();
		}
	}
	
	void resized() {
		
		if(m_toolbar != NULL)		m_toolbar->		setBounds(0,getHeight()-s_MAINWINDOW_PADDING_LEFT,s_MAINWINDOW_PADDING_LEFT, s_MAINWINDOW_PADDING_LEFT);/*en bas à gauche*/
		if(m_editorFrame != NULL)	m_editorFrame->	setBounds(s_MAINWINDOW_PADDING_LEFT, 0, getWidth() - s_MAINWINDOW_PADDING_LEFT, getHeight());
		
		
		if(getHeight() > MIN_HEIGHT) m_originalHeight = getHeight();
		if(getHeight() < MIN_HEIGHT) setSize(getWidth(), MIN_HEIGHT);
	}
	
};

#endif
