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
 *  SafeComponentGetter.cpp
 *  Virage
 *
 *  Created by Henry on 02/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "SafeComponentGetter.h"
#include "MainComponent.h"

SafeComponentGetter:: SafeComponentGetter(MainComponent* _highestComponent) : m_mainComponent(_highestComponent){/*actually always use mainComponent in mainWindow, window is not considered as a component here*/
}

//Frame
TransportBar* SafeComponentGetter::getTransportBar() {

	return m_mainComponent->m_transportBar;
}

//BottomFrame* SafeComponentGetter::getBottomFrame(){return m_mainComponent>m_bottomFrame;}

EditorFrame* SafeComponentGetter::getEditorFrame(){
	return m_mainComponent->m_bottomFrame->m_editorFrame;
}

AutomationScope* SafeComponentGetter::getAutomationScope(){
	return m_mainComponent->m_bottomFrame->m_editorFrame->m_editor;
}

MiddleFrame* SafeComponentGetter::getMiddleFrame(){
	return m_mainComponent->m_middleFrame;
}

PropertyFrame* SafeComponentGetter::getPropertyFrame(){
	return m_mainComponent->m_middleFrame->m_propertyFrame;
}

SequencerFrame* SafeComponentGetter::getSequencerFrame(){
	return m_mainComponent->m_middleFrame->m_sequenceFrame;
}

Sequencer* SafeComponentGetter::getConduite(){
	return m_mainComponent->m_middleFrame->m_sequenceFrame->getConduite();
}

CommComponent* SafeComponentGetter::getCommPanel(){
	return m_mainComponent->m_middleFrame->m_commPanel;
}

SequenceOtPropertyPanel* SafeComponentGetter::getOtPropPanel(){
	return m_mainComponent->m_middleFrame->m_otPanel;
}

SequencePropertyPanel* SafeComponentGetter::getConduitePanel(){
	return m_mainComponent->m_middleFrame->m_sequencePanel;
}

NetComponent* SafeComponentGetter::getNetPanel(){
	return m_mainComponent->m_middleFrame->m_namespacePanel;
}

BottomFrame* SafeComponentGetter::getBottomFrame(){
	return m_mainComponent->m_bottomFrame;
}