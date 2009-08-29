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
		image="backgrounds/game_back",

		DGGameWindow
		{
			x=0,
			y=0,
			w=800,
			h=600,
			rows=gRows,
			columns=gColumns,
		},
		SetStyle(LongButtonStyle),
		Button
		{
			x=511,
			y=559,
			name="pause",
			label="pause",
			cancel=true,
			command=
				function()
					DoModal("scripts/pause.lua");
				end
		},
	},
}
