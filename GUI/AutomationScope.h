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
 *  AutomationScope.h
 *  Virage
 *
 *  Created by Henry on 02/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _AUTOMATIONSCOPE_H_
#define _AUTOMATIONSCOPE_H_

#include "includes.h"
#include "AutomationCurve.h"
#include "EditablePoint.h"
#include "Engines.hpp"

class AutomationScope : public AutomationCurve, public ChangeListener {

public:
	AutomationScope(int _id, Engines* _en, int startValue, int endValue);
	~AutomationScope();

	/*Component*/
	void paint(Graphics& g);
	void mouseDown(const MouseEvent& e);
	void mouseUp(const MouseEvent& e);
	void mouseDrag(const MouseEvent& e);
	void resized();

	/*AutomationScope*/
	void addAPoint(int xClick, int yClick);
	void addAPCPoint(int xpc, int ypc, bool sendUpdateMessage = true);
	void delAPoint(int xpc, int ypc, bool sendUpdateMessage = true);
	void setConcerned(AutomationCurve* _concerned, String _concernedName = String::empty, Component* _OTConcerned = NULL);
	void removeConcerned();
	void refreshConcerned();
	void setExtInfo(String _concernedName, Component* _OTConcerned);
	AutomationCurve* getConcerned();
	void setEditionTool(E_AutomationToolMode _newTool);
	int convertXToRelative(int _xa);
	int convertYToRelative(int _ya);
	int convertXToAbsolute(int _xr);
	int convertYToAbsolute(int _yr);
	void editablePointHasMoved(EditablePoint* _epRef);
	void moveALine(int xinit, int yinit, int deltaX, int deltaY, bool pure_vertical);
	void lineHasMoved();
	void refreshEditablePointsPositions();
	void holdAPoint(EditablePoint* _epRef);
	void reset();

	/*ChangeListener*/
	void changeListenerCallback(void* objectThatHasChanged);


	void clear();

private:
	AutomationCurve* m_concerned;
	String m_concernedName;
	String m_currentAddress;
	std::map<int/*xpc*/, EditablePoint*>* m_editablePoints;

	std::map<int, int> buffPoints;

	E_AutomationToolMode m_editionTool;

	int previous_x_line;
	int previous_y_line;
	EditablePoint* m_mvlepl;//m_movingLinePointLeft;
	EditablePoint* m_mvlepr;//m_movingLinePointRight;

	bool m_isMovingLine;
	bool m_isMovingPoint;

	//recouvrement à la Logic
	map<int, EditablePoint*> * m_epCoveredBuffer;
	int xpc_start_point_covering;

#ifdef VDBG
	String m_cout;
#endif
};

#endif
