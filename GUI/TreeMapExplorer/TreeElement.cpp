/*
 *  TreeElement.cpp
 *  TreeTouchTest
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

#include "TreeElement.h"

TreeElement::TreeElement(){}

TreeElement::TreeElement(const std::string& level, unsigned int priority, const std::string& absolutePath, const std::string& relativePath, const std::string& value, const std::string& type) :
m_level(level), m_priority(priority), m_absolutePath(absolutePath), m_relativePath(relativePath), m_attributeValue(value), m_attributeType(type)
{
	m_attributeRange = "";
}


TreeElement::~TreeElement()
{}

void 
TreeElement::setLevel(const std::string& level)
{
	m_level = level;
}

const std::string& 
TreeElement::getLevel()
{
	return m_level;
}

void 
TreeElement::setPriority(unsigned int priority)
{
	m_priority = priority;
}

unsigned int 
TreeElement::getPriority()
{
	return m_priority;
}

void 
TreeElement::setAbsolutePath(const std::string& absolutePath)
{
	m_absolutePath = absolutePath;
}

const std::string& 
TreeElement::getAbsolutePath()
{
	return m_absolutePath;
}

void 
TreeElement::setRelativePath(const std::string& relativePath)
{
	m_relativePath = relativePath;
}

const std::string& 
TreeElement::getRelativePath()
{
	return m_relativePath;
}

void 
TreeElement::setAttributeValue(const std::string& value)
{
	m_attributeValue = value;	
}

const std::string& 
TreeElement::getAttributeValue()
{
	return m_attributeValue;
}

void 
TreeElement::setAttributeType(const std::string& type)
{
	m_attributeType = type;
}

const std::string& 
TreeElement::getAttributeType()
{
	return m_attributeType;
}

void 
TreeElement::setAttributeRange(const std::string& range)
{
	m_attributeRange = range;	
}

const std::string& 
TreeElement::getAttributeRange()
{
	return m_attributeRange;
}