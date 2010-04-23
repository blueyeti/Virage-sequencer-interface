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
 *  TextEditorContentComponent.cpp
 *  Virage
 *
 *  Created by Laurent Garnier on 13/03/09.
 *  Copyright 2009 BlueYeti.Fr. All rights reserved.
 *
 */
#include "includes.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "MessageTextEditorWindow.h"
#include "TextEditorContentComponent.h"

#include "TimeBox.h"
using namespace std;

TextEditorContentComponent::TextEditorContentComponent() {

	setVisible(false);

	/*editor*/
	addAndMakeVisible(m_messagesEditor = new TextEditor());
	m_messagesEditor->addListener(this);
	m_messagesEditor->setMultiLine(true, true);
	m_messagesEditor->setReturnKeyStartsNewLine(true);
	m_messagesEditor->setBorder(5);

	/*buttons*/
	addAndMakeVisible(m_loadButton = new TextButton(T("Load")));
	m_loadButton->addButtonListener (this);
	m_loadButton->setColour (TextButton::buttonColourId, Colours::white);
	m_loadButton->setColour (TextButton::textColourId, Colours::black);

	addAndMakeVisible(m_saveButton = new TextButton(T("Save as...")));
	m_saveButton->addButtonListener (this);
	m_saveButton->setColour (TextButton::buttonColourId, Colours::white);
	m_saveButton->setColour (TextButton::textColourId, Colours::black);

	addAndMakeVisible(m_validButton = new TextButton(T("OK")));
	m_validButton->addButtonListener (this);
	m_validButton->setColour (TextButton::buttonColourId, Colours::white);
	m_validButton->setColour (TextButton::textColourId, Colours::black);

	addAndMakeVisible(m_clearButton = new TextButton(T("Clear")));
	m_clearButton->addButtonListener (this);
	m_clearButton->setColour (TextButton::buttonColourId, Colours::white);
	m_clearButton->setColour (TextButton::textColourId, Colours::black);


	m_validButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	m_saveButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	m_loadButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	m_clearButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
}

TextEditorContentComponent::~TextEditorContentComponent() {
	deleteAllChildren();
}

/*JUCE*/
void TextEditorContentComponent::paint(Graphics &g){
}

void TextEditorContentComponent::resized(){
	m_messagesEditor->setBounds(0, 0, getWidth(), getHeight()-30);
	m_validButton->setBounds(0, getHeight()-30, 100, 30);
	m_saveButton->setBounds(100, getHeight()-30, 50, 30);
	m_loadButton->setBounds(150, getHeight()-30, 50, 30);
	m_clearButton->setBounds(200, getHeight()-30, 50, 30);
}

String TextEditorContentComponent::getText(){
	return m_messagesEditor->getText();
}

void TextEditorContentComponent::setText(String _text){
	m_messagesEditor->setText(_text);
}

void TextEditorContentComponent::updateOscMessages(String _messages){
	string tmp1;
	istringstream iss1( (string)_messages );
	vector<String> messagesToAdd;
	while(getline(iss1, tmp1, '\n')){
		if(tmp1 != "") messagesToAdd.push_back(tmp1.data());
	}

	for(uint i = 0; i < messagesToAdd.size(); i++){
			String messageToAdd = messagesToAdd.at(i);
			String messageToAddWithoutValue = messageToAdd.initialSectionNotContaining(String(" "));

			/*if the editor doesn't contains the new message, it add it at the end*/
			if(!getText().contains(messageToAddWithoutValue) || getText().isEmpty()){
				m_messagesEditor->setCaretPosition(m_messagesEditor->getText().length());
				m_messagesEditor->insertTextAtCursor(messagesToAdd.at(i)+"\n");
			}
			/*if the editor contains the message, it replace it by the new to update the value*/
			else{
				String wholeText = getText();
				if (!(wholeText.isEmpty())) {
					int msgStartIndex = wholeText.indexOf(messageToAddWithoutValue);
					if (msgStartIndex != -1) {
						int msgStartEndlIndex = wholeText.indexOfChar(msgStartIndex, '\n');
						if (msgStartEndlIndex != -1) {
							if (msgStartIndex < msgStartEndlIndex && msgStartEndlIndex < wholeText.length()) {
								String parsedText = wholeText.replace(wholeText.substring(msgStartIndex, msgStartEndlIndex + 1),
								messageToAdd+"\n");
								m_messagesEditor->setText(parsedText);
							}
						}
					}
				}
			}
	}
	/*re-assign the box with the new messages*/
	((MessageTextEditorWindow*)getParentComponent())->setOscMessages(getText());
}

void TextEditorContentComponent::buttonClicked (Button* buttonThatWasClicked){

	if(buttonThatWasClicked == m_validButton){

		/*pour message entrés à la main, attention il faut un saut de ligne à la fin */	string tmp1;
		istringstream iss1( (string)getText() );
		while(getline(iss1, tmp1, '\n')){
		}
		if(tmp1 != "") setText(m_messagesEditor->getText() + String("\n"));
		/*fin*/
		((MessageTextEditorWindow*)getParentComponent())->setOscMessages(getText());
		((MessageTextEditorWindow*)getParentComponent())->setVisible(false);
		((MessageTextEditorWindow*)getParentComponent())->getConcerned()->initInterpolationWindow();
		
		((MessageTextEditorWindow*)getParentComponent())->m_isTextEditorModified = false;
	}

	if(buttonThatWasClicked == m_saveButton)
		((MessageTextEditorWindow*)getParentComponent())->saveSnapshot(getText());

	if(buttonThatWasClicked == m_loadButton)
		((MessageTextEditorWindow*)getParentComponent())->loadSnapshot();

	if(buttonThatWasClicked == m_clearButton)
		m_messagesEditor->clear();

}

TextEditor* TextEditorContentComponent::getTextEditor(){ 
	return m_messagesEditor;
}

void TextEditorContentComponent::mouseExit(const MouseEvent &e){
	m_messagesEditor->setCaretPosition(m_messagesEditor->getText().length());
}

void TextEditorContentComponent::textEditorTextChanged (TextEditor &editor){
	((MessageTextEditorWindow*)getParentComponent())->m_isTextEditorModified = true;
}

void TextEditorContentComponent::textEditorReturnKeyPressed (TextEditor &editor){
}

void TextEditorContentComponent::textEditorEscapeKeyPressed (TextEditor &editor){
}

void TextEditorContentComponent::textEditorFocusLost (TextEditor &editor){
}
