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
#include "SequenceOtPropertyPanel.h"
#include "Sequencer.h"
#include "TimeBox.h"
#include "Trigger.h"
#include "Relation.h"
#include "PropertyFrame.h"
#include "MainWindow.h"

class TitlePropertyComponent : public TextPropertyComponent {
private:
	SequenceOtPropertyPanel* m_panel;
public:
	TitlePropertyComponent(SequenceOtPropertyPanel* _panel) : TextPropertyComponent(String("Title"), 100, false) {
		m_panel = _panel;
	}
	
	~TitlePropertyComponent(){}
	
	void 	setText (const String &newText) {
		if(m_panel->getConcerned() == 0 || dynamic_cast<Trigger*> (m_panel->getConcerned()) || dynamic_cast<Relation*> (m_panel->getConcerned()))
			return;
		if( !newText.isEmpty() && dynamic_cast<TimeBox*> (m_panel->getConcerned())) {
			String titleWithoutSpace = newText.replace(String(" "), String("_"));
			
			((TimeBox*)(m_panel->getConcerned()))->setTitleText(getText(), titleWithoutSpace);
		}
	}
	
 	const String 	getText () const {
		if(m_panel->getConcerned() == 0 || dynamic_cast<Relation*> (m_panel->getConcerned()) || dynamic_cast<Trigger*> (m_panel->getConcerned()))
		{
			((Component*)this)->setVisible(false);
			return String("(disabled)");
		}
		
		((Component*)this)->setVisible(true);
		return m_panel->getConcerned()->getName();
	}
	
	void refresh() {
		/*refresh*/setVisible((m_panel->getConcerned() != 0) && dynamic_cast<TimeBox*> (m_panel->getConcerned()));
		
		TextPropertyComponent::refresh();
		
	}
};

class StartPropertyComponent : public TextPropertyComponent {
private:
	SequenceOtPropertyPanel* m_panel;
	
public:
	StartPropertyComponent(SequenceOtPropertyPanel* _panel) : TextPropertyComponent(String("Position (s)"), 100, false) { m_panel = _panel; }
	~StartPropertyComponent() {}
	
	void 	setText (const String &newText) {
		if(m_panel->getConcerned() == 0 || dynamic_cast<Relation*> (m_panel->getConcerned()) || dynamic_cast<Trigger*> (m_panel->getConcerned())) return;
		
		int boxId = m_panel->getConcerned()->getId();
		
		vector<unsigned int> movedBoxes;
		m_panel->getConcerned()->getParentSequencer()->getEngines()->performBoxEditing(boxId, newText.getFloatValue() * 1000,
																					   newText.getFloatValue() * 1000 + m_panel->getConcerned()->getParentSequencer()->getEngines()->getBoxDuration(boxId), &movedBoxes);
		m_panel->getConcerned()->getParentSequencer()->isResizingByMouseOT = false;
		m_panel->getConcerned()->setTopLeftPosition((m_panel->getConcerned()->getParentSequencer()->getZoomer()->convertTimeToPix((newText.getFloatValue()) * 1000)), m_panel->getConcerned()->getY());
	}
	
 	const String 	getText () const {
		
		if(m_panel->getConcerned() == 0 || dynamic_cast<Relation*> (m_panel->getConcerned()) || dynamic_cast<Trigger*> (m_panel->getConcerned())) {
			return String("(disabled)");
		}
		
		((Component*)this)->setVisible(true);
		
		if(dynamic_cast<TimeBox*> (m_panel->getConcerned())) {
			
			int boxId = m_panel->getConcerned()->getId();
			if (m_panel->getConcerned()->getParentSequencer()->getOtManager()->findBox(boxId)) {
				
				String value = String((float)(m_panel->getConcerned()->getParentSequencer()->getEngines()->getBoxBeginTime(boxId)) / 1000.); 
				
				int i = value.indexOf(T("."));
				if (i != -1 && (i+3) < value.length()) {
					return String(value.substring(0,i+3));
				}
				else if (value.length() > 0) {
					return String(value);
				}
				else {
					return String("--Position Display Error--");
				}
			}
		}
		
		((Component*)this)->setVisible(false);
		return String("(disabled)");
	}
	
	void refresh() {
		
		/*refresh*/setVisible((m_panel->getConcerned() != 0) && dynamic_cast<TimeBox*> (m_panel->getConcerned()));
		
		TextPropertyComponent::refresh();
		
	}
};

class DurationPropertyComponent : public TextPropertyComponent {
private:
	SequenceOtPropertyPanel* m_panel;
public:
	DurationPropertyComponent(SequenceOtPropertyPanel* _panel) : TextPropertyComponent(String("Duration (s)"), 100, false) { m_panel = _panel; };
	~DurationPropertyComponent(){};
	
	void 	setText (const String &newText){
		
		if(m_panel->getConcerned() == 0 || dynamic_cast<Trigger*> (m_panel->getConcerned())) return;
		
		if(dynamic_cast<TimeBox*> (m_panel->getConcerned())){
			int newWidth =  (m_panel->getConcerned()->getParentSequencer()->getZoomer()->convertTimeToPix(newText.getFloatValue() * 1000));
			int boxId = m_panel->getConcerned()->getId();
			vector<unsigned int> movedBoxes;
			
			m_panel->getConcerned()->getParentSequencer()->getEngines()->performBoxEditing(boxId, m_panel->getConcerned()->getParentSequencer()->getEngines()->getBoxBeginTime(boxId),
																						   m_panel->getConcerned()->getParentSequencer()->getEngines()->getBoxBeginTime(boxId) + newText.getFloatValue() * 1000, &movedBoxes);
			
			
			
			m_panel->getConcerned()->getParentSequencer()->isResizingByMouseOT = false;
			m_panel->getConcerned()->setSize(newWidth, m_panel->getConcerned()->getHeight());
			
		}
		else if(dynamic_cast<Relation*> (m_panel->getConcerned())){
			Relation* concerned = (Relation*)(m_panel->getConcerned());
			unsigned int idBox1 = concerned->getMotherId();
			unsigned int idBox2 = concerned->getDaughterId();
			unsigned int xBox1 = concerned->getParentSequencer()->getBox(idBox1)->getX();
			unsigned int xBox2 = concerned->getParentSequencer()->getBox(idBox2)->getX();
			unsigned int idToMove = (xBox1 < xBox2) ? idBox2 : idBox1;
			unsigned int idToStay = (xBox1 > xBox2) ? idBox2 : idBox1;
			vector<unsigned int> movedBoxes;
			
			m_panel->getConcerned()->getParentSequencer()->getEngines()->performBoxEditing(idToMove, m_panel->getConcerned()->getParentSequencer()->getEngines()->getBoxEndTime(idToStay) + newText.getFloatValue() * 1000,
																						   m_panel->getConcerned()->getParentSequencer()->getEngines()->getBoxEndTime(idToStay) + newText.getFloatValue() * 1000
																						   +  m_panel->getConcerned()->getParentSequencer()->getEngines()->getBoxDuration(idToMove), &movedBoxes);
			
			m_panel->getConcerned()->getParentSequencer()->isResizingByMouseOT = false;
			m_panel->getConcerned()->getParentSequencer()->getBox(idToMove)->setTopLeftPosition(abs(std::min(concerned->getZoneX(), concerned->getZoneRight())
																									+ concerned->getParentSequencer()->getZoomer()->convertTimeToPix(newText.getFloatValue() * 1000.)) + 2,//+2 pixels for adjustment
																								concerned->getParentSequencer()->getBox(idToMove)->getY());
		}
	}
	
 	const String 	getText () const {
		
		if(m_panel->getConcerned() == 0 || dynamic_cast<Trigger*> (m_panel->getConcerned())) {
			
			return String("(disabled)");
		}
		
		((Component*)this)->setVisible(true);
		
		if(dynamic_cast<TimeBox*> (m_panel->getConcerned())) {
			int boxId = m_panel->getConcerned()->getId();
			
			if (m_panel->getConcerned()->getParentSequencer()->getOtManager()->findBox(boxId)) {
				
				String value = String( (m_panel->getConcerned()->getParentSequencer()->getEngines()->getBoxEndTime(boxId) -
										m_panel->getConcerned()->getParentSequencer()->getEngines()->getBoxBeginTime(boxId)) /1000.);
				//				String value = String( (m_panel->getConcerned()->getParentSequencer()->getEngines()->getBoxDuration(boxId)) /1000.);
				
				
				int i = value.indexOf(T("."));
				if (i != -1 && (i+3) < value.length()) {
					return String(value.substring(0,i+3));
				}
				else if (value.length() > 0) {
					return String(value);
				}
				else {
					return String("--Duration Display Error--");
				}
			}
		}
		else if(dynamic_cast<Relation*> (m_panel->getConcerned())) {
			Relation* concerned = (Relation*)(m_panel->getConcerned());
			
			String value =   String(concerned->getDuration() / 1000.);
			
			int i = value.indexOf(T("."));
			value = value.substring(0,i+3);
			
			return String(value);
		}
		
		//else
		((Component*)this)->setVisible(false);
		return String("(disabled)");
		
	}
	
	void refresh(){
		
		/*refresh*/setVisible((m_panel->getConcerned() != 0) && dynamic_cast<TimeBox*> (m_panel->getConcerned()));
		
		TextPropertyComponent::refresh();
		
	}
};

class MessageOutFromStartPropertyComponent : public ButtonPropertyComponent {
private:
	SequenceOtPropertyPanel* m_panel;
public:
	MessageOutFromStartPropertyComponent(SequenceOtPropertyPanel* _panel) : ButtonPropertyComponent(String("Start Out Messages"), true) { m_panel = _panel;}
	~MessageOutFromStartPropertyComponent(){};
	void buttonClicked(){
		
		if(!dynamic_cast<TimeBox*> (m_panel->getConcerned())) return;
		((TimeBox*)(m_panel->getConcerned()))->showMessagesEditor(true/*start*/);
	}
	const String 	getButtonText () const {
		return "Edit";
	}
	void refresh(){
		ButtonPropertyComponent::refresh();
		/*refresh*/setVisible((m_panel->getConcerned() != 0) && dynamic_cast<TimeBox*> (m_panel->getConcerned()));
	}
};

class MessageOutFromEndPropertyComponent : public ButtonPropertyComponent {
private:
	SequenceOtPropertyPanel* m_panel;
public:
	MessageOutFromEndPropertyComponent(SequenceOtPropertyPanel* _panel) : ButtonPropertyComponent(String("End Out Messages"), true) {m_panel = _panel;}
	~MessageOutFromEndPropertyComponent(){};
	void buttonClicked(){
		
		if(!dynamic_cast<TimeBox*> (m_panel->getConcerned())) return;
		((TimeBox*)(m_panel->getConcerned()))->showMessagesEditor(false/*end*/);
	}
 	const String 	getButtonText () const {
		return "Edit";
	}
	void refresh(){
		ButtonPropertyComponent::refresh();
		/*refresh*/setVisible((m_panel->getConcerned() != 0) && dynamic_cast<TimeBox*> (m_panel->getConcerned()));
	}
};

class MutedPopertyComponent : public BooleanPropertyComponent {
private:
	SequenceOtPropertyPanel* m_panel;
public:
	MutedPopertyComponent(SequenceOtPropertyPanel* _panel) : BooleanPropertyComponent(String("Mute"), String(""), String("")) { m_panel = _panel;};
	~MutedPopertyComponent(){};
	void setState(const bool newState) {
		if(m_panel->getConcerned() == 0 ) return;
		m_panel->getConcerned()->setMuted(newState);
	}
	bool getState() const{
		if(m_panel->getConcerned() == 0) return false;
		return m_panel->getConcerned()->isMuted();
	}
	void buttonClicked(Button *){
		setState( ! this->getState() == true);
		refresh();
	}
	void refresh(){
		/*refresh*/setVisible((m_panel->getConcerned() != 0) && !dynamic_cast<Relation*> (m_panel->getConcerned()));
		BooleanPropertyComponent::refresh();
		
	}
};

class InterpolationStatePropertyComponent : public BooleanPropertyComponent {
private:
	SequenceOtPropertyPanel* m_panel;
public:
	InterpolationStatePropertyComponent(SequenceOtPropertyPanel* _panel) : BooleanPropertyComponent(String("Interpolation"), String(""), String("")) { m_panel = _panel; };
	~InterpolationStatePropertyComponent(){};
	void setState(bool newState){
		if(!dynamic_cast<TimeBox*> (m_panel->getConcerned()))
			return;
		((TimeBox*)(m_panel->getConcerned()))->setInterpolated(newState);
	}
	bool getState() const {
		if(!dynamic_cast<TimeBox*> (m_panel->getConcerned())) return false;
		return ((TimeBox*)(m_panel->getConcerned()))->isInterpolated();
	}
	void buttonClicked(Button *){
		setState(!(this->getState() == true));
		refresh();
	}
	void refresh(){
		/*refresh*/setVisible((m_panel->getConcerned() != 0) && dynamic_cast<TimeBox*> (m_panel->getConcerned()));
		BooleanPropertyComponent::refresh();
	}
};

class InterpolationEditingPropertyComponent : public ButtonPropertyComponent {
public:
	InterpolationEditingPropertyComponent(SequenceOtPropertyPanel* _panel) : ButtonPropertyComponent(String("Interpolation"), true){ m_panel = _panel;}
	
	~InterpolationEditingPropertyComponent(){};
	
	void buttonClicked(){
		if(!dynamic_cast<TimeBox*> (m_panel->getConcerned())) return;
		((TimeBox*)(m_panel->getConcerned()))->showInterpolationWindow();
	}
	
	const String 	getButtonText () const {
		return "Edit";
	}
	
	void refresh(){
		/*refresh*/setVisible((m_panel->getConcerned() != 0) && dynamic_cast<TimeBox*> (m_panel->getConcerned()));
		ButtonPropertyComponent::refresh();
	}
	
private:
	SequenceOtPropertyPanel* m_panel;
};

class FlexibilityPropertyComponent : public BooleanPropertyComponent {
private:
	SequenceOtPropertyPanel* m_panel;
public:
	FlexibilityPropertyComponent(SequenceOtPropertyPanel* _panel) : BooleanPropertyComponent(String("Flexible"), String(""), String("")) { 
		m_panel = _panel; 
		setEnabled(false);
	};
	~FlexibilityPropertyComponent(){};
	void setState(bool newState){
		if(m_panel->getConcerned() == 0  || dynamic_cast<Trigger*> (m_panel->getConcerned())) return;
		m_panel->getConcerned()->getDurationBehavior()->setFlexible(newState);
		m_panel->getConcerned()->getDurationBehavior()->repaint();
		m_panel->getConcerned()->resized();
		
	}
	
	bool getState() const {
		if(m_panel->getConcerned() == 0 || dynamic_cast<Trigger*> (m_panel->getConcerned())) return false;
		return m_panel->getConcerned()->getDurationBehavior()->isFlexible();
	}
	
	void buttonClicked(Button *){
		setState( this->getState() != true);
		refresh();
	}
	
	void refresh(){
		/*refresh*/setVisible((m_panel->getConcerned() != 0) && !dynamic_cast<Trigger*> (m_panel->getConcerned()));//uniquement trigger invisible
		BooleanPropertyComponent::refresh();
		
	}
};

class FlexLeftPropertyComponent : public TextPropertyComponent {
private:
	SequenceOtPropertyPanel* m_panel;
public:
	FlexLeftPropertyComponent(SequenceOtPropertyPanel* _panel) : TextPropertyComponent(String("Flex : Left boundary (s)"), 150, false){ m_panel = _panel; };
	~FlexLeftPropertyComponent(){};
	void 	setText (const String &newText){
		if(m_panel->getConcerned() == 0 || dynamic_cast<Trigger*> (m_panel->getConcerned())) return;
		
		m_panel->getConcerned()->getDurationBehavior()->setFlexStart((unsigned int)newText.getFloatValue()*1000);
		m_panel->checkFlexCoherence();
		m_panel->getConcerned()->getDurationBehavior()->repaint();
		m_panel->getConcerned()->resized();
	}
	
 	const String 	getText () const {
		
		((Component*)this)->setVisible(false);
		if(m_panel->getConcerned() == 0 || dynamic_cast<Trigger*> (m_panel->getConcerned())) return String("(disabled)");
		
		((Component*)this)->setVisible(true);
		return String(String( (float) ( m_panel->getConcerned()->getDurationBehavior()->getFlexStart()   ) /1000.).substring(0,4));
	}
	
	void refresh(){
		/*refresh*/setVisible((m_panel->getConcerned() != 0) && !dynamic_cast<Trigger*> (m_panel->getConcerned()));
		
		TextPropertyComponent::refresh();
		
	}
};

class FlexRightPropertyComponent : public TextPropertyComponent {
private:
	SequenceOtPropertyPanel* m_panel;
public:
	FlexRightPropertyComponent(SequenceOtPropertyPanel* _panel) : TextPropertyComponent(String("Flex : Right boundary (s)"), 150, false) { m_panel = _panel; };
	~FlexRightPropertyComponent(){};
	void 	setText (const String &newText){
		
		if( (m_panel->getConcerned() == 0) || dynamic_cast<Trigger*> (m_panel->getConcerned()) ) return;
		m_panel->getConcerned()->getDurationBehavior()->setFlexStop((unsigned int)newText.getFloatValue()*1000);
		m_panel->checkFlexCoherence();
		m_panel->getConcerned()->getDurationBehavior()->repaint();
		m_panel->getConcerned()->resized();
	}
 	const String 	getText () const {
		
		((Component*)this)->setVisible(false);
		if(m_panel->getConcerned() == 0 || dynamic_cast<Trigger*> (m_panel->getConcerned())) return String("(disabled)");
		
		((Component*)this)->setVisible(true);
		
		int stop  = m_panel->getConcerned()->getDurationBehavior()->getFlexStop();
		if(stop == NO_STOP)
			return String("inf.");
		else
			return String(String( (float)(  stop   ) /1000. ).substring(0,4));
	}
	void refresh(){
		/*refresh*/setVisible((m_panel->getConcerned() != 0) && !dynamic_cast<Trigger*> (m_panel->getConcerned()));
		
		TextPropertyComponent::refresh();
	}
};

class CommentPropertyComponent : public TextPropertyComponent {
private:
	SequenceOtPropertyPanel* m_panel;
public:
	CommentPropertyComponent(SequenceOtPropertyPanel* _panel) : TextPropertyComponent(String("Comment"), 150, true) { m_panel = _panel; };
	~CommentPropertyComponent(){};
	void 	setText (const String &newText){
		if(!dynamic_cast<TimeBox*> (m_panel->getConcerned())) return;
		((TimeBox*)(m_panel->getConcerned()))->setComment(newText);
	}
 	const String 	getText () const {
		
		((Component*)this)->setVisible(false);
		if(!dynamic_cast<TimeBox*> (m_panel->getConcerned())) return String("(disabled)");
		
		((Component*)this)->setVisible(true);
		return ((TimeBox*)(m_panel->getConcerned()))->getComment();
	}
	void refresh(){
		/*refresh*/setVisible((m_panel->getConcerned() != 0) && dynamic_cast<TimeBox*> (m_panel->getConcerned()));
		
		TextPropertyComponent::refresh();
	}
};

class ColourPropertyComponent : public PropertyComponent, public ChangeListener {
public:
	ColourPropertyComponent(SequenceOtPropertyPanel* _panel) : PropertyComponent(String("Colour"), 100) {
		m_panel = _panel;
		m_colourSelector = new ColourSelector(ColourSelector::showColourspace);
		m_colourSelector->setCurrentColour(s_COLOUR_UNSELECTION);
		m_colourSelector->addChangeListener(this);
		addAndMakeVisible (m_colourSelector);
	}
	
	~ColourPropertyComponent() {
		deleteAllChildren();
	}
	
	Colour getColour() {
		if(m_panel->getConcerned() == 0 || dynamic_cast<Relation*> (m_panel->getConcerned())) return Colours::white;
		return m_panel->getConcerned()->getColour();
	}
	
	void setColour(Colour newColour) {
		if(m_panel->getConcerned() == 0 || dynamic_cast<Relation*> (m_panel->getConcerned())) return;
		(m_panel->getConcerned())->setOtColour(newColour);
		m_panel->getConcerned()->repaint();
		if(dynamic_cast<TimeBox*> (m_panel->getConcerned()) ) ((TimeBox*)(m_panel->getConcerned()))->setMessagesEditorColour(newColour);
	}
	
	void refresh() {
		/*refresh*/setVisible((m_panel->getConcerned() != 0) && dynamic_cast<TimeBox*> (m_panel->getConcerned()));
		
		m_colourSelector->setCurrentColour(getColour());
	}
	
	void changeListenerCallback(void* /*source*/) {
		setColour(m_colourSelector->getCurrentColour());
	}
	
private:
	SequenceOtPropertyPanel* m_panel;
	ColourSelector* m_colourSelector;
};

//==============================================================================

//==============================================================================
SequenceOtPropertyPanel::SequenceOtPropertyPanel () {
	
	m_concerned = 0;
	
	setWantsKeyboardFocus(false);
	
	/* specify property list*/
	propertiesArray = new Array<PropertyComponent*>;
	
	propertiesArray->add(new TitlePropertyComponent(this));
	propertiesArray->add(new StartPropertyComponent(this));
	propertiesArray->add(new DurationPropertyComponent(this));
	propertiesArray->add(new MutedPopertyComponent(this));
	propertiesArray->add(new MessageOutFromStartPropertyComponent(this));
	propertiesArray->add(new MessageOutFromEndPropertyComponent(this));
	
	propertiesArray->add(new InterpolationStatePropertyComponent(this));
	propertiesArray->add(new InterpolationEditingPropertyComponent(this));
	propertiesArray->add(new FlexibilityPropertyComponent(this));
	propertiesArray->add(new FlexLeftPropertyComponent(this));
	propertiesArray->add(new FlexRightPropertyComponent(this));
	propertiesArray->add(new CommentPropertyComponent(this));
	propertiesArray->add(new ColourPropertyComponent(this));
	/*add it to one section of the panel*/
	
	addProperties(*propertiesArray);
	
	setAllDisabled();
	
	setSize(400, 495);
}

SequenceOtPropertyPanel::~SequenceOtPropertyPanel() {
	
	deleteAllChildren();
}

SequenceOT* SequenceOtPropertyPanel::getConcerned() {
	
	return m_concerned;
}

void SequenceOtPropertyPanel::changeListenerCallback (void */*objectThatHasChanged*/) {
	
	if( ! ((MainComponent*)((PropertyFrame*)getParentComponent()))->getSCG()->getConduite()->isOneOtSelected())
		setAllDisabled();
	else
		m_concerned = dynamic_cast<SequenceOT*> (((MainComponent*)((PropertyFrame*)getParentComponent()))->getSCG()->getConduite()->getChildComponent(getNumChildComponents()-1));
	
	refreshAll();
}

void SequenceOtPropertyPanel::setAllDisabled() {
	
	for(int i=0; i< propertiesArray->size(); ++i){
		
		PropertyComponent* x = propertiesArray->getReference(i);
		x->setVisible(false);
	}
}

void SequenceOtPropertyPanel::setConcerned(Component* _concerned) {
	
	if(_concerned == NULL){
		setAllDisabled();
		return;
	}
	m_concerned = dynamic_cast<SequenceOT*>(_concerned);
	refreshAll();
}

void SequenceOtPropertyPanel::checkFlexCoherence() {
	/*
	 int fstart = getConcerned()->getDurationBehavior()->getFlexStart();
	 int fstop  = getConcerned()->getDurationBehavior()->getFlexStop();
	 
	 if( fstop != NO_STOP && fstop < fstart ) {//si fstop défini et inférieur à fstart
	 
	 getConcerned()->getDurationBehavior()->setFlexStop( fstart + ((TimeBox*)getConcerned())->getParentSequencer()->getZoomer()->convertTimeToPix(abs(fstop-fstart)));//on fait bondir le stop en symétrie positive
	 refreshAll();
	 }
	 */
}

//==============================================================================
void SequenceOtPropertyPanel::paint (Graphics& g) {
	
    g.fillAll (Colours::lightgrey);
	PropertyPanel::paint(g);
}

