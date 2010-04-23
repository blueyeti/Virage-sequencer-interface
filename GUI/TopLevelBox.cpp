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
 *  TopLevelBox.cpp
 *  Virage
 *
 *  Created by Henry Bernard on 15/07/09.
 *  Copyright 2009 BlueYeti. All rights reserved.
 *
 */
#ifndef _TopLevelBoxManager_H_
#define _TopLevelBoxManager_H_

#include "TopLevelBox.h"

class TopLevelBoxManager  : public Timer, public DeletedAtShutdown
{
public:
  //==============================================================================
  TopLevelBoxManager()
    : windows (8),
      currentActive (0)
  {
  }

  ~TopLevelBoxManager()
  {
    clearSingletonInstance();
  }

  juce_DeclareSingleton_SingleThreaded_Minimal (TopLevelBoxManager)

  void timerCallback()
  {
    startTimer (1731);
    TopLevelBox* active = 0;

    if (Process::isForegroundProcess())
      {
	active = currentActive;

	Component* const c = Component::getCurrentlyFocusedComponent();

	TopLevelBox* tlw = dynamic_cast <TopLevelBox*> (c);

	if (tlw == 0 && c != 0)
	  // (unable to use the syntax findParentComponentOfClass <TopLevelBox> () because of a VC6 compiler bug)
	  tlw = c->findParentComponentOfClass ((TopLevelBox*) 0);

	if (tlw != 0)
	  active = tlw;
      }

    if (active != currentActive)
      {
	currentActive = active;

	for (int i = windows.size(); --i >= 0;)
	  {
	    //HENRY    TopLevelBox* const tlw = (TopLevelBox*) windows.getUnchecked (i);
	    //HENRY    tlw->setWindowActive (isWindowActive (tlw));

	    i = jmin (i, windows.size() - 1);
	  }

	//MODIF //HENRY//Desktop::getInstance().triggerFocusCallback();
      }
  }

  bool addWindow (TopLevelBox* const w) throw()
  {
    windows.add (w);
    startTimer (10);

    return isWindowActive (w);
  }

  void removeWindow (TopLevelBox* const w) throw()
  {
    startTimer (10);

    if (currentActive == w)
      currentActive = 0;

    windows.removeValue (w);

    if (windows.size() == 0)
      deleteInstance();
  }

  VoidArray windows;

private:
  TopLevelBox* currentActive;

  bool isWindowActive (TopLevelBox* const tlw) const throw()
  {
    return (tlw == currentActive
	    || tlw->isParentOf (currentActive)
	    || tlw->hasKeyboardFocus (true))
      && tlw->isShowing();
  }

  TopLevelBoxManager (const TopLevelBoxManager&);
  const TopLevelBoxManager& operator= (const TopLevelBoxManager&);
};

#endif

juce_ImplementSingleton_SingleThreaded (TopLevelBoxManager)

void juce_CheckCurrentlyFocusedTopLevelBox() throw()
{
  if (TopLevelBoxManager::getInstanceWithoutCreating() != 0)
    TopLevelBoxManager::getInstanceWithoutCreating()->startTimer (20);
}
//DOUBLON

/** implementation */
//==============================================================================
TopLevelBox::TopLevelBox (const String& name,
			  const bool addToDesktop_)
  : SequenceOT (NULL, 0), useDropShadow (true), useNativeTitleBar (false), windowIsActive_ (false), shadower (0)
{
  setOpaque (true);

  if (addToDesktop_)
    Component::addToDesktop (getDesktopWindowStyleFlags());
  //MODIF HENRY A REMETTRE POUR OMBRE = else
  //setDropShadowEnabled (true);

  setWantsKeyboardFocus (true);
  setBroughtToFrontOnMouseClick (true);
  windowIsActive_ = TopLevelBoxManager::getInstance()->addWindow (this);
}

TopLevelBox::~TopLevelBox()
{
  if (shadower) {
    deleteAndZero (shadower);
  }
  TopLevelBoxManager::getInstance()->removeWindow (this);
}

//==============================================================================
void TopLevelBox::focusOfChildComponentChanged (FocusChangeType)
{
  if (hasKeyboardFocus (true))
    TopLevelBoxManager::getInstance()->timerCallback();
  else
    TopLevelBoxManager::getInstance()->startTimer (10);
}

void TopLevelBox::setWindowActive (const bool isNowActive) throw()
{
  if (windowIsActive_ != isNowActive)
    {
      windowIsActive_ = isNowActive;
      activeWindowStatusChanged();
    }
}

void TopLevelBox::activeWindowStatusChanged()
{
}

void TopLevelBox::parentHierarchyChanged()
{
  setDropShadowEnabled (useDropShadow);
}

void TopLevelBox::visibilityChanged()
{
  if (isShowing())
    toFront (true);
}

int TopLevelBox::getDesktopWindowStyleFlags() const
{
  int flags = ComponentPeer::windowAppearsOnTaskbar;

  if (useDropShadow)
    flags |= ComponentPeer::windowHasDropShadow;

  if (useNativeTitleBar)
    flags |= ComponentPeer::windowHasTitleBar;

  return flags;
}

void TopLevelBox::setDropShadowEnabled (const bool useShadow)
{
  useDropShadow = useShadow;

  if (isOnDesktop())
    {
      if (shadower) {
	deleteAndZero (shadower);
      }
      Component::addToDesktop (getDesktopWindowStyleFlags());
    }
  else
    {
      if (useShadow && isOpaque())
        {
	  if (shadower == 0)
            {
	      shadower = getLookAndFeel().createDropShadowerForComponent (this);

	      if (shadower != 0)
		shadower->setOwner (this);
            }
        }
      else
        {
	  if (shadower) {
	    deleteAndZero (shadower);
	  }

        }
    }
}

void TopLevelBox::setUsingNativeTitleBar (const bool useNativeTitleBar_)
{
  if (useNativeTitleBar != useNativeTitleBar_)
    {
      useNativeTitleBar = useNativeTitleBar_;
      recreateDesktopWindow();
      sendLookAndFeelChange();
    }
}

void TopLevelBox::recreateDesktopWindow()
{
  if (isOnDesktop())
    {
      Component::addToDesktop (getDesktopWindowStyleFlags());
      toFront (true);
    }
}

void TopLevelBox::addToDesktop (int windowStyleFlags, void* nativeWindowToAttachTo)
{
  /* It's not recommended to change the desktop window flags directly for a TopLevelBox,
     because this class needs to make sure its layout corresponds with settings like whether
     it's got a native title bar or not.

     If you need custom flags for your window, you can override the getDesktopWindowStyleFlags()
     method. If you do this, it's best to call the base class's getDesktopWindowStyleFlags()
     method, then add or remove whatever flags are necessary from this value before returning it.
  */

  if ((windowStyleFlags & ~ComponentPeer::windowIsSemiTransparent)
	   == (getDesktopWindowStyleFlags() & ~ComponentPeer::windowIsSemiTransparent)) {

			Component::addToDesktop (windowStyleFlags, nativeWindowToAttachTo);

  	if (windowStyleFlags != getDesktopWindowStyleFlags()) {
			sendLookAndFeelChange();
  	}
  }
}

//==============================================================================
void TopLevelBox::centreAroundComponent (Component* c, const int width, const int height)
{
  if (c == 0)
    c = TopLevelBox::getActiveTopLevelBox();

  if (c == 0)
    {
      centreWithSize (width, height);
    }
  else
    {
      int x = (c->getWidth() - width) / 2;
      int y = (c->getHeight() - height) / 2;
      c->relativePositionToGlobal (x, y);

      Rectangle parentArea (c->getParentMonitorArea());

      if (getParentComponent() != 0)
        {
	  getParentComponent()->globalPositionToRelative (x, y);
	  parentArea.setBounds (0, 0, getParentWidth(), getParentHeight());
        }

      parentArea.reduce (12, 12);

      setBounds (jlimit (parentArea.getX(), jmax (parentArea.getX(), parentArea.getRight() - width), x),
		 jlimit (parentArea.getY(), jmax (parentArea.getY(), parentArea.getBottom() - height), y),
		 width, height);
    }
}

//==============================================================================
int TopLevelBox::getNumTopLevelBoxs() throw()
{
  return TopLevelBoxManager::getInstance()->windows.size();
}

TopLevelBox* TopLevelBox::getTopLevelBox (const int index) throw()
{
  return (TopLevelBox*) TopLevelBoxManager::getInstance()->windows [index];
}

TopLevelBox* TopLevelBox::getActiveTopLevelBox() throw()
{
  TopLevelBox* best = 0;
  int bestNumTWLParents = -1;

  for (int i = TopLevelBox::getNumTopLevelBoxs(); --i >= 0;)
    {
      TopLevelBox* const tlw = TopLevelBox::getTopLevelBox (i);

      if (tlw->isActiveWindow())
        {
	  int numTWLParents = 0;

	  const Component* c = tlw->getParentComponent();

	  while (c != 0)
            {
	      if (dynamic_cast <const TopLevelBox*> (c) != 0)
		++numTWLParents;

	      c = c->getParentComponent();
            }

	  if (bestNumTWLParents < numTWLParents)
            {
	      best = tlw;
	      bestNumTWLParents = numTWLParents;
            }
        }
    }

  return best;
}

