#include "ddd/Game.h"
#include "ddd/Application.h"

namespace ddd
{
	Game::Game()
		: ILua()
	{
	}

	Game::~Game()
	{
	}

	void Game::onInit()
	{
		removeAllEntity();
		initLuaFunction( 0, "onInit" );

		ddd::Application::get_mutable_instance().addEntity(*this);
		executeLuaFunction( 0 );
	}

	void Game::onRelease()
	{
		releaseLuaFunction( 0 );
		LevelWindow* level = begin();
		while(level)
		{
			if ( level->isInited() )
			{
				level->release();
			}
			level = getNextEntity();
		}
		removeAllEntity();
	}
}