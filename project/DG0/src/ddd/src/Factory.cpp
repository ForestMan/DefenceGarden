#include "ddd/Factory.h"
#include "ddd/Application.h"
#include "ddd/Game.h"
#include "ddd/Level.h"
#include "ddd/Actor.h"

namespace ddd
{
	Factory::Factory()
		: script_( 0 )
		, application_( 0 )
	{
	}

	Factory::~Factory()
	{
	}

	void Factory::init( TScript* script, Application* app )
	{
		assert( script );
		assert( app );
		assert( !isInited() );
		script_ = script;
		application_ = app;
	}
	
	void Factory::release()
	{
		assert( isInited() );
		script_ = 0;
	}

	Game* Factory::createGame( const char* name )
	{
		Game* result( 0 );
		if ( !strcmp( name, "game" ) )
		{
			result = new Game();
		}
		return result;
	}
	
	Level* Factory::createLevel( const char* name )
	{
		Level* result( 0 );
		if ( !strcmp( name, "level" ) )
		{
			result = new Level();
		}
		return result;
	}
	
	Actor* Factory::createActor( const char* name )
	{
		Actor* result( 0 );
		if ( !strcmp( name, "actor" ) )
		{
			result = new Actor();
		}
		return result;
	}

}