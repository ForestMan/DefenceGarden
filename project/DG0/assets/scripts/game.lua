require( "scripts/style.lua" );

-- gRows and gColumns are set up in mainmenu.lua

-- Our mini-game set-up
MakeDialog
{
	-- Name the modal dialog
	name="thegame",
	Bitmap
	{
		name="backgroundimage",
		image="backgrounds/gamebackground",

		GameWindow
		{
			x=0,
			y=0,
			w=800,
			h=600,

			rows=gRows,
			columns=gColumns,
		},
		Text
		{
			x=20,y=20,
			w=kMax-20,h=kMax,
			flags=kHAlignCenter+kVAlignCenter,
			label="squaresinfo",
		},

		SetStyle( ButtonStyle ),
		Button
		{
			x=650,
			y=550,
			name="pause",
			label="pause",
			cancel=true,
			command=
				function()
					DoModal("scripts/pause.lua");
				end
		},
		Button
		{
			x=650,
			y=500,
			name="playmovie",
			label="playmovie",
			command=
				function()
						DisplaySplash("splash/playfirst_animated_logo.swf", "splash/playfirst_logo",4000);
				end
		},
	},
}