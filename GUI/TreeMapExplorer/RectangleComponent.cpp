/*
 *  GraphicsInfos.cpp
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

#include "RectangleComponent.h"
#include "TreeMapCanvas.h"
#include "Commons.h"

#include <string>

using namespace std;

RectangleComponent::RectangleComponent(TreeMapCanvas *parent, Commons *commons, std::string absoluteAddress, std::string relativeAddress, std::string value, 
									   std::string type, std::string range, int recListIndex, float x, float y, float width, float height, uint instanceNb, bool hasToBeVisible) :
m_parent(parent), m_commons(commons), m_absoluteAddress(absoluteAddress), m_relativeAddress(relativeAddress), m_value(value), m_type(type), m_range(range),
m_recListIndex(recListIndex), m_x(x), m_y(y), m_width(width), m_height(height), m_instanceNb(instanceNb), m_hasToBeVisible(hasToBeVisible)

{
	m_istargeted = false;
	m_isselected = false;
	m_isPopuped = false;

	//Instances process : TODO
//	if (m_instanceNb != 0) {
//		m_instanceNb++;
//		for (uint i = 0; i <= m_instanceNb; i++) {
//			TextButton *t = new TextButton(String(i));
//			addAndMakeVisible(t);
//			m_instanceButtons.push_back(t);
//		}	
//	}
}

RectangleComponent::~RectangleComponent()
{
	deleteAllChildren();	
}

std::string
RectangleComponent::getAbsoluteAddress()
{
	return m_absoluteAddress;	
}

std::string
RectangleComponent::getRelativeAddress()
{
	return m_relativeAddress;	
}

std::string
RectangleComponent::getValue()
{
	return m_value;	
}

std::string
RectangleComponent::getType()
{
	return m_type;	
}

std::string
RectangleComponent::getRange()
{
	return m_range;	
}

float 
RectangleComponent::getX()
{
	return m_x;	
}

float 
RectangleComponent::getY()
{
	return m_y;
}

float 
RectangleComponent::getWidth()
{
	return m_width;
}

float 
RectangleComponent::getHeight()
{
	return m_height;
}

bool
RectangleComponent::isTargeted()
{
	return m_istargeted;
}

bool
RectangleComponent::isSelected()
{
	return m_isselected;	
}

bool 
RectangleComponent::hasToBeVisible()
{
	return m_hasToBeVisible;	
}


void 
RectangleComponent::paint(Graphics& g){
}

void
RectangleComponent::resized()
{
	setBounds(m_x, m_y, m_width, m_height);
//	for (uint i = 0; i < m_instanceNb; i++) {
//		m_instanceButtons.at(i)->setBounds(i*20, 0, 20, 20); 	
//	}
}

void 
RectangleComponent::mouseEnter(const MouseEvent& e)
{
	m_istargeted = true;
	m_parent->updateTarget(m_recListIndex, true);	
}

void 
RectangleComponent::mouseExit(const MouseEvent& e)
{
	m_istargeted = false;
	m_parent->updateTarget(m_recListIndex, false);
}

void 
RectangleComponent::mouseDown(const MouseEvent& e)
{	
	if (e.mods.isPopupMenu()) {
		m_isPopuped = true;
		m_istargeted = true;
		
		m_parent->updateTarget(m_recListIndex, true);	
		m_commons->showPopupmenu(m_absoluteAddress);
	} else {
		m_parent->updateVisualSelection(this, e);
	}
	m_isPopuped = false;
}

void 
RectangleComponent::mouseDoubleClick(const MouseEvent& e)
{
	//m_commons->displayOnlySelectedItems();
	if (!e.mods.isPopupMenu()) {
		m_commons->displayOnlyDoubleClickedItem(m_absoluteAddress);	
	}
}


