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
#ifndef _TimeBoxContentComponent_H_
#define _TimeBoxContentComponent_H_

#include "includes.h"
#include "Anchor.h"
#include "BoxButton.h"

class Sequencer;
class TimeBox;
class AutomationCurve;

#ifndef _MESSAGEBUTTON_H_
#define _MESSAGEBUTTON_H_

class MessageButton : public TextButton, public DragAndDropTarget {
private:
	ComponentDragger* m_dragger;
	bool somethingIsBeingDraggedOver;
	Colour bgColour;
	bool m_isMuted, m_isFilled;
	
public:
	MessageButton(ComponentDragger* _dragger, String _toolTip = T("")) : TextButton(T("MessageButton"), _toolTip) {
		
		somethingIsBeingDraggedOver = false;
		m_dragger = _dragger;
		setWantsKeyboardFocus(false);
		bgColour = Colours::lightgrey;
		m_isMuted = false;
		m_isFilled = false;
	}
	
	~MessageButton(){}
	
	const Font getFont() {
	
		return s_GUIFX_FONT;
	}
	
	void setColour(Colour _colour) {
	
		bgColour =  _colour;
		repaint();
	}
	
	void paint(Graphics& g) {
	
		Colour pColour = bgColour;
		if(m_isFilled)								pColour = s_COLOUR_SELECTION;
		if(m_isMuted)								pColour = Colours::red;
		if(isDown())								pColour = pColour.brighter(0.5f);
		else if(getToggleState() || isMouseOver())	pColour = pColour.brighter(1.0f);
		
		g.fillAll(pColour);
		g.setColour(pColour.contrasting ());
		
		g.setColour( m_isFilled ? Colours::white.contrasting(1.0f) : pColour.contrasting(1.0f)  );
		g.drawText(getButtonText(), 0, 0, getWidth(), getHeight(),  Justification::centred,false);
		
		//filled ?
		if(m_isFilled) {
			
			g.setBrush( new GradientBrush (pColour, 0, 0, Colours::white, 0, getHeight()/2, false) );
			g.fillRect( 0,0,getWidth(),getHeight()/2 );
			g.setBrush( new GradientBrush (Colours::white, 0, getHeight()/2, pColour, 0, getHeight(), false) );
			g.fillRect( 0,getHeight()/2,getWidth(),getHeight()/2 );
		}
		
		//draganddrop
		if(somethingIsBeingDraggedOver) {
			g.fillAll(Colours::white.withAlpha(0.8f));
			g.setColour(s_COLOUR_SELECTION_GREEN);
			g.drawRect(0, 0, getWidth(), getHeight(), 2);
		}		
		
		//icone "</>"
	//	g.setColour(Colours::black);
	//	int padding=2;
	//	g.fillEllipse(padding, padding, getWidth()-2*padding, getWidth()-2*padding);
	}
	
	
	void setMuted(bool x) {
		m_isMuted = x;
		repaint();
	}
	
	void setFilled(bool x) {
		m_isFilled = x;
		repaint();
	}
	
	void mouseDown(const MouseEvent& e) {
	
		TextButton::mouseDown(e);
	}
	
	
	/**DragAndDropTarget*/
	bool isInterestedInDragSource  	( const String & sourceDescription, Component * /*sourceComponent*/){
		
		return verifyDraggedSource(sourceDescription);
	}
	
	void 	itemDropped (const String &/*sourceDescription*/, Component */* sourceComponent */, int /*x*/, int /*y*/){
		
		somethingIsBeingDraggedOver = false;
		repaint();
	}
	
	void 	itemDragEnter (const String &/*sourceDescription*/, Component */*sourceComponent*/, int /*x*/, int /*y*/){
		somethingIsBeingDraggedOver = true;
		repaint();
	}
	
	void 	itemDragExit (const String &/*sourceDescription*/, Component */*sourceComponent*/){
		somethingIsBeingDraggedOver = false;
		repaint();
	}
	
	bool verifyDraggedSource(const String & /*sourceDescription*/ ){
		
		return true;
	}
};
#endif


class TimeBoxContentComponent  : public Component, public ComponentDragger/*dragger pour les ME, MS*/, public ButtonListener
{
public:
    //==============================================================================
    TimeBoxContentComponent (Anchor* _containerL, Anchor* _containerR, TimeBox* _parent);
    ~TimeBoxContentComponent();

    //==============================================================================
    Anchor* getAnchorLeft();
	Anchor* getAnchorRight();
	AutomationCurve* getAutomationCurve();
	TimeBox* getTimeBoxParent();
	Sequencer* getParentSequencer();
	void init();
	void controlPointDone(unsigned int controlPointIndex);
	void setMessageStartButtonVisible(bool _visible);
	void setMessageEndButtonVisible(bool _visible);
	void setMutedContent(bool _m);
	/**ButtonLister*/
	void buttonClicked (Button* buttonThatWasClicked);	
	bool keyPressed	(const KeyPress & 	key);

	/**Component*/
	/*pour passage du click à la fenetre et mise en sélection*/
	void mouseDown(const MouseEvent &e);
	void mouseUp(const MouseEvent &e);
	void mouseDoubleClick(const MouseEvent &e);
	void paint (Graphics& g);
    void resized();
	
	void muteAll(bool _mute = true);
    //==============================================================================
    
private:
    //==============================================================================
	TimeBox* m_parent;
	
	MessageButton* messageStart;
	MessageButton* messageEnd;
	TextButton* muteStart;
	TextButton* muteEnd;
	BoxButton* automationStretch;
	BoxButton* relationLocker;
	BoxButton* automationEditor;
	Label* ledStart;
	Label* ledEnd;
	AutomationCurve* curveComponent;
	
	bool m_contentIsMuted;
	
	unsigned int m_first_CP_done;
	unsigned int m_last_CP_done;
	
	Anchor*  container1;
	Anchor*  container2;
};

#endif
