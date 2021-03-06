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
 *  InterpolationContentComponent.h
 *  Virage
 *
 *  Created by Laurent Garnier on 22/04/09.
 *  Copyright 2009 __BlueYeti__. All rights reserved.
 *
 */

#ifndef _INTERPOLATIONCONTENTCOMPONENT_H_
#define _INTERPOLATIONCONTENTCOMPONENT_H_

#include "includes.h"

#include "MessageInterpolationWindow.h"
#include <string>
#include <vector> 

using namespace std;

class InterpolationContentComponent : public Component, public TableListBoxModel, public ButtonListener
{
private:
    TableListBox* table;    // the table component itself
	TextButton* m_validButton;
    Font font;
    int numRows;            // The number of rows of data we've got
	
	vector<string> m_interpolableMessages;
	vector<string> m_startValues;
	vector<string> m_endValues;
	bool* states;
	bool* redondance;
	String m_frequency;
	//String* m_frequency;
	
public:
    InterpolationContentComponent();
    ~InterpolationContentComponent();
	
	void initialise(vector<string> _interpolableMessages, vector<string> _startValues, vector<string> _endValues);
	void selectAll();	
	void deselectAll();

    bool getState (const int rowNumber, const int columnId) const;
	void setState (const int rowNumber, const int columnId, bool newstate);

	String getText (const int rowNumber, const int columnId) const;
	void setText (const int rowNumber, const int columnId, String newText);
	
	int getInterpolationFrequency();
	
	void sendInterpolatedMessages();
	
   	int getNumRows();
    void paintRowBackground (Graphics& g, int rowNumber, int width, int height, bool rowIsSelected);
    void paintCell (Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected);
	
    void sortOrderChanged (int newSortColumnId, const bool isForwards);
    Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate);
	
	void resized();
	void buttonClicked(Button* buttonThatWasClicked);
	void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent &e);
};

#endif
