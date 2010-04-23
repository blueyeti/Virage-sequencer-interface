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
#include "NetComponent.h"
#include "MainWindow.h"
#include "PropertyFrame.h"
#include "ScanDeviceThread.h" 

class TitleNetPropertyComponent : public TextPropertyComponent 
	{
	public:
		TitleNetPropertyComponent(NetComponent* _panel) : TextPropertyComponent(String("Title"), 100, false) 
		{ m_panel = _panel; };
		~TitleNetPropertyComponent()
		{};
		void 	setText (const String &newText)
		{
			if( m_panel->getEditedBox() != 0) {
				m_panel->getEditedBox()->setTitleText(getText(), newText);
			}
			
			refresh();
		}
		const String 	getText () const 
		{
			if( m_panel->getEditedBox() != 0) {
				return (m_panel->getEditedBox()->getName());
			}
			else {
				return String("nothing selected");
			}
		}
	private:
		NetComponent* m_panel;
	};

//tree nodes type
class MinuitTreeItem : public TreeViewItem 
	{
	protected:
		string m_address;
		bool m_isChildrenSelected;
		
	public:
		void setSemiSelected(bool _isChildrenSelected){
			
			m_isChildrenSelected = _isChildrenSelected;
			//itemSelectionChanged (true);
		}
		
		string getAddress(){
			return m_address;
		}
		
		/*Juce->DragAndDropContainer*/
		const String getDragSourceDescription(){
			
			return String(m_address.data());
		}
	};

class MinuitTreeAttribute : public MinuitTreeItem 
	{
	private:
		std::string m_name;
		std::string m_value;
		
	public:
		MinuitTreeAttribute(string /*_address*/, const string &name,  const string &value) {
			m_name = name;
			m_value = value;
		};
		~MinuitTreeAttribute(){};
		
		void paintItem	( Graphics & 	g, int 	width, int /*height*/) {
			g.setColour(Colours::black);
			g.fillRect(3,7,4, 4);
			g.setColour(Colours::white);
			g.drawRect(3,7,4, 4, 1);
			g.drawText(String(m_name.data()) + String(" : ") + String(m_value.data()), 12, 5, width, 10, Justification::left, false);
		}
		
		bool 	mightContainSubItems (){
			return false;
		}
		
	};

class MinuitTreeLeave : public MinuitTreeItem 
	{
	private:
		NetComponent* m_NetComponent;
		string m_name;
		string m_value;
		
	public:
		MinuitTreeLeave(NetComponent* _NetComponent, const string &address, const string &name) 
		{
			m_NetComponent = _NetComponent;
			m_name = name;
			m_address = address;
			m_isChildrenSelected = false;
			m_value = getMessageValue();
		}
		
		~MinuitTreeLeave()
		{}
		
		string getMessageValue()
		{
			vector<std::string> values;
			
			if(m_NetComponent->getEngines()!=0)
				values = m_NetComponent->getEngines()->requestNetworkSnapShot(m_address);
			
			if (values.size() > 0) {
				std::string result = values[0];
				size_t firstSpace = result.find_first_of(" ", 1);
				if (firstSpace != string::npos) {
					return result.substr(firstSpace, result.size());
				}
				else {
					return "NO VALUE";
				}
			} else {
				return "NO VALUE";
			}
		}
		
		void itemSelectionChanged(bool isNowSelected)
		{
			((MinuitTreeItem*)getParentItem())->setSemiSelected(isNowSelected);
		}
		
		void setSemiSelected(bool _isChildrenSelected)
		{
			
			m_isChildrenSelected = _isChildrenSelected;
		}
		
		bool mightContainSubItems()
		{
			return true;
		}
		
		void paintItem(Graphics& g, int width, int height) 
		{
			if(isSelected()) {
				g.setColour(s_COLOUR_UNSELECTION);
				g.setOpacity(0.3f);
				g.fillRect(0,0,width,height);
				g.setOpacity(1.0f);
			};
			g.setColour(Colours::lightgreen);
			g.fillEllipse(2,6,8,8);
			g.setColour(Colours::white);
			g.drawEllipse(2,6,8,8, 1);
			g.drawText(String(m_name.data()) + String(" : ") + String(m_value.data()), 12, 5, width, 10, Justification::left, false);
		}
		
		void itemOpennessChanged(bool isNowOpen) 
		{
			if(isNowOpen) {
				
				vector<std::string>* nodes = new vector<std::string>;
				vector<std::string>* leaves = new vector<std::string>;
				
				vector<std::string>* attributs = new vector<std::string>;
				vector<std::string>* values = new vector<std::string>;
				
				int state;
				int valueIndice;
				if (m_NetComponent->getEngines()!=0) {
					state = m_NetComponent->getEngines()->requestNetworkNamespace(m_address, nodes, leaves, attributs, values);
				}
				
				if (values->size() > 0) {
					for (unsigned int i = 0; i < attributs->size(); i++) {						
						if (attributs->at(i).compare("value") == 0) {
							valueIndice = i;
							m_value = values->at(valueIndice);
						}
					}
					
					for(unsigned int i = 0 ; i < attributs->size(); ++i) {
						MinuitTreeAttribute* added = new MinuitTreeAttribute(std::string("/")+std::string(attributs->at(i)), std::string(attributs->at(i)), std::string(values->at(i)));
						addSubItem(added);
					}
				}
				
				m_NetComponent->updateNamespaceSelection();
				
				delete nodes;
				delete leaves;
				delete attributs;
				delete values;
				
			}//end if isNowOpen
			else {
				clearSubItems();
			}
		}
	};

class MinuitTreeNode : public MinuitTreeItem 
	{
	private:
		NetComponent* m_NetComponent;
		string m_name;
		
	public:
		MinuitTreeNode(NetComponent* _NetComponent, const string &address, const string &name) {
			m_NetComponent = _NetComponent;
			m_name = name;
			m_address = address;
			m_isChildrenSelected = false;
		};
		~MinuitTreeNode(){};
		
		bool 	mightContainSubItems (){
			return true;
		}
		
		void itemSelectionChanged (bool isNowSelected){
			
			((MinuitTreeItem*)getParentItem())->setSemiSelected(isNowSelected);
			
		}
		
		void paintItem	( Graphics & 	g, int 	width, int 	height) {
			
			/*surligange sélection*/
			if(isSelected()) {
				g.setColour(s_COLOUR_UNSELECTION);
				g.setOpacity(0.3f);
				g.fillRect(0,0,width,height);
				g.setOpacity(1.0f);
			}
			else if(m_isChildrenSelected) {
				g.setColour(s_COLOUR_UNSELECTION);
				g.setOpacity(0.3f);
				
				//dessin des hachures
				for(int i = 0; i < width; i+=20){
					
					Path hashure;
					hashure.startNewSubPath(i, 0);
					hashure.lineTo(i+13, 0);
					hashure.lineTo(i+3, height);
					hashure.lineTo(i-10, height);
					hashure.closeSubPath();
					g.fillPath(hashure);
				}
				
				g.setOpacity(1.0f);
				
			};
			
			/*dessin de l'icone*/
			g.setColour(s_COLOUR_UNSELECTION);
			g.fillEllipse(1,5,9,9);
			g.setColour(Colours::white);
			g.drawEllipse(1,5,9,9,1);
			g.drawText(String(m_name.data()), 12, 5, width, 10, Justification::left, false);
			
		}
		
		void itemOpennessChanged (bool isNowOpen) {
			if(isNowOpen) {
				
				vector<std::string>* nodes = new vector<std::string>;
				vector<std::string>* leaves = new vector<std::string>;
				vector<std::string>* attributs = new vector<std::string>;
				vector<std::string>* values = new vector<std::string>;
				
				if(m_NetComponent->getEngines()!=0)
					m_NetComponent->getEngines()->requestNetworkNamespace(m_address, nodes, leaves, attributs, values);
				
				for(unsigned int i = 0 ; i < nodes->size(); ++i) {
					MinuitTreeNode* added = new MinuitTreeNode(m_NetComponent, m_address+std::string("/")+std::string(nodes->at(i)), std::string(nodes->at(i)));
					addSubItem (added);
				}
				for(unsigned int i = 0 ; i < leaves->size(); ++i) {
					MinuitTreeLeave* added = new MinuitTreeLeave(m_NetComponent, m_address+std::string("/")+std::string(leaves->at(i)), std::string(leaves->at(i)));
					addSubItem (added);
				}
				
				m_NetComponent->updateNamespaceSelection();
				
				delete nodes;
				delete leaves;
				delete attributs;
				delete values;
			}//end if isNowOpen
			
			else clearSubItems();
		}
	};

class MinuitTreeDevice : public MinuitTreeItem 
	{
	private:
		NetComponent* m_NetComponent;
		string m_name;
		
	public:
		MinuitTreeDevice(NetComponent* _NetComponent, const string &address, const string &name) {
			m_NetComponent = _NetComponent;
			m_name = name;
			m_address = address;
			m_isChildrenSelected = false;
		};
		~MinuitTreeDevice(){};
		
		bool 	mightContainSubItems (){
			return true;
		}
		
		void itemSelectionChanged (bool isNowSelected){
			((MinuitTreeItem*)getParentItem())->setSemiSelected(isNowSelected);
		}
		
		void paintItem	( Graphics & 	g, int 	width, int 	height) {
			/*surligange sélection*/
			if(isSelected()) {
				g.setColour(s_COLOUR_SELECTION_BLUE);
				g.setOpacity(0.3f);
				g.fillRect(0,0,width,height);
				g.setOpacity(1.0f);
			}
			else if(m_isChildrenSelected) {
				g.setColour(s_COLOUR_SELECTION_BLUE);
				g.setOpacity(0.3f);
				
				//dessin des hachures
				for(int i = 0; i < width; i+=20){
					
					Path hashure;
					hashure.startNewSubPath(i, 0);
					hashure.lineTo(i+13, 0);
					hashure.lineTo(i+3, height);
					hashure.lineTo(i-10, height);
					hashure.closeSubPath();
					g.fillPath(hashure);
				}
				
				g.setOpacity(1.0f);
				
			}
			
			/*dessin de l'icone*/
			g.setColour(Colours::white);
			g.fillEllipse(1,5,9,9);
			g.setColour(Colours::white);
			g.drawEllipse(1,5,9,9,1);
			g.drawText(String(m_name.data()), 12, 5, width, 10, Justification::left, false);
			
		}
		
		void itemOpennessChanged (bool isNowOpen) {
			if(isNowOpen) {
				
				vector<std::string>* nodes = new vector<std::string>;
				vector<std::string>* leaves = new vector<std::string>;
				vector<std::string>* attributs = new vector<std::string>;
				vector<std::string>* values = new vector<std::string>;
				
				if(m_NetComponent->getEngines()!=0) m_NetComponent->getEngines()->requestNetworkNamespace(m_address, nodes, leaves, attributs, values);
				
				for(unsigned int i = 0 ; i < nodes->size(); ++i) {
					MinuitTreeNode* added = new MinuitTreeNode(m_NetComponent, m_address+std::string("/")+std::string(nodes->at(i)), std::string(nodes->at(i)));
					addSubItem (added);
				}
				for(unsigned int i = 0 ; i < leaves->size(); ++i) {
					MinuitTreeLeave* added = new MinuitTreeLeave(m_NetComponent, m_address+std::string("/")+std::string(leaves->at(i)), std::string(leaves->at(i)));
					addSubItem (added);
				}
				
				m_NetComponent->updateNamespaceSelection();
				
				delete nodes;
				delete leaves;
				delete attributs;
				delete values;
				
			}//end if isNowOpen
			else clearSubItems();
		}
		
		void itemDoubleClicked (const MouseEvent &/*e*/){
			m_NetComponent->flipToDeviceEditor();
		}
		
		void itemClicked (const MouseEvent &e){
			
			if(!e.mods.isPopupMenu()) return;
			
			PopupMenu m;
			m.addItem (1, "Edit (inactive)");
			m.addItem (2, "Delete");
			
			const int result = m.show();
			
			if (result == 0)
			{
				// user dismissed the menu without picking anything
			}
			else if (result == 1)
			{
				m_NetComponent->flipToDeviceEditor();
			}
			else if (result == 2)
			{
				if(m_NetComponent->getEngines()!=0) m_NetComponent->getEngines()->networkRemoveDevices(m_name);
			}
		}
	};

class MinuitTreeRoot : public MinuitTreeItem 
	{
	private:
		NetComponent* m_NetComponent;
		
	public:
		MinuitTreeRoot(NetComponent* _NetComponent)
		{
			setOpen(false);
			m_address = "/";
			m_isChildrenSelected = false;
			m_NetComponent = _NetComponent;
		}
		
		~MinuitTreeRoot()
		{
		}
		
		bool mightContainSubItems()
		{
			return true;
		}
		
		void paintItem(Graphics & g, int width, int height) 
		{
			if(isSelected()) {
				g.setColour(s_COLOUR_UNSELECTION);
				g.setOpacity(0.3f);
				g.fillRect(0,0,width,height);
				g.setOpacity(1.0f);
			}
			g.setColour(Colours::white);
			g.drawText(String("Network"), 5, 5, 70, 12, Justification::left, false);
		}
		
		void itemOpennessChanged (bool isNowOpen) 
		{
			if(!isNowOpen) {
				clearSubItems();
				return;
			}
			
			/* RAPHAEL */
			std::vector<std::string>* devices = new std::vector<std::string>();
			std::vector<bool>* isDeviceNamespaceAllowed = new std::vector<bool>();
			m_NetComponent->getEngines()->getNetworkDevicesName(devices, isDeviceNamespaceAllowed);
			/* ------- */
			
			for(unsigned int i = 0 ; i < devices->size(); ++i) {
				/* RAPHAEL */
				MinuitTreeDevice* added = new MinuitTreeDevice(m_NetComponent, /* "/" = déjà comprise dans devices */ std::string(devices->at(i))/*+std::string("/")*/, std::string(devices->at(i)));
				/* ------- */
				
				addSubItem(added);
			}
			m_NetComponent->updateNamespaceSelection();
			
			delete devices;
			delete isDeviceNamespaceAllowed;
		}//end void
	};

//==============================================================================
NetComponent::NetComponent (Engines* _engines)
: m_namespaceTree (0), m_boxTarget (0), m_assignLeft (0), m_assignRight (0), m_refresh(0), m_deviceList(0)
{
	m_buttonBarHeight = 25;
	
	addAndMakeVisible (m_namespaceTree = new TreeView(String::empty));
	//setViewedComponent(m_namespaceTree = new TreeView(String::empty));
	m_namespaceTree->setDefaultOpenness (false);
	m_namespaceTree->setColour (TreeView::backgroundColourId, Colour (0xffffff));
	m_namespaceTree->setColour (TreeView::linesColourId, Colour (0x4cffffff));
	
	addAndMakeVisible (m_assignLeft = new TextButton (String::empty));
	m_assignLeft->setButtonText (T("assign start"));
	m_assignLeft->addButtonListener (this);
	m_assignLeft->setColour (TextButton::textColourId, Colours::white);
	m_assignLeft->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	m_assignLeft->setEnabled(false);
	m_assignLeft->setColour (TextButton::buttonColourId, Colours::darkgrey);
	m_assignLeft->setWantsKeyboardFocus(false);
	m_assignLeft->setEnabled(false);
	
	addAndMakeVisible (m_assignRight = new TextButton (String::empty));
	m_assignRight->setButtonText (T("assign end"));
	m_assignRight->addButtonListener (this);
	m_assignRight->setColour (TextButton::textColourId, Colours::white);
	m_assignRight->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	m_assignRight->setEnabled(false);
	m_assignRight->setColour (TextButton::buttonColourId, Colours::darkgrey);
	m_assignRight->setWantsKeyboardFocus(false);
	m_assignRight->setEnabled(false);
	
	addAndMakeVisible (m_addDeviceButton = new NetButton (String::empty));
	m_addDeviceButton->setButtonText (T("="));/* + */
	m_addDeviceButton->addButtonListener (this);
	m_addDeviceButton->setColour (TextButton::buttonColourId, Colours::white);
	m_addDeviceButton->setColour (TextButton::textColourId, Colours::black);
	m_addDeviceButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	m_addDeviceButton->setWantsKeyboardFocus(false);
	
	addAndMakeVisible (m_refresh = new NetButton (String::empty));
	m_refresh->setButtonText (T("0"));/* reload */
	m_refresh->addButtonListener (this);
	m_refresh->setEnabled(false);//version 0.1
	m_refresh->setColour (TextButton::buttonColourId , Colours::white);
	m_refresh->setColour (TextButton::textColourId, Colours::black);
	m_refresh->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	m_refresh->setWantsKeyboardFocus(false);
	
	m_propertiesArray = new Array<PropertyComponent*>;
	m_propertiesArray->add(new TitleNetPropertyComponent(this));
	
	addAndMakeVisible(m_netPropertyPanel = new PropertyPanel());
	m_netPropertyPanel->addProperties(*m_propertiesArray);
	m_netPropertyPanel->setWantsKeyboardFocus(false);
	
	m_MainWindowForDialog = 0;
	
	/*TreeMapExplorer added code*/
	addAndMakeVisible (m_changeExplorerButton = new TextButton (T("View"), T("Use TreeMap Explorer")));
	m_changeExplorerButton->addButtonListener (this);
	m_changeExplorerButton->setColour (TextButton::buttonColourId, Colours::white);
	m_changeExplorerButton->setColour (TextButton::textColourId, Colours::black);
	m_changeExplorerButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	m_changeExplorerButton->setWantsKeyboardFocus(false);
	
	addChildComponent(m_separateTreeMapWindowButton = new NetButton (T("|"), T("Separate TreeMap Window")));
	m_separateTreeMapWindowButton->addButtonListener (this);
	m_separateTreeMapWindowButton->setColour (TextButton::buttonColourId, Colours::white);
	m_separateTreeMapWindowButton->setColour (TextButton::textColourId, Colours::black);
	m_separateTreeMapWindowButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	m_separateTreeMapWindowButton->setWantsKeyboardFocus(false);
	m_separateTreeMapWindowButton->setVisible(false);
	
	TreeComm* m_treeComm = new TreeComm(_engines);
	
	ScanDeviceThread scan(m_treeComm);
	
	if (scan.runThread())
	{
		// thread finished normally..
	}
	else
	{
		// user pressed the cancel button..
	}
	
	//m_treeComm->init();//now this method is called by ScanDeviceThread class to display a progress bar in same time
	
	m_treeMapExplorerWindow = new TreeMapExplorerWindow(m_engines, m_editedBox);//Separated TreeMap DocumentWindow
	
	m_treeMapExplorerComponent = new Explorer(m_treeComm, m_editedBox, &scan, false);//TreeMap Content Component 
	addChildComponent(m_treeMapExplorerComponent);
	
	m_istreeMapUsed = false;
	m_istreeMapFullScreen = false;
	
	/****************************/
	
	setSize (600, 600);
	
	m_namespaceTree->setRootItem(new MinuitTreeRoot(this));
	m_namespaceTree->setMultiSelectEnabled (true);
	m_namespaceTree->setWantsKeyboardFocus(false);
	
	m_addDeviceComponent = new deviceManager(this);
	
	m_engines = _engines;
}

NetComponent::~NetComponent()
{	
	delete m_treeMapExplorerWindow;
}

vector<Device*> NetComponent::getDeviceList()
{
	return m_deviceList;
}

void NetComponent::paint (Graphics& g)
{
	g.fillAll (s_COLOUR_BACKGROUND);
}

void NetComponent::resized()
{	
	m_changeExplorerButton->setBounds(3, 3, 33 , m_buttonBarHeight);
	m_addDeviceButton->setBounds(39, 3, 33 , m_buttonBarHeight);
	m_refresh->setBounds(75, 3, 33 , m_buttonBarHeight);
	m_netPropertyPanel->setBounds(3*36 + 3, 3, getWidth()*2/3-3*36, m_buttonBarHeight);
	m_assignLeft->setBounds (getWidth()*2/3+6, 3,  getWidth()/3/2-5, m_buttonBarHeight); 
	m_assignLeft->setButtonText( (getWidth()/3/2-5 > 40/*40 = minimum pour affichage correct "assign start"*/) ? String("assign start") : String("<") );
	m_assignRight->setBounds (getWidth()*2/3+6 + getWidth()/3/2, 3, getWidth()/3/2-5, m_buttonBarHeight); 
	m_assignRight->setButtonText( (getWidth()/3/2-5 > 40/*40 = minimum pour affichage correct "assign start"*/) ? String("assign end") : String(">") );
	m_separateTreeMapWindowButton->setBounds(75, 3, 33 , m_buttonBarHeight);
	
	m_namespaceTree->setBounds (0, m_buttonBarHeight+6, proportionOfWidth (1.0000f), getHeight() - m_namespaceTree->getY());
	m_treeMapExplorerComponent->setBounds(3, m_buttonBarHeight+6, INCRUSTED_EXPLORER_WIDTH, INCRUSTED_EXPLORER_HEIGHT);
}

void NetComponent::mouseDrag(const MouseEvent& e)
{
	for(int i=0; i < m_namespaceTree->getNumRowsInTree(); ++i) {
		
		if( m_namespaceTree->getItemOnRow(i)->isSelected()/* (e.getMouseDownX(), e.getMouseDownY())*/)
			startDragging(String("sourceDescription"), this, /*dragImage*/ 0, true);
	}
}

void NetComponent::buttonClicked (Button* buttonThatWasClicked) 
{
	
	if (m_editedBox != 0 && ((buttonThatWasClicked == m_assignLeft) || (buttonThatWasClicked == m_assignRight)))
	{		
		int numSelected = m_namespaceTree->getNumSelectedItems();
		
		//clear selectedItems list
		m_selectedItems.clear();
		
		//fill selectedItems list
		for (int i = 0; i < numSelected; ++i) {
			TreeViewItem* selectedItem = m_namespaceTree->getSelectedItem(i);
			
			if (dynamic_cast<MinuitTreeItem*> (selectedItem)) {
				MinuitTreeItem* minuitSelectedItem = dynamic_cast<MinuitTreeItem*> (selectedItem);
				m_selectedItems.push_back(minuitSelectedItem->getAddress());
			}
		}
		
		//get the snapshot of nodes
		vector<string> snapshot;
		for (uint i = 0; i < m_selectedItems.size(); i++) {
			vector<string> reponse; 
			if (getEngines() != 0) {
				reponse = getEngines()->networkSnapShot(m_selectedItems[i]);
			}
			snapshot.insert(snapshot.end(), reponse.begin(), reponse.end());	
		}
		
		//assign
		if (buttonThatWasClicked == m_assignLeft) {
			if (snapshot.size() > 0) {
				if (getEngines() != 0) {
					getEngines()->setCtrlPointMessagesToSend(m_editedBox->getId(), 1, snapshot);
				}
				m_editedBox->updateMessagesEditor(1, snapshot);
				m_editedBox->initInterpolationWindow();
			}
		}
		if (buttonThatWasClicked == m_assignRight) {
			if (snapshot.size() > 0) {
				if (getEngines() != 0) {
					getEngines()->setCtrlPointMessagesToSend(m_editedBox->getId(), 2, snapshot);
				}
				m_editedBox->updateMessagesEditor(2, snapshot);
				m_editedBox->initInterpolationWindow();
			}
		}
		
		m_editedBox->sendSynchronousChangeMessage(m_editedBox);
	}
	
	if(buttonThatWasClicked == m_addDeviceButton) {
		flipToDeviceEditor();
	}
	
	/*TreeMapExplorer added code*/
	if (buttonThatWasClicked == m_changeExplorerButton) {
		
		m_istreeMapUsed = !m_istreeMapUsed;
		
		m_namespaceTree->setVisible(!m_istreeMapUsed);
		m_assignLeft->setVisible(!m_istreeMapUsed);
		m_assignRight->setVisible(!m_istreeMapUsed);
		m_refresh->setVisible(!m_istreeMapUsed);
		
		m_treeMapExplorerComponent->setVisible(m_istreeMapUsed);
		m_separateTreeMapWindowButton->setVisible(m_istreeMapUsed);
		
	}
	
	if (buttonThatWasClicked == m_separateTreeMapWindowButton) {
		m_istreeMapFullScreen = !m_istreeMapFullScreen;
		
		if (m_istreeMapFullScreen) {
			m_treeMapExplorerWindow->setContentComponent(m_treeMapExplorerComponent, false, true);
			m_treeMapExplorerWindow->setVisible(true);
			m_separateTreeMapWindowButton->setButtonText("\\");
		} else {
			m_treeMapExplorerWindow->setContentComponent(0, false);
			addAndMakeVisible(m_treeMapExplorerComponent);
			m_treeMapExplorerWindow->setVisible(false);
			m_separateTreeMapWindowButton->setButtonText("|");
		}		
	}
	/****************************/
}

bool NetComponent::keyPressed(const KeyPress& key)
{
	if(key == KeyPress(KeyPress::tabKey))
	{
		flipToDeviceEditor();
		return true;
	}
	return false;
}

void NetComponent::changeListenerCallback(void* objectThatHasChanged)
{
	if ((TimeBox*)objectThatHasChanged == m_editedBox) {
		m_netPropertyPanel->refreshAll();
		
		//the tree item pre-selection action take a long time when the tree contains a lot of items
		updateNamespaceSelection();
	}
}

PropertyPanel* NetComponent::getPanel() 
{
	return m_netPropertyPanel;
}

TreeView* NetComponent::getTreeView() 
{
	return m_namespaceTree;
}

Engines* NetComponent::getEngines() 
{
	return m_engines;
}

void NetComponent::setMainWindow(MainWindow* _mainWindow) 
{
	m_MainWindowForDialog = _mainWindow;
}

void NetComponent::setEditedBox(TimeBox* _editedBox)
{
	m_editedBox = _editedBox;
	m_assignLeft->setEnabled(_editedBox != 0);
	m_assignRight->setEnabled(_editedBox != 0);
	changeListenerCallback(m_editedBox);
	
	/*TreeMapExplorer added code*/
	if (_editedBox != 0) {
		m_treeMapExplorerComponent->setEditedTimeBox(_editedBox);
	}
	/****************************/
}

TimeBox* NetComponent::getEditedBox()
{
	return m_editedBox;
}

void NetComponent::updateNamespaceSelection() 
{
	m_namespaceTree->clearSelectedItems();
	
	if(getEditedBox() == 0 ) return;
	
	if (getEditedBox()->getOscMessageOutStartVector()->size() > 0 || getEditedBox()->getOscMessageOutStopVector()->size() > 0) {
		
		/*si une boite est sélectionnée, on highlight ses devices/noeuds/feuilles assignées*/
		for (int i = 1/*enleve ma racine*/ ; i < m_namespaceTree->getNumRowsInTree() ; i++) {
			
			((MinuitTreeItem*)(m_namespaceTree->getItemOnRow(i)))->setSemiSelected(false);
			((MinuitTreeItem*)(m_namespaceTree->getItemOnRow(i)))->treeHasChanged();
			
			/*message au moins*/
			if(getEditedBox() -> containsString(String(  (  (MinuitTreeItem*)( m_namespaceTree->getItemOnRow(i)) )->getAddress().data()) , 1/* left */) ) {
				((MinuitTreeItem*)(m_namespaceTree->getItemOnRow(i)))->setSemiSelected(true);
				
			}
			else if(getEditedBox() -> containsString(String(  ( (MinuitTreeItem*)( m_namespaceTree->getItemOnRow(i)) )->getAddress().data()) , 2/* right */) ) {
				((MinuitTreeItem*)(m_namespaceTree->getItemOnRow(i)))->setSemiSelected(true);
			}
			
			/*message exacte*/
			if(getEditedBox() -> containsMessage(String(  ( (MinuitTreeItem*)( m_namespaceTree->getItemOnRow(i)) )->getAddress().data()), 1) ) {
				m_namespaceTree->getItemOnRow(i)->setSelected(true, false);
			}
			else if(getEditedBox() -> containsMessage(String(  ( (MinuitTreeItem*)( m_namespaceTree->getItemOnRow(i)) )->getAddress().data()), 2) ) {
				m_namespaceTree->getItemOnRow(i)->setSelected(true, false);
			}
		}
	}
}

void NetComponent::putNewStateToCapture(string _new) 
{
	m_selectedItems.push_back(_new);
}

void NetComponent::setStateSnapShot(vector<std::string> _sss) 
{
	m_selectedItems = _sss ;
}

void NetComponent::flipToDeviceEditor()
{
	((PropertyFrame*)getParentComponent())->setContentComponent(m_addDeviceComponent);
}

void NetComponent::addDevice(string _name, string _protocole, string _ip, string _port)
{
	if(m_engines!=0) m_engines->networkAddDevices(_name, _protocole, _ip, _port);
	
	//MAJ device list
	m_deviceList.push_back(new Device((String)_name.data(), (String)_protocole.data(), (String)_ip.data(), (String)_port.data()));
	
	/*TreeMapExplorer added code*/
	m_treeMapExplorerComponent->networkTreeUpdate();
	m_treeMapExplorerComponent->update();
	//	if (m_treeMapExplorerWindow != NULL) {
	//		m_treeMapExplorerWindow->networkTreeUpdate();
	//		m_treeMapExplorerWindow->update();
	//	}
	/****************************/
}

void NetComponent::modifyDevice(string /*oldName*/, string /*name*/, string /*protocole*/, string /*ip*/, string /*port*/)
{
	//if(m_panel->getConduite()!=0) m_panel->getConduite()->getEngines()->networkAddDevices(_name, _protocole, _ip, _port);
	
	/*TreeMapExplorer added code*/
	m_treeMapExplorerComponent->networkTreeUpdate();
	m_treeMapExplorerComponent->update();
	//	if (m_treeMapExplorerWindow != NULL) {
	//		m_treeMapExplorerWindow->networkTreeUpdate();
	//		m_treeMapExplorerWindow->update();
	//	}
	/****************************/
}

//==============================================================================
#if 0
/*  -- Jucer information section --
 
 This is where the Jucer puts all of its metadata, so don't change anything in here!
 
 BEGIN_JUCER_METADATA
 
 <JUCER_COMPONENT documentType="Component" className="NetComponent" componentName=""
 parentClasses="public Component" constructorParams="" variableInitialisers=""
 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
 fixedSize="0" initialWidth="600" initialHeight="600">
 <METHODS>
 <METHOD name="childrenChanged()"/>
 </METHODS>
 <BACKGROUND backgroundColour="ff292929"/>
 <IMAGEBUTTON name="new button" id="733462111f0d4bde" memberName="imageButton"
 virtualName="" explicitFocusOrder="0" pos="0 0 100% 100%" buttonText="new button"
 connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="0"
 resourceNormal="midnightClockSml2_jpg" opacityNormal="0.20408164"
 colourNormal="ffffff" resourceOver="" opacityOver="0.20408164"
 colourOver="0" resourceDown="" opacityDown="0.20408164" colourDown="0"/>
 <TREEVIEW name="" id="4869613b7f2f4c61" memberName="m_namespaceTree" virtualName=""
 explicitFocusOrder="0" pos="0 88 100% 96M" backgroundColour="ffffff"
 linecol="4cffffff" rootVisible="1" openByDefault="1"/>
 <LABEL name="" id="5ce49dcd89dbc274" memberName="ip" virtualName=""
 explicitFocusOrder="0" pos="5 32 71 24" bkgCol="3c3c3c" textCol="ffffffff"
 outlineCol="0" edTextCol="ff000000" edBkgCol="0" labelText="Ip Adresse"
 editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
 fontname="Default font" fontsize="15" bold="0" italic="0" justification="34"/>
 <LABEL name="" id="ba767e7c4c993907" memberName="ip_value" virtualName=""
 explicitFocusOrder="0" pos="88 32 80 24" textCol="ffffffff" edTextCol="ff000000"
 edBkgCol="0" labelText="0" editableSingleClick="1" editableDoubleClick="1"
 focusDiscardsChanges="0" fontname="Default font" fontsize="15"
 bold="0" italic="0" justification="33"/>
 <LABEL name="" id="dd6b66f431a2845d" memberName="port" virtualName=""
 explicitFocusOrder="0" pos="5 57 78 24" textCol="ffffffff" edTextCol="ff000000"
 edBkgCol="0" labelText="Target port" editableSingleClick="0"
 editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
 fontsize="15" bold="0" italic="0" justification="34"/>
 <LABEL name="" id="bd353d0344784632" memberName="port_value" virtualName=""
 explicitFocusOrder="0" pos="88 57 80 24" textCol="ffffffff" edTextCol="ff000000"
 edBkgCol="0" labelText="0" editableSingleClick="1" editableDoubleClick="1"
 focusDiscardsChanges="0" fontname="Default font" fontsize="15"
 bold="0" italic="0" justification="33"/>
 <LABEL name="" id="2d661511572fff38" memberName="minuitTitle" virtualName=""
 explicitFocusOrder="0" pos="3 8 96.357% 24" textCol="ffc76dff"
 edTextCol="ff000000" edBkgCol="0" labelText="minuit namespace"
 editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
 fontname="Default font" fontsize="24.2" bold="1" italic="0" justification="33"/>
 <LABEL name="" id="24155a596cd68bd3" memberName="m_boxTarget" virtualName=""
 explicitFocusOrder="0" pos="192 32 71 24" bkgCol="3c3c3c" textCol="ffffffff"
 outlineCol="0" edTextCol="ff000000" edBkgCol="0" labelText="(no box selected)"
 editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
 fontname="Default font" fontsize="15" bold="0" italic="0" justification="34"/>
 <TOGGLEBUTTON name="" id="ef76c0eb1bea0398" memberName="m_m_assignLeft" virtualName=""
 explicitFocusOrder="0" pos="264 32 40 24" txtcol="ffffffff" buttonText="L"
 connectedEdges="0" needsCallback="1" radioGroupId="0" state="1"/>
 <TEXTBUTTON name="" id="4d3728768d4f68ba" memberName="assignButton" virtualName=""
 explicitFocusOrder="0" pos="200 60 144 24" buttonText="assign"
 connectedEdges="0" needsCallback="1" radioGroupId="0"/>
 <TOGGLEBUTTON name="" id="c836ac846b1780f8" memberName="m_assignRight" virtualName=""
 explicitFocusOrder="0" pos="304 32 40 24" txtcol="ffffffff" buttonText="R"
 connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
 </JUCER_COMPONENT>
 
 END_JUCER_METADATA
 */
#endif
