#pragma once

#include "ddd/ILevelComponent.h"

namespace ddd
{
	class LevelWindow;

	class RenderLevelComponent
		: public ILevelComponent
	{
	public:
		
		RenderLevelComponent();
		virtual ~RenderLevelComponent();

		void render( LevelWindow* owner );

	private:
		
		virtual void onCreate();
		virtual void onInit();
		virtual void onRelease();
	};
}