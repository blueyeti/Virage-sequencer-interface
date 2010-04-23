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
 *  SafeComponentGetter.h
 *  Virage
 *
 *  Created by Henry on 02/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

/* NE PAS APPLIQUER A SEQUENCER.CLASS */

#ifndef _SafeComponentGetter_H_
#define _SafeComponentGetter_H_

#include "includes.h"
#include "TransportBar.h"
#include "BottomFrame.h"
#include "EditorFrame.h"
#include "AutomationScope.h"
#include "MiddleFrame.h"
#include "Sequencer.h"
#include "SequenceOtPropertyPanel.h"

class MainComponent;

class SafeComponentGetter {

private:
	MainComponent* m_mainComponent;
	
public:
	SafeComponentGetter(MainComponent* _higherParent);
	~SafeComponentGetter();
	
	//maincomponent
	MainComponent* getMainComponent();
	//Frames
	TransportBar* getTransportBar();
	
	
	MiddleFrame* getMiddleFrame();
		PropertyFrame* getPropertyFrame();		
			CommComponent* getCommPanel();
			SequenceOtPropertyPanel* getOtPropPanel();
			SequencePropertyPanel* getConduitePanel();
			NetComponent* getNetPanel();
		SequencerFrame* getSequencerFrame();
			Sequencer* getConduite();
	
	BottomFrame* getBottomFrame();
	EditorFrame* getEditorFrame();
	AutomationScope* getAutomationScope();
};

#endif
