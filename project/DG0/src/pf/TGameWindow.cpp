//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------


#include "TGameWindow.h"
#include "globaldefines.h"
#include "settings.h"

#include <pf/pflib.h>

#include "pf/debug.h"

PFTYPEIMPL_DC(TGameWindow);

TGameWindow::TGameWindow()
{
	TGameState::GetInstance()->SetState("++PlayedSimpleGame");

	mBigTex = TTexture::Get("backgrounds/bigtex");
	mOffset =0;

	TPlatform::SetConfig( "vsync", "1" );
}

TGameWindow::~TGameWindow()
{
	TGameState::GetInstance()->SetState("begin-status","SimpleGameComplete");
	TGameState::GetInstance()->SetState("add-state","LastGameHiScore");
	TGameState::GetInstance()->SetState("end-status","SimpleGameComplete");

	ScriptUnregisterFunction(TWindowManager::GetInstance()->GetScript(),"SendGameMessage");
}

void TGameWindow::Draw()
{
	TBegin2d begin2d;
	TRenderer * r = TRenderer::GetInstance();

	r->FillRect( TURect(0,0,800,600), TColor(0,0,0,0) );

	TTransformedLitVert verts[4];
	TVec2 corners[4]=
	{
		TVec2(0,0),
		TVec2(800,0),
		TVec2(800,600),
		TVec2(0,600)
	};
	while (mOffset>1)
	{
		mOffset--;
	}
	TVec2 uv[2]=
	{
		TVec2(0+mOffset,0+mOffset),
		TVec2(1+mOffset,1+mOffset),
	};

	CreateVertsFromRect( TVec2(0,0),verts,corners,uv,TMat3(),1,TColor(1,1,1,1) ) ;

	r->SetTexture(mBigTex);
	r->SetTextureMapMode( TRenderer::kMapWrap, TRenderer::kMapWrap );
	r->DrawVertices(TRenderer::kDrawTriFan, TVertexSet(verts,4) );

}

/**
 * Called if you have initiated a window animation with
 * TWindow::StartWindowAnimation.
 *
 * @return True to continue animating. False to stop.
 */
bool TGameWindow::OnTaskAnimate()
{
    // And don't forget to update the screen
	TWindowManager::GetInstance()->InvalidateScreen();

	mOffset += 0.01F ;

	if (TGameState::GetInstance()->QueryJumpToMenu())
	{
		SendGameMessage("quit");
	}
	if (TGameState::GetInstance()->QueryRestartMode())
	{
		// Restart our game mode!
	}

    // Keep animating
	return true;
}

/**
 * Initialize the Window. Called by the system only in
 * Lua initialization.
 *
 * When you create your own custom window, this is where you put
 * your own custom initialization that needs to happen before
 * children are created. Fundamental window initialization is
 * handled in every class by this function, so <b>when you
 * override this function you almost always want to call your
 * base class to handle base class initialization.</b>
 */
void TGameWindow::Init(TWindowStyle &style)
{
	TScript * s = TWindowManager::GetInstance()->GetScript();
	ScriptRegisterMemberDirect(s,"SendGameMessage",this,TGameWindow::SendGameMessage);

    // Start the window animation
    StartWindowAnimation( 16 );

	mRows = style.GetInt("rows");
	mColumns= style.GetInt("columns");

	// Send keyboard events our way if no one else uses them
	FindParentModal()->SetDefaultFocus(this);
}

/**
 * Mouse down handler.
 *
 * @param point      Location of mouse press in client coordinates.
 *
 * @return true if message was handled, false to keep searching
 *         for a handler.
 */
bool TGameWindow::OnMouseDown(const TPoint & /*point*/)
{

    return true;
}


void TGameWindow::SendGameMessage( str message )
{
	if (message=="quit")
	{
		// Call the helper function in TSettings that pops all of the windows off the stack.
		// It assumes that you still have the forever loop in mainloop.lua that recreates the
		// main menu screen when it's been popped.
		TSettings::GetInstance()->QuitToMainMenu();

	}
}

/**
 * Raw key hit on keyboard.
 *
 * @param key    Key pressed on keyboard.
 * @param flags  TEvent::EKeyFlags mask representing the state of other keys on the keyboard
 *               when this key was hit.
 *
 * @return true if message was handled, false to keep searching
 *         for a handler.
 */
bool TGameWindow::OnChar(char key)
{
	if (key == TEvent::kEscape)
	{
		TWindowManager::GetInstance()->DoLuaString(this,"DoModal('scripts/pause.lua')");
		return true;
	}

	return false;
}
