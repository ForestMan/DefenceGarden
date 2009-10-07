#pragma once

#include <pf/window.h>
#include "ddd/Container.h"
#include "ddd/ILevelComponent.h"
#include <ddd/ILua.h> 

namespace ddd
{

	class BaseWindow 
		: public TWindow
		, public ddd::ILua
	{
		PFTYPEDEF_DC( BaseWindow, TWindow )
	public:
		BaseWindow();

		inline const unsigned long getGameID()const;
	protected:

		virtual void onInit();
		virtual void onRelease();
	};

	inline const unsigned long BaseWindow::getGameID()const
	{
		return getULong( getLuaTable(), "gameID_" );
	}

}