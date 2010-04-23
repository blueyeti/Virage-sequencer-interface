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
 *  EditorFrame.h
 *  Virage
 *
 *  Created by Henry on 02/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _EDITORFRAME_H_
#define _EDITORFRAME_H_

#include "includes.h"
#include "AutomationScope.h"
#include "penIcon.h"

#define BUTTON_PADDING 3

class AutomationEditorButton : public TextButton {
private:
	E_AutomationToolMode m_tool;
public:
	AutomationEditorButton(String _text = T("."), E_AutomationToolMode _tool = NONE) : TextButton (_text), m_tool(_tool) {
		setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnLeft );
	}
	~AutomationEditorButton(){}
	
	void paint(Graphics& g) {
	
		TextButton::paint(g);
		
		int size = 5;
		
		g.setColour(Colours::black);
		switch (m_tool) {
			case DOT_MAKER:
				break;
			case PEN:
				g.drawLine(getWidth()-BUTTON_PADDING - size, BUTTON_PADDING, getWidth()-BUTTON_PADDING, size+BUTTON_PADDING,1);
				g.drawLine(getWidth()-BUTTON_PADDING, size+BUTTON_PADDING, BUTTON_PADDING + size,  getHeight()-BUTTON_PADDING,1);
				g.drawLine(BUTTON_PADDING + size,  getHeight()-BUTTON_PADDING, BUTTON_PADDING, getHeight()-BUTTON_PADDING,1);
				g.drawLine(BUTTON_PADDING, getHeight()-BUTTON_PADDING, BUTTON_PADDING, getHeight()-BUTTON_PADDING-size,1);
				g.drawLine(BUTTON_PADDING, getHeight()-BUTTON_PADDING-size,getWidth()-BUTTON_PADDING - size, BUTTON_PADDING,1);
				break;
			default:
				break;
		}
	}
};

class EditorFrame : public Component, public ButtonListener {
	
private:
	penIcon* m_crayonToolButton;
	/*AutomationEditorButton* m_lineToolButton;
	AutomationEditorButton* m_eraseToolButton;
	AutomationEditorButton* m_pen;
	AutomationEditorButton* m_bezier;
	AutomationEditorButton* m_b6;
	AutomationEditorButton* m_b7;
	AutomationEditorButton* m_b8;
	AutomationEditorButton* m_b9;*/
	
	Label* m_editedName;
	AutomationScope* m_editor;
	
	friend class SafeComponentGetter;
	
public:
	EditorFrame();
	~EditorFrame();
	
	/*Component*/
	void paint(Graphics& g);
	void resized();
	
	/*EditorFrame*/
	void setScopeTitleName(String _concernedName);
	void setScopeTitleColour(Colour _concernedColour);
	
	/*ButtonListener*/
	void buttonClicked (Button* buttonThatWasClicked);	
	
	
	void modifierKeysChanged(const ModifierKeys& modifiers);
};

#endif
