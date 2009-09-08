#pragma once

#include "ddd/ILevelComponent.h"

namespace ddd
{
	class LevelWindow;

	class LogicLevelComponent
		: public ILevelComponent
	{
	public:
		
		LogicLevelComponent();
		virtual ~LogicLevelComponent();

		void update( LevelWindow* owner );

	private:
		
		virtual void onCreate();
		virtual void onInit();
		virtual void onRelease();
	};
}