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
/*
 *  OpenNetwork.h
 *  Virage
 *
 *  Created by dev on 17/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "includes.h"

#ifndef _CommButton_H_
#define _CommButton_H_


class OpenCommButtonDrawable : public Drawable {
	
public:
	OpenCommButtonDrawable(){}
	~OpenCommButtonDrawable(){}
	
	void draw (Graphics &g, const AffineTransform &/*transform*/=AffineTransform::identity) const {
		
		int width = s_MAINWINDOW_PADDING_LEFT-5;
		
		g.setColour(s_COLOUR_SELECTION_BLUE);
		g.fillEllipse(0, 0, width, width);
		g.fillRect(width/2, 0, 300, width);
		
		g.setColour(Colours::black);
		g.drawText(String("C"), 0,0,width,width,Justification::centred,false);
	}
	
	void 	getBounds (float &/*x*/, float &/*y*/, float &/*width*/, float &/*height*/) const {}
	bool 	hitTest (float /*x*/, float /*y*/) const {return true;}
	Drawable * 	createCopy () const {return 0;}
};

/****************************************************************************/

class CloseCommButtonDrawable : public Drawable {
public:
	CloseCommButtonDrawable(){}
	~CloseCommButtonDrawable(){}
	
	void draw (Graphics &g, const AffineTransform &/*transform*/=AffineTransform::identity) const {
		
		int width = s_MAINWINDOW_PADDING_LEFT-5;
		
		g.setColour(Colours::lightgrey);
		g.fillEllipse(0, 0, width, width);
		
		g.setColour(Colours::black);
		g.drawText(String("C"), 0,0,width,width,Justification::centred,false);
	}
	
	void 	getBounds (float &/*x*/, float &/*y*/, float &/*width*/, float &/*height*/) const {}
	bool 	hitTest (float /*x*/, float /*y*/) const {return true;}
	Drawable * 	createCopy () const {return 0;}		
};

/****************************************************************************/

class CommButton : public ToolbarButton {
public:
	CommButton() : ToolbarButton (/*id*/5, String("Expand"), new CloseCommButtonDrawable, new OpenCommButtonDrawable )
	{
		setWantsKeyboardFocus(false);
		setClickingTogglesState(true);
	}
	
	~CommButton(){};
	
};

#endif
