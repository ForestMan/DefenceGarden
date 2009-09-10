#pragma once

#include "ddd/ILua.h"
#include "ddd/Container.h"
#include "ddd/LevelWindow.h"

namespace ddd
{
	//class Level;

	class Game 
		: public ILua
		, public Container< LevelWindow >
	{
	public:
		Game();
		virtual ~Game();

	protected:

		virtual void onInit();
		virtual void onRelease();

	private:
	};
}