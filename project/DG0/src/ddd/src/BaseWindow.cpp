#include "ddd/BaseWindow.h"

#include <pf/pflib.h>
#include "pf/debug.h"

namespace ddd
{
	PFTYPEIMPL_DC(BaseWindow);

	BaseWindow::BaseWindow()
	{
	}

	void BaseWindow::onInit()
	{
		initLuaFunction( 0, "onInit" );
		executeLuaFunction( 0 );
	}
	
	void BaseWindow::onRelease()
	{
		releaseLuaFunction( 0 );
	}
}