#include "ddd/Game.h"

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
		executeLuaFunction( 0 );
	}

	void Game::onRelease()
	{
		releaseLuaFunction( 0 );
		removeAllEntity();
	}
}