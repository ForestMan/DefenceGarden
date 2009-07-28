//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------


#include "options.h"
#include "settings.h"
#include "globaldefines.h"

#include <pf/slider.h>
#include <pf/windowmanager.h>
#include <pf/script.h>
#include <pf/message.h>
#include <pf/soundmanager.h>

PFTYPEIMPL_DC(TOptions);

TOptions::TOptions()
{
	TWindowManager *wm = TPlatform::GetInstance()->GetWindowManager();

	ScriptRegisterMemberDirect( wm->GetScript(), "SaveVolumes", this, TOptions::SaveVolumes );
}
	
TOptions::~TOptions()
{
	
}

void TOptions::PostChildrenInit(TWindowStyle & /*style*/)
{
	TWindow *window = GetChildWindow("fullscreen", -1);
	if (window)
	{
		TButton *button = window->GetCast<TButton>();
		if (button)
		{
			if (TSettings::GetInstance()->IsFullScreen())
			{
				button->SetOn(true);
			}
			else
			{
				button->SetOn(false);
			}
		}
	}

	window = GetChildWindow("mutebox", -1);
	if (window)
	{
		TButton *button = window->GetCast<TButton>();
		if (button)
		{
			int currentUser = TSettings::GetInstance()->GetPreferences()->GetInt("curuser", TPrefs::kGlobalIndex, TPrefs::kGlobalIndex);
			int soundoff = TSettings::GetInstance()->GetPreferences()->GetInt("soundoff", 0, currentUser);
			if (soundoff == 1)
			{
				button->SetOn(true);
			}
			else
			{
				button->SetOn(false);
			}
		}
	}

	window = GetChildWindow("sfxlevelslider", -1);
	if (window)
	{	
		TSlider *slider = window->GetCast<TSlider>();
		if (slider)
		{
			int currentUser = TSettings::GetInstance()->GetPreferences()->GetInt("curuser", TPrefs::kGlobalIndex, TPrefs::kGlobalIndex);
			int soundVal = TSettings::GetInstance()->GetPreferences()->GetInt("soundlevel", DEFAULT_SOUND_VOLUME, currentUser);
			slider->SetValue((float)soundVal/1000.0f);
		}
	}

	window = GetChildWindow("musiclevelslider", -1);
	if (window)
	{	
		TSlider *slider = window->GetCast<TSlider>();
		if (slider)
		{
			int currentUser = TSettings::GetInstance()->GetPreferences()->GetInt("curuser", TPrefs::kGlobalIndex, TPrefs::kGlobalIndex);
			int soundVal = TSettings::GetInstance()->GetPreferences()->GetInt("musiclevel", DEFAULT_SOUND_VOLUME, currentUser);
			slider->SetValue((float)soundVal/1000.0f);
		}
	}
}

void TOptions::SaveVolumes()
{
	TWindow *window;


	window = GetChildWindow("sfxlevelslider", -1);
	if (window)
	{	
		TSlider *slider = window->GetCast<TSlider>();
		if (slider)
		{
			int currentUser = TSettings::GetInstance()->GetPreferences()->GetInt("curuser", TPrefs::kGlobalIndex, TPrefs::kGlobalIndex);
			float sliderVal = slider->GetValue();
			TSettings::GetInstance()->GetPreferences()->SetInt("soundlevel", (int)(sliderVal * 1000.0f), currentUser);
		}
	}

	window = GetChildWindow("musiclevelslider", -1);
	if (window)
	{	
		TSlider *slider = window->GetCast<TSlider>();
		if (slider)
		{
			int currentUser = TSettings::GetInstance()->GetPreferences()->GetInt("curuser", TPrefs::kGlobalIndex, TPrefs::kGlobalIndex);
			float sliderVal = slider->GetValue();
			TSettings::GetInstance()->GetPreferences()->SetInt("musiclevel", (int)(sliderVal * 1000.0f), currentUser);
		}
	}
}


bool TOptions::OnMessage(TMessage * message)
{

	bool handled = false;
	if (message->mType == TMessage::kSliderValChanged)
	{
		if (message->mName == "sfxlevelslider")
		{
			TWindow *window = GetChildWindow("sfxlevelslider", -1);
			if (window)
			{
				TSlider *slider = window->GetCast<TSlider>();
				if (slider)
				{
					float val = slider->GetValue();
					TPlatform::GetInstance()->GetSoundManager()->SetTypeVolume(kSFXSoundGroup, val);
					TPlatform::GetInstance()->GetSoundManager()->SetTypeVolume(TSound::kButtonSound, val);
					handled = true;
				}
				
			}
		}
		else if (message->mName == "musiclevelslider")
		{
			TWindow *window = GetChildWindow("musiclevelslider", -1);
			if (window)
			{
				TSlider *slider = window->GetCast<TSlider>();
				if (slider)
				{
					float val = slider->GetValue();
					TPlatform::GetInstance()->GetSoundManager()->SetTypeVolume(kMusicSoundGroup, val);
					handled = true;
				}
			}
		}
	}


	return handled;
}
