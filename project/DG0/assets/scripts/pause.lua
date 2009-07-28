require( "scripts/style.lua" );

kButtonSpacing = 60;	-- Put our buttons 60 pixels apart
kButtonY = kCenter-60;	-- Three buttons means center 60 pixels above the center

-- When this dialog is popped up, we're entering pause state, so
-- let's just notify TGameState now.
SetState( kPause, true );

MakeDialog
{
	-- Name the modal dialog
	name="pausemenu",
	Bitmap
	{
		image="backgrounds/dialog",
		x=kCenter,
		y=kCenter,

		SetStyle(ButtonStyle),
		Button
		{
			x=kCenter,
			y=kButtonY,
			name="resumebutton",
			label="resume",
			close = true,	-- Just close this window
			cancel=true,
			command=
				function()
	                SetState( kPause, false );
				end;

		},

		Button
		{
			x=kCenter,
			y=kButtonY + (kButtonSpacing * 1),
			name="optionsbutton",
			label="optionsbutton",
			command=
				function()
					-- Pop up an options screen
					DoModal("scripts/options.lua");
				end
		},

		Button
		{
			x=kCenter,
			y=kButtonY + (kButtonSpacing * 2),
			label="quit",
			command=
				function()
					PopModal('pausemenu');
					DoModal('scripts/gameover.lua');
					-- Our game processes this message--it's not
					-- handled by Playground
                    SendGameMessage( "quit" );
	                SetState( kPause, false );
				end
		},
	},
}