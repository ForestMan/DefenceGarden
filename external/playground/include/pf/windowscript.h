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
 * Interface for class TWindowScript
 *
 * \ingroup windowgroup
 *
 */

#ifndef WINDOWSCRIPT_H_INCLUDED
#define WINDOWSCRIPT_H_INCLUDED

#include <vector>
#include <map>
#include <set>

#ifndef SCRIPT_H_INCLUDED
#include "script.h"
#endif

#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

#ifndef PFTYPEINFO_H_INCLUDED
#include "pftypeinfo.h"
#endif

#ifndef WINDOW_H_INCLUDED
#include "window.h"
#endif

class TFlashHost;
class TMessageListener;
class TWindowStyle;

#if !USER_DOCS
// TWindowScript is becoming private

/**
 * A script that creates windows, monitors window messages,
 * and supplies supporting window creation functionality for
 * use in user-created window types.
 *
 * @see \ref luacustomwindow
 *
 * \ingroup windowgroup
 */
class PFLIB_API TWindowScript : public TScript
{
public:
	class TWindowCreateNotify
	{
	public:
		virtual ~TWindowCreateNotify() {}
		virtual void NotifyCreate( TWindow * newWindow, const TWindowStyle & style )=0;
	};

	/**
	 * Default Constructor.
	 */
	TWindowScript(lua_State* luaState=NULL);

	/// Destructor
	virtual ~TWindowScript();

	/**
	 * Script message handling
	 *
	 * @param message Message to handle.
	 * @param command Command to execute
	 *
	 * @return True if message handled. False otherwise.
	 */
	bool OnMessage( TMessage * message, TLuaFunction * command=NULL );

	/**
	 * Get the current top window. During construction, this is
	 * the topmost window currently being built. After construction
	 * is complete, this is the window that is the parent of the
	 * TWindowScript.
	 *
	 * @return The internal topmost window.
	 */
	TWindow * GetTopWindow() { return mWindowStack.back(); }

	/**
	 * Push a window onto the internal stack. The top window on the stack is
	 * the one that gets controls if you run a dialog-creation Lua script.
	 *
	 * @param window Window to push.
	 */
	void PushWindow( TWindow * window );

	/**
	 * Pop a window from the internal stack.
	 */
	void PopWindow();

	/**
	 * Set the current top level window name.
	 *
	 * @param name   New window name
	 */
	void SetWindowName( str name );

	/**
     * Reset to a new modal window
	 *
     * @param window Window to use as new window base.
	 */
	void NewModalWindow( TWindow * window );

#if !USER_DOCS
	/**
	 * @nouser
	 * Adjust the x or y values based on parent window size.
	 */
	static void _AdjustPos( int * pos, int size, int bound );
	static void _AdjustMaxPos( int * pos, int * size, int bound );
#endif

	int SetStyle(lua_State * L);		///< Set the current style

    void SetFocus( str focus );         ///< Set the focal window by name

	void PostChildCreate(TLuaTable * table, str windowtype);

	/**
	 * Create a window.
	 *
	 * @param L      Lua state.
	 *
	 * @return Number of return values (0)
	 */
	int CreateGenericWindow(lua_State * L);

	/**
	 * Function that centers the current window within its parent.
	 */
	void DoCenterToParent();

	/**
	 * Function to push a basic TModalWindow onto the window stack.
	 * Used by Lua script to push, e.g., a dialog onto the stack.
	 *
	 * Returns a unique string handle to the window.
	 */
	str PushModal( str name );

	/**
	 * Clear all children for the top modal window.
	 */
	void  ClearModalChildren();

	void FillRect( TLuaTable * rect, TLuaTable * color );
	/**
	 * Add a custom-defined window type to the script context.
	 *
	 * @param command Window type name.
	 * @param classId The TWindow::ClassId() of the custom defined
	 * 				  window. Note that the window needs to have
	 * 				  PFTYPEDEF_DC() in the header and PFTYPEIMPL_DC()
	 *                in the implmentation file for this to work.
	 */
	void AddWindowType( str command, PFClassId classId );

	/**
	 * Toggle on/off using the string table to convert labels found in LUA
	 * to properly localized strings (see TStringTable for more information).
	 *
	 * @param bUse true to use the table, false to not use the table
	 *
	 * @return returns true if string table was previously enabled, false otherwise
	 */
	bool EnableStringTable( bool bEnable );

	/**
	 * Inject a function into a running Lua script. Default
	 * implementation is empty, but the TWindowManager::GetScript()
	 * script has a derived implementation.
	 *
	 * Attempts to inject the function on the top of the Lua stack
	 * into the current coroutine.
	 *
	 * @return True on success, false on failure.
	 */
	virtual bool InjectFunction();

	void SelectLayer( int32_t layer );

#if !USER_DOCS
	void CloseRequested();
	/**
	 * Add a callback that gets called after <i>any</i> window is created
	 * by a Lua script.
	 */
	void SetWindowCreateNotify( TWindowCreateNotify * notify )
	{
		mCreateNotify = notify ;
	}
private:
	void RegisterHotkey( str hotkey, str buttonName );
	int StripKeyFromLabel( lua_State * L );
	str TagHotKey( str key, str label, str name );
	bool EnableWindow( str name, bool enable );
	int  GetWindowDepth();
	TMessage * CreateCloseMessage();
	str LookupString( str id, str p1, str p2, str p3, str p4, str p5 );
#endif
protected:
	/** \name Lua Creator Functions
	 *
	 * Functions that are usable only by a derived Window-builder
	 * class.
	 *
	 *  @{
	 */

	/**
	 * Do the standard window setup.
	 *
	 * Sets the position, width, height, and some other properties
	 * of the window.
	 *
	 * Indirectly calls Init() on the window as part of its
	 * setup.
	 *
	 * @param window Window to initialize.
	 *
	 * @return True on success.
	 */
	bool InitWindow( TWindow * window ) ;
	/** @} */
	void FitToChildren();
	void UpdateWindow();
	void DestroyWindow( str window );

private:
	bool GetButtonToggleState( str name );
	void SetButtonToggleState( str name, bool on );

    // Post a message to the window message queue
    int PostWindowMessage( lua_State * L );
    // Post a message to the window message queue, targetted at the parent
    int PostWindowMessageToParent( lua_State * L );

	/**
	 * Get a label from a TText-derived window
	 *
	 * @param name   Name of the window to query.
	 *
	 * @return A window label.
	 */
	str GetLabel( str name );

	/**
	 * Set a label for a TText-derived window
	 *
	 * @param name   Name of the window to query.
	 * @param label  New label value.
	 */
	void SetLabel( str name, str label );

	/**
	 * Set a window position
	 *
	 * @param name   Name of window to reposition.
	 * @param x      New x position.
	 * @param y      New y position.
	 */
	void SetPosition( str name, int32_t x, int32_t y );

	/**
	 * Set the command for a button.
	 *
	 * @param name   Name of button to reset.
	 * @param command  New Command
	 */
	int SetCommand( lua_State * L );

	/**
	 * Set the bitmap on a window.
	 *
	 * @param name
	 * @param image
	 */
	void SetBitmap( str name, str image, TReal scale );

	int CreateCustomWindow( lua_State * L );
	/**
	 * Handle the GetMessage() call from Lua.
	 *
	 * @param L      Lua state.
	 *
	 * @return Lua C callback return.
	 */
	int HandleGetMessage( lua_State * L );


	/**
	 * Handle the StartMovie() call from Lua.
	 *
	 * @param movie      Path to movie.
	 * @param transalte  Whether or not to translate the movie
	 * @param allowInput Whether or not the movie accepts mouse input
	 *
	 * @return true on success, false on failure
	 */
	bool StartMovie( str movie, bool translate = false, bool allowInput = false );

	/**
	 * Handle the MoviePlaying() call from Lua.
	 *
	 *
	 * @return true if a movie is currently playing, false if not
	 */
	bool IsMoviePlaying();

	/**
	 * Handle the StopMovie() call from Lua.
	 *
	 */
	void StopMovie();

	/**
	 * Check to see if a TImage successfully loaded from a Lua script
	 *
	 * @param image      Name of TImage window to check.
	 *
	 * @return true on success, false on failure
	 */
	bool ImageLoaded(str image);


	enum EScriptState
	{
		eScriptOver,
		eScriptPaused,
		eScriptRunning,
		eScriptGetMessage
	};

	std::vector<TWindow*>	mWindowStack ;
	std::map<str,PFClassId>	mCustomWindows ;
	EScriptState			mScriptState ;

	TWindowCreateNotify		*mCreateNotify ;

	bool					mUseStringTable;

	TFlashHost				*mFlashHost;

};
#endif

#endif // WINDOWSCRIPT_H_INCLUDED
