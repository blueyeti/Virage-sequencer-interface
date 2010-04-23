/*
 *  Commons.cpp
 *  JuceCommons
 *
 *  Created by Laurent Garnier on 27/11/09.
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

#include "Commons.h"
#include "Explorer.h"
#include "TimeBox.h"

#include <string>
#include <sstream>
#include <fstream>

//////////////////////////////////////////////////////////////////
//A under class to catch slider actions 
//////////////////////////////////////////////////////////////////
class SliderItem : public SliderListener
	{
	public:
		SliderItem(Commons *commons, const std::string& absoluteAddress, Slider *slider) : 
		m_commons(commons), m_absoluteAddress(absoluteAddress), m_slider(slider)
		{
			m_slider->addListener(this);
		};
		
		~SliderItem()
		{
			deleteAndZero (m_slider);
		};
		
		void sliderValueChanged(Slider *slider)
		{
			std::ostringstream value;
			//			if (m_commons->getMessageType(m_absoluteAddress) == "integer" || m_commons->getMessageType(m_absoluteAddress) == "i") {
			//				if (value << (int)m_slider->getValue()) {
			//					m_commons->getExplorer()->deviceSendMessage(m_absoluteAddress + " " + value.str());
			//				}
			//			} else {
			if (value << m_slider->getValue()) {
				m_commons->getExplorer()->deviceSendMessage(m_absoluteAddress + " " + value.str());
			}
			//			}
		};
		
		void sliderDragStarted(Slider *slider){};
		
		void sliderDragEnded(Slider *slider)
		{
			std::ostringstream value;
			//			if (m_commons->getMessageType(m_absoluteAddress) == "integer" || m_commons->getMessageType(m_absoluteAddress) == "i") {
			//				if (value << (int)m_slider->getValue()) {
			//					m_commons->setMessageValue(m_absoluteAddress, value.str());	
			//				}
			//			} else {
			if (value << m_slider->getValue()) {
				m_commons->setMessageValue(m_absoluteAddress, value.str());
			}
			//			}
		};
		
	private:
		Commons *m_commons;
		std::string m_absoluteAddress;
		Slider *m_slider;
	};
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//A under class to catch slider actions 
//////////////////////////////////////////////////////////////////
class ToggleItem : public Component, public ButtonListener
{
public:
	ToggleItem(Commons *commons, const std::string& absoluteAddress, bool isActive) :
	m_commons(commons), m_absoluteAddress(absoluteAddress), m_state(isActive)
	{
		m_toggle = new ToggleButton(T(""));
		
		if (!isActive) {
			m_toggle->setButtonText(T(""));
		}
		addAndMakeVisible(m_toggle);
		m_toggle->addButtonListener(this);
		m_toggle->setToggleState(isActive, false);
	};
	
	~ToggleItem()
	{
		deleteAndZero (m_toggle);
	};
	
	void paint(Graphics& g) {};
	
	void resized()  
	{
		m_toggle->setBounds(15, 0, 100, 18);	
	}
	
	void buttonClicked (Button* buttonThatWasClicked)
	{
		std::ostringstream value;
		if (m_state) {
			m_toggle->setToggleState(false, false);
			m_state = false;
		} else {
			m_toggle->setToggleState(true, false);
			m_state = true;
		}
		
		if (value << m_state) {
			m_commons->setMessageValue(m_absoluteAddress, value.str());
			m_commons->getExplorer()->deviceSendMessage(m_absoluteAddress + " " + value.str());
		}
	}
	
private:
	Commons *m_commons;
	std::string m_absoluteAddress;
	ToggleButton *m_toggle;
	bool m_state;
};
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//A under class to display message instance buttons
//////////////////////////////////////////////////////////////////
class InstanceItem : public Component, public ButtonListener
{
public:
	InstanceItem(Commons *commons, const std::string& absoluteAddress, uint instanceNb) :
	m_commons(commons), m_absoluteAddress(absoluteAddress), m_instanceNb(instanceNb)
	{
		//		if (m_instanceNb != 0) {
		//			if (fromZero) {
		//				m_instanceNb++;
		for (uint i = 0; i <= m_instanceNb; i++) {
			TextButton *t = new TextButton(String(i));
			t->addButtonListener(this);
			t->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
			t->setClickingTogglesState(true);
			t->setState(Button::buttonDown);
			addAndMakeVisible(t);
			m_instanceButtons.push_back(t);
		}	
		//			} else {
		//				for (uint i = 1; i <= m_instanceNb; i++) {
		//					TextButton *t = new TextButton(String(i));
		//					addAndMakeVisible(t);
		//					m_instanceButtons.push_back(t);
		//				}	
		//			}
		//		}
		
	};
	
	~InstanceItem()
	{
		for (uint i = 0; i < m_instanceButtons.size(); i++) {
			deleteAndZero(m_instanceButtons.at(i));
		}
		m_instanceButtons.clear();
		//delete m_instanceButtons;
	};
	
	void paint(Graphics& g) {};
	
	void resized()  
	{
		for (uint i = 0; i <= m_instanceNb; i++) {
			m_instanceButtons.at(i)->setBounds(i * 15, 0, 15, 15); 	
		}	
	}
	
	void buttonClicked (Button* buttonThatWasClicked)
	{
		for (uint i = 0; i < m_instanceButtons.size(); i++) {
			if (buttonThatWasClicked == m_instanceButtons.at(i)) {
				string instance = (string)m_instanceButtons.at(i)->getButtonText();
				if (instance.compare("0") == 0) {
					m_commons->setSelectedToAssign(m_absoluteAddress, m_instanceButtons.at(i)->getToggleState(), false);
				} else {
					m_commons->setSelectedToAssign(m_absoluteAddress + "." + instance, m_instanceButtons.at(i)->getToggleState(), false);
				}
			}
		}
	}
	
private:
	Commons *m_commons;
	std::string m_absoluteAddress;
	uint m_instanceNb;
	std::vector<TextButton *> m_instanceButtons;
	
};
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//Commons class
//////////////////////////////////////////////////////////////////
Commons::Commons(Explorer *explorer) : 
FileBasedDocument(String(".xml"), String("*.xml"), String("Load Preset..."), String("Save Preset as...")), m_explorer(explorer)
{
	m_presets = new XmlElement(T("NAMESPACE_PRESETS"));
}

Commons::~Commons()
{
	delete m_presets;
}


Explorer*
Commons::getExplorer()
{
	return m_explorer;	
}

void 
Commons::addStateMapMessage(const std::string& absoluteAddress, bool isSelected)
{
	m_selectionState[absoluteAddress] = isSelected;
}

void 
Commons::setSelectedToAssign(const std::string& absoluteAddress, bool isSelected, bool TreeMapToRefresh)
{
	m_selectionState[absoluteAddress] = isSelected;
	if (TreeMapToRefresh) {
		m_explorer->selectTreeMapItem(absoluteAddress, isSelected);
	}
}

void 
Commons::refresh(bool TreeMapToRefresh)
{	
	m_explorer->refresh(TreeMapToRefresh);
}

bool 
Commons::isSelectedToAssign(const std::string& absoluteAddress)
{
	hash_map<std::string, bool>::iterator it = m_selectionState.find(absoluteAddress);
	if (it != m_selectionState.end()) {
		return it->second;
	} else 
		return false;
}

void 
Commons::addValueMapMessage(const std::string& absoluteAddress, const std::string& value)
{
	m_messageValue[absoluteAddress] = value;
}

void 
Commons::setMessageValue(const std::string& absoluteAddress, const std::string& value)
{
	m_messageValue[absoluteAddress] = value;
}

std::string 
Commons::getMessageValue(const std::string& absoluteAddress)
{
	return m_messageValue[absoluteAddress];
}

void
Commons::setMessageType(const std::string& absoluteAddress, const std::string& type)
{
	m_messageType[absoluteAddress] = type;	
}

std::string 
Commons::getMessageType(const std::string& absoluteAddress)
{
	return m_messageType[absoluteAddress];	
}

void
Commons::setMessageRange(const std::string& absoluteAddress, const std::string& range)
{
	m_messageRange[absoluteAddress] = range;
	
	//setMessagePopup(absoluteAddress);
}

void 
Commons::setMessagePopup(const std::string&  absoluteAddress)
{
	uint instanceNb = 0;
	hash_map<std::string, uint>::iterator it = m_instances.find(absoluteAddress);
	if (it != m_instances.end()) {
		instanceNb = it->second;	
	}
	
	std::string value = m_messageValue[absoluteAddress];
	std::string type = m_messageType[absoluteAddress];
	std::string range = m_messageRange[absoluteAddress];
	
	PopupMenu *pop = new PopupMenu();
	pop->addItem(1, absoluteAddress.data(), false);
	pop->addItem(2, String("Value : ") + value.data(), false);
	pop->addItem(3, String("Type : ") + type.data(), false);
	pop->addItem(4, String("Range : ") + range.data(), false);
	
	if (type == "integer" || type == "i" || type == "decimal" || type == "f") {
		Slider *slider = new Slider(T("slider"));
		SliderItem *sliderItem = new SliderItem(this, absoluteAddress, slider);
		size_t start = range.find_first_not_of(" ");
		if (start != string::npos) {
			string tmp = range.substr(start, range.size());
			size_t pos = tmp.find_first_of(" ");
			if (pos != string::npos) {
				string minValue = range.substr(start, pos);
				string maxValue = range.substr(pos+1, range.size());
				if (type.compare("integer") == 0) {
					int min, max;
					valueOf(minValue, min);
					valueOf(maxValue, max);
					slider->setRange(min, max, 1);
				} else {
					float min, max;	
					valueOf(minValue, min);
					valueOf(maxValue, max);
					slider->setRange(min, max, 0.0001);
				}
			}
		}
		float valueFloat;
		valueOf(value, valueFloat);
		slider->setValue(valueFloat);
		
		pop->addCustomItem(5, slider, 40, 13, false);
		
	}
	
	if (type == "boolean" || type == "t") {
		bool valueBool;
		valueOf(value, valueBool);
		ToggleItem *tog = new ToggleItem(this, absoluteAddress, valueBool);
		pop->addCustomItem(5, tog, 40, 18, false);
	}
	
	pop->addSeparator();
	pop->addItem(6, String("Reinit. Value"), true);
	
	if (instanceNb != 0) {
		pop->addSeparator();
		InstanceItem *instanceItem = new InstanceItem(this, absoluteAddress, instanceNb);
		pop->addCustomItem(7, instanceItem, 40, 13, false);
	}
	
	pop->addSeparator();
	pop->addItem(8, String("Hide selected"), true);
	pop->addItem(9, String("Hide similars"), true);
	pop->addItem(10, String("Display only"), true);
	pop->addItem(11, String("Reinit. display"),true);
	
	pop->addSeparator();
	pop->addItem(12, String("Store preset"),true);
	pop->addItem(13, String("Store preset as..."),true);
	
	PopupMenu presetMenu;
	presetMenu.addItem(101, String("Load presets from xml..."), true);
	presetMenu.addItem(100, String("Save presets to xml..."), true);
	pop->addSubMenu(String("Presets"), presetMenu);
	
	pop->addSeparator();
	for (uint i = 102; i < 102 + m_loadedPresetsNames.size(); i++) {
		pop->addItem(i, m_loadedPresetsNames.at(i-102), true/*m_loadedPresetsNames.at(i-102) != m_currentPresetName*/);
	}
	
	m_messagePopup[absoluteAddress] = pop;
}

void 
Commons::showPopupmenu(const std::string& absoluteAddress)
{
	bool isItemSelected = isSelectedToAssign(absoluteAddress);
	
	setMessagePopup(absoluteAddress);
	int result = m_messagePopup[absoluteAddress]->show(0, 0, 0, 15);
	
	if (result == 6) { m_explorer->reinitValues(absoluteAddress); }
	if (result == 8) { hideSelectedItems(isItemSelected, absoluteAddress); }
	if (result == 9) { hideSimilarItems(isItemSelected, absoluteAddress); }
	if (result == 10) { displayOnlySelectedItems(isItemSelected, absoluteAddress); }
	if (result == 11) { reinitDisplay(); }
	if (result == 12) { storePreset((std::string)m_currentPresetName); }
	if (result == 13) { storePresetAs(); }
	
	if (result == 100) { savePresetsToXml(); }
	if (result == 101) { loadPresetsFromXml(); }
	
	if (result >= 102) { activePreset(result-102); }
	
}	

std::map<std::string, std::string>
Commons::getSelectedLeaves()
{
	std::map<std::string, std::string> selectedLeavesValues;
	
	hash_map<std::string, bool>::iterator it = m_selectionState.begin();
	
	//the message (leave or node) is considered by assigned only if a value is associated
	while (it != m_selectionState.end()) {
		if (/*m_explorer->getTreeElementLevel(it->first).compare("LEAF") == 0 &&*/ it->second == true && m_hiddenItems.find(it->first) == m_hiddenItems.end() && m_messageValue[it->first] != "") {
			selectedLeavesValues[it->first] = m_messageValue[it->first];
			
		}
		it++;	
	}
	return selectedLeavesValues;
}

std::vector<std::string> 
Commons::getSelectedLeavesValues (std::vector<std::string> leaves)
{
	std::vector<std::string> selectedLeavesValues;
	
	for (uint i = 0; i < leaves.size(); i++) {
		selectedLeavesValues.push_back(m_messageValue[leaves.at(i)]);
	}
	
	return selectedLeavesValues;
}

void
Commons::hideSelectedItems(bool isItemSelected, const std::string& absoluteAddress)
{
	if (isItemSelected) {
		hash_map<std::string, bool>::iterator it = m_selectionState.begin();
		while (it != m_selectionState.end()) {
			if (it->second) {
				m_hiddenItems[it->first] = true;
				it->second = false;
			}
			it++;
		}
	} else {
		m_hiddenItems[absoluteAddress] = true;
	}
	
	m_explorer->update();
}

void
Commons::hideSimilarItems(bool isItemSelected, const std::string& absoluteAddress)
{
	
	std::vector<std::string> itemToHide;
	std::vector<std::string> similarItemToHide;
	
	if (isItemSelected) {
		hash_map<std::string, bool>::iterator it = m_selectionState.begin();
		hash_map<std::string, bool>::iterator it1 = m_selectionState.begin();
		
		while (it != m_selectionState.end()) {
			if (it->second) {
				itemToHide.push_back(it->first);
				it->second = false;
				
				size_t indexOfLastSlash = it->first.find_last_of("/");
				std::string relAddress = it->first.substr(indexOfLastSlash, it->first.size());
				
				it1 = m_selectionState.begin();
				
				while (it1 != m_selectionState.end()) {
					if (it1->first.find(relAddress) != string::npos) {
						itemToHide.push_back(it1->first);
					}
					it1++;
				}
				
			}
			it++;
		}
		
	} else {
		size_t indexOfLastSlash = absoluteAddress.find_last_of("/");
		std::string relAddress = absoluteAddress.substr(indexOfLastSlash, absoluteAddress.size());
		hash_map<std::string, bool>::iterator it = m_selectionState.begin();
		while (it != m_selectionState.end()) {
			if (it->first.find(relAddress) != string::npos) {
				itemToHide.push_back(it->first);
			}
			it++;
		}
	}
	
	for (uint i = 0; i < itemToHide.size(); i++) {
		m_hiddenItems[itemToHide.at(i)] = true;
	}
	
	m_explorer->update();
}

void
Commons::displayOnlySelectedItems(bool isItemSelected, const std::string& absoluteAddress)
{
	if (!isItemSelected) {
		//first deselect all
		hash_map<std::string,  bool>::iterator it1 = m_selectionState.begin();

		it1 = m_selectionState.begin();
		while (it1 != m_selectionState.end()) {
			it1->second = false;
			it1++;
		}	
		
		//then select only targeted items
		std::vector<std::string> subTree;
		m_explorer->getSubTreeAddresses(absoluteAddress, &subTree);
		std::vector<std::string>::iterator iSubTree = subTree.begin();
		setSelectedToAssign(absoluteAddress, true, false);
		while (iSubTree != subTree.end()) {
			setSelectedToAssign(*iSubTree, true, false);	
			iSubTree++;
		}
	}
	
	std::vector<std::string> temporaryHiddenItems;
	std::vector<std::string> temporaryVisibleItems;
	
	//first pass : leaves to hide
	hash_map<std::string,  bool>::iterator it = m_selectionState.begin();
	while (it != m_selectionState.end()) {
		if (!it->second && m_explorer->getTreeElementLevel(it->first) == "LEAF") {
			m_hiddenItems[it->first] = true;
			temporaryHiddenItems.push_back(it->first);
		} else if (it->second && m_explorer->getTreeElementLevel(it->first) == "LEAF") {
			temporaryVisibleItems.push_back(it->first);
		}
		it++;
	}
	
	//second pass : nodes which not contain leaves
	it = m_selectionState.begin();
	while (it != m_selectionState.end()) {
		if (!it->second && m_explorer->getTreeElementLevel(it->first) != "LEAF") {
			bool found = false;
			std::string node = it->first;
			
			for (uint i = 0; i < temporaryVisibleItems.size(); i++) {
				if (temporaryVisibleItems.at(i).find(node) == 0) {
					found = true;
				}
			}
			if (!found) {
				m_hiddenItems[node] = true;
				temporaryHiddenItems.push_back(node);
			} else {
				temporaryVisibleItems.push_back(node);
			}
		}
		it++;
	}
	
	//third pass : deselect displayed items
	it = m_selectionState.begin();
	while (it != m_selectionState.end()) {
		it->second = false;
		it++;
	}
	
	m_explorer->update();
}

void 
Commons::displayOnlyDoubleClickedItem(const std::string& absoluteAddress)
{
	m_hiddenItems.clear();
	displayOnlySelectedItems(false, absoluteAddress);
	m_explorer->update();
}

void 
Commons::reinitDisplay()
{
	m_hiddenItems.clear();
	m_explorer->update();
}

void 
Commons::setPreSelectedItems(TimeBox *edited)
{
	m_preSelectedItems.clear();
	
	hash_map<std::string, bool>::iterator it0 = m_selectionState.begin();
	while (it0 != m_selectionState.end()) {
		if (edited->containsMessage(it0->first.data(), 1) || edited->containsMessage(it0->first.data(), 2)) {
			m_preSelectedItems[it0->first] = true;	
		}
		
		it0++;
	}	
	
	m_explorer->update();
	
	hash_map<std::string, bool>::iterator it = m_preSelectedItems.begin();
	while (it != m_preSelectedItems.end()) {
		setSelectedToAssign(it->first, true, true);
		it++;
	}	
	
	refresh(true);
}

void
Commons::storePreset(const std::string& presetName)
{	
	if (m_presets->getNumChildElements() == 0 || m_currentPresetName.length() == 0) {
		storePresetAs();	
	} 
	else 
	{
		forEachXmlChildElement (*m_presets, f)
		{
			if (f->hasTagName (T("PRESET")) && f->getStringAttribute (T("name")) == (String)presetName.data())
			{
				XmlElement *newItem = new XmlElement(T("PRESET"));
				newItem->setAttribute(T("name"), f->getStringAttribute (T("name")));
				newItem->setAttribute(T("number"), f->getStringAttribute (T("number")));
				
				hash_map<std::string,  bool>::iterator it = m_selectionState.begin();
				while (it != m_selectionState.end()) 
				{
					if (m_hiddenItems.find(it->first) == m_hiddenItems.end()) 
					{
						XmlElement *item = new XmlElement(T("ITEM"));
						item->setAttribute (T("address"), (String)it->first.data());
						item->setAttribute(T("selected"), it->second);
						newItem->addChildElement (item);
					}
					it++;
				}
				
				m_presets->replaceChildElement(f, newItem);
			}
		}
	}
}

void
Commons::storePresetAs()
{
	AlertWindow w (T("Save Preset As.."),
				   T("Enter a new name or select an existing preset to overwrite"),
				   AlertWindow::NoIcon);
	
	w.addTextEditor (T("text"), T("unnamed"));
	
	StringArray existPresets;
	existPresets.add (T(" "));
	
	for (uint i = 0; i < m_loadedPresetsNames.size(); i++) {
		existPresets.add (m_loadedPresetsNames.at(i));
	}
	
	w.addComboBox (T("existPresets"), existPresets, T("Existing presets"));
	
	w.addButton (T("ok"), 1, KeyPress (KeyPress::returnKey, 0, 0));
	w.addButton (T("cancel"), 0, KeyPress (KeyPress::escapeKey, 0, 0));
	
	if (w.runModalLoop() != 0) // is they picked 'ok'
	{
		// this is the item they chose in the drop-down list..
		const int presetIndexChosen = w.getComboBoxComponent (T("existPresets"))->getSelectedItemIndex();
		
		if (presetIndexChosen != 0) 
		{
			String presetNameChosen = m_loadedPresetsNames.at(presetIndexChosen - 1);
			storePreset((std::string)presetNameChosen);
		} 
		else 
		{
			// this is the text they entered..
			String text = w.getTextEditorContents (T("text"));
			
			//store the preset as text
			XmlElement *preset = new XmlElement(T("PRESET"));
			preset->setAttribute (T("number"), m_presets->getNumChildElements());
			preset->setAttribute (T("name"), text);
			
			hash_map<std::string,  bool>::iterator it = m_selectionState.begin();
			while (it != m_selectionState.end()) 
			{
				if (m_hiddenItems.find(it->first) == m_hiddenItems.end()) 
				{
					XmlElement *item = new XmlElement(T("ITEM"));
					item->setAttribute (T("address"), (String)it->first.data());
					item->setAttribute(T("selected"), it->second);
					
					preset->addChildElement (item);
				}
				it++;
			}
			
			m_presets->addChildElement(preset);
			m_loadedPresetsNames.push_back(text);
		}
	}
}

void
Commons::activePreset(uint index)
{
	reinitDisplay();
	
	m_currentPresetName = m_loadedPresetsNames.at(index);
	std::map<std::string, bool> tmp;
	
	forEachXmlChildElement (*m_presets, e)
	{		
		if (e->hasTagName (T("PRESET")) && e->getStringAttribute (T("name")) == m_currentPresetName)
		{
			forEachXmlChildElement (*e, g)
			{
				if (g->hasTagName (T("ITEM"))) 
				{
					String address = g->getStringAttribute(T("address"));
					tmp[(std::string)address] = g->getBoolAttribute(T("selected"));
				}
			}
			
			hash_map<std::string,  bool>::iterator it = m_selectionState.begin();
			while (it != m_selectionState.end()) {
				if (tmp.find(it->first) == tmp.end()) {
					m_hiddenItems[it->first] = true;
				}
				if (tmp[it->first]) {
					m_preSelectedItems[it->first] = true;
				}
				it++;
			}
			m_explorer->update();
		} 
	}
	
	hash_map<std::string,  bool>::iterator it = m_preSelectedItems.begin();
	while (it != m_preSelectedItems.end()) {
		setSelectedToAssign(it->first, true, true);
		it++;
	}
	
	refresh(true);
}

void
Commons::savePresetsToXml()
{
	//	save(true, true);
	File newFile;
	saveAs(newFile, true, true, true);
}

void
Commons::loadPresetsFromXml()
{
	reinitDisplay();
	m_loadedPresetsNames.clear();
	loadFromUserSpecifiedFile(true);
}

/*************************** FileBasedDocument overwritted-must methods *******************/
const String 
Commons::loadDocument(const File & file) 
{
	File _xml = file;
	
	/********* XML PARSER *********/
	File* openedFile = new File(_xml);
	
	if (openedFile->getFileExtension() != String(".xml")) {
		return String();
	}
	
	//first remove the local XmlElement childs
	//m_presets->deleteAllChildElements();
	
	//Then get the loaded XmlElement
	XmlDocument openedDocument(*openedFile);
    XmlElement* projectElement = openedDocument.getDocumentElement();
	
	if (projectElement == 0) {
		
        String error = openedDocument.getLastParseError();
		return error;
		
	} else {
		// check we're looking at the right kind of document..
		if(projectElement->hasTagName(T("VIRAGE")))
		{
			forEachXmlChildElement (*projectElement, e)
			{
				if (e->hasTagName (T("NAMESPACE_PRESETS")))
				{
					XmlElement* preset = projectElement->getChildByName(T("NAMESPACE_PRESETS"));
					
					m_presets = new XmlElement(*preset);
					
					forEachXmlChildElement (*preset, f)
					{
						if (f->hasTagName (T("PRESET")))
						{
							//m_presets->addChildElement(f);
							
							String presetName  = f->getStringAttribute (T("name"));
							m_loadedPresetsNames.push_back(presetName);
							
						}
					}
				}
			}
		}
    }
	
	//	File* savedFile = new File("test.xml");
	//	const String savedDocument = m_presets->createDocument (String::empty);
	//	
	//	std::ofstream myXmlDocument(savedFile->getFullPathName().toUTF8(), ios::trunc);
	//	myXmlDocument <<  (std::string)savedDocument;
	//	myXmlDocument.close();
	
	//	for (uint i = 0; i < m_loadedPresetsNames.size(); i++) {
	//		cout << (std::string)m_loadedPresetsNames.at(i) << endl;
	//	}
	
	if (projectElement) {
		deleteAndZero(projectElement);
	}
	if (openedFile) {
		deleteAndZero(openedFile);
	}
	
	return String::empty;
}

const String 
Commons::saveDocument(const File & file) 
{
	File _xml = file;
	
	/********* XML PARSER *********/
	File* savedFile;
	
	if (_xml.getFileExtension() == String(".xml")) {
		savedFile = new File(_xml.getFullPathName());
	} else {
		savedFile = new File(_xml.getFullPathName() + String(".xml"));
	}
	
	if (savedFile == 0) {
		
		return String("Error, impossible to save.");
		
	} else {
		
		//<VIRAGE>
		XmlElement projet ("VIRAGE");
		projet.setAttribute(T("version"), T("0.2"));
		
		/*		//<NAMESPACE_PRESET>
		 XmlElement* namespacePreset = new XmlElement(T("NAMESPACE_PRESET"));
		 projet.addChildElement(namespacePreset);
		 
		 //<PRESET>
		 hash_map<std::string,  bool>::iterator it = m_selectionState.begin();
		 while (it != m_selectionState.end()) {
		 if (m_hiddenItems.find(it->first) == m_hiddenItems.end()) {
		 XmlElement* preset = new XmlElement ("PRESET");
		 preset->setAttribute (T("address"), (String)it->first.data());
		 
		 //...and add our new element to the parent node
		 namespacePreset->addChildElement (preset);
		 }
		 it++;
		 }
		 
		 //</PRESET>
		 //</NAMESPACE_PRESET>
		 */
		projet.addChildElement(m_presets);
		//</VIRAGE>
		
		// now we can turn the whole thing into a text document..
		const String savedDocument = projet.createDocument (String::empty);
		
		std::ofstream myXmlDocument(savedFile->getFullPathName().toUTF8(), ios::trunc);
		myXmlDocument <<  (std::string)savedDocument;
		myXmlDocument.close();
    }
	
	if (savedFile) {
		deleteAndZero(savedFile);
	}
	
	return String::empty;
}

const String 
Commons::getDocumentTitle()
{
	if(getFile().exists()) {
		return getFile().getFileName();
	} else {
		return "";//s_DEFAULT_MESSAGELIST_NAME;
	}
	
}

void 
Commons::setLastDocumentOpened(const File & file) 
{
	lastDocumentOpened = file;
}

const File 
Commons::getLastDocumentOpened() 
{
	return lastDocumentOpened;
}
/*************************** end FileBasedDocument overwritted-must methods *******************/

