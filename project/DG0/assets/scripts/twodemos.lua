require( "scripts/style.lua" );

-- The "Twodemos" game handles its messages in Lua, unlike the "TGame" game
-- which handles them in C++.
function SendGameMessage( msg )
	if msg=="quit" then
		QuitToMainMenu();
	end
end

MakeDialog
{
	Bitmap
	{
		name="backgroundimage",
		image="backgrounds/gamebackground",

		ChessPiece
		{
			x=kCenter-200,
			y=kCenter-100,
			w=400,
			h=400,
		},
		Text
		{
			x=410,y=20,
			w=400-20,h=180,
			flags=kHAlignCenter+kVAlignCenter,
			label="twodemosinfo",
		},
		Swarm
		{
			name='swarm',
			x=kCenter+200,
			y=kCenter+100,
			w=400, h=400,

			-- For this simple example, we can define some parameters right here

			-- For a game with levels, you might want to use globals or an XML
			-- file to define specifics, depending on the nature of the data.
			edgebounce=0.5,		-- How bouncy is the edge
			damping=0.95,		-- How much damping they experience
			butterflypull=4,	-- How much they're pulled to your mouse
			butterflyrepel=80	-- How much they push each other apart
		},
		Button
		{
			x=20,
			y=-50,
			font = StandardButtonFont,
			graphics = StandardButtonGraphics,
			name="pause",
			label="pause",
			type = kPush,
			command=
				function()
					DoModal("scripts/pause.lua");
				end
		},
		Button
		{
			x=20,
			y=-100,
			font = StandardButtonFont,
			graphics = StandardButtonGraphics,
			name="playmovie",
			label="playmovie",
			type = kPush,
			command=
				function()
						DisplaySplash("splash/playfirst_animated_logo.swf", "splash/playfirst_logo",4000);
				end
		},
	},
}

-- The swarm window wants to get any key presses, so let's give it the focus
SetFocus("swarm");