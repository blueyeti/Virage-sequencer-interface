/*
 *  Explorer.h
 *  TreeMapExplorer
 *
 *  Created by Laurent Garnier on 21/08/09.
 *  Copyright 2009 LIMSI. All rights reserved.
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
 * \file Explorer.h
 * \author Laurent Garnier (LIMSI)
 * \date 2009-2010
 *
 * \brief The over component which contains the treeMap explorer at the top and the column explorer at the bottom.
 *
 */

#ifndef _Explorer_H_
#define _Explorer_H_

#include "includes.h"
#include "ScanDeviceThread.h"

#include <string>
#include <iostream>

class TreeComm;
class Commons;
class TreeMapCanvas;
class ColumnExplorerComponent;
class TimeBox;

class Explorer : public Component, public ButtonListener
{
public:
	Explorer(TreeComm *treeComm, TimeBox *editedBox, ScanDeviceThread *scan, bool widerSize);
	~Explorer();
	
	void setEditedTimeBox(TimeBox* edited);
	
	void reinitValues(const std::string& absoluteAddress);
	void deviceSendMessage(const std::string& message);
	void networkTreeUpdate();
	void update();
	void refresh(bool TreeMapToRefresh);
	void selectTreeMapItem(const std::string& absoluteAddress, bool isSelected);
	void setTargetedAddressLabel(const std::string& absoluteAddress);
	
	void paint (Graphics& g);
    void resized();
	void buttonClicked (Button* buttonThatWasClicked);
	
	//Access to TreeComm
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

	bool isFirstLeave(const std::string& address);
	bool isLastLeave(const std::string& address);	
	
private:
	TreeComm *m_tree;
	Commons *m_commons;
	TreeMapCanvas *m_treeMap;
	ColumnExplorerComponent *m_columnExpl;
	TextButton *m_assignStartButton;
	TextButton *m_assignEndButton;
	NetButton *m_updateButton;
	Label *m_targetedAddressLabel;
	
	TimeBox *m_editedBox;
	ScanDeviceThread *m_scanThread;
};

#endif