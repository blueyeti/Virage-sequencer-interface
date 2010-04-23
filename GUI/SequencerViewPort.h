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
Henry Bernard from Juce::ViewPort class
 */

#ifndef _SequencerViewPort_H_
#define _SequencerViewPort_H_

#include "includes.h"
#include "AdaptativeScrollBar.h"

//==============================================================================
class SequencerViewPort  : public Component,
private ComponentListener,
private ScrollBarListener,
private SliderListener,
public ButtonListener
{
public:
    //==============================================================================
    /** Creates a SequencerViewPort.
	 
	 The SequencerViewPort is initially empty - use the setViewedComponent() method to
	 add a child component for it to manage.
	 */
    SequencerViewPort (const String& componentName = String::empty);
	
    /** Destructor. */
    ~SequencerViewPort();
	
    void setViewedComponent (Component* const newViewedComponent);
	Component* getViewedComponent() const throw()                   { return contentComp; }
	Slider* getVZoomSlider();
    void setViewPosition (const int xPixelsOffset,
                          const int yPixelsOffset);
	void setViewPositionProportionately (const double proportionX,
                                         const double proportionY);
	
    int getViewPositionX() const throw()                    { return lastVX; }
	int getViewPositionY() const throw()                    { return lastVY; }
	int getViewWidth() const throw()                        { return lastVW; }
	int getViewHeight() const throw()                       { return lastVH; }
	int getMaximumVisibleWidth() const throw();
	int getMaximumVisibleHeight() const throw();
	virtual void visibleAreaChanged (int visibleX, int visibleY,
                                     int visibleW, int visibleH);
	
    //==============================================================================
    void setScrollBarsShown (const bool showVerticalScrollbarIfNeeded,
                             const bool showHorizontalScrollbarIfNeeded);
	bool isVerticalScrollBarShown() const throw()               { return showVScrollbar; }
	bool isHorizontalScrollBarShown() const throw()             { return showHScrollbar; }
	void setScrollBarThickness (const int thickness);
	int getScrollBarThickness() const throw();
	void setSingleStepSizes (const int stepX, const int stepY);
	void setScrollBarButtonVisibility (const bool buttonsVisible);
	ScrollBar* getVerticalScrollBar() const throw()             { return verticalScrollBar; }
	ScrollBar* getHorizontalScrollBar() const throw()           { return horizontalScrollBar; }
	
	
    //==============================================================================
    juce_UseDebuggingNewOperator
	
    /** @internal */
    void resized();
    void scrollBarMoved (ScrollBar* scrollBarThatHasMoved, const double newRangeStart);
    //custom BY
	/** @internal */
	void 	sliderValueChanged (Slider *slider);
    virtual void zoomSliderChanged() = 0;
	double getHZoomSliderValue();
	double getVZoomSliderValue();
	void buttonClicked (Button* buttonThatWasClicked);	
	//end custom BY
	/** @internal */
    void mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY);
    bool keyPressed (const KeyPress& key);
    void componentMovedOrResized (Component& component, bool wasMoved, bool wasResized);
    bool useMouseWheelMoveIfNeeded (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY);
	
private:	
	//custom BY
	Slider* hZoom;
	TextButton* hMinus;
	TextButton* hPlus;
	Slider* vZoomSlider;
	TextButton* vMinus;
	TextButton* vPlus;
	double vZoomSliderMax;
	double vZoomSliderMin;
	//fin Custom BY
	
    Component* contentComp;
    int lastVX, lastVY, lastVW, lastVH;
    int scrollBarThickness;
    int singleStepX, singleStepY;
    bool showHScrollbar, showVScrollbar;
    Component* contentHolder;
    ScrollBar* verticalScrollBar;
    ScrollBar* horizontalScrollBar;
		
    void updateVisibleRegion();
    SequencerViewPort (const SequencerViewPort&);
    const SequencerViewPort& operator= (const SequencerViewPort&);
};


#endif   // __JUCE_SequencerViewPort_JUCEHEADER__
