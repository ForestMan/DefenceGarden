#pragma once

#include "ddd/BaseWindow.h"

#include "ddd/LogicLevelComponent.h"
#include "ddd/RenderLevelComponent.h"

namespace ddd
{
	class LogicLevelComponent;

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

		void Draw();
		virtual bool OnTaskAnimate();

	private:
		
		//Level window interface
		inline LogicLevelComponent& getLogicComponent();
		inline RenderLevelComponent& getRenderComponent();

	private:
		LogicLevelComponent* logicComponent_;
		RenderLevelComponent* renderComponent_;
	};

	//-------------------------------------------------------------------------

	inline LogicLevelComponent& LevelWindow::getLogicComponent()
	{
		assert(0 != logicComponent_);
		return *logicComponent_;
	}

	//-------------------------------------------------------------------------

	inline RenderLevelComponent& LevelWindow::getRenderComponent()
	{
		assert(0 != renderComponent_);
		return *renderComponent_;
	}

	//-------------------------------------------------------------------------
}