/*
 *  RectangleComponent.h
 *  TreeMapExplorer
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

/*!
 * \file RectangleComponent.h
 * \author Laurent Garnier (LIMSI)
 * \date 2009-2010
 *
 * \brief This class represents a treeMap item over invisible component which catch the mouse behaviours.
 *
 */

#ifndef _RectangleComponent_H_
#define _RectangleComponent_H_

#include "includes.h"
#include <string>

class TreeMapCanvas;
class Commons;

class RectangleComponent : public Component
{	
public:
	RectangleComponent(TreeMapCanvas *parent, Commons *commons, std::string absoluteAddress, std::string relativeAddress, std::string value, 
					   std::string type, std::string range, int recListIndex, float x, float y, float width, float height, uint instanceNb=0, bool hasToBeVisible=true);
	~RectangleComponent();
	
	std::string getAbsoluteAddress();
	std::string getRelativeAddress();
	std::string getValue();
	std::string getType();
	std::string getRange();
	float getX();
	float getY();
	float getWidth();
	float getHeight();
	bool isTargeted();
	bool isSelected();
	bool hasToBeVisible();
	
	void paint(Graphics& g);
	void resized();
	
	void mouseEnter(const MouseEvent& e);
	void mouseExit(const MouseEvent& e);
	void mouseDown(const MouseEvent& e);
	void mouseDoubleClick(const MouseEvent& e);
		
private:
	TreeMapCanvas *m_parent;
	Commons *m_commons;
	std::string m_absoluteAddress;
	std::string m_relativeAddress;
	std::string m_value;
	std::string m_type;
	std::string m_range;
	int m_recListIndex;
	float m_x;
	float m_y;
	float m_width;
	float m_height;
	bool m_istargeted;
	bool m_isselected;
	bool m_isPopuped;
	bool m_hasToBeVisible;
	
	uint m_instanceNb;
	std::vector<TextButton *> m_instanceButtons;
};

#endif