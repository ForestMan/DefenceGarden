//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef CREDITS_H_INCLUDED
#define CREDITS_H_INCLUDED



#include <pf/pflib.h>
#include "time.h"

class TCredits : public TWindow
{
	PFTYPEDEF_DC(TCredits, TWindow);
public:
	/**
	 * Called if you have initiated a window animation with
	 * TWindow::StartWindowAnimation.
	 *
	 * @return True to continue animating. False to stop.
	 */
	virtual bool OnTaskAnimate();
	TCredits();
	~TCredits();

	virtual void	Draw();
	virtual void	Init(TWindowStyle & style);

private:
	TTextGraphic*	mText1;
	TTextGraphic*	mText2;
	TTextGraphic*	mText3;
	unsigned long	mStart;
	unsigned long	mHeight;
	str				mFont;
	uint32_t		mFontSize;
	uint32_t		mColumnGap;
	uint32_t		mCreditsTime;
	uint32_t		mCreditsIntroPause;
	uint32_t		mColumnWidth;
	TColor			mFontColor;
	str				mHeaderColorHex;
	str				mCreditsFile;
};

#endif // CREDITS_H_INCLUDED
