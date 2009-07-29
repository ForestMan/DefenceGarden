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
 * Interface for class TGamestatehandler
 */


#ifndef GAMESTATEHANDLER_H_INCLUDED
#define GAMESTATEHANDLER_H_INCLUDED

#ifndef PFCONFIG_H_INCLUDED
#include "pfconfig.h"
#endif

/**
 * A handler for game state actions. Typically implemented by
 * Playground or PlayFirst.
 */
class TGameStateHandler
{
public:
	/**
	 * Virtual Destructor
	 */
	virtual ~TGameStateHandler(){};

	/**
	 * Handle SetState(). Handler can modify the key or value.
	 *
	 * @param key    State to set.
	 * @param value  New state value.
	 *
     * @return True if the state has been handled, and you'd like to
     *         eat the state. If you return true, it will not
     *         perform any other operations on this state, though it
     *         will still be cached by TGameState.
	 */
	virtual bool SetState( const char * key, const char * value )=0;

	/**
	 * Get the value of a state.
	 *
	 * @param key    Value to retrieve.
	 * @param value  Cached value (the last value set at this key).
	 *
     * @return Potentially modified state value.
	 */
	virtual const char * GetState( const char * key, const char * value ) { (void)key; return value ; }

	/**
	 * Handle QueryRestartMode()
	 *
	 * @return True to request a restart.
	 */
	virtual bool QueryRestartMode(){ return false; }

	/**
     * Handle QueryJumpToMenu()
	 *
     * @return True to jump to the main menu.
	 */
	virtual bool QueryJumpToMenu(){ return false; }

	/**
     * Handle QueryMuteGame()
	 *
     * @return True to mute the game.
	 */
	virtual bool QueryMuteGame(){ return false; }

	/**
     * Allow the handler to force the pause of the game; this is
     * handled internally by Playground.
	 *
	 * @return True to pause the game
	 */
	virtual bool QueryPauseGame(){ return false; }
};

#endif // GAMESTATEHANDLER_H_INCLUDED

