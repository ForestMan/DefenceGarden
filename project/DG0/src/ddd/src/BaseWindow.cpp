#include "ddd/BaseWindow.h"

#include <pf/pflib.h>
#include "pf/debug.h"
#include "ddd/Application.h"
#include "ddd/Game.h"

namespace ddd
{
	PFTYPEIMPL_DC(BaseWindow);

	BaseWindow::BaseWindow()
	{
	}

	void BaseWindow::onInit()
	{
		initLuaFunction( 0, "onInit" );
		//ddd::Application::get_mutable_instance().getEntity( getGameID() ).addEntity( *this );

		executeLuaFunction( 0 );
	}
	
	void BaseWindow::onRelease()
	{
		releaseLuaFunction( 0 );
	}
}