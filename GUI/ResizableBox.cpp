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
#include "ResizableBox.h"
//==============================================================================
ResizableBox::ResizableBox (const String& name,
                                  const Colour& backgroundColour_,
                                  const bool addToDesktop_)
: TopLevelBox (name, addToDesktop_),
resizableCorner (0),
resizableBorder (0),
contentComponent (0),
resizeToFitContent (false),
fullscreen (false),
constrainer (0),
#ifdef JUCE_DEBUG
  hasBeenResized (false),
#endif
  backgroundColour(Colours::black)
{
    setBackgroundColour (backgroundColour_);

    defaultConstrainer.setMinimumOnscreenAmounts (0x10000, 16, 24, 16);

    lastNonFullScreenPos.setBounds (50, 50, 256, 256);

    if (addToDesktop_)
        Component::addToDesktop (getDesktopWindowStyleFlags());
}

ResizableBox::~ResizableBox()
{
  if (resizableCorner) {
    deleteAndZero (resizableCorner);
  }
  if (resizableBorder) {
    deleteAndZero (resizableBorder);
  }
  if (contentComponent) {
    deleteAndZero (contentComponent);
  }

    // have you been adding your own components directly to this window..? tut tut tut.
    // Read the instructions for using a ResizableBox!
}

int ResizableBox::getDesktopWindowStyleFlags() const
{
    int flags = TopLevelBox::getDesktopWindowStyleFlags();

    if (isResizable() && (flags & ComponentPeer::windowHasTitleBar) != 0)
        flags |= ComponentPeer::windowIsResizable;

    return flags;
}

//==============================================================================
void ResizableBox::setContentComponent (Component* const newContentComponent,
                                           const bool deleteOldOne,
                                           const bool resizeToFit)
{
    resizeToFitContent = resizeToFit;

    if (contentComponent != newContentComponent)
    {
        if (deleteOldOne)
            delete contentComponent;
        else
            removeChildComponent (contentComponent);

        contentComponent = newContentComponent;

        Component::addAndMakeVisible (contentComponent);
    }

    if (resizeToFit)
        childBoundsChanged (contentComponent);

    resized(); // must always be called to position the new content comp
}

void ResizableBox::setContentComponentSize (int width, int height)
{
	if (width > 0 || height > 0){ // not a great idea to give it a zero size..

		const BorderSize border (getContentComponentBorder());

		setSize (width + border.getLeftAndRight(),
				height + border.getTopAndBottom());
	}
}

const BorderSize ResizableBox::getBorderThickness()
{
    return BorderSize (isUsingNativeTitleBar() ? 0 : ((resizableBorder != 0 && ! isFullScreen()) ? 5 : 3));
}

const BorderSize ResizableBox::getContentComponentBorder()
{
    return getBorderThickness();
}

void ResizableBox::moved()
{
    updateLastPos();
}

void ResizableBox::visibilityChanged()
{
    TopLevelBox::visibilityChanged();

    updateLastPos();
}

void ResizableBox::resized()
{
    if (resizableBorder != 0)
    {
        resizableBorder->setVisible (! isFullScreen());
        resizableBorder->setBorderThickness (getBorderThickness());

        resizableBorder->setSize (getWidth(), getHeight());
        resizableBorder->toBack();
    }

    if (resizableCorner != 0)
    {
        resizableCorner->setVisible (! isFullScreen());

        const int resizerSize = 18;
        resizableCorner->setBounds (getWidth() - resizerSize,
                                    getHeight() - resizerSize,
                                    resizerSize, resizerSize);
    }

    if (contentComponent != 0)
        contentComponent->setBoundsInset (getContentComponentBorder());

    updateLastPos();

#ifdef JUCE_DEBUG
    hasBeenResized = true;
#endif
}

void ResizableBox::childBoundsChanged (Component* child)
{
    if ((child == contentComponent) && (child != 0) && resizeToFitContent)
    {
        // not going to look very good if this component has a zero size..
    	if (child->getWidth() > 0 || child->getHeight() > 0) {
    		const BorderSize borders (getContentComponentBorder());

        setSize (child->getWidth() + borders.getLeftAndRight(),
                 child->getHeight() + borders.getTopAndBottom());
    	}
    }
}


//==============================================================================
void ResizableBox::activeWindowStatusChanged()
{
    const BorderSize borders (getContentComponentBorder());

    repaint (0, 0, getWidth(), borders.getTop());
    repaint (0, borders.getTop(), borders.getLeft(), getHeight() - borders.getBottom() - borders.getTop());
    repaint (0, getHeight() - borders.getBottom(), getWidth(), borders.getBottom());
    repaint (getWidth() - borders.getRight(), borders.getTop(), borders.getRight(), getHeight() - borders.getBottom() - borders.getTop());
}

//==============================================================================
void ResizableBox::setResizable (const bool shouldBeResizable,
                                    const bool useBottomRightCornerResizer)
{
    if (shouldBeResizable)
    {
        if (useBottomRightCornerResizer)
        {
	  if (resizableBorder) {
	    deleteAndZero (resizableBorder);
	  }

            if (resizableCorner == 0)
            {
                Component::addChildComponent (resizableCorner = new ResizableCornerComponent (this, constrainer));
                resizableCorner->setAlwaysOnTop (true);
            }
        }
        else
        {
	  if (resizableCorner) {
            deleteAndZero (resizableCorner);
	  }

            if (resizableBorder == 0)
                Component::addChildComponent (resizableBorder = new ResizableBorderComponent (this, constrainer));
        }
    }
    else
    {
      if (resizableCorner) {
	deleteAndZero (resizableCorner);
      }
      if (resizableBorder) {
	deleteAndZero (resizableBorder);
      }
    }

    if (isUsingNativeTitleBar())
        recreateDesktopWindow();

    childBoundsChanged (contentComponent);
    resized();
}

bool ResizableBox::isResizable() const throw()
{
    return resizableCorner != 0
	|| resizableBorder != 0;
}

void ResizableBox::setResizeLimits (const int newMinimumWidth,
                                       const int newMinimumHeight,
                                       const int newMaximumWidth,
                                       const int newMaximumHeight) throw()
{
	// if you've set up a custom constrainer then these settings won't have any effect..
	if (constrainer == &defaultConstrainer || constrainer == 0) {

		if (constrainer == 0)
			setConstrainer (&defaultConstrainer);

		defaultConstrainer.setSizeLimits (newMinimumWidth, newMinimumHeight,
				newMaximumWidth, newMaximumHeight);

		setBoundsConstrained (getX(), getY(), getWidth(), getHeight());
	}
}

void ResizableBox::setConstrainer (ComponentBoundsConstrainer* newConstrainer)
{
    if (constrainer != newConstrainer)
    {
        constrainer = newConstrainer;

        const bool useBottomRightCornerResizer = resizableCorner != 0;
        const bool shouldBeResizable = useBottomRightCornerResizer || resizableBorder != 0;

      if (resizableCorner) {
	deleteAndZero (resizableCorner);
      }
      if (resizableBorder) {
	deleteAndZero (resizableBorder);
      }

        setResizable (shouldBeResizable, useBottomRightCornerResizer);

        ComponentPeer* const peer = getPeer();
        if (peer != 0)
            peer->setConstrainer (newConstrainer);
    }
}

void ResizableBox::setBoundsConstrained (int x, int y, int w, int h)
{
    if (constrainer != 0)
        constrainer->setBoundsForComponent (this, x, y, w, h, false, false, false, false);
    else
        setBounds (x, y, w, h);
}

//==============================================================================
void ResizableBox::paint (Graphics& /*g*/)
{
    //g.fillAll (backgroundColour);

    if (! isFullScreen())
    {
   //     getLookAndFeel().drawResizableBoxBorder (g, getWidth(), getHeight(),
     //HENRY                                               getBorderThickness(), *this);
    }

#ifdef JUCE_DEBUG
    if (!(hasBeenResized || (getWidth() == 0 && getHeight() == 0))) {
    	std::cerr << "ResizableBox::paint : non retaille ou taille nulle" << std::endl;
    }
#endif
}

void ResizableBox::lookAndFeelChanged()
{
    resized();

    if (isOnDesktop())
    {
        Component::addToDesktop (getDesktopWindowStyleFlags());

        ComponentPeer* const peer = getPeer();
        if (peer != 0)
            peer->setConstrainer (constrainer);
    }
}

void ResizableBox::setBackgroundColour (const Colour& newColour)
{
    if (Desktop::canUseSemiTransparentWindows())
        backgroundColour = newColour;
    else
        backgroundColour = newColour.withAlpha (1.0f);

    setOpaque (backgroundColour.isOpaque());
    repaint();
}

//==============================================================================
bool ResizableBox::isFullScreen() const
{
    if (isOnDesktop())
    {
        ComponentPeer* const peer = getPeer();
        return peer != 0 && peer->isFullScreen();
    }

    return fullscreen;
}

void ResizableBox::setFullScreen (const bool shouldBeFullScreen)
{
    if (shouldBeFullScreen != isFullScreen())
    {
        updateLastPos();
        fullscreen = shouldBeFullScreen;

        if (isOnDesktop())
        {
            ComponentPeer* const peer = getPeer();

            if (peer != 0)
            {
                // keep a copy of this intact in case the real one gets messed-up while we're un-maximising
                const Rectangle lastPos (lastNonFullScreenPos);

                peer->setFullScreen (shouldBeFullScreen);

                if (! shouldBeFullScreen)
                    setBounds (lastPos);
            }
            else
            {
                std::cerr << "ResizableBox::setFullScreen : ComponentPeer NULL" << std::endl;
            }
        }
        else
        {
            if (shouldBeFullScreen)
                setBounds (0, 0, getParentWidth(), getParentHeight());
            else
                setBounds (lastNonFullScreenPos);
        }

        resized();
    }
}

bool ResizableBox::isMinimised() const
{
    ComponentPeer* const peer = getPeer();

    return (peer != 0) && peer->isMinimised();
}

void ResizableBox::setMinimised (const bool shouldMinimise)
{
    if (shouldMinimise != isMinimised())
    {
        ComponentPeer* const peer = getPeer();

        if (peer != 0)
        {
            updateLastPos();
            peer->setMinimised (shouldMinimise);
        }
        else
        {
        	std::cerr << "ResizableBox::setMinimized : ComponentPeer NULL" << std::endl;
        }
    }
}

void ResizableBox::updateLastPos()
{
    if (isShowing() && ! (isFullScreen() || isMinimised()))
    {
        lastNonFullScreenPos = getBounds();
    }
}

void ResizableBox::parentSizeChanged()
{
    if (isFullScreen() && getParentComponent() != 0)
    {
        setBounds (0, 0, getParentWidth(), getParentHeight());
    }
}

//==============================================================================
const String ResizableBox::getWindowStateAsString()
{
    updateLastPos();

    String s;

    if (isFullScreen())
        s << "fs ";

    s << lastNonFullScreenPos.getX() << T(' ')
	<< lastNonFullScreenPos.getY() << T(' ')
	<< lastNonFullScreenPos.getWidth() << T(' ')
	<< lastNonFullScreenPos.getHeight();

    return s;
}

bool ResizableBox::restoreWindowStateFromString (const String& s)
{
    StringArray tokens;
    tokens.addTokens (s, false);
    tokens.removeEmptyStrings();
    tokens.trim();

    const bool fs = tokens[0].startsWithIgnoreCase (T("fs"));
    const int n = fs ? 1 : 0;

    if (tokens.size() != 4 + n)
        return false;

    Rectangle r (tokens[n].getIntValue(),
                 tokens[n + 1].getIntValue(),
                 tokens[n + 2].getIntValue(),
                 tokens[n + 3].getIntValue());

    if (r.isEmpty())
        return false;

    const Rectangle screen (Desktop::getInstance().getMonitorAreaContaining (r.getX(), r.getY()));

    r = r.getIntersection (screen);

    lastNonFullScreenPos = r;

    if (isOnDesktop())
    {
        ComponentPeer* const peer = getPeer();

        if (peer != 0)
            peer->setNonFullScreenBounds (r);
    }

    setFullScreen (fs);

    if (! fs)
        setBoundsConstrained (r.getX(),
                              r.getY(),
                              r.getWidth(),
                              r.getHeight());

    return true;
}

//==============================================================================
void ResizableBox::mouseDown (const MouseEvent&)
{
    if (! isFullScreen())
        dragger.startDraggingComponent (this, constrainer);
}

void ResizableBox::mouseDrag (const MouseEvent& e)
{
    if (! isFullScreen())
        dragger.dragComponent (this, e);
}

//==============================================================================
void ResizableBox::addChildComponent (Component* const child, int zOrder)
{
		Component::addChildComponent (child, zOrder);
}

void ResizableBox::addAndMakeVisible (Component* const child, int zOrder)
{
    Component::addAndMakeVisible (child, zOrder);
}
