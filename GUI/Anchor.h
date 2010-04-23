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
 *  Anchor.h
 *  Virage
 *
 *  Created by Henry Bernard on 20/02/09.
 *  Copyright 2009 BlueYeti. All rights reserved.
 *
 */

#ifndef _ANCHOR_H_
#define _ANCHOR_H_

#include "includes.h"
/*!
 * \class Anchor
 * Anchor is a squared in a virage::Box,
 * where a relation can be slotted.
 *@note Anchor are MouseEvent transparent, any mouse click/move/drag/up will be communicated to the "grand grand parent", which means the first upper sequencer. Therefore you can directly ask the sequencer, helped with Sequencer::getHitAnchorBoxId()
 * Anchor.class is derived from juce::Component
 */
class Anchor : public Component {
private:
	bool attached; 
	
public:
	Anchor(){
		attached = false;
		setMouseCursor(MouseCursor::MouseCursor(MouseCursor::PointingHandCursor));
	}
	~Anchor(){}
	
	void paint(Graphics &g){
		g.fillAll(Colours::white);
		if(isAttached()) {g.fillAll(s_COLOUR_UNSELECTION);}; 
		g.setColour(Colours::black);
		g.drawRect(0,0,getWidth(),getHeight(),1);
	}
	
	void setAttached(bool _attached){
		attached = _attached;
	}
	
	bool isAttached(){
		return attached;
	}
	
	void mouseDown(const MouseEvent &e){	
		getParentComponent()->getParentComponent()->getParentComponent()->mouseDown(e.getEventRelativeTo(getParentComponent()->getParentComponent()->getParentComponent()));
		//fonctionne
	}	
	
	void mouseUp(const MouseEvent &e){	
		getParentComponent()->getParentComponent()->getParentComponent()->mouseUp(e.getEventRelativeTo(getParentComponent()->getParentComponent()->getParentComponent()));
		//fonctionne
	}
	
	void mouseDrag(const MouseEvent &e){	
		getParentComponent()->getParentComponent()->getParentComponent()->mouseDrag(e.getEventRelativeTo(getParentComponent()->getParentComponent()->getParentComponent()));
		//fonctionne
	}
	void mouseMove(const MouseEvent &e){	
		
		getParentComponent()->getParentComponent()->getParentComponent()->mouseDrag(e.getEventRelativeTo(getParentComponent()->getParentComponent()->getParentComponent()));
		//fonctionne
	}

};

#endif