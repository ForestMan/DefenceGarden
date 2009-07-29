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
 * Playground debug and error utility routines.
 */

#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#ifndef PFCONFIG_H_INCLUDED
#include "pf/pfconfig.h"
#endif

#ifdef NO_PFLIB
#define ASSERT(x)
#define DEBUG_WRITE(x)
#else

#ifdef _WINDOWS
#include <tchar.h>

#ifdef _DEBUG
#ifndef USE_DEBUG_NEW
#define USE_DEBUG_NEW 1
#endif
#endif

#endif
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include "platform.h"

//-----------------------------------------------------------------------------
// global constants
//-----------------------------------------------------------------------------

//#define DELETE_OLD_LOG

// Only one of these should be active
//#define WRITE_TO_DEBUG_MONITOR

/**
 * \def ASSERT( STATEMENT )
 *
 * ASSERT that a statement is returning non-zero.
 *
 * In builds with INCLUDE_DEBUG_STATEMENTS set to zero (by default any non-debug build),
 * STATEMENT is NOT executed.
 *
 * @param STATEMENT Statement to test.
 */

/**
 * \def VERIFY( STATEMENT )
 *
 * Verify that a statement is returning non-zero.
 *
 * Acts like ASSERT, only STATEMENT is always included in the program.
 *
 * @param STATEMENT Statement to test.
 */

/**
 * \def ERROR_WRITE( (string, ...) )
 *
 *  In all builds, this function will write the contained
 * printf-style string and parameters to the debug log, and also
 * to the debug monitor in debug builds.

	Note you need to enclose the parameters in a second pair of parenthesis:

	\code
	ERROR_WRITE(("Format String %d",5));
	\endcode

	All printf format codes are supported.
 */

/**
 * \def DEBUG_WRITE( (string, ...) )
 *
 * In debug builds (when _DEBUG is 1), this
 * function will send the contained printf-style string and
 * parameters to the debug log, and to the debug monitor.
 *
 * \code
 *  DEBUG_WRITE(("Format String %d",5));
 * \endcode
 *
 * All printf format codes are supported.
 *
 * Note you need to enclose the parameters in a second pair of parenthesis:
 *

\code
DEBUG_WRITE(("Format String %d",5));
\endcode

All printf format codes are supported.

*/

/**
 * \def VERBOSE_ERROR( (string, ...) )
 *
 * This macro will write an error string to the debug log in
 * debug and release builds if you enable verbose debugging by
 * calling TPlatform::SetConfig( TPlatform::kVerboseDebug );
 *
 * @see DEBUG_WRITE
 */
/**
 * \def VERBOSE_DEBUG( (string, ...) )
 *
 * This macro will write an error string to the debug log in
 * debug builds if you enable verbose debugging by calling
 * TPlatform::SetConfig( TPlatform::kVerboseDebug );
 *
 * @see DEBUG_WRITE
 */
/**
 * \def VERBOSE_TRACE()
 *
 * This macro will write an error string to the debug log in
 * debug builds indicating the name of the function, file, and
 * line to the debug log if you enable verbose debugging by
 * calling TPlatform::SetConfig( TPlatform::kVerboseDebug );
 *
 * @see DEBUG_WRITE
 */

/**
 * \def TRACE_WRITE()
 *
 * When INCLUDE_TRACE_STATEMENTS in debug.h is set to 1, this function will output
 * the name of the function, file, and line to the debug log.
 *
 **/


#ifndef DOXYGEN
#ifndef WRITE_TO_DEBUG_MONITOR
#define WRITE_TO_FILE
#endif

#ifdef _WINDOWS
#define INCLUDE_ERROR_STATEMENTS 1

#ifndef INCLUDE_DEBUG_STATEMENTS
#define INCLUDE_DEBUG_STATEMENTS _DEBUG
#endif

#ifndef INCLUDE_TRACE_STATEMENTS
#define INCLUDE_TRACE_STATEMENTS _DEBUG
#endif

#define USE_MEMORY_TRACKING 0

#endif

#if USE_DEBUG_NEW
#define _NORMAL_BLOCK    1

#if USE_MEMORY_TRACKING
#define TRACK_MEMORY( MALLOC ) pf::TrackMemory( MALLOC )
namespace pf
{
	PFLIB_API void *  TrackMemory( void * );
};
#else
#define TRACK_MEMORY( MALLOC ) MALLOC
#endif

#if __cplusplus
extern "C" {
#endif
PFLIB_API void * pf_malloc_dbg(size_t size,const char * file,int line);
PFLIB_API void pf_free_dbg( void * );
#if __cplusplus
}
#endif

// Memory tracking allocation
inline void* __cdecl operator new(size_t nSize, const char * lpszFileName, int nLine)
{
	return TRACK_MEMORY( pf_malloc_dbg(nSize, lpszFileName, nLine) );
}
inline void* __cdecl operator new(size_t nSize)
{
	return TRACK_MEMORY( pf_malloc_dbg(nSize, "FileWithoutDEBUG_NEW", 0) );
}
inline void* __cdecl operator new[](size_t nSize, const char * lpszFileName, int nLine)
{
	return TRACK_MEMORY( pf_malloc_dbg(nSize, lpszFileName, nLine) );
}
inline void __cdecl operator delete(void* p, const char * lpszFileName, int nLine)
{
	(void)nLine ;
	(void)lpszFileName;
	pf_free_dbg(p);
}
inline void __cdecl operator delete[](void* p, const char * lpszFileName, int nLine)
{
	(void)nLine ;
	(void)lpszFileName;
	pf_free_dbg(p);
}
inline void __cdecl operator delete(void* p)
{
	pf_free_dbg(p);
}
inline void __cdecl operator delete[](void* p)
{
	pf_free_dbg(p);
}
#define DEBUG_NEW new(THIS_FILE, __LINE__)
#endif // USE_DEBUG_NEW

#endif
#ifdef MACINTOSH
#define INCLUDE_ERROR_STATEMENTS 1
#ifndef INCLUDE_DEBUG_STATEMENTS
#define INCLUDE_DEBUG_STATEMENTS _DEBUG
#endif

#ifndef INCLUDE_TRACE_STATEMENTS
#define INCLUDE_TRACE_STATEMENTS _DEBUG
#endif
#endif

//-----------------------------------------------------------------------------
// global macros
//-----------------------------------------------------------------------------

#if INCLUDE_ERROR_STATEMENTS || INCLUDE_DEBUG_STATEMENTS
#if defined(MACINTOSH)
#define ERROR_WRITE(STATEMENT) WriteDbg STATEMENT
#else
#define ERROR_WRITE(STATEMENT) WriteDbg##STATEMENT
#endif
#else
#define ERROR_WRITE( OUT_STATEMENT )
#endif

#if _DEBUG
void PFLIB_API pfDebugBreak();
#else
#define pfDebugBreak()
#endif

#if INCLUDE_DEBUG_STATEMENTS
#if INCLUDE_TRACE_STATEMENTS
#if defined(MACINTOSH)
#define TRACE_WRITE() WriteDbg("TRACE: %s, %d", __FILE__, __LINE__)
#else
#define TRACE_WRITE() WriteDbg("TRACE:" __FUNCTION__ " %s, %d", __FILE__, __LINE__)
#endif
#else
#define TRACE_WRITE()
#endif

#define VERBOSE_ERROR(X) do { if (TPlatform::IsEnabled( TPlatform::kVerboseDebug ) ) { ERROR_WRITE(X); } } while (0)
#define VERBOSE_DEBUG(X) do { if (TPlatform::IsEnabled( TPlatform::kVerboseDebug ) ) { DEBUG_WRITE(X); } } while (0)
#define VERBOSE_TRACE() do { if (TPlatform::IsEnabled( TPlatform::kVerboseDebug ) ) { TRACE_WRITE(); } } while (0)

#if defined(MACINTOSH)
#define DEBUG_WRITE(STATEMENT) WriteDbg STATEMENT
#define VERIFY( STATEMENT ) do { bool success=!(!(STATEMENT)); ASSERT(success && #STATEMENT); } while (0)
#define ASSERT( STATEMENT ) do{ if(!(STATEMENT)) { WriteDbg("ASSERT(%s) FAILED!! %s, %d", #STATEMENT, __FILE__, __LINE__); pfDebugBreak(); } }while(0)
#else
#define DEBUG_WRITE(STATEMENT) WriteDbg##STATEMENT
#define VERIFY( STATEMENT ) do { bool success=!(!(STATEMENT)); ASSERT(success && #STATEMENT); } while (0)
#define ASSERT( STATEMENT ) do{ if(!(STATEMENT)) { WriteDbg("ASSERT(%s) FAILED!! %s, %d", #STATEMENT, __FILE__, __LINE__); pfDebugBreak(); } }while(0)
#endif
#else
#define TRACE_WRITE()
#define ASSERT( STATEMENT )
#define VERIFY( STATEMENT ) (void)(STATEMENT)
#define DEBUG_WRITE( OUT_STATEMENT )
#define VERBOSE_ERROR(X)
#define VERBOSE_DEBUG(X)
#define VERBOSE_TRACE()
#endif


//-----------------------------------------------------------------------------
// global enums
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// global type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// global class definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// global function declarations
//-----------------------------------------------------------------------------

#if !USER_DOCS
/** \nouser */
void PFLIB_API ResetDbg(const char * dataDir);  ///< Initializes debug output and deletes all data in the debug file.
extern "C++" void PFLIB_API WriteDbg(const char *sMsg, ...);

void PFLIB_API CloseDbg(void);
void PFLIB_API KillProcess(char *sProcess);
int32_t PFLIB_API pfGetTotalAllocated();
int32_t PFLIB_API pfGetMaxAllocated();
#endif


#if USE_DEBUG_NEW
#ifndef NO_DEBUG_NEW_DEFINE
#define new DEBUG_NEW
#define THIS_FILE  __FILE__
#endif //NO_DEBUG_NEW_DEFINE
#endif //USE_DEBUG_NEW

#endif
#endif // DEBUG_H_INCLUDED
