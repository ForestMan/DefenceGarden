require( "scripts/style.lua" );

gCurScreen = 0;

function SetScreen( screen )

	if screen == 1 then
		EnableWindow("helptext1",true)
		EnableWindow("helptext2",false)
	elseif screen ==2 then
		EnableWindow("helptext1",false)
		EnableWindow("helptext2",true)
	end

	gCurScreen = screen ;
end

TextStyle=
{
	parent=DialogBodyText,
	x=0,y=46,w=kMax,h=kMax-80
};

MakeDialog
{
	-- Name the modal dialog
	name="help",
	Bitmap
	{
		image="backgrounds/largedialog",
		x=kCenter,
		y=kCenter,

		Window
		{
			name="helptext1",
			w=kMax,h=kMax,

			SetStyle(TextStyle),
			Text
			{
				label="help1",
			},

			Text
			{
				flags=kVAlignBottom+kHAlignCenter,
				label="#"..GetString("version",GetConfig(kGameVersion)),
			},
			SetStyle(ButtonStyle),
			Button
			{
				label="back",
				name="back",
				cancel=true,
				align=kHAlignLeft+kVAlignBottom,
				x=20,y=kMax-20,
				command=function() CloseWindow(); end;
			},
			Button
			{
				label="next",
				name="next",
				default=true,
				align=kHAlignRight+kVAlignBottom,
				x=kMax-20,y=kMax-20,
				command=function() SetScreen( gCurScreen+1 ); end;
			}
		},
		Window
		{
			name="helptext2",
			w=kMax,h=kMax,

			SetStyle(TextStyle),
			Text
			{
				label="help2",
			},
			SetStyle(ButtonStyle),
			Button
			{
				label="prev",
				name="prev",
				align=kHAlignLeft+kVAlignBottom,
				x=20,y=kMax-20,
				command=function() SetScreen( gCurScreen-1 ); end;
			},
			Button
			{
				label="done",
				name="done",
				cancel=true,
				default=true,
				align=kHAlignRight+kVAlignBottom,
				x=kMax-20,y=kMax-20,
				command=function() CloseWindow(); end;
			}
		}
	}
}

SetScreen(1);
