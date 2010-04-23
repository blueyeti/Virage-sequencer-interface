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
 *  deviceManagerWindow.h
 *  Virage
 *
 *  Created by Henry Bernard on 24/06/09.
 *  Copyright 2009 BlueYeti. All rights reserved.
 *
 */

#ifndef _deviceManager_H_
#define _deviceManager_H_

#include "includes.h"

class Sequencer;
class NetComponent;

/* *** */
class deviceManager : public Component, public ButtonListener {
	
private:	
	Device* m_device_to_modify;
	
	PropertyPanel* m_netPropertyPanel;
	Array<PropertyComponent*> * m_propertiesArray;
	
	//variables nouveau device :
	String m_name_of_nu_device;
	String m_ip_of_nu_device;
	String m_port_of_nu_device;
	String m_protocole_of_nu_device;
	
	TextButton* m_addButton;
	TextButton* m_clearButton;
	TextButton* m_validButton;
	Label* m_confirmLabel;
	
	NetComponent* m_target;
	
public:
	deviceManager(NetComponent* _target = NULL);
	~deviceManager();
	
	void resized();
	void raz();
	bool verifyForm();
	void flipToNetExplorer();
	
	bool keyPressed	(const KeyPress & 	key);
	void setNewDeviceIp(String _ip);	
	void setNewDevicePort(String _port);	
	void setNewDeviceName(String _name);	
	void setNewDeviceProtocole(String _protocole);	
	
	void buttonClicked (Button* buttonThatWasClicked);
};

#endif