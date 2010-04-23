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
 *  OtManager.h
 *  Virage
 *
 *  Created by Henry on 25/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OTMANAGER_H_
#define _OTMANAGER_H_

#include "includes.h"
class Sequencer;
#include "TimeBox.h"
#include "Relation.h"
#include "Trigger.h"


class OtManager {

private:
	Sequencer* m_parent;
	int m_box_counter;

	friend class Sequencer;
	
public:
	OtManager(Sequencer* m_parent);
	virtual ~OtManager();
	
	
	Relation* addRelation(unsigned int _MotherId, unsigned int _DaughterId, unsigned int mother_ctrlPt, unsigned int daughter_ctrlPt, unsigned int pre_index = 0, bool isFlexible = false);
	Trigger* addTrigger(int x, Colour _couleur = s_COLOUR_BOX, TimeBox* controlledBox = 0, bool anchoringStart = false, unsigned int pre_index = 0, bool loadedTrig = false);	
	TimeBox* addBox(String etiquette = "default", bool _isFlexible = false, int _x = 0, int _y = 0, int _width = 100, int _height = 50, unsigned int previousId=0, Component* _seqFrame = 0);
	
	
	int getBoxCounter();
	void setBoxCounter( int _counter);
	
	void removeAllBox();
	void removeAllRelation();
	void removeAllTrigger();
	
	TimeBox* duplicateBox(TimeBox* copiedbox);
		
	void removeBox(unsigned int id);
	void removeRelation(unsigned int id);
	void removeTrigger(unsigned int id);
	
	bool findBox(int boxId);
};

#endif
