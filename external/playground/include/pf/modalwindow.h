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
 * Interface for class TModalWindow
 */


#ifndef MODALWINDOW_H_INCLUDED
#define MODALWINDOW_H_INCLUDED

#ifndef WINDOW_H_INCLUDED
#include "window.h"
#endif

#ifndef TASK_H_INCLUDED
#include "task.h"
#endif

#ifndef CLOCK_H_INCLUDED
#include "clock.h"
#endif

#ifndef TAGGING_IGNORE
class TTaskList;
#endif

#include <set>

/**
 * Base class for any window that can be a modal window.
 *
 * Adds a number of functions to the default TWindow:
 *
 *   - Handles tasks that are associated with a modal window.
 *   - Manages a TClock that automatically gets paused when the
 *     modal is covered by another modal.
 *   - Does some internal bookeeping relevant to the window
 *     stack and opaque full-screen windows.
 *   - Handles dispatching key messages to a default focus
 *     window.
 */
class PFLIB_API TModalWindow : public TWindow
{
	PFTYPEDEF(TModalWindow,TWindow);
public:
	/**
	 * @name Construction/destruction
	 * @{
	 */
	/**
	 * Default constructor.
	 *
	 * Sets window type to include kModal.
	 */
	TModalWindow();

	/**
	 * Destructor.
	 */
	virtual ~TModalWindow();

	/*@}*/

	/**
	 * @name Modal Window Task Handling
	 * @{
	 */

	/**
	 * Add a task to the modal window's task list. Task list takes ownership of the
	 * task and will delete it when the task notifies that it is complete.
	 *
	 * @param task	Task to add.
	 */
	void AdoptTask( TTask * task );

	/**
	 * Remove a task from the task list. Does not
	 * delete the task, but rather releases ownership;
	 * calling function now owns task.
	 *
	 * @param task   Task to remove.
	 * @return true if task was removed, false if task was not found
	 */
	bool OrphanTask( TTask * task );

	/**
	 * Destroy all tasks this window owns. Used when the window is
	 * about to be destroyed.
	 *
	 * Destruction is "safe"--tasks will be added to a destroy list
	 * if the list is being iterated.
	 */
	void DestroyTasks();

	/*@}*/

	/**
     * @name Focus and Key Handling
	 *
	 * @{
	 */
	/**
	 * Set the window that will receive an "implicit" focus when
	 * no other window has the focus.
	 *
	 * @param focus  Default focus target. NULL to remove the default focus.
	 */
	void SetDefaultFocus( TWindow * focus );

    /** Get the current default focus */
    TWindow * GetDefaultFocus() { return mDefaultFocus; }

	/**
	 * Add a hot key to the current modal window.
	 *
	 * When someone presses the given key, the given button
	 * name will be pressed.
	 *
	 * To create more arbitrary commands that don't press
	 * visible buttons, create invisible buttons
	 * that can be "pressed" by name.
	 *
	 * @param key    Key to bind this button to. Can include:
	 *
	 *               - Ctrl+ for control, e.g., "Ctrl+C" would be control-c.
	 *               - Alt+ for the alt key, e.g., "Alt+C" would be alt-C.
     *               - Shift+ for the shift key, e.g., "Shift+Alt+C" would be shift-alt-C.
     *
     *               If the key starts with '@', then ANY modifiers
     *               will be accepted, e.g., "@A" would work for an
     *               (uncaptured) "A", "Ctrl-A", "Alt-A", etc.
     *
     * @param buttonToPress The name of the button to press. An
     *                      empty string will delete the key
     *                      association.
	 */
	virtual void AddHotkey( str key, str buttonToPress );

	/**
     * Raw key hit on keyboard: TModalWindow will check to see if
     * it's a "hot" key, and if so, then trigger the appropriate
     * child button.
	 *
	 * @param key    Key pressed on keyboard.
	 * @param flags  TEvent::EKeyFlags mask representing the state of other keys on the keyboard
	 *               when this key was hit.
	 *
	 * @return true if message was handled, false to keep searching
	 *         for a handler.
	 */
	virtual bool OnKeyDown(char key, uint32_t flags);

protected:
	/**
	 * Take a string with a key description and normalize
	 * the description.
	 *
	 * For example:
	 *
	 * @verbatim
	 * alt-SHIFT-Ctrl-f
	 * @endverbatim
	 *
	 * would return
	 *
	 * @verbatim
	 * shift-ctrl-alt-F
	 * @endverbatim
	 *
	 * The normalized string always sorts the operators
	 * in the order shift, ctrl, alt, and presents them in
     * lower case. The key name itself is capitalized.
     *
     * This function is used internally to normalize the key string
     * in AddHotkey() and TModalWindow::OnKeyDown()--there's no need
     * for you to call it yourself unless you're extending
     * TModalWindow.
     *
	 * @param key    Key to normalize.
	 *
	 * @return A normalized string.
	 */
	static str NormalizeKeyString( str key );

	/*@}*/
public:
	/**
	 * @name Message handlers
	 *
     * %TModalWindow handles these messages for you.
	 *
	 * @{
	 */

	/**
	 * This window is receiving the keyboard focus.
	 *
	 * @param previous The window that was previously focused.
	 *                 Can be NULL.
	 */
	virtual void OnSetFocus(TWindow *previous);

	/**
	 * Handle any initialization or setup that is required when
	 * this window is assigned to a new parent. No initialization
	 * of the window has happened prior to this call.
	 *
	 * @return True on success; false on failure.
	 *
	 * @see Init
	 * @see PostChildrenInit
	 */
	virtual bool OnNewParent();

	/**
	 * Handle a message.
	 *
	 * @param message   Payload of message.
	 *
	 * @return True if message handled; false otherwise.
	 */
	virtual bool OnMessage(TMessage *message);

	/**
	 * Translated character handler.
	 *
	 * @param key    Key hit on keyboard, along with shift translations.
	 * @return true if message was handled, false to keep searching
	 *         for a handler.
	 */
	virtual bool OnChar(char key);

    /**
     * Handles any modal processing that needs to happen.
     *
     * Processes tasks in the task list. This is called internally
     * by the system once per frame in TTask::eNormal state and once
     * in TTask::eOnDraw state.
     *
     * @param context The context the processes should be called in.
     */
	void DoModalProcess(TTask::ETaskContext context=TTask::eNormal) ;

	/*@}*/

#if !USER_DOCS
	/**
	 * @nouser
	 */
	void DispatchWindowMessage( TMessage * message );

	/**
	 * Manage opaque children.
	 *
	 * @param child  Child to manage.
	 * @param opaque True if this child is opaque and covers the entire surface.
	 */
	void SetOpaqueChild( TWindow * child, bool opaque );
#endif

	/**
	 * Get a reference to the clock associated with this modal
	 * window. This clock will be automatically paused and unpaused
	 * when other modal windows hide and reveal this modal window.
	 *
     * @return A pointer to the clock.
	 */
	TClock * GetClock() { return &mClock; }

    virtual void PostChildrenInit(TWindowStyle & style);
private:
	void PressButton( str button );
private:
	TTaskList*	            mpTaskList ;

	TWindow		            *mDefaultFocus ;

	std::set<TWindow*>		mOpaqueChildren ;

	TClock					mClock ;

	std::map<str,str>		mHotKeys;
};

#endif // MODALWINDOW_H_INCLUDED
