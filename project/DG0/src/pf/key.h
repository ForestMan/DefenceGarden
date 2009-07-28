//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

/**
 * @file 
 * Definition for information needed to communicate with PlayFirst servers.
 * PlayFirst will provide you with new copies of key.h to communicate with
 * the servers. When provided with a new copy, the developer MUST include
 * the PlayFirst provided key.h file in their next delivery, or 
 * the build will not be accepted.
 * These values need to be used to configure Playground. Please see the
 * file main.cpp, inside of PlaygroundInit(), for the proper configuration.
 */

#ifndef KEY_H_INCLUDED
#define KEY_H_INCLUDED

static const char * const ENCRYPTION_KEY = "abcdefghAAAA";

static const char * const PFGAMEHANDLE = "Playground Skeleton Application";


// If your game has fewer or more game modes, you can add them here, but note that
// PlayFirst will provide the exact name and order for the game modes when
// the key.h file is delivered.
static const char * const PFGAMEMODENAMES[] = 
{
	"playground_gamemode1",
	"playground_gamemode2"
};

// If your game has fewer or more medals, you can add them here, but note that
// PlayFirst will provide the exact name and order for the medals when
// the key.h file is delivered.
static const char * const PFGAMEMEDALNAMES[] = 
{
	"playground_medal1",
	"playground_medal2"
};


// DO NOT ADD ANYTHING ELSE TO THIS FILE. THIS FILE CAN BE OVERWRITTEN DURING
// THE PLAYFIRST BUILD PROCESS

#endif // KEY_H_INCLUDED
