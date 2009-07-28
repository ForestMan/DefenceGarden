//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef OPTIONS_H_INCLUDED
#define OPTIONS_H_INCLUDED

#include <pf/pflib.h>

class TOptions : public TWindow
{
	PFTYPEDEF_DC(TOptions,TWindow)
public:
	TOptions();
	~TOptions();

	virtual void PostChildrenInit(TWindowStyle & style);

	void SaveVolumes();
	virtual bool OnMessage(TMessage * message);
};

#endif
