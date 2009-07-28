//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#include "credits.h"

#include <pf/animtask.h>
#include <pf/windowmanager.h>
#include <pf/script.h>
#include <pf/luatable.h>
#include <pf/file.h>
#include <pf/windowstyle.h>
#include <pf/renderer.h>

//------------------------------------------------------------------
static str CreditLine(char* b, str headerColor)
{
	int len= (int)strlen(b);
	// trim whitespace
	while (b[len-1] == ' ' || b[len-1] == 0x09) {
		b[len-1]=0;
		len--;
	}
	while (b[0] ==  ' ' || b[0] == 0x09) {
		b++;
	}

	str line;
	for (int i=0; i<len; i++) {
		if (b[i] == '[') {
			line += str(str("<font color='") + headerColor + str("'>"));
		} else if (b[i] == ']') {
			line += str("</font>");
		} else {
			char foo[2];
			foo[0]= b[i];
			foo[1]= 0;
			line += str(foo);
		}
	}
	line += "\n";
	return line;
}

//------------------------------------------------------------------
PFTYPEIMPL_DC(TCredits);

TCredits::TCredits()
{
	mText1 = NULL;
	mText2 = NULL;
	mText3 = NULL;
	mStart = 0;
	mColumnGap = 0;
	mColumnWidth = 0;
	mCreditsTime = 0;
	mCreditsIntroPause = 0;
}

TCredits::~TCredits()
{
	mText1->Destroy();
	mText2->Destroy();
	mText3->Destroy();
}

void TCredits::Init(TWindowStyle & style)
{
	TWindow::Init(style);

	mCreditsTime = style.GetInt("time");
	mCreditsIntroPause = style.GetInt("intropause");
	mColumnGap = style.GetInt("columngap");
	mColumnWidth = style.GetInt("columnwidth");

	ASSERT(mColumnWidth);
	ASSERT(mColumnGap);
	ASSERT(mCreditsTime);

	mFont = style.GetString( "font","");
    mFontSize = (int)style.GetNumber( "fontsize", 16 );
	mFontColor = style.GetColor( "fontcolor", TColor(1,1,1,1));
	TColor headerColor = style.GetColor( "headercolor", TColor(1,1,1,1));
	mHeaderColorHex.format("%02x%02x%02x%02x", (int)(headerColor.r * 255), (int)(headerColor.g * 255), (int)(headerColor.b * 255), (int)(headerColor.a * 255));
	mCreditsFile = style.GetString("file","");

	TFile* fp = pf_open(mCreditsFile.c_str(), kReadText);

	str col1="\n";
	str col2="\n";
	str col3="\n";
	const int kSize= 1024;
	char buffer[kSize];
	while (pf_gets(fp, buffer, kSize)) 
	{
		buffer[strlen(buffer)-1]=0;
		char* sep = strchr(buffer, '|');
		if (sep) {
			*sep= 0;
			sep++;
			col1 += CreditLine(buffer, mHeaderColorHex);
			col2 += CreditLine(sep, mHeaderColorHex);
			col3 += str("\n");
		} else {
			col1 += str("\n");
			col2 += str("\n");
			col3 += CreditLine(buffer, mHeaderColorHex);
		}
	}

	pf_close(fp);

	int space = GetWindowHeight();

	for (uint32_t i=0; i<space / mFontSize; i++) 
	{
		col1 += "\n";
		col2 += "\n";
		col3 += "\n";
	}

	mText1 = TTextGraphic::Create(col1+col1, mColumnWidth, space, TText::kHAlignCenter, mFont.c_str(), mFontSize, mFontColor);
	mText2 = TTextGraphic::Create(col2+col2, mColumnWidth, space, TText::kHAlignCenter, mFont.c_str(), mFontSize, mFontColor);
	mText3 = TTextGraphic::Create(col3+col3, mColumnWidth*2+mColumnGap, space, TText::kHAlignCenter, mFont.c_str(), mFontSize, mFontColor);

	TRect bounds;
	mText1->GetTextBounds(&bounds);
	mHeight = bounds.mY2/2;
	mStart = 0;

	StartWindowAnimation(16);
}

void TCredits::Draw()
{
	if (mStart == 0) {
		mStart= TPlatform::GetInstance()->Timer();
	}

	unsigned long now = TPlatform::GetInstance()->Timer();
	unsigned long elapsed = (now - mStart);
	if (elapsed < mCreditsIntroPause) {
		elapsed = 0;
	} else {
		elapsed -= mCreditsIntroPause;
	}
	elapsed = elapsed % mCreditsTime;

	TReal pos = (TReal)(elapsed * mHeight)/mCreditsTime;

	TRenderer::GetInstance()->Begin2d();

	uint32_t middle = GetWindowWidth()/2 ;

	TRect screenRect;

	GetWindowRect(&screenRect);

	TRect dst(
		mColumnGap, -(int32_t)mFontSize, 
		middle-mColumnGap/2, GetParentRelativeRect().GetHeight()+mFontSize*2
		);

	mText1->SetStartLine(pos/mFontSize);
	mText1->Draw(dst);

	dst.x1 = GetWindowWidth()/2+mColumnGap/2;
	dst.x2 = GetWindowWidth()-mColumnGap;
	mText2->SetStartLine(pos/mFontSize);
	mText2->Draw(dst);

	dst.x1 = mColumnGap;
	dst.x2 = GetWindowWidth()-mColumnGap;
	mText3->SetStartLine(pos/mFontSize);
	mText3->Draw(dst);

	TRenderer::GetInstance()->End2d();
}


/**
 * Called if you have initiated a window animation with
 * TWindow::StartWindowAnimation.
 *
 * @return True to continue animating. False to stop.
 */
bool TCredits::OnTaskAnimate()
{
	TWindowManager::GetInstance()->InvalidateScreen();
	return true;
}
