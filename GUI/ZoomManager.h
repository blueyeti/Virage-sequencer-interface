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
 *  ZoomManager.h
 *  Virage
 *
 *  Created by Henry Bernard and Luc Vercellin on 14/05/09.
 *  Copyright 2009 BlueYeti.Fr. All rights reserved.
 *
 */

#ifndef _ZoomManager_H_
#define _ZoomManager_H_

#include "includes.h"
using namespace std;

class ZoomManager {

 private :
  unsigned int m_horizontal_ratio;/*ms to pixel*/
  unsigned int m_vertical_ratio;/* root to user display */

 public:

  ZoomManager(unsigned int _horizontal_ratio, unsigned int _vertical_ratio) {
    m_horizontal_ratio = _horizontal_ratio;
    m_vertical_ratio = _vertical_ratio;
  }

  ~ZoomManager() {}

  //====conversion pixel->MILLIseconde
  float convertPixToTime(unsigned int pixelValue) {
    return (pixelValue*m_horizontal_ratio);
  }

  //conversion milliseconde->pixel
  inline int convertTimeToPix(unsigned int msecondValue) {
    return round((float)msecondValue/(float)m_horizontal_ratio);
  }

  inline int convertRootPixToPix(unsigned int rootValue) {
    return round(rootValue*m_vertical_ratio);
  }

  inline int convertPixToRootPix(unsigned int pixValue) {
    return round((float)pixValue/(float)m_vertical_ratio);
  }

  inline void setHorizontalRatio(float ratio) {
    m_horizontal_ratio = ratio;
  }

  inline unsigned int getRatio() {
    return m_horizontal_ratio;
  }

  inline void setVerticalRatio(unsigned int ratio) {
    m_vertical_ratio = ratio;
  }

  inline unsigned int getVerticalRatio() {
    return m_vertical_ratio;
  }
};

#endif
