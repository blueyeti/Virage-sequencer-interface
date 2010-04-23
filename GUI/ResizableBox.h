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

#ifndef _ResizableBox_h_
#define _ResizableBox_h_

#include "includes.h"
#include "TopLevelBox.h"

//==============================================================================
class  ResizableBox  : public TopLevelBox
{


protected:
    void paint (Graphics& g);
    void moved();
    void resized();
    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);
    void lookAndFeelChanged();
    void childBoundsChanged (Component* child);
    void parentSizeChanged();
    void visibilityChanged();
    void activeWindowStatusChanged();
    int getDesktopWindowStyleFlags() const;
	virtual const BorderSize getBorderThickness();
	virtual const BorderSize getContentComponentBorder();

   void addChildComponent (Component* const child, int zOrder = -1);
   void addAndMakeVisible (Component* const child, int zOrder = -1);

    ResizableCornerComponent* resizableCorner;
    ResizableBorderComponent* resizableBorder;

private:
    Component* contentComponent;
    bool resizeToFitContent, fullscreen;
    ComponentDragger dragger;
    Rectangle lastNonFullScreenPos;
    ComponentBoundsConstrainer defaultConstrainer;
    ComponentBoundsConstrainer* constrainer;
#ifdef JUCE_DEBUG
    bool hasBeenResized;
#endif

    void updateLastPos();

    ResizableBox (const ResizableBox&);
    const ResizableBox& operator= (const ResizableBox&);
	void getBorderThickness (int& left, int& top, int& right, int& bottom);
    void getContentComponentBorder (int& left, int& top, int& right, int& bottom);

public:
    //==============================================================================
    ResizableBox (const String& name,
                     const bool addToDesktop);
	ResizableBox (const String& name,
                     const Colour& backgroundColour,
                     const bool addToDesktop);
	virtual ~ResizableBox();

    //==============================================================================
    const Colour getBackgroundColour() const throw();
	void setBackgroundColour (const Colour& newColour);
	void setResizable (const bool shouldBeResizable,
                       const bool useBottomRightCornerResizer);
	bool isResizable() const throw();
	void setResizeLimits (const int newMinimumWidth,
                          const int newMinimumHeight,
                          const int newMaximumWidth,
                          const int newMaximumHeight) throw();
	ComponentBoundsConstrainer* getConstrainer() throw()            { return constrainer; }
	void setConstrainer (ComponentBoundsConstrainer* newConstrainer);
	void setBoundsConstrained (int x, int y, int width, int height);
	bool isFullScreen() const;
	void setFullScreen (const bool shouldBeFullScreen);
	bool isMinimised() const;
	void setMinimised (const bool shouldMinimise);
	const String getWindowStateAsString();
    bool restoreWindowStateFromString (const String& previousState);
	Component* getContentComponent() const throw()                  { return contentComponent; }
	void setContentComponent (Component* const newContentComponent,
                              const bool deleteOldOne = true,
                              const bool resizeToFit = false);

    void setContentComponentSize (int width, int height);
	enum ColourIds
    {
        backgroundColourId          = 0x1005700,  /**< A colour to use to fill the window's background. */
    };

	Colour backgroundColour;
    //==============================================================================
    juce_UseDebuggingNewOperator
};


#endif
