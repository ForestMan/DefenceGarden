//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef MAINMENU_H_INCLUDED
#define MAINMENU_H_INCLUDED

#include <pf/pflib.h>
#include <pf/sound.h>

class TMainMenu : public TWindow {
	PFTYPEDEF_DC(TMainMenu,TWindow)
public:
	TMainMenu();
	~TMainMenu();

	virtual void PostChildrenInit(TWindowStyle & style);
	
private:
	void SetWelcomeName();
	TSoundRef mMusic;
};
#endif
