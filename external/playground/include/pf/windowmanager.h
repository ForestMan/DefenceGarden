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
 * @defgroup windowgroup Windowing and Widget Functionality
 *
 * Everything related to windows and their descendents: buttons,
 * sliders, image windows, and custom windows.
 */

/**
 * @condfile
 * Interface for class TWindowManager
 *
 * \ingroup windowgroup
 */



#ifndef WINDOWMANAGER_H_INCLUDED
#define WINDOWMANAGER_H_INCLUDED

#include <set>
#include <map>

#ifndef WINDOW_H_INCLUDED
#include "window.h"
#endif

#ifndef MODALWINDOW_H_INCLUDED
#include "modalwindow.h"
#endif

#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

#ifndef TAGGING_IGNORE
class TWindowScript ;
class TMessageListener ;
class TLuaFunction ;
class TScript ;
class TWindowCreator ;
class TWindowStyle ;
class TLuaTable ;
#endif


/**
 * A callback that receives notification that a window has
 * had the mouse hover over it.
 *
 * @see TWindowManager::AdoptHoverHandler
 * \ingroup windowgroup
 */
class TWindowHoverHandler
{
public:
	virtual ~TWindowHoverHandler(){}

    /**
     * Abstract virtual function for handling hover events.
     *
     * @param window Window that mouse is hovering over.
     * @param point  Point in window client coordinates.
     *
     * @return True if event handled; false to continue processing.
     */
	virtual bool Handle(TWindow * window, const TPoint & point)=0;
};

/**
 * The TWindowManager class manages, controls, and delegates
 * messages to the window system.
 *
 * TWindowManager contains a stack of TModalWindows, the top of
 * which is considered to be the active window in the system. If
 * the a TModalWindow covers the entire viewable area and is
 * flagged as opaque, only the top modal window and its children
 * are drawn.
 *
 * The main message pump hands messages to TWindowManager using
 * TWindowManager::HandleEvent(), which then dispatches the
 * event to the appropriate listener(s).
 *
 * \ingroup windowgroup
 */
class PFLIB_API TWindowManager
{
public:
	/**
     * @name Construction, Destruction, and Singleton Access
	 * @{
	 */

	/// Default Constructor
	TWindowManager();

	/// Destructor
	virtual ~TWindowManager();

    /**
     * Get the global TWindowManager instance.
     *
     * @return A pointer to the TWindowManager.
     */
    static TWindowManager * GetInstance();

    /*@}*/
	/**
	 * @name Message handling.
	 * @{
	 */

    /**
     * Post a message to the queue. Takes ownership of the message,
     * and will expect to be able to delete the message when it has
     * been delivered.
     *
     * @param message Message to post.
     */
	void PostWindowMessage( TMessage * message );

	/**
	 * Add a message listener that will be able to receive messages
	 * that aren't targeted at a specific window.
	 *
	 * @param messageListener
	 *               Listener to adopt. Will be destroyed by TWindowManager unless
	 *               it is orphaned prior to the destruction of the TWindowManager.
	 */
	void AdoptMessageListener( TMessageListener * messageListener );

	/**
	 * Remove a message listener from the TWindowManager.
	 *
	 * @param messageListener
	 *               Listener to remove.
	 *
	 * @return True if it was found and removed; false otherwise. If true
	 *         it's safe to delete, otherwise it's been deleted already.
	 */
	bool OrphanMessageListener( TMessageListener * messageListener );

	/*@}*/

#if !USER_DOCS
	/**
	 * @nouser
	 * Disable all drawing.
	 *
	 * @param disable True to disable drawing.
	 */
	void	DisableDrawing( bool disable ) { mNoDraw = disable; }

	/**
	 * @nouser
	 * Return whether drawing is disabled
	 *
	 * @return True if drawing disabled.
	 */
	bool	IsDrawingDisabled(){ return mNoDraw ; }

	/**
	 * @nouser
	 * Disable all input.
	 *
	 * @param disable True to disable input.
	 */
	void	DisableInput( bool disable ) { mNoInput = disable; }

	/**
	 * @nouser
	 * Return whether input is disabled
	 *
	 * @return True if input disabled.
	 */
	bool	IsInputDisabled(){ return mNoInput ; }

	/**
	 * @nouser
	 * Dispatch a message to a window
	 */
	void DispatchWindowMessage( TMessage * message );

#endif

	/**
	 * @name Top-level Screen Related Functions
	 * @{
	 */

	/**
	 * Get the global screen object (the top level application
	 * TWindow).
	 *
	 * @return A pointer to the application TScreen.
	 */
	class TScreen * GetScreen() { return mScreen; }

	/**
	 * Mark the current screen as needing to be redrawn.
	 */
	void InvalidateScreen();

	/**
	 * Set the screen to a particular TScreen and initializes it to be the
	 * correct size. Also calls Init() on the screen, since no one will be
	 * calling AdoptChild() with the TScreen as a parameter.
	 *
	 * This function can only be called once in a Playground game. It is called
	 * internally by the library when you first call TPlatform::SetDisplay(),
	 * so if you want to create a custom-derived TScreen then you need to
	 * add it before you call SetDisplay() the first time.
	 *
	 * @param screen Screen to set to be the one-and-only top level screen class.
	 */
	void SetScreen( TScreen * screen ) ;

#if !USER_DOCS

	/**
	 * @nouser
	 * Get the current background cache texture.
	 *
	 * @return A pointer to the background cache texture.
	 */
	TTextureRef GetCache() { return mCache; }
#endif

	/*@}*/

	/**
	 * @name Modal window handling and supporting functions.
	 * @{
	 */

	/**
	 * Push a modal window onto the modal window stack.
	 *
	 * @param w      Window to push.
	 */
	void    PushModal(TModalWindow * w);

    /**
     * Pop a modal window off the modal window stack. Safe
     * to do at any time--window will be deleted at next
     * event.
     *
     * @param windowName Name of the window to pop. Will pop that window and any
     *                   of its descendents from the stack, if found.
     */
	void	PopModal( str windowName );

	/**
	 * Get the return value from a modal window.
	 *
	 * @return A string return value.
	 */
    str     GetModalReturnStr();

	/**
	 * Get the return value from a modal window.
	 *
	 * @return An integer return value.
	 */
    int32_t GetModalReturnInt();

	/**
	 * Display a modal dialog box.
	 *
     * @param dialogSpec	Handle to a Lua dialog specification
     *                      file.
     * @param body          String to be placed in the dialog body.
     *                      Selects style DialogBodyText and sets
     *                      gDialogTable.body to the given body. The
     *                      Lua dialog specification can then use
     *                      gDialogTable.body to set the text of the
     *                      body of the dialog.
     * @param title			Title of dialog. Selects style
     *                      DialogTitleText and adds the text to the
     *                      dialog as gDialogTable.title.
     * @param name			The name to be given to the resulting
     *                      dialog window.
	 *
	 * @return A pointer to the dialog. The dialog will already have been pushed as the top modal window,
	 * 			but you may need this pointer to set additional fields.
	 */
	class TDialog * DisplayDialog( str dialogSpec, str body, str title, str name="" );

	/**
     * Get the current top-most modal window. Note that this window
	 * may have some number of children--this is just the modal
	 * window that is currently receiving the processing.
	 *
     * @return A reference to the top-most modal window.
	 */
	class TModalWindow* 	GetTopModalWindow();

    /**
     * Enable "Draw Top Modal Window Only" mode. Only the top layer
     * will be drawn when true.
     *
     * @param enable True to enable.
     */
    void		SetTopModalOnly( bool enable );
    /*@}*/

	/**
	 * @name Pop-up help handling
	 *
	 * Customize the default pop-up help features of your application.
	 *
	 * @{
	 */

    /**
     * Set the current pop-up default help handler.
     *
     * To add pop-up help to your application, you can either override individual
     * TWindow::OnMouseHover handlers, or you can allow TWindow to call the default
     * handler, which you can set using this function.
     *
     * The previous handler, if any, is deleted when you call this function.
     *
     * @param handler A handler to add.
     */
	void AdoptHoverHandler( TWindowHoverHandler * handler );

	/**
	 * Get the current pop-up help handler.
	 *
	 * @return A pointer to the current handler, if any.
	 */
	TWindowHoverHandler * GetHoverHandler()
	{
		return mHoverHandler ;
	}

#if !USER_DOCS
	/**
	 * Handle a mouse hover event.
	 *
     * @param window The window that the mouse is hovering over.
     * @param point  The point within window the mouse is hovering
     *               over.
	 *
	 * @return True if handled.
	 */
	bool OnMouseHover( TWindow * window, const TPoint & point );
#endif
	/*@}*/

	/**
	 * @name Overlay Window management.
	 *
	 * An overlay window allows you to draw to a window that lives "on top" of the
	 * hierarchy.
	 *
	 * @{
	 */

	/**
	 * Add an overlay window to the TWindowManager. As always, the "Adopt"
	 * semantics implies ownership, so when TWindowManager is destroyed, it will
	 * attempt to delete this window. To prevent this behavior, call OrphanOverlayWindow
	 * to release it from TWindowManager.
	 *
	 * An overlay window allows you to draw to a window that lives "on top" of the
     * hierarchy. Set the window rectangle to the area that should be redrawn next frame.
     *
     * If you want your overlay window to be able to process mouse
     * messages, you need to globally enable the feature by calling:
     *
     * @code
     * TPlatform::SetConfig(TPlatform::kOverlayWindowMouseEvents,"1");
     * @endcode
     *
     * ...in your program initialization code.
     *
     * @param overlay An overlay window to add.
	 */
	void AdoptOverlayWindow( TWindow * overlay );

	/**
	 * Release an overlay window from the TWindowManager.
	 *
	 * @param overlay Window to release.
	 *
	 * @return True if it was found and released. False if it was not found.
	 */
	bool OrphanOverlayWindow( TWindow * overlay );

	/*@}*/

	/**
     * @name Event Management and Routing
     *
     * Member functions that manipulate how messages our routed
     * through the system.
     *
	 * @{
	 */

    /**
	 * Handle a system event. Processes the event and passes it
     * along as a message or a callback to the appropriate window.
     * Typically called in the main loop message pump.
     *
	 * @param e      Event.
	 */
	void HandleEvent(TEvent* e);

	/**
	 * Capture the mouse and other input events.
	 *
	 * Implemenation is low-overhead, and so can be safely called in OnMouseMove().
	 * If you request capture a second time with the same window pointer, the
     * new window will not be added to the list of listeners, so a
     * window that wants capture does not need to remember whether
     * it has called AddMouseListener() already--it can just add
     * itself again.
     *
     * Events are dispatched to all registered mouse listeners,
     * regardless of return values from handled functions.
	 *
     * @param window Window that wants to receive all mouse events.
	 *
	 */
	void AddMouseListener( TWindow * window );

	/**
     * Stop listening to all mouse messages.
	 *
	 * @param window Window to release from capturing the mouse.
     *               Silently fails if \c window is not currently a
     *               mouse listener.
	 */
	void RemoveMouseListener( TWindow * window );

	/**
	 * Set the window that is to receive the keyboard focus.
	 *
	 * Note that the window will lose the focus if someone clicks unless
	 * it has the style kFocusTarget. On construction of a window that is
	 * to receive the focus, call
	 *
	 * @code
	 * 		AddWindowType( kFocusTarget );
	 * @endcode
	 *
	 * ...and this will flag that window as being able to accept focus
	 * when clicked. Otherwise focus goes it its nearest kFocusTarget
	 * ancestor, or is delegated to the default-focus defined by the
	 * window's parent modal.
	 *
	 * @param focus  New focus window.
	 */
	void SetFocus(TWindow * focus)
	{
		TWindow * oldFocus = mFocusWindow;
		mFocusWindow = focus;
		if (mFocusWindow)
		{
			mFocusWindow->OnSetFocus(oldFocus);
		}
	}

	/**
	 * Get the window that is currently receiving keyboard events.
	 *
	 * @return A reference to the current focused window.
	 */
	TWindow * GetFocus() { return mFocusWindow; }

	/*@}*/

	/**
     * @name Lua GUI Script Access
     *
     * Functions that access and manipulate the Lua GUI script
     * supplied by TWindowManager.
     *
	 * @{
	 */

	/**
     * Get the current TWindowManager GUI script.
	 *
	 * @return A pointer to the current script.
	 */
	TScript*	GetScript();

    /**
     * Use a Lua Script in an external resource to populate a Window.
     *
     * @param window   Window to apply script to.
     * @param filename Name of Lua file.
     */
	void		RunScript(TWindow* window, const char* filename);

    /**
     * Use a Lua Script in a str to populate a Window.
     *
     * @param window Window to apply script to.
     * @param script Lua commands to run.
     */
	void		DoLuaString(TWindow* window, str script);

    /**
     * Dispatch a message to the GUI script.
     *
     * @param message Message to pass to Lua.
     * @param command Command to pass to Lua to execute in GUI thread.
     */
    void		OnScriptMessage(TMessage* message, TLuaFunction * command=NULL);

    /**
     * Add a custom-defined window type to the script context.
     *
     * @param command Window type name.
     * @param classId The TWindow::ClassId() of the custom defined
     * 				  window. Note that the window needs to have
     * 				  PFTYPEDEF_DC() in the header and PFTYPEIMPL_DC()
     *                in the implementation file for this to work.
     */
    void		AddWindowType( str command, PFClassId classId );

  	/**
	 * Toggle on/off using the string table to convert labels found in LUA
	 * to properly localized strings (see TStringTable for more information).
	 *
	 * @param bEnable true to use the table, false to not use the table
	 *
	 * @return returns true if string table was previously enabled, false otherwise
	 */
	bool		EnableStringTable( bool bEnable );

	/*@}*/

	/**
     * @name Utility Functions
     *
     *
     *  @{
	 */

    /**
     * Convenience function that creates a TText child of the given
     * window, using the given bodyText, in the given Lua style.
     *
     * @param window   The window to add a new TText child to.
     * @param bodyText Text to add.
     * @param style    Name of the Lua style to use (must already be loaded in
     *                 the TWindowManager::GetScript() Lua script),
     *                 or a style definition in curly brackets.
     */
	void 		AddText( TWindow* window, str bodyText, str style );

	/**
	 * Safely destroy a window at the beginning of an event loop.
	 * Allows you to mark a window for destruction when processing
	 * an event that may need to continue accessing the window.
	 *
	 * @param window Window to destroy.
	 */
	void		SafeDestroyWindow(TWindow * window );

    /** @} */
private:
	TWindow::WindowList::iterator NextListenerWindow( TWindow::WindowList::iterator w );
	void	_PopModal();
	friend class TPlatform;
	/**
	 * Attach a top-level window script to the window manager.
	 * TWindowManager takes ownership of the script and will
	 * destroy it when it is destroyed.
	 *
	 * @warning Note that only one TWindowScript can be added to TWindowManager.
	 *
	 * @param script Script to attach.
	 */
	void AttachWindowScript( TWindowScript * script );

private:
	void ClearDestroyedWindows();

public:
#if !USER_DOCS
	/**
	 * @nouser
	 * Destroy dirt.
	 */
	void OnDestroyWindow( TWindow * w );

	void ResetStackDirty() { mStackDirty = false; }

	/**
	 * @nouser
	 * Draw the entire window stack from the bottom up. Called in response to a
	 * kRedraw event.
	 *
	 * @internal
	 */
	void DrawStack();
#endif
public:
#if !USER_DOCS
	/**
     * @nouser
	 * Draw one of the layer types.
	 *
	 * @param mode   Draw mode for the layer.
	 */
	void DrawLayer( TWindow::EDrawMode mode );
public:
	/**
	 * @nouser
	 *
	 * Dispatch any queued window messages.
	 */
	void DispatchMessages(void);
#endif
private:
	typedef std::list<TModalWindow *>	ModalWindowList ;
	typedef std::set<TWindow *>			WindowSet ;

	TTextureRef				mCache;
	TScreen					*mScreen;
	bool					mStackDirty ;
	uint32_t				mLastDraw ;

	ModalWindowList 		*mModalWindowStack;
	TWindow::WindowList 	*mDestroyedWindows;
	TWindow::WindowList 	*mOverlayWindows;

	bool					mNoDraw;
	bool					mNoInput;
	bool					mTopModalOnly ;
	bool					mScreenDirty ;
	bool					mShowMouseWindow ;

	bool					mCaptureWindowListDirty ;
	TWindow					*mMouseWindow;

	TWindow::WindowList 	mCaptureWindow ;

	TWindow					*mFocusWindow;

	TWindowScript			*mScript ;
    bool					mFullScreenBlit;
    std::list<TMessage*>    mMessageList ;

	std::list<TMessageListener*> mMessageListeners ;

	TWindowHoverHandler		*mHoverHandler ;
};

#endif // WINDOWMANAGER_H_INCLUDED
