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

#ifndef __JUCER_HEADER_SequencerJucered_SequencerJucered_8AB3187A__
#define __JUCER_HEADER_SequencerJucered_SequencerJucered_8AB3187A__

//[Headers]     -- You can add your own extra header files here --
#include "includes.h"
//[/Headers]



//==============================================================================
/**
 //[Comments]
 An auto-generated component, created by the Jucer.
 Describe your class and how it works here!
 //[/Comments]
 */
class SequencerJucered  : public Component
	{
	public:
		//==============================================================================
		SequencerJucered();
		~SequencerJucered();
		
		//==============================================================================
		//[UserMethods]     -- You can add your own custom methods in this section.
		//[/UserMethods]
		
		/*JUCE*/void paint (Graphics& g);
		/*JUCE*/void resized();
		
		
		//==============================================================================
		juce_UseDebuggingNewOperator
	protected:
		//[UserVariables]   -- You can add your own custom variables in this section.
		//[/UserVariables]
		
		//==============================================================================
		Viewport* viewport;
		
		//==============================================================================
		// (prevent copy constructor and operator= being generated..)
		SequencerJucered (const SequencerJucered&);
		const SequencerJucered& operator= (const SequencerJucered&);
	};


#endif   // __JUCER_HEADER_SequencerJucered_SequencerJucered_8AB3187A__
