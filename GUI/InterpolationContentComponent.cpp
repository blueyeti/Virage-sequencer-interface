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
 *  InterpolationContentComponent.cpp
 *  Virage
 *
 *  Created by Laurent Garnier on 22/04/09.
 *  Copyright 2009 __BlueYeti__. All rights reserved.
 */

#include "InterpolationContentComponent.h"

//==============================================================================
// This is a custom component containing a radio button, which we're going to put inside our table's "interpolation" column.
class InterpolColumnCustomComponent : public Component, public ButtonListener
{
public:
	InterpolColumnCustomComponent (InterpolationContentComponent* owner_)
	: owner (owner_)
	{
		addAndMakeVisible (m_toggleButton = new ToggleButton (String::empty));
		m_toggleButton->addButtonListener(this);
	}

	~InterpolColumnCustomComponent()
	{
		deleteAllChildren();
	}

	void resized()
	{
		m_toggleButton->setBoundsInset (BorderSize (2));
	}

	void setRowAndColumn (const int newRow, const int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		m_toggleButton->setToggleState(owner->getState(row, columnId), false);
	}

	void setRowAndColumn (const int newRow, const int newColumn, bool isSelected)
	{
		row = newRow;
		columnId = newColumn;
		if(isSelected)
			m_toggleButton->setToggleState(/*owner->getState(row, columnId)*/true, false);
		else
			m_toggleButton->setToggleState(owner->getState(row, columnId), false);
	}

	void setState()
	{
		owner->setState (row, columnId, true);
	}

  void buttonClicked(Button* /*buttonThatWasClicked*/)
	{
  	owner->setState (row, columnId, m_toggleButton->getToggleState());
	}

private:
	InterpolationContentComponent* owner;
	ToggleButton* m_toggleButton;
	int row, columnId;
};

class LabelColumnCustomComponent : public Component, public LabelListener
{
public:
	LabelColumnCustomComponent (InterpolationContentComponent* owner_)
	: owner (owner_)
	{
		addAndMakeVisible(m_label = new Label(T(""), T("")));
		m_label->setEditable(false, true);
		m_label->addListener(this);
	}

	LabelColumnCustomComponent()
	{
		deleteAllChildren();
	}

	void resized()
	{
		m_label->setBounds(0, 0, 50, 20);
	}

	void setRowAndColumn (const int newRow, const int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		m_label->setText(owner->getText(row, columnId), false);
	}

  virtual void labelTextChanged (Label* /*labelThatHasChanged*/){
		owner->setText(row, columnId, m_label->getText(true));
	}

private:
	InterpolationContentComponent* owner;
	Label* m_label;
	int row, columnId;
};

InterpolationContentComponent::InterpolationContentComponent()
: font (14.0f)
{
	// Create our table component and add it to this component..
	addAndMakeVisible (table = new TableListBox (T("interpolation table"), this));

	// give it a border
	table->setColour (ListBox::outlineColourId, Colours::grey);
	table->setOutlineThickness (1);

	// Add some columns to the table header
	table->getHeader()->addColumn ("Messages", 1, 300, 250, 400, TableHeaderComponent::defaultFlags);
	table->getHeader()->addColumn ("Start Value", 2, 100, 100, 100, TableHeaderComponent::defaultFlags);
	table->getHeader()->addColumn ("End Value", 3, 100, 100, 100, TableHeaderComponent::defaultFlags);
	table->getHeader()->addColumn ("Interpolation", 4, 100, 100, 100, TableHeaderComponent::defaultFlags);
	table->getHeader()->addColumn ("Frequency", 5, 100, 100, 100, TableHeaderComponent::defaultFlags);
	table->getHeader()->addColumn ("Redundancy", 6, 100, 100, 100, TableHeaderComponent::defaultFlags);

	// we could now change some initial settings..
	table->getHeader()->setSortColumnId (1, true); // sort forwards by the ID column
	//table->getHeader()->setColumnVisible (7, false); // hide the "length" column until the user shows it

	// un-comment this line to have a go of stretch-to-fit mode
	table->getHeader()->setStretchToFitActive (true);

	table->setMultipleSelectionEnabled (true);

	addAndMakeVisible(m_validButton = new TextButton(String::empty));
    m_validButton->setButtonText (T("OK"));
    m_validButton->addButtonListener (this);
	m_validButton->setColour (TextButton::buttonColourId, Colours::white);
	m_validButton->setColour (TextButton::textColourId, Colours::black);

}

InterpolationContentComponent::~InterpolationContentComponent()
{
	deleteAllChildren();
}

void InterpolationContentComponent::initialise(vector<string> _interpolableMessages, vector<string> _startValues, vector<string> _endValues){
	m_interpolableMessages = _interpolableMessages;
	m_startValues = _startValues;
	m_endValues = _endValues;

	numRows = _interpolableMessages.size();
	m_frequency = "10";

	states = new bool[numRows];
	for(int i = 0; i <  numRows; i++) {
		//by default messages are interpolated only if message values from start vs end change
		if(m_startValues.at(i).compare(m_endValues.at(i)) != 0){
			states[i] = true;
		}
		else {
			states[i] = false;
		}
	}

	redondance = new bool[numRows];
	for(int i = 0; i <  numRows; i++) {
		redondance[i] = false;
	}

	//by default messages are automatically interpolated
	sendInterpolatedMessages();

	table->updateContent();
}

void InterpolationContentComponent::selectAll(){
	for(int i = 0; i < getNumRows(); i++)
		if(m_startValues.at(i).compare(m_endValues.at(i)) != 0)
			states[i] = true;

	table->updateContent();
}

void InterpolationContentComponent::deselectAll(){


	if(getNumRows()==0) return;

	for(int i = 0; i < getNumRows(); i++)  states[i] = false;

	table->updateContent();
}

void InterpolationContentComponent::sendInterpolatedMessages(){
	//collect the messages to interpolate and send them to engine
	vector<string> addresses;
	vector<bool> redondances;
	for(int i = 0; i < getNumRows(); i++){
		if(getState(i, 4)){
			addresses.push_back(m_interpolableMessages.at(i));
			redondances.push_back(redondance[i]);
		}
	}
	
	((MessageInterpolationWindow*)getParentComponent())->sendInterpolatedMessages(addresses, redondances, m_frequency.getIntValue());
}

// This is overloaded from TableListBoxModel, and must return the total number of rows in our table
int InterpolationContentComponent::getNumRows()
{
	return numRows;
}

// This is overloaded from TableListBoxModel, and should fill in the background of the whole row
void InterpolationContentComponent::paintRowBackground (Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
{
	if (rowIsSelected)
		g.fillAll (Colours::lightblue);

	if (getState(rowNumber, 4) && !rowIsSelected)
		g.fillAll(Colours::lightskyblue);
}

// This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
// components.
void InterpolationContentComponent::paintCell (Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/)
{
	g.setColour (Colours::black);
	g.setFont (font);

	if(columnId == 1)
		g.drawText ((String)m_interpolableMessages.at(rowNumber).data(), 2, 0, width - 4, height, Justification::centredLeft, true);

	g.setColour (Colours::black.withAlpha (0.2f));
	g.fillRect (width - 1, 0, 1, height);
}

// This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header
// to change the sort order.
void InterpolationContentComponent::sortOrderChanged (int /*newSortColumnId*/, const bool /*isForwards*/)
{
/*	if (newSortColumnId != 0)
	{
		DemoDataSorter sorter (getAttributeNameForColumnId (newSortColumnId), isForwards);
		dataList->sortChildElements (sorter);

		table->updateContent();
	}*/
}


// This is overloaded from TableListBoxModel, and must update any custom components that we're using
  Component* InterpolationContentComponent::refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate)
{
	if(columnId == 2){
		LabelColumnCustomComponent* startValueLabel = (LabelColumnCustomComponent*) existingComponentToUpdate;

		// If an existing component is being passed-in for updating, we'll re-use it, but
		// if not, we'll have to create one.
		if (startValueLabel == 0)
			startValueLabel = new LabelColumnCustomComponent (this);

		startValueLabel->setRowAndColumn (rowNumber, columnId);

		return startValueLabel;
	}

	else if(columnId == 3){
		LabelColumnCustomComponent* endValueLabel = (LabelColumnCustomComponent*) existingComponentToUpdate;

		if (endValueLabel == 0)
			endValueLabel = new LabelColumnCustomComponent (this);

		endValueLabel->setRowAndColumn (rowNumber, columnId);

		return endValueLabel;
	}

	else if (columnId == 4)
	{
		InterpolColumnCustomComponent* interpolState = (InterpolColumnCustomComponent*) existingComponentToUpdate;

		if (interpolState == 0)
			interpolState = new InterpolColumnCustomComponent (this);

		interpolState->setRowAndColumn (rowNumber, columnId);

		return interpolState;
	}

	else if (columnId == 5)
	{
		LabelColumnCustomComponent* frequencyLabel = (LabelColumnCustomComponent*) existingComponentToUpdate;

		if (frequencyLabel == NULL)
			frequencyLabel = new LabelColumnCustomComponent (this);

		frequencyLabel->setRowAndColumn (rowNumber, columnId);

		return frequencyLabel;
	}

	else if (columnId == 6)
	{
		InterpolColumnCustomComponent* interpolRepeat = (InterpolColumnCustomComponent*) existingComponentToUpdate;

		if (interpolRepeat == 0)
			interpolRepeat = new InterpolColumnCustomComponent (this);

		interpolRepeat->setRowAndColumn (rowNumber, columnId);

		return interpolRepeat;
	}

	else
	{
		// for any other column, just return 0, as we'll be painting these columns directly.
		jassert (existingComponentToUpdate == 0);
		return 0;
	}
}

// A couple of quick methods to set and get the "interpolation state"
bool InterpolationContentComponent::getState (const int rowNumber, const int columnId) const
{
	if(columnId == 4)
		return states[rowNumber];
	if(columnId == 6)
		return redondance[rowNumber];

	return false;
}

void InterpolationContentComponent::setState (const int rowNumber, const int columnId, bool newState)
{
	if(columnId == 4){
		states[rowNumber] = newState;
		table->updateContent();
	}
	if(columnId == 6)
		redondance[rowNumber] = newState;
}


String InterpolationContentComponent::getText (const int rowNumber, const int columnId) const
{
	if(columnId == 2)
		return (String)m_startValues.at(rowNumber).data();
	if(columnId == 3)
		return (String)m_endValues.at(rowNumber).data();
	if(columnId == 5)
		return m_frequency;

	return String::empty;
}

void InterpolationContentComponent::setText (const int rowNumber, const int columnId, String newText)
{
	if(columnId == 2)
		m_startValues.at(rowNumber) = newText;
	if(columnId == 3)
		m_endValues.at(rowNumber) = newText;
	if(columnId == 5)
		m_frequency = newText;
}

int InterpolationContentComponent::getInterpolationFrequency(){
	return m_frequency.getIntValue();
}

void InterpolationContentComponent::resized()
{
	table->setBounds(0, 0, getWidth(), getHeight()-30);

	m_validButton->setBounds(0, getHeight()-30, 50, 30);
	m_validButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
}

void InterpolationContentComponent::cellDoubleClicked(int rowNumber, int columnId, const MouseEvent &/*e*/){
	if(columnId == 1){
		setState(rowNumber, 4, !getState(rowNumber, 4));
	}
	table->updateContent();
}

void InterpolationContentComponent::buttonClicked(Button* buttonThatWasClicked){
	if(buttonThatWasClicked == m_validButton){
		//update message values in message editors
		vector<string> startMessages;
		vector<string> endMessages;

		for(int i = 0; i < getNumRows(); i++){
			startMessages.push_back(m_interpolableMessages.at(i) +" "+ m_startValues.at(i));
			endMessages.push_back(m_interpolableMessages.at(i) +" "+ m_endValues.at(i));
		}
		((MessageInterpolationWindow*)getParentComponent())->updateMessagesEditor(1, startMessages);
		((MessageInterpolationWindow*)getParentComponent())->updateMessagesEditor(2, endMessages);

		sendInterpolatedMessages();
		((MessageInterpolationWindow*)getParentComponent())->closeButtonPressed();
	}
}
