#pragma once

#include <pf/window.h>
#include "ddd/Container.h"
#include "ddd/ILevelComponent.h"

namespace ddd
{

	class BaseWindow 
		: public TWindow
		, public Container< ILevelComponent >
	{
		PFTYPEDEF_DC( BaseWindow, TWindow )
	public:
	private:
	};

}