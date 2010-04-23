/*
 *  TreeMapCanvas.h
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
 * \file TreeMapCanvas.h
 * \author Laurent Garnier (LIMSI)
 * \date 2009-2010
 *
 * \brief The component which draws the treemap explorer. 
 *
 */

#ifndef _TreeMapCanvas_H_
#define _TreeMapCanvas_H_

#include "includes.h"
#include "TreeComm.h"
#include <vector>
#include <map>

class Explorer;
class Generics;
class Commons;
class RectangleComponent;

class TreeMapCanvas : public Component
{
public:
	TreeMapCanvas(Explorer *explorer, Commons *commons, bool widerSize);
	~TreeMapCanvas();
	
	void drawTreeMap();
	void generateTreeMap(std::string address, float x1, float y1, float x2, float y2, int axis);
	
	void drawTreeElement(void *ptr, std::string text, int x, int y, int width, int height, int color, int weight, int offset);
	
	std::vector<bool> processInstances(std::vector<std::string> siblings, std::string absAddress);
	bool isAnInstance(std::string address);

	void update();
	void updateTarget(int recListIndex, bool isTargeted);
	void updateVisualSelection(RectangleComponent* rectComp, const MouseEvent& e);
	
	SelectedItemSet<RectangleComponent*> *getSelectionSet();
	
	RectangleComponent* getRectangleComponent(std::string absoluteAddress);
	
	void commonSetMessageValue(std::string address, std::string value);
	std::string commonGetMessageValue(std::string address);
	
	void paint(Graphics &g);
	void resized();
	void addItemToSelection(std::string absoluteAddress, bool isSelected);
	void refresh();
		
private:
	Explorer *m_explorer;
	Generics *m_generics;
	Commons *m_commons;
	
	DrawableComposite *dc;
	DrawableImage *di;
	Image *myImage;
	
	std::vector<RectangleComponent*> *recList;
	
	SelectedItemSet<RectangleComponent*> *m_selectedVisualItem;
	
};

#endif