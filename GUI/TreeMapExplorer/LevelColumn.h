/*
 *  LevelColumn.h
 *  TreeMapExplorer
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

/*!
 * \file LevelColumn.h
 * \author Laurent Garnier (LIMSI)
 * \date 2009-2010
 *
 * \brief This class inherits from the Juce ListBoxModel to draw a list box representing a tree level (siblings)
 *
 */

#ifndef _LevelColumn_H_
#define _LevelColumn_H_

#include "includes.h"
#include <string>
#include <vector> 

class Commons;
class Explorer;

using namespace std;

class LevelColumn : public Component, public ListBoxModel
{
public:
    LevelColumn(int levelNumber, Commons *Commons, Explorer *explorer);
    ~LevelColumn();
	
	void initialise(vector<string> childs, vector<string> absoluteAddresses, vector<bool> childsAreLeaves);
	void clear();
	void selectAll();	
	void deselectAll();
	
	Commons *getCommons();
	
    bool getState (const int rowNumber) const;
	void setState (const int rowNumber, bool newstate);
	string getText (const int rowNumber) const;
	void setText (const int rowNumber, const int columnId, String newText);
	string getAbsoluteAddress(const int rowNumber) const;
	
   	int getNumRows();
	
    void paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected);
	Component* refreshComponentForRow (int rowNumber, bool isRowSelected, Component* existingComponentToUpdate);
	
	void resized();
	void refresh();
	void listBoxItemClicked(int row, const MouseEvent &e);
	void listBoxItemDoubleClicked(int row, const MouseEvent &e);

	void selectedRowsChanged(int lastRowSelected);
	
	string getSelectedRow();
	
private:
	Commons *m_commons;
	Explorer *m_explorer;

    ListBox* table;    // the table component itself
    Font font;
    int numRows;       // The number of rows of data we've got
	int m_levelNumber;
	int m_rowSelected;
	
	vector<string> m_messages;
	vector<string> m_absoluteAddresses;
	vector<bool> m_messagesAreLeaves;
	
	vector<Colours> m_colours;
 
};

#endif
