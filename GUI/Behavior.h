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
 *  Behavior.h
 *  Virage
 *
 *  Created by Henry Bernard Bernard on 28/10/08.
 *  Copyright 2008 by Henry Bernard, Blue Yeti. All rights reserved.
 *
 */
#ifndef _Behavior_H_
#define _Behavior_H_

#define NO_STOP  0
#define NO_START 0

#include "includes.h"
#include <iostream>
#include <stdio.h>

class TimeBox;
class Relation;
class SequenceOT;
class Sequencer;

/*!
 * \class Behavior
 * Since both boxes and relations get finally the same duration constraints,
 * we have objectivised it into the Behavior.class
 *
 * Behavior is a set of N flagged mode, you can choose one amongst for a Box or a Relation.
 * Behavior has its own graphic component, added to the upper border of a Box, or into the horizontal line of a Relation.
 */
class Behavior : public Component, public ChangeListener {
	
private:
	bool m_flexible;
	int m_flexStart, m_flexStop;
	int m_startpoint;
	Colour m_colour;
	SequenceOT* m_concerned;
	
public:
	Behavior(SequenceOT* _concerned, bool _flexible, int flex_start = NO_START, int flex_stop = NO_STOP);
	~Behavior();
	/*set*/
	void setFlexGroup(bool isFlex, int Fs, int Fe, int _parentWidth);
	void setFlexible(bool _flexible);
	void setFlexStart(int _flexStart);
	void setFlexStop(int _flexStop);
	void setColour(Colour _b);
	/*get*/
	bool isFlexible();
	int getFlexStart();
	int getFlexStop();
	
	/*JUCE*/void paint(Graphics &g);
	/*JUCE*/void mouseDown(const MouseEvent &e);	
	/*JUCE*/void focusGained (FocusChangeType cause);	
	/*JUCE*/void focusLost (FocusChangeType cause);
	
	int getDuration();
	void setDuration(int ms);
	void changeListenerCallback(void* objectThatHasChanged);
};

#endif