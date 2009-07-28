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
 * Interface for class TCustomTextEdit
 */


#ifndef CUSTOMTEXTEDIT_H_INCLUDED
#define CUSTOMTEXTEDIT_H_INCLUDED

#include <pf/textedit.h>
#include <pf/assetmap.h>
#include <pf/random.h>

/**
 * A customized text edit window that knows how to:
 * 
 * - Play "click" noises when you hit a key.
 * - Enable a window when there is content, but disable when
 *   there is no content.
 * - Clear some text from the window when the user first hits a
 *   key.
 * 
 * New parameters:
 * - \c clearinitial True to clear the text the window is
 *   initialized with when it's first edited.
 * - \c enablewindow The name of a window to enable/disable when
 *   the text is present/empty.
 */
class TCustomTextEdit : public TTextEdit
{
	PFTYPEDEF_DC(TCustomTextEdit,TTextEdit);

public:
	/// Default Constructor
	TCustomTextEdit();

	/// Destructor
	virtual ~TCustomTextEdit();

	/**
	 * Mouse up handler.
	 *
	 * Used to detect clicks on embedded text links. Returns false if
	 * no text link was previously clicked on.
	 *
	 * @param point      Location of mouse release in client coordinates.
	 *
	 * @return true if message was handled, false to keep searching
	 *         for a handler.
	 */
	virtual bool OnMouseUp(const TPoint &point);

	/**
	 * Virtual function to notify child that a key was pressed.
	 *
	 * @param type   key type
	 * @param key	 key that was hit when appropriate
	 *
	 * @return True to accept the key. False to ignore.
	 */
	virtual bool KeyHit(eKeyType type, char key=0);

	/**
	 * Initialize the textedit window
	 *
	 * \warning Be sure to call this Init() from your derived
	 * class Init() function.
	 *
	 * @return true on success.
	 */
	virtual void Init(TWindowStyle & style);

	/**
	 * Test to see if we should enable or disable our connected 
	 * window.
	 * 
	 * @param count  Number of characters added or removed from the string.
	 */
	void TestEnable(int count);

private:
	/**
	 * Our key click asset holder.
	 */
	TAssetMap 	mAssetMap ;

	/**
	 * A random number generator for getting key-clicks.
	 */
	TRandom   	mRandom ;

	/**
	 * Whether the initial text has been cleared already.
	 */
	bool		mClearedInitialText ;

	/**
	 * The name of our associated enable window.
	 */
	str			mEnableWindow ;
};

#endif // CUSTOMTEXTEDIT_H_INCLUDED
