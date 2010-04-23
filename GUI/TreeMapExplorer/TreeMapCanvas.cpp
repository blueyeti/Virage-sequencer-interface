/*
 *  TreeMapCanvas.cpp
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

#include "TreeMapCanvas.h"
#include "Explorer.h"
#include "Commons.h"
#include "Generics.h"
#include "RectangleComponent.h"
#include "Math.h"

TreeMapCanvas::TreeMapCanvas(Explorer *explorer, Commons *commons, bool widerSize) :
m_explorer(explorer), m_commons(commons)
{
	recList = new std::vector<RectangleComponent*>;
	m_selectedVisualItem = new SelectedItemSet<RectangleComponent*>();
	
	dc = new DrawableComposite();	
	di = new DrawableImage();
	
	if (widerSize) {
		setBounds(0, 0, TREE_MAP_WIDTH, TREE_MAP_HEIGHT);
		myImage = new Image(Image::RGB, TREE_MAP_WIDTH, TREE_MAP_HEIGHT, true);
	} else {
		setBounds(0, 0, INCRUSTED_TREEMAP_WIDTH, INCRUSTED_TREEMAP_HEIGHT);
		myImage = new Image(Image::RGB, INCRUSTED_TREEMAP_WIDTH, INCRUSTED_TREEMAP_HEIGHT, true);
	}
	
	drawTreeMap();
}

TreeMapCanvas::~TreeMapCanvas()
{
	delete dc;
	delete myImage;
	delete di;
	
	//	recList->clear();
	//	delete recList;
	
	deleteAndZero(m_selectedVisualItem);
}

void
TreeMapCanvas::drawTreeMap()
{
	float xstart = 0.0;
	float ystart = 0.0;
	float xend = getWidth();
	float yend = getHeight();
	
	generateTreeMap("", xstart, ystart, xend, yend, 0);
	
	di->setImage(myImage, false);
	dc->insertDrawable(*di);
	
	repaint();
	resized();
	
	for (vector<RectangleComponent*>::iterator it = recList->begin(); it != recList->end(); it++) {
		(*it)->resized();
		addAndMakeVisible(*it);
		
		//create the general selection map
		string tmp = (*it)->getAbsoluteAddress();
		m_commons->addStateMapMessage(tmp);	
		m_commons->setMessageValue(tmp, (*it)->getValue());
		m_commons->setMessageType(tmp, (*it)->getType());
		m_commons->setMessageRange(tmp, (*it)->getRange());
	}
	
	repaint();
}

void 
TreeMapCanvas::update()
{	
	m_selectedVisualItem->deselectAll();
	
	myImage->clear(0, 0, getWidth(), getHeight());
	di->clearImage();
	dc->removeDrawable(0);
	
	int i = 0;
	for (vector<RectangleComponent*>::iterator it = recList->begin(); it != recList->end(); it++) {
		removeChildComponent(*it);
		delete (*it);
	}
	
	recList->clear();	
	
	drawTreeMap();
}

/*****************************************************/
/***********Instances process : TODO******************/
std::vector<bool>
TreeMapCanvas::processInstances(std::vector<std::string> siblings, std::string absAddress)
{
	
	std::vector<bool> isInstance;
	uint num = 0;
	for (uint i = 0; i < siblings.size(); i++) {
		//cout << absAddress + siblings.at(i) << endl;
		
		size_t pointPos = siblings.at(i).find(".");
		if (pointPos != string::npos) {
			std::string	address = siblings.at(i).substr(0, pointPos);
			std::string instance = siblings.at(i).substr(pointPos+1, siblings.size());
			
			bool startFromZero = false;
			for (uint j = 0; j < siblings.size(); j++) {
				if (siblings.at(j).compare(address) == 0) {
					startFromZero = true;
				}
			}
			m_commons->m_instancesStartFromZero[absAddress + address] = startFromZero;
			
			uint instanceNb = 0;
			valueOf(instance, instanceNb);
			if (instanceNb >= num) {
				num = instanceNb;
				m_commons->m_instances[absAddress + address] = instanceNb;
				cout << absAddress + address << " " << instanceNb << endl;
				
			}
			
			if (!startFromZero /*&& instanceNb == 1*/) {
				isInstance.push_back(false);
			} else {
				isInstance.push_back(true);
			}
		} else {
			isInstance.push_back(false);
		}
	}
	
	return isInstance;
}

bool
TreeMapCanvas::isAnInstance(std::string address)
{
	return (address.find(".") != string::npos);
}
/*****************************************************/
/*****************************************************/

void
TreeMapCanvas::generateTreeMap(std::string address, float x1, float y1, float x2, float y2, int axis)
{
	int num = 0;
	std::vector<std::string> childs = m_explorer->getChilds(address);
	
	//TODO
	//	std::vector<bool> isInstance = processInstances(childs, address);
	//	for (uint i = 0; i < isInstance.size(); i++) {
	//		if (isInstance.at(i))
	//			num++;
	//	}

	std::string relativeAddress = m_explorer->getTreeElementRelativeAddress(address);
	std::string value = m_explorer->getTreeElementValue(address);
	std::string type = m_explorer->getTreeElementType(address);
	std::string range = m_explorer->getTreeElementRange(address);
	std::string absoluteAddress;
	
	float w; 
	float h;
	bool hasToBeVisible;
	
	if (axis == 0) {
		w = x2 - x1;
		h = y2 - y1;
	} else { 
		w = y2 - y1;
		h = x2 - x1;
	}
	
	if (w > 0 && h > 0) {
		m_generics->fillRectangle(myImage, relativeAddress, x1, y1, x2-x1, y2-y1, 0);
		m_generics->drawText(myImage, relativeAddress,  x1, y1, x2-x1, y2-y1);
		m_generics->drawRectangle(myImage, x1, y1, x2-x1, y2-y1, 0);
		hasToBeVisible = true;
	} else {
		hasToBeVisible = false;	
	}
	
	//TODO
	int numInstance = 0;
	//		hash_map<std::string, uint>::iterator it = m_commons->m_instances.find(address);
	//		if (it != m_commons->m_instances.end()) {
	//			numInstance = it->second;	
	//			//cout << address << ": " << numInstance << endl;
	//		}
	
	//the over invisible component which implements mouse activity 
	RectangleComponent *rec;
	rec = new RectangleComponent(this, m_commons, address, relativeAddress, value, type, range, recList->size(), x1, y1, x2-x1, y2-y1, numInstance, hasToBeVisible);
	recList->push_back(rec);
	
	//Apply an OFFSET between nodes in tree visualisation
	x1 = x1+OFFSET;
	y1 = y1+OFFSET;
	x2 = x2-OFFSET;
	y2 = y2-OFFSET;
	
	//no OFFSET between the leaves
	if (m_explorer->getTreeElementLevel(address).compare("LEAF") == 0 && !m_explorer->isFirstLeave(address) && !m_explorer->isLastLeave(address)) {
		y1 = y1-OFFSET;
		y2 = y2+OFFSET;
	}
	
	//except the first top
	if (m_explorer->getTreeElementLevel(address).compare("LEAF") == 0 && m_explorer->isFirstLeave(address)) {
		y2 = y2+OFFSET;
	}
	
	//except the last bottom
	if (m_explorer->getTreeElementLevel(address).compare("LEAF") == 0 && m_explorer->isLastLeave(address)) {
		y1 = y1-OFFSET;
	}
	//	}
	
	//treemap algorithm
	float width; 
	if (axis == 0) {
		width = x2 - x1;
	} else { 
		width = y2 - y1;
	}
	
	for (uint i = 0; i < childs.size(); i++) {
		
		absoluteAddress = address + childs.at(i);
		
		if (axis == 0) {
			x2 = x1 + (1.0f/(childs.size()-num))*width;
			generateTreeMap(absoluteAddress, x1, y1, x2, y2, 1-axis);
			x1 = x2;
		} else {
			y2 = y1 + (1.0f/(childs.size()-num))*width;
			generateTreeMap(absoluteAddress, x1, y1, x2, y2, 1-axis);
			y1 = y2;	
		}
	}
}

void
TreeMapCanvas::drawTreeElement(void *ptr, std::string text, int x, int y, int width, int height, int color, int weight, int offset)
{
	if(offset == 0){ 
		
		m_generics->fillRectangle(ptr, text, x, y, width, height, color);
		m_generics->drawRectangle(ptr, x, y, width, height, weight);
		
		m_generics->drawText(ptr, text, x, y, width, height);
	} else {
		
		m_generics->fillRectangle(ptr, text, x, y, width, offset, color);
		m_generics->fillRectangle(ptr, text, x, y, offset, height, color);
		m_generics->fillRectangle(ptr, text, x, y+height-offset, width, offset, color);
		m_generics->fillRectangle(ptr, text, x+width-offset, y, offset, height, color);
		m_generics->drawRectangle(ptr, x, y, width, height, weight);
		
		m_generics->drawText(ptr, text, x, y, width, height);
	}
}

void
TreeMapCanvas::updateTarget(int recListIndex, bool isTargeted)
{
	if (recList->size() > 0) {
		if (!m_selectedVisualItem->isSelected(recList->at(recListIndex))) {
			if (m_explorer->getTreeElementLevel(recList->at(recListIndex)->getAbsoluteAddress()).compare("LEAF") == 0) {
				//leave targeted
				if (recList->at(recListIndex)->isTargeted()) {
					drawTreeElement(myImage, recList->at(recListIndex)->getRelativeAddress(),  
									recList->at(recListIndex)->getX(), 
									recList->at(recListIndex)->getY(), 
									recList->at(recListIndex)->getWidth(), 
									recList->at(recListIndex)->getHeight(), 
									1, 0, 0);
				} 
				//leave untargeted
				else {
					drawTreeElement(myImage, recList->at(recListIndex)->getRelativeAddress(),  
									recList->at(recListIndex)->getX(), 
									recList->at(recListIndex)->getY(), 
									recList->at(recListIndex)->getWidth(), 
									recList->at(recListIndex)->getHeight(), 
									0, 0, 0);
				}
				
			} else {
				//node targeted
				if (recList->at(recListIndex)->isTargeted()) {
					drawTreeElement(myImage, recList->at(recListIndex)->getRelativeAddress(),  
									recList->at(recListIndex)->getX(), 
									recList->at(recListIndex)->getY(), 
									recList->at(recListIndex)->getWidth(), 
									recList->at(recListIndex)->getHeight(),
									1, 0, OFFSET);
				} 
				//node untargeted
				else {
					drawTreeElement(myImage, recList->at(recListIndex)->getRelativeAddress(),  
									recList->at(recListIndex)->getX(), 
									recList->at(recListIndex)->getY(), 
									recList->at(recListIndex)->getWidth(), 
									recList->at(recListIndex)->getHeight(), 
									0, 0, OFFSET);
				}
			}
			di->setImage(myImage, false);
			dc->insertDrawable(*di);
			repaint();
		}
	}
	
	m_explorer->setTargetedAddressLabel(recList->at(recListIndex)->getAbsoluteAddress());
}

void 
TreeMapCanvas::updateVisualSelection(RectangleComponent* rectComp, const MouseEvent& e)
{
	m_selectedVisualItem->addToSelectionBasedOnModifiers(rectComp, e.mods);
	
	//if a node is selected, it subtree too (recip. deselected)
	std::vector<std::string> *subTree = new std::vector<std::string>;
	m_explorer->getSubTreeAddresses(rectComp->getAbsoluteAddress(), subTree);
	
	for (uint i = 0; i < subTree->size(); i++) {
		if (m_selectedVisualItem->isSelected(rectComp)) {
			m_selectedVisualItem->addToSelection(getRectangleComponent(subTree->at(i)));
			m_commons->setSelectedToAssign(subTree->at(i), true, false);
		} else {
			m_selectedVisualItem->deselect(getRectangleComponent(subTree->at(i)));
			m_commons->setSelectedToAssign(subTree->at(i), false, false);	
		}
	}	
	delete subTree;
	
	for (uint i = 0; i < recList->size(); i++) {
		
		if (m_selectedVisualItem->isSelected(recList->at(i))) {
			if (recList->at(i)->hasToBeVisible()) {
				drawTreeElement(myImage, recList->at(i)->getRelativeAddress(),  
								recList->at(i)->getX(), 
								recList->at(i)->getY(), 
								recList->at(i)->getWidth(), 
								recList->at(i)->getHeight(), 
								2, 0, 0);
			}
			m_commons->setSelectedToAssign(recList->at(i)->getAbsoluteAddress(), true, false);
		} else {
			if (recList->at(i)->hasToBeVisible()) {
				drawTreeElement(myImage, recList->at(i)->getRelativeAddress(),  
								recList->at(i)->getX(), 
								recList->at(i)->getY(), 
								recList->at(i)->getWidth(), 
								recList->at(i)->getHeight(), 
								0, 0, 0);
			}
			m_commons->setSelectedToAssign(recList->at(i)->getAbsoluteAddress(), false, false);
		}
	}
	m_commons->refresh(false);
	
	di->setImage(myImage, false);
	dc->insertDrawable(*di);
	repaint();
}

SelectedItemSet<RectangleComponent*>*
TreeMapCanvas::getSelectionSet() 
{
	return m_selectedVisualItem;
}

RectangleComponent*
TreeMapCanvas::getRectangleComponent(std::string absoluteAddress)
{
	for (uint i = 0; i < recList->size(); i++)
		if (recList->at(i)->getAbsoluteAddress().compare(absoluteAddress) == 0)
			return recList->at(i);
	return NULL;
}

void 
TreeMapCanvas::paint(Graphics &g)
{
	dc->drawAt(g, 0, 0);
}

void 
TreeMapCanvas::resized()
{
	for (uint i = 0; i < recList->size(); i++) {
		recList->at(i)->setBounds(recList->at(i)->getX(), recList->at(i)->getY(), recList->at(i)->getWidth(), recList->at(i)->getHeight());	
	}
}

void
TreeMapCanvas::addItemToSelection(std::string absoluteAddress, bool isSelected) 
{
	if (isSelected)
		m_selectedVisualItem->addToSelection(getRectangleComponent(absoluteAddress));
	else
		m_selectedVisualItem->deselect(getRectangleComponent(absoluteAddress));	
}

void
TreeMapCanvas::refresh()
{
	for (uint i = 0; i < recList->size(); i++) {
		
		if (m_selectedVisualItem->isSelected(recList->at(i))) {
			if (recList->at(i)->hasToBeVisible()) {
				drawTreeElement(myImage, recList->at(i)->getRelativeAddress(),  
								recList->at(i)->getX(), 
								recList->at(i)->getY(), 
								recList->at(i)->getWidth(), 
								recList->at(i)->getHeight(), 
								2, 0, 0);	
			}
		} else {
			if (recList->at(i)->hasToBeVisible()) {
				drawTreeElement(myImage, recList->at(i)->getRelativeAddress(),  
								recList->at(i)->getX(), 
								recList->at(i)->getY(), 
								recList->at(i)->getWidth(), 
								recList->at(i)->getHeight(), 
								0, 0, 0);
			}
		}
	}
	
	di->setImage(myImage, false);
	dc->insertDrawable(*di);
	repaint();
}
