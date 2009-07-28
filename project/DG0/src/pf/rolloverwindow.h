//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef ROLLOVERWINDOW_H_INCLUDED
#define ROLLOVERWINDOW_H_INCLUDED

#include <pf/pflib.h>

// A rollover window is a window that handles mouse enter and leave
// events, and when an event is detected, it turns on/off some
// other window.
class TRolloverWindow : public TWindow
{
	PFTYPEDEF_DC(TRolloverWindow,TWindow)
public:
	TRolloverWindow();
	~TRolloverWindow();

	virtual void PostChildrenInit(TWindowStyle & style);
	virtual bool OnMouseMove(const TPoint & point);
	virtual bool OnMouseLeave();

private:
	void Activate(bool bOn);

	str mActivateWindow;
	str mActivateText;
	bool mActivated;
};



#endif