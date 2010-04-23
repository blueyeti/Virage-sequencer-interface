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
 *  Facility.h
 *  Virage
 *
 *  Created by Henry Bernard on 23/02/09.
 *  Copyright 2009 BY. All rights reserved.
 *
 */

#ifndef _FACILITY_H_
#define _FACILITY_H_

#define PI 3.14

#include "includes.h"
#include <cmath>


class Facility : public Component {
	
private:
	bool side_factor;
	
public:
	Facility(){
		//setSize(s_FACILITY_WIDTH*2, s_FACILITY_HEIGHT*2);
		setVisible(false);
		side_factor = false;
	};
	
	~Facility(){};
	
	void paint(Graphics &g){
		
		g.setColour(Colours::darkred);
		g.setOpacity(0.3f);
		g.drawEllipse(getWidth()/2 - s_FACILITY_WIDTH, getHeight()/2 - s_FACILITY_HEIGHT, s_FACILITY_WIDTH*2, s_FACILITY_HEIGHT*2, 2);
	}
	
	void setCenter(int x, int y){
		setTopLeftPosition(x-getWidth()/2, y-getHeight()/2);
	}
	
	void setSide(bool anchoringStart){
		side_factor = anchoringStart;
	}
	
	void mouseDown(const MouseEvent &e){	
		getParentComponent()->mouseDown(e.getEventRelativeTo(getParentComponent()));
		//fonctionne
	}	
	
	void mouseUp(const MouseEvent &e){	
		getParentComponent()->mouseUp(e.getEventRelativeTo(getParentComponent()));
		//fonctionne
	}
	
	void mouseDrag(const MouseEvent &e){	
		getParentComponent()->mouseDrag(e.getEventRelativeTo(getParentComponent()));
		//fonctionne
	}
};

#endif
