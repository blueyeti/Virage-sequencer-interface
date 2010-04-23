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
 *  RackWindow.cpp
 *  Virage
 *
 *  Created by Henry Bernard Bernard on 25/09/08.
 *  Copyright 2008 by Blue Yeti. All rights reserved.
 *
 */

#include "RackWindow.h"

class RackComponent  : public ListBox, public ListBoxModel
{
private:
	String* toolName;
	
public:
    //==============================================================================
    RackComponent()
	: ListBox (String::empty, 0)
    {
        // tells the ListBox that this object supplies the info about
        // its rows.
        setModel (this);
		setMultipleSelectionEnabled (true);
		
		toolName = new String[getNumRows()];
		
		toolName[0]=T("Box");
		toolName[1]=T("Trigger");
		
		
	}
	
    ~RackComponent()
    {
    }
	
	//==============================================================================
	int getNumRows()
	{
		return 2;
	}

	void paintListBoxItem (int rowNumber,
					   Graphics& g,
					   int width, int height,
					   bool rowIsSelected)
	{
		if (rowIsSelected) {	g.fillAll (s_COLOUR_UNSELECTION);}
	
		g.setColour (Colours::white);
		g.setFont (height * 0.7f);
		
		switch (rowNumber) {
			case 0:
				g.fillRect(width/2-20,height/2-10,40,20);
				g.setColour(Colours::black);
				g.drawRect(width/2-18, height/2-7, 36, 16, 1);
				break;
			case 1:
				g.fillEllipse(width/2-8,height/2-8,16,16);
				g.fillRect(width/2-8, height/2-8, 8, 8);
				g.setColour(Colours::black);
				g.drawEllipse(width/2-2, height/2-2, 4, 4, 1);
				break;
			default:
				break;
		}
		
		g.setColour(Colours::white);
		g.drawText (toolName[rowNumber], 5, 0, width, height, Justification::centredLeft, true);
	}

	const String getDragSourceDescription (const SparseSet<int>& selectedRows)
	{
	// for our drag desctription, we'll just make a list of the selected
	// row numbers - this will be picked up by the drag target and displayed in
	// its box.
	String desc;
	
		for (int i = 0; i < selectedRows.size(); ++i) {
			desc << (selectedRows [i] + 1) << T(" ");
		}
		
		return desc.trim();
	}

	//==============================================================================
	// this just fills in the background of the listbox
	void paint (Graphics& g)
	{
		g.setColour(Colours::black.withAlpha (0.7f));
		g.fillRoundedRectangle(0,0,getWidth(), getHeight(), 8);
	}
	
	
	void mouseDown (const MouseEvent& e)
	{
		getParentComponent()->mouseDown(e);
	}
	
	void  mouseDrag (const MouseEvent& e) {
		getParentComponent()->mouseDrag(e);
	}

/*void listBoxItemClicked (int row, const MouseEvent& e)
 {
 PopupMenu m;
 m.addItem (1, "sdfsdfs");
 
 m.show();
 
 //AlertWindow::showMessageBox (AlertWindow::InfoIcon, "asdfsadfads", "srdfsdfa");
 DocumentWindow* dw = new DocumentWindow ("sfdsd", Colours::white, DocumentWindow::allButtons, true);
 dw->setBounds (100, 100, 500, 500);
 dw->setVisible (true);
 }*/
	
};



RackWindow::RackWindow(ComponentDragger* _mainWindow)
{
	contentComp = new RackComponent();
	addAndMakeVisible(contentComp);
	
	m_mainWindow = _mainWindow;
	
	setSize(200,100);
};

RackWindow::~RackWindow(){
	deleteAllChildren();
};

void RackWindow::mouseDown (const MouseEvent& /*e*/)
{
	m_mainWindow->startDraggingComponent (this, 0);
}

void RackWindow::mouseDrag (const MouseEvent& e)
{ 
	/*if (! getParentComponent()->isValidComponent())
	{
		delete this;
	}
	else
	*/{
		MouseEvent e2 (e.getEventRelativeTo(getParentComponent()));
	
		// if the mouse is inside the parent component, we'll make that the
		// parent - otherwise, we'll put this comp on the desktop.
		if (getParentComponent()->isValidComponent() && e2.x >= 0 && e2.y >= 0 && e2.x < getParentComponent()->getWidth() && e2.y < getParentComponent()->getHeight())
		{
			// re-add this component to a parent component, which will
			// remove it from the desktop..
			getParentComponent()->addChildComponent (this);
		}
		else
		{
			// add the component to the desktop, which will remove it
			// from its current parent component..
			addToDesktop (ComponentPeer::windowIsTemporary);
		}
	
		m_mainWindow->dragComponent (this, e);
	}
}

void RackWindow::resized() {
	contentComp->setBounds(0,0,getWidth(),getHeight());
}
