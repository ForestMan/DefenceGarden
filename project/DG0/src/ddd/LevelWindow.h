#pragma once

#include <pf/window.h>

#include <ddd/ILua.h> 
#include "ddd/Actor.h"

#include "ddd/LogicLevelComponent.h"
#include "ddd/RenderLevelComponent.h"

namespace ddd
{
	class Actor;

	class LevelWindow
		: public TWindow
		, public ddd::ILua
	{
		PFTYPEDEF_DC( LevelWindow, TWindow )

		//Enumerate level window standart components
		enum ELevelWindowComponents
		{
			LWC_LOGIC = 0,
			LWC_RENDER,

			LWC_FIRST = LWC_LOGIC,
			LWC_LAST = LWC_RENDER
		};
	public:
	
		//Window interface
		LevelWindow();
		~LevelWindow();

		virtual void Init(TWindowStyle &style);
		void initWindow(TWindowStyle &style, 
					const unsigned long levelID,
					const unsigned long gameID );

		void Draw();
		virtual bool OnTaskAnimate();

		//logic level interface
		inline void addActor( Actor& actor );
		inline Actor& getActor( const unsigned long actorID );
		inline const Actor& getActorConst( const unsigned long actorID );
		inline void removeActor(  const unsigned long actorID  );

		inline const unsigned long getGameID()const;

		//lua object realization
		virtual void onInit();
		virtual void onRelease();
	private:
		
		//Level window interface
		inline LogicLevelComponent& getLogicComponent();
		inline RenderLevelComponent& getRenderComponent();

	private:

		LogicLevelComponent logicComponent_;
		RenderLevelComponent renderComponent_;
	};

	//-------------------------------------------------------------------------

	inline LogicLevelComponent& LevelWindow::getLogicComponent()
	{
		return logicComponent_;
	}

	//-------------------------------------------------------------------------

	inline RenderLevelComponent& LevelWindow::getRenderComponent()
	{
		return renderComponent_;
	}

	//-------------------------------------------------------------------------

	inline void LevelWindow::addActor( Actor& actor )
	{
		getLogicComponent().addActor(actor);
	}

	//-------------------------------------------------------------------------

	inline Actor& LevelWindow::getActor( const unsigned long actorID )
	{
		return getLogicComponent().getActor(actorID);
	}

	//-------------------------------------------------------------------------

	inline const Actor& LevelWindow::getActorConst( const unsigned long actorID )
	{
		return getLogicComponent().getActorConst(actorID);
	}

	//-------------------------------------------------------------------------

	inline void LevelWindow::removeActor(  const unsigned long actorID  )
	{
		getLogicComponent().removeActor(actorID);
	}

	//-------------------------------------------------------------------------

	inline const unsigned long LevelWindow::getGameID()const
	{
		return getULong( getLuaTable(), "gameID_" );
	}

	//-------------------------------------------------------------------------
}