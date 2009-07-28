require( "scripts/style.lua" );



MakeDialog
{
    Bitmap
    {
        name="yesnobackground",
		image="backgrounds/dialog",
		x=kCenter,
		y=kCenter,

        Button
        {
			font = StandardButtonFont,
			graphics = StandardButtonGraphics,
            close = true,
			cancel = true,
            flags = 5,
            label = "ok",
            name = "ok",
            x = kCenter,
            y = 250,
        },

        Text
		{
			font = DialogTitleFont,
			name = "oktitle",
			x=12, y=8,
			w=kMax,h=40,
			flags = kVAlignCenter + kHAlignLeft,
			label= gDialogTable.title,
		};

		Text
		{
			font = DialogBodyFont,
			name = "okbody",
			x=12,y=46,
			w=kMax-12,h=kMax-70,
			flags = kVAlignCenter + kHAlignCenter,
			label= gDialogTable.body,
		};
    },

} -- MakeDialog
