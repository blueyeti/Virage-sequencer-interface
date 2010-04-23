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
 ==============================================================================
 
 This is an automatically generated file created by the Jucer!
 
 Creation date:  5 Feb 2009 7:38:08 pm
 
 Be careful when adding custom code to these files, as only the code within
 the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
 and re-saved.
 
 Jucer version: 1.11
 
 ------------------------------------------------------------------------------
 
 The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
 Copyright 2004-6 by Raw Material Software ltd.
 
 ==============================================================================
 */

#ifndef __JUCER_HEADER_NetComponent_NetComponent_5EB0F9AF__
#define __JUCER_HEADER_NetComponent_NetComponent_5EB0F9AF__

//[Headers]     -- You can add your own extra header files here --
#include "includes.h"
#include "TimeBox.h"
#include <string>
#include "deviceManager.h"
#include "Engines.hpp"

#include "TreeMapExplorerWindow.h"
#include "TreeComm.h"
#include "Explorer.h"

class MainWindow;

using namespace std;
//[/Headers]

class NetComponent  : public ButtonListener, public Component,  public DragAndDropContainer
{
public:
    //==============================================================================
    NetComponent (Engines* _engines);
    ~NetComponent();
	
	void paint (Graphics& g);
    void resized();
	void mouseDrag(const MouseEvent& e);
	bool keyPressed	( const KeyPress & 	key);
	void buttonClicked (Button* buttonThatWasClicked);
	void changeListenerCallback(void* objectThatHasChanged);
	void visibleAreaChanged(int visibleX, int visibleY, int visibleW, int visible);
	
    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setMainWindow(MainWindow* _mainWindow);
	void setEditedBox(TimeBox* _editedBox);
	TimeBox* getEditedBox();
	
	void putNewStateToCapture(string _new);
	void setStateSnapShot(vector<std::string> _sss);
	void updateNamespaceSelection();
	
	Engines* getEngines();
	PropertyPanel* getPanel();
	TreeView* getTreeView();
	void flipToDeviceEditor();
	void addDevice(string _name, string _protocole, string _ip, string _port);
	void modifyDevice(string _oldName, string _name, string _protocole, string _ip, string _port);
	
	std::vector<Device*> getDeviceList();
	//==============================================================================
    juce_UseDebuggingNewOperator
	
private:
	MainWindow* m_MainWindowForDialog;
	TimeBox* m_editedBox;
	std::vector<string> m_selectedItems; 
	PropertyPanel* m_netPropertyPanel;
	Array<PropertyComponent*>* m_propertiesArray;
	
	Engines* m_engines;
	
    TreeView* m_namespaceTree;
	Label* m_boxTarget;
    TextButton* m_assignLeft;
    TextButton* m_assignRight;
	
	NetButton* m_refresh;
	NetButton* m_addDeviceButton;
	
	int m_buttonBarHeight;

	//add a device
	deviceManager* m_addDeviceComponent;
	std::vector<Device*> m_deviceList;
	
	/*TreeMapExporer added code*/
	TextButton* m_changeExplorerButton;
	NetButton* m_separateTreeMapWindowButton;
	
	TreeMapExplorerWindow* m_treeMapExplorerWindow;
	Explorer* m_treeMapExplorerComponent;
	bool m_istreeMapUsed;
	bool m_istreeMapFullScreen;
	/***************************/
	
    // (prevent copy constructor and operator= being generated..)
    NetComponent (const NetComponent&);
    const NetComponent& operator= (const NetComponent&);
};


#endif   // __JUCER_HEADER_NetComponent_NetComponent_5EB0F9AF__
