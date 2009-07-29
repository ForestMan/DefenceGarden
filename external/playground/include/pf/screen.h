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
 * Interface for TScreen class.
 * \ingroup windowgroup
 */

#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include <set>
#include <map>

#ifndef RECT_H_INCLUDED
#include "rect.h"
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#ifndef MODALWINDOW_H_INCLUDED
#include "modalwindow.h"
#endif

#ifndef PLATFORM_H_INCLUDED
#include "platform.h"
#endif

#ifndef TAGGING_IGNORE
class	TScriptManager ;
#endif

/**
 * The base level modal window. Top-level application
 * control logic can be handled at this level by
 * deriving from TScreen and implementing a handler
 * for DoModalProcess.
 *
 * TScreen also handles caching of the window background,
 * so TScreen::Draw should NOT be overridden.
 * \ingroup windowgroup
 */
class PFLIB_API TScreen : public TModalWindow
{
	PFTYPEDEF(TScreen,TModalWindow);
public:
	TScreen();
	virtual ~TScreen();

	/**
	 * Tell the screen to clear its background.
	 *
	 * This should only be done in a situation where you have a
	 * sparse set of windows covering the background.
	 *
	 * @param clear  True to clear the background.
	 */
	void ClearBackground( bool clear ) { mClearBackground = clear ; }

	/**
	 * Tell the screen to never clear its backrgound
	 *
	 * This causes the screen to never redraw the background,
	 * even if is the only window.
	 *
	 * @param neverClear  True to never clear the background.
	 */
	void NeverClearBackground( bool neverClear ) {mNeverClearBackground = neverClear; }

	/**
	 * Set the background color.
	 *
	 * Note this will only have an effect if ClearBackground() is set
	 * to true.
	 *
	 * @param color  Color to clear the background.
	 */
	void SetBackgroundColor( const TColor & color )
	{
		mClearColor = color ;
	}

	/**
	 * Get the current background color.
	 *
	 * @return The current background color.
	 */
	TColor GetBackgroundColor()
	{
		return mClearColor;
	}
private:
	TScreen( const TScreen & button );
	const TScreen & operator=( const TScreen & button );

	virtual void Draw();


private:
	bool				mClearBackground ;
	bool				mNeverClearBackground ;
	TColor				mClearColor ;
};

//------------------------------------------------------------------
#endif // SCREEN_H_INCLUDED
