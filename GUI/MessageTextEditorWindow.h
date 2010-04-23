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
 *  MessageTextEditor.h
 *  Virage
 *
 *  Created by Laurent Garnier on 12/03/09.
 *  Copyright 2009 __BlueYeti__. All rights reserved.
 *
 */

#ifndef _MESSAGETEXTEDITORWINDOW_H_
#define _MESSAGETEXTEDITORWINDOW_H_

#include "includes.h"
#include "TextEditorContentComponent.h"

class TimeBox;

class MessageTextEditorWindow : public DocumentWindow, public FileBasedDocument {
	
private:
	TimeBox* m_concerned;
	int m_controlPointIndex;
	TextEditorContentComponent* m_contentComponent;
	File lastDocumentOpened;
	
	
public:
	MessageTextEditorWindow(TimeBox* _concerned, int _controlPointIndex);
	~MessageTextEditorWindow();	
	
	void updateOscMessages(String _messages);
	void setOscMessages(String _messages);
	void setTitle(String _title);
	void setTitleColour(Colour _colour);
	void loadSnapshot();
	void saveSnapshot(String _snapshot);
	TimeBox* getConcerned();
	TextEditor*	getTextEditor();
	
	/*JUCE*/void closeButtonPressed();
	
	//NE PAS CHANGER !!! OVERWRITE de fileBasedDocument
	/*JUCE*/const String loadDocument(const File & file);
	/*JUCE*/const String saveDocument(const File & file);
	/*JUCE*/const String getDocumentTitle();
	/*JUCE*/const File getLastDocumentOpened();
	/*JUCE*/void setLastDocumentOpened(const File & file);
	
	bool m_isTextEditorModified;
	
}; 

#endif
