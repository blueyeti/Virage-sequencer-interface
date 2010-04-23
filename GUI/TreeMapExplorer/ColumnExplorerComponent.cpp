/*
 *  ColumnExplorerComponent.cpp
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

#include "ColumnExplorerComponent.h"
#include "Explorer.h"
#include "Commons.h"
#include "TreeComm.h"
#include "LevelColumn.h"

ColumnExplorerComponent::ColumnExplorerComponent(Explorer *explorer, Commons *commons)
: m_explorer(explorer), m_commons(commons), font (14.0f)
{
	maxDepth = m_explorer->getTreeTotalDepth();
	
	//first column always displayed (DEVICES)
	vector<string> devicesVect = m_explorer->getDevices();
	
	vector<bool> areLeaves;
	for (uint i = 0; i < devicesVect.size(); i++) {
		m_selectionState[devicesVect.at(i)] = false;
		areLeaves.push_back(false);
	}
	
	LevelColumn *devices = new LevelColumn(0, m_commons, m_explorer);
	devices->initialise(devicesVect, devicesVect, areLeaves);
	columns.push_back(devices);

	for (int i = 1; i < maxDepth; i++)
		columns.push_back(new LevelColumn(i, m_commons, m_explorer));
	
	for (uint i = 0; i < columns.size(); i++) {
		addAndMakeVisible(columns.at(i));
	}
	
	//setSize (600, 300);
}

ColumnExplorerComponent::~ColumnExplorerComponent()
{
	//delete m_explorer;
}

void 
ColumnExplorerComponent::paint (Graphics& g)
{}

void 
ColumnExplorerComponent::resized()
{
	for (uint i = 0; i < columns.size(); i++) {
		columns.at(i)->setBounds(i*(getWidth()/columns.size()), 0, getWidth()/columns.size(), getHeight());
	}
}

void
ColumnExplorerComponent::refreshByTreeMapExpl()
{
	for (uint i = 0; i < columns.size(); i++) {
		columns.at(i)->refresh();
	}
}

void
ColumnExplorerComponent::rebuild()
{
	columns.clear();
	
	vector<string> devicesVect = m_explorer->getDevices();
	vector<bool> areLeaves;
	for (uint i = 0; i < devicesVect.size(); i++) {
		areLeaves.push_back(false);	
	}
	
	
	LevelColumn *devices = new LevelColumn(0, m_commons, m_explorer);
	devices->initialise(devicesVect, devicesVect, areLeaves);
	columns.push_back(devices);
	

	//columns.at(0)->initialise(devicesVect, devicesVect, areLeaves);
	
	for (int i = 1; i < m_explorer->getTreeTotalDepth(); i++)
		columns.push_back(new LevelColumn(i, m_commons, m_explorer));
	
	for (uint i = 0; i < columns.size(); i++) {
		addAndMakeVisible(columns.at(i));
	}
	
	for (uint i = 0; i < devicesVect.size(); i++) {
		update(0, devicesVect.at(i));	
	}

	for (uint i = 0; i < columns.size(); i++) 
		columns.at(i)->refresh();
	resized();
}

void
ColumnExplorerComponent::update(uint levelIndex, string relativePath)
{	
	string absolutePath = "";
	
	if (levelIndex < columns.size()-1) {
		for (uint i = 0; i < levelIndex; i++) {
			absolutePath += columns.at(i)->getSelectedRow();
		}
		absolutePath += relativePath;

		vector<string> childs = m_explorer->getChilds(absolutePath);
		vector<string> absoluteChildsAddresses;

		//that's to know if an element get childs or not in order to display a little arrow in the element cell
		vector<bool> childsAreLeaves;
		for (uint i = 0; i < childs.size(); i++) {
			childsAreLeaves.push_back((m_explorer->getChilds(absolutePath + childs.at(i))).size() == 0);
			
			absoluteChildsAddresses.push_back(absolutePath + childs.at(i));
		}
		
		//init the next level column with messages, level state
		columns.at(levelIndex+1)->initialise(childs, absoluteChildsAddresses, childsAreLeaves);
		
		//if a click is performed in a column, deselects items in the next one  
		columns.at(levelIndex+1)->deselectAll();
		
		//if a click is performed in a column, clears columns from the 2 next
		for (uint i = levelIndex+2; i < columns.size(); i++){
			columns.at(i)->clear();
		}
	}
	
	m_explorer->setTargetedAddressLabel(absolutePath);
}