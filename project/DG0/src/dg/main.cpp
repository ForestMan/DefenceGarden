//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------



#define PLAYFIRST_MAIN
#define PLAYGROUND_INIT

#include "main.h"
#include "globaldefines.h"
#include "game.h"
#include "mainmenu.h"
#include "options.h"
#include "settings.h"
#include "hiscore.h"
#include "serversubmit.h"
#include "chooseplayer.h"
#include "customtextedit.h"
#include "credits.h"
#include "key.h"
#include "rolloverwindow.h"

#include <pf/windowmanager.h>
#include <pf/image.h>
#include <pf/button.h>
#include <pf/texture.h>
#include <pf/event.h>
#include <pf/task.h>
#include <pf/stringtable.h>
#include <pf/screen.h>
#include <pf/slider.h>
#include <pf/script.h>
#include <pf/gamestate.h>

#include <pf/prefsdb.h>

#include "ddd/Application.h"

void ddd::Application::initGameStates()
{
	TGameState * gs = TGameState::GetInstance();
	
	// Start the session timer
	gs->SetState("SessionTime",1);

    // Create a metric line that will be written when the
    // session completes.
	gs->SetState("begin-session","SessionData");
	gs->SetState("add-value", TPlatform::GetConfig(TPlatform::kGameVersion)); // Write our version
	gs->SetState("add-timer","SessionTime");		// Write the session timer
	gs->SetState("add-state","LastGameHiScore");	// Write the last game's high score
	gs->SetState("add-state","HighestSwarmLevel");	// Write the highest swarm level achieved
	gs->SetState("end-session","SessionData");

    // Create a metric line that will be written when the
    // user has played the game for 15 minutes cumulatively
	gs->SetState("begin-elapsed(15)","Progress-15");
	gs->SetState("add-state","PlayedSimpleGame");	// Write how many times they played the Simple Game
	gs->SetState("end-elapsed(15)","Progress-15");

    // Create a metric line that will be written when the
    // user has played the game for 30 minutes cumulatively
	gs->SetState("begin-elapsed(30)","Progress-30");
	gs->SetState("add-state","PlayedSimpleGame");	// Write how many times they played the Simple Game
	gs->SetState("end-elapsed(30)","Progress-30");

    // Create a metric line that will be written when the
    // user has played the game for 45 minutes cumulatively
	gs->SetState("begin-elapsed(45)","Progress-45");
	gs->SetState("add-state","PlayedSimpleGame");	// Write how many times they played the Simple Game
	gs->SetState("end-elapsed(45)","Progress-45");

    // Create a metric line that will be written when the
    // user has played the game for 59 minutes cumulatively
	gs->SetState("begin-elapsed(59)","Progress-59");
	gs->SetState("add-state","PlayedSimpleGame");	// Write how many times they played the Simple Game
	gs->SetState("end-elapsed(59)","Progress-59");
}

void ddd::Application::initWindows( TPlatform* pPlatform )
{
	// Default to the new subtractive renderer behavior, which is
	// correctly cross-platform to the Mac.
	TRenderer::GetInstance()->SetOption("new_subtractive","1");

	// Default to the new rendering blend mode; similarly
	// performs more correctly cross-platform to the Mac
	TRenderer::GetInstance()->SetOption("new_render_blend","1");

	// Default to non-square textures on the Mac to save video memory
	// By default TRenderer::GetTextureSquareFlag() is true on Mac,
	// but no supported Mac requires square textures, so this option
	// can be used to change it to false, allowing TTexture::GetInternalSize()
	// to be closer to the size of the image loaded. For instance,
	// 1000x100 would by default have an internal size of 1024x1024, but
	// with the below, it would be 1024x128. This option has no affect
	// on Windows, which correctly detects whether the video card
	// requires square textures.
	TRenderer::GetInstance()->SetOption("texture_rectangle","1");

	// Set the application name
	pPlatform->SetWindowTitle(pPlatform->GetStringTable()->GetString("windowtitle"));

	TSettings::CreateSettings();
	TSettings::GetInstance()->InitGameToSettings();

	pPlatform->SetCursor( TTexture::GetSimple("cursor/cursor"), TPoint(1,1) );

	pPlatform->SetCursor( TTexture::Get("cursor/thumb.png"), TPoint(12,2), true );

	TWindowManager * wm = TWindowManager::GetInstance();
	wm->AddWindowType("GameWindow",TGameWindow::ClassId());
	wm->AddWindowType("MainMenu",TMainMenu::ClassId());
	wm->AddWindowType("OptionsWindow",TOptions::ClassId());
	wm->AddWindowType("HiscoreWindow",THiscore::ClassId());
	TServerSubmit::Register();
	wm->AddWindowType("ChoosePlayerWindow",TChoosePlayer::ClassId());
	wm->AddWindowType("CreditsWindow",TCredits::ClassId());
    //wm->AddWindowType("Swarm", TSwarm::ClassId());
    //wm->AddWindowType("ChessPiece", TChessPiece::ClassId());
	wm->AddWindowType("RolloverWindow", TRolloverWindow::ClassId());

    // Overriding the default TTextEdit window.
	wm->AddWindowType("TextEdit",TCustomTextEdit::ClassId());
}

// Early initialization of Playground values. TPlatform has not yet
// been initialized here, so only static functions on TPlatform are
// legal! At present this is basically only SetConfig(). This function
// is really designed only for setting up values that TPlatform will need
// during initialization; most application initialization still belongs in
// Main().
void PlaygroundInit()
{
	// Set the name of the game. This name will be used in the user: and
	// common: data folder names.
	TPlatform::SetConfig( TPlatform::kGameName, "DefenseGarden" );

	// Change this to change the publisher name from PlayFirst.
//	TPlatform::SetConfig( TPlatform::kPublisherName, "PlayFirst" );

	// Fix button mask scaling
	TPlatform::SetConfig(TPlatform::kFixButtonMaskScaling,"1");

	// Enable UTF-8 mode, so we can
	TPlatform::SetConfig( TPlatform::kUTF8Mode, "1" );

	// Enable auto-merge of .mask.png files
	TPlatform::SetConfig( TPlatform::kAutoMergeMask, "1" );

	//TPlatform::SetConfig( TPlatform::kVerboseDebug, "1" );

	// Enable TSprite::HitTest() of sprites that have no textures.
	// Previously TSprite::HitTest() would short-circuit to false if
	// the sprite had no texture. Since TSprite::HitTest() can also
	//
	TPlatform::SetConfig( TPlatform::kSpriteAlwaysTestChildren, "1" );

	// Set the PlayFirst game information. All PlayFirst games MUST set the
	// information below in order to successfully communicate with PlayFirst
	// services, such as the hiscore system. Removing this code will cause
	// the hiscore system to not work.
	// DO NOT REMOVE THIS CODE IF YOU PLAN ON USING PLAYFIRST SERVICES
	// ---------------------------------------------------------------
	TPlatform::SetConfig(TPlatform::kEncryptionKey,ENCRYPTION_KEY);
	TPlatform::SetConfig(TPlatform::kPFGameHandle,PFGAMEHANDLE);
	for (int i = 0; i < sizeof(PFGAMEMODENAMES)/sizeof(char *); i++)
	{
		str name = str::getFormatted("%s%d", TPlatform::kPFGameModeName, i+1);
		TPlatform::SetConfig(name,PFGAMEMODENAMES[i]);
	}

	for (int i = 0; i < sizeof(PFGAMEMEDALNAMES)/sizeof(char *); i++)
	{
		str name = str::getFormatted("%s%d", TPlatform::kPFGameMedalName, i+1);
		TPlatform::SetConfig(name,PFGAMEMEDALNAMES[i]);
	}
	// ---------------------------------------------------------------
}

/// Playfirst main application entry point
/// Uncomment cmdLine below if you need to process the command line
void Main(TPlatform* pPlatform, const char* /*cmdLine*/ )
{
	ddd::Application::get_mutable_instance().initPlayground(pPlatform);
	ddd::Application::get_mutable_instance().run(pPlatform);
}

