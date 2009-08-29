
require( "scripts/style.lua" );

kButtonSpacing = 45;
kButtonY = 293;
kButtonX = 0;

function CheckUsers()
	if (GetNumUsers() == 0) then
		gEnterNameValue = "";
		name = DoModal("scripts/entername.lua");
		if (not(name == nil)) then
			CreateNewUser(name);
			return true ;
		end
		return false ;
	else
		return true;
	end
end

function QuitGame()
	CloseWindow();
end

MakeDialog
{
	-- Name the modal dialog
	name="mainmenu",
	MainMenu
	{
		fit=true;
		Bitmap
		{
			name="backgroundimage",
			image="backgrounds/mainmenu",

			Text
			{
				font = StandardButtonFont,
				name = "welcome",
				x=0,y=550,w=kMax,h=50,
				flags = kHAlignCenter + kVAlignCenter,
				label = "welcome"
			};

			SetStyle( MenuButtonStyle ),
			--MenuButton
			--{
			--	x=kButtonX,
			--	y=kButtonY,
			--	name="gamemode1",
			--	MenuButtonLabel("gamemode1"),
			--	type = kPush,
			--	command=
			--		function()
			--			if (CheckUsers()) then
			--				-- Tell TSettings what game mode we're entering
			--				SetCurrentGameMode(0);
			--				-- Switch to the appropriate screen
			--				SwapToModal("scripts/twodemos.lua");
			--				SetState( kGameMode, "twodemos" );
			--			end
			--		end
			--},

			MenuButton
			{
				x=kButtonX,
				y=kButtonY + (kButtonSpacing * 1),
				name="gamemode2",
				MenuButtonLabel("gamemode2"),
				type = kPush,
				command=
					function()
						-- Set up the number of rows and columns
						gRows=5; gColumns=5;
						if (CheckUsers()) then
							-- Tell TSettings what game mode we're entering
							SetCurrentGameMode(1);
							-- Switch to the appropriate screen
							SwapToModal("scripts/DGGameWindow.lua");
							SetState( kGameMode, "samplegame" );
						end
					end
			},

			MenuButton
			{
				x=kButtonX,
				y=kButtonY + (kButtonSpacing * 2),
				name="options",
				MenuButtonLabel("options"),
				type = kPush,
				command=
					function()
						DoModal("scripts/options.lua");
					end
			},

			MenuButton
			{
				x=kButtonX,
				y=kButtonY + (kButtonSpacing * 3),
				name="highscores",
				MenuButtonLabel("highscores"),
				type = kPush,
				command=
					function()
						SwapToModal("scripts/hiscore.lua");
					end
			},
			
			MenuButton
			{
				x=kButtonX,
				y=kButtonY + (kButtonSpacing * 4),
				name="medals",
				MenuButtonLabel("medals"),
				type = kPush,
				command=
					function()
						SwapToModal("scripts/medals.lua");
					end
			},

			MenuButton
			{
				x=kButtonX,
				y=kButtonY + (kButtonSpacing * 5),
				name="quit",
				MenuButtonLabel("quit"),
				type = kPush,
				command=
					function()
						result = DisplayDialog {
							"scripts/yesno.lua",
							body ="quitconfirm",
							title = "quittitle"
						};
						if (result=='yes') then
							PostMessage( CreateNamedMessage( kQuitNow, "Quit" ) );
						end
					end
			},

			SetStyle(LongButtonStyle),
			Button
			{
				x=610,
				y=480,
				name="changeplayer",
				label="changeplayer",
				type = kPush,
				command=
					function()
						DoModal("scripts/chooseplayer.lua");
					end
			},
			Button
			{
				x=610,
				y=520,
				name="help",
				label="help",
				type = kPush,
				command=
					function()
						DoModal("scripts/help.lua");
					end
			},
		},
	}
}