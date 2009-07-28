//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------


#include "mainmenu.h"
#include "globaldefines.h"
#include "settings.h"

#include <pf/stringtable.h>
#include <pf/windowmanager.h>
#include <pf/script.h>

PFTYPEIMPL_DC(TMainMenu);

TMainMenu::TMainMenu()
{
	mMusic = TSound::Get("audio/music/mainmenumusic.ogg", false, kMusicSoundGroup);
	mMusic->Play(true);

	TWindowManager *wm = TPlatform::GetInstance()->GetWindowManager();

	ScriptRegisterMemberDirect( wm->GetScript(), "SetWelcomeName", this, TMainMenu::SetWelcomeName );
}


TMainMenu::~TMainMenu()
{

}


void TMainMenu::PostChildrenInit(TWindowStyle & style)
{
	SetWelcomeName();
	TWindow::PostChildrenInit(style);
}


void TMainMenu::SetWelcomeName()
{
	if(TSettings::GetInstance()->GetNumUsers() == 0)
	{
		TWindow *window = GetChildWindow("welcome", -1);
		if (window)
		{
			window->SetFlags(window->GetFlags() & ~kEnabled);
		}

		window = GetChildWindow("changeplayer", -1);
		if (window)
		{
			window->SetFlags(window->GetFlags() & ~kEnabled);
		}

	}
	else
	{
		TWindow *window = GetChildWindow("welcome", -1);
		if (window)
		{
			TText *text = window->GetCast<TText>();
			text->SetText(TPlatform::GetInstance()->GetStringTable()->GetString("welcome", TSettings::GetInstance()->GetCurrentUserName()));
		}
		
	}
}
