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
 *  AllStatic.h
 *  Virage
 *
 *  Created by Henry Bernard Bernard on 05/11/08.
 *  Copyright 2008 by Blue Yeti. All rights reserved.
 */

#ifndef _ALLSTATIC_H_
#define _ALLSTATIC_H_

#include <juce.h>


static const String s_DEFAULT_PROJECT_NAME = String("NewVirageProject");
static const String s_DEFAULT_MESSAGELIST_NAME = String("New MessageList");

static const int s_PIX_TO_MS_RATIO = 10;
static const int s_MAINWINDOW_PADDING_LEFT = 25;
static const int s_MAINWINDOW_PADDING_TOP = 0;
static const int s_MAINWINDOW_PADDING_RIGHT = 0;

//sequence
static const int s_SEQUENCE_MAX_SIZE = 7200000;//ms
static const int s_SEQUENCE_MAX_HEIGHT = 1000;//pixels

static const int s_MIN_XSCALE = 30;//ms
static const int s_TIME_LEGEND_MULTIPLE = 5;//ms

static const float s_FRAME_BORDER_X_PERCENT = 0.16f/* % */;//division sequence/frame gauche ds mainwindow

//viseur
static const int s_MIN_MAP_WIDTH = 6;
static const int s_DEFAULT_MAP_WIDTH = 20;

//text about
static const String s_About_Text = String("This version is freely distributed with all rights reserved.\n\nVirage Sequencer 0.3: * An intermedia interactive sequencer * for inter-media behaviours and interactions scripting, in stage or installations contexts.\nVirage Sequencer is part from the Virage Platform, funded by the French National Agency for Research, which aims to survey creative needs and practices through experimentation, within the fields of the performing and interactive arts .\n\nVirage Platform is a partnership between: BEK, Blue Yeti, CICM, CopperLan, didascalie.net, ENSATT, GMEA and GMEM - Centres Nationaux de Creation Musicale d'Albi-Tarn et de Marseille, iMAL, ISTS, LaBRI, LIMSI, RSF, Stantum.\nMore information on *www.plateforme-virage.org*\n\nVirage Sequencer 0.3 hand-crafted in Bordeaux, France, by Henry Bernard, Laurent Garnier (Blue Yeti) and Luc Vercellin.\n\nLibrary credits:\n* 'iScore' library developped by Raphael Marczak, Antoine Allombert, Bruno Valeze and Myriam Desainte-Catherine), (LaBRI), France,\n* Juce (Jules' Utility Class Extensions), UK.\n\n For more details about license, please contact www.blueyeti.fr\n\nCopyright (c) 2008-2009 Blue Yeti, All rights reserved.");

//Ancres
static const int s_ANCHOR_MARGIN = 10;	//margin in  mainComponent from borders for anchors
static const int s_ANCHOR_HEIGHT = 15;	//margin in  mainComponent from borders for anchors
static const int s_ANCHOR_WIDTH = 6;	//margin in  mainComponent from borders for anchors

static const int S_MAGNETISM_DELTA_H = 180;

//fonts
static const Font s_FONT_WEBDINGS = Font(String("Webdings"), 14, Font::plain);
static const Font s_SYMBOL1_FONT = Font(String("Wingdings 2"), 23, Font::plain);
static const Font s_HELVETICA_FONT = Font(String("Helvetica"), 14, Font::plain);
static const Font s_COMMENT_FONT = s_HELVETICA_FONT;
static const Font s_GUIFX_FONT = Font(String("Guifx v2 Transports Labeled"), 37, Font::plain);

//cercles rouges de facilité
static const int s_FACILITY_WIDTH = 12;
static const int s_FACILITY_HEIGHT = 12;
static const int s_FACILITY_AREA = 50;

//rail haut
static const unsigned int s_TIMERAIL_HEIGHT = 17*2;//dy entre 0 et hauteur timeline TOTALE (trig + time)
static const unsigned int s_TRIGRAIL_HEIGHT = 17;//dy entre 0 et hauteur trigline 
static const int s_COPY_DELTA = 50;

//boîtes
static const int s_BOX_MIN_WIDTH = 20 /*px*/;
static const int s_BOX_MIN_HEIGHT = 50 /*px*/;
static const int s_BOX_TITLE_PADDING = 2 /*px*/;
static const int s_BOX_BORDER = 3 /*px*/;
static const int s_BOX_TITLE_LEFT = 3 /*px*/; 
static const int s_BOX_TITLE_RIGHT = 50/*px*/;
static const int s_BOX_DEFAULT_LENGTH = 200/*px*/;
static const int s_BOX_DEFAULT_HEIGHT = 100/*px*/;
static const int s_BOX_MESSAGEBUTTON_WIDTH = 10/*px*/;
static const int s_BOX_MESSAGEBUTTON_HEIGHT = 20/*px*/;
static const int s_BOX_MUTE_HEIGHT = 20/*px*/;
static const int s_BOX_INFO_BUTTON = 17/*px*/;
static const int behaviourInfoThickness = 3/*px*/;
static const int relThickness = 2/*px*/;
static const int S_BOX_BUTTON_PADDING_RIGHT = 3/*px*/;

//trigger
static const int s_TRIGGER_BUTTON_SIZE = 15/*px*/;

//RELATIONS (fils constituants)
static const Colour s_COLOUR_UNSELECTION = Colour (0xff009edf);
static const Colour s_BGCOLOUR = Colour (0xffaab2b7);
static const Colour s_COLOUR_BACKGROUND = Colour(0xff0F0F0F);
static const int s_WIRE_THICKNESS = 3;

//raccourcis claviers
static const KeyPress s_TIMEBOX_KEY = KeyPress::F1Key ;

//pointing facilities unique-maker
static const int s_BOX_FACILITY_THRESHOLD = 10;
static const Colour s_COLOUR_BOX = Colour (0xffffffff);
static const Colour s_COLOUR_ORANGE = Colour(0xffff6d00);
static const Colour s_COLOUR_PINK = Colour(0xffea2d75);
static const Colour s_COLOUR_PASCAL = Colour(0xff00041F);
static const Colour s_COLOUR_SELECTION  = Colour (0xff009edf);
static const Colour s_COLOUR_SELECTION_BLUE  =  Colour (0xff009edf);
static const Colour s_COLOUR_SELECTION_GREEN  = Colour(0xff95ff00);

//fenetres -> toolbar
static const int s_DEFAULT_BOTTOM_BORDER = 6;

//Toolbox
static const int s_TOOLBOX_FLEXIBLEBOX = 3;

//AUTOMATION EDITOR
static const int s_SCALE_LENGTH  = 100;

//sign function
inline int signof(float a){
  if(a>0.){
    return 1;
  }
  if(a<0.) {
    return -1;
  }
  return 0;
}

#endif
