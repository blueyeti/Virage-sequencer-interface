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
 *  EditorFrame.cpp
 *  Virage
 *
 *  Created by Henry on 02/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include "EditorFrame.h"

#define ToolButtonWidth 20
#define B_PADDING 10
#define TOOLQuantityWidth 3
#define CELL_PADDING 5
#define LabelHeight 20

EditorFrame::EditorFrame()
{
	addAndMakeVisible(m_crayonToolButton = new penIcon());
	m_crayonToolButton->addButtonListener(this);
	m_crayonToolButton->setToggleState(false, false);

	addAndMakeVisible(m_editedName = new Label(T(""), T("")));
	m_editedName->setColour(Label::backgroundColourId, Colours::white);

	addAndMakeVisible(m_editor = new AutomationScope(NO_ID, NULL,AutomationCurve::DEFAULT_START_VALUE,AutomationCurve::DEFAULT_END_VALUE));
	//m_editor->setEnabled(false);

	setWantsKeyboardFocus(false);
}

EditorFrame::~EditorFrame(){
	//deleteAllChildren();
	delete m_editor;
}

void EditorFrame::paint(Graphics& g){
	g.fillAll(Colours::black);
	g.setColour(s_COLOUR_SELECTION_GREEN);
	g.fillRoundedRectangle(
						B_PADDING/4,
						B_PADDING/4,
						getWidth()-2*B_PADDING/4,
						getHeight()-2*B_PADDING/4,
					   /*rounded coeff*/B_PADDING);
}

void EditorFrame::resized(){
	  m_editedName->setBounds(B_PADDING, B_PADDING, getWidth() - (TOOLQuantityWidth * ToolButtonWidth) - 2*B_PADDING - CELL_PADDING, LabelHeight);
	  m_editor->setBounds(B_PADDING, m_editedName->getBottom(), getWidth()-(TOOLQuantityWidth*ToolButtonWidth)-2*B_PADDING-CELL_PADDING, getHeight() - m_editedName->getBottom()-B_PADDING);
					  m_crayonToolButton->setBounds(
													m_editor->getWidth() + m_editor->getX() + CELL_PADDING + ToolButtonWidth*0, B_PADDING, ToolButtonWidth, ToolButtonWidth);
}

void EditorFrame::setScopeTitleName(String _concernedName){

	m_editedName->setText(_concernedName, false);
}

void EditorFrame::setScopeTitleColour(Colour /*_concernedColour*/){

	//m_editedName->setColour(Label::backgroundColourId, _concernedColour);
	//m_editedName->setColour(Label::textColourId,       _concernedColour.contrasting(1.0f));
}

void EditorFrame::buttonClicked (Button* buttonThatWasClicked){

	buttonThatWasClicked->setToggleState(!buttonThatWasClicked->getToggleState(), false);
	if(buttonThatWasClicked == m_crayonToolButton)		m_editor->setEditionTool(m_crayonToolButton->getToggleState() ? PEN:  DOT_MAKER);
}

void EditorFrame::modifierKeysChanged(const ModifierKeys& modifiers) {

	m_crayonToolButton->setToggleState( modifiers.isAltDown(), false);
	m_editor->setEditionTool(m_crayonToolButton->getToggleState() ? PEN:  DOT_MAKER);
}
