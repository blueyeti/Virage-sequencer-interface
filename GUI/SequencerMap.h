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
 *  SequencerMap.h
 *  Virage
 *
 *  Created by Henry Bernard and Luc Vercellin on 20/04/09.
 *  Copyright 2009 BlueYeti.Fr. All rights reserved.
 *
 * SequencerMap est une map de visualisation type AbletonLive/MaxMSP
 * qui permet une vision synthétique de la séquence, le zoom par MouseDrag,
 * le déplacement de la zone de visualisation par MouseMove,
 * la modification de la zone de visualition en resizant une fenêtre libre dans SequencerMap
 *
 */

#include "includes.h"
#include "Sequencer.h"

#define VIEWER_BORDER_THICKNESS 3

class TimeBox;

class Viseur : public Component, public ChangeBroadcaster {

private:
	int _previous_e_y, _previous_e_x, _clickedCenterX, _centerShiftX;
	bool _resizeByBorder, _clicked, _cmdDown;
	Sequencer *m_sequencer;

public:
	Viseur(Sequencer* _sequencer) : Component(){
		m_sequencer = _sequencer;
		_previous_e_y = 0;
		_previous_e_x = 0;
		_centerShiftX = 0;
		_clickedCenterX = 0;
		_cmdDown = false;
		_resizeByBorder = false;

		setSize(s_DEFAULT_MAP_WIDTH, 200);
		setAlwaysOnTop(true);

		_clicked = false;
	}

	~Viseur() {}
	
	void mouseOver(const MouseEvent& e) 
	{
		setMouseCursor(MouseCursor::MouseCursor(MouseCursor::PointingHandCursor));
	}

	void mouseUp(const MouseEvent& /*e*/)
	{
		_resizeByBorder = false;
		_clicked = false;
		_cmdDown = false;
	}

	void mouseDown(const MouseEvent& e)
	{
		if (getMouseCursor() == MouseCursor::LeftEdgeResizeCursor
				|| getMouseCursor() == MouseCursor::RightEdgeResizeCursor) {
			_resizeByBorder = true;
		}
		else {
			_resizeByBorder = false;
		}

		_clicked = true;

		_previous_e_y = e.getEventRelativeTo(getParentComponent()).y;
		_previous_e_x = e.getEventRelativeTo(getParentComponent()).x;

		_clickedCenterX = getX() + getWidth()/2;
		_centerShiftX = _clickedCenterX - _previous_e_x;
		_cmdDown = e.mods.isCommandDown();
	}

	//trivial (feedback pointeur souris)
	void mouseMove(const MouseEvent& e) {
		if (!_clicked) {
			if( e.x > getWidth() - VIEWER_BORDER_THICKNESS) {
				setMouseCursor(MouseCursor::RightEdgeResizeCursor);
			}
			else if(e.x < VIEWER_BORDER_THICKNESS) {
				setMouseCursor(MouseCursor::LeftEdgeResizeCursor);
			}
			else {
				setMouseCursor(MouseCursor::MouseCursor(MouseCursor::PointingHandCursor));
			}
		}
	}

	void mouseDrag(const MouseEvent &e) {
		m_sequencer->isZooming = true;
		
		if (_clicked) {
			if (e.getEventRelativeTo(getParentComponent()).x < 0) {
				return;
			}
			if (!_resizeByBorder) {
				if(e.mods.isPopupMenu())
					return;
				int newWidth = getWidth();
				
				/*zoom horizontal*/
				if (std::abs(_previous_e_x-e.getEventRelativeTo(getParentComponent()).x) < VIEWER_BORDER_THICKNESS
						&& _cmdDown) {
					float zoom = 3.0f * (_previous_e_y - e.y);
					int maxWidth = getParentComponent()->getWidth();
					int minWidth = s_MIN_MAP_WIDTH;
					newWidth = getWidth() + zoom;
					newWidth = std::min(newWidth,maxWidth);
					newWidth = std::max(newWidth,minWidth);
				}
				
				/*deplacement*/
				int newX = _clickedCenterX + _centerShiftX - newWidth/2;
				if (!_cmdDown) {
					newX = e.getEventRelativeTo(getParentComponent()).x + _centerShiftX - newWidth/2;
				}
				if (newX  >= 0 && newX <= (getParentComponent()->getWidth() - s_MIN_MAP_WIDTH)) {
					setBounds(newX, 0,  newWidth, getHeight() );
					resized();
				}
			}
			else {
				//resize par bord gauche
				if (getMouseCursor() == MouseCursor::LeftEdgeResizeCursor) {
					setBounds(std::max(0, e.getEventRelativeTo(getParentComponent()).x), 0 , std::max(s_MIN_MAP_WIDTH,getRight() - e.getEventRelativeTo(getParentComponent()).x) , getHeight());
					resized();
				}
				//resize par bord droit
				else if (getMouseCursor() == MouseCursor::RightEdgeResizeCursor) {
					setSize(std::min(getParentComponent()->getWidth(),e.getEventRelativeTo(getParentComponent()).x - getX()), getHeight());
					resized();
				}
			}
			_previous_e_y = e.getEventRelativeTo(getParentComponent()).y;
			_previous_e_x = e.getEventRelativeTo(getParentComponent()).x;
		}
		
		m_sequencer->isZooming = false;
	}

	void paint(Graphics& g) {
		g.setColour(Colours::black.withAlpha(0.4f)/*transparence pour voir boite sous traits noirs*/);
		//contour
		g.drawRect(0,0,getWidth(), getHeight(), VIEWER_BORDER_THICKNESS);
		//coins
		//g.fillRect(VIEWER_BORDER_THICKNESS, VIEWER_BORDER_THICKNESS, 6, 6);
	}

	void resized() {
		verifyBounds();
		sendSynchronousChangeMessage(this);
	}

	void moved() {
		verifyBounds();
		sendSynchronousChangeMessage(this);
	}

	void verifyBounds() {
	//	/*init*/ if(getHeight() < 18) setSize(getWidth(), 18);
		/*init*/
		if(getWidth() < s_MIN_MAP_WIDTH)
			setSize(s_MIN_MAP_WIDTH, getHeight());

		/*si deborde a gauche*/
		if( getX() < 0 )
			setTopLeftPosition(0,/*getY()*/0);
		/*si deborde a droite*/
		if( getRight() > getParentWidth() )
			setTopLeftPosition(  getParentWidth() - getWidth(), getY());
	}

	void mouseWheelMove(const MouseEvent& /*e*/, float wheelIncrementX, float /*wheelIncrementY*/){

	  setTopLeftPosition(getX() - signof(wheelIncrementX), getY() );//- signof( wheelIncrementY));
	}
};


/**
 *	class SequencerMap
 */
class SequencerMap : public Component {

public:
	SequencerMap(Sequencer* _sequencer);
	~SequencerMap();

	void paint(Graphics& g);
	void resized();
	void mouseDown(const MouseEvent& e);
	void changeListenerCallback (void *objectThatHasChanged);
	Viseur* getViseur();
	void mouseWheelMove(const MouseEvent& e, float wheelIncrementX, float wheelIncrementY);

private:
	Sequencer* m_sequencer;
	Viseur* m_viseur;

};
