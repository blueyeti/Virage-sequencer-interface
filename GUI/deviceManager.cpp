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
 *  deviceManager.cpp
 *  Virage
 *
 *  Created by Henry Bernard on 24/06/09.
 *  Copyright 2009 BlueYeti. All rights reserved.
 *
 */

#include "deviceManager.h"
#include "Sequencer.h"
#include "NetComponent.h"

/********** NAME */
class DeviceNameProperty : public TextPropertyComponent {
public: 
	DeviceNameProperty(deviceManager* _panel) : TextPropertyComponent(String("Device name"), 100, false) { m_panel = _panel; };
	~DeviceNameProperty(){};
	void 	setText (const String &newText){
		m_panel->setNewDeviceName(newText);
	}
 	const String 	getText () const {
		return String::empty;		
	}
private:
	deviceManager* m_panel;
};
/********** IP */
class DeviceIpProperty : public TextPropertyComponent {
public: 
	DeviceIpProperty(deviceManager* _panel) : TextPropertyComponent(String("Device IP"), 100, false) { m_panel = _panel; };
	~DeviceIpProperty(){};
	void 	setText (const String &newText){		
		m_panel->setNewDeviceIp(newText);
	}
 	const String 	getText () const {
		return String::empty;		
	}
private:
	deviceManager* m_panel; 
};
/********** PORT */
class DevicePortProperty : public TextPropertyComponent {
public: 
	DevicePortProperty(deviceManager* _panel) : TextPropertyComponent(String("Device Port"), 100, false) { m_panel = _panel; };
	~DevicePortProperty(){};
	void 	setText (const String &newText){
		m_panel->setNewDevicePort(newText);
	}
 	const String 	getText () const {
		return String::empty;		
	}
private:
	deviceManager* m_panel; 
};
/********** PROTOCOLE */
class DeviceProtocoleProperty : public ChoicePropertyComponent {
public: 
	DeviceProtocoleProperty(deviceManager* _panel) : ChoicePropertyComponent(String("Protocole (PlugIn)")) { 
		
		m_panel = _panel;
		
		choices.add(String("OSC"));
		choices.add(String("Minuit"));
		choices.add( String::empty);
	}
	
	~DeviceProtocoleProperty(){};
	void 	setIndex (const int newIndex){	
		m_panel->setNewDeviceProtocole(newIndex==0?String("OSC"):String("Minuit"));
	}
	
	int getIndex() const{
		return 0;
	}
	
private:
	deviceManager* m_panel; 
	/*StringArray ChoicePropertyComponent::choices [protected] The list of options that will be shown in the combo box. Your subclass must populate this array in its constructor. If any empty strings are added, these will be replaced with horizontal separators (see ComboBox::addSeparator() for more info*/
};
/***********************************************************************************************************************************************************************************************************/

deviceManager::deviceManager(NetComponent* _target) :
m_name_of_nu_device(String::empty), m_ip_of_nu_device(String::empty), m_port_of_nu_device(String::empty), m_protocole_of_nu_device(String("OSC"))
{
	setColour(0, s_COLOUR_BACKGROUND);
	
	m_target = _target;
	
	m_propertiesArray = new Array<PropertyComponent*>;
	m_propertiesArray->add(new DeviceNameProperty(this));
	m_propertiesArray->add(new DeviceIpProperty(this));
	m_propertiesArray->add(new DevicePortProperty(this));
	m_propertiesArray->add(new DeviceProtocoleProperty(this));
	
	addAndMakeVisible(m_netPropertyPanel = new PropertyPanel());
	m_netPropertyPanel->addProperties(*m_propertiesArray);
	
	addAndMakeVisible(m_addButton = new TextButton(T("Add")));
	m_addButton->addButtonListener (this);
	m_addButton->setColour (TextButton::buttonColourId, Colours::white);
	m_addButton->setColour (TextButton::textColourId, Colours::black);
	m_addButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	
	addAndMakeVisible(m_validButton = new TextButton(T("Validate modif.")));
	m_validButton->addButtonListener (this);
	m_validButton->setColour (TextButton::buttonColourId, Colours::white);
	m_validButton->setColour (TextButton::textColourId, Colours::black);
	m_validButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	m_validButton->setEnabled(false);
	
	addAndMakeVisible(m_clearButton = new TextButton(T("Back to Explorer")));
	m_clearButton->addButtonListener (this);
	m_clearButton->setColour (TextButton::buttonColourId, Colours::white);
	m_clearButton->setColour (TextButton::textColourId, Colours::black);
	m_clearButton->setConnectedEdges(Button::ConnectedOnBottom | Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop);
	
	addAndMakeVisible(m_confirmLabel = new Label(T(""), T("Fill infos and click \'ADD\'")));
	m_confirmLabel->setJustificationType(Justification::centred);
	m_confirmLabel->setColour (TextButton::textColourId, Colours::black);
	
	m_device_to_modify = new Device();
}


deviceManager::~deviceManager() {

  if (m_propertiesArray) {
    deleteAndZero(m_propertiesArray );	
  }
  if (m_netPropertyPanel) {
    deleteAndZero(m_netPropertyPanel);
  }
	//deleteAllChildren();
}

void deviceManager::resized() {

	int buttonHeight = 40;
	int buttonWidth = 75;
	m_netPropertyPanel->setBounds(0,0,getWidth(), 100);
	m_confirmLabel->setBounds(2, m_netPropertyPanel->getHeight()+2,  getWidth(), getHeight()-buttonHeight-m_netPropertyPanel->getHeight()-4);
	m_validButton->setBounds(getWidth()-buttonWidth*3-2 , getHeight()-buttonHeight-2, buttonWidth, buttonHeight);
	m_addButton->setBounds(getWidth()-buttonWidth*2-2 , getHeight()-buttonHeight-2, buttonWidth, buttonHeight);
	m_clearButton->setBounds(getWidth()-buttonWidth-2, getHeight()-buttonHeight-2, buttonWidth, buttonHeight);	
}

void deviceManager::setNewDeviceIp(String _ip){
	m_ip_of_nu_device = _ip;
}

void deviceManager::setNewDevicePort(String _port){
	m_port_of_nu_device = _port;
}

void deviceManager::setNewDeviceName(String _name){
	m_name_of_nu_device = _name;
}

void deviceManager::setNewDeviceProtocole(String _protocole){
	m_protocole_of_nu_device = _protocole;
}

bool deviceManager::keyPressed	(const KeyPress & 	key){
	if(key == KeyPress(KeyPress::tabKey)) 
	{
		flipToNetExplorer();
		return true;
	}
	if(key == KeyPress(KeyPress::returnKey)) {
		m_addButton->triggerClick(); 
		return true;
	}
	return false;
}

void deviceManager::raz(){

	m_netPropertyPanel->refreshAll();//clear all forms
	m_name_of_nu_device = String::empty;
	m_ip_of_nu_device = String::empty;
	m_port_of_nu_device = String("OSC");
}


void deviceManager::flipToNetExplorer(){
	
	((PropertyFrame*)getParentComponent())->setContentComponent((Component*)m_target);
}
	   
/*JUCE*/void deviceManager::buttonClicked (Button* buttonThatWasClicked){

	if(buttonThatWasClicked == m_addButton){
		
		if(verifyForm()){
			m_target->addDevice((string) m_name_of_nu_device , (string) m_protocole_of_nu_device, (string) m_ip_of_nu_device , (string) m_port_of_nu_device  );
			m_confirmLabel->setText(String(m_name_of_nu_device+ String(" is added")), false);
		}
		
		//on considère le device ajouté comme nouveau "modifiable"
		m_device_to_modify = new Device(m_name_of_nu_device, m_ip_of_nu_device, m_port_of_nu_device, m_protocole_of_nu_device);
		
	}
	if(buttonThatWasClicked == m_clearButton){
		m_confirmLabel->setText(String("Fill infos and click \'ADD\'"), false);	
		m_netPropertyPanel->refreshAll();//clear all actually
		flipToNetExplorer();
	}
	
	if(buttonThatWasClicked == m_validButton && m_device_to_modify != 0) {
	
		String m_device_to_modify_old_name = m_device_to_modify->name;
		
		if(m_name_of_nu_device!=String::empty) {
			m_device_to_modify->name = m_name_of_nu_device;
		}
		
		//product
		if(verifyForm()){
			m_target->modifyDevice((string) m_device_to_modify_old_name , (string) m_name_of_nu_device , (string) m_protocole_of_nu_device, (string) m_ip_of_nu_device , (string) m_port_of_nu_device  );
			String modifOrAdd =  m_device_to_modify==0?String("is added"):String(" is modified");
			m_confirmLabel->setText(String(m_name_of_nu_device + modifOrAdd), false);
		}
	}
}



bool deviceManager::verifyForm(){

	//scan
	if(m_name_of_nu_device==String::empty) {
		m_confirmLabel->setText(String("Name is missing !"), false);
		return false;
	}
	else if(m_ip_of_nu_device==String::empty) {
		m_confirmLabel->setText(String("IP address is missing !"), false);
		return false;
	}
	else if(m_port_of_nu_device==String::empty) {
		m_confirmLabel->setText(String("Port is missing !"), false);
		return false;
	}
	else if(m_protocole_of_nu_device==String::empty) {
		m_confirmLabel->setText(String("Protocole is missing !"), false);
		return false;
	}
	//ok:
	return true;
}
