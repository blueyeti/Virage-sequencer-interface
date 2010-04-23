/*
 *  TreeMapStatics.h
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
 * \file TreeMapStatics.h
 * \author Laurent Garnier (LIMSI)
 * \date 2009-2010
 *
 * \brief Contains all the statics variables used in the TreeMap Explorer. This file is included in all TreeMap Explorer classes
 *
 */

#ifndef _TreeMapStatics_H_
#define _TreeMapStatics_H_
#include "includes.h"
#include <sstream>

class NetButton : public TextButton {
	
public:
	NetButton(const String &buttonName=String::empty, const String &toolTip=String::empty) : TextButton(buttonName, toolTip) {
	}
	
	~NetButton() {
	}
	
	const Font getFont() {
		
		return Font(String("Guifx v2 Transports Labeled"), 22, 0);
	}
};

static const float EXPLORER_WIDTH = 1280;
static const float EXPLORER_HEIGHT = 768;
static const float TREE_MAP_WIDTH = 1280;
static const float TREE_MAP_HEIGHT = 2*EXPLORER_HEIGHT/3;

static const float INCRUSTED_EXPLORER_WIDTH = 1200;
static const float INCRUSTED_EXPLORER_HEIGHT = /*620*/700;
static const float INCRUSTED_TREEMAP_WIDTH = 1200;
static const float INCRUSTED_TREEMAP_HEIGHT = 2*INCRUSTED_EXPLORER_HEIGHT/3;

static const float UPDATE_BUTTON_WIDTH = 80;

static const int OFFSET = 10;
static const float LINE_THICKNESS = 1.0;
static const int ACCURACY_DEGREE = 8; //pixels

static const Colour JUCE_BACK_COLOUR = Colour(0xff696969);
static const Colour JUCE_TARGET_COLOUR = Colour(0xff236B8E);
static const Colour JUCE_SELECT_COLOUR = /*Colour(0xff465945)*/Colour(0xff699864);
static const Colour JUCE_PRESELECT_COLOUR = Colour(0xff465945);
static const Colour JUCE_LINE_COLOUR = Colour(0xff000000);
static const Colour JUCE_TEXT_COLOUR = Colour(0xffffffff);

template<typename T>
inline bool valueOf(const std::string &s, T &obj)
{
	std::istringstream is(s);
	return is >> obj;
}

#endif