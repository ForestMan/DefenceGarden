//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------


#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED

/**
 * @condfile
 * Interface for TClock.
 */

/**
 * The TClock class encapsulates timer functionality.
 *
 * When you have a need for a time source in your game, you
 * can instantiate a TClock object that has its own Start,
 * Stop, Reset, and Pause controls.
 *
 * TClocks can be assigned to TAnimTask objects to control
 * when they animate.
 *
 * For example, if you create a TClock in your game, you
 * can use it to time the animations your game employs.
 * When implementing game-pause functionality, if all animations that
 * should pause reference the game TClock object, then you can
 * pause your game by simply pausing the TClock.
 */
class PFLIB_API TClock
{
public:
	/// Constructor
	TClock();

	/// Destructor
	virtual ~TClock();

	/**
	 * Start the timer. Has no effect on a running timer.
	 */
	void Start(void);

	/**
	 * Pause the timer. Will have no effect on a paused timer.
	 *
	 * @return true if timer was paused already.
	 */
	bool Pause(void);

	/**
	 * Pauses and zeros a timer.
	 */
	void Reset(void);

	/**
	 * Get the current running millisecond count.
	 *
	 * @return Number of milliseconds clock has been allowed to run.
	 */
	uint32_t GetTime(void);

	/**
	 * Set the current clock time.
	 *
	 * Resets the clock and sets the current elapsed time.
	 *
     * @param t  Value to set the current time to.
	 */
	void SetTime(uint32_t t);

	/**
	 * Return whether the clock is paused.
	 *
	 * @return True if paused.
	 */
	bool GetPaused() { return mbPaused; }
private:
	bool mbPaused;
	bool mbInit;
	uint32_t muStartTime;
	uint32_t muBaseTime;
	uint32_t muPauseStartTime;
	uint32_t muPauseAccum;
};


#endif // CLOCK_H_INCLUDED
