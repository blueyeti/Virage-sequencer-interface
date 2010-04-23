/*
 *  ColumnExplorerComponent.h
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
 * \file ColumnExplorerComponent.h
 * \author Laurent Garnier (LIMSI)
 * \date 2009-2010
 *
 * \brief Represent a column explorer (mac-like) to explore a namespace tree.
 *
 */

#ifndef _ColumnExplorerComponent_H_
#define _ColumnExplorerComponent_H_

#include "includes.h"

#include <string>
#include <vector> 
#include <map>

class Explorer;
class TreeComm;
class Commons;
class LevelColumn;

using namespace std;

class ColumnExplorerComponent : public Component
{
public:
    ColumnExplorerComponent(Explorer *explorer, Commons *commons);
    ~ColumnExplorerComponent();
	
	void paint (Graphics& g);
    void resized();
	void refreshByTreeMapExpl();
	void rebuild();
	
	void update(uint levelIndex, string message);
	
private:
	Explorer *m_explorer;
	Commons *m_commons;
    Font font;
	int maxDepth;
	
	vector<LevelColumn*> columns;
	map<string, bool> m_selectionState;
};

#endif
