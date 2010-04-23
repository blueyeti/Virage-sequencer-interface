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
 *  CommComponent.cpp
 *  Virage
 *
 *  Created by Laurent Garnier on 07/04/09.
 *  Copyright 2009 BlueYeti.Fr. All rights reserved.
 *
 */

#include "CommComponent.h"

CommComponent::CommComponent ()
{
	m_currentComment = new vector<String>;
	m_availableComment = new vector<String>;
	
	addAndMakeVisible (m_currentCommentFrame = new TextEditor ());
	m_currentCommentFrame->setBorder(5);
    m_currentCommentFrame->setMultiLine (true);
    m_currentCommentFrame->setReadOnly (true);
	m_currentCommentFrame->setColour(TextEditor::backgroundColourId, Colour(0x000F0F0F));
	m_currentCommentFrame->setColour(TextEditor::textColourId, Colours::white);
	
	addAndMakeVisible (m_availableCommentFrame = new TextEditor ());
	m_availableCommentFrame->setBorder(5);
    m_availableCommentFrame->setMultiLine (true);
    m_availableCommentFrame->setReadOnly (true);
	m_availableCommentFrame->setColour(TextEditor::backgroundColourId, Colours::darkgrey);
	m_availableCommentFrame->setColour(TextEditor::textColourId, Colours::white);
	
	setSize(300, 400);
}

CommComponent::~CommComponent()
{
    deleteAllChildren();
}

//==============================================================================
void CommComponent::paint (Graphics& g){
	g.fillAll(s_COLOUR_BACKGROUND);
}

void CommComponent::resized()
{
	 m_currentCommentFrame->setBounds (5, 5, getWidth()-10, getHeight()/2-5);
	m_availableCommentFrame->setBounds (5, getHeight()/2+5, getWidth()-10, getHeight()/2-10);
	repaint();
}

void CommComponent::refresh() {
	
	clear();
	
	//• current comment frame refresh
	vector<String>::iterator it = m_currentComment->begin();
	while(it != m_currentComment->end()){
		String comment = *it;
		
		if(!m_currentCommentFrame->getText().contains(comment)){
			m_currentCommentFrame->setCaretPosition(m_currentCommentFrame->getText().length());
			m_currentCommentFrame->insertTextAtCursor(comment + "\n");
		}
		it++;
	}
	
	
	//• available comment frame refresh
	vector<String>::iterator it2 = m_availableComment->begin(); 
	while(it2 != m_availableComment->end()){

		String comment = *it2;
		
		if(!m_availableCommentFrame->getText().contains(comment)){
			m_availableCommentFrame->setCaretPosition(m_availableCommentFrame->getText().length());
			m_availableCommentFrame->insertTextAtCursor(comment + "\n");
		}
		it2++;
	}
	
	repaint();
}


void CommComponent::fillComments(String _comments){
	m_currentCommentFrame->insertTextAtCursor(_comments);
	repaint();
}

void CommComponent::clear(){
	
	m_availableCommentFrame->clear();
	m_currentCommentFrame->clear();
	repaint();
}

void CommComponent::reset(){
	
	m_availableComment->clear();
	m_currentComment->clear();
	repaint();
}

//==============================================================================
void CommComponent::appendCurrentComment(String _comment){
	removeFromAvailableComment(_comment);
	m_currentComment->push_back(_comment);	
}


void CommComponent::appendAvailableComment(String _comment){
	/*ATTENTION AUX DOUBLONS*/
	std::vector<String>::iterator itVectorData;
	for(itVectorData = m_availableComment->begin(); itVectorData != m_availableComment->end(); itVectorData++)		
		if( *itVectorData == _comment) return;
		
	/*PAS DE DOUBLONS ? : */
	m_availableComment->push_back(_comment);
}

void CommComponent::removeFromCurrentComment(String _comment){
	
	std::vector<String>::iterator itVectorData;
	for(itVectorData = m_currentComment->begin(); itVectorData != m_currentComment->end(); itVectorData++)
	{
		if( *itVectorData == _comment) {
			m_currentComment->erase(itVectorData);
			return;
		}
	}	
}

void CommComponent::removeFromAvailableComment(String _comment){
	
	std::vector<String>::iterator itVectorData;
	for(itVectorData = m_availableComment->begin(); itVectorData != m_availableComment->end(); itVectorData++)
	{
		if( *itVectorData == _comment) {
			m_availableComment->erase(itVectorData);
			return;
		}
	}
}



