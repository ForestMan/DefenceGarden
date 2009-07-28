//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------


#ifndef SERVERSUBMIT_H_INCLUDED
#define SERVERSUBMIT_H_INCLUDED

#include <pf/pflib.h>
#include "hiscore.h"

class TServerSubmit : public TWindow
{
	PFTYPEDEF_DC(TServerSubmit,TWindow)
public:
	TServerSubmit();
	~TServerSubmit();

	static void Register();

	// This function should be called before building this window - it sets up 
	// various variables and state for the window.
	static str GetLuaSetupVars(bool medalsMode);
	
	void SwitchModes( bool submit );
	void SubmitToServer(str name, str account, str pass, bool remember, bool medalsOnly);

	virtual bool OnMessage(TMessage * message);
	virtual bool OnTaskAnimate();


private:
	
	TServerSubmit(TServerSubmit &rhs);
	TServerSubmit& operator= (const TServerSubmit& f);

	TPfHiscores *mpPFHiscores;
	
};

#endif //SERVERSUBMIT_H_INCLUDED