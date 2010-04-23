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
#include "SequencerFrame.h"


//==============================================================================
SequencerFrame::SequencerFrame (int _scrollBarThickness, bool _horizontalSlider)
: m_viewport (0)
{
	m_sequencer = Sequencer::getInstance(this, s_SEQUENCE_MAX_SIZE, s_SEQUENCE_MAX_HEIGHT);
	m_sequencer->addChangeListener(this);

	/*viewport*/
    addAndMakeVisible (m_viewport = new myViewport (_horizontalSlider, m_sequencer));
	m_viewport->setViewedComponent(m_sequencer);
	
	/*map*/
	addAndMakeVisible (m_map = new SequencerMap (m_sequencer));
	m_map->getViseur()->addChangeListener(this);

	/*parametre*/
	m_viewport->setScrollBarThickness(14);
	m_viewport->getVZoomSlider()->setValue(m_viewport->getVZoomSliderValue());

	refresh();
}

SequencerFrame::~SequencerFrame()
{
  if(m_viewport) {
    deleteAndZero( m_viewport );
  }
  if(m_map) {
    deleteAndZero( m_map );
  }
  if(m_sequencer) {
    deleteAndZero( m_sequencer );
  }
}

//==============================================================================
void SequencerFrame::changeListenerCallback(void* objectThatHasChanged){

	if(objectThatHasChanged == m_sequencer)	{
		m_map->getViseur()->repaint();
		m_map->repaint();
		return;
	}
	if(objectThatHasChanged == m_map->getViseur()) {
		refresh();
	}
}

void SequencerFrame::refresh() {

	/*rescale (x,y) de viewport = f( map & viseur )*/
	float newViewXPosition = 1.0f * m_map->getViseur()->getX() * m_sequencer->getWidth()  /  m_map->getWidth();/* on prend le bord gauche */
	float newViewYPosition = 1.0f * m_map->getViseur()->getY()  * m_sequencer->getHeight() / m_map->getHeight();/* on prend le bord haut */
	m_viewport->setViewPosition (newViewXPosition, newViewYPosition);

	float visuWidth = (float)getWidth();
	float totalWidth = (float)m_map->getWidth();
	float currentWidth = (float)m_map->getViseur()->getWidth();
	
	//float visuHeight = (float)(getHeight() - m_transportBar->getHeight() - m_map->getHeight());
	//float totalHeight = (float)m_map->getHeight();
	//float currentHeight = (float)m_map->getViseur()->getHeight();

	/*rescale (w, h) de viewport = f( map & viseur )*/
	float newHorizontalRatio =  1.0f *( (currentWidth / totalWidth) * (m_sequencer->getDuration()) / visuWidth);
	m_sequencer->getZoomer()->setHorizontalRatio(newHorizontalRatio);

//	float newVerticalRatio =  1.0f /( currentHeight / totalHeight  * m_sequencer->getRootHeight() / visuHeight );
	m_sequencer->getZoomer()->setVerticalRatio( 1.0f );
	m_sequencer->refresh();
}
//==============================================================================
void SequencerFrame::paint (Graphics& g)
{
    g.fillAll (Colours::darkgrey);
}

void SequencerFrame::resized()
{
	/*map*/
    m_map->setBounds(0, 0, getWidth(), 80-2);
	/*sequence*/
	m_viewport->setBounds (0, 80+1, proportionOfWidth (1.0000f), proportionOfHeight (1.0000f)-80-1/*-1pour séparer la map de la sequence d'un rang de pixels*/ );
	//m_viewport->getViewedComponent()->setBounds (0, 0, m_viewport->getViewedComponent()->getWidth(), m_viewport->getViewedComponent()->getWidth());
}

Sequencer* SequencerFrame::getConduite() {
	return m_sequencer;
}

myViewport * SequencerFrame::getViewPort() {
	return m_viewport;
}
