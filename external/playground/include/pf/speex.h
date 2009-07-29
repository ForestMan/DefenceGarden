//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
//
// This material is the confidential trade secret and proprietary
// information of PlayFirst, Inc. It may not be reproduced, used, disclosed,
// sold or transferred without the prior written consent of PlayFirst, Inc.
// Any use is subject to license by PlayFirst, Inc.
//------------------------------------------------------------------


/**
 * Interface for class TSpeex
 */


#ifndef SPEEX_H_INCLUDED
#define SPEEX_H_INCLUDED

#ifndef PFCONFIG_H_INCLUDED
#include "pfconfig.h"
#endif


/**
 * A class that wraps the registration of Speex support with
 * the sound engine. Once you call TSpeex::Register(),
 * TSound::Get() will properly load a TSound object that
 * streams an Ogg/Speex file, assuming the file has the
 * expected .spx extension.
 *
 * See more important information in the TSound documentation.
 */
class PFLIB_API TSpeex
{
public:
	/**
	 * Register the Speex decoder with the sound engine.
	 */
	static void Register();
};

#endif // SPEEX_H_INCLUDED
