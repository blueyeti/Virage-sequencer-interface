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
 
 This is an automatically generated file created by the Jucer!
 
 Creation date:  19 Sep 2008 4:45:16 pm
 
 Be careful when adding custom code to these files, as only the code within
 the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
 and re-saved.
 
 Jucer version: 1.11
 
 ------------------------------------------------------------------------------
 
 The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
 Copyright 2004-6 by Raw Material Software ltd.
 
 ==============================================================================
 */

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "SequencerJucered.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
SequencerJucered::SequencerJucered ()
: viewport (0)
{
    addAndMakeVisible (viewport = new Viewport (T("SeqWindow")));
	
	
    //[UserPreSize]
    //[/UserPreSize]
	
    setSize (200, 200);
	
	
    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SequencerJucered::~SequencerJucered()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]
	
  if (viewport ) {
    deleteAndZero (viewport);
  }
	
    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SequencerJucered::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]
	
    g.fillAll (Colours::white);
	
    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SequencerJucered::resized()
{
    viewport->setBounds (0, 0, getWidth(),getHeight());
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --
 
 This is where the Jucer puts all of its metadata, so don't change anything in here!
 
 BEGIN_JUCER_METADATA
 
 <JUCER_COMPONENT documentType="Component" className="SequencerJucered" componentName=""
 parentClasses="public Component" constructorParams="" variableInitialisers=""
 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
 fixedSize="0" initialWidth="200" initialHeight="200">
 <BACKGROUND backgroundColour="ffffffff"/>
 <VIEWPORT name="SeqWindow" id="96ded27e81bcaa72" memberName="viewport"
 virtualName="" explicitFocusOrder="0" pos="0 0 100% 100%" vscroll="1"
 hscroll="1" scrollbarThickness="18" contentType="0" jucerFile=""
 contentClass="SeqGLSpace" constructorParams=""/>
 </JUCER_COMPONENT>
 
 END_JUCER_METADATA
 */
#endif
