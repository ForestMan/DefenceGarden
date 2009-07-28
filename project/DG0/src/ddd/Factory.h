#pragma once

#include <pf/luatable.h> 
#include <pf/script.h>

namespace ddd
{
	
	class Application;
	class Game;
	class Level;
	class Actor;

	class Factory
	{
	public:
		Factory();
		virtual ~Factory();

		void init( TScript* script, Application* app );
		void release();

		inline const bool isInited()const;

		void addGame( TLuaTable* gameTable );
		void addLevel( TLuaTable* levelTable, 
						const unsigned long gameID );
		void addActor( TLuaTable* actorTable, 
						const unsigned long gameID, 
						const unsigned long levelID );
	protected:

		virtual Game* createGame( const char* name );
		virtual Level* createLevel( const char* name );
		virtual Actor* createActor( const char* name );

		inline Application& getApplication();
	private:

		TScript* script_;
		Application* application_;
	};

	inline const bool Factory::isInited()const
	{
		return 0 != script_ && 0 != application_;
	}

	inline Application& Factory::getApplication()
	{
		assert( isInited() );
		return *application_;
	}
}