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
#ifndef _RELATIONS_H_
#define _RELATIONS_H_

#include "includes.h"
#include "Box.h"
#include "Behavior.h"
#include "SequenceOT.h"

class TimeBox;

const int behavior_thickness = 10;

//===================================================================================================================================================
/*
 *  Relation.h
 *  Virage
 *
 *  Created by Henry Bernard Bernard on 02/10/08.
 *  Copyright 2008 by Blue Yeti. All rights reserved.
 *
 */


class Relation : public SequenceOT/*pour PropPanel*/ {
	
private:
	int previousY;
	int m_MotherId;
	int m_DaughterId;
	signed int m_originalDeltaT;
	bool m_isToFront;
	bool m_askDeletion;
	Behavior* m_dBehavior;
	
	//dessin
	float lastInputX, lastInputY, lastOutputX, lastOutputY;
    Path linePath, hitPath;
	Colour m_colour;
	
	bool m_isResized;
	
	void getDistancesFromEnds (int /*x*/, int /*y*/, double& /*distanceFromStart*/, double& /*distanceFromEnd*/) const
    {
        float x1, y1, x2, y2;
        getPoints (x1, y1, x2, y2);
		
    //  distanceFromStart = juce_hypot (x - (/*x1 -*/ getX()), y - (/*y1 -*/ getY()));
    //  distanceFromEnd = juce_hypot (x - (/*x2 -*/ getX()), y - (/*y2 -*/ getY()));
    }
	
	void getPoints (float& x1, float& y1, float& x2, float& y2) const
    {
        x1 = lastInputX;
        y1 = lastInputY;
        x2 = lastOutputX;
        y2 = lastOutputY;
		
      /*  GraphEditorPanel* const hostPanel = getGraphPanel();
		
        if (hostPanel != 0)
        {
            FilterComponent* srcFilterComp = hostPanel->getComponentForFilter (sourceFilterID);
			
            if (srcFilterComp != 0)
                srcFilterComp->getPinPos (sourceFilterChannel, false, x1, y1);
			
            FilterComponent* dstFilterComp = hostPanel->getComponentForFilter (destFilterID);
			
            if (dstFilterComp != 0)
                dstFilterComp->getPinPos (destFilterChannel, true, x2, y2);
        }*/
    }
	
public:
	
	int xi, yi, xf, yf;
	/**
	 @brief constructor 
	 constructs a relation between two anchors from two boxes.
	 
	 @param xi x coordinate of the input (left) box
	 @param yi y coordinate of the input (left) box
	 @param xo x coordinate of the output (right) box
	 @param yo y coordinate of the output (right) box
	 @warning input = relation input = former time bound
	 */	 	
	Relation(Sequencer* _parentSequencer, unsigned int _id, int xi, int yi, int xo, int yo, int _MotherId, int _DaughterId, bool isFlexible = false);
	~Relation();	
	
	void listenToMeSequencer();
	/*JUCE*/void paint(Graphics& g);
	/*JUCE*/ bool hitTest (int x, int y);
	
	void setZone(int _xi, int _yi, int _xf, int _yf);

	//Graphical representation
	void setAnchorsPositions(int _x, int _y, int _xx, int _yy);
	void setAnchorsPositions(int _x, int _y, bool _motherBox);

	Path wirePath;
	Path* getWirePath();
	void refresh();
	
	/******************************************************************************/
	//Relation Components
	Behavior* getDurationBehavior();

	/******************************************************************************/
	//Relation focus
	/*JUCE*/void focusLost(FocusChangeType cause);
	unsigned int getMotherId();
	unsigned int getDaughterId();
	void setMotherId(unsigned int _MotherId);
	void setDaughterId(unsigned int _DaughterId);
	/*JUCE*/void resized();
	/*JUCE*/void mouseDown(const MouseEvent &e);
	/*JUCE*/void mouseDrag(const MouseEvent &e);
	/*JUCE*/void mouseMove(const MouseEvent &e);
	/*JUCE*/void mouseUp(const MouseEvent &e);	
	void mouseExit(const MouseEvent &e);
	bool isTriggered(unsigned int _ctrlPtId);
	
	int getBeginValue();
	int getEndValue();
	int getDuration();
	
	/******************************************************************************/
	//Graphical datas
	int getThickness();
	//Component OverWriting
	int getZoneHeight();
	int getZoneWidth();
	int getZoneRight();
	int getZoneX();
	int getZoneY();
	int getZoneBottom();
	Colour getColour();
	
	void setOtColour(Colour _newColour);
	
	void setMuted(bool _m);
	bool isMuted();
};

#endif
