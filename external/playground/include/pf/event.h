//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

#ifndef POINT_H_INCLUDED
#include "point.h"
#endif
/**
 * @condfile
 * Interface for TEvent, and event definitions.
 */

/**
 * System event encapsulation.
 */
class PFLIB_API TEvent {
public:
	/// Event codes.
	enum EEventCode {
		kIdle = 1,			///< Idle event processing time
		kNull,				///< EMPTY event
		kClose,				///< A close request (Alt-F4, clicking close button)
		kQuit,				///< A QUIT NOW event.
		kMouseDown,			///< Mouse down
		kExtendedMouseEvent,///< Right Mouse Button/Wheel down
		kMouseUp,			///< Mouse up
		kMouseMove,			///< Mouse move
		kMouseLeave,		///< Mouse has left the window. You must TWindowManager::AddMouseListener() the mouse to receive this message; note that TButton-derived classes automatically capture the mouse on mouse-over.
		kMouseHover,		///< Mouse has hovered over a point on the window.
		kKeyDown,			///< Key down
		kKeyUp,				///< Key up
		kChar,				///< translated character event
		kUTF32Char,			///< translated utf32 event
		kRedraw,			///< Redraw the screen now
		kTimer,				///< A timer has triggered
		kDisplayModeChange,	///< The display mode has changed
		kActivate,			///< Our application has activated/deactivated. mKey is set to 0 on deactivate, or 1 on activate.
		kFullScreenToggle	///< The user has toggled full screen mode. mKey is set to 0 on windowed mode, or non-zero on full screen mode.
	};

	/// Event key flags.
	enum EKeyFlags {
		kShift=1,		///< Shift is pressed.
		kControl=2,		///< Control (or command) is pressed.
		kAlt=4,			///< Alt key is pressed.
		kExtended=8,	///< Reserved.
	};

	/** @name Key Codes
     *
     *  \anchor keycodes
     *
     *  Use these constants to refer to the given keys in your code
     *  when processing an OnKeyDown() message.
     *
     *  @see TWindow::OnKeyDown
     *
	 * @{
	 */

	/// Up arrow key.
	static int32_t	kUp;
	/// Down arrow key.
	static int32_t	kDown;
	/// Left arrow key.
	static int32_t	kLeft;
	/// Right arrow key.
	static int32_t	kRight;
	/// Enter key. On keyboards with more than one key of this description,
	/// the same constant is returned for both.
	static int32_t	kEnter;
	/// The "Esc" key.
	static int32_t  kEscape;
	/// The Tab key.
	static int32_t  kTab;
	/// The "Paste" keyboard combination. Typically Control-V on Windows, Command-V on
	/// OS X.
	static int32_t  kPaste;
	/// The Page-Up key.
	static int32_t  kPageUp;
	/// The Page-Down key.
	static int32_t  kPageDown;
	/// The backspace key.
	static int32_t  kBackspace;
	/// The Delete key.
	static int32_t  kDelete;

	/*@}*/

	/**
	 * Type of event
	 *
	 * @see EEventCode
	 */
	int32_t			mType;

	/**
	 * Key for keyboard events.
	 */
	int32_t			mKey;

	/**
	 * Point for mouse events.
	 *
	 */
	TPoint		mPoint ;

	/**
	 * Flags for key events.
	 *
	 */
	EKeyFlags   mKeyFlags;
};

//------------------------------------------------------------------
#endif // EVENT_H_INCLUDED
