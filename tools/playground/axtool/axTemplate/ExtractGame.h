//------------------------------------------------------------------
// Copyright (c) 2006 PlayFirst, Inc.
//
// This material is the confidential trade secret and proprietary
// information of PlayFirst, Inc. It may not be reproduced, used,
// sold or transferred to any third party without the prior written
// consent of PlayFirst, Inc. All rights reserved.
//------------------------------------------------------------------

#ifndef EXTRACTGAME_H_INCLUDED
#define EXTRACTGAME_H_INCLUDED

#include <windows.h>

//------------------------------------------------------------------
class TExtractGame {
public:
	TExtractGame(HMODULE module);
	~TExtractGame();

private:
	char	mDir[MAX_PATH];
	char	mSave[MAX_PATH];
};

//------------------------------------------------------------------
#endif // EXTRACTGAME_H_INCLUDED
