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
/**
   includes.h : contains all necessary includes to use Virage API
*/
#ifndef _INCLUDES_H_
#define _INCLUDES_H_

/*** DBG ***/
/** VIRAGE VDBG MODE ON/OFF, un-comment to get debug visu feedback in the soft, or comment it to normal user mode */
//#define VDBG

/*** VIRAGE ***/
#include "AllStatic.h"
#include "TreeMapStatics.h"

/*** ENUM ***/
enum E_MouseMode { DRAWBOX, SELECTION };
enum E_TriggerState { READY_STATE, WAITED_STATE, TRIGGERED_STATE };
enum E_Messaging { MESSAGE_IN, MESSAGE_OUT };
enum E_Callback { CONTROL_POINT, TRIGGER_POINT };
enum E_ControlPoint { LEFT, RIGHT };

enum E_AutomationToolMode {
	
  DOT_MAKER,
  LINE_MAKER,
  BEZIER_MAKER,
  ERASER,
  PEN,
  NONE
};
using namespace std;

class Device {
 public:
  Device(String _name=String("0"), String _protocole=String("0"), String _ip=String("0"), String _port=String("0")){
    name = _name;
    protocole = _protocole;
    ip = _ip;
    port = _port;
  }
  ~Device(){}

  String name, protocole, ip, port;
};

/*** JUCE ***/
#include "juce_Config.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <cmath>
#include <map>
#include <vector>
#include <juce.h>// And this includes all the juce headers.. (toujours en dernier!)

#endif