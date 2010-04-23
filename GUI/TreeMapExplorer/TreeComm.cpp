/*
 *  TreeComm.cpp
 *  TreeComm
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

#include "TreeComm.h"
#include "TreeElement.h"
#include "Engines.hpp"

#include <vector>
#include <string>
#include <map>

using namespace std;

TreeComm::TreeComm(Engines *engines) : m_controller(engines)
{
//	m_controller = new Controller();
//	m_controller->pluginLoad("/Library/Application Support/Virage");
//	m_controller->deviceSetCurrent();
	
	//	std::map<std::string, Device*> *devices = m_controller->getCurrentDevices();
	//	map<string, Device*>::iterator itr = devices->begin();
	//	
	//	while( itr != devices->end()){
	//		cout << "device : " << itr->first << endl;
	//		
	//		itr++;
	//	}
}

TreeComm::~TreeComm()
{
	delete m_controller;
}

void
TreeComm::init()
{
	tree<TreeElement*>::iterator root, rootElt, deviceElt;
	root = m_namespaceTree.begin();
	TreeElement *rootDevice = new TreeElement("ROOT", 0, "", "", "", "");
	rootElt = m_namespaceTree.insert(root, rootDevice);
	
//	map<string, Device*> *devices = m_controller->deviceGetCurrent();
	vector<string> devicesName;
	vector<bool> couldSendNamespaceRequest;
	m_controller->getNetworkDevicesName(&devicesName, &couldSendNamespaceRequest);
//	map<string, Device*>::iterator itr = devices->begin();
	vector<string>::iterator itr = devicesName.begin();
	vector<bool>::iterator itr1 = couldSendNamespaceRequest.begin();

	
	//Devices
	while (itr != devicesName.end() && itr1 != couldSendNamespaceRequest.end()) {
			TreeElement *device = new TreeElement();
			device->setLevel("DEVICE");
			
			device->setAbsolutePath(*itr);
			device->setRelativePath(*itr);
			
			deviceElt = m_namespaceTree.append_child(rootElt, device);
			
			if (*itr1) {
				re_init(*itr, deviceElt);
			}
		
		itr++;
		itr1++;
	}
}

void
TreeComm::re_init(const std::string& address, tree<TreeElement*>::iterator currentPosition)
{
	vector<std::string>* nodes = new vector<std::string>;
	vector<std::string>* leaves = new vector<std::string>;
	vector<std::string>* attributs = new vector<std::string>;
	vector<std::string>* values = new vector<std::string>;
	
	m_controller->requestNetworkNamespace(address, nodes, leaves, attributs, values);
	
	if (attributs->size() != 0) {
		for (uint k = 0; k < attributs->size(); k++) {
			if (attributs->at(k).compare("value") == 0) {
				(*currentPosition)->setAttributeValue(values->at(k));
			}
			if (attributs->at(k).compare("type") == 0) {
				(*currentPosition)->setAttributeType(values->at(k));
			}
			if (attributs->at(k).compare("range") == 0) {
				(*currentPosition)->setAttributeRange(values->at(k));
			}
		}
	}
	
	if (leaves->size() != 0) {
		for (uint i = 0; i < leaves->size(); i++) {
			string newAbsPath = address + "/" + leaves->at(i);
			string newRelPath = "/" + leaves->at(i);
			TreeElement *leave = new TreeElement("LEAF", 0, newAbsPath, newRelPath, "", "");
			tree<TreeElement*>::iterator newLeavePosition = m_namespaceTree.append_child(currentPosition, leave);
			
			re_init(newAbsPath, newLeavePosition);
		}
	}
	
	if (nodes->size() != 0) {
		for (uint i = 0; i < nodes->size(); i++) {
			string newAbsPath = address + "/" + nodes->at(i);
			string newRelPath = "/" + nodes->at(i);
			TreeElement *node = new TreeElement("NODE", 0, newAbsPath, newRelPath, "", "");
			tree<TreeElement*>::iterator newNodePosition = m_namespaceTree.append_child(currentPosition, node);
			
			re_init(newAbsPath, newNodePosition);
		}
	}
	
	delete nodes;
	delete leaves;
	delete attributs;
	delete values;
}

void 
TreeComm::sendMessage(const std::string& message)
{	
	m_controller->sendNetworkMessage(message);
}


/*
 //String
 //TreeComm::initWithXML(const File & file){
 //	tree<TreeElement*>::iterator root, rootElt, deviceElt;
 //	root = m_namespaceTree.begin();
 //	
 //	TreeElement *device1 = new TreeElement();
 //	device1->setLevel("DEVICE");
 //	device1->setAbsolutePath("/MinuitDevice");
 //	device1->setRelativePath("/MinuitDevice");
 //	deviceElt = m_namespaceTree.insert(root, device1);
 //	
 //	File* openedFile = new File(file);
 //	XmlDocument openedDocument(*openedFile);
 //    XmlElement* projectElement = openedDocument.getDocumentElement();
 //	
 //	if (projectElement == 0)
 //    {
 //        String error = openedDocument.getLastParseError();
 //		return error;
 //    }
 //    else
 //    {
 //		std::string filenameUTF8((String( file.getFullPathName() )).toUTF8());
 //	
 //		if(projectElement->hasTagName(T("DEVICE"))){
 //			
 //			re_initWithXml("/MinuitDevice", deviceElt, projectElement);
 //		}
 //
 //	}
 //		
 //}
 //String
 //TreeComm::re_initWithXml(std::string address, tree<TreeElement*>::iterator currentPosition, XmlElement *xmlElt){
 //	forEachXmlChildElement(*xmlElt, e){
 //		if(e->hasTagName(T("LEAF"))){
 //			string newAbsPath = address + "/" + e->getStringAttribute(T("address"));
 //			string newRelPath = "/" + e->getStringAttribute(T("address"));
 //			TreeElement *leave = new TreeElement("LEAF", 0, newAbsPath, newRelPath, "", "");
 //			tree<TreeElement*>::iterator newLeavePosition = m_namespaceTree.append_child(currentPosition, leave);
 //			
 //			//re_initWithXml(newAbsPath, newLeavePosition, e);
 //		}
 //		if(e->hasTagName(T("NODE"))){
 //			string newAbsPath = address + "/" + e->getStringAttribute(T("address"));
 //			string newRelPath = "/" + e->getStringAttribute(T("address"));
 //			TreeElement *node = new TreeElement("NODE", 0, newAbsPath, newRelPath, "", "");
 //			tree<TreeElement*>::iterator newNodePosition = m_namespaceTree.append_child(currentPosition, node);
 //			
 //			re_initWithXml(newAbsPath, newNodePosition, e);
 //		}
 //	}
 //}
 */

void
TreeComm::clear()
{
	m_namespaceTree.clear();
}

void
TreeComm::display()
{
	tree<TreeElement*>::iterator sib2 = m_namespaceTree.begin();
	tree<TreeElement*>::iterator end2 = m_namespaceTree.end();
	
	while(sib2 != end2) {
		for(int i = 0; i < m_namespaceTree.depth(sib2); ++i) 
            cout << "   ";
		
		cout << (*sib2)->getRelativePath() << " " << (*sib2)->getAttributeValue() << endl;
		++sib2;
	}
}

int
TreeComm::getTreeTotalDepth()
{
	return m_namespaceTree.max_depth();
}

std::string 
TreeComm::getTreeElementLevel(const std::string& address)
{
	tree<TreeElement*>::iterator sib2 = m_namespaceTree.begin();
	tree<TreeElement*>::iterator end2 = m_namespaceTree.end();
	std::string level = "";
	
	while(sib2 != end2) {
		if((*sib2)->getAbsolutePath().compare(address) == 0){
			level = (*sib2)->getLevel();
			return level;
		}
		++sib2;
	}
	return "ERROR";
}

std::string 
TreeComm::getTreeElementRelativeAddress(const std::string& address)
{
	tree<TreeElement*>::iterator sib2 = m_namespaceTree.begin();
	tree<TreeElement*>::iterator end2 = m_namespaceTree.end();
	std::string relativeAddress = "";
	
	while(sib2 != end2) {
		if((*sib2)->getAbsolutePath().compare(address) == 0){
			relativeAddress = (*sib2)->getRelativePath();
			return relativeAddress;
		}
		++sib2;
	}
	return "ERROR";	
}

std::string 
TreeComm::getTreeElementValue(const std::string& address)
{
	tree<TreeElement*>::iterator sib2 = m_namespaceTree.begin();
	tree<TreeElement*>::iterator end2 = m_namespaceTree.end();
	std::string value = "";
	
	while(sib2 != end2) {
		if((*sib2)->getAbsolutePath().compare(address) == 0){
			value = (*sib2)->getAttributeValue();
			return value;
		}
		++sib2;
	}
	return "ERROR";		
}

std::string 
TreeComm::getTreeElementType(const std::string& address)
{
	tree<TreeElement*>::iterator sib2 = m_namespaceTree.begin();
	tree<TreeElement*>::iterator end2 = m_namespaceTree.end();
	std::string type = "";
	
	while(sib2 != end2) {
		if((*sib2)->getAbsolutePath().compare(address) == 0){
			type = (*sib2)->getAttributeType();
			return type;
		}
		++sib2;
	}
	return "ERROR";		
}

std::string 
TreeComm::getTreeElementRange(const std::string& address)
{
	tree<TreeElement*>::iterator sib2 = m_namespaceTree.begin();
	tree<TreeElement*>::iterator end2 = m_namespaceTree.end();
	std::string range = "";
	
	while(sib2 != end2) {
		if((*sib2)->getAbsolutePath().compare(address) == 0){
			range = (*sib2)->getAttributeRange();
			return range;
		}
		++sib2;
	}
	return "ERROR";		
}


vector<string>
TreeComm::getDevices()
{
	vector<string> devices;
	tree<TreeElement*>::iterator sib2 = m_namespaceTree.begin();
	tree<TreeElement*>::iterator end2 = m_namespaceTree.end();
	
	while(sib2 != end2) {
		if((*sib2)->getLevel().compare("DEVICE") == 0){
			//cout << (*sib2)->getAbsolutePath() << endl;
			devices.push_back((*sib2)->getAbsolutePath());
		}
		++sib2;
	}
	
	return devices;
}

vector<string>
TreeComm::getChilds(const string& address)
{
	vector<string> sons;
	tree<TreeElement*>::iterator dad = NULL;
	tree<TreeElement*>::iterator sib2 = m_namespaceTree.begin();
	tree<TreeElement*>::iterator end2 = m_namespaceTree.end();
	
	while(sib2 != end2) {
		if((*sib2)->getAbsolutePath().compare(address) == 0){
			dad = sib2;
		}
		++sib2;
	}
	
	if (dad != NULL) {
		tree<TreeElement*>::sibling_iterator sib = m_namespaceTree.begin(dad);
		while(sib != m_namespaceTree.end(dad)) {
			//cout << (*sib)->getRelativePath() << endl;
			sons.push_back((*sib)->getRelativePath());
			++sib;
		}	
	} else {
		cout << "address doesn't appear in the tree" << endl; 	
	}
	
	return sons;
}

void
TreeComm::getSnapshot(const string& address, std::vector<std::string> *snapshot)
{
	vector<string> childs = getChilds(address);
	
	for(uint i = 0; i < childs.size(); i++){
		string newAddress = address + childs.at(i);
		
		if(getTreeElementLevel(newAddress).find("LEAF") == 0){
			snapshot->push_back(newAddress);	
		} else {
			getSnapshot(newAddress, snapshot);	
		}
	}		
}

void 
TreeComm::getSubTreeAddresses(const std::string& address, std::vector<std::string> *subTree)
{
	vector<string> childs = getChilds(address);
	
	for(uint i = 0; i < childs.size(); i++){
		string newAddress = address + childs.at(i);
		//cout << "newAddress " << newAddress << endl;
		
		subTree->push_back(newAddress);	
		getSubTreeAddresses(newAddress, subTree);	
		
	}	
}


void
TreeComm::setPriority(const std::string& address, int priority)
{
	tree<TreeElement*>::iterator sib2 = m_namespaceTree.begin();
	tree<TreeElement*>::iterator end2 = m_namespaceTree.end();
	
	while(sib2 != end2) {
		if((*sib2)->getAbsolutePath().compare(address) == 0)
			(*sib2)->setPriority(priority);
		++sib2;
	}	
}

void 
TreeComm::reinitValue(const std::string& address)
{
	//send a request to address to get the message value
	vector<std::string>* nodes = new vector<std::string>;
	vector<std::string>* leaves = new vector<std::string>;
	vector<std::string>* attributs = new vector<std::string>;
	vector<std::string>* values = new vector<std::string>;
	
	m_controller->requestNetworkNamespace(address, nodes, leaves, attributs, values);
	
	std::string value = "";
	if (attributs->size() != 0) {
		for (uint k = 0; k < attributs->size(); k++) {
			if (attributs->at(k).compare("value") == 0) {
				value = values->at(k);
			}
		}
	}
	
	//update the value in the tree
	tree<TreeElement*>::iterator sib2 = m_namespaceTree.begin();
	tree<TreeElement*>::iterator end2 = m_namespaceTree.end();
	
	while(sib2 != end2) {
		if((*sib2)->getAbsolutePath().compare(address) == 0)
			(*sib2)->setAttributeValue(value);
		++sib2;
	}	
	
	delete nodes;
	delete leaves;
	delete attributs;
	delete values;
}

bool 
TreeComm::isFirstLeave(const std::string& address)
{
	tree<TreeElement*>::iterator current = m_namespaceTree.begin();
	tree<TreeElement*>::iterator end = m_namespaceTree.end();
	
	tree<TreeElement*>::sibling_iterator sib;
	
	while(current != end) {
		if((*current)->getAbsolutePath().compare(address) == 0)
			sib = current;
		++current;
	}
	
	int index = m_namespaceTree.index(sib);
	//std::cout << index << std::endl;
	
	return index == 0;
}

bool 
TreeComm::isLastLeave(const std::string& address)
{
	tree<TreeElement*>::iterator current = m_namespaceTree.begin();
	tree<TreeElement*>::iterator end = m_namespaceTree.end();
	
	tree<TreeElement*>::sibling_iterator sib;
	
	while(current != end) {
		if((*current)->getAbsolutePath().compare(address) == 0)
			sib = current;
		++current;
	}
	
	uint index = m_namespaceTree.index(sib);
	//std::cout << index << std::endl;
	
	return index ==  m_namespaceTree.number_of_siblings(sib);
}
