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
 * AccelerationSlider.h
 *
 *  Created on: 9 déc. 2009
 *      Author: Luc Vercellin
 */

#ifndef ACCELERATIONSLIDER_H_
#define ACCELERATIONSLIDER_H_

#include "includes.h"

class Engines;

class AccelerationSlider : public Slider {
	public :
		AccelerationSlider(const String &componentName);
		virtual double snapValue(double attemptedValue, const bool userIsDragging);
		virtual void valueChanged();
		vector<double> getSteps();

/*		static const float START_VALUE = 0.;
		static const float END_VALUE = 5.;
		static const double STEP_TOLERANCE = 0.01;
		static const float SKEW_FACTOR = 0.45;//0.37
		static const float DEFAULT_VALUE = 1.;*/

		static const float START_VALUE;
		static const float END_VALUE;
		static const double STEP_TOLERANCE;
		static const float SKEW_FACTOR;
		static const float DEFAULT_VALUE;

	protected :
		void mouseDoubleClick (const MouseEvent & e);
		void modifierKeysChanged(const ModifierKeys &mods);
	private :
		bool _shiftPressed;
		vector<double> _steps;
		Engines * _engines;
};

#endif /* ACCELERATIONSLIDER_H_ */
