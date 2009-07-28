-- FirstStage generated output file
require('style.lua');

SetDefaultStyle( DefaultStyle );

local ok="ok" ;	
local okname="OK";
local name="okbox";
local okcommand = gDialogTable.okcommand;
	
if (gDialogTable.oklabel) then
	ok = gDialogTable.oklabel ;
end

if (gDialogTable.name) then
	name = gDialogTable.name;
end

if (gDialogTable.okname) then
	okname = gDialogTable.okname;
end

d = MediumDialog

MakeDialog
{
	Bitmap
	{
		image="menu/main/greybox",
		x=0,
		y=0,
		scale=4,
		name = name,
		Bitmap
		{
			image = "backgrounds/dialog",
			x = kCenter,
			y = kCenter,

			SetStyle( ButtonStyle ),
			Button
			{
				close = true,
				flags = 5,
				label=ok,
				name = okname,
				x = kCenter,
				y = kMax-50,
				command=okcommand
			},
			SetStyle( DialogTitleText ),
			Text{ label=gDialogTable.title },
			SetStyle( DialogBodyText ),
			Text{ label=gDialogTable.body },
		},
	};

} -- MakeDialog

gDialogTable = {}
