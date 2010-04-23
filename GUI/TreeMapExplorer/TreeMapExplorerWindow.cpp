/*
 *  TreeMapExplorerWindow.cpp
 *  sequenceur
 *
 *  Created by Laurent Garnier on 14/01/10.
 *  Copyright 2010 LIMSI. All rights reserved.
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

#include "TreeMapExplorerWindow.h"
#include "TreeComm.h"
#include "Explorer.h"
#include "TimeBox.h"
#include "Engines.hpp"

TreeMapExplorerWindow::TreeMapExplorerWindow(Engines* engines, TimeBox* editedBox) : DocumentWindow (T("TreeMap Explorer"), Colours::lightgrey, DocumentWindow::minimiseButton, true)
{
	//m_treeComm = new TreeComm(engines);
	//m_treeComm->init();
	//m_explorer = new Explorer(m_treeComm, editedBox, true);
	//setContentComponent(m_explorer, true, true);
	setTitleBarHeight(25);
}

TreeMapExplorerWindow::~TreeMapExplorerWindow()
{
	delete m_explorer;
	delete m_treeComm;
	setContentComponent(0, true);
}

Explorer* 
TreeMapExplorerWindow::getExplorer()
{
	return m_explorer;	
}

void 
TreeMapExplorerWindow::setEditedTimeBox(TimeBox* edited)
{
	m_explorer->setEditedTimeBox(edited);	
}

void 
TreeMapExplorerWindow::networkTreeUpdate()
{
	m_explorer->networkTreeUpdate();
}

void 
TreeMapExplorerWindow::update()
{
	m_explorer->update();	
}

void 
TreeMapExplorerWindow::closeButtonPressed()
{
	setVisible(false);
}