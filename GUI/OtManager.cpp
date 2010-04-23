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
 *  OtManager.cpp
 *  Virage
 *
 *  Created by Henry on 25/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OtManager.h"

#include "Sequencer.h"
#include "MainWindow.h"

OtManager::OtManager(Sequencer* _parent) : m_parent(_parent), m_box_counter(-1) {}

OtManager::~OtManager(){}

TimeBox* OtManager::addBox(String etiquette, bool _isFlexible, int _x, int _y, int _width, int _height, unsigned int pre_index, Component* /*seqFrame*/){
	
	unsigned int index;
	
	if(pre_index == 0) {
		index = m_parent->getEngines()->addBox(m_parent->getZoomer()->convertPixToTime(_x),m_parent->getZoomer()->convertPixToTime(_width),ROOT_BOX_ID);/*ajout moteur*/
	}
	else //dans le cas du load d'un fichier, par ex, l'id est déjà spécifié
		index = pre_index; //cas de duplication, dans ce cas l'id est gérée par ::duplicateBox->!! DIFFÉRENT DE ADDBOX
	
	if(index == NO_ID) {
		return NULL;
	}
	
	cout << " add a box with id " << index << endl;
	
	TimeBox* addedBox = new TimeBox(m_parent, index, etiquette, _isFlexible, _x, _y, _width, _height);
	
	(*(m_parent->getBoxMap()))[index] = ((addedBox));/*ajout registre*/
	m_parent->addAndMakeVisible((Component*)addedBox);/*ajout graphique*/
	m_parent->addAndMakeVisible((Component*)(addedBox->getDurationBehavior()));/*ajout graphique*/
	addedBox->toFront(true);/*prend le focus*/
	//m_parent->m_electedItemSet->addToSelectionBasedOnModifiers(addedBox, ModifierKeys::commandModifier);
	m_parent->getMainWindow()->getNamespacePanel()->setEditedBox(addedBox);
	m_parent->sendSynchronousChangeMessage(m_parent);
	
	return addedBox;
}

TimeBox* OtManager::duplicateBox(TimeBox* copiedBox){
	
	unsigned int pastedBoxId = m_parent->getEngines()->addBox(
															  m_parent->getZoomer()->convertPixToTime(copiedBox->getX()+s_COPY_DELTA),
															  m_parent->getZoomer()->convertPixToTime(copiedBox->getWidth()),
															  ROOT_BOX_ID);
	m_parent->deltaCopiedId = pastedBoxId - copiedBox->getId();
	
	if(pastedBoxId==0) return 0;
	
	TimeBox* pastedBox = new TimeBox(pastedBoxId, copiedBox);
	(*(m_parent->getBoxMap()))[pastedBoxId] = pastedBox;/*ajout registre*/
	m_parent->addAndMakeVisible((Component*)pastedBox);/*ajout graphique*/
	m_parent->addAndMakeVisible((Component*)(pastedBox->getDurationBehavior()));/*ajout graphique*/
	pastedBox->toFront(true);/*prend le focus*/
	m_parent->m_electedItemSet->addToSelection(pastedBox);
	m_parent->m_electedItemSet->deselect(copiedBox);
	m_parent->repaint();
	m_parent->sendSynchronousChangeMessage(m_parent);
	
	
	pastedBox->toFront(true);
	//m_parent->componentBroughtToFront((Component&)pastedBox);
	
	return pastedBox;
}

Relation* OtManager::addRelation(unsigned int _InBoxId, unsigned int _OutBoxId, unsigned int mother_ctrlPt, unsigned int daughter_ctrlPt, unsigned int pre_index, bool isFlexible) {
	m_parent->m_electedItemSet->deselectAll();
	
	int direction, xi, xf;
	
	if(_InBoxId>0) xi = (mother_ctrlPt == CPT_L) ? m_parent->getBox(_InBoxId)->getPlugStartX() : m_parent->getBox(_InBoxId)->getPlugEndX();
	if(_OutBoxId>0) xf = (daughter_ctrlPt == CPT_L) ? m_parent->getBox(_OutBoxId)->getPlugStartX() : m_parent->getBox(_OutBoxId)->getPlugEndX();
	
	direction = xi - xf;
	unsigned int index;
	
	if(pre_index == 0) index = m_parent->getEngines()->addTemporalRelation(_InBoxId, mother_ctrlPt, _OutBoxId, daughter_ctrlPt, (direction<0)?ANTPOST_ANTERIORITY:ANTPOST_POSTERIORITY, 0);/*retourne 0 si impossible ou redondant*/
	else index = pre_index;
	
	if(index== 0) return 0;
	
	Relation* addedRelation = new Relation(m_parent, index, xi, m_parent->getBox(_InBoxId)->getPlugY(), xf, m_parent->getBox(_OutBoxId)->getPlugY(), _InBoxId, _OutBoxId, isFlexible);
	//Automatic temporal mode
	if(addedRelation->isTriggered(daughter_ctrlPt)) addedRelation->getDurationBehavior()->setFlexible(true);
	
	m_parent->addAndMakeVisible(addedRelation);//juce
	addedRelation->setBounds(0,0,m_parent->getWidth(),m_parent->getHeight());
	m_parent->sendSynchronousChangeMessage(m_parent);
	
	/*ajout registre*/
	(*(m_parent->getRelMap()))[index] = addedRelation;
	m_parent->getBox(_InBoxId)->setNewRelation(   index, (unsigned int)((m_parent->getEngines()->getRelationFirstCtrlPointIndex(index)) == BEGIN_CONTROL_POINT_INDEX), true);
	m_parent->getBox(_OutBoxId)->setNewRelation(  index, (unsigned int)((m_parent->getEngines()->getRelationSecondCtrlPointIndex(index)) == BEGIN_CONTROL_POINT_INDEX), false);
	
	addedRelation->toFront(true);
	
	m_parent->m_electedItemSet->addToSelectionBasedOnModifiers (addedRelation, ModifierKeys::commandModifier);
	return addedRelation;
}

Trigger* OtManager::addTrigger(int x, Colour _couleur, TimeBox* controlledBox, bool anchoringStart,  unsigned int pre_index, bool loadedTrig) {
	
	unsigned int idTrig;
	
	if(pre_index == 0) {
		idTrig = m_parent->getEngines()->addTriggerPoint();
	}
	else {
		idTrig = pre_index;
	}
	
	if(idTrig == NO_ID) {
		return 0;
	}
	
	Trigger* added = new Trigger(idTrig, m_parent, controlledBox, anchoringStart, x, m_parent->getViewedPositionY());
	added->setColour(_couleur);
	
	(*(m_parent->getTrigMap()))[idTrig] = added;
	m_parent->addAndMakeVisible(added);
	m_parent->addAndMakeVisible(added->getControlJack());
	
	if(controlledBox != 0) {
		added->setLinkedBox(controlledBox, anchoringStart, loadedTrig);/* on renseigne le trigger */
		(*(controlledBox->getTriggersMap())) [idTrig] = anchoringStart; /* on renseigne la boite */
		added->getControlJack()->resized();
	}
	
	added->toFront(true);
	
	m_parent->m_electedItemSet->addToSelectionBasedOnModifiers (added, ModifierKeys::commandModifier);
	m_parent->sendSynchronousChangeMessage(m_parent);
	
	return added;
}


/* ONE REMOVE */
void OtManager::removeBox(unsigned int id) {
	
  	m_parent->getMainWindow()->getPropertyFrame()->setConcerned(NULL);
	m_parent->getMainWindow()->getSafeComponentGetter()->getAutomationScope()->setConcerned(NULL);
	
	if (id != NO_ID) {
		TimeBox* deleted = m_parent->getBox(id);
		map<unsigned int , bool> deletedRelRegister = *(deleted->getRelationMap());/* pointer directement sur deleted->getRelationMap() ne marche pas */
		map<unsigned int, bool>::iterator itrel = deletedRelRegister.begin();
		while(itrel  !=  deletedRelRegister.end()) {
			if(itrel->first != 0) {
				unsigned int id = itrel->first;
				removeRelation(id);
			}
			itrel++;/* incrémentation malgré le removeRelation car le erase se fait sur le registre de la boite, pas sur deletedRelRegister*/
		}
		
		map<unsigned int , bool> deletedTriRegister = *(deleted->getTriggersMap());
		map<unsigned int , bool>::iterator itt = deletedTriRegister.begin();
		while(itt  !=  deletedTriRegister.end()) {
			m_parent->getTrigger(itt->first)->linkHasBroken();/* on renseigne le trigger */
			m_parent->getEngines()->setTriggerPointMessage(itt->first, "");/* on renseigne la boite */
			itt++;
		}
		
		/*NETBROWSER*/m_parent->getMainWindow()->getNamespacePanel()->setEditedBox(0);
		deleted->fadeOutComponent(100, 0, 0, 1.0f);
		m_parent->removeChildComponent(deleted);
		m_parent->removeChildComponent(deleted->getDurationBehavior());
		m_parent->getEngines()->removeBox(id);
		m_parent->getBoxMap()->erase(id);
		m_parent->sendSynchronousChangeMessage(m_parent);
		m_parent->repaint();
	}
}

void OtManager::removeTrigger(unsigned int id) {
	
	m_parent->getMainWindow()->getPropertyFrame()->setConcerned(NULL);
	m_parent->getMainWindow()->getSafeComponentGetter()->getAutomationScope()->setConcerned(NULL);
	
	if (id != NO_ID) {
		Trigger* deleted = m_parent->getTrigger(id);
		if(deleted->isConnected())	{
			deleted->getLinkedBox()->deleteTriggerId(id);
			deleted->linkHasBroken();
		}
		deleted->fadeOutComponent(100, 0, 0, 1.0f);
		m_parent->removeChildComponent(deleted);
		m_parent->removeChildComponent(deleted->getControlJack());
		m_parent->getEngines()->removeTriggerPoint(id);
		m_parent->getTrigMap()->erase(id);
		m_parent->sendSynchronousChangeMessage(m_parent);
		m_parent->repaint();
	}
}

void OtManager::removeRelation(unsigned int id) {
	
	m_parent->getMainWindow()->getPropertyFrame()->setConcerned(NULL);
	m_parent->getMainWindow()->getSafeComponentGetter()->getAutomationScope()->setConcerned(NULL);
	
	if (id != NO_ID) {
		Relation* deleted = m_parent->getRelation(id);
		//on la supprime des répertoires des boîtes
		m_parent->getBox(deleted->getMotherId())->deleteRelationId(id);
		m_parent->getBox(deleted->getDaughterId())->deleteRelationId(id);
		/*graphic*/m_parent->removeChildComponent(deleted->getDurationBehavior());
		/*graphic*/m_parent->removeChildComponent(deleted);
		/*engine*/m_parent->getEngines()->removeTemporalRelation(id);
		/*registre*/m_parent->getRelMap()->erase(id);
		m_parent->sendSynchronousChangeMessage(m_parent);
		m_parent->repaint();
	}
}

/* ALL REMOVE */
void OtManager::removeAllRelation() {
	
	if(m_parent->getRelMap()->empty() ) return;
	
	map<unsigned int, Relation*>::iterator it = m_parent->getRelMap()->begin();
	while(it != m_parent->getRelMap()->end() ) {
		Relation * deleted = it->second;
		if(it->second != NULL){
			unsigned int id = it->first;
			//on la supprime des répertoires des boîtes (facultatif ?)
			m_parent->getBox(deleted->getMotherId())->deleteRelationId(id);
			m_parent->getBox(deleted->getDaughterId())->deleteRelationId(id);
			/*graphic*/m_parent->removeChildComponent(deleted->getDurationBehavior());
			/*graphic*/m_parent->removeChildComponent(deleted);
			/*engine*/m_parent->getEngines()->removeTemporalRelation(id);
		}
		it++;
	}
	m_parent->getRelMap()->clear();
	m_parent->sendSynchronousChangeMessage(m_parent);
	m_parent->repaint();
}

void OtManager::removeAllBox() {
	
	if(m_parent->getBoxMap()->empty()) return;
	
	/* Sur l'ensemble des Box... */
	map<unsigned int, TimeBox*>::iterator it = m_parent->getBoxMap()->begin();
	while(it != m_parent->getBoxMap()->end() ) {
		TimeBox* deleted = it->second;
		/* Se couper de tous les triggers... */
		map<unsigned int, bool> triggerMap = *(deleted->getTriggersMap());
		map<unsigned int , bool>::iterator itt = triggerMap.begin();
		while(itt  !=  triggerMap.end()) {
			m_parent->getTrigger(itt->first)->linkHasBroken();/*on renseigne le trigger*/
			m_parent->getEngines()->setTriggerPointMessage(itt->first, "");/*on renseigne la boite*/
			itt++;
		}
		/* ...puis on delete... */
		m_parent->removeChildComponent(deleted->getDurationBehavior());
		m_parent->getEngines()->removeBox(it->first);
		if (deleted) {
			deleteAndZero (deleted);
		}
		/* ...pas de erase(), donc on incrémente. Up.*/
		it++;
	}
	m_parent->getBoxMap()->clear();
	m_parent->repaint();
	m_parent->sendSynchronousChangeMessage(m_parent);
}

void OtManager::removeAllTrigger() {
	
	if(m_parent->getTrigMap()->size() ==0) return;
	
	m_parent->getMainWindow()->getPropertyFrame()->setConcerned(NULL);
	
	map<unsigned int, Trigger*>::iterator it3 = m_parent->getTrigMap()->begin();
	while(it3 != m_parent->getTrigMap()->end() ) {
		m_parent->getEngines()->removeTriggerPoint(it3->first);
		m_parent->removeChildComponent(it3->second);
		m_parent->removeChildComponent(it3->second->getControlJack());
		it3++;
	}
	m_parent->getTrigMap()->clear();
	m_parent->sendSynchronousChangeMessage(m_parent);
	m_parent->repaint();
}



int OtManager::getBoxCounter() {
	
	return m_box_counter;
}

void OtManager::setBoxCounter(int _counter) {
	
	m_box_counter = _counter;
}

bool OtManager::findBox(int boxId) {
	return m_parent->getBoxMap()->find(boxId) != m_parent->getBoxMap()->end();	
}
