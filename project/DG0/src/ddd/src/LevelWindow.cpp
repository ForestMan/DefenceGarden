#include "ddd/LevelWindow.h"

#include <pf/pflib.h>
#include "pf/debug.h"

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

	void LevelWindow::Init(TWindowStyle &style)
	{
		// Start the window animation
		StartWindowAnimation( 16 );

		// Send keyboard events our way if no one else uses them
		FindParentModal()->SetDefaultFocus(this);
	}

	//-------------------------------------------------------------------------

	void LevelWindow::Draw()
	{
	}

	//-------------------------------------------------------------------------

	bool LevelWindow::OnTaskAnimate()
	{
		return true;
	}

	//-------------------------------------------------------------------------
}