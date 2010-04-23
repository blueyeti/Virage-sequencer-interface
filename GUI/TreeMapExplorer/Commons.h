/*
 *  Commons.h
 *  TreeMapExplorer
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

/*!
 * \file Commons.h
 * \author Laurent Garnier (LIMSI)
 * \date 2009-2010
 *
 * \brief Make the link between the treeMap and the column explorer.
 *
 */

#ifndef _Commons_H_
#define _Commons_H_

#include "includes.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <ext/hash_map> 

class Explorer;
class TimeBox;

//redefine the namespace __gnu_cxx to use hash_map with string type key
using namespace __gnu_cxx;
namespace __gnu_cxx                                                                                 
{                                                                                             
	template<> struct hash< std::string >                                                       
	{                                                                                           
		size_t operator()( const std::string& x ) const                                           
		{                                                                                         
			return hash< const char* >()( x.c_str() );                                              
		}                                                                                         
	};                                                                                          
}

class Commons : public FileBasedDocument
{
public:
	Commons(Explorer *explorer);
	~Commons();
	
	Explorer *getExplorer();
	
	void addStateMapMessage(const std::string& absoluteAddress, bool isSelected = false);
	void setSelectedToAssign(const std::string& absoluteAddress, bool isSelected, bool TreeMapToRefresh);
	bool isSelectedToAssign(const std::string& absoluteAddress);
	void refresh(bool TreeMapToRefresh);
	
	void addValueMapMessage(const std::string& absoluteAddress, const std::string& value);
	void setMessageValue(const std::string& absoluteAddress, const std::string& value);
	std::string getMessageValue(const std::string& absoluteAddress);
	
	void setMessageType(const std::string& absoluteAddress, const std::string& type);
	std::string getMessageType(const std::string& absoluteAddress);
	
	void setMessageRange(const std::string& absoluteAddress, const std::string& range);
	void setMessagePopup(const std::string& absoluteAddress);
	void showPopupmenu(const std::string& absoluteAddress);
	
	std::map<std::string, std::string> getSelectedLeaves();
	std::vector<std::string> getSelectedLeavesValues(std::vector<std::string> leaves);
	
	//note that if the right click is not performed on the selected items, it concerned only the targeted items
	//that's for the 3 next methods
	void hideSelectedItems(bool isItemSelected, const std::string& absoluteAddress);
	void hideSimilarItems(bool isItemSelected, const std::string& absoluteAddress);
	void displayOnlySelectedItems(bool isItemSelected, const std::string& absoluteAddress);
	
	void displayOnlyDoubleClickedItem(const std::string& absoluteAddress);
	void reinitDisplay();
	void setPreSelectedItems(TimeBox *edited);
	
	void storePreset(const std::string& presetName);
	void storePresetAs();
	void activePreset(uint index);
	
	void savePresetsToXml();
	void loadPresetsFromXml();
	
	//FileBasedDocument Methods Overwriting 
	/*JUCE*/const String loadDocument(const File & file);
	/*JUCE*/const String saveDocument(const File & file);
	/*JUCE*/const String getDocumentTitle();
	/*JUCE*/const File getLastDocumentOpened();
	/*JUCE*/void setLastDocumentOpened(const File & file);
	
	hash_map<std::string, uint> m_instances;
	hash_map<std::string, bool> m_instancesStartFromZero;
	hash_map<std::string, bool> m_hiddenItems;
	hash_map<std::string, bool> m_preSelectedItems;
	
private:
	Explorer *m_explorer;
	hash_map<std::string, bool> m_selectionState;
	hash_map<std::string, std::string> m_messageValue;
	hash_map<std::string, std::string> m_messageType;
	hash_map<std::string, std::string> m_messageRange;
	hash_map<std::string, PopupMenu*> m_messagePopup;
	
	File lastDocumentOpened;
	XmlElement *m_presets;
	String m_currentPresetName;
	std::vector<String> m_loadedPresetsNames;
	
};

#endif