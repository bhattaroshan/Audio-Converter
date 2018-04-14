
#ifndef GLOBALDATA_H
#define GLOBALDATA_H


#include<windows.h>

#include"registry.h"
#include"general.h"
#include"button.h"
#include"shell.h"
#include"AudioConverter.h"
#include"system.h"
#include"bitmaps.h"
#include"menu.h"
#include"identifier.h"
#include"dialog.h"
#include"odmenu.h"
#include"taginfo.h"

#include"bass.h"
#include"bassenc.h"
#include"tags.h"

#define AUDIO_CONVERTER_PATH "Software\\RB Super Player\\RB Audio Converter"
#define AAC_ENCODER_PATH "Software\\RB Super Player\\RB Audio Converter\\Encoders\\Aac"
#define FLAC_ENCODER_PATH "Software\\RB Super Player\\RB Audio Converter\\Encoders\\Flac"
#define MP3_ENCODER_PATH "Software\\RB Super Player\\RB Audio Converter\\Encoders\\Mp3"
#define MPC_ENCODER_PATH "Software\\RB Super Player\\RB Audio Converter\\Encoders\\Mpc"

// MAIN HANDLE

extern  HWND             MainHandle;





//  VARIABLE OF GENERAL

extern  GENERAL          General;
//extern  bool             IsChildPresent;
//extern  HWND             ChildDialogHandle;


// VARIABLE OF REGISTRY

extern  REGISTRY         Registry; 



// VARIABLE OF BUTTON
extern  BUTTON           Button;


// VARIABLE OF AUDIO_CONVERTER

extern  AUDIO_CONVERTER  AudioConverter;


// VARIABLE OF SHELL

extern  SHELL            Shell;

// ODMENU VARIABLES

extern    ODMENU         AC_ODMenu;
extern    ODMENU         ACN_ODMenu;

//SYSTEM VARIABLES

extern    SYSTEM         System;


//TAGS VARIABLES

extern    TAGINFO       TagInfo;

#endif
