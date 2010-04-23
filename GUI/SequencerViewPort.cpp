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

#include "SequencerViewPort.h"

static const int s_viewport_zoom_margin = /*150*/0;

//==============================================================================
SequencerViewPort::SequencerViewPort (const String& componentName)
: Component (componentName),
contentComp (0),
lastVX (0),
lastVY (0),
lastVW (0),
lastVH (0),
scrollBarThickness (0),
singleStepX (16),
singleStepY (16),
showHScrollbar (true),
showVScrollbar (true)
{
    // content holder is used to clip the contents so they don't overlap the scrollbars
    addAndMakeVisible (contentHolder = new Component());
    contentHolder->setInterceptsMouseClicks (false, true);
	
    verticalScrollBar = new ScrollBar (true/*isVertical*/);
    horizontalScrollBar = new ScrollBar (false/*isVertical*/);
	
    addChildComponent (verticalScrollBar);
    addChildComponent (horizontalScrollBar);
	
    verticalScrollBar->addListener (this);
    horizontalScrollBar->addListener (this);
	
	
    setInterceptsMouseClicks (false, true);
    setWantsKeyboardFocus (true);
	
	//custom BY
	//attention la barre de titre ne sera pas zoomé
	vZoomSliderMax = 100.0;
	vZoomSliderMin = 1.0;
	
	/*vertical zoom slider*/
	addAndMakeVisible(vZoomSlider = new Slider(T("")));
	vZoomSlider->addListener(this);
    vZoomSlider->setRange (0, 10, 0);
    vZoomSlider->setSliderStyle (Slider::LinearVertical);
    vZoomSlider->setTextBoxStyle (Slider::TextBoxLeft, true, 0, 0);
	vZoomSlider->setRange(vZoomSliderMin, vZoomSliderMax, 1.0);
	vZoomSlider->setEnabled(true);
	
	addAndMakeVisible(vMinus = new TextButton(T("-")));
	vMinus->addButtonListener(this);
	vMinus->setEnabled(true);
	addAndMakeVisible(vPlus = new TextButton(T("+")));
	vPlus->addButtonListener(this);
	vPlus->setEnabled(true);
	
	/*horizontal zoom slider*/
	addAndMakeVisible(hZoom = new Slider(T("")));
	hZoom->addListener(this);
	hZoom->setEnabled(true);
	hZoom->setRange (0, 10, 0);
	hZoom->setValue(5, false, false);
	hZoom->setSliderStyle (Slider::LinearHorizontal);
	hZoom->setTextBoxStyle (Slider::TextBoxLeft, true, 0, 20);
	
	addAndMakeVisible(hMinus = new TextButton(T("-")));
	hMinus->addButtonListener(this);
	hMinus->setEnabled(true);	
	addAndMakeVisible(hPlus = new TextButton(T("+")));
	hPlus->addButtonListener(this);
	hPlus->setEnabled(true);
	//fin custom BY
}

SequencerViewPort::~SequencerViewPort()
{
    contentHolder->deleteAllChildren();
    deleteAllChildren();
}

//==============================================================================

void SequencerViewPort::visibleAreaChanged(int visibleX, int visibleY, int visibleW, int visibleH){
}

double SequencerViewPort::getHZoomSliderValue(){
	return hZoom->getValue();
}

double SequencerViewPort::getVZoomSliderValue(){
	return vZoomSlider->getValue();
}

Slider* SequencerViewPort::getVZoomSlider(){
	return vZoomSlider;
}


///VERTICAL SLIDER LISTENER
void SequencerViewPort::buttonClicked (Button* buttonThatWasClicked)
{
	if(buttonThatWasClicked == vMinus) vZoomSlider->setValue( (vZoomSlider->getValue()) + 10, true, true) ;
	if(buttonThatWasClicked == vPlus)  vZoomSlider->setValue( (vZoomSlider->getValue()) - 10, true, true) ;
}

void SequencerViewPort::sliderValueChanged (Slider *slider){
	if(slider == vZoomSlider) zoomSliderChanged();
}


//==============================================================================
void SequencerViewPort::setViewedComponent (Component* const newViewedComponent)
{
    if (contentComp != newViewedComponent)
    {
        if (contentComp->isValidComponent())
        {
            Component* const oldComp = contentComp;
            contentComp = 0;
            delete oldComp;
        }
		
        contentComp = newViewedComponent;
		
        if (contentComp != 0)
        {
            contentComp->setTopLeftPosition (0, 0);
            contentHolder->addAndMakeVisible (contentComp);
            contentComp->addComponentListener (this);
        }
		
        updateVisibleRegion();
    }
}

int SequencerViewPort::getMaximumVisibleWidth() const throw()
{
    return jmax (0, getWidth() - (verticalScrollBar->isVisible() ? getScrollBarThickness() : 0));
}

int SequencerViewPort::getMaximumVisibleHeight() const throw()
{
    return jmax (0, getHeight() - (horizontalScrollBar->isVisible() ? getScrollBarThickness() : 0));
}

void SequencerViewPort::setViewPosition (const int xPixelsOffset,
										 const int yPixelsOffset)
{
    if (contentComp != 0)
        contentComp->setTopLeftPosition (-xPixelsOffset,
                                         -yPixelsOffset);
}

void SequencerViewPort::setViewPositionProportionately (const double x,
														const double y)
{
    if (contentComp != 0)
        setViewPosition (jmax (0, roundDoubleToInt (x * (contentComp->getWidth() - getWidth()))),
                         jmax (0, roundDoubleToInt (y * (contentComp->getHeight() - getHeight()))));
}

void SequencerViewPort::componentMovedOrResized (Component&, bool, bool)
{
    updateVisibleRegion();
}

void SequencerViewPort::resized()
{
	//custom BY
//	hZoom->setBounds(15, getMaximumVisibleHeight(), s_viewport_zoom_margin-30, getScrollBarThickness());
//	hMinus->setBounds(0, getMaximumVisibleHeight(), 15, getScrollBarThickness());
//	hPlus->setBounds(s_viewport_zoom_margin-15, getMaximumVisibleHeight(), 15, getScrollBarThickness());
	
//	vZoomSlider->setBounds(getMaximumVisibleWidth(), 15, getScrollBarThickness(), s_viewport_zoom_margin-30);
//	vMinus->setBounds(getMaximumVisibleWidth(), 0, getScrollBarThickness(), 15);
//	vPlus->setBounds(getMaximumVisibleWidth(), s_viewport_zoom_margin-15, getScrollBarThickness(), 15);
	
    updateVisibleRegion();
}

//==============================================================================
void SequencerViewPort::updateVisibleRegion()
{
    if (contentComp != 0)
    {
        const int newVX = -contentComp->getX();
        const int newVY = -contentComp->getY();
		
        if (newVX == 0 && newVY == 0
            && contentComp->getWidth() <= getWidth()
            && contentComp->getHeight() <= getHeight())
        {
            horizontalScrollBar->setVisible (false);
            verticalScrollBar->setVisible (false);
        }
		
        if ((contentComp->getWidth() > 0) && showHScrollbar
			&& getHeight() > getScrollBarThickness())
        {
            horizontalScrollBar->setRangeLimits (0.0, contentComp->getWidth());
            horizontalScrollBar->setCurrentRange (newVX, getMaximumVisibleWidth());
            horizontalScrollBar->setSingleStepSize (singleStepX);
        }
        else
        {
            horizontalScrollBar->setVisible (false);
        }
		
        if ((contentComp->getHeight() > 0) && showVScrollbar
			&& getWidth() > getScrollBarThickness())
        {
            verticalScrollBar->setRangeLimits (0.0, contentComp->getHeight());
            verticalScrollBar->setCurrentRange (newVY, getMaximumVisibleHeight());
            verticalScrollBar->setSingleStepSize (singleStepY);
        }
        else
        {
            verticalScrollBar->setVisible (false);
        }
		
        if (verticalScrollBar->isVisible())
        {
            horizontalScrollBar->setCurrentRange (newVX, getMaximumVisibleWidth());
            verticalScrollBar->setCurrentRange (newVY, getMaximumVisibleHeight());
			
            verticalScrollBar
			->setBounds (getMaximumVisibleWidth() ,  s_viewport_zoom_margin/*BY custom*/,
						 getScrollBarThickness(), getMaximumVisibleHeight()-s_viewport_zoom_margin/*BY custom*/);
        }
		
        if (horizontalScrollBar->isVisible())
        {
            horizontalScrollBar->setCurrentRange (newVX, getMaximumVisibleWidth());
			
            horizontalScrollBar
			->setBounds ( s_viewport_zoom_margin, getMaximumVisibleHeight(),
						 getMaximumVisibleWidth()-s_viewport_zoom_margin, getScrollBarThickness());
        }
		
        contentHolder->setSize (getMaximumVisibleWidth(),
                                getMaximumVisibleHeight());
		
        const int newVW = jmin (contentComp->getRight(),  getMaximumVisibleWidth());
        const int newVH = jmin (contentComp->getBottom(), getMaximumVisibleHeight());
		
        if (newVX != lastVX
			|| newVY != lastVY
			|| newVW != lastVW
			|| newVH != lastVH)
        {
            lastVX = newVX;
            lastVY = newVY;
            lastVW = newVW;
            lastVH = newVH;
			
            visibleAreaChanged (newVX, newVY, newVW, newVH);
        }
		
        horizontalScrollBar->handleUpdateNowIfNeeded();
        verticalScrollBar->handleUpdateNowIfNeeded();
    }
    else
    {
        horizontalScrollBar->setVisible (false);
        verticalScrollBar->setVisible (false);
    }
}

//==============================================================================
void SequencerViewPort::setSingleStepSizes (const int stepX,
											const int stepY)
{
    singleStepX = stepX;
    singleStepY = stepY;
    updateVisibleRegion();
}

void SequencerViewPort::setScrollBarsShown (const bool showVerticalScrollbarIfNeeded,
											const bool showHorizontalScrollbarIfNeeded)
{
    showVScrollbar = showVerticalScrollbarIfNeeded;
    showHScrollbar = showHorizontalScrollbarIfNeeded;
    updateVisibleRegion();
}

void SequencerViewPort::setScrollBarThickness (const int thickness)
{
    scrollBarThickness = thickness;
    updateVisibleRegion();
}

int SequencerViewPort::getScrollBarThickness() const throw()
{
    return (scrollBarThickness > 0) ? scrollBarThickness
	: getLookAndFeel().getDefaultScrollbarWidth();
}

void SequencerViewPort::setScrollBarButtonVisibility (const bool buttonsVisible)
{
    verticalScrollBar->setButtonVisibility (buttonsVisible);
    horizontalScrollBar->setButtonVisibility (buttonsVisible);
}

void SequencerViewPort::scrollBarMoved (ScrollBar* scrollBarThatHasMoved, const double newRangeStart)
{
    if (scrollBarThatHasMoved == horizontalScrollBar)
    {
        setViewPosition (roundDoubleToInt (newRangeStart), getViewPositionY());
    }
    else if (scrollBarThatHasMoved == verticalScrollBar)
    {
        setViewPosition (getViewPositionX(), roundDoubleToInt (newRangeStart));
    }
}

void SequencerViewPort::mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY)
{
    if (! useMouseWheelMoveIfNeeded (e, wheelIncrementX, wheelIncrementY))
        Component::mouseWheelMove (e, wheelIncrementX, wheelIncrementY);
}

bool SequencerViewPort::useMouseWheelMoveIfNeeded (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY)
{
    if (! (e.mods.isAltDown() || e.mods.isCtrlDown()))
    {
        const bool hasVertBar = verticalScrollBar->isVisible();
        const bool hasHorzBar = horizontalScrollBar->isVisible();
		
        if (hasHorzBar && (wheelIncrementX != 0 || e.mods.isShiftDown() || ! hasVertBar))
        {
            horizontalScrollBar->mouseWheelMove (e.getEventRelativeTo (horizontalScrollBar),
                                                 wheelIncrementX, wheelIncrementY);
            return true;
        }
        else if (hasVertBar && wheelIncrementY != 0)
        {
            verticalScrollBar->mouseWheelMove (e.getEventRelativeTo (verticalScrollBar),
                                               wheelIncrementX, wheelIncrementY);
            return true;
        }
    }
	
    return false;
}

bool SequencerViewPort::keyPressed (const KeyPress& key)
{
    const bool isUpDownKey = key.isKeyCode (KeyPress::upKey)
	|| key.isKeyCode (KeyPress::downKey)
	|| key.isKeyCode (KeyPress::pageUpKey)
	|| key.isKeyCode (KeyPress::pageDownKey)
	|| key.isKeyCode (KeyPress::homeKey)
	|| key.isKeyCode (KeyPress::endKey);
	
    if (verticalScrollBar->isVisible() && isUpDownKey)
        return verticalScrollBar->keyPressed (key);
	
    const bool isLeftRightKey = key.isKeyCode (KeyPress::leftKey)
	|| key.isKeyCode (KeyPress::rightKey);
	
    if (horizontalScrollBar->isVisible() && (isUpDownKey || isLeftRightKey))
        return horizontalScrollBar->keyPressed (key);
	
    return false;
}


