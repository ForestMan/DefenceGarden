#include "ddd/LevelWindow.h"

#include <pf/pflib.h>
#include "pf/debug.h"

#include "ddd/Application.h"
#include "ddd/Game.h"

namespace ddd
{
	//-------------------------------------------------------------------------

	PFTYPEIMPL_DC(LevelWindow);

	//-------------------------------------------------------------------------

	LevelWindow::LevelWindow()
	{
	}

	//-------------------------------------------------------------------------

	LevelWindow::~LevelWindow()
	{
		if ( isInited() )
		{
			ddd::Application::get_mutable_instance().getEntity( getGameID() ).removeEntity( getID() );
			release();
		}
	}

	//-------------------------------------------------------------------------

	void LevelWindow::Init(TWindowStyle &/*style*/)
	{
		// Start the window animation
		StartWindowAnimation( 16 );

		// Send keyboard events our way if no one else uses them
		FindParentModal()->SetDefaultFocus(this);
	}

	//-------------------------------------------------------------------------

	void LevelWindow::initWindow(TWindowStyle &style, 
			const unsigned long levelID,
			const unsigned long gameID )
	{
		LevelWindow::Init(style);
		getLogicComponent().create();
		getLogicComponent().init();
		getRenderComponent().create();
		getRenderComponent().init();
		ddd::Application::get_mutable_instance().createLevelTable(this, levelID, gameID);
	}

	//-------------------------------------------------------------------------

	void LevelWindow::Draw()
	{
		getRenderComponent().render(this);
	}

	//-------------------------------------------------------------------------

	bool LevelWindow::OnTaskAnimate()
	{
		getLogicComponent().update(this);
		return true;
	}

	//-------------------------------------------------------------------------

	void LevelWindow::onInit()
	{
		initLuaFunction( 0, "onInit" );
		ddd::Application::get_mutable_instance().getEntity( getGameID() ).addEntity( *this );

		executeLuaFunction( 0 );
	}

	//-------------------------------------------------------------------------
	
	void LevelWindow::onRelease()
	{
		releaseLuaFunction( 0 );
		getLogicComponent().release();
		getRenderComponent().release();
	}

	//-------------------------------------------------------------------------
}