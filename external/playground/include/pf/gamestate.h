//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#ifndef TAGGING_IGNORE
class TGameStateData;
class TGameStateHandler;
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

/**
 * An object that allows the game to communicate state
 * information to the system. This allows the system to, for
 * example, interact with MSN or Zylom web hooks. In
 * Playground-published games this system is also used to
 * collect game metrics.
 *
 * @see \ref firstpeekgamestate
 */
class PFLIB_API TGameState
{
private:
	/// Private constructor; construct using GetInstance().
	TGameState();

public:
	virtual ~TGameState();

	/// Get an instance to the global game state object.
	static TGameState * GetInstance();

	/// Key to pass command line into attached game monitor.
	/// This allows plug-in to interpret its own command line
	/// options.
	///
	/// This option is handled by Playground; there's no need for a game to
	/// use it.
	static const char * kCmdLine ;

	/// State key for setting current game level. If your game has no concept
	/// of levels, then set this to a constantly increasing value.
	static const char * kLevel ;

	/// State key for setting current game sub-level. (optional)
	static const char * kSubLevel ;

	/// State key to let the system know your game has been paused or
	/// resumed. Value is non-zero for pause and zero for resume.
	static const char * kPause ;

	/// State key to indicate the game is in MUTE state.
	/// Use this to enable or disable muting from within the game.
	/// Monitor QueryMuteGame() to determine actual game mute settings.
	static const char * kMute ;

	/// Key to signify that the game is at a pause in the action and
	/// can be stopped. This should ideally be triggered at the end
	/// of each level. Value is ignored.
	static const char * kBreakNow ;

	/// State key for score. In games with no score, this can
	/// be safely ignored. Call this function every game loop.
	static const char * kScore ;

	/// State key for game mode. Should be set when user has selected
	/// a mode and is about to start playing. Value should be a one 
	/// word description of the game mode, like "Story" or "Endless".
	static const char * kGameMode ;

	/// State key for game over. Value is ignored. Be sure to set the
	/// final score value BEFORE calling kGameOver.
	static const char * kGameOver ;

	/// State key for no more free content in a Web version. Value is ignored.
	/// Set this after the player has beaten the last level available in a
	/// trial version of the game or has otherwise played past the end of the
	/// available content.
	static const char * kNoMoreContent ;

	/// Download button pressed: In a Web game, there will be a "download (and buy) the real
	/// game" button. When it's pressed, call SetState(kDownloadButton).
	static const char * kDownloadButton ;

	/// The frequency you should use to display upsell screens. The game should show an upsell
	/// screen at the end of any level divisible by this number. Default is 0, which indicates
	/// that the game should use its default upsell frequency.
	static const char * kUpsellFrequency ;

	/// Query this value to determine whether a restart button should be displayed
	/// instead of an "OK" button on an upsell screen. In the case that you've displayed
	/// a restart button, call SetState() with this value to indicate the restart button has
	/// been pressed.
	static const char * kRestartButton ;

	/**
	 * Key used to query the current system mode(s). Return
	 * result may have several modes separated by commas, including
	 * "web", "msn", "zylom", "metrics", etc.
	 */
	static const char * kSystemMode ;

	/**
	 * Key used to query what level the game should cap out at;
	 * used by some web providers to vary how many levels can be
	 * played. Default is -1, which indicates no limit.
	 */
	static const char * kMaxLevel ;

	/**
     * Set a numeric key value.
     *
     * To time states in a FirstPeek build, set a state to "1" when
     * active and "0" when inactive. States always set in this
     * manner will have their time written to the FirstPeek file.
     *
	 * @param key		Key to set.
	 * @param value		Value to set it to.
	 */
	void SetState(str key, int32_t value=0);

	/**
     * Set a string key value.
	 *
	 * @param key		Key to set.
	 * @param value		Value to set it to.
	 */
	void SetState(str key, str value);

	/**
	 * Get a string key value.
	 *
	 * @param key    Key to read.
	 *
	 * @return Current state of key.
	 */
	str GetState( str key );

	/**
	 * Query this function frequently; if it ever returns
	 * true, immediately restart the current game mode at level 1.
	 *
	 * @return true to restart game mode.
	 */
	bool QueryRestartMode();

	/**
	 * Query this function frequently; if it ever returns
	 * true, immediately exit the current game mode and
	 * bring up the main menu.
	 *
	 * @return true to go to menu.
	 */
	bool QueryJumpToMenu();

	/**
     * Query this function to determine the mute state displayed in
     * the game.
     *
     * As your user modifies the mute state within the game, you
     * should call SetState() with kMute to notify the system of the
     * change.
     *
     * @return true to mute game.
	 */
	bool QueryMuteGame();

	/**
	 * TPlatform::GetEvent() will handle QueryPauseGame(); no user
	 * code is required to handle it correctly.
	 *
	 * @return True to pause game; false otherwise.
	 */
	bool QueryPauseGame();

	/**
	 * Register a game state handler.
	 *
	 * @param handler Handler to register.
	 */
	static void RegisterHandler( TGameStateHandler * handler );

private:
	TGameStateData * mData ;
};

#endif //GAMESTATE_H_INCLUDED

