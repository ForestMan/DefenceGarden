#include "ddd/LevelWindow.h"

#include <pf/pflib.h>
#include "pf/debug.h"

#include "ddd/Application.h"
#include "ddd/Game.h"

namespace ddd
{
	//-------------------------------------------------------------------------

	PFTYPEIMPL_DC(LevelWindow);

	//-------------------------------------------------------------------------

	LevelWindow::LevelWindow()
	{
	}

	//-------------------------------------------------------------------------

	LevelWindow::~LevelWindow()
	{
	}

	//-------------------------------------------------------------------------

	void LevelWindow::Init(TWindowStyle &/*style*/)
	{
		// Start the window animation
		StartWindowAnimation( 16 );

		// Send keyboard events our way if no one else uses them
		FindParentModal()->SetDefaultFocus(this);

		Application::get_mutable_instance().getEntity( 1 ).addEntity( *this );
	}

	//-------------------------------------------------------------------------

	void LevelWindow::Draw()
	{
		getRenderComponent().render(this);
	}

	//-------------------------------------------------------------------------

	bool LevelWindow::OnTaskAnimate()
	{
		getLogicComponent().update(this);
		return true;
	}

	//-------------------------------------------------------------------------
}