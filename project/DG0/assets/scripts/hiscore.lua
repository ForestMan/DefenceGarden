require( "scripts/style.lua" );

HeaderFont = {
  standardFont,
  70,
  BlueColor
};


GameModeFont = {
  standardFont,
  50,
  BlueColor
};

TableInfoFont = {
  standardFont,
  30,
  BlackColor
};

EligibleFont = {
  standardFont,
  18,
  BlackColor
};

EligibleAsteriskFont = {
  standardFont,
  12,
  BlackColor
};


ConnectingToServerFont = {
	standardFont,
	18,
	BlackColor
};

TableHeaderFont = {
  standardFont,
  30,
  BlackColor
};

ScoreInfoFont = {
  standardFont,
  18,
  BlackColor
};


ScoreFont = {
  standardFont,
  20,
  BlackColor
};

RankFont = {
  standardFont,
  26,
  BlackColor
};

LeftButtonGraphics = {
	"buttons/arrowleft_up",
	"buttons/arrowleft_down",
	"buttons/arrowleft_over"
};

kScoreRowSpace = 25;
kRowY = 75;
kNumberX = 37;
kNameX = 52;
kScoreX = 335;
kP1X = kNumberX + 1;

kNameW = 250
kNameH = 21

eLocalView=0;
eRequestingCategories=1;
eRequestingScores=2;
eSubmitting=3;
eGlobalView=4;
eError=5;

function UpdateButtons()
	local state = GetState();
	local localHS = IsEnabled(kHiscoreLocalOnly);
	local anonHS = IsEnabled(kHiscoreAnonymous);
	local fullHS = (not localHS) and (not anonHS);

	EnableWindow("view",false);
	EnableWindow("viewlocal",false);
	EnableWindow("submit",false);
	EnableWindow("resubmit",false);
	EnableWindow("categoryleft",false);
	EnableWindow("categoryright",false);
	
	if (state==eLocalView) then
		local eligibleScore;

		if fullHS then
			EnableWindow("moreinfo",true);
		else 
			EnableWindow("moreinfo",false);
		end
		
		if not localHS then
			EnableWindow("view",true);

			if (EligibleScoreAvailable()) then
				EnableWindow("submit",true);
			elseif (ScoreAvailable()) then
				EnableWindow("resubmit",true);			
			end
		end
	elseif (state == eGlobalView) then
		EnableWindow("viewlocal",true);
		EnableWindow("categoryleft",true);
		EnableWindow("categoryright",true);
	elseif (state == eError) then
		EnableWindow("viewlocal",true);
	end

end


MakeDialog
{
	-- Name the modal dialog
	name="hiscorescreen",
	Bitmap
	{
		image="backgrounds/fullscreendialog",

		HiscoreWindow
		{
			x=0,y=0,h=kMax,w=kMax,

			Text
			{
				font = HeaderFont,
				name = "header",
				x=0,y=20,w=kMax,h=kMax,
				flags = kHAlignCenter + kVAlignTop,
				label = "highscoreheader"
			};

			Text
			{
				font = GameModeFont,
				name = "gamemode",
				x=0,y=92,w=kMax,h=kMax,
				flags = kHAlignCenter + kVAlignTop,
				label = "#GAMEMODE"
			};

			Bitmap
			{
				x = 70,
				y = 150,
				image="hiscore/global-hs-bb_large",
				name="leftpanel",

				Text
				{
					font = TableHeaderFont,
					name = "local",
					x=0,y=10,w=kMax,h=kMax,
					flags = kHAlignCenter + kVAlignTop,
					label = "localhighscores"
				};


				Text
				{
					font = TableHeaderFont,
					name = "global",
					x=0,y=10,w=kMax,h=kMax,
					flags = kHAlignCenter + kVAlignTop,
					label = "globalhighscores"
				};

				Text
				{
					font = TableHeaderFont,
					name = "topplayers",
					x=0,y=10,w=kMax,h=kMax,
					flags = kHAlignCenter + kVAlignTop,
					label = "topplayers"
				};

				Text
				{
					font = ScoreFont,
					name = "1",
					x=0,y=kRowY,w=kNumberX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "1."
				};

				Bitmap
				{
					image = "hiscore/p1icon",
					name = "p1_1",
					x=kP1X,y=kRowY
				};

				Text
				{
					font = ScoreFont,
					name = "name1",
					x=kNameX,y=kRowY,w=kNameW,h=kNameH,
					flags = kHAlignLeft + kVAlignCenter,
					label = "#name"
				};

				Text
				{
					font = ScoreFont,
					name = "score1",
					x=0,y=kRowY,w=kScoreX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "12345"
				};

				Text
				{
					font = ScoreFont,
					name = "2",
					x=0,y=kRowY+(1*kScoreRowSpace),w=kNumberX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "2."
				};

				Bitmap
				{
					image = "hiscore/p1icon",
					name = "p1_2",
					x=kP1X,y=kRowY+(1*kScoreRowSpace)
				};


				Text
				{
					font = ScoreFont,
					name = "name2",
					x=kNameX,y=kRowY+(1*kScoreRowSpace),w=kNameW,h=kNameH,
					flags = kHAlignLeft + kVAlignCenter,
					label = "#name"
				};

				Text
				{
					font = ScoreFont,
					name = "score2",
					x=0,y=kRowY+(1*kScoreRowSpace),w=kScoreX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "12345"
				};

				Text
				{
					font = ScoreFont,
					name = "3",
					x=0,y=kRowY+(2*kScoreRowSpace),w=kNumberX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "3."
				};

				Bitmap
				{
					image = "hiscore/p1icon",
					name = "p1_3",
					x=kP1X,y=kRowY+(2*kScoreRowSpace)
				};

				Text
				{
					font = ScoreFont,
					name = "name3",
					x=kNameX,y=kRowY+(2*kScoreRowSpace),w=kNameW,h=kNameH,
					flags = kHAlignLeft + kVAlignCenter,
					label = "#name"
				};

				Text
				{
					font = ScoreFont,
					name = "score3",
					x=0,y=kRowY+(2*kScoreRowSpace),w=kScoreX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "12345"
				};

				Text
				{
					font = ScoreFont,
					name = "4",
					x=0,y=kRowY+(3*kScoreRowSpace),w=kNumberX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "4."
				};

				Bitmap
				{
					image = "hiscore/p1icon",
					name = "p1_4",
					x=kP1X,y=kRowY+(3*kScoreRowSpace)
				};

				Text
				{
					font = ScoreFont,
					name = "name4",
					x=kNameX,y=kRowY+(3*kScoreRowSpace),w=kNameW,h=kNameH,
					flags = kHAlignLeft + kVAlignCenter,
					label = "#name"
				};

				Text
				{
					font = ScoreFont,
					name = "score4",
					x=0,y=kRowY+(3*kScoreRowSpace),w=kScoreX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "12345"
				};

				Text
				{
					font = ScoreFont,
					name = "5",
					x=0,y=kRowY+(4*kScoreRowSpace),w=kNumberX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "5."
				};

				Bitmap
				{
					image = "hiscore/p1icon",
					name = "p1_5",
					x=kP1X,y=kRowY+(4*kScoreRowSpace)
				};

				Text
				{
					font = ScoreFont,
					name = "name5",
					x=kNameX,y=kRowY+(4*kScoreRowSpace),w=kNameW,h=kNameH,
					flags = kHAlignLeft + kVAlignCenter,
					label = "#name"
				};

				Text
				{
					font = ScoreFont,
					name = "score5",
					x=0,y=kRowY+(4*kScoreRowSpace),w=kScoreX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "12345"
				};

				Text
				{
					font = ScoreFont,
					name = "6",
					x=0,y=kRowY+(5*kScoreRowSpace),w=kNumberX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "6."
				};

				Bitmap
				{
					image = "hiscore/p1icon",
					name = "p1_6",
					x=kP1X,y=kRowY+(5*kScoreRowSpace)
				};

				Text
				{
					font = ScoreFont,
					name = "name6",
					x=kNameX,y=kRowY+(5*kScoreRowSpace),w=kNameW,h=kNameH,
					flags = kHAlignLeft + kVAlignCenter,
					label = "#name"
				};

				Text
				{
					font = ScoreFont,
					name = "score6",
					x=0,y=kRowY+(5*kScoreRowSpace),w=kScoreX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "12345"
				};

				Text
				{
					font = ScoreFont,
					name = "7",
					x=0,y=kRowY+(6*kScoreRowSpace),w=kNumberX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "7."
				};

				Bitmap
				{
					image = "hiscore/p1icon",
					name = "p1_7",
					x=kP1X,y=kRowY+(6*kScoreRowSpace)
				};

				Text
				{
					font = ScoreFont,
					name = "name7",
					x=kNameX,y=kRowY+(6*kScoreRowSpace),w=kNameW,h=kNameH,
					flags = kHAlignLeft + kVAlignCenter,
					label = "#name"
				};

				Text
				{
					font = ScoreFont,
					name = "score7",
					x=0,y=kRowY+(6*kScoreRowSpace),w=kScoreX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "12345"
				};

				Text
				{
					font = ScoreFont,
					name = "8",
					x=0,y=kRowY+(7*kScoreRowSpace),w=kNumberX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "8."
				};

				Bitmap
				{
					image = "hiscore/p1icon",
					name = "p1_8",
					x=kP1X,y=kRowY+(7*kScoreRowSpace)
				};

				Text
				{
					font = ScoreFont,
					name = "name8",
					x=kNameX,y=kRowY+(7*kScoreRowSpace),w=kNameW,h=kNameH,
					flags = kHAlignLeft + kVAlignCenter,
					label = "#name"
				};

				Text
				{
					font = ScoreFont,
					name = "score8",
					x=0,y=kRowY+(7*kScoreRowSpace),w=kScoreX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "12345"
				};

				Text
				{
					font = ScoreFont,
					name = "9",
					x=0,y=kRowY+(8*kScoreRowSpace),w=kNumberX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "9."
				};

				Bitmap
				{
					image = "hiscore/p1icon",
					name = "p1_9",
					x=kP1X,y=kRowY+(8*kScoreRowSpace)
				};

				Text
				{
					font = ScoreFont,
					name = "name9",
					x=kNameX,y=kRowY+(8*kScoreRowSpace),w=kNameW,h=kNameH,
					flags = kHAlignLeft + kVAlignCenter,
					label = "#name"
				};

				Text
				{
					font = ScoreFont,
					name = "score9",
					x=0,y=kRowY+(8*kScoreRowSpace),w=kScoreX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "12345"
				};

				Text
				{
					font = ScoreFont,
					name = "10",
					x=0,y=kRowY+(9*kScoreRowSpace),w=kNumberX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "10."
				};

				Bitmap
				{
					image = "hiscore/p1icon",
					name = "p1_10",
					x=kP1X,y=kRowY+(9*kScoreRowSpace)
				};

				Text
				{
					font = ScoreFont,
					name = "name10",
					x=kNameX,y=kRowY+(9*kScoreRowSpace),w=kNameW,h=kNameH,
					flags = kHAlignLeft + kVAlignCenter,
					label = "#name"
				};

				Text
				{
					font = ScoreFont,
					name = "score10",
					x=0,y=kRowY+(9*kScoreRowSpace),w=kScoreX,h=kMax,
					flags = kHAlignRight + kVAlignTop,
					label = "12345"
				};

			};

			Bitmap
			{
				x = 480,
				y = 150,
				image="hiscore/local-hs-bb",
				name="rightpanelsmall",

				Text
				{
					font = ScoreInfoFont,
					name = "yourrankglobalinfo",
					x=40,y=0,w=kMax-40,h=kMax,
					flags = kHAlignCenter + kVAlignCenter,
					label = "globalhighscoreinfo"
				};

				Text
				{
					font = RankFont,
					name = "yourrank",
					x=10,y=15,w=kMax-10,h=kMax,
					flags = kHAlignCenter + kVAlignTop,
					label = "yourrank"
				};

				Text
				{
					font = RankFont,
					name = "congratulations",
					x=10,y=50,w=kMax-10,h=kMax,
					flags = kHAlignCenter + kVAlignTop,
					label = "congratshighscore"
				};

				Text
				{
					font = RankFont,
					name = "dnq",
					x=10,y=50,w=kMax-10,h=kMax,
					flags = kHAlignCenter + kVAlignTop,
					label = "scorednq"
				};
			},


			Bitmap
			{
				x = 480,
				y = 150,
				image="hiscore/local-hs-bb",
				name="rightpanel",

				Text
				{
					font = TableHeaderFont,
					name = "globalinfoheader",
					x=0,y=15,w=kMax,h=kMax,
					flags = kHAlignCenter + kVAlignTop,
					label = "globalhighscores"
				};

				Text
				{
					font = ScoreInfoFont,
					name = "info",
					x=40,y=55,w=kMax-40,h=kMax,
					flags = kHAlignCenter + kVAlignTop,
					label = "globalhighscoreinfo"
				};


				Text
				{
					font = EligibleFont,
					name = "eligible",
					x=60,y=180,w=kMax-60,h=kMax,
					flags = kHAlignCenter + kVAlignTop,
					label = "eligible"
				};

				SetStyle(ButtonStyle),
				Button
				{
					x=kCenter,
					y=kMax-100,
					name="moreinfo",
					label="moreinfo",
					command =
						function()
							DoModal("scripts/hiscoreinfo.lua");
						end

				};

				Button
				{
					x=kCenter,
					y=kMax-60,
					name="submit",
					label="submit",
					command =
						function()
							vars = loadstring(GetLuaServerSubmitSetupVars(false));
							vars();
							val = DoModal("scripts/serversubmit.lua");
							if (val == 'qualified')	 then
								SubmissionDone(true);
							elseif (val == 'success') then
								SubmissionDone(false);
							else
								-- nothing
							end
						end
				};

				Button
				{
					x=kCenter,
					y=kMax-60,
					name="resubmit",
					label="resubmit",
					command =
						function()
							vars = loadstring(GetLuaServerSubmitSetupVars(false));
							vars();
							val = DoModal("scripts/serversubmit.lua");
							if (val == 'qualified')	 then
								SubmissionDone(true);
							elseif (val == 'success') then
								SubmissionDone(false);
							else
								-- nothing
							end
						end
				};
			};

			Text
			{
				font = ConnectingToServerFont,
				name = "server",
				x=485,y=362,w=258,h=130,
				flags = kHAlignCenter + kVAlignCenter,
				label = "connectingtoserver"
			};

			Text
			{
				font = ConnectingToServerFont,
				name = "error",
				x=485,y=362,w=258,h=130,
				flags = kHAlignCenter + kVAlignCenter,
			};



			Text
			{
				font = ScoreFont,
				name = "category",
				x=163,y=194,w=208,h=40,
				flags = kHAlignCenter + kVAlignTop,
			};

			SetStyle(ButtonStyle),

			Button
			{
				graphics = LeftButtonGraphics,
				name="gamemodeleft",
				x=160,
				y=90,
			};

			Button
			{
				graphics = LeftButtonGraphics,
				name="gamemoderight",
				x=570,
				y=90,
				hflip=true,
			};

			Button
			{
				graphics = LeftButtonGraphics,
				name="categoryleft",
				x=160,
				y=188,
				scale = 0.6
			};

			Button
			{
				graphics = LeftButtonGraphics,
				name="categoryright",
				x=330,
				y=188,
				scale = 0.6,
				hflip=true,
			};

			Button
			{
				graphics = LeftButtonGraphics,
				rotate=true,
				name="scrollup",
				x=420,
				y=225,
				scale = 0.4
			};

			Button
			{
				graphics = LeftButtonGraphics,
				rotate=true,
				hflip=true,
				name="scrolldown",
				x=420,
				y=440,
				scale = 0.4
			};

			Button
			{
				x=20,
				y=kMax-50,
				name="back",
				type = kPush,
				flags = kHAlignCenter + kVAlignCenter,
				label="back",
				command =
					function()
						PopModal("hiscorescreen"); -- take off high score menu, restart game loop
					end
			};

            SetStyle(LongButtonStyle),
			Button
			{
				align=kHAlignRight, -- Position this button at its right edge
				x=780,				-- With above align, position the right edge
				y=kMax-56,
				name="viewlocal",
				label="viewlocal",
				scale=1.1
			};
			Button
			{
				align=kHAlignRight, -- Position this button at its right edge
				x=780,				-- With above align, position the right edge
				y=kMax-56,
				name="view",
				label="viewglobal",
				scale=1.1
			};


		}
	}
}