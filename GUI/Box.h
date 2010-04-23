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


 unsigned int addBox(int boxBeginPos, int boxLength, unsigned int motherId, int maxSceneWidth, int minBound, int maxBound);

 et

 void changeBoxBounds(unsigned int boxId, int minBound, int maxBound);


 */

#include "includes.h"
#include "Relation.h"
#include <vector>
#include "ResizableBox.h"
#include "BoxButton.h"


#ifndef _VirageButton_H_
#define _VirageButton_H_

class VirageButton : public TextButton {

private:
	Colour bgColour;
public:
	VirageButton(const String& buttonName, Colour buttonColour) : TextButton(buttonName) {

		setButtonText(buttonName);
		bgColour = buttonColour;
		setWantsKeyboardFocus(false);
	}
	~VirageButton(){}

	void paint(Graphics& g) {
		int padding =1;

		g.setColour(bgColour);
		g.fillRoundedRectangle(padding,padding,getWidth()-2*padding, getHeight()-2*padding, (getWidth()+getHeight())/2 * 0.2f );
		g.setColour(bgColour.contrasting ());
		g.drawText(getButtonText(), 0, 0, getWidth(), getHeight(),  Justification::centred,false);
	}

	void setColour(Colour _newColour) {
		bgColour = _newColour;
		repaint();
	}

	Colour getColour() {
		return bgColour;
	}
};

#endif



#ifndef _Box_H_
#define _Box_H_

/*
 *  Box.h
 *  Virage
 *
 *  Created by Henry Bernard and Luc Vercellin on 02/10/08.
 *  Copyright 2008 by Blue Yeti. All rights reserved.
 */

class WaitingLabel : public Label, public Timer {
public:
	WaitingLabel() : Label (T(""), T("wait" /*waiting*/)) {

		m_showText = true;
		startTimer(500);
		setJustificationType(Justification::centred);
	}

	~WaitingLabel(){}

	/*LABEL*/
	void resized(){

		//setFont(Font(String("Guifx v2 Transports Labeled"), getHeight()+2, Font::plain));
	}

	void paint(Graphics& g){

		g.fillAll(Colours::lightgrey);
		Label::paint(g);
	}

	/*Timer*/
	void timerCallback(){

		setText( m_showText ?  String("wait"/*waiting...*/) : String::empty, false);

		m_showText = ! m_showText;
	}

private:
	bool m_showText;
};




class Box : public TextEditorListener, public ButtonListener, public ResizableBox {
protected:
	bool deletionAsked;
	Colour m_titleColour;
	float m_titleAlpha;

	unsigned int m_starting_global_time;

	WaitingLabel* m_waiting;

	float m_duration;
	bool m_minimised;
	unsigned int m_originalBoxHeight, m_titleBarHeight, m_footerBarHeight, m_buttonBarHeight;
	int m_seqId;
	Colour* m_bgColour;

	VirageButton* m_expandArrowButton;
	//VirageButton* m_editAutomationButton;
	VirageButton* m_muteButton;
    VirageButton* m_commentButton;

	BoxButton* muteContent;

	BubbleMessageComponent* m_bubbleComment;
	String m_comment;
	bool m_isInterpolated;
	
	TextEditor* titleEditor;

public:
	Box(String etiquette, int _x, int _y, int _width, int _height);
	~Box();

	/*Box*/
	virtual void setOtColour(Colour _newColour);
	virtual void editAutomation() = 0;
	virtual bool isTriggered(unsigned int _ctrlPtId) = 0;
	//virtual void refresh() = 0;
	virtual void listenToMeSequencer() = 0;
	void expandButtonPressed();
	Colour getTitleColour();
	Colour getColour();
	float getTitleAlpha();
	virtual bool hitAnchor(int x, int y) =0;
	bool isDeletionAsked();
	void setTitleColour(Colour _titleColour, float _alpha = 1.0f);
	const BorderSize getBorderThickness() ;
	const BorderSize getContentComponentBorder();
	int getButtonBarHeight();
	void mouseDoubleClick(const MouseEvent &e);
	void mouseMove(const MouseEvent& e) ;

	bool hasBeenMoved;
	unsigned int m_previous_height; // utiliser avec boutton shift

	/*DocumentWindow*/
	void resized();			//overload DocumentWindow::resized()
	void buttonClicked (Button* buttonThatWasClicked);
	virtual void titleChanged(String _oldTitle) = 0;

	/*TextEditorListener*/
	void textEditorTextChanged (TextEditor& editor);
	virtual void textEditorReturnKeyPressed (TextEditor& editor);
	void textEditorEscapeKeyPressed (TextEditor& editor);
	virtual void textEditorFocusLost (TextEditor& editor);

	virtual void updateTriggers(String _s) = 0;
};

#endif
