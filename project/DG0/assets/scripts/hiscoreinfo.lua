require( "scripts/style.lua" )

TableInfoFont = {
  standardFont,
  18,
  BlackColor
};

MakeDialog
{
	Bitmap
	{ 
		image="backgrounds/largedialog",	
		x=kCenter,
		y=kCenter,

		Text
		{
			font = TableInfoFont,
			name = "info",
			x=50,y=90,w=307 ,h=kMax-50,
			flags = kHAlignLeft+ kVAlignTop,
			label = "highscorecompleteinfo"
		};
			
		Button{
			x=150,
			y=450,
			font = StandardButtonFont,
			graphics = StandardButtonGraphics,
			name="hiscoreinfook",
			label="ok",
			type = kPush,
			close=true
		};
		
		Button
		{
			x=0,
			y=0,
			w=0,
			h=0,
			graphics={},
			name="privacypolicy",
			command = 
				function()
					LaunchPrivacyPolicy();
				end
		};
	}
}



