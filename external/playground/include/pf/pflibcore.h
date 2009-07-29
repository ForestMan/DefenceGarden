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
 *
 * Include file that PFLIB requires you load before anything else.
 */

#ifndef PFLIBCORE_H_INCLUDED
#define PFLIBCORE_H_INCLUDED

#if !defined(PFCONFIG_H_INCLUDED)
#include "pfconfig.h"
#endif

// The rest of this should only happen in C++ files
#if __cplusplus

//------------------------------------------------------------------
// platform specific entry point

class TPlatform;

#ifndef DOXYGEN
typedef void (*pTMainFunc)(TPlatform* pPlatform, const char* cmdLine);

void PFLIB_API PlayFirstExec(
#ifdef _WINDOWS
		HINSTANCE   	hInstance,
		HINSTANCE   	hPrevInstance,
		char*		lpCmdLine,
		int			nCmdShow,
#endif
#ifdef MACINTOSH
		int			argc,
		char**		argv,
#endif
		pTMainFunc	pMainFunc,
		bool		bAllowMultipleInstances);
#endif

//------------------------------------------------------------------
// boiler plate
#ifdef PLAYFIRST_MAIN

#ifdef PLAYGROUND_INIT
void PlaygroundInit();
#endif

/**
 * Prototype for Playground main entry point.
 *
 * @param pPlatform Pointer to the global TPlatform. Also accessible using
 *                  TPlatform::GetInstance().
 * @param cmdLine   The command line used to launch the game.
 */
void Main(TPlatform* pPlatform, const char* cmdLine);

#ifndef DOXYGEN
#ifdef _WINDOWS
int __stdcall WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     char*     lpCmdLine,
                     int       nCmdShow)
{
#ifdef PLAYGROUND_INIT
	PlaygroundInit();
#endif
#ifdef PLAYFIRST_ALLOW_MULTIPLE_INSTANCES
	PlayFirstExec(hInstance, hPrevInstance, lpCmdLine, nCmdShow, Main, true);
#else
	PlayFirstExec(hInstance, hPrevInstance, lpCmdLine, nCmdShow, Main, false);
#endif
	return 0;
}
#endif


#ifdef MACINTOSH
int main(int argc, char** argv)
{
#ifdef PLAYGROUND_INIT
	PlaygroundInit();
#endif
#ifdef PLAYFIRST_ALLOW_MULTIPLE_INSTANCES
	PlayFirstExec(argc, argv, Main, true);
#else
	PlayFirstExec(argc, argv, Main, false);
#endif
	return 0;
}
#endif

#endif // DOXYGEN

#endif
#endif __cplusplus

#ifdef DOXYGEN
/**
 * A user-defined function that will be called by Playground
 * <em>before TPlatform is initialized</em>.  No non-static
 * calls on TPlatform are allowed.
 *
 * In order for this function to be called, you must define
 * PLAYGROUND_INIT in the file where you're defining
 * PLAYFIRST_MAIN, before you include pflibcore.h.
 *
 * This function is really designed only for setting up values
 * with TPlatform::SetConfig() that TPlatform will need during
 * initialization; most application initialization still belongs
 * in Main().
 */
void PlaygroundInit();
#endif

#endif // PFLIBCORE_H_INCLUDED
