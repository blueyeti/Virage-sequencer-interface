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
 *  SequenceOT.h
 *  Virage
 *
 * Sequence Item = objet temporel = boite, trigger, relation
 *
 *
 *  Created by Henry Bernard BERNARD on 06/04/09.
 *  Copyright 2009 by Blue Yeti. All rights reserved.
 *
 */
#ifndef _SEQUENCEOT_H_
#define _SEQUENCEOT_H_

#include "includes.h"
#include "Behavior.h"

class Sequencer;

class SequenceOT  : public ChangeBroadcaster, public Component /*propertypanel*/{
	
protected:
	unsigned int m_id;
	Sequencer* m_parentSequencer;
	bool m_muted;
	
public:
	SequenceOT(Sequencer* _parent, unsigned int _id);
	~SequenceOT();
	
	unsigned int getId();
	
	void setId(unsigned int _id);

	Sequencer* getParentSequencer();

	virtual bool isTriggered(unsigned int _ctrlPtId) = 0;
	virtual void refresh() = 0;
	virtual void listenToMeSequencer() = 0;
	
	virtual	void setMuted(bool _m) = 0;
	virtual bool isMuted() = 0;
	
	virtual void setOtColour(Colour _newColour) = 0;
	
	virtual Behavior* getDurationBehavior() = 0; 
	
	virtual Colour getColour() = 0;
	
	void focusGained(FocusChangeType cause);
	
	
	
	bool keyPressed(const KeyPress & key);
		
};

#endif