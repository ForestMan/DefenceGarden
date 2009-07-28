require( "scripts/style.lua" )

HeaderFont = {
  standardFont,
  35,
  BlackColor
};

TableInfoFont = {
  standardFont,
  30,
 BlackColor
};

ErrorFont = {
  standardFont,
  24,
 BlackColor
};

AccountFont = {
  standardFont,
  16,
  BlackColor
};

ScoreFont = {
  standardFont,
  16,
  BlackColor
};

HiscoreSubmitLegal = {
  standardFont,
  14,
  BlackColor
};

kPlayerNameY = 150;
kScoreY = 200;
kYourPFAccountY = 235;
kAccountNameY = 275;
kEditOffsetY = -7 ;
kPasswordY = 310;
kRememberY = 350;

kPrivacyLabel = 'privacy';
if (IsEnabled(kHiscoreAnonymous)) then
	kPrivacyLabel = 'privacy_info';
end


function SwitchModes( submit )

	local localHS = IsEnabled(kHiscoreLocalOnly);
	local anonHS = IsEnabled(kHiscoreAnonymous);
	local fullHS = (not localHS) and (not anonHS);

	if submit then
		DebugOut( "SwitchModes true "..tostring(localHS).." "..tostring(anonHS));
	else
		DebugOut( "SwitchModes false "..tostring(localHS).." "..tostring(anonHS));
	end

	EnableWindow("header",not submit);
	EnableWindow("headershadow",not submit);
	EnableWindow("playertext", not submit and gMedalsMode == false);
	EnableWindow("yourscore", not submit and gMedalsMode == false);
	EnableWindow("eligiblescore", not submit and gMedalsMode == false);
	EnableWindow("yourpfaccount", not submit and fullHS);
	EnableWindow("nametext", not submit and fullHS);
	EnableWindow("passtext", not submit and fullHS);

	EnableWindow("nameeditbox", not submit  and gMedalsMode == false);
	EnableWindow("accounteditbox", not submit and fullHS);
	EnableWindow("passeditbox", not submit and fullHS);
	EnableWindow("remember", not submit and fullHS);

	EnableWindow("submittoserver", not submit);
	EnableWindow("submitcancel", not submit);
	EnableWindow("submitconnect", submit);

end

ServerSubmitEditStyle=
{
	parent=DefaultStyle,
	font = AccountFont,
	x=0,y=kCenter,
	w=kMax,h=32,
	flags=kVAlignCenter
};

MakeDialog
{
	name="hiscoresubmitscreen",
	Bitmap
	{
		image="backgrounds/largedialog",
		x=kCenter,
		y=kCenter,
		SubmitWindow
		{
			x=0,y=0,h=kMax,w=kMax,
			SetStyle(DefaultStyle);
			Text
			{
				font = HeaderFont,
				name = "header",
				x=0,y=40,w=kMax,h=kMax,
				flags = kHAlignCenter + kVAlignTop,
				label = "submitglobal"
			};

			Text
			{
				font = AccountFont,
				name = "playertext",
				x=0,y=kPlayerNameY,w=145,h=kMax,
				flags = kHAlignRight + kVAlignTop,
				label = "playerlabel"
			};

			Text
			{
				font = AccountFont,
				name = "yourscore",
				x=0,y=kScoreY,w=142,h=kMax,
				flags = kHAlignRight + kVAlignTop,
				label = "scorelabel"
			};

			Text
			{
				font = AccountFont,
				name = "eligiblescore",
				x=205,y=kScoreY,w=100,h=kMax,
				flags = kHAlignLeft + kVAlignTop,
				label = gEligibleScore
			};

			Text
			{
				font = ScoreFont,
				name = "yourpfaccount",
				x=145,y=kYourPFAccountY,w=150,h=kMax,
				flags = kHAlignLeft + kVAlignTop,
				label = "yourpfaccount"
			};

			Text
			{
				font = AccountFont,
				name = "nametext",
				x=0,y=kAccountNameY,w=145,h=kMax,
				flags = kHAlignRight + kVAlignTop,
				label = "namelabel"
			};

			Text
			{
				font = AccountFont,
				name = "passtext",
				x=0,y=kPasswordY,w=145,h=kMax,
				flags = kHAlignRight + kVAlignTop,
				label = "passwordlabel"
			};

			Text
			{
				font = TableInfoFont,
				name = "submitconnect",
				x=0,y=0,w=kMax,h=kMax,
				flags = kHAlignCenter + kVAlignCenter,
				label = "connectingtoserver"
			};

			Text
			{
				font = ErrorFont,
				name = "submiterror",
				x=0,y=0,w=kMax,h=kMax,
				flags = kHAlignCenter + kVAlignCenter,
				
				-- "submiterror" is only enabled when there's an error,
				-- in which case SetText() is used to set an error specific message,
				-- so no label is provided here.
			};

			Bitmap
			{
				name="nameeditbox",
				image="backgrounds/textfield.png",
				x=155,
				y=kPlayerNameY+kEditOffsetY,

				SetStyle(ServerSubmitEditStyle),
				TextEdit
				{
					name = "nameedit",
					utf8=false, -- the hiscore servers only take low-ASCII names
					label = gNameEdit,
					length=20,
					ignore = kIllegalNameChars

				};
			};

			Bitmap
			{
				name="accounteditbox",
				image="backgrounds/textfield.png",
				x=155,
				y=kAccountNameY+kEditOffsetY,

				SetStyle(ServerSubmitEditStyle),
				TextEdit
				{
					name = "accountedit",
					utf8=false, -- the hiscore servers only take low-ASCII names
					label= gAccountEdit,
					length=26,
					ignore = kIllegalNameChars

				};
			};

			Bitmap
			{
				name="passeditbox",
				image="backgrounds/textfield.png",
				x=155,
				y=kPasswordY+kEditOffsetY,

				SetStyle(ServerSubmitEditStyle),
				TextEdit
				{
					name = "passedit",
					label= gPassEdit,
					utf8=true,
					password = true,
					length=26
				};
			};

			SetStyle( CheckboxButtonStyle ),
			Button
			{
				x=75, y=kRememberY,

				name="remember",
				type = kToggle,
				w=260,
				scale=0.5,
				rollover = "",
				drop = false,
				sound = "audio/sfx/checkbox.ogg",
				Text
				{
					font = StandardButtonFont,
					name = "rememberlabel",
					x=25,
					y=10,
					w=250,
					h=25,
					flags = kHAlignLeft + kVAlignTop,
					label="rememberpfaccount",
				};
			};

			SetStyle( ButtonStyle ),
			Button
			{
				x=50, y=kMax-50,

				name="submittoserver",
				label="submit",
				hotkey="alt-s", -- override the hot key here so it doesn't steal our TTextEdit keys
				type = kPush,
				default= true, -- this should be the default button (on enter)

				command =
					function()
						name = GetLabel("nameedit");
						account = GetLabel("accountedit");
						pass = GetLabel("passedit");
						remember = GetButtonToggleState("remember");
						SubmitToServer(name,account,pass,remember,gMedalsMode);
					end
			},

			Button
			{
				x=237, y=kMax-50,

				name="submitcancel",
				label="cancel",
				cancel = true,
				type = kPush,
				command =
					function()
						PopModal("hiscoresubmitscreen"); -- take off high score menu, restart game loop
					end
			};

			Button
			{
				x=150, y=kMax-50,

				name="submiterrorok",
				label="ok",
				type = kPush,
				command=
					function()
						EnableWindow("submiterrorok",false);
						EnableWindow("submiterror",false);
						SwitchModes(false);
					end
			};

			Button
			{
				x=0,
				y=0,
				w=0,
				h=0,
				graphics={},
				name="privacybutton",
				command =
					function()
						if (IsEnabled(kHiscoreAnonymous)) then
							DisplayDialog {
								"scripts/ok.lua",
								body ="privacy_anon",
								title = ""
							};
						else
							LaunchPrivacyPolicy();
						end
					end
			};

			Text
			{
				x = 20, w = kMax-20,
				y = 400, h = 39,
				font = HiscoreSubmitLegal;
				flags = kHAlignLeft + kVAlignTop,
				label = kPrivacyLabel
			};
		}
	}
}

EnableWindow("submitconnect", false);
EnableWindow("submiterror",false);
EnableWindow("submiterrorok", false);
SetButtonToggleState('remember',gRemember);
if (gMedalsMode) then
	SetFocus("accountedit");
else
	SetFocus("nameedit");
end
SwitchModes(false);