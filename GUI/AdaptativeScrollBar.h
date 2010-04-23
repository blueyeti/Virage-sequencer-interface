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
 *  AdaptativeScrollbar.h
 *  Virage
 *
 *  Created by Henry on 28/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

/*
 ==============================================================================
 
 This file is part of the JUCE library - "Jules' Utility Class Extensions"
 Copyright 2004-7 by Raw Material Software ltd.
 
 ------------------------------------------------------------------------------
 
 JUCE can be redistributed and/or modified under the terms of the
 GNU General Public License, as published by the Free Software Foundation;
 either version 2 of the License, or (at your option) any later version.
 
 JUCE is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with JUCE; if not, visit www.gnu.org/licenses or write to the
 Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 Boston, MA 02111-1307 USA
 
 ------------------------------------------------------------------------------
 
 If you'd like to release a closed-source product which uses JUCE, commercial
 licenses are also available: visit www.rawmaterialsoftware.com/juce for
 more information.
 
 ==============================================================================
 */

#ifndef _ADAPTATIVESCROLLBAR_H_
#define _ADAPTATIVESCROLLBAR_H_

#include "includes.h"

class  AdaptativeScrollBar  : public Component,
public AsyncUpdater,
private Timer
{
public:
    //==============================================================================
    /** Creates a Scrollbar.
	 
	 @param isVertical           whether it should be a vertical or horizontal bar
	 @param buttonsAreVisible    whether to show the up/down or left/right buttons
	 */
    AdaptativeScrollBar (const bool isVertical,
               const bool buttonsAreVisible = true);
	
    /** Destructor. */
    ~AdaptativeScrollBar();
	
    //==============================================================================
    /** Returns true if the scrollbar is vertical, false if it's horizontal. */
    bool isVertical() const throw()                                 { return vertical; }
	
    /** Changes the scrollbar's direction.
	 
	 You'll also need to resize the bar appropriately - this just changes its internal
	 layout.
	 
	 @param shouldBeVertical     true makes it vertical; false makes it horizontal.
	 */
    void setOrientation (const bool shouldBeVertical) throw();
	
    /** Shows or hides the scrollbar's buttons. */
    void setButtonVisibility (const bool buttonsAreVisible);
	
    /** Tells the scrollbar whether to make itself invisible when not needed.
	 
	 The default behaviour is for a scrollbar to become invisible when the thumb
	 fills the whole of its range (i.e. when it can't be moved). Setting this
	 value to false forces the bar to always be visible.
	 */
    void setAutoHide (const bool shouldHideWhenFullRange);
	
    //==============================================================================
    /** Sets the minimum and maximum values that the bar will move between.
	 
	 The bar's thumb will always be constrained so that the top of the thumb
	 will be >= minimum, and the bottom of the thumb <= maximum.
	 
	 @see setCurrentRange
	 */
    void setRangeLimits (const double minimum,
                         const double maximum) throw();
	
    /** Returns the lower value that the thumb can be set to.
	 
	 This is the value set by setRangeLimits().
	 */
    double getMinimumRangeLimit() const throw()                     { return minimum; }
	
    /** Returns the upper value that the thumb can be set to.
	 
	 This is the value set by setRangeLimits().
	 */
    double getMaximumRangeLimit() const throw()                     { return maximum; }
	
    //==============================================================================
    /** Changes the position of the scrollbar's 'thumb'.
	 
	 This sets both the position and size of the thumb - to just set the position without
	 changing the size, you can use setCurrentRangeStart().
	 
	 If this method call actually changes the scrollbar's position, it will trigger an
	 asynchronous call to ScrollBarListener::scrollBarMoved() for all the listeners that
	 are registered.
	 
	 @param newStart     the top (or left) of the thumb, in the range
	 getMinimumRangeLimit() <= newStart <= getMaximumRangeLimit(). If the
	 value is beyond these limits, it will be clipped.
	 @param newSize      the size of the thumb, such that
	 getMinimumRangeLimit() <= newStart + newSize <= getMaximumRangeLimit(). If the
	 size is beyond these limits, it will be clipped.
	 @see setCurrentRangeStart, getCurrentRangeStart, getCurrentRangeSize
	 */
    void setCurrentRange (double newStart,
                          double newSize) throw();
	
    /** Moves the bar's thumb position.
	 
	 This will move the thumb position without changing the thumb size. Note
	 that the maximum thumb start position is (getMaximumRangeLimit() - getCurrentRangeSize()).
	 
	 If this method call actually changes the scrollbar's position, it will trigger an
	 asynchronous call to ScrollBarListener::scrollBarMoved() for all the listeners that
	 are registered.
	 
	 @see setCurrentRange
	 */
    void setCurrentRangeStart (double newStart) throw();
	
    /** Returns the position of the top of the thumb.
	 
	 @see setCurrentRangeStart
	 */
    double getCurrentRangeStart() const throw()                     { return rangeStart; }
	
    /** Returns the current size of the thumb.
	 
	 @see setCurrentRange
	 */
    double getCurrentRangeSize() const throw()                      { return rangeSize; }
	
    //==============================================================================
    /** Sets the amount by which the up and down buttons will move the bar.
	 
	 The value here is in terms of the total range, and is added or subtracted
	 from the thumb position when the user clicks an up/down (or left/right) button.
	 */
    void setSingleStepSize (const double newSingleStepSize) throw();
	
    /** Moves the scrollbar by a number of single-steps.
	 
	 This will move the bar by a multiple of its single-step interval (as
	 specified using the setSingleStepSize() method).
	 
	 A positive value here will move the bar down or to the right, a negative
	 value moves it up or to the left.
	 */
    void moveScrollbarInSteps (const int howManySteps) throw();
	
    /** Moves the scroll bar up or down in pages.
	 
	 This will move the bar by a multiple of its current thumb size, effectively
	 doing a page-up or down.
	 
	 A positive value here will move the bar down or to the right, a negative
	 value moves it up or to the left.
	 */
    void moveScrollbarInPages (const int howManyPages) throw();
	
    /** Scrolls to the top (or left).
	 
	 This is the same as calling setCurrentRangeStart (getMinimumRangeLimit());
	 */
    void scrollToTop() throw();
	
    /** Scrolls to the bottom (or right).
	 
	 This is the same as calling setCurrentRangeStart (getMaximumRangeLimit() - getCurrentRangeSize());
	 */
    void scrollToBottom() throw();
	
    /** Changes the delay before the up and down buttons autorepeat when they are held
	 down.
	 
	 For an explanation of what the parameters are for, see Button::setRepeatSpeed().
	 
	 @see Button::setRepeatSpeed
	 */
    void setButtonRepeatSpeed (const int initialDelayInMillisecs,
                               const int repeatDelayInMillisecs,
                               const int minimumDelayInMillisecs = -1) throw();
	
    //==============================================================================
    /** A set of colour IDs to use to change the colour of various aspects of the component.
	 
	 These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
	 methods.
	 
	 @see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
	 */
    enum ColourIds
    {
        backgroundColourId          = 0x1000300,    /**< The background colour of the scrollbar. */
        thumbColourId               = 0x1000400,    /**< A base colour to use for the thumb. The look and feel will probably use variations on this colour. */
        trackColourId               = 0x1000401     /**< A base colour to use for the slot area of the bar. The look and feel will probably use variations on this colour. */
    };
	
    //==============================================================================
    /** Registers a listener that will be called when the scrollbar is moved. */
    void addListener (ScrollBarListener* const listener) throw();
	
    /** Deregisters a previously-registered listener. */
    void removeListener (ScrollBarListener* const listener) throw();
	
    //==============================================================================
    /** @internal */
    bool keyPressed (const KeyPress& key);
    /** @internal */
    void mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY);
    /** @internal */
    void lookAndFeelChanged();
    /** @internal */
    void handleAsyncUpdate();
    /** @internal */
    void mouseDown (const MouseEvent& e);
    /** @internal */
    void mouseDrag (const MouseEvent& e);
    /** @internal */
    void mouseUp   (const MouseEvent& e);
    /** @internal */
    void paint (Graphics& g);
    /** @internal */
    void resized();
	
    juce_UseDebuggingNewOperator
	
private:
    //==============================================================================
    double minimum, maximum;
    double rangeStart, rangeSize;
    double singleStepSize, dragStartRange;
    int thumbAreaStart, thumbAreaSize, thumbStart, thumbSize;
    int dragStartMousePos, lastMousePos;
    int initialDelayInMillisecs, repeatDelayInMillisecs, minimumDelayInMillisecs;
    bool vertical, isDraggingThumb, alwaysVisible;
    Button* upButton;
    Button* downButton;
    SortedSet <void*> listeners;
	
    void updateThumbPosition() throw();
    void timerCallback();
	
    AdaptativeScrollBar (const AdaptativeScrollBar&);
    const AdaptativeScrollBar& operator= (const AdaptativeScrollBar&);
};


#endif   // __JUCE_SCROLLBAR_JUCEHEADER__
