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
 * Implementation for class TCustomTextEdit, a customized text-edit control.
 */

#include "customtextedit.h"
#include "globaldefines.h"

#include <pf/script.h>
#include <pf/windowstyle.h>
#include <pf/windowmanager.h>
#include <pf/modalwindow.h>

PFTYPEIMPL_DC(TCustomTextEdit);

const int kNumSounds = 4;

TCustomTextEdit::TCustomTextEdit()
{
	const char * assets[] =
	{
		SFX("audio/sfx/sfx_typing_01.ogg"),
		SFX("audio/sfx/sfx_typing_02.ogg"),
		SFX("audio/sfx/sfx_typing_03.ogg"),
		SFX("audio/sfx/sfx_typing_04.ogg"),
		NULL
	};
	mAssetMap.AddAssets(assets);
	mClearedInitialText = true ; // No initial text
}

TCustomTextEdit::~TCustomTextEdit()
{

}

/**
 * Virtual function to notify child that a key was pressed.
 *
 * @param type   key type
 * @param key	 key that was hit when appropriate
 *
 * @return True to accept the key. False to ignore.
 */
bool TCustomTextEdit::KeyHit(eKeyType type, char key)
{
	if (!mClearedInitialText)
	{
		mClearedInitialText = true ;
		SetText("");
	}
	if (type==kKeyChar)
	{
		if (TTextEdit::KeyHit(type,key))
		{
			TSoundRef sound =
				mAssetMap.GetSound(str::getFormatted(SFX("audio/sfx/sfx_typing_%02d.ogg"),mRandom.RandRange(1,kNumSounds)));
			if (sound)
				sound->Play(false);
			TestEnable(1);
			return true ;
		}
		return false ;
	}
	if (! TTextEdit::KeyHit(type,key) )
		return false;

	if (type==kKeyMove && key==0x08)
	{
		TestEnable(-1);
	}
	return true ;
}

/**
 * Initialize the custom text edit window
 *
 * \warning Be sure to call this Init() from your derived
 * class Init() function.
 *
 * @return true on success.
 */
void TCustomTextEdit::Init(TWindowStyle & style)
{
	mClearedInitialText = !style.GetBool("clearinitial",false);
	mEnableWindow = style.GetString("enablewindow","");

	TTextEdit::Init(style);
}

void TCustomTextEdit::TestEnable(int count)
{
	if ( (mEnableWindow.length()) > 0 )
	{
		TWindow * window = FindParentModal()->GetChildWindow(mEnableWindow,-1);
		if (window)
		{
			if (((int)GetText().length()+count)>0)
			{
				window->SetFlags(window->GetFlags()|TWindow::kEnabled);
			}
			else
			{
				window->SetFlags(window->GetFlags()&~TWindow::kEnabled);
			}
			TWindowManager::GetInstance()->InvalidateScreen();
		}
	}
}

/**
 * Mouse up handler.
 *
 * This handles clicking on text and clearing the initial text.
 *
 * @param point      Location of mouse release in client coordinates.
 *
 * @return true if message was handled, false to keep searching
 *         for a handler.
 */
bool TCustomTextEdit::OnMouseUp(const TPoint &point)
{
	if (!mClearedInitialText)
	{
		mClearedInitialText = true ;
		SetText("");
		return true ;
	}

	return TTextEdit::OnMouseUp(point);
}
