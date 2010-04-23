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
 *  Sequencer.h
 *  Virage
 *
 *  Created by Henry Bernard and Luc Vercellin on 02/10/08.
 *  Copyright 2008 by Blue Yeti. All rights reserved.
 */
#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

#define DIST_MIN_CONNEXION 40
#define OFFSET_TRIGGER_HIT 15

#define CPT_L 1
#define CPT_R 2

#include "includes.h"
#include "ZoomManager.h"
#include "PropertyFrame.h"
#include "PlayController.h"
#include "SequenceOT.h"
#include "TimeBox.h"
#include "Relation.h"
#include "Trigger.h"
#include "Engines.hpp" //includes CSP
#include <map>
#include "SafeUpdater.h"
#include "AllPointingFacilities.h"
#include "TransportBar.h"
#include "TemporaryBox.h"
#include "CursorFactory.h"
#include "OtManager.h"

class SequencerFrame;
class MainWindow;
class HeadPlay;
/*!
 * \class Sequencer
 */

class Sequencer : public Component,
  public Timer,
  public LassoSource <Component*>,
  public ChangeBroadcaster,
  public ChangeListener,
  public ComponentListener
{
 private:

  OtManager* m_otManager;

  unsigned int m_SCENE_MAXLENGTH_MS;
  unsigned int m_SCENE_ROOTHEIGHT;
	
  static const float MS_2_PIX_RATIO; /*convertit les millisecondes en pixel*/
  static const float BPIX_2_SPIX_RATIO;	/*convertit la taille intrinseque des boites en taille visible dans le sequenceur*/

  bool m_magnetised;

  //Boxes
  unsigned int m_box_counter;

  std::map<unsigned int, TimeBox*> *_boxMap;//parc des box qui vont etre creees
  unsigned int _dragBoxId, _dropBoxId, _dragTrigId, _dropTrigId;
  int deltaCopiedId;
  bool _isRelBeingDragged;
  bool _anchoringStart, _anchoringEnd;

  TimeBox* currentPropertiedBox;
  MainWindow* m_ParentWindow;
  HeadPlay* m_headPlay;
  TemporaryLink* tempLink;
  TemporaryBox* previewRect;
  ZoomManager* m_zoomer;
  PlayController* m_controller;

  //Relations
  std::map<unsigned int, Relation*> *_relMap;//parc des relations qui vont etre creees
  unsigned int current_rel_catched;
  Facility* _facilityCircle;

  //TriggerPoint
  std::map<unsigned int, Trigger*> *_trigMap;//parc des relations qui vont etre creees

  //Drag&Drop
  bool _somethingIsBeingDraggedOver;
  bool _isDrawingBox;
  bool _isDrawingBus;
  bool _isSelecting;
  int visibleX, visibleY;
  Colour bgColour;

  //CSP engine
  Engines* m_engines;
  unsigned int m_relControlPointMother;
  unsigned int m_relControlPointDaughter;

  //Selection
  LassoComponent<Component*> lassoComp;
  int lasso_xi, lasso_xf, lasso_yi, lasso_yf;
  SelectedItemSet<Component*> *m_electedItemSet;
  SelectedItemSet<Component*> m_copiedItemSet;
  bool _allOtSelected;
  //Mouse mode
  int m_mouseMode;

  SequencerFrame* m_frameParent;
  unsigned int m_spaceBar_temporisator;

  SafeUpdater* m_safeUpdater;

  bool m_wantsStop;
  bool m_areJacksVisible;

  CursorFactory* m_cursorFactory;

  friend class OtManager;
  friend class PlayController;

  Sequencer(SequencerFrame * _parent = 0, unsigned int _maxDuration = s_SEQUENCE_MAX_SIZE, int _height = s_SEQUENCE_MAX_HEIGHT);

 public:
  //==============================================================================
	static Sequencer *getInstance(SequencerFrame * parent = 0, unsigned int maxDuration = s_SEQUENCE_MAX_SIZE, int height = s_SEQUENCE_MAX_HEIGHT) {
		static Sequencer *instance = new Sequencer(parent, maxDuration, height);
		return instance;
	}

  virtual ~Sequencer();
	
  bool isResizingByMouseOT;
  bool isZooming;

  /*Component*/
  void paint (Graphics& g);
  void mouseDown(const MouseEvent &e);
  void mouseDrag(const MouseEvent &e);
  void mouseUp(const MouseEvent &e);
  void mouseMove(const MouseEvent &e);
  void mouseDoubleClick(const MouseEvent &e);
  bool keyPressed	(const KeyPress & 	key);

  /*Component Listener*/
  void componentMovedOrResized (Component& component, bool wasMoved, bool wasResized);
  void componentBroughtToFront (Component& component);
  void componentChildrenChanged(Component& component);
  void modifierKeysChanged(const ModifierKeys& modifiers);

  /*LassoSource*/
  SelectedItemSet<Component*> * getSelectionSet();
  void findLassoItemsInArea (Array <Component*>& results, int x, int y, int w, int h);
  SelectedItemSet<Component*>& getLassoSelection();

  /*ChangeListener*/
  void changeListenerCallback(void * objectThatHasChanged);

  /*Timer*/
  void timerCallback() ;

  /*Sequencer*/
  PropertyFrame* getPropertyFrame();
  void setJacksVisible(bool x);
  bool areJacksVisible();
  bool wantsStopWhatever();
  void setAskForStop(bool _shouldStopEvenIfLoop);
  void refreshCursor();
  MainWindow* getMainWindow();
  void setParentWindow(MainWindow* _window);
  TransportBar* getTransportBar() ;
  Trigger* nextTrig();
  void simulateNextTrig();
  bool isAllOtSelected();
  bool isOneOtSelected();
  bool isMagnetised();
  void setMagnetised(bool _m);
  void duplicateBufferedSelection();
  void selectAll();
  void deselectAll();
  void clearAllOt();
  int getDuration();
  int getRootHeight();
  PlayController* getController();
  ZoomManager* getZoomer();
  Engines* getEngines();
  void select(SequenceOT* _ot, bool _isCmdDown);
  void showCommentFrame(bool x);

  /* Playback management */
  void reinitAfterReading();
  void letsGoForReading();
  bool isMessaging();
  void setMessaging(bool _isMessaging, E_Messaging _inOrOut);
  void showComments(unsigned int boxId);
  void appendTriggeredBoxComment(String _comment);
  void removeComment(unsigned int boxId);
  void removeComment(String _comment);
  void concatComments();

  void moveSelectionFrom(TimeBox* _movedBox);

  Component* getFacilityCircle();
  void activateFacility(TimeBox* _bx, bool sideFactor);
  void activateFacility(Trigger* _bx);
  void desactivateFacility();

  unsigned int getHitAnchorBoxId(int x, int y, unsigned int _minusId = 0) ;
  unsigned int getHitTriggerId(int x, int y) ;

  bool performChildMoves(TimeBox* _movedBox = 0, bool wasMoved=false, bool wasResized=false);
  void refresh();
  void setViewedPositionX(int x);
  void setViewedPositionY(int y);
  int getViewedPositionX();
  int getViewedPositionY();

  void setMouseMode(int _mode);
  int getMouseMode();

  void resetHeadPlay();

  TimeBox* getBoxToNetExplore();

  TimeBox* getBox(unsigned int _id);
  Relation* getRelation(unsigned int _id);
  Trigger* getTrigger(unsigned int _id);

  std::map<unsigned int, TimeBox*> *getBoxMap();
  std::map<unsigned int, Relation*> *getRelMap();
  std::map<unsigned int, Trigger*> *getTrigMap();

  //EXEC
  SafeUpdater* getSafeUpdater();
  void refreshComment();

  OtManager* getOtManager();
  int getHeadPlayPos();
  void setHeadPlayPos(unsigned int newPixPos);

  void burnTrigBefore(int _fromMs);

  void load(const std::string &filename);
  void addExecutionFinishedCallback();
};

void controlPointCrossedCallBack(unsigned int boxId, unsigned int controlPointIndex);

void triggeredTPCallBack(bool isWaited, unsigned int triggerId, unsigned int /*boxId*/, unsigned int /*controlPointIndex*/, std::string /*triggerMessage*/);

void executionFinishedCallBack();

void transportBarCallBack(unsigned int type,std::string message,std::string arg);

#endif
