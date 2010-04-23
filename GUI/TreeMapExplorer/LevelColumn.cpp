/*
 *  LevelColumn.cpp
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

#include "LevelColumn.h"
#include "ColumnExplorerComponent.h"
#include "Commons.h"
#include "Explorer.h"
#include <vector>

//////////////////////////////////////////////////////////////////
//Under class representing a column cell
//////////////////////////////////////////////////////////////////
class CellComponent : public Component, public ButtonListener
{
public:
	CellComponent (LevelColumn* owner_)
	: owner (owner_)
	{
		isSelected = false;
		
		addAndMakeVisible (m_toggleButton = new ToggleButton (String::empty));
		m_toggleButton->addButtonListener(this);
		
	}
	
	CellComponent()
	{
		deleteAllChildren();
	}
	
	void paint(Graphics &g)
	{
		g.fillAll(JUCE_BACK_COLOUR);

		if (isTargeted) {
			g.fillAll (JUCE_TARGET_COLOUR);
		}
		if (isSelected) {
			g.fillAll(JUCE_SELECT_COLOUR);	
		}
		
		g.setColour (Colours::white);
		g.setFont (font);
		g.drawText (m_text.data(), 2, 0, getWidth() - 4, getHeight(), Justification::centredLeft, true);		

		if (!isALeaf)
			g.drawArrow(getWidth()-20, getHeight()/2, getWidth()-10, getHeight()/2, 5, 5, 10);
	}
	
	void resized()
	{
		m_toggleButton->setBounds(getWidth()-60, 0, 30, 20);
	}
	
	void setRow(const int newRow, bool isRowSelected, bool isLeaf, bool isRowSelectedToAssign)
	{
		row = newRow;
		isTargeted = isRowSelected;
		isSelected = isRowSelectedToAssign;
		m_toggleButton->setToggleState(isRowSelectedToAssign, false);
		
		isALeaf = isLeaf;
		m_text = owner->getText(row).data();
		repaint();
	}
	
	void setState()
	{
		//owner->setState (row, columnId, true);
	}
	
	void buttonClicked(Button* buttonThatWasClicked)
	{
		owner->setState (row, m_toggleButton->getToggleState());
		isSelected = !isSelected;
		repaint();
	}
	
	void mouseDown(const MouseEvent& e)
	{
		if (e.mods.isPopupMenu()) {
			owner->listBoxItemClicked(row, e);
			owner->getCommons()->showPopupmenu(owner->getAbsoluteAddress(row));
			
		} else {
			owner->listBoxItemClicked(row, e);
		}
	}
	
private:
	LevelColumn *owner;
	
	Label *m_label;
	ToggleButton *m_toggleButton;
	
	string m_text;
	Font font;
	int row;
	bool isTargeted;
	bool isSelected;
	bool isALeaf;
};

//////////////////////////////////////////////////////////////////
//The LevelColumn class implementation
//////////////////////////////////////////////////////////////////

LevelColumn::LevelColumn(int levelNumber, Commons *commons, Explorer *explorer) : 
m_levelNumber(levelNumber), m_commons(commons), m_explorer(explorer)
{
	numRows = 0;
	
	// Create our table component and add it to this component..
	addAndMakeVisible (table = new ListBox (T(""), this));

	// give it a border
	table->setColour (ListBox::outlineColourId, JUCE_LINE_COLOUR);
	table->setOutlineThickness (1);
	
	// background color
	table->setColour(ListBox::backgroundColourId, Colours::darkgrey);
	
	//set a column header
	String levelType;
	if(levelNumber == 0)
		levelType = "DEVICES";
	
	Label *header =	new Label("", levelType);
	header->setSize(100, 20);
	header->setVisible(true);
	header->setColour(Label::backgroundColourId, Colours::grey);
	header->setJustificationType(Justification::horizontallyCentred);
	table->setHeaderComponent(header);
	table->setMultipleSelectionEnabled(true);	
}

LevelColumn::~LevelColumn()
{
	deleteAllChildren();
}

void LevelColumn::initialise(vector<string> _messages, vector<string> absoluteAddresses, vector<bool> childsAreLeaves)
{
	m_messages.clear();
	numRows = _messages.size();
	for (uint i = 0; i < _messages.size(); i++) {
		m_messages.push_back(_messages.at(i));
	}
	
	m_absoluteAddresses.clear();
	for (uint i = 0; i < absoluteAddresses.size(); i++) {
		m_absoluteAddresses.push_back(absoluteAddresses.at(i));
	}
	
	m_messagesAreLeaves.clear();
	for (uint i = 0; i < _messages.size(); i++) {
		m_messagesAreLeaves.push_back(childsAreLeaves.at(i));
	}

	//table->updateContent();
	//repaint();
}

void LevelColumn::clear()
{
	m_messages.clear();	
	m_messagesAreLeaves.clear();
	m_absoluteAddresses.clear();
	numRows = 0;

	table->updateContent();
}

void LevelColumn::selectAll(){
	//table->updateContent();
}

void LevelColumn::deselectAll(){
	table->deselectAllRows();
	table->updateContent();
}

Commons *
LevelColumn::getCommons()
{
	return m_commons;	
}

// This is overloaded from ListBoxModel, and must return the total number of rows in our table
int 
LevelColumn::getNumRows()
{
	return numRows;
}

// This is overloaded from ListBoxModel, and must paint any cells that aren't using custom
// components.
void 
LevelColumn::paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected)
{}

// This is overloaded from ListBoxModel, and must update any custom components that we're using
Component* 
LevelColumn::refreshComponentForRow (int rowNumber, bool isRowSelected,
												Component* existingComponentToUpdate)
{
	if (rowNumber < numRows) {
		CellComponent* cellComp = (CellComponent*) existingComponentToUpdate;
		
		// If an existing component is being passed-in for updating, we'll re-use it, but
		// if not, we'll have to create one.		
		if (cellComp == 0)
			cellComp = new CellComponent (this);
		
//		cout << "numRows : " << numRows << endl;
//		cout << "row : " << rowNumber << endl;
//		cout << "row selected : " << isRowSelected << endl;
		
		bool isRowSelectedToAssign = m_commons->isSelectedToAssign(m_absoluteAddresses[rowNumber]);
		cellComp->setRow(rowNumber, isRowSelected, m_messagesAreLeaves.at(rowNumber), isRowSelectedToAssign);

		return cellComp;
	}
	else
		return 0;
}

string 
LevelColumn::getText (const int rowNumber) const
{
	return m_messages.at(rowNumber);
}

string
LevelColumn::getAbsoluteAddress(const int rowNumber) const
{
	return m_absoluteAddresses[rowNumber];	
}

void 
LevelColumn::setState (const int rowNumber, bool newState)
{
	vector<string> *subTree = new vector<string>;
	m_explorer->getSubTreeAddresses(m_absoluteAddresses[rowNumber], subTree);
	for (uint i = 0; i < subTree->size(); i++) {
		m_commons->setSelectedToAssign(subTree->at(i), newState, true);
	}
	
	//cout << m_absoluteAddresses[rowNumber] << " " << newState << endl;
	m_commons->setSelectedToAssign(m_absoluteAddresses[rowNumber], newState, true);	
	
	m_commons->refresh(true);
	
}

void
LevelColumn::refresh()
{
	table->updateContent();	
}

void 
LevelColumn::resized()
{
	table->setBounds(0, 0, getWidth(), getHeight());
}

void 
LevelColumn::listBoxItemClicked(int row, const MouseEvent &e)
{
	table->selectRow(row);
	((ColumnExplorerComponent*)getParentComponent())->update(m_levelNumber, getText(row));
}

void 
LevelColumn::listBoxItemDoubleClicked(int row, const MouseEvent &e){}

void 
LevelColumn::selectedRowsChanged(int lastRowSelected){}

string 
LevelColumn::getSelectedRow()
{
	int row;
	for (int i = 0; i < getNumRows(); i++) {
		if (table->isRowSelected(i)) {
			row = i;
		}
	}
	
	return getText(row);
}

