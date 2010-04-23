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
 *  EditablePoint.h
 *  Virage
 *
 *  Created by Henry on 08/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _EDITABLEPOINT_H_
#define _EDITABLEPOINT_H_

#define POINT_SIZE 7

class EditablePoint : public Component, public ChangeBroadcaster {
private:
	int xpc;
	int ypc;
	bool m_askToBeDeleted;
	bool m_isbeingdragged;
	
public:
	EditablePoint(ChangeListener* _parent, int _xpc = 0, int _ypc = 0) : xpc(_xpc), ypc(_ypc), m_askToBeDeleted(false), m_isbeingdragged(false) {
		
		setSize(POINT_SIZE, POINT_SIZE);
		addChangeListener(_parent);
		setAlwaysOnTop(true);
		setWantsKeyboardFocus(true);
	}
	
	~EditablePoint(){}
	
	void paint(Graphics& g){
		
		#ifdef VDBG
			g.setColour(Colours::lightgrey);
		#endif
		
		/*focused ?*/		if(getParentComponent()->getNumChildComponents() - 1 == getParentComponent()->getIndexOfChildComponent(this) ) {
			#ifdef VDBG	
				g.setColour(Colours::red);
			#endif
			#ifndef VDBG	
				g.setColour(Colours::lightgrey);
			#endif
		}
		g.fillEllipse(0, 0, getWidth(), getHeight());
		g.setColour(Colours::black);
		g.drawEllipse(0, 0, getWidth(), getHeight(), 1);
	}
	
	void mouseDrag(const MouseEvent& e){m_isbeingdragged = true;
		
		int y = e.getEventRelativeTo(getParentComponent()).y - POINT_SIZE/2;
		int x = e.getEventRelativeTo(getParentComponent()).x - POINT_SIZE/2;
		if(isAnExtrema()) x = getX();
		setTopLeftPosition(x, y);
		sendSynchronousChangeMessage(this);/*a destination du scope*/
		m_isbeingdragged = true;
	}
	
	void mouseUp(const MouseEvent& e) {m_isbeingdragged = false;
		
		m_isbeingdragged = false;
		
		if(e.mouseWasClicked()) {
		
			m_askToBeDeleted = true;//clicker une fois sur le point le détruit, géré par le change listener
		}
		
		sendSynchronousChangeMessage(this);/*a destination du scope*/
	}
	
	bool isBeingDragged() {
		return m_isbeingdragged;
	}
	
	
	int getXPC(){return xpc;}
	int getYPC(){return ypc;}
	void setXYPC(int _xpc, int _ypc){
		xpc = _xpc;
		ypc = _ypc;
	}
	
	bool askDeletetion(){
		
		return m_askToBeDeleted;
	}

	bool isMoving() {
		
		return m_isbeingdragged;
	}

	void setIsMoving(bool _isBeingMoved){
	
		m_isbeingdragged = _isBeingMoved;
	}
	
	
	
	
	
	
	
	
	
	
	
	


	
	bool keyPressed	(const KeyPress & 	key) {
		
		
		if(key==KeyPress::deleteKey || key==KeyPress::backspaceKey) {
			m_askToBeDeleted = true;
			sendSynchronousChangeMessage(this);
		}
		
		/*
		if(key==KeyPress::rightKey) {
			m_isbeingdragged = true;
			setTopLeftPosition(getX()+1, getY());
			sendSynchronousChangeMessage(this);
			m_isbeingdragged = false;
			return true;
		}
		if(key==KeyPress::leftKey) {
			m_isbeingdragged = true;
			setTopLeftPosition(getX()-1, getY());
			sendSynchronousChangeMessage(this);
			m_isbeingdragged = false;
			return true;
		}
		if(key==KeyPress::upKey) {
			m_isbeingdragged = true;
			setTopLeftPosition(getX(), getY()-1);
			sendSynchronousChangeMessage(this);
			m_isbeingdragged = false;
			return true;
		}
		if(key==KeyPress::downKey) {
			m_isbeingdragged = true;
			setTopLeftPosition(getX(), getY()+1);
			sendSynchronousChangeMessage(this);
			m_isbeingdragged = false;
			return true;
		}
		if(key==KeyPress(KeyPress::rightKey, ModifierKeys::shiftModifier, 0)) {
			m_isbeingdragged = true;
			setTopLeftPosition(getX()+20, getY());
			sendSynchronousChangeMessage(this);
			m_isbeingdragged = false;
			return true;
		}
		if(key==KeyPress(KeyPress::leftKey, ModifierKeys::shiftModifier, 0)) {
			m_isbeingdragged = true;
			setTopLeftPosition(getX()-20, getY());
			sendSynchronousChangeMessage(this);
			m_isbeingdragged = false;
			return true;
		}
		if(key==KeyPress(KeyPress::upKey, ModifierKeys::shiftModifier, 0)) {
			m_isbeingdragged = true;
			setTopLeftPosition(getX(), getY()-20);
			sendSynchronousChangeMessage(this);
			m_isbeingdragged = false;
			return true;
		}
		if(key==KeyPress(KeyPress::downKey, ModifierKeys::shiftModifier, 0)) {
			m_isbeingdragged = true;
			setTopLeftPosition(getX(), getY()+20);
			sendSynchronousChangeMessage(this);
			m_isbeingdragged = false;
			return true;
		}
		*/
		
		return false;
	}
	
	
	
	bool isAnExtrema(){
	
		return (getXPC() == s_SCALE_LENGTH || getXPC() == 0);
	}

};



#endif
