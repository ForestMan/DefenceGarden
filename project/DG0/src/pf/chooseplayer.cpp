//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#include "chooseplayer.h"
#include "settings.h"

#include <pf/windowmanager.h>
#include <pf/script.h>


static const int MAX_PLAYERS = 10;

PFTYPEIMPL_DC(TChoosePlayer);

TChoosePlayer::TChoosePlayer()
{
	TWindowManager *wm = TPlatform::GetInstance()->GetWindowManager();

	ScriptRegisterMemberDirect( wm->GetScript(), "SetPlayerChoices", this, TChoosePlayer::SetPlayerChoices );
	ScriptRegisterMemberDirect( wm->GetScript(), "GetCurrentSelectedUser", this, TChoosePlayer::GetCurrentSelectedUser );
	ScriptRegisterMemberDirect( wm->GetScript(), "GetSelectedPlayerName", this, TChoosePlayer::GetSelectedPlayerName );
}


TChoosePlayer::~TChoosePlayer()
{

}


void TChoosePlayer::PostChildrenInit(TWindowStyle & style)
{
	TWindow::PostChildrenInit(style);
	SetPlayerChoices();
}

void TChoosePlayer::SetPlayerChoices()
{
	int numUsers = TSettings::GetInstance()->GetNumUsers();

	str nameList = "{";

	for (int i = 0; i < numUsers && i < MAX_PLAYERS; i++)
	{
		nameList += str("'") + TSettings::GetInstance()->GetUserName(i)+"',";
	}

	nameList += "}";

	int currentUser = TSettings::GetInstance()->GetCurrentUser();

	TScript * s = TWindowManager::GetInstance()->GetScript();
	s->DoLuaString( str::getFormatted("UpdatePlayerChoices( %d, %d, %s )", currentUser, numUsers, nameList.c_str() ) );
}


int TChoosePlayer::GetCurrentSelectedUser()
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		str buttonName;
		buttonName.format("button%d", i);
		TWindow *window = GetChildWindow(buttonName, -1);
		if (window)
		{
			TButton *button = window->GetCast<TButton>();
			if (button)
			{
				if (button->GetOn())
				{
					return i;
				}
			}
		}
	}

	return 0;
}

str TChoosePlayer::GetSelectedPlayerName()
{
	return TSettings::GetInstance()->GetUserName(GetCurrentSelectedUser());
}


