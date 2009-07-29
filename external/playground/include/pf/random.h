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
 * Interface for class TRandom
 */


#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#ifndef PFCONFIG_H_INCLUDED
#include "pfconfig.h"
#endif

#ifndef PFTYPES_H_INCLUDED
#include "pftypes.h"
#endif

// Define to 1 to support 64 bit compiles
#ifndef SUPPORT_64_BIT
#define SUPPORT_64_BIT 0
#endif


/**
 * A deterministic random number generator.
 *
 * Based on the Mersenne Twister algorithm, it has a period
 * of \f$2{19937}-1\f$ iterations, it's as fast as or faster
 * than rand(),  and it's equally distributed in 623 dimensions.
 */
class PFLIB_API TRandom
{
public:
	/// Default Constructor
	TRandom();

	/**
	 * Seed with a 32-bit number.
	 *
	 * @param s      A random 32-bit seed. Once seeded with a particular number,
	 *               the sequence will always be the same.
	 */
	void Seed(uint32_t s);

	/**
	 * Seed with array. Use this if 4 billion possible sequences are
	 * not enough.
	 *
	 * @param init_key   Array of long integers.
	 * @param key_length Number of long integers in the array.
	 */
	void SeedArray(unsigned long init_key[], uint32_t key_length);

	/**
	 * Get the current random number generator state, which you can then use to
	 * restore to a known state using RestoreState.
	 *
	 * @param buffer - buffer to store state in
	 * @param bufferLength - number of bytes in buffer
	 *
	 * @return 0 if successful, otherwise returns the length that buffer needs to
	 *			 be for success;
	 */
	uint32_t SaveState(void *buffer, uint32_t bufferLength);

	/**
	 * Restore random number generator state from SaveState
	 *
	 * @param buffer Buffer to read state from.
	 */
	void RestoreState(void *buffer);

	/**
     * Generates a random unsigned long.
     *
     * @return A random 32-bit integer that is equally random across
     *         all 32 bits.
	 */
	unsigned long Rand32();

	/**
	 * Generates a random number on [0,1)-real-interval
	 *
	 * @return A number between zero and one, never equalling 1.0
	 */
	TReal RandFloat()
	{
		// This number is a bit bigger than 2^32, so that the floating point
		// calculation will not end up rounding up.
		return (TReal)(Rand32()*(1.0/4294967424.0));
	}

	/**
	 * Generates a random number on [0,1)-real-interval, that is random
	 * out to double-precision granularity. Since a double has (on
	 * the Wintel reference platform) more bits of precision than an int/long,
	 * it takes two calls from Rand32 and combines them into a double
	 * precision random number.
	 *
	 * @return A number between zero and one, never equalling 1.0
	 */
	double RandDouble()
	{
		uint32_t a=Rand32()>>5, b=Rand32()>>6;
		return(a*67108864.0+b)*(1.0/9007199254740992.0);
	}

	/**
	 * Generates a random integer between the two parameters,
	 * inclusive. Will generate all options equally, including bottom
	 * and top.
	 *
	 * @param bottom Lowest number that will be returned.
	 * @param top    Highest number that will be returned.
	 *
	 * @return A random integer.
	 */
	int32_t RandRange( int32_t bottom, int32_t top )
	{
		double range = (double)top-((double)bottom)+1.0 ;
		int32_t offset = (int32_t)(RandDouble()*range) ;

		return offset+bottom;
	}

private:
	enum
	{
		N=624
	};
	uint32_t mMt[N]; /* the array for the state vector  */
	int32_t mMti; /* mMti==N+1 means mMt[N] is not initialized */

};

#endif // RANDOM_H_INCLUDED
