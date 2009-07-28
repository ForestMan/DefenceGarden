require( "scripts/style.lua" );


local yes="yes" ;
local yesname="yes" ;
local no="no" ;	
local noname="no" ;	
local name="yesnobox";
local yescmd=gDialogTable.yes;
local nocmd=gDialogTable.no;
	
if (gDialogTable.yeslabel) then
	yes = gDialogTable.yeslabel ;
end
if (gDialogTable.nolabel) then
	no = gDialogTable.nolabel ;
end
if (gDialogTable.yesname) then
	yesname = gDialogTable.yesname;
end
if (gDialogTable.noname) then
	noname = gDialogTable.noname;
end

if (gDialogTable.name) then
	name = gDialogTable.name;
end

MakeDialog
{
	name=name, -- Remember, this is like saying table['name']=name, so it works.
    Bitmap
    {
		image="backgrounds/dialog",
		x=kCenter,
		y=kCenter,

		SetStyle(DialogTitleText),
        Text
		{
			name = "yesnotitle",
			label= gDialogTable.title,
		};
		
		SetStyle(DialogBodyText),
		Text
		{
			name = "yesnobody",
			label= gDialogTable.body,
		};

        SetStyle(ButtonStyle),
        Button
        {
			command=
				function()
					if (yescmd) then 
						yescmd() ; 
					end
				end;
            close = true,
            flags = 5,
            label = yes,
            name = yesname,
            x = kCenter-80,
            y = kMax-50,
        },
        Button
        {
			command=
				function() 
					if (nocmd) then
						nocmd() ; 
					end
                end;
            close = true,
            flags = 5,
            label = no,
            name = noname,
            x = kCenter+80,
            y = kMax-50,
        },
        
    },

} -- MakeDialog
