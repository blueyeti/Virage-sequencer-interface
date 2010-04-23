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
#include "SequencePropertyPanel.h"
#include "Sequencer.h"
#include "MainComponent.h"

class DurationPropertyComponent : public TextPropertyComponent {
private:
	SequencePropertyPanel* m_panel;
public:
	DurationPropertyComponent(SequencePropertyPanel* _panel) : TextPropertyComponent(String("Duration"), 100, false) { m_panel = _panel;}
	~DurationPropertyComponent(){}

	void 	setText (const String &newText){
		m_panel->getConcerned()->setSize(m_panel->getConcerned()->getZoomer()->convertTimeToPix(newText.getFloatValue()), m_panel->getConcerned()->getHeight());
	}
	
 	const String 	getText () const {

 		String value = String((float)(m_panel->getConcerned()->getZoomer()->convertPixToTime(m_panel->getConcerned()->getWidth())) / 1000.);
 		int index = value.indexOf(T("."));

		if (index != -1 && value.length() >= index+3) {
			value = String(value.substring(0,index+3)+" s");
		}
		else {
			value = String(value + " s");
		}
		return value;
	}
};

class VisuStartPropertyComponent : public TextPropertyComponent {
private:
	SequencePropertyPanel* m_panel;
public:
	VisuStartPropertyComponent(SequencePropertyPanel* _panel) : TextPropertyComponent(String("Sight from..."), 100, false) { m_panel = _panel; };
	~VisuStartPropertyComponent(){};
  void 	setText (const String &/*newText*/){

	//	m_panel->getConcerned()->setViewedPositionX(m_panel->getConcerned()->getZoomer()->convertTimeToPix(newText.getIntValue()));
	//	((MainComponent*)m_panel->getParentComponent())->getSequencerFrame()->getViewPort()->setViewPosition(m_panel->getConcerned()->getZoomer()->convertTimeToPix(newText.getIntValue()), 0) ;
	}
 	const String 	getText () const {
	if(m_panel->getConcerned()==0)	return String::empty;
			String value = String((float)round(m_panel->getConcerned()->getZoomer()->convertPixToTime(m_panel->getConcerned()->getViewedPositionX()))/1000.);
			int i = value.indexOf(T("."));
			String valueStr;
			if (i != -1 && value.length() >= i+3) {
				valueStr = String(value.substring(0,i+3)+" s");
			}
			else {
				valueStr = String(value.substring(0)+" s");
			}
			return valueStr;
	}
};

class VisuEndPropertyComponent : public TextPropertyComponent {
private:
	SequencePropertyPanel* m_panel;
public:
	VisuEndPropertyComponent(SequencePropertyPanel* _panel) : TextPropertyComponent(String("Sight to..."), 100, false) { m_panel = _panel; };
	~VisuEndPropertyComponent(){};
  void 	setText (const String &/*newText*/){

		//	m_panel->getConcerned()->setSize(m_panel->getConcerned()->getZoomer()->convertTimeToPix(newText.getIntValue()), m_panel->getConcerned()->getHeight());
	}
 	const String 	getText () const {
		return String::empty;
		//	String value = String((float)round(m_panel->getConcerned()->getZoomer()->convertPixToTime(m_panel->getConcerned()->getWidth()))/1000);
		//	int i = value.indexOf(T("."));
		//	return String(value.substring(0,i+2)+" s");
	}
};



class ViewJacksPropComponent : public BooleanPropertyComponent {
private:
	SequencePropertyPanel* m_panel;
public:
	ViewJacksPropComponent(SequencePropertyPanel* _panel) : BooleanPropertyComponent(String("View Trigger Links"), String(""), String("")) { m_panel = _panel;}
	~ViewJacksPropComponent(){}

	void setState(const bool newState) {

		m_panel->getConcerned()->setJacksVisible(newState);
	}
	bool getState() const{
		if(m_panel->getConcerned() != 0) return m_panel->getConcerned()->areJacksVisible() ;
		return true;
	}
	void buttonClicked(Button *){
		setState( ! this->getState() );
		refresh();
	}
};

class MagnetPropComponent : public BooleanPropertyComponent {
private:
	SequencePropertyPanel* m_panel;
public:
	MagnetPropComponent(SequencePropertyPanel* _panel) : BooleanPropertyComponent(String("Magnetism"), String(""), String("")) { m_panel = _panel;}
	~MagnetPropComponent(){}

	void setState(const bool newState) {

		m_panel->getConcerned()->setMagnetised(newState);
	}
	bool getState() const{
		if(m_panel->getConcerned() != 0) return m_panel->getConcerned()->isMagnetised() ;
		return true;
	}
	void buttonClicked(Button *){
		setState(! this->getState() );
		refresh();
	}
};

//==============================================================================

//==============================================================================
SequencePropertyPanel::SequencePropertyPanel () {

	m_concerned = 0 ;

	setWantsKeyboardFocus(false);

	/* specify property list*/
	propertiesArray = new Array<PropertyComponent*>;
	propertiesArray->add(new DurationPropertyComponent(this));
	propertiesArray->add(new VisuStartPropertyComponent(this));
	propertiesArray->add(new VisuEndPropertyComponent(this));
	propertiesArray->add(new MagnetPropComponent(this));
	propertiesArray->add(new ViewJacksPropComponent(this));
	addProperties(*propertiesArray);
	setSize(400, 100);
	
}

SequencePropertyPanel::~SequencePropertyPanel(){
	deleteAllChildren();
}

void SequencePropertyPanel::setConcerned(Sequencer* _concerned){
	m_concerned  = _concerned;
}

Sequencer* SequencePropertyPanel::getConcerned(){
	return m_concerned;
}

void SequencePropertyPanel::changeListenerCallback (void */*objectThatHasChanged*/){
	refreshAll();
}


//==============================================================================
void SequencePropertyPanel::paint (Graphics& g) {

    g.fillAll (Colour (0xff272424));
	PropertyPanel::paint(g);
}

