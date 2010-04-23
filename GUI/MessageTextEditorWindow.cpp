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
 *  MessageTextEditorWindow.cpp
 *  Virage
 *
 *  Created by Laurent Garnier on 13/03/09.
 *  Copyright 2009 BlueYeti.Fr. All rights reserved.
 *
 */

#include "MessageTextEditorWindow.h"
#include "TimeBox.h"
#include <iostream>
#include <fstream>
#include <string>
#include "CSPTypes.h"

MessageTextEditorWindow::MessageTextEditorWindow(TimeBox* _concerned, int _controlPointIndex) : DocumentWindow ("", s_COLOUR_BOX, DocumentWindow::closeButton | DocumentWindow::minimiseButton, true),
																								FileBasedDocument(String(".xml"), String("*.xml"), String("Choose a Snapshot to open..."), String("Choose a Snapshot to save as..."))
{
	m_concerned = _concerned;
	m_controlPointIndex = _controlPointIndex;

	m_contentComponent = new TextEditorContentComponent();
	setContentComponent(m_contentComponent, true, false);

	setVisible(false);
	setSize(350, 250);

	setTopLeftPosition(200, 200);
	setResizable(true,true);
	setName(_concerned->getName() + ((_controlPointIndex == 1)? " Start Messages":" End Messages"));
	setTitleBarTextCentred (true);

	setAlwaysOnTop(true);

	lastDocumentOpened = File::nonexistent;
}

MessageTextEditorWindow::~MessageTextEditorWindow(){
	setContentComponent(0, true);
}

void MessageTextEditorWindow::updateOscMessages(String _messages){
	m_contentComponent->updateOscMessages(_messages);
}

void MessageTextEditorWindow::setOscMessages(String _messages){
	if(m_controlPointIndex == 1)
		m_concerned->setOscMessageOutStart(_messages);
	else
		m_concerned->setOscMessageOutEnd(_messages);
}

void MessageTextEditorWindow::setTitle(String _title){
	setName(_title + ((m_controlPointIndex == BEGIN_CONTROL_POINT_INDEX)? " Start Messages":" End Messages"));
	//setTitle(_title + ((m_controlPointIndex == BEGIN_CONTROL_POINT_INDEX)? " Start Messages":" End Messages"));
}

void MessageTextEditorWindow::setTitleColour(Colour _colour){
	setBackgroundColour(_colour);
}

void MessageTextEditorWindow::loadSnapshot(){
	loadFromUserSpecifiedFile(true);
}

void MessageTextEditorWindow::saveSnapshot(String /*snapshot*/) {
//	save(true, true);

	File newFile;
	saveAs(newFile, true, true, true);
}

TimeBox* MessageTextEditorWindow::getConcerned(){
	return m_concerned;
}

void MessageTextEditorWindow::closeButtonPressed(){
	if (m_isTextEditorModified) {
		bool userPickedOk = AlertWindow::showOkCancelBox (AlertWindow::QuestionIcon,
										T("Messages have been modified"),
										T("Would you assign ?"));
		if (userPickedOk) {
			setOscMessages(m_contentComponent->getText());
			m_isTextEditorModified = false;
		}
	}
	setVisible(false);
}

/*************************** FileBasedDocument overwritted-must methods *******************/
const String MessageTextEditorWindow::loadDocument(const File & file) {
	File _xml = file;
	/********* XML PARSER *********/
	File* openedFile = new File(_xml);

	if (openedFile->getFileExtension() != String(".xml")) {
		return String();
	}

	XmlDocument openedDocument(*openedFile);

    XmlElement* projectElement = openedDocument.getDocumentElement();
	if (projectElement == 0)
    {
        String error = openedDocument.getLastParseError();
		return error;
    }
    else
    {
		// check we're looking at the right kind of document..
		if(projectElement->hasTagName(T("VIRAGE")))
		{
			forEachXmlChildElement (*projectElement, e)
			{

				if (e->hasTagName (T("BOXPARK")))
				{
					// found a BX park
					XmlElement* boxParkElement = projectElement->getChildByName(T("BOXPARK"));

					forEachXmlChildElement (*boxParkElement, f)
					{
						if (f->hasTagName (T("BOX")))
						{
							//GUI
							String snapshot  = f->getStringAttribute (T("snapshot"));
							m_contentComponent->setText(snapshot);
						}
					}
				}
			}
		}
    }

	if (projectElement) {
	  deleteAndZero( projectElement);
	}
	if (openedFile) {
	  deleteAndZero( openedFile);
	}

	return String::empty;

}

const String MessageTextEditorWindow::saveDocument(const File & file) {
	File _xml = file;

	/********* XML PARSER *********/
	File* savedFile;

	if(_xml.getFileExtension() == String(".xml")) {
		savedFile = new File(_xml.getFullPathName());
	}else {
		savedFile = new File(_xml.getFullPathName() + String(".xml"));
	};

	if (savedFile->getFileExtension() != String(".xml")) {
		return String();
	}

	if (savedFile == 0)
    {
		return String("Error, impossible to save.");
    }
    else
    {

		//<VIRAGE>
		XmlElement projet ("VIRAGE");
		projet.setAttribute(T("version"), T("0.2"));

			//<BOXPARK>
			XmlElement* boxPark = new XmlElement(T("BOXPARK"));
			projet.addChildElement(boxPark);
				//<BOX>

					// create an inner element...
					XmlElement* box = new XmlElement ("BOX");
					box->setAttribute (T("snapshot"), m_contentComponent->getText());

					//...and add our new element to the parent node
					boxPark->addChildElement (box);

				//</BOX>
			//</BOXPARK>
		//</VIRAGE>

		// now we can turn the whole thing into a text document..
		const String savedDocument = projet.createDocument (String::empty);

		std::ofstream myXmlDocument(savedFile->getFullPathName().toUTF8(), ios::trunc);
		myXmlDocument <<  (std::string)savedDocument;
		myXmlDocument.close();
    }

	if (savedFile) {
	  deleteAndZero( savedFile);
	}


	return String::empty;

}

TextEditor* MessageTextEditorWindow::getTextEditor(){
	return m_contentComponent->getTextEditor();
}

const String MessageTextEditorWindow::getDocumentTitle(){
	if(getFile().exists()) {
		return getFile().getFileName();
	}
	else {
		return s_DEFAULT_MESSAGELIST_NAME;
	};

}

void MessageTextEditorWindow::setLastDocumentOpened(const File & file){
	lastDocumentOpened = file;
}

const File MessageTextEditorWindow::getLastDocumentOpened(){
	return lastDocumentOpened;
}
/*************************** end FileBasedDocument overwritted-must methods *******************/
