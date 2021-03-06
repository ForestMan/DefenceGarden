#pragma once 

#include <map>
#include <pf/luatable.h> 
#include "ddd/Types.h"
#include "boost/serialization/singleton.hpp"
#include "ddd/ILua.h"
#include "ddd/Container.h"

class TPlatform;

namespace ddd
{

	class Factory;
	class Game;
	class LevelWindow;

	class Application 
		: public boost::serialization::singleton<Application>
		, public ILua
		, public Container< Game >
	{
	public:
		~Application();

		void initPlayground( TPlatform* pPlatform );
		void run( TPlatform* pPlatform );

		inline void setFactory( Factory* factory );
		inline Factory* getFactory()const;
		inline const bool hasFactory()const;

		void addGame( TLuaTable* gameTable );
		void addLevel( TLuaTable* gameTable );
		void createLevelTable( ddd::LevelWindow* window, 
				const unsigned long levelID,
				const unsigned long gameID);
		void addActor( TLuaTable* actorTable, 
				const unsigned long gameID, 
				const unsigned long levelID );

	protected:

		void initGameStates();
		void initWindows( TPlatform* pPlatform );

		virtual void onInit();
		virtual void onRelease();

	private:
		
		void initApplication( TLuaTable* luaTable );
		
		Factory* factory_;
	};

	//-------------------------------------------------------------------------
	
	inline void Application::setFactory( Factory* factory )
	{
		factory_ = factory;
	}
	
	//-------------------------------------------------------------------------
	
	inline Factory* Application::getFactory()const
	{
		return factory_;
	}
	
	//-------------------------------------------------------------------------

	inline const bool Application::hasFactory()const
	{
		return 0 != factory_;
	}

	//-------------------------------------------------------------------------
}