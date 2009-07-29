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
 *
 * Include file that contains macros for library configuration.
 *
 */

#if !defined(NDEBUG) && !defined(_DEBUG)
#error "Either NDEBUG for release or _DEBUG for development must be defined."
#endif

#ifndef PFCONFIG_H_INCLUDED
#define PFCONFIG_H_INCLUDED

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef USER_DOCS
//#define BOOST_NO_EXCEPTIONS
#endif // USER_DOCS

#ifndef NULL
#define NULL 0L
#endif

//------------------------------------------------------------------
// necessary to avoid pulling in any platform specific headers
#ifdef _WINDOWS
#ifndef _WINDEF_
typedef struct HINSTANCE__ *HINSTANCE;
#endif
#endif

// work around Windows platform issue
#ifndef DrawText
#define DrawText DrawTextA
#endif
#ifndef PlaySound
#define PlaySound PlaySoundA
#endif

#ifdef _MSC_VER
#pragma warning(disable:4127) // Conditional expression is constant
#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable:4251) // warning about dll interface needed in base class
#pragma warning(disable:4345) // POD objects will be initialized to zero when () constructed
#pragma warning(disable:4702) // unreachable code (pops up in STL classes all over the place)
#pragma warning(disable:4996) // deprecated functions
#define NEED_MALLOC_H 1
#define NEED_ZUTIL_H 1
#define NEED_TEXT_FILE_TYPE 1	// Need the "t" for text or "b" for binary in fopen()
#define NEED_DIRECT_H 1
#define NEED_STRING_H 1
#define NEED_IO_H 1
#define HAVE_VSCPRINTF 1

// If we're defining a TGameStateHandler DLL...
#ifdef GAMESTATE_EXPORTS
#define GAMESTATE_API __declspec(dllexport)
#else
#define GAMESTATE_API __declspec(dllimport)
#endif

#endif

// The Macintosh side of avoiding platform specific headers
#if defined(__APPLE__) || defined(__MWERKS__)
#if !defined(__MWERKS__)
#define XCODE 1
#endif

#if defined(XCODE)	// Metrowerks defines MACINTOSH for us
#define MACINTOSH 1
#define GAMESTATE_API
#endif

static const int PFMAC_MAX_PATH=4096 ;

#define PFLIB_STATIC_LINK 1
#define _MAX_PATH 1024

#define NEED_ALLOCA_H 1
#define NEED_STRING_H 1
#define NEED_UNISTD_H 1
#define HAVE_STDINT_H 1

#if defined(__BIG_ENDIAN__)
#define PF_BIG_ENDIAN 1
#endif

#endif

//------------------------------------------------------------------
// If we're building as a dll...
#ifndef PFLIB_STATIC_LINK

#ifdef PFLIB_EXPORTS
#define PFLIB_API __declspec(dllexport)
#else  //PFLIB_EXPORTS
#define PFLIB_API __declspec(dllimport)
#endif //PFLIB_EXPORTS

#else

#define PFLIB_API

#endif // _USRDLL

#endif
