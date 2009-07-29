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
 * Interface for TButton
 *
 * \ingroup windowgroup
 */

#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#ifndef TEXT_H_INCLUDED
#include "text.h"
#endif

#ifndef LAYEREDWINDOW_H_INCLUDED
#include "layeredwindow.h"
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#ifndef TAGGING_IGNORE
class TLuaFunction;
#endif

/**
 * Encapsulation for button functionality. Handles
 * push-buttons, radio buttons, and toggles. Also
 * handles mouse-over highlighting.
 *
 * A TButton is implemented as a TLayeredWindow with
 * 3 or 4 layers, depending on the type of button (a
 * kPush button only needs three layers, while the other
 * types need four).
 *
 * If you define a button in Lua, you can use the special
 * options defined in the documentation of the Lua call \ref
 * Button().
 *
 * \ingroup windowgroup
 */
class PFLIB_API TButton : public TLayeredWindow
{
	PFTYPEDEF_DC(TButton,TLayeredWindow);
public:

	/// An abstract action class for button actions.
	class Action
	{
	public:
		virtual ~Action()
		{
		}
        /**
         * Override this member to perform the action.
         *
         * @param button A pointer to the button triggering the action.
         *
         * @return
         */
		virtual void DoAction( TButton * button )=0;
	};

	/// A class that wraps a Lua command in an action.
	class LuaAction : public Action
	{
		public:
		/**
		 * Constructor
		 *
		 * @param action A TLuaFunction to wrap.
		 */
		LuaAction( TLuaFunction * action ) :
			mAction(action)
		{
		}
		/**
		 * Destructor.
		 */
		~LuaAction();

		virtual void DoAction(TButton *button);
		/// The wrapped action.
		TLuaFunction * mAction ;
	};

    /**
     * The type of a TButton. These constants are also available
     * in the Lua GUI thread.
     */
 	enum EButtonType {
		kPush = 0,		///< A normal "push" button.
		kToggle,		///< A 2-state toggle button.
		/// One of several "radio" buttons in a group. Must call BeginGroup()
		/// before the first button in the Lua script, or call TButton::AddFlags(kGroupStart)
		/// on the first button in the group.
		kRadio,
        /// Number of button types (not a real type).
		kNumTypes
	};

	/**
	 * The button layers.
	 */
	enum EButtonLayer {
		kUp = 0,	///< Button is in "up" or "off" state.
		kDown,		///< Button is in "down" or "on" state.
		kOver,		///< Button is in roll-over state (and is off for toggles and radio buttons).
		kOverOn		///< Button is in roll-over state and is "on".
	};

    /**
     * TButton internal dynamic state.
     */
	enum EMouseState
	{
		kMouseIdle = 0,	///< Button mouse state is idle.
		kMousePush,	///< Button is being pushed by a mouse click.
		kMouseOver,	///< Button is being rolled over by the mouse.
		kMouseActivated ///< A transient state indicating that the button has been activated. Reverts immediately to kMouseIdle
	};

    /**
     * Button creation flags.
     */
	enum EButtonCreateFlags
	{
		kGroupStart=1, 			///< This button is the first in a group
		kSendMessageToParent=2, ///< Send the button's message to the parent modal
		kCloseWindow=4,			///< Automatically send the parent TModalWindow a close message when this button is pushed.
		kDefaultButton=8,		///< This button is default button in the local context.
		kCancelButton=16		///< This button is a cancel button in the local context.
	};
public:
	/**
	 * Constructor.
	 */
	TButton();

    /**
     * Destructor.
     */
    ~TButton();

    /**
     * Set the button's type.
     *
     * @param type   New type for button.
     */
	void		SetType(EButtonType type);

    /**
     * Toggle the button's state. Button must be of type kToggle, or
     * this method will ASSERT.
     */
	void Toggle();

    /**
     * Get the button's type.
     *
     * @return Type of the button.
     */
	EButtonType	GetType() {return mType;};

	/**
	 * Get the current mouse state of the button.
	 *
	 * @return The state of the button with regards to the mouse.
	 */
	EMouseState	GetState()	{ return mMouseState; }

	/**
	 * Set a toggle or radio button on.
	 *
	 * @param on     True to set to on. False to set to off (toggle only).
	 */
	void		SetOn(bool on);
	/**
	 * Return whether a particular radio button is on.
	 *
	 * @return True for on.
	 */
	bool		GetOn() { return mOn; }

	/**
	 * Return true if this is a default button.
	 *
	 * @return True if default.
	 */
	bool		IsDefault() { return mDefault; }

	/**
	 * Return if this is a cancel button.
	 *
	 * @return True if this button should abort the window.
	 */
	bool		IsCancel() { return mCancel; }

	/**
	 * Add flags to the current button.
	 *
	 * @param flags  Flags to add. Does not erase flags.
	 */
	void 		AddFlags( uint32_t flags );

	/**
	 * Set the tooltip for this button.
	 *
	 * @warning Not implemented yet!
	 * @warning Specification subject to change.
	 *
	 * @param tooltip A string for the tooltip.
	 */
	void		SetTooltip(str tooltip) { mTooltipStr = tooltip; };

	/**
	 * Set the label for this button
	 *
	 *
	 * @param label Text to place in the label for this button
	 */
	void SetLabel(str label);

	/**
	 * Get the label for this button.
	 *
	 * @return The button's label.
	 */
	str GetLabel();

	/**
	 * Notification that the mouse has left the window.
	 */
	virtual bool OnMouseLeave();
	/**
     * Mouse up handler.
     * @param point Mouse position.
	 */
	virtual bool OnMouseUp(const TPoint &point);
	/**
	 * Mouse down handler.
	 * @param point Mouse position.
	 */
	virtual bool OnMouseDown(const TPoint &point);
	/**
	 * Mouse motion handler.
	 * @param point Mouse position.
	 */
	virtual bool OnMouseMove(const TPoint &point);

	/**
	 * Press the button from C++: Equivalent to clicking on the
	 * button.
	 *
	 * @return true if the window is still around by the end; false if
	 *         it's been closed and/or removed from the hierarchy.
	 */
	virtual bool Press();

	/**
	 * Set the current TButton::Action associated with the button's action.
     * Create a TButton::LuaAction to wrap a Lua command. The
     * TButton expects to own the command, and will delete it on
     * TButton destruction.
	 *
     * @param action Action to bind to button.
     * @todo Change the name from SetCommand to AdoptCommand()
     *
	 */
	void SetCommand( TButton::Action * action );

	/**
	 * Do the command associated with this button
	 *
	 * @return True if the window was deleted as a result of the command.
	 */
	bool DoCommand();

	/**
	 * Set a sound for a specific button state
	 *
	 * @param state Which mouse state triggers the sound
     * @param sound name of sound file to play
	 */
	void SetSound(EMouseState state, str sound);

	/**
	 * Set a click mask for the button. A click mask is an image where
	 * anything black (all pixels have RGB of less than 20)
	 * in the image is considered outside the button, and the mouse will
	 * not activate the button if it is in a black area.
	 *
	 * @param texture texture to use as the click mask
	 */
	void SetClickMask(TTextureRef texture);

	/**
	 * @name Derived Button virtual functions.
	 *
	 * Functions that a derived TButton class can override to respond to
	 * button state changes with custom behaviors and/or animations.
	 *
	 * @{
	 */

	/**
	 * Called when the button is activated so that a derived class
	 * may trigger an animation or change the button's state.
	 *
	 * Call the base class if you want to add behavior to the default behavior;
	 * otherwise you will need to take responsibility for
	 * changing the button's appearance.
	 *
	 * @param newState State we're transitioning to.
	 * @param previousState
	 *                 State we're transitioning from.
	 */
	virtual void StateUpdate(EMouseState newState, EMouseState previousState);

	/** @} */

	/**
	 * Do Lua initialization.
	 *
		 Supported tags include:
		 - \c close   (bool) This button closes its window/dialog if true.
		 - \c flags   (number) Button label text alignment.
		 - \c graphics (table) An array of up to four images for the button:
		   Three for push-buttons (Up, RollOver, Down), Four for toggle and
		   radio buttons (Up, RollOver-Up, Down, RollOver-Down). If the array has
		   fewer than 3 or 4 images, additional images are duplicated from the last
		   given image.
		 - \c label   (string) Default button text label.
		 - \c scale   (number) Scale to apply to the button graphics.
		 - \c sound   (string) Name of sound to play when button pressed.
		 - \c rolloversound (string) Name of sound to play when mouse rolls over the button.
		 - \c type    (number) Button type (kPush, kToggle, kRadio).
		 - Other generic window tags.
     * @param style Style definition for button.
	 */
	virtual void Init(TWindowStyle & style);

	virtual bool OnNewParent();
	virtual void PostChildrenInit(TWindowStyle &style);

private:
	virtual void SetState(EMouseState state);

	/**
	 * Internal handler for testing a point and releasing
	 * capture if the point is outside of our rectangle.
	 *
	 * @param point  Point to test.
	 *
	 * @return True if the point is outside and we've released the
	 *         capture. False otherwise.
	 */
	bool ReleaseCapture( const TPoint & point );

private:
	TButton( const TButton & button );
	const TButton & operator=( const TButton & button );

	EButtonType		mType:4 ;
	EMouseState		mMouseState:3 ;

	bool			mSendToParent:1 ;
	bool			mOn:1 ;
	bool			mClose:1 ;
	bool			mDefault:1 ;
	bool			mCancel:1 ;

	TButton::Action *mAction ;

	TSoundRef		mSounds[kMouseOver+1];

	/// Set our tooltip. Will get tooltip "style" from TButtonManager.
	str			mTooltipStr ;

	TColor32 		*mMaskData;
	int32_t			mMaskWidth;
	int32_t			mMaskHeight;

};

//------------------------------------------------------------------
#endif // BUTTON_H_INCLUDED

