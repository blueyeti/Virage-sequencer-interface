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
 *  AdaptativeScrollbar.cpp
 *  Virage
 *
 *  Created by Henry on 28/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "AdaptativeScrollBar.h"
class AdaptativeScrollBar;

//==============================================================================
class AdaptativeScrollbarButton  : public Button
{

private:
  AdaptativeScrollBar& owner;

  AdaptativeScrollbarButton (const AdaptativeScrollbarButton&);
  const AdaptativeScrollbarButton& operator= (const AdaptativeScrollbarButton&);

public:
  int direction;

  AdaptativeScrollbarButton (const int direction_, AdaptativeScrollBar& owner_) throw()
    : Button (String::empty),
      owner (owner_),
      direction (direction_)

  {
    setWantsKeyboardFocus (false);
  }

  ~AdaptativeScrollbarButton()
  {
  }

  void paintButton (Graphics& /*g*/,
		    bool /*isMouseOver*/,
		    bool /*isMouseDown*/)
  {
    //	BY		getLookAndFeel().drawScrollbarButton (g, owner, getWidth(), getHeight(), direction, owner.isVertical(), isMouseOver, isMouseDown);
  }

  void clicked()
  {
    owner.moveScrollbarInSteps ((direction == 1 || direction == 2) ? 1 : -1);
  }
};


//==============================================================================
AdaptativeScrollBar::AdaptativeScrollBar (const bool vertical_,
					  const bool buttonsAreVisible)
  : minimum (0.0),
    maximum (1.0),
    rangeStart (0.0),
    rangeSize (0.1),
    singleStepSize (0.1),
    thumbAreaStart (0),
    thumbAreaSize (0),
    thumbStart (0),
    thumbSize (0),
    initialDelayInMillisecs (100),
    repeatDelayInMillisecs (50),
    minimumDelayInMillisecs (10),
    vertical (vertical_),
    isDraggingThumb (false),
    alwaysVisible (false),
    upButton (0),
    downButton (0),
    listeners (2)
{
  setButtonVisibility (buttonsAreVisible);

  setRepaintsOnMouseActivity (true);
  setFocusContainer (true);
}

AdaptativeScrollBar::~AdaptativeScrollBar()
{
  deleteAllChildren();
}

//==============================================================================
void AdaptativeScrollBar::setRangeLimits (const double newMinimum,
					  const double newMaximum) throw()
{
	if (newMaximum > newMinimum) {
		minimum = newMinimum;
		maximum = newMaximum;

		setCurrentRangeStart (rangeStart);
		updateThumbPosition();
	}
}

void AdaptativeScrollBar::setCurrentRange (double newStart,
					   double newSize) throw()
{
  newSize  = jlimit (0.0, maximum - minimum, newSize);
  newStart = jlimit (minimum, maximum - newSize, newStart);

  if (rangeStart != newStart
      || rangeSize != newSize)
    {
      rangeStart = newStart;
      rangeSize = newSize;

      updateThumbPosition();
      triggerAsyncUpdate();
    }
}

void AdaptativeScrollBar::setCurrentRangeStart (double newStart) throw()
{
  setCurrentRange (newStart, rangeSize);
}

void AdaptativeScrollBar::setSingleStepSize (const double newSingleStepSize) throw()
{
  singleStepSize = newSingleStepSize;
}

void AdaptativeScrollBar::moveScrollbarInSteps (const int howManySteps) throw()
{
  setCurrentRangeStart (rangeStart + howManySteps * singleStepSize);
}

void AdaptativeScrollBar::moveScrollbarInPages (const int howManyPages) throw()
{
  setCurrentRangeStart (rangeStart + howManyPages * rangeSize);
}

void AdaptativeScrollBar::scrollToTop() throw()
{
  setCurrentRangeStart (minimum);
}

void AdaptativeScrollBar::scrollToBottom() throw()
{
  setCurrentRangeStart (maximum - rangeSize);
}

void AdaptativeScrollBar::setButtonRepeatSpeed (const int initialDelayInMillisecs_,
						const int repeatDelayInMillisecs_,
						const int minimumDelayInMillisecs_) throw()
{
  initialDelayInMillisecs = initialDelayInMillisecs_;
  repeatDelayInMillisecs = repeatDelayInMillisecs_;
  minimumDelayInMillisecs = minimumDelayInMillisecs_;

  if (upButton != 0)
    {
      upButton->setRepeatSpeed (initialDelayInMillisecs,  repeatDelayInMillisecs,  minimumDelayInMillisecs);
      downButton->setRepeatSpeed (initialDelayInMillisecs,  repeatDelayInMillisecs,  minimumDelayInMillisecs);
    }
}

//==============================================================================
void AdaptativeScrollBar::addListener (ScrollBarListener* const listener) throw()
{
  if (listener != 0)
    listeners.add (listener);
}

void AdaptativeScrollBar::removeListener (ScrollBarListener* const listener) throw()
{
  listeners.removeValue (listener);
}

void AdaptativeScrollBar::handleAsyncUpdate()
{
  //const double value = getCurrentRangeStart();

  for (int i = listeners.size(); --i >= 0;)
    {
      //BY  ((ScrollBarListener*) listeners.getUnchecked (i))->scrollBarMoved (this, value);
      i = jmin (i, listeners.size());
    }
}

//==============================================================================
void AdaptativeScrollBar::updateThumbPosition() throw()
{
  int newThumbSize = roundDoubleToInt ((maximum > minimum) ? (rangeSize * thumbAreaSize) / (maximum - minimum)
				       : thumbAreaSize);

  //BY if (newThumbSize < getLookAndFeel().getMinimumScrollbarThumbSize (*this))
  //BY newThumbSize = jmin (getLookAndFeel().getMinimumScrollbarThumbSize (*this), thumbAreaSize - 1);

  if (newThumbSize > thumbAreaSize)
    newThumbSize = thumbAreaSize;

  int newThumbStart = thumbAreaStart;

  if (maximum - minimum > rangeSize)
    newThumbStart += roundDoubleToInt (((rangeStart - minimum) * (thumbAreaSize - newThumbSize))
				       / ((maximum - minimum) - rangeSize));

  setVisible (alwaysVisible || (maximum - minimum > rangeSize && rangeSize > 0.0));

  if (thumbStart != newThumbStart  || thumbSize != newThumbSize)
    {
      const int repaintStart = jmin (thumbStart, newThumbStart) - 4;
      const int repaintSize = jmax (thumbStart + thumbSize, newThumbStart + newThumbSize) + 8 - repaintStart;

      if (vertical)
	repaint (0, repaintStart, getWidth(), repaintSize);
      else
	repaint (repaintStart, 0, repaintSize, getHeight());

      thumbStart = newThumbStart;
      thumbSize = newThumbSize;
    }
}

void AdaptativeScrollBar::setOrientation (const bool shouldBeVertical) throw()
{
  if (vertical != shouldBeVertical)
    {
      vertical = shouldBeVertical;

      if (upButton != 0)
        {
	  ((AdaptativeScrollbarButton*) upButton)->direction    = (vertical) ? 0 : 3;
	  ((AdaptativeScrollbarButton*) downButton)->direction  = (vertical) ? 2 : 1;
        }

      updateThumbPosition();
    }
}

void AdaptativeScrollBar::setButtonVisibility (const bool buttonsAreVisible)
{
  if (upButton) {
    deleteAndZero (upButton);
  }
  if (downButton) {
    deleteAndZero (downButton);
  }

  if (buttonsAreVisible)
    {
      addAndMakeVisible (upButton   = new AdaptativeScrollbarButton ((vertical) ? 0 : 3, *this));
      addAndMakeVisible (downButton = new AdaptativeScrollbarButton ((vertical) ? 2 : 1, *this));

      setButtonRepeatSpeed (initialDelayInMillisecs, repeatDelayInMillisecs, minimumDelayInMillisecs);
    }

  updateThumbPosition();
}

void AdaptativeScrollBar::setAutoHide (const bool shouldHideWhenFullRange)
{
  alwaysVisible = ! shouldHideWhenFullRange;
  updateThumbPosition();
}

//==============================================================================
void AdaptativeScrollBar::paint (Graphics& /*g*/)
{
  if (thumbAreaSize > 0)
    {
      //LookAndFeel& lf = getLookAndFeel();

      //BY    const int thumb = (thumbAreaSize > lf.getMinimumScrollbarThumbSize (*this))
      //	? thumbSize : 0;

      if (vertical)
        {
	  /*BY lf.drawScrollbar (g, *this,
	    0, thumbAreaStart,
	    getWidth(), thumbAreaSize,
	    vertical,
	    thumbStart, thumb,
	    isMouseOver(), isMouseButtonDown());
	  */ }
      else
        {
	  /*BY   lf.drawScrollbar (g, *this,
	    thumbAreaStart, 0,
	    thumbAreaSize, getHeight(),
	    vertical,
	    thumbStart, thumb,
	    isMouseOver(), isMouseButtonDown());
	  */}
    }
}

void AdaptativeScrollBar::lookAndFeelChanged()
{
  setComponentEffect (getLookAndFeel().getScrollbarEffect());
}

void AdaptativeScrollBar::resized()
{
  //const int length = ((vertical) ? getHeight() : getWidth());

  //BY  const int buttonSize = (upButton != 0) ? jmin (getLookAndFeel().getAdaptativeScrollbarButtonSize (*this), (length >> 1))	: 0;

  /*BYif (length < 32 + getLookAndFeel().getMinimumScrollbarThumbSize (*this))
    {
    thumbAreaStart = length >> 1;
    thumbAreaSize = 0;
    }
    else
    {
    thumbAreaStart = buttonSize;
    thumbAreaSize = length - (buttonSize << 1);
    }*/

  if (upButton != 0)
    {
      if (vertical)
        {
	  /*BY upButton->setBounds (0, 0, getWidth(), buttonSize);
            downButton->setBounds (0, thumbAreaStart + thumbAreaSize, getWidth(), buttonSize);
	    }
	    else
	    {
            upButton->setBounds (0, 0, buttonSize, getHeight());
            downButton->setBounds (thumbAreaStart + thumbAreaSize, 0, buttonSize, getHeight());
	  */ }
    }

  updateThumbPosition();
}

void AdaptativeScrollBar::mouseDown (const MouseEvent& e)
{
  isDraggingThumb = false;
  lastMousePos = vertical ? e.y : e.x;
  dragStartMousePos = lastMousePos;
  dragStartRange = rangeStart;

  if (dragStartMousePos < thumbStart)
    {
      moveScrollbarInPages (-1);
      startTimer (400);
    }
  else if (dragStartMousePos >= thumbStart + thumbSize)
    {
      moveScrollbarInPages (1);
      startTimer (400);
    }
  else
    {
      //BY isDraggingThumb = (thumbAreaSize > getLookAndFeel().getMinimumScrollbarThumbSize (*this))&& (thumbAreaSize > thumbSize);
    }
}

void AdaptativeScrollBar::mouseDrag (const MouseEvent& e)
{
  if (isDraggingThumb)
    {
      const int deltaPixels = ((vertical) ? e.y : e.x) - dragStartMousePos;

      setCurrentRangeStart (dragStartRange
			    + deltaPixels * ((maximum - minimum) - rangeSize)
			    / (thumbAreaSize - thumbSize));
    }
  else
    {
      lastMousePos = (vertical) ? e.y : e.x;
    }
}

void AdaptativeScrollBar::mouseUp (const MouseEvent&)
{
  isDraggingThumb = false;
  stopTimer();
  repaint();
}

void AdaptativeScrollBar::mouseWheelMove (const MouseEvent&,
					  float wheelIncrementX,
					  float wheelIncrementY)
{
  float increment = vertical ? wheelIncrementY : wheelIncrementX;

  if (increment < 0)
    increment = jmin (increment * 10.0f, -1.0f);
  else if (increment > 0)
    increment = jmax (increment * 10.0f, 1.0f);

  setCurrentRangeStart (rangeStart - singleStepSize * increment);
}

void AdaptativeScrollBar::timerCallback()
{
  if (isMouseButtonDown())
    {
      startTimer (40);

      if (lastMousePos < thumbStart)
	setCurrentRangeStart (rangeStart - rangeSize);
      else if (lastMousePos > thumbStart + thumbSize)
	setCurrentRangeStart (rangeStart + rangeSize);
    }
  else
    {
      stopTimer();
    }
}

bool AdaptativeScrollBar::keyPressed (const KeyPress& key)
{
  if (! isVisible())
    return false;

  if (key.isKeyCode (KeyPress::upKey) || key.isKeyCode (KeyPress::leftKey))
    moveScrollbarInSteps (-1);
  else if (key.isKeyCode (KeyPress::downKey) || key.isKeyCode (KeyPress::rightKey))
    moveScrollbarInSteps (1);
  else if (key.isKeyCode (KeyPress::pageUpKey))
    moveScrollbarInPages (-1);
  else if (key.isKeyCode (KeyPress::pageDownKey))
    moveScrollbarInPages (1);
  else if (key.isKeyCode (KeyPress::homeKey))
    scrollToTop();
  else if (key.isKeyCode (KeyPress::endKey))
    scrollToBottom();
  else
    return false;

  return true;
}

