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
#ifndef __JUCER_HEADER_TRIGGER_VIRAGETRIGGER_EC729DB8__
#define __JUCER_HEADER_TRIGGER_VIRAGETRIGGER_EC729DB8__

//[Headers]     -- You can add your own extra header files here --
#include "includes.h"
#include "TimeBox.h"
#include "ControlJack.h"
#include "SequenceOT.h"
//[/Headers]

class Trigger  : public SequenceOT
{
public:
    //==============================================================================
    Trigger (unsigned int _id, Sequencer* _parent, TimeBox* _controlledBox, bool triggedToBegin , int x, int y);
    ~Trigger();

	void listenToMeSequencer();
    //==============================================================================
	//Virage
	void init();
	void disconnect();
	bool isConnected();
	bool isAskingDeletion();
	ControlJack* getControlJack();
	void setLinkedBox(TimeBox* _box, bool _anchoringStart, bool loadedTrig = false);
	TimeBox* getLinkedBox();
	E_ControlPoint getLinkedCtrlPoint();
	bool isAnchoringStart();
	bool isDeletionAsked();
	Colour getColour();
	void setColour(Colour newColour);
	void setMinuitMessage(String _minuitMessage);
	String getMinuitMessage();
	/***********************************/
	
	
	bool isTriggered(unsigned int _ctrlPtId);
	void refresh() {};
	//callback
	void linkHasBroken();
	void waitedTriggerPointCallBack(bool _isWaited);
	
	/*Component*/
	void paint (Graphics& g);
    void moved();
    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);
    void mouseMove (const MouseEvent& e);
    void mouseUp (const MouseEvent& e); 
    void mouseDoubleClick (const MouseEvent& e);

	
	void setOtColour(Colour _newColour);
	
	 Behavior* getDurationBehavior();
	
	int getPlugX();
	int getPlugY();
	
	E_TriggerState getState();

	void setMuted(bool _m);
	bool isMuted();
	void burn();
    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	TimeBox* m_controlledBox;
	ControlJack* mControlJack;
	BubbleMessageComponent* m_bubbleInfo;	
	bool m_anchoringBoxStart;
	bool m_muted;
	bool m_connected;
	bool m_askDeletion;	
	E_TriggerState m_execState;
	Colour m_colour;
	String m_minuitMessage;
   //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    Trigger (const Trigger&);
    const Trigger& operator= (const Trigger&);
};


#endif   // __JUCER_HEADER_TRIGGER_VIRAGETRIGGER_EC729DB8__
