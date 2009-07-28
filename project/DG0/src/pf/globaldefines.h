//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------


#ifndef GLOBALDEFINES_H_INCLUDED
#define GLOBALDEFINES_H_INCLUDED

#include <pf/sound.h>
#include <pf/message.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int DEFAULT_SOUND_VOLUME = 750;

enum ESoundGroups {
	kSFXSoundGroup = TSound::kUserSoundBase,	// This is 1000
	kMusicSoundGroup
};

// Define a quoted sound to be in the SFX group
#define SFX(SndFilename) SndFilename "?group=1000"


#endif //GLOBALDEFINES_H_INCLUDED
