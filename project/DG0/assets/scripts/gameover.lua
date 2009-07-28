require( "scripts/style.lua" );


HeaderFont = {
  standardFont,
  30,
  BlackColor
};

BodyFont = {
  standardFont,
  16,
  BlackColor
};




MakeDialog
{
	-- Name the modal dialog
	name="pausemenu",
	Bitmap
	{
		image="backgrounds/dialog",
		x=kCenter,
		y=kCenter,

		Text
		{
			font = HeaderFont,
			name = "gameover",
			x=0,y=20,w=kMax,h=30,
			flags = kHAlignCenter + kVAlignTop,
			label = "gameover"
		};

		Text
		{
			font = BodyFont,
			name = "earnscore",
			x=0,y=50,w=kMax,h=60,
			flags = kHAlignCenter + kVAlignTop,
			label = "earnscore"
		};

		Text
		{
			font = BodyFont,
			name = "earnedscore",
			x=100,y=133,w=kMax,h=60,
			flags = kHAlignLeft + kVAlignTop,
			label = "earnedscore"
		};

		Bitmap
		{
			image="backgrounds/textfield.png",
			x=150,
			y=125,

			TextEdit
			{
				font = BodyFont,
				name = "scoreedit",
				label = '1000',
				x=3,y=5,w=kMax-3,h=kMax,
				length=12

			};
		};

		SetStyle(CheckboxButtonStyle),
		AppendStyle
		{
			tx=5, -- move the text over a bit
			w=150,-- set our common width
		},
		Button{
			name="medal1",
			x=50,
			y=160,
			tx=5, -- move the text over a bit
			label = "medal1",
		};

		Button{
			name="medal2",
			x=235,
			y=160,
			tx=5, -- move the text over a bit
			label = "medal2",
		};


		SetStyle(ButtonStyle),
		AppendStyle
		{
			scale=1.2
		},

		Button
		{
			x=kCenter,
			y=kMax - 90,
			label="logscore",
			close=true,
			command =
				function()
					LogHighScore(tonumber(GetLabel('scoreedit')), GetButtonToggleState('medal1'), GetButtonToggleState('medal2'));
				end
		},

		Button
		{
			x=kCenter,
			y=kMax - 50,
			label="donotlog",
			close=true
		},
	},
}