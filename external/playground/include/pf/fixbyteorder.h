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
 * @condfile
 * fixbyteorder.h
 *
 * This file contains utilities for swapping byte orders.
 * By default, all Playground games store data in little endian format. Therefore,
 * in order to ensure that files can be read on both big and little endian machines,
 * any data that is written to or read from disk should be properly swapped.
 * Because the byte order operations are NOP's on little endian machines, it is safe
 * to call them on all machines. For example, if you have an int you wish to
 * swap, you can write:
 *
 *		int a = 5;
 *		FixByteOrder(&a);
 *		fwrite(&a, sizeof(int), 1, filePtr);		
 * 
 * Note that all Playground library code takes care of byte swapping internally,
 * so this file only needs to be used for client code that is not using the Playground
 * library to read/store data.
 */

#if !defined(FIXBYTEORDER_H_INCLUDED)
#ifndef DOXYGEN
#define FIXBYTEORDER_H_INCLUDED
#endif

#include <stdio.h>
#include <algorithm>

#if !defined(PFCONFIG_H_INCLUDED)
#include "pfconfig.h"
#endif

/// Swap (reverse) bytes in place.
template<int size> 
void SwapBytes(void * p);

/// Implementation of SwapBytes for 4-byte types.
template<>
inline void SwapBytes<4>(void * p)
{
	uint32_t * c= (uint32_t*)p;
	*c = ((((*c) & 0xff000000) >> 24) | 
		  (((*c) & 0x00ff0000) >> 8)  | 
		  (((*c) & 0x0000ff00) << 8)  | 
		  (((*c) & 0x000000ff) << 24));
	(void)p;
}

/// Implementation of SwapBytes for 8-byte types.
template<>
inline void SwapBytes<8>(void * p)
{
	uint32_t * c= (uint32_t*)p;
	SwapBytes<4>(c);
	SwapBytes<4>(c + 1);
	std::swap(*c, *(c+1));
	(void)p;
}

/// Implementation of SwapBytes for 2-byte types.
template<>
inline void SwapBytes<2>(void * p)
{
	uint16_t * c= (uint16_t*)p;
	*c = ((((*c) & 0xff00) >> 8) | (((*c) & 0x00ff) << 8)) ;
	(void)p;
}

/// Implementation of SwapBytes for 1-byte types.
template<>
inline void SwapBytes<1>(void * p)
{
	(void)p;
}

/**
 * Conditionally fix the byte order, in place, of a basic type.
 * Will auto-detect the size of the type.
 * 
 * @param p      Pointer to data to modify.
 */
template<typename T>
inline void FixByteOrder(T * p)
{
#if defined(PF_BIG_ENDIAN)
	SwapBytes<sizeof(T)>(p);
#else
	(void)p;
#endif
}

/**
 * Conditionally fix the byte order, in place, of an array
 * of a basic type. Will auto-detect the size of the type.
 * 
 * @param start  Start of array.
 * @param finish One past the end of an array (STL style end iterator).
 */
template<typename T>
inline void FixByteOrder(T* start, T* finish)
{
#if defined(PF_BIG_ENDIAN)
	while (start != finish) {
		FixByteOrder(start);
		++start;
	}
#else
	(void)start;
	(void)finish;
#endif	
}

#if !defined(DOXYGEN)
inline int16_t GetShort(const uint8_t* mem) {
	return ((int16_t)((int16_t)mem[0] + ((int16_t)mem[1] << 8)));
}

inline uint16_t GetUnsignedShort(const uint8_t* mem) {
	return ((uint16_t)((uint16_t)mem[0] + ((uint16_t)mem[1] << 8)));
}

inline uint32_t GetUnsignedLong(const uint8_t* mem) {
	return (
	((uint32_t)(mem[0])) +
	(((uint32_t)(mem[1])) << 8) +
	(((uint32_t)(mem[2])) << 16) +
	(((uint32_t)(mem[3])) << 24)
	);
}
#endif

#endif
