require( "scripts/style.lua" );


MyFont = {
  standardFont,
  30,
  BlackColor
};

NameFont = {
  standardFont,
  15,
  BlackColor
};

local playername = gEnterNameValue;
local deleteselected = false;

if not playername or playername=="" then
	deleteselected = true;
	playername = GetString("enternameprompt") ;
end


MakeDialog
{
	Bitmap
	{
		image="backgrounds/dialog",
		x=kCenter,
		y=kCenter,

		Text
		{
			font = MyFont,
			name = "entername",
			x=0,y=20,w=kMax,h=30,
			flags = kHAlignCenter + kVAlignTop,
			label = "entername"
		};

		Bitmap
		{
			image="backgrounds/textfield.png",
			x=kCenter,
			y=kCenter,
			scale = 1.1,

			TextEdit
			{
				font = NameFont,
				name = "nameedit",
				label = playername,
				flags = kVAlignCenter,
				enablewindow = "enterok",
				utf8=true,
				clearinitial = deleteselected,
				x=4,y=0,w=kMax-4,h=kMax,
				length=20,
				ignore = kIllegalNameChars

			};
		};

		SetStyle( ButtonStyle ),

		Button
		{
			x=50, y=-60,

			name="enterok",
			label="ok",
			default=true,	-- Make this the default button
			command =
				function()
					local newname = GetLabel("nameedit");
					-- crop leading and trailing spaces
					newname = string.gsub( newname, "^%s+", "" );
					newname = string.gsub( newname, "%s+$", "" );
					if (string.len( newname )==0 ) then
						DisplayDialog {
							"scripts/ok.lua",
							body ="pleaseenteraname",
							title = ""
						};
						SetFocus( "nameedit" )
					elseif (newname ~= playername and IsNameInUse(newname)) then
						DisplayDialog {
							"scripts/ok.lua",
							body ="nameinuse",
							title = ""
						};
						SetFocus( "nameedit" )
					else
						CloseWindow(newname);
					end
				end
		},

		Button
		{
			x=250, y=-60,

			name="entercancel",
			label="cancel",
			cancel=true,
			command = function() CloseWindow(nil); end;

		};
	},

}

SetFocus( "nameedit" );

if (deleteselected) then
	EnableWindow("enterok", false);
end


