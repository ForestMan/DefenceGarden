//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED


#include <pf/prefsdb.h>
#include <pf/pfhiscores.h>

class TSettings
{
public:
	enum eGameMode { eGameMode1 = 0, eGameMode2, eMaxGameModes };

	static TSettings *GetInstance();
	static TSettings *CreateSettings();
	static void DeleteSettings();

	/// Initialize game using saved settings
	void InitGameToSettings();

	// Global helper functions
	void CreateNewUser(str userName);
	void ChangeCurrentUserName(str newName);
	str GetCurrentUserName();
	str GetUserName(int index);
	void SetCurrentUser(int userNum);
	int GetCurrentUser();
	int GetNumUsers();
	void DeleteUser(int userNum);
	bool IsNameInUse(str name);
	void QuitToMainMenu();

	eGameMode GetCurrentGameMode() {return mGameMode;}
	void SetCurrentGameMode(int gameMode);

	bool IsFullScreen();
	void UpdateFullScreen();
	bool ToggleFullScreen();

	void ToggleSoundMute();

	void LogHighScore(int score, bool medal1, bool medal2);

	TPrefsDB *GetPreferences() {return mPreferences;}
	TPfHiscores *GetHiscores() {return mHiscores;}

	str GetPFGameModeName(int index);
	str GetPFMedalName(int index);

	int NumMedalsToSubmit();
	int NumEarnedMedals();
	str GetMedalName(int index);
	int GetMedalType(int index);
	str GetMedalGameMode(int index);
	str GetMedalGameData(int index);

private:
	TSettings();
	~TSettings();

	void SetSoundForCurrentUser();

	TPrefsDB		*mPreferences;
	TPfHiscores		*mHiscores;

	eGameMode		mGameMode;
};

#endif
