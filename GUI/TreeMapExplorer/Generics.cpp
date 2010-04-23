/*
 *  Generics.cpp
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

#include "Generics.h"
#include <string>

//Colour JUCE_BACK_COLOUR = Colours::darkgrey;
//Colour JUCE_TARGET_COLOUR = Colour(123, 150, 100);
//Colour JUCE_SELECT_COLOUR = Colour(4, 139, 154);
//Colour JUCE_LINE_COLOUR = Colours::black;
//Colour JUCE_TEXT_COLOUR = Colours::white;

void
Generics::drawRectangle(void *ptr, float x, float y, float width, float height, float weight)
{

#ifdef JUCE_API
	{
		Image *image = (Image *)ptr;
		
		Graphics g (*image);
		g.setColour (JUCE_LINE_COLOUR);
		
		g.drawRect(x, y, width, height, LINE_THICKNESS);
		
	}

#endif
	
#ifdef QC_API
	{
		
	}

#endif
}

void
Generics::fillRectangle(void *ptr, std::string text, float x, float y, float width, float height, int color)
{
	
#ifdef JUCE_API
	{
		Image *image = (Image *)ptr;
		
		Graphics g (*image);
		//neither targeted nor selected  
		if (color == 0) {
			g.setColour (JUCE_BACK_COLOUR);
			g.fillRect(x, y, width, height);
		}
		//targeted
		else if (color == 1) {
			g.setColour (JUCE_TARGET_COLOUR);
			g.fillRect(x, y, width, height);
		}
		//selected
		else if (color == 2) {
			g.setColour (JUCE_SELECT_COLOUR);
			g.fillRect(x, y, width, height);	
		}
		//preselected
		else if (color == 3) {
			g.setColour (JUCE_PRESELECT_COLOUR);
			g.fillRect(x, y, width, height);
		}
	}
	
#endif
	
#ifdef QC_API
	{
		
	}
	
#endif
}

void 
Generics::drawText(void *ptr, std::string text, float x, float y, float width, float height)
{
	
#ifdef JUCE_API
	{
		Image *image = (Image *)ptr;
		
		Graphics g (*image);
		g.setColour (JUCE_TEXT_COLOUR);
		if(height > OFFSET)
			g.setFont(12.0);
		else
			g.setFont(1.0 * height);
		
		std::string textWithoutSlash = text;
		if (text.length() > 0) {
			if (text[0] == '/') {
				textWithoutSlash = text.substr(1);
			}
		}
		//g.drawText(text.data(), x, y, width, height, Justification::topLeft, true);
		g.drawFittedText(textWithoutSlash.data(), x, y, width, height, /*Justification::topLeft*/Justification::centredTop, 1);
	}
	
#endif
	
#ifdef QC_API
	{
		
	}
	
#endif
}

void 
Generics::drawLine(void *ptr, float x, float y, float endX, float endY)
{
	
#ifdef JUCE_API
	{
		Image *image = (Image *)ptr;
		
		Graphics g (*image);
		g.setColour (JUCE_LINE_COLOUR);
		
		g.drawLine(x, y, endX, endY, LINE_THICKNESS);
	}
	
#endif
	
#ifdef QC_API
	{
		
	}
	
#endif
}

//void
//Generics::drawTreeMap(void *ptr, std::vector<void *> *recList, std::string address, float x1, float y1, float x2, float y2, int axis)
//{
//	Image *image = (Image *)ptr;
//	
//	std::vector<std::string> childs = m_tree->getChilds(address);
//	std::string relativeAddress = m_tree->getTreeElementRelativeAddress(address);
//	std::string value = m_tree->getTreeElementValue(address);
//	std::string type = m_tree->getTreeElementType(address);
//	std::string range = m_tree->getTreeElementRange(address);
//	std::string absoluteAddress;
//	
//	//"Deep" in the treemap
//	//	if(childs.size() != 0){
//	//		drawLine(image, x1, y1, x1+OFFSET, y1+OFFSET);
//	//		drawLine(image, x2-OFFSET, y1+OFFSET, x2, y1);
//	//		drawLine(image, x1, y2, x1+OFFSET, y2-OFFSET);
//	//		drawLine(image, x2-OFFSET, y2-OFFSET, x2, y2);
//	//	}
//	
//	if(relativeAddress.length() != 0 /*&& !(m_tree->getTreeElementLevel(address).compare("DEVICE")==0 && childs.size()==0)*/){
//		drawText(image, relativeAddress,  x1, y1, x2-x1, y2-y1);
//		drawRectangle(image, x1, y1, x2-x1, y2-y1, 0);
//		
//		//the over invisible component which implements mouse activity 
//		RectangleComponent *rec;
//		rec = new RectangleComponent(this, address, relativeAddress, value, type, range, recList->size(), x1, y1, x2-x1, y2-y1);
//		recList->push_back(rec);
//		
//		//Apply an OFFSET between nodes in tree visualisation
//		x1 = x1+OFFSET;
//		y1 = y1+OFFSET;
//		x2 = x2-OFFSET;
//		y2 = y2-OFFSET;
//		
//		//no OFFSET between the leaves
//		if(m_tree->getTreeElementLevel(address).compare("LEAF") == 0 && !m_tree->isFirstLeave(address) && !m_tree->isLastLeave(address)){
//			y1 = y1-OFFSET;
//			y2 = y2+OFFSET;
//		}
//		
//		//except the first top
//		if(m_tree->getTreeElementLevel(address).compare("LEAF") == 0 && m_tree->isFirstLeave(address)){
//			y2 = y2+OFFSET;
//		}
//		
//		//except the last bottom
//		if(m_tree->getTreeElementLevel(address).compare("LEAF") == 0 && m_tree->isLastLeave(address)){
//			y1 = y1-OFFSET;
//		}
//	}
//	
//	//treemap algorithm
//	float width; 
//	if(axis == 0)
//		width = x2 - x1;
//	else
//		width = y2 - y1;
//	
//	for(int i = 0; i < childs.size(); i++){
//		absoluteAddress = address + childs.at(i);
//		if(axis == 0){
//			x2 = x1 + (1.0f/childs.size())*width;
//			drawTreeMap(image, recList, absoluteAddress, x1, y1, x2, y2, 1-axis);
//			x1 = x2;
//		}
//		else{
//			y2 = y1 + (1.0f/childs.size())*width;
//			drawTreeMap(image, recList, absoluteAddress, x1, y1, x2, y2, 1-axis);
//			y1 = y2;	
//		}
//	}
//	
//	//cout << "fin treemap" << endl;
//}




