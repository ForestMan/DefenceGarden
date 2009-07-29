//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------



#ifndef COMPUTERID_H_INCLUDED
#define COMPUTERID_H_INCLUDED

#ifndef PFCONFIG_H_INCLUDED
#include "pfconfig.h"
#endif

#ifndef USER_DOCS

/**
 * @condfile 
 * @nouser
 * Interface for GetComputerId
 */

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

/**
 * @nouser
 * Get the computer's ID.
 * 
 * @return A unique ID for the current computer.
 */
str PFLIB_API pfGetComputerId();

#endif // USER_DOCS
#endif // COMPUTERID_H_INCLUDED
