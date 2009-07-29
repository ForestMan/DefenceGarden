//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef MSNZONE_H_INCLUDED
#define MSNZONE_H_INCLUDED
/**
 * @condfile
 * Hooks for MSN Zone Web Game Support
 *
 * @deprecated These functions should no longer be called
 *             directly; instead, the equivalent functions in
 *             TGameState functions should be called.
 *
 */

/**
 * To be called at the end of a game session when the player's score is final.
 *
 * @param score - number of points the player has earned
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 *
 */
void PFLIB_API MsnZoneScoreSubmit(unsigned long score);

/**
 * To be called when game is finished - at this point MSNZone might switch
 * to an upsell screen
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 *
 */
void PFLIB_API MsnZoneGameEnd();

/**
 * To be called each time through the game loop. The score should be the
 * total number of points the player has earned in the game session.
 *
 * @param score - total number of points the player has earned
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 */
void PFLIB_API MsnZoneScoreBroadcast(unsigned long score);


/**
 * To be called whenever the level changes. It is ok to call this
 * even when the level has not changed (i.e. each time through the game
 * update loop).  Sublevel is optional, but applies to some games.
 *
 * @param level - current level the player is on
 * @param sublevel - current sublevel the player is on
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 */
void PFLIB_API MsnZoneLevelUpdate(int level, int sublevel = 0);


/**
 * To be called whenever the player reaches the end of the
 * available free content in the web version of a game.
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 */
void PFLIB_API MsnZoneMaxLevelReached();


/**
 * To be called at an appropriate time for the game to take a break
 * (i.e. show an ad). This should be called at the end of a level,
 * or when an event happens where play can be stopped.
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 */
void PFLIB_API MsnZoneGameBreak();

/**
 * To be called when the user has chosen a game mode and the game is
 * set up and ready to begin play.
 *
 * @param mode - name of game mode that is about to be played
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 */
void PFLIB_API MsnZoneGameReady(const char* mode);

/**
 * To be called repeatedly as the game assets are loading.
 * Must be called with 100 percent before MsnZoneSessionReady()
 * can be called.
 *
 * @param percent - percentage of assets loaded.
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 */
void PFLIB_API MsnLoadBroadcast(int percent);

/**
 * To be called when the application has been initialized and assets
 * have been loaded, but before splash screens are displayed.
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 */
void PFLIB_API MsnZoneSessionReady();

/**
 * To be called when the user has pressed a download button. This should be
 * called INSTEAD of launching an actual web page.
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 */
void PFLIB_API MsnZoneDownloadButton();

/**
 * To be polled each time through the main loop.
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 *
 * @return  - If this function returns true, the app must stop what it is doing
 *			  and return to the main game menu
 */
bool PFLIB_API IsMsnZoneGameMenuRequested();

/**
 * To be polled each time through the main loop.
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 *
 * @return  - If this function returns true, the app must stop what it is doing
 *			  and restart the current game mode.
 */
bool PFLIB_API IsMsnZoneRestartGameRequested();

/**
 * To be called when the game play has been paused or resumed
 *
 * @param bPause - true if the gameplay has been paused, false if it has been resumed
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 */
void PFLIB_API MsnZoneGamePaused(bool bPause);

/**
 * Used to sync in game muting with external muting. Call this function to
 * mute the game in MSN and Zylom versions instead of your normal muting method.
 *
 * @param bMuteFromMenu - true if game is muted, false if game is not muted
 *
 * @deprecated Use TGameState instead to handle game muting!
 */
void PFLIB_API SetMute(bool bMuteFromMenu);

/**
 * Used to test if MSN is enabled. Can be used instead of searching
 * the command line for -msn
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 *
 * @return  - true if MSN is enabled
 *
 */
bool PFLIB_API MsnZoneIsEnabled();

/********************************************************************************
*
* The following hooks are for the Zylom API
*
********************************************************************************/

/**
 * Used to test if Zylom is enabled. Can be used instead of searching
 * the command line for -zylom
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 *
 * @return  - true if Zylom is enabled
 *
 */
bool PFLIB_API ZylomIsEnabled();

/**
 * Gets Zylom's upsell screen frequency. The game should show an upsell
 * screen at the end of any level divisible by this number when in Zylom mode.
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 *
 * @return  - number of levels between upsell screens, -1 if not set.
 *
 */
int PFLIB_API GetUpsellLevelFrequency();

/**
 * Gets Zylom's maximum level cap. Game should end in Zylom mode if
 * this level is beaten.
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 *
 * @return  - number of levels of gameplay allowed, -1 if not set.
 *
 */
int PFLIB_API GetMaxLevelCap();

/**
 * Used to determine if an upsell screen displayed after the game has ended, replaces
 * the ok button with a restart button that sends Zylom a restart message.
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 *
 * @return  - true if an upsell screen with a restart button should be displayed
 *
 */
bool PFLIB_API ZylomDisplayRestart();

/**
 * To be called when the user has pressed the restart button on the upsell screen.
 *
 * @deprecated Use TGameState to handle all ActiveX integration.
 */
void PFLIB_API ZylomRestart();


//------------------------------------------------------------------
#endif // MSNZONE_H_INCLUDED
