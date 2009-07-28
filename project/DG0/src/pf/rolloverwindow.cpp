//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#include "rolloverwindow.h"

PFTYPEIMPL_DC(TRolloverWindow);


TRolloverWindow::TRolloverWindow() :
	mActivated(false)
{

}

TRolloverWindow::~TRolloverWindow()
{
	if (mActivated)
	{
		TPlatform::GetInstance()->GetWindowManager()->RemoveMouseListener(this);
		Activate(false);
	}
}

void TRolloverWindow::PostChildrenInit(TWindowStyle & style)
{
	TWindow::PostChildrenInit(style);
	mActivateWindow = style.GetString("activate", "");
	mActivateText = style.GetString("text", "");

}

bool TRolloverWindow::OnMouseMove(const TPoint & point)
{
	if (mActivated)
	{
		TRect client ;
		GetClientRect(&client);

		if (!client.IsInside(point))
		{
			TPlatform::GetInstance()->GetWindowManager()->RemoveMouseListener(this);
			mActivated = false;
			Activate(false);

		}
	}
	else
	{
		mActivated = true;
		TPlatform::GetInstance()->GetWindowManager()->AddMouseListener(this);
		Activate(true);
	}

	return false;
}

bool TRolloverWindow::OnMouseLeave()
{
	if (mActivated)
	{
		TPlatform::GetInstance()->GetWindowManager()->RemoveMouseListener(this);
		Activate(false);
		mActivated = false;
	}
	return false;
}

void TRolloverWindow::Activate(bool bOn)
{
	if (mActivateWindow.empty() == false)
	{
		TWindow *modal = FindParentModal();
		if (modal)
		{
			TWindow *child = modal->GetChildWindow(mActivateWindow);
			if (child)
			{
				if (bOn)
				{
					child->SetFlags(child->GetFlags() | kEnabled);
					if (mActivateText.empty() == false)
					{
						TText *text = child->GetCast<TText>();
						if (text)
						{
							text->SetText(mActivateText);
						}
					}
				}
				else
				{
					child->SetFlags(child->GetFlags() & ~kEnabled);
				}
			}
		}
	}
	TWindowManager::GetInstance()->InvalidateScreen();
}