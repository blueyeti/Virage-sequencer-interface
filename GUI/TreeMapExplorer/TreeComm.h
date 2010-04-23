/*
 *  TreeComm.h
 *  TreeMapExplorer
 *
 *  Created by Laurent Garnier on 21/08/09.
 *  Copyright 2009 LIMSI. All rights reserved.
 *  
 *  Use Peter Kasper's tree.hh class : http://tree.phi-sci.com/
 *									   kasper.peeters@durham.ac.uk
 *
 *
 *  This software is governed by the CeCILL license under French law and
 *  abiding by the rules of distribution of free software.  You can  use, 
 *  modify and/ or redistribute the software under the terms of the CeCILL
 *  license as circulated by CEA, CNRS and INRIA at the following URL
 *  "http://www.cecill.info". 
 *
 *  As a counterpart to the access to the source code and  rights to copy,
 *  modify and redistribute granted by the license, users are provided only
 *  with a limited warranty  and the software's author,  the holder of the
 *  economic rights,  and the successive licensors  have only  limited
 *  liability. 
 *
 *  In this respect, the user's attention is drawn to the risks associated
 *  with loading,  using,  modifying and/or developing or reproducing the
 *  software by the user in light of its specific status of free software,
 *  that may mean  that it is complicated to manipulate,  and  that  also
 *  therefore means  that it is reserved for developers  and  experienced
 *  professionals having in-depth computer knowledge. Users are therefore
 *  encouraged to load and test the software's suitability as regards their
 *  requirements in conditions enabling the security of their systems and/or 
 *  data to be ensured and,  more generally, to use and operate it in the 
 *  same conditions as regards security.
 
 *  The fact that you are presently reading this means that you have had
 *  knowledge of the CeCILL license and that you accept its terms.
 */

/*!
 * \file TreeComm.h
 * \author Laurent Garnier (LIMSI)
 * \date 2009-2010
 *
 * \brief This class uses a network controller to explorer network devices, get their namespace and store each address like a TreeElement in a tree using Peter Kasper's tree.hh class(http://tree.phi-sci.com/)
 * Then TreeComm proposes access methods to TreeElement Items.
 *
 */

#ifndef _TreeComm_H_
#define _TreeComm_H_

#include "includes.h"
#include "tree.hh"

#include <string>
#include <vector>

class Engines;
class TreeElement;

static const int  REQUEST_NOT_SENT = -2;
static const int  TIMEOUT_EXCEEDED = -1;
static const int  NO_ANSWER = 0;
static const int  ANSWER_RECEIVED = 1;

class TreeComm
{
public:
	TreeComm(Engines *engines);
	~TreeComm();
	
	void init();
	void re_init(const std::string& address, tree<TreeElement*>::iterator nodePosition);
	
	void sendMessage(const std::string& message);
	
	String initWithXML(const File & file);
	String re_initWithXml(const std::string& address, tree<TreeElement*>::iterator currentPosition, XmlElement *xmlElt);
						  
	void clear();
	void display();
	
	int getTreeTotalDepth();
	std::string getTreeElementLevel(const std::string& address);
	std::string getTreeElementRelativeAddress(const std::string& address);
	std::string getTreeElementValue(const std::string& address);
	std::string getTreeElementType(const std::string& address);
	std::string getTreeElementRange(const std::string& address);

	std::vector<std::string> getDevices();
	std::vector<std::string> getChilds(const std::string& address);
	void getSnapshot(const std::string& address, std::vector<std::string> *snapshot);
	void getSubTreeAddresses(const std::string& address, std::vector<std::string> *subTree);
	
	void setPriority(const std::string& address, int priority);
	void reinitValue(const std::string& address);

	bool isFirstLeave(const std::string& address);
	bool isLastLeave(const std::string& address);	
	
private:
	Engines *m_controller;
	tree<TreeElement*> m_namespaceTree;
};

#endif