//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------



#ifndef CHOOSEPLAYER_H_INCLUDED
#define CHOOSEPLAYER_H_INCLUDED

#include <pf/pflib.h>

class TChoosePlayer : public TWindow 
{

PFTYPEDEF_DC(TChoosePlayer,TWindow);
public:
	TChoosePlayer();
	~TChoosePlayer();
	
	virtual void PostChildrenInit(TWindowStyle & style);
	void SetPlayerChoices();
	int GetCurrentSelectedUser();
	str GetSelectedPlayerName();

private:
	

};



#endif //SCRNSTART_H_INCLUDED
