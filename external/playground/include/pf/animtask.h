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
 * Interface for class TAnimTask.
 */

#ifndef ANIMTASK_H_INCLUDED
#define ANIMTASK_H_INCLUDED

#ifndef PFTYPES_H_INCLUDED
#include "pftypes.h"
#endif

#ifndef TASK_H_INCLUDED
#include "task.h"
#endif

#ifndef CLOCK_H_INCLUDED
#include "clock.h"
#endif

/**
 * The TAnimTask interface. Used as a "callback" for animation
 * or other timed repeating tasks. Can also be used as a simple
 * delayed task: You can call TPlatform::AdoptTask() to adopt a
 * TAnimTask-derived class that will trigger after its delay
 * expires, and then just have its Animate() call return false
 * to let it be destroyed.
 *
 * @see TTask
 */
class PFLIB_API TAnimTask : public TTask
{
public:
	/**
	 * Constructor
	 *
	 * @param clock      Clock that this anim task uses to determine how much time has passed.
	 *					 If this parameter is null then the global clock is used.
	 */
	TAnimTask(TClock *clock = NULL);

	/**
	 * Set the animation delay.
	 *
	 * @param delay      Number of milliseconds to wait to call DoTask().
	 * @param autoRepeat Repeat this delay after every task.
	 * @param resetTime  Reset the time so that the (initial) delay is counted from
     *                   <i>now</i> rather than from the last task
     *                   event time.
     *
	 * @param forceFrequency
	 *                   Force the frequency to be the same as the delay implies; will
	 *                   run the Animate() call multiple times if more than twice the
     *                   time of delay has elapsed since the last call.
     *
     *                   Will not work if resetTime is also true,
     *                   since resetTime causes the time to be set
     *                   to the now+delay after the first call, and
     *                   as such forceFrequency will never cause it
     *                   to loop.
	 */
	void SetDelay( uint32_t delay, bool autoRepeat=true, bool resetTime=true, bool forceFrequency=false );

	/**
	 * Pause the current task. Prevents the Animate task from being called.
	 *
	 * Resume by calling SetDelay() or RunOnDraw(), as appropriate.
	 */
	void Pause();

	/**
	 * Get the number of milliseconds before this task will
	 * be ready to trigger again.
	 *
	 * Returns a negative number if the next call to Ready()
	 * would be true immediately.
	 *
	 * @return Number of milliseconds before this task is ready. Returns
	 * 			UINT_MAX if task is disabled.
	 */
	uint32_t GetTimeUntilReady();

	/**
	 * Define this function to add the actual animation task.
	 *
	 * @return True to continue, false when we're done.
	 */
	virtual bool Animate()=0;

	/**
	 * Enable the "Run on Draw" feature, which executes this task
	 * prior to every actual screen update. If you want the task to
	 * run on draw as well as on a timer, set up the timer using
	 * SetDelay() normally. If you want to only get called when
	 * the screen is about to update, then call Pause() <i>before</i> you
	 * call RunOnDraw(). Calling Pause after you call RunOnDraw will
	 * disable RunOnDraw.
	 *
	 * @param enable True to enable "Run on Draw", false to disable.
	 */
	void RunOnDraw( bool enable );

	/**
	 * Set the reference clock. Useful for having one clock that
	 * can be paused and resumed to pause and resume a set or class
	 * of TAnimTasks.
	 *
	 * @param clock  Clock to use to time animations.
	 */
	void SetClock( TClock * clock );

	/**
	 * Get the reference clock.
	 *
	 * @return A pointer to the currently associated clock.
	 */
	TClock * GetClock() { return mClock; }

	/**
	 * Get the current elapsed time of the attached clock.
	 *
	 * @return Current elapsed time.
	 */
	uint32_t GetTime();

#ifndef TAGGING_IGNORE
private:
#endif
	/// Internal TAnimTask implementation
	virtual bool DoTask();
	/// Internal TAnimTask implementation
	virtual bool Ready(ETaskContext context);


	bool				mRunOnDraw ;

	uint32_t			mTime ;
	int32_t				mAutoIncrement ;
	bool				mReset ;
	bool				mForceFrequency ;
	TClock				*mClock;
};

#endif // ANIMTASK_H_INCLUDED
