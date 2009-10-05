#pragma once

#include "ddd/BaseWindow.h"

#include "ddd/LogicLevelComponent.h"
#include "ddd/RenderLevelComponent.h"

namespace ddd
{
	class Actor;

	class LevelWindow
		: public BaseWindow
	{
		PFTYPEDEF_DC( LevelWindow, BaseWindow )

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
		void init(TWindowStyle &style, 
					const unsigned long levelID,
					const unsigned long gameID );

		void Draw();
		virtual bool OnTaskAnimate();

		//logic level interface
		inline void addActor( Actor& actor );
		inline Actor& getActor( const unsigned long actorID )const;
		inline const Actor& getActorConst( const unsigned long actorID )const;
		inline void removeActor(  const unsigned long actorID  );

	private:
		
		//Level window interface
		inline LogicLevelComponent& getLogicComponent()const;
		inline RenderLevelComponent& getRenderComponent()const;

	private:


		LogicLevelComponent* logicComponent_;
		RenderLevelComponent* renderComponent_;
	};

	//-------------------------------------------------------------------------

	inline LogicLevelComponent& LevelWindow::getLogicComponent()const
	{
		assert(0 != logicComponent_);
		return *logicComponent_;
	}

	//-------------------------------------------------------------------------

	inline RenderLevelComponent& LevelWindow::getRenderComponent()const
	{
		assert(0 != renderComponent_);
		return *renderComponent_;
	}

	//-------------------------------------------------------------------------

	inline void LevelWindow::addActor( Actor& actor )
	{
		getLogicComponent().addActor(actor);
	}

	//-------------------------------------------------------------------------

	inline Actor& LevelWindow::getActor( const unsigned long actorID )const
	{
		return getLogicComponent().getActor(actorID);
	}

	//-------------------------------------------------------------------------

	inline const Actor& LevelWindow::getActorConst( const unsigned long actorID )const
	{
		return getLogicComponent().getActorConst(actorID);
	}

	//-------------------------------------------------------------------------

	inline void LevelWindow::removeActor(  const unsigned long actorID  )
	{
		getLogicComponent().removeActor(actorID);
	}

	//-------------------------------------------------------------------------
}