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
 * Interface for class TTask
 */

#ifndef TASK_H_INCLUDED
#define TASK_H_INCLUDED

#ifndef PFLIBCORE_H_INCLUDED
#include "pflibcore.h"
#endif

/**
 * The task interface.  Used as a "callback" for events and periodic tasks.
 */
class PFLIB_API TTask
{
public:
    /// Task context.
	enum ETaskContext
	{
		eNormal,	///< Normal update context.
		eOnDraw,	///< Immediately prior to the next screen draw context.
	};

	/**
	 * Virtual Destructor.
	 */
	virtual ~TTask() ;

    /**
     * Is this task ready?
     * 
     * A virtual function that returns whether this task is
     * ready to be executed.  Derived classes should override this
     * function to provide more control over when a task is executed.
     * 
     * @param context The context in which we're being called.
     * 
     * @return eReady if it's ready, eNotReady to wait, or eOnDraw to execute prior to the next screen draw. Default implementation returns eReady.
     */
	virtual bool Ready(ETaskContext context=eNormal) { (void)context; return true ; }

	/**
	 * This function is called when it's time to execute this task.
	 * 
	 * @return True to keep the task alive. False to destroy the task.
	 */
	virtual bool DoTask()=0;
};

#endif // TASK_H_INCLUDED
