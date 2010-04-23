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
 *  TimeBox.h
 *  Virage
 *
 *  Created by Henry Bernard and Luc Vercellin on 02/10/08.
 *  Copyright 2008 by Blue Yeti. All rights reserved.
 *
 */

#ifndef _TimeBox_H_
#define _TimeBox_H_

#include "includes.h"
#include "Box.h"
#include "Behavior.h"
#include "TimeBoxContentComponent.h"
#include "SequenceOT.h"
#include "MessageTextEditorWindow.h"
#include "MessageInterpolationWindow.h"

#include <map>

#define CPT_L 1
#define CPT_R 2

/*!
 * \class TimeBox
 *
 * TimeBox.class is abstract, to generate different TimeBox as OscBox, MidiBox, MinuitBox... and your own stuff you may want to.
 *
 * TimeBox.class is derived from virage::Box.class
 */
class TimeBox : public Box, public Timer {
public:
	TimeBox(Sequencer* _parentSequencer, unsigned int index, String etiquette = "default", bool _isFlexible = false, int _x = 0, int _y = 0, int _width = 100, int _height = 50);
	TimeBox(unsigned int _id, TimeBox* _timeBox);
	//copy of timeboxes
	unsigned int copyRange;
	String m_originName;
	virtual ~TimeBox();

	ProgressBar* getProgressBar();

	unsigned int getId();

	void listenToMeSequencer();

	//root const Y, H
	float getRootY();
	float getRootH();

	/*Timer*/
	void timerCallback();
	Anchor* getAnchorLeft();
	Anchor* getAnchorRight();
	bool idDuplicatingWithAlt;

	//return x,ycoordinate of left & right anchor for parent XY system
	int getPlugStartX();
	int getPlugEndX();
	int getPlugY();
	bool hitAnchor(int x, int y);

	void setTitleColour(Colour _titleColour, float _alpha = 1.0f);

	/*MUTING*/
	void setMuted(bool _muted);
	bool isMuted();

	//Parent Sequencer
	void mouseDown(const MouseEvent &e);
	void mouseUp(const MouseEvent &e);
	void mouseDrag(const MouseEvent &e);
	void mouseEnter(const MouseEvent &e);

	std::map<unsigned int, unsigned int> getConnectedBoxesMap(unsigned int _ctrlPtId, unsigned int _ctrlPtIdTo);

	unsigned int getTimeInMs();
	void init();
	void raz();
	void refresh();

	/*Component*/
	void resized();
	void paint(Graphics &g);
	bool keyPressed(const KeyPress & key);
	void modifierKeysChanged(const ModifierKeys& modifiers);
	void buttonClicked (Button* buttonThatWasClicked);
	
	//selection
	/*JUCE overwriting*/void setVisible(bool shouldBeVisible);

	void moved();
	/*VIRAGE*/int getMovedDX();
	/*VIRAGE*/int getMovedDY();


	/*contient exactement le message*/
	/*VIRAGE*/ bool containsMessage(String _message, unsigned int _cp);
	/*contient la chaine de caractere (donc tous les messages de tailles supérieur)*/
	/*VIRAGE*/ bool containsString(String _message, unsigned int _cp);

	/*VIRAGE*/ /*CALLBACK*/ void controlPointDone(unsigned int controlPointIndex);

	//Relation register management
	virtual void setNewRelation(unsigned int id_rel, bool start, bool mother);
	bool isRelStart(unsigned int i);
	bool isMotherOfRel(unsigned int i);
	void deleteRelationId(int id);
	std::map<unsigned int, bool>* getRelationMap();
	std::map<unsigned int, bool>* getTriggersMap();
	bool isConnectedByRel(int id);

	void updateTriggers(String _s);

	//Triggers register management
	virtual void setNewTrigger(unsigned int id_rel, bool connectedToStart);
	void deleteTrigger(bool connectedToStart);
	void updateTriggersMessage(String _boxOldTitle);
	void titleChanged(String _oldTitle);
	void deleteTriggerId(int id);
	bool isTriggered(unsigned int _ctrlPtId);

	//element access
	Behavior* getDurationBehavior();

	void setTitleText(String _oldTitle, String _newTitle);
	String getTitle();
	void setComment(String _comment);
	String getComment();
	void showComment();

	//Control point
	void setOscMessageOutStart(String _message);
	void setOscMessageOutEnd(String _message);
	String getOscMessageInStart();
	String getOscMessageInStop();
	String getOscMessageOutStart();
	String getOscMessageOutStop();
	vector<std::string>* getOscMessageOutStartVector();
	vector<std::string>* getOscMessageOutStopVector();

	void sendInterpolatedMessages(vector<string> messages, vector<bool> redondances, int frequency);
	vector<string> getInterpolatedMessages();
	void setInterpolated(bool _isInterpolated);
	bool isInterpolated();

	//WINDOWS PROP

	void showMessagesEditor(bool _startMessagesWanted);
	void setMessageEditorsEnabled(bool _isEnabled);
	void updateMessagesEditor(int _controlPointIndex, std::vector<string> _messages);
	void updateMessagesEditor(int _controlPointIndex, String _messages);

	void setMessagesEditorTitle(String _title);
	void setMessagesEditorColour(Colour _colour);

	void initInterpolationWindow();
	void showInterpolationWindow();

	void setAllInterpolated(bool _areInterpolated);

	//Automation
	void editAutomation();

	TimeBoxContentComponent* getTimeBoxContentComponent();

	virtual void textEditorReturnKeyPressed (TextEditor& editor);
	virtual void textEditorFocusLost (TextEditor& editor);

private:
	//internal graphical components
	Component* mainComponent;
	Anchor* m_anchorRight;
	Anchor* m_anchorLeft;
	double progressVal;
	ProgressBar* m_progressBar;
	unsigned int m_time;
	
	//selection et copie
	bool m_resultOfMouseDownSelectMethod;
	unsigned int coeff_de_copie;
	
	//delta from each move step
	int m_getX_after_moved, m_getY_after_moved, m_getX_before_moved, m_getY_before_moved;
	int m_counter_move;
	const int m_latency_move;
	
	bool isShiftDown;
	unsigned int _lastUpdate;
	
	//relations register
	std::map<unsigned int, bool>* m_relRegister;
	std::map<unsigned int, bool>* m_trigRegister;
	std::map<unsigned int, bool>* m_motherOfRel;
	
	//mouse facility zones
	Path m_Path;
	Behavior* m_dBehavior;
	TimeBoxContentComponent *m_contentComponent;
	
	unsigned int m_first_CP_done;
	unsigned int m_last_CP_done;
	
	//base statique
	float unzoomed_y;
	float unzoomed_height;
	
	/*MUTING*/
	bool m_isMuted;
	
	MessageTextEditorWindow* m_startMessagesEditorWindow;
	MessageTextEditorWindow* m_endMessagesEditorWindow;
	MessageInterpolationWindow* m_interpolationWindow;
};

#endif
