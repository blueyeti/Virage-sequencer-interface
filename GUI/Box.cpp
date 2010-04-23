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
 *  Boxes.cpp
 *  Virage
 *
 *  Created by Henry Bernard and Luc Vercellin on 02/10/08.
 *  Copyright 2008 by Blue Yeti. All rights reserved.
 *
 */

#include "Box.h"
#include "TimeBox.h"
#include "Sequencer.h"
#include <algorithm>

//================================================================================================
Box::Box(String etiquette, int _x, int _y, int _width, int _height)
  : ResizableBox (etiquette/*will be invisible since the first repaint()*/, Colours::white, false), m_starting_global_time(0) , titleEditor(0)

{
	setName(etiquette);

	//Info button (must be added before the Size definition)
	Component::addAndMakeVisible(m_expandArrowButton = new VirageButton (T("-"), Colours::darkgrey));
	m_expandArrowButton->setButtonText (T("-"));
	m_expandArrowButton->setTooltip(String("minimise"));

	m_expandArrowButton->addButtonListener (this);
	m_expandArrowButton->setToggleState(true, false);
	m_expandArrowButton->setClickingTogglesState(true);

	//Info button (must be added before the Size definition)
//	Component::addAndMakeVisible(m_commentButton = new VirageButton (T("i"), Colours::orange));
//	m_commentButton->setButtonText (T("i"));
//	m_commentButton->setTooltip(String("show comments"));
//  m_commentButton->addButtonListener (this);

	//ATTENTION A L'ORDRE DES IMPLEMENTATIONS
	addAndMakeVisible(m_muteButton = new VirageButton(T("M"), Colours::red));
    m_muteButton->setButtonText (T("M"));
	m_muteButton->setTooltip(String("mute"));
    m_muteButton->addButtonListener (this);
	m_muteButton->setToggleState(false, true);
	m_muteButton->setClickingTogglesState(true);
	m_muteButton->setColour(Colours::darkred);

	//Title Editor (must be added before the Size definition)
	// (call the Component method directly to avoid the assertion in ResizableWindow)
	Component::addChildComponent(titleEditor = new TextEditor());
	titleEditor->addListener(this);
	titleEditor->setText(etiquette, false);
	//titleEditor->setInputRestrictions(0, T("0123456789azertyuiopqsdfghjklmwxcvbnéèçàùAZERTYUIOPQSDFGHJKLMWXCVBN,;:=?./+%*^¨""'()!"));
	titleEditor->setSelectAllWhenFocused(true);

	m_minimised = false;

	//bubble comment
	m_bubbleComment = new BubbleMessageComponent();
	m_bubbleComment->setAlwaysOnTop (true);
	m_bubbleComment->addToDesktop (0);
	m_bubbleComment->setAllowedPlacement(1);

	//mute content bar
	addChildComponent(muteContent = new BoxButton (T("M"), T("Mute end shot"), true));
    muteContent->addButtonListener(this);
	muteContent->setTooltip(String("mute interpolations"));
	muteContent->setConnectedEdges(Button::ConnectedOnRight | Button::ConnectedOnBottom| Button::ConnectedOnTop);
    muteContent->setColour ( Colours::darkred);
	muteContent->setToggleState(false, false);
	muteContent->setClickingTogglesState(true);

	addChildComponent(m_waiting = new WaitingLabel());
	m_waiting->setColour(Label::backgroundColourId, Colours::white);

	//Size
	setVisible(true);
	setBounds(_x,_y,_width,_height);
	setResizable(true,false);

	//Colour
	setDropShadowEnabled(false);
	setBackgroundColour(Colours::transparentWhite.withAlpha(0.0f));
	setTitleColour(s_COLOUR_BOX);

	//Title
	setOpaque(true);
	m_originalBoxHeight = ResizableBox::getHeight();

	m_titleBarHeight = 15;
	m_footerBarHeight = 13;
	m_buttonBarHeight = 0;

//	setResizeLimits(10, m_titleBarHeight + m_footerBarHeight, 32768, 32768);

	setInterceptsMouseClicks(true, true);
	m_minimised = false;//use it ONLY in the minimiseButtonPressed() method please !
	hasBeenMoved = false;//use only for callbacks please !

	deletionAsked = false;
	m_seqId = -1;

	m_isInterpolated = false;

	m_previous_height = getHeight();

}

Box::~Box() {
  if (titleEditor) {
    deleteAndZero(titleEditor);
  }
//  if (m_commentButton) {
//    deleteAndZero(m_commentButton);
//  }
  if (m_bubbleComment) {
    deleteAndZero(m_bubbleComment);
  }
  if (m_expandArrowButton) {
    deleteAndZero(m_expandArrowButton);
  }
}

//================================================================================================
Colour Box::getColour() {

	return getTitleColour();
}

bool Box::isDeletionAsked() {

	return deletionAsked;
}

void Box::buttonClicked(Button* buttonThatWasClicked) {

	if(buttonThatWasClicked == m_muteButton) {
		setMuted(buttonThatWasClicked->getToggleState());
	}

	if(buttonThatWasClicked == m_expandArrowButton) {
		expandButtonPressed();
	}

	repaint();
}

 int Box::getButtonBarHeight() {

	return m_buttonBarHeight;
}

//===========================================

void Box::expandButtonPressed() {

	if( !  m_expandArrowButton->getToggleState())	{		
		setSize(getWidth(), m_originalBoxHeight);
		m_expandArrowButton->setButtonText(T("-"));
		m_expandArrowButton->setTooltip(String("minimise"));
		m_expandArrowButton->setColour (Colours::darkgrey);
	}
	else{
		setSize(getWidth(), m_titleBarHeight  + m_footerBarHeight + m_buttonBarHeight );
		m_expandArrowButton->setButtonText(T("+"));
		m_expandArrowButton->setTooltip(String("maximise"));
		m_expandArrowButton->setColour (Colours::black);
	}
}

void Box::resized() {
	ResizableBox::resized();//is strictly linked to minimisedButtonPressed()

	if( (unsigned int)getHeight() != m_titleBarHeight + m_footerBarHeight + m_buttonBarHeight) {

		m_originalBoxHeight = getHeight();
		m_expandArrowButton->setToggleState(false, true);

	}
	
	//visibilité boutton
//	m_editAutomationButton->setVisible(	getWidth() > m_titleBarHeight*6);
	m_muteButton->setVisible( (unsigned int)getWidth() > m_titleBarHeight*5);
//	m_commentButton->setVisible((unsigned int)getWidth() > m_titleBarHeight*3);
	m_expandArrowButton->setVisible((unsigned int)getWidth() > m_titleBarHeight*2);

	if(m_expandArrowButton != 0)
		m_expandArrowButton->setBounds(getWidth()-m_titleBarHeight, 0 , m_titleBarHeight, m_titleBarHeight);
//	if(m_commentButton != 0)
//		m_commentButton->setBounds(m_expandArrowButton->getX()-m_titleBarHeight, 0 , m_titleBarHeight, m_titleBarHeight);
	if(m_muteButton != 0)
		m_muteButton->setBounds(m_expandArrowButton->getX()-m_titleBarHeight, 0 , m_titleBarHeight, m_titleBarHeight);
//	if(m_editAutomationButton != 0)
//		m_editAutomationButton->setBounds(m_muteButton->getX()-m_titleBarHeight , 0 , m_titleBarHeight, m_titleBarHeight);
	if(muteContent != 0)
		muteContent->setBounds (s_BOX_MESSAGEBUTTON_WIDTH, (getHeight()-m_footerBarHeight), std::min(30,getWidth()-s_BOX_MESSAGEBUTTON_WIDTH*2), (m_footerBarHeight) );

	if(this!=NULL) sendSynchronousChangeMessage(this);
}

void Box::setTitleColour(Colour _titleColour, float _alpha) {
	m_titleColour = _titleColour;
	m_titleAlpha = _alpha;
}

Colour Box::getTitleColour() {

	return m_titleColour;
}

float Box::getTitleAlpha() {

	return m_titleAlpha;
}

const BorderSize Box::getBorderThickness() {
	return BorderSize(0, 1, 18, 1);
}

const BorderSize Box::getContentComponentBorder() {
    return BorderSize (m_titleBarHeight,
                       1,
					   m_footerBarHeight,
					   1);
}

void Box::setOtColour(Colour _newColour) {

	setTitleColour(_newColour);
}

/*TEXT EDITOR*/
void Box::textEditorReturnKeyPressed (TextEditor& /*editor*/) {
	String titleWithoutSpace = titleEditor->getText().replace(String(" "), String("_"));
	setName(titleWithoutSpace);
	updateTriggers(titleWithoutSpace);
	titleEditor->setVisible(false);
}

void Box::textEditorTextChanged (TextEditor& /*editor*/) {}

void Box::textEditorEscapeKeyPressed (TextEditor& /*editor*/) {}

void Box::textEditorFocusLost (TextEditor& /*editor*/) {
	String titleWithoutSpace = titleEditor->getText().replace(String(" "), String("_"));
	setName(titleWithoutSpace);
	updateTriggers(titleWithoutSpace);
	titleEditor->setVisible(false);
	
}
/*FIN TEXT EDITOR*/

void Box::mouseMove(const MouseEvent& e) {
	
	getParentComponent()->mouseMove(e.getEventRelativeTo(getParentComponent()));
}

void Box::mouseDoubleClick(const MouseEvent& /*e*/) {
	titleEditor->setVisible(true);
	titleEditor->setCaretVisible(true);
	titleEditor->toFront(true);
}