//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------


#include "serversubmit.h"

PFTYPEIMPL_DC(TServerSubmit);


TServerSubmit::TServerSubmit()
{
	mpPFHiscores = TSettings::GetInstance()->GetHiscores();

	TScript * s = TWindowManager::GetInstance()->GetScript();
	ScriptRegisterMemberDirect(s,"SubmitToServer",this,TServerSubmit::SubmitToServer);
}

TServerSubmit::~TServerSubmit()
{
	TScript * s = TWindowManager::GetInstance()->GetScript();
	ScriptUnregisterFunction(s,"SubmitToServer");
}

void TServerSubmit::Register()
{
	TWindowManager::GetInstance()->AddWindowType("SubmitWindow",TServerSubmit::ClassId());
	ScriptRegisterDirect(TWindowManager::GetInstance()->GetScript(), "GetLuaServerSubmitSetupVars", TServerSubmit::GetLuaSetupVars);

}

void TServerSubmit::SwitchModes( bool submit )
{
	TWindowManager::GetInstance()->GetScript()->DoLuaString( str::getFormatted("SwitchModes(%s)",submit?"true":"false") );
}

void TServerSubmit::SubmitToServer(str name, str account, str pass, bool remember, bool medalsOnly)
{
	bool success;
	TPfHiscores::ESubmitMode mode = TPfHiscores::kSubmitAll;
	if (medalsOnly)
	{
		mode = TPfHiscores::kSubmitMedal;
	}
	if (account.length() > 0)
	{
		success=mpPFHiscores->SubmitData(account.c_str(), pass.c_str(), remember, mode);
	}
	else
	{
		TSettings::GetInstance()->GetPreferences()->SetStr("_anonusername", name, TSettings::GetInstance()->GetCurrentUser(), true );
		success=mpPFHiscores->SubmitData(name.c_str(), NULL, false, mode);
	}

	ASSERT(success);

	SwitchModes(true);
	StartWindowAnimation(20);
}

str TServerSubmit::GetLuaSetupVars(bool medalsMode)
{
	str setupString;
	str tmp ;

	tmp = TSettings::GetInstance()->GetPreferences()->GetStr("_anonusername","", TSettings::GetInstance()->GetCurrentUser() );

	setupString.format("gNameEdit='%s'; ", tmp.c_str() );

	str userName;
	str password;
	if (TSettings::GetInstance()->GetHiscores()->GetRememberedUserInfo(&userName, &password))
	{
		setupString += str::getFormatted("gAccountEdit='%s'; ", userName.c_str() );
		setupString += str::getFormatted("gPassEdit='%s'; ", password.c_str() );
		setupString += "gRemember=true; ";
	}
	else
	{
		setupString += "gAccountEdit=''; gPassEdit=''; gRemember=false; ";
	}



	if (medalsMode == true)
	{
		setupString+= "gMedalsMode=true;";
	}
	else
	{
		setupString+= "gMedalsMode=false;";
		int score = 0;
		TSettings::GetInstance()->GetHiscores()->GetUserBestScore(TPfHiscores::eLocalBest, &score, NULL, NULL, 0);
		setupString += str::getFormatted("gEligibleScore=%d", score);
	}
	return setupString;
}

bool TServerSubmit::OnMessage(TMessage * message)
{
	if (message->mType == TMessage::kTextEditChanged)
	{
		// Enable/disable the window based on text value.
		//
		// We can't use the magic TCustomTextEdit enable window feature here because
		// either of the two edit fields can have text to enable the button.
		TTextEdit *textEdit = GetChildWindow("nameedit", -1)->GetCast<TTextEdit>();
		TTextEdit *textEdit2 = GetChildWindow("accountedit", -1)->GetCast<TTextEdit>();
		if (textEdit->GetText().length() > 0 || textEdit2->GetText().length() > 0)
		{
			TWindow *okwindow = GetChildWindow("submittoserver", -1);
			okwindow->SetFlags(okwindow->GetFlags() | kEnabled);
		}
		else
		{
			TWindow *okwindow = GetChildWindow("submittoserver", -1);
			okwindow->SetFlags(okwindow->GetFlags() & ~kEnabled);
			TButton *button = okwindow->GetCast<TButton>();
			button->OnMouseLeave();
		}
		TWindowManager::GetInstance()->InvalidateScreen();
		return true;
	}

	return false;
}

bool TServerSubmit::OnTaskAnimate()
{
	char msg[512];
	bool qual;
	TPfHiscores::EStatus status = mpPFHiscores->GetServerRequestStatus(msg, 512, &qual);
	if (status == TPfHiscores::eError)
	{
		TWindow *childWindow;

		childWindow = GetChildWindow("submiterror", -1);
		if (childWindow)
		{
			TText *text = childWindow->GetCast<TText>();
			text->SetFlags(text->GetFlags() | TWindow::kEnabled);
			text->SetText(msg);
		}

		TWindow *button;
		button = GetChildWindow("submitconnect", -1);
		if (button) button->SetFlags(button->GetFlags() & ~TWindow::kEnabled);

		button = GetChildWindow("submiterrorok", -1);
		if (button) button->SetFlags(button->GetFlags() | TWindow::kEnabled);

		return false;
	}
	else if (status == TPfHiscores::eSuccess)
	{
		if (qual)
		{
			TWindowManager::GetInstance()->GetScript()->DoLuaString("gReturnValue='qualified'");
		}
		else
		{
			TWindowManager::GetInstance()->GetScript()->DoLuaString("gReturnValue='success'");
		}
		TPlatform::GetInstance()->GetWindowManager()->PopModal( FindParentModal()->GetID());
		return false;
	}
	return true;
}