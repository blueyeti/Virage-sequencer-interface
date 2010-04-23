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
 * AccelerationSlider.cpp
 *
 *  Created on: 9 déc. 2009
 *      Author: Luc Vercellin
 */

#include "AccelerationSlider.h"
#include "TransportBar.h"
#include "MainComponent.h"
#include "Engines.hpp"

const float AccelerationSlider::START_VALUE = 0.;
const float AccelerationSlider::END_VALUE = 5.;
const double AccelerationSlider::STEP_TOLERANCE = 0.01;
const float AccelerationSlider::SKEW_FACTOR = 0.6;
const float AccelerationSlider::DEFAULT_VALUE = 1.;

const double BOUNDS_VALUES[] = {1.};
const double BOUNDS_STEP_VALUES[] = {AccelerationSlider::START_VALUE, 0.2, 0.33, 0.5, 0.66, 1., 2., 3., 4., AccelerationSlider::END_VALUE};

AccelerationSlider::AccelerationSlider(const String &componentName) : Slider(componentName) {
	_engines = NULL;
}

double AccelerationSlider::snapValue(double attemptedValue, const bool userIsDragging) {
	static vector<double> BOUNDS;
	bool steps = false;
	if (_shiftPressed) {
		steps = true;
		if (BOUNDS.size() <= 1) { // was empty or without steps
			BOUNDS = vector<double>(BOUNDS_STEP_VALUES,BOUNDS_STEP_VALUES + sizeof(BOUNDS_STEP_VALUES)/sizeof(double));
		}
	}
	else {
		if (BOUNDS.size() > 1 || BOUNDS.empty()) { // was empty or with steps
			BOUNDS = vector<double>(BOUNDS_VALUES,BOUNDS_VALUES + sizeof(BOUNDS_VALUES)/sizeof(double));
		}
	}

	if (userIsDragging) {
		vector<double>::const_iterator it;
		double leftTolerance = 0.09;
		double rightTolerance = 0.09;
		if (steps) {
			leftTolerance = 0.04;
			rightTolerance = 0.04;
		}
		for (it = BOUNDS.begin(); it != BOUNDS.end() ; ++it) {
			if (it != BOUNDS.begin()) {
				leftTolerance = (*it - *(it - 1))/2. ;
			}
			if ((it+1) != BOUNDS.end()) {
				rightTolerance = (*(it + 1) - *it)/2.;
			}
			if (attemptedValue >= (*it - leftTolerance) && attemptedValue <= (*it + rightTolerance)) {
				return *it;
			}
		}
	}

	return attemptedValue;
}

void AccelerationSlider::valueChanged() {
	if (_engines == NULL) {
		_engines = static_cast<TransportBar*>(getParentComponent())->getMainComponent()->getMiddleFrame()->getConduite()->getEngines();
	}
	_engines->setExecutionSpeedFactor(getValue());
}

void AccelerationSlider::mouseDoubleClick (const MouseEvent & e) {
	setValue(1.,true,true);
}

void AccelerationSlider::modifierKeysChanged(const ModifierKeys &mods) {
	(mods.isShiftDown()) ? _shiftPressed = true : _shiftPressed = false;
	setVelocityBasedMode(false);
}

vector<double> AccelerationSlider::getSteps()
{
	if (_steps.empty()) {
		for (unsigned int i = 0 ; i < (sizeof(BOUNDS_STEP_VALUES)/sizeof(double)) ; ++i) {
			_steps.push_back(BOUNDS_STEP_VALUES[i]);
		}
	}
	return _steps;
}
