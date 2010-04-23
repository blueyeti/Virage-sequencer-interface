/*
 *  TreeElement.h
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
 * \file TreeElement.h
 * \author Laurent Garnier (LIMSI)
 * \date 2009-2010
 *
 * \brief A TreeElement represents a network hierarchical address (osc-like).
 *
 */

#ifndef _TreeElement_H_
#define _TreeElement_H_

#include <string>

class TreeElement
{
public:
	TreeElement();
	TreeElement(const std::string& level, unsigned int priority, const std::string& absolutePath, const std::string& relativePath, const std::string& value, const std::string& type);
	~TreeElement();
	
	void setLevel(const std::string& level);
	const std::string& getLevel();
	
	void setPriority(unsigned int priority);
	unsigned int getPriority();
	
	void setAbsolutePath(const std::string& absolutePath);
	const std::string& getAbsolutePath();
	
	void setRelativePath(const std::string& absolutePath);
	const std::string& getRelativePath();
	
	void setAttributeValue(const std::string& value);
	const std::string& getAttributeValue();
	
	void setAttributeType(const std::string& type);
	const std::string& getAttributeType();
	
	void setAttributeRange(const std::string& range);
	const std::string& getAttributeRange();
	
private:
	std::string m_level; //DEVICE || NODE || LEAF || ATTRIBUTE
	unsigned int m_priority;
	std::string m_absolutePath;
	std::string m_relativePath;
	std::string m_attributeValue;
	std::string m_attributeType;
	std::string m_attributeRange;
};

#endif