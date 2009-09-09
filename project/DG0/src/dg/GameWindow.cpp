#include "dg/GameWindow.h"
#include "globaldefines.h"
#include "settings.h"

#include <pf/pflib.h>

#include "pf/debug.h"

PFTYPEIMPL_DC(GameWindow);

GameWindow::GameWindow()
{
	TGameState::GetInstance()->SetState("++PlayedSimpleGame");

	TPlatform::SetConfig( "vsync", "1" );
}

GameWindow::~GameWindow()
{
	TGameState::GetInstance()->SetState("begin-status","SimpleGameComplete");
	TGameState::GetInstance()->SetState("add-state","LastGameHiScore");
	TGameState::GetInstance()->SetState("end-status","SimpleGameComplete");

	ScriptUnregisterFunction(TWindowManager::GetInstance()->GetScript(),"SendGameMessage");
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
void GameWindow::Init(TWindowStyle &style)
{
	TScript * s = TWindowManager::GetInstance()->GetScript();
	ScriptRegisterMemberDirect(s,"SendGameMessage",this,GameWindow::SendGameMessage);

	ddd::LevelWindow::Init( style );
}

/**
 * Mouse down handler.
 *
 * @param point      Location of mouse press in client coordinates.
 *
 * @return true if message was handled, false to keep searching
 *         for a handler.
 */
bool GameWindow::OnMouseDown(const TPoint & /*point*/)
{
    return true;
}


void GameWindow::SendGameMessage( str message )
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
bool GameWindow::OnChar(char key)
{
	if (key == TEvent::kEscape)
	{
		TWindowManager::GetInstance()->DoLuaString(this,"DoModal('scripts/pause.lua')");
		return true;
	}

	return false;
}
