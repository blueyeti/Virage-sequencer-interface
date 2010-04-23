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
#ifndef _SEQUENCERFRAME_H_
#define _SEQUENCERFRAME_H_

#include "includes.h"
#include "Sequencer.h"
#include "SequencerViewPort.h"
#include "SequencerMap.h"

/*
 * class myViewport
 *
 * The Viewport used for "framing" any sequencer, mainly created in order to
 * make possible this : overriding the visibleAreaChanged(int visibleX, int visibleY, int visibleW, int visibleH)
 * callback, and thus informs the (child) sequencer of its new top left visible coordonate
 * and, finally, it allow us to correctly paint the "drageEnter" and "dragMove" rectangle
 * that appears when a box is dragged in.
 *
 * Quite unimportant in a certain extent.
 *
 */
class myViewport : public SequencerViewPort, public ChangeBroadcaster {
	
public :
 myViewport(bool /*_horizontalSlider*/, Sequencer* _mySequencer):SequencerViewPort(T("")){
		
		setScrollBarsShown(true/*vertical scrollbar*/, true/*horizontal scrollbar*/);
		setScrollBarThickness(20);
		mySequencer=_mySequencer;
	};
	
	~myViewport(){};
	
	void visibleAreaChanged(int visibleX, int visibleY, int /*visibleW*/, int /*visibleH*/){
		mySequencer->setViewedPositionX(visibleX);
		mySequencer->setViewedPositionY(visibleY);
		std::map<unsigned int, Trigger*>::iterator it = mySequencer->getTrigMap()->begin();
		while(it != mySequencer->getTrigMap()->end()){
		
			it->second->setTopLeftPosition( it->second->getX(), visibleY );
			it++;
		}
	}
		
	void zoomSliderChanged () {
		float newRatio = 1.0f + getVZoomSliderValue()/10.0f;
		getSequencer()->getZoomer()->setVerticalRatio( newRatio );
		getSequencer()->refresh();
	};
	
	Sequencer* getSequencer(){		
		return mySequencer;
	};
	

	
		
private:
	Sequencer* mySequencer;
};

//==============================================================================
/*!
 * \class SequencerFrames
 */
class SequencerFrame  : public Component, public ChangeListener
{
public:
    //==============================================================================
    SequencerFrame (int _scrollBarThickness = 20, bool _horizontalSlider = true);
    ~SequencerFrame();
	
	/*JUCE*/void paint (Graphics& g);
    /*JUCE*/void resized();
	Sequencer* getConduite();
	myViewport* getViewPort();
	/*JUCE*/void changeListenerCallback(void* objectThatHasChanged);
	void refresh();
	
protected:
	//ordre importe :
    myViewport* m_viewport;
	Sequencer* m_sequencer;
	SequencerMap* m_map;
};


#endif
