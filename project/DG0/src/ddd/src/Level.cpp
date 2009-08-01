#include "ddd/Level.h"

namespace ddd
{
	Level::Level()
		: ILua()
	{
	}

	Level::~Level()
	{
	}

	void Level::onInit()
	{
		removeAllEntity();
		initLuaFunction( 0, "onInit" );
		executeLuaFunction( 0 );
	}

	void Level::onRelease()
	{
		releaseLuaFunction( 0 );
		removeAllEntity();
	}
}