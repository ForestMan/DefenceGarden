require( "scripts/style.lua" );



TitleFont = {
  standardFont,
  70,
  BlackColor
};


MakeDialog
{
	-- Name the modal dialog
	name="creditswindow",
	Bitmap
	{
		image="backgrounds/fullscreendialog",

		Text
		{
			font = TitleFont,
			name = "title",
			x=kCenter,y=60,w=kMax ,h=75,
			flags = kHAlignCenter + kVAlignTop,
			label = "credits"
		};

		CreditsWindow
		{
			x = 0, y = 150,
			w = 800, h = 386,
			font = standardFont,
			fontsize = 16,
			fontcolor = BlackColor,
			headercolor = DarkBlueColor,
			columngap = 50,
			columnwidth = 325,
			time = 20000, 		-- Time to play credits in milliseconds
			intropause = 3000, 	-- Time to pause credits at start, in ms
			file = "credits/credits.txt",
		},

		Text
		{
			flags=kHAlignCenter + kVAlignTop,
			x=0,y=-22,
			w=kMax;h=39;
			label="#"..GetString("version",GetConfig(kGameVersion)) ;
		};

		SetStyle(ButtonStyle),
		Button
		{
			x=kCenter,
			y=kMax-60,
			name="back",
			default=true,
			cancel=true,
			label="back",
			command =
				function()
					PopModal("creditswindow");
				end
		};
	},
}