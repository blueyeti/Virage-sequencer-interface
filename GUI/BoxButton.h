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
 *  BoxButton.h
 *  Virage
 *
 *  Created by dev on 22/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "includes.h"

#ifndef _BOXBUTTON_H_
#define _BOXBUTTON_H_

class BoxButton : public TextButton {
	
private:
	Colour m_colour;
	bool m_shouldPassMouseEvent;
public:
	BoxButton(String _label, String _toolTip = T(""), bool shouldPassMouseEvent=false) : TextButton(_label, _toolTip){
		
		setButtonText(_label);
		setWantsKeyboardFocus(false);
		m_shouldPassMouseEvent = shouldPassMouseEvent;
	}
	~BoxButton(){}
	
	void setColour(Colour _colour){ m_colour = _colour; repaint();}
	
	void paint(Graphics& g){
		
		int padding=2;
		int padding_text = 2;
		Colour paint_colour = m_colour;
		if(isMouseOver())					{paint_colour = m_colour.brighter(0.2f); padding_text = 0;}
		if(isDown() || getToggleState() ) 	 paint_colour = m_colour.brighter(0.4f);
		
		g.setColour( paint_colour);
		g.fillRoundedRectangle(padding,padding,getWidth()-2*padding, getHeight()-2*padding, 0);//std::min(4,(int)((getWidth()+getHeight())/2 * 0.2f)));
		
		
		g.setColour(m_colour.contrasting(1.0f));
		
		
		g.drawText(getButtonText(), padding_text, padding_text, getWidth()-2*padding_text, getHeight()-2*padding_text,  Justification::centred,false);
		/*{
			//verrou fermé
			g.drawEllipse(getWidth()/4 + 2, getHeight()/4, getWidth()/2 - 2*2, getHeight()/2, 1);
			g.fillRect(getWidth()/4, getHeight()/2, getWidth()/2, getHeight()*2/5);
		}
		 */
	}
	
	/*
	void mouseMove(const MouseEvent& e){
		
		
		TextButton::mouseMove(e);
		if(!m_shouldPassMouseEvent) return;
		
		getParentComponent()->mouseMove(e);
	}
	
	void mouseDown(const MouseEvent& e){
		
		TextButton::mouseDown(e);
		if(!m_shouldPassMouseEvent) return;
		
		getParentComponent()->mouseDown(e);
	}
	
	void mouseUp(const MouseEvent& e){
		
		TextButton::mouseUp(e);
		if(!m_shouldPassMouseEvent) return;
		
		getParentComponent()->mouseUp(e);
	}
	
	void mouseDrag(const MouseEvent& e){
		
		TextButton::mouseDrag(e);
		if(!m_shouldPassMouseEvent) return;
		
		getParentComponent()->mouseDrag(e);
	}*/
	
};
#endif