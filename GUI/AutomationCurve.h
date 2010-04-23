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
 *  AutomationCurve.h
 *  Virage
 *
 *  Created by dev on 29/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _AUTOMATIONCURVE_H_
#define _AUTOMATIONCURVE_H_

#define POINT_SIZE 7
#define LINE_THICKNESS 1

#include "includes.h"
#include "TimeBoxContentComponent.h"
#include "TimeBox.h"
#include "Sequencer.h"
#include "Engines.hpp"
#include <algorithm>

class AutomationCurve : public Component {

protected:
	std::map<int,int> points; // stockage des coordonnées en pourcentage pour l'instant
	Engines* m_engines;
	unsigned int m_id_relative;
#ifdef VDBG
	int updcube;
#endif

public:
	AutomationCurve(int _id, Engines* _ptb, int startValue, int endValue) : m_engines(_ptb), m_id_relative(_id)
	{
		points[0] = startValue;//s_SCALE_LENGTH;
		points[s_SCALE_LENGTH] = endValue;//0;
	}

	~AutomationCurve() {
		//delete m_parent;w
	}

	std::map<int,int> getPoints(){ return points;}

	void setPoints(std::map<int,int> _newPoints){
		points = _newPoints;
		repaint();
	}

	inline void setStartValue (int startValue) {
		points[0] = startValue;
		repaint();
	}

	inline void setEndValue (int endValue) {
		points[s_SCALE_LENGTH] = endValue;
		repaint();
	}

	void mouseDoubleClick(const MouseEvent& e) {
		getParentComponent()->mouseDoubleClick(e.getEventRelativeTo(getParentComponent()));
	}

	void paint(Graphics& g) {

		int x = 0;
		int y = 0;
		int w = getWidth();
		int h = getHeight();

		//TRAME
		g.setColour(Colours::lightgrey.withAlpha(0.2f));
		g.fillRect(x, y, w, h);
		g.setColour(Colours::white.withAlpha(0.2f));
		for(int i=x; i<w+x; i+=5) {
			g.drawLine(i, y, i, h, LINE_THICKNESS);
		}
		for(int j=y; j<h; j+=5) {
			g.drawLine(x, j, w+x, j, LINE_THICKNESS);
		}


		//Courbe
		g.setColour(Colours::black);

		std::map<int,int>::iterator it = points.begin();
		while(it != points.end()) {

			int xi = it->first;
			int yi = it->second;

			if(++it != points.end()) {

				int xf = it->first;
				int yf = it->second;

				g.drawLine(xi*getWidth()/s_SCALE_LENGTH, (s_SCALE_LENGTH - yi)*getHeight()/s_SCALE_LENGTH, xf*getWidth()/s_SCALE_LENGTH, (s_SCALE_LENGTH - yf)*getHeight()/s_SCALE_LENGTH, 2);
				//points
				//g.fillEllipse(xi*getWidth()/s_SCALE_LENGTH - POINT_SIZE/2, yi*getHeight()/s_SCALE_LENGTH - POINT_SIZE/2, POINT_SIZE, POINT_SIZE);
			}
		}
#ifdef VDBG//appel d'updated()
		g.setColour(Colours::red);
		g.drawText(String("updated() calls:"), 0,0,100,10,Justification::left, false);
		for(int i=0; i<updcube; i++){

			g.fillRect(i*4+100, 0, 3, 3);
		}
#endif

		//repere pour Oliv'
		g.setColour(Colours::black.withAlpha(0.3f));
		g.drawLine(0, getHeight()/2, getWidth(), getHeight()/2, 1);
		g.drawLine(getWidth()/2, 0, getWidth()/2, getHeight(), 1);
	}


	void updated()/*callback to update Engine*/ {

//		cout << "UPDATING BOX " << m_id_relative << endl;

		/*pour chaque message*/
		vector<string> addresses = m_engines->getCurvesAddress(m_id_relative);

//		cout << endl << "NB OF ADRESSES TO INTERPOLATE FROM : " << addresses.size() << endl;

		vector<string>::iterator addrIt;
		for(addrIt = addresses.begin() ; addrIt != addresses.end(); ++addrIt) {

			vector<float> xCoordsRatio; //from points[][]
			vector<float> yCoordsRatio; //from points[][]
			vector<float> powCoeff;
			vector<short> sectionType;

			string address = *addrIt;
			std::vector<float> result;//debut"/volume 50" end"/volume 100"

			if(!m_engines->getCurveValues(m_id_relative, address, 0, &result)) {
				continue; //min des points de controles g. et d.
			}

			int min = *min_element(result.begin(),result.end());
			int max = *max_element(result.begin(),result.end());

			//map to vectors
			copyPointsToVectors(min, max, &xCoordsRatio, &yCoordsRatio, &sectionType, &powCoeff);//ne pas copier les extremes !

			//inform engine
			m_engines->curvesSetSections(m_id_relative, address, 0, xCoordsRatio, yCoordsRatio, sectionType, powCoeff);

		}//end for

#ifdef VDBG
		updcube++;
		if(updcube>getWidth()/4-100) updcube = 1;
#endif
	}


	void copyPointsToVectors(int min, int max, vector<float>* xCoordsRatio, vector<float>* yCoordsRatio, vector<short>* sectionType, vector<float>* powCoeff)
	{
		// Copy
		map<int,int>::iterator it;
		for (it = points.begin(); it != points.end(); ++it){
//			std::cout << "AutomationCurve::copyPointsToVector : copying point " << it->first << std::endl;
			//if (it->first != 0 && it->first != 100) {
				xCoordsRatio->push_back(it->first);
				yCoordsRatio->push_back(it->second);
				powCoeff->push_back(1);
				sectionType->push_back(CURVE_POW);
			//}
		}
		powCoeff->push_back(1);
		sectionType->push_back(CURVE_POW);
	}

	static const int DEFAULT_START_VALUE = 0;
	static const int DEFAULT_END_VALUE = 100;
};
#endif
