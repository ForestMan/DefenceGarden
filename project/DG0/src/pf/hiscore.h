//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------



#ifndef HISCORE_H_INCLUDED
#define HISCORE_H_INCLUDED

#include "game.h"
#include "settings.h"

#include <pf/pflib.h>


const int NUM_SCORES_ON_SCREEN = 10;

class THiscore : public TWindow 
{
	PFTYPEDEF_DC(THiscore,TWindow)
public:
		void RequestScores();
		bool ScoreAvailable();
		bool EligibleScoreAvailable();
	THiscore();
	~THiscore();

	virtual void PostChildrenInit(TWindowStyle & style);
	virtual bool OnMessage(TMessage * message);
	void SubmissionDone(bool qualified);

	/**
	 * Called if you have initiated a window animation with
	 * TWindow::StartWindowAnimation.
	 *
	 * @return True to continue animating. False to stop.
	 */
	virtual bool OnTaskAnimate();
private:
	enum EState {eLocalView, eRequestingCategories, eRequestingScores, eSubmitting, eGlobalView, eError};
	enum ESubmitState {eNone, eSuccess, eDNQ};

	void UpdateText();
	void UpdateScoresTable();
	void UpdateButtons();
	void HideAllText();
	void HideAllButtons();
	void LaunchPrivacyPolicy();
	int GetState()			{ return (int)mState; }
	
	EState					mState;
	ESubmitState			mSubmitState;
	TPfHiscores				*mpPFHiscores;
	str						mServerError;
	int						mCurrentCategory;
	int						mCurrentEditField;
	int						mScoreOffset;
	TText					*mNames[NUM_SCORES_ON_SCREEN];
	TText					*mScores[NUM_SCORES_ON_SCREEN];
	TText					*mNumbers[NUM_SCORES_ON_SCREEN];
	TImage					*mP1s[NUM_SCORES_ON_SCREEN];
	TText					*mGameMode;
};



#endif //SCRNSTART_H_INCLUDED
