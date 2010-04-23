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
 *  PropertyFrame.h
 *  Virage
 *
 *  Created by dev on 17/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PropertyFrame_h_
#define _PropertyFrame_h_

#include "includes.h"
#include "SequenceOtPropertyPanel.h"
#include "Sequencer.h"

#define PADDING 3
#define MIN_WIDTH 50


class PropertyFrame : public Component, public ChangeListener {

public:
	PropertyFrame(Component* _highestComponent) : m_highestParent(_highestComponent){
	}

	~PropertyFrame(){}

	Component* getHighestParent() {

		return m_highestParent;
	}

	void setContentComponent(Component* _contentComp){

		//if(deleteOldOne) deleteAndZero(m_contentComponent);
		removeChildComponent(m_contentComponent);
		addAndMakeVisible(m_contentComponent = _contentComp);
		resized();
	}

	void resized(){

		if(m_contentComponent!=0) m_contentComponent->setBounds(PADDING, PADDING, getWidth() - PADDING*2, getHeight() - PADDING*2);
		repaint();

		if(getWidth() > MIN_WIDTH) m_originalWidth = getWidth();
		if(getWidth() < MIN_WIDTH) setSize(MIN_WIDTH, getHeight());
	}

	void refreshAll(){

		//SequenceOtPropertyPanel
		if(m_contentComponent!=0)
			if(dynamic_cast<PropertyPanel*>  (m_contentComponent))
				((PropertyPanel*) m_contentComponent)->refreshAll();
	}

	void setConcerned(Component* _component){

		//SequenceOtPropertyPanel
		if(m_contentComponent!=0)
			if(dynamic_cast<SequenceOtPropertyPanel*>  (m_contentComponent)) ((SequenceOtPropertyPanel*) m_contentComponent)->setConcerned(_component);
	}

	void changeListenerCallback(void* objectThatHasChanged){

		//SequenceOtPropertyPanel
		if(m_contentComponent!=0)
			if(dynamic_cast<ChangeListener*>  (m_contentComponent)) ((ChangeListener*) m_contentComponent)->changeListenerCallback(objectThatHasChanged);
	}

	void paint(Graphics& g) {
		g.fillAll(Colours::black/*black*/);
		g.setColour(s_COLOUR_SELECTION_BLUE);
		if(m_contentComponent!=0) g.fillRoundedRectangle(0, 0, getWidth(),  m_contentComponent-> getHeight()+PADDING*2, 4);
	}

	int getOriginalWidth() {
		return m_originalWidth;
	}

	unsigned int getMinWidth() {

		return MIN_WIDTH;
	}



private:
	int m_originalWidth;
	Component* m_contentComponent;
	Component* m_highestParent;
};

#endif
