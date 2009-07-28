require( "scripts/style.lua" );

SliderFont = {
  standardFont,
  16,
  BlackColor
};


MakeDialog
{
	-- Name the modal dialog
	name="optionsmenu",
	Bitmap
	{
		image="backgrounds/dialog",
		x = kCenter,
		y = kCenter,

		OptionsWindow
		{
			x=0,y=0,w=kMax,h=kMax,

			Text
			{
				font = SliderFont,
				name = "sfx",
				x=20,y=40,w=50,h=50,
				flags = kHAlignCenter + kVAlignCenter,
				label = "sfxlevel"
			};

			Text
			{
				font = SliderFont,
				name = "music",
				x=20,y=110,w=50,h=50,
				flags = kHAlignCenter + kVAlignCenter,
				label = "musiclevel"
			};

			SetStyle(CheckboxButtonStyle),
			Button{
				name="fullscreen",
				label="fullscreen",
				x=30,
				y=175,
				w=150,
				command=
				function()
					if (ToggleFullScreen() == false) then
						DisplayDialog {
							"scripts/ok.lua",
							body ="screenswitchfailed",
							title = "playground"
						};
					end
				end

			};

			Button{
				name="mutebox",
				x=205,
				y=175,
				w=175,
				label = "mutesound",
				command =
					function()
						ToggleSoundMute();
					end
			};

			SetStyle(ButtonStyle),
			Button
			{
				x = kCenter-80,
				y = kMax-50,
				name="back",
				label="back",
				cancel=true,
				default=true,
				type = kPush,
				command =
					function()
						SaveVolumes();
						PopModal("optionsmenu");
					end
			},

			Button
			{
				x = kCenter+80,
				y = kMax-50,
				name="credits",
				label="creditsbutton",
				type = kPush,
				command =
					function()
						DoModal("scripts/credits.lua");
					end
			},

			SetStyle(SliderStyle),
			Slider
			{
				name="sfxlevelslider",
				x = 90,
				y = 50,
				w = 270,
			};

			Slider
			{
				name="musiclevelslider",
				x = 90,
				y = 120,
				w = 270,
			};
		},
	},
}