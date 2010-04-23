/*
 *  Explorer.cpp
 *  JuceExplorer
 *
 *  Created by Laurent Garnier on 21/08/09.
 *  Copyright 2009 LIMSI. All rights reserved.
 *
 *
 *  This software is governed by the CeCILL license under French law and
 *  abiding by the rules of distribution of free software.  You can  use, 
 *  modify and/ or redistribute the software under the terms of the CeCILL
 *  license as circulated by CEA, CNRS and INRIA at the following URL
 *  "http://www.cecill.info". 
 *
 *  As a counterpart to the access to the source code and  rights to copy,
 *  modify and redistribute granted by the license, users are provided only
 *  with a limited warranty  and the software's author,  the holder of the
 *  economic rights,  and the successive licensors  have only  limited
 *  liability. 
 *
 *  In this respect, the user's attention is drawn to the risks associated
 *  with loading,  using,  modifying and/or developing or reproducing the
 *  software by the user in light of its specific status of free software,
 *  that may mean  that it is complicated to manipulate,  and  that  also
 *  therefore means  that it is reserved for developers  and  experienced
 *  professionals having in-depth computer knowledge. Users are therefore
 *  encouraged to load and test the software's suitability as regards their
 *  requirements in conditions enabling the security of their systems and/or 
 *  data to be ensured and,  more generally, to use and operate it in the 
 *  same conditions as regards security.
 
 *  The fact that you are presently reading this means that you have had
 *  knowledge of the CeCILL license and that you accept its terms.
 */

#include "Explorer.h"
#include "TreeComm.h"
#include "Commons.h"
#include "TreeMapCanvas.h"
#include "ColumnExplorerComponent.h"
#include "TimeBox.h"

Explorer::Explorer(TreeComm *treeComm, TimeBox *editedBox, ScanDeviceThread *scan, bool widerSize) :
m_tree(treeComm), m_editedBox(editedBox)
{
	m_commons = new Commons(this);
	
	m_treeMap = new TreeMapCanvas(this, m_commons, widerSize);
	addAndMakeVisible(m_treeMap); 
	
	addAndMakeVisible(m_targetedAddressLabel = new Label(T("Pointed Address"), T("Pointed Address : ")));
	m_targetedAddressLabel->setColour (Label::textColourId, Colours::white);
	m_targetedAddressLabel->setColour(Label::backgroundColourId, JUCE_TARGET_COLOUR);
	
	addAndMakeVisible (m_assignStartButton = new TextButton (String::empty));
    m_assignStartButton->setButtonText (T("<"));
	m_assignStartButton->addButtonListener (this);
    m_assignStartButton->setColour (TextButton::textColourId, Colours::darkgrey);
	m_assignStartButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	m_assignStartButton->setEnabled(false);
	
	addAndMakeVisible (m_updateButton = new NetButton (String::empty));
    m_updateButton->setButtonText (T("0"));
	m_updateButton->addButtonListener (this);
    m_updateButton->setColour (TextButton::textColourId, Colours::darkgrey);
	m_updateButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	
	addAndMakeVisible (m_assignEndButton = new TextButton (String::empty));
	m_assignEndButton->setButtonText (T(">"));
	m_assignEndButton->addButtonListener (this);
    m_assignEndButton->setColour (TextButton::textColourId, Colours::darkgrey);
	m_assignEndButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	m_assignEndButton->setEnabled(false);
	
	m_columnExpl = new ColumnExplorerComponent(this, m_commons);
	addAndMakeVisible(m_columnExpl);
	
//	if (widerSize) {
//		setSize(EXPLORER_WIDTH, EXPLORER_HEIGHT);
//	} 
//	else {
		setSize(INCRUSTED_EXPLORER_WIDTH, INCRUSTED_EXPLORER_HEIGHT);
//	}

	m_scanThread = scan;
}

Explorer::~Explorer()
{	
	deleteAndZero(m_treeMap);
	deleteAndZero(m_columnExpl);
	deleteAndZero(m_assignStartButton);
	deleteAndZero(m_assignEndButton);
	deleteAndZero(m_updateButton);
	
	delete m_commons;
	m_tree->clear();
	delete m_tree;
}

void 
Explorer::setEditedTimeBox(TimeBox* edited)
{
	if (m_editedBox != edited) {
		m_editedBox = edited;
	
		//the treemap items pre-selection operation takes a long time when the tree contains a lot of items
		if (m_editedBox->getOscMessageOutStartVector()->size() > 0 || m_editedBox->getOscMessageOutStopVector()->size() > 0) {
			m_commons->setPreSelectedItems(m_editedBox);
		}
	}
	
	m_assignStartButton->setEnabled(true);
	m_assignEndButton->setEnabled(true);
}

void
Explorer::reinitValues(const std::string& absoluteAddress)
{
	std::vector<std::string> *snapshot = new std::vector<std::string>; 	
	getSnapshot(absoluteAddress, snapshot);
	
	if (snapshot->size() != 0) {
		for (uint i = 0; i < snapshot->size(); i++) {
			m_tree->reinitValue(snapshot->at(i));
			m_commons->setMessageValue(snapshot->at(i), m_tree->getTreeElementValue(snapshot->at(i)));
		}
	} else {
		m_tree->reinitValue(absoluteAddress);
		m_commons->setMessageValue(absoluteAddress, m_tree->getTreeElementValue(absoluteAddress));
	}
}

void 
Explorer::deviceSendMessage(const std::string& message)
{
	m_tree->sendMessage(message);	
}

void
Explorer::networkTreeUpdate()
{	
	m_tree->clear();
	//m_scanThread->runThread();
	m_tree->init();
}	

void
Explorer::update()
{		
	m_treeMap->update();
	m_columnExpl->rebuild();
}

void 
Explorer::refresh(bool TreeMapToRefresh)
{
	if (TreeMapToRefresh) {
		m_treeMap->refresh();
	} else {
		m_columnExpl->refreshByTreeMapExpl();
	}
}

void 
Explorer::selectTreeMapItem(const std::string& absoluteAddress, bool isSelected)
{
	m_treeMap->addItemToSelection(absoluteAddress, isSelected);
}

void 
Explorer::setTargetedAddressLabel(const std::string& absoluteAddress)
{
	m_targetedAddressLabel->setText((String)("Pointed Address : ") + (String)(absoluteAddress.data()), false);
}

void 
Explorer::paint (Graphics& g){}

void 
Explorer::resized ()
{
	m_targetedAddressLabel->setBounds(0, 0, getWidth(), 12);
	m_treeMap->setBounds(0, 12, getWidth(), 2*getHeight()/3);
	m_assignStartButton->setBounds(0, 2*getHeight()/3+12, getWidth()/2-UPDATE_BUTTON_WIDTH/2, 30);
	m_updateButton->setBounds(getWidth()/2 - UPDATE_BUTTON_WIDTH/2, 2*getHeight()/3+12, UPDATE_BUTTON_WIDTH, 30);
	m_assignEndButton->setBounds(getWidth()/2 + UPDATE_BUTTON_WIDTH/2, 2*getHeight()/3+12, getWidth()/2, 30);
	m_columnExpl->setBounds(0, 2*getHeight()/3 + 30 + 12, getWidth(), getHeight()/3-30-12);
}

void 
Explorer::buttonClicked (Button* buttonThatWasClicked)
{
	String result = "";
	
	std::map<string, string> selectedLeavesValues = m_commons->getSelectedLeaves(); 
	std::map<string, string>::iterator it = selectedLeavesValues.begin();
	
	while (it != selectedLeavesValues.end()) {
		result += it->first.data() + String(" ") + it->second.data() + String("\n");
		it++;
	}
	
	if (buttonThatWasClicked == m_assignStartButton) {
		if (m_editedBox != NULL) {
			m_editedBox->updateMessagesEditor(1, result);
			m_editedBox->initInterpolationWindow();
		}
	}
	if (buttonThatWasClicked == m_assignEndButton) {
		if (m_editedBox != NULL) {
			m_editedBox->updateMessagesEditor(2, result);
			m_editedBox->initInterpolationWindow();
		}
	}
	if (buttonThatWasClicked == m_updateButton) {
		networkTreeUpdate();
		update();
	}
}

//////////////////////////////////////////////////////////////////
//Access Methods to TreeComm
//////////////////////////////////////////////////////////////////

int
Explorer::getTreeTotalDepth()
{
	return m_tree->getTreeTotalDepth();
}

std::string 
Explorer::getTreeElementLevel(const std::string& address)
{
	return m_tree->getTreeElementLevel(address);
}

std::string
Explorer::getTreeElementRelativeAddress(const std::string& address)
{
	return m_tree->getTreeElementRelativeAddress(address);
}

std::string
Explorer::getTreeElementValue(const std::string& address)
{
	return m_tree->getTreeElementValue(address);
}

std::string
Explorer::getTreeElementType(const std::string& address)
{
	return m_tree->getTreeElementType(address);	
}

std::string 
Explorer::getTreeElementRange(const std::string& address)
{
	return m_tree->getTreeElementRange(address);	
}


vector<string>
Explorer::getDevices()
{
	return m_tree->getDevices();
}

vector<string>
Explorer::getChilds(const std::string& address)
{
	vector<string> childs = m_tree->getChilds(address);
	vector<string> childsWithoutHiddenItems;

	for (uint i = 0; i < childs.size(); i++) {
		if (m_commons->m_hiddenItems.find(address + childs.at(i)) == m_commons->m_hiddenItems.end()) {
			childsWithoutHiddenItems.push_back(childs.at(i));	
		}
	}
	return childsWithoutHiddenItems;
}

void
Explorer::getSnapshot(const std::string& address, std::vector<std::string> *snapshot)
{
	m_tree->getSnapshot(address, snapshot);	
}

void 
Explorer::getSubTreeAddresses(const std::string& address, std::vector<std::string> *subTree)
{
	m_tree->getSubTreeAddresses(address, subTree);
}

bool 
Explorer::isFirstLeave(const std::string& address)
{
	return m_tree->isFirstLeave(address);
}

bool 
Explorer::isLastLeave(const std::string& address)
{
	return m_tree->isLastLeave(address);

}