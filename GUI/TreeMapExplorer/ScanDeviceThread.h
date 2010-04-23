/*
 *  ScanDeviceThread.h
 *  Virage
 *
 *  Created by Laurent Garnier on 09/02/10.
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

/*!
 * \file ScanDeviceThread.h
 * \author Laurent Garnier (LIMSI)
 * \date 2009-2010
 *
 * \brief This class inherits from a Juce Thread class to display a progress bar while an action is performed.
 * Here the action is the Virage Network Device Scanning to init the TreeMap
 *
 */

#ifndef __ScanDeviceThread_h__
#define __ScanDeviceThread_h__

#include "TreeComm.h"

class ScanDeviceThread : public ThreadWithProgressWindow 
{
public:
	ScanDeviceThread(TreeComm *tree)    : ThreadWithProgressWindow (T("Network Device Scanning..."), true, false)
	{
		m_tree = tree;
	}
	
	~ScanDeviceThread()
	{
	}
	
	void run()
	{
		setProgress (-0.5);
		setStatusMessage(T("Please wait"));
		wait(2000);

		m_tree->init();
	}
	
private:
	TreeComm* m_tree;
};

#endif