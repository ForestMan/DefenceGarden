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
		ScriptRegisterMemberDirect( script_,"addGame", this, Factory::addGame );
		ScriptRegisterMemberDirect( script_,"addLevel", this, Factory::addLevel );
		ScriptRegisterMemberDirect( script_,"addActor", this, Factory::addActor );
	}
	
	void Factory::release()
	{
		assert( isInited() );
		ScriptUnregisterFunction( script_, "addGame" );
		ScriptUnregisterFunction( script_, "addLevel" );
		ScriptUnregisterFunction( script_, "addActor" );
		script_ = 0;
	}

	void Factory::addGame( TLuaTable* gameTable )
	{
		assert( 0 != gameTable );
		Game* game = createGame( gameTable->GetString( "type_" ).c_str() );
		assert( 0 != game );
		game->init( gameTable );
		getApplication().addEntity( *game );
	}
	
	void Factory::addLevel( TLuaTable* levelTable, 
					const unsigned long gameID )
	{
		assert( 0 != levelTable );
		Level* level = createLevel( levelTable->GetString( "type_" ).c_str() );
		assert( 0 != level );
		level->init( levelTable );
		getApplication().getEntity( gameID ).addEntity( *level );
	}
	
	void Factory::addActor( TLuaTable* actorTable, 
					const unsigned long gameID, 
					const unsigned long levelID )
	{
		assert( 0 != actorTable );
		Actor* actor = createActor( actorTable->GetString( "type_" ).c_str() );
		assert( 0 != actor );
		actor->init( actorTable );
		getApplication().getEntity( gameID ).getEntity( levelID ).addEntity( *actor );
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
		return 0;
	}
	
	Actor* Factory::createActor( const char* name )
	{
		return 0;
	}

}