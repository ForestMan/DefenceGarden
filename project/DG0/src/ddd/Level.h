#pragma once

#include <pf/window.h>
#include "ddd/ILua.h"
#include "ddd/Container.h"

namespace ddd
{
	class Actor;

	class Level 
		: public ILua
		, public Container< Actor >
		, public TWindow
	{
	public:
		Level();
		virtual ~Level();

	protected:

		virtual void onInit();
		virtual void onRelease();

	private:
	};
}