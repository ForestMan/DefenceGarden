require( "scripts/style.lua" );



TitleFont = {
  standardFont,
  70,
  BlackColor
};

MedalFont = {
  standardFont,
  20,
  BlackColor
};


function UpdateSubmit()
	local localHS = IsEnabled(kHiscoreLocalOnly);
	local anonHS = IsEnabled(kHiscoreAnonymous);
	local fullHS = (not localHS) and (not anonHS);
	EnableWindow('submit', fullHS and NumMedalsToSubmit() > 0);
	EnableWindow('resubmit', fullHS and NumMedalsToSubmit() == 0 and NumEarnedMedals() > 0);
end


function DoMedal(medalName, gameMode, posX, descrip)
	numMedals = NumEarnedMedals();
	medalimage = "hiscore/medal_unearned";
	for i=0,numMedals-1 do
		if (GetMedalName(i) == medalName and
			(gameMode == nil or gameMode == GetMedalGameMode(i))) then
			medalimage="hiscore/medal_earned";
			break;
		end
	end
	return Group
	{
		Bitmap
		{
			image=medalimage,
			x = posX,
			y = 200,
			
			RolloverWindow
			{
				x=0,y=0,w=kMax,h=kMax,
				activate = 'medaldescrip',
				text = descrip;
			};
		}
	};
	
end
function MakeMedals()
	local windows = {};
	table.insert(windows, DoMedal(GetPFMedalName(0), nil, 120, GetString('medaldescrip1'))); 
	table.insert(windows, DoMedal(GetPFMedalName(1), GetPFGameModeName(0), 320, GetString('medaldescrip2')));
	table.insert(windows, DoMedal(GetPFMedalName(1), GetPFGameModeName(1), 520, GetString('medaldescrip3')));
	return Group(windows);
end

local function DoSubmit()
	vars = loadstring(GetLuaServerSubmitSetupVars(true));
	vars();
	val = DoModal("scripts/serversubmit.lua");
	if (val == 'success') then
		DisplayDialog {
			"scripts/ok.lua",
			body ="medalsuccess",
			title = ""
		};
	end
	UpdateSubmit();
end

MakeDialog
{
	-- Name the modal dialog
	name="medalswindow",
	Bitmap
	{
		image="backgrounds/fullscreendialog",

		Text
		{
			font = TitleFont,
			name = "title",
			x=kCenter,y=60,w=kMax ,h=75,
			flags = kHAlignCenter + kVAlignTop,
			label = "medals",
		};

		SetStyle(ButtonStyle),
		MakeMedals(),
		
		Text
		{
			font = MedalFont,
			x = 0,
			y = 400,
			h = 100,
			w = kMax,
			flags = kHAlignCenter + kVAlignTop,
			name = 'medaldescrip',
			
			
		};
		
		Button
		{
			x=kCenter,
			y=kMax-50,
			name="submit",
			type = kPush,
			flags = kHAlignCenter + kVAlignCenter,
			label="submit",
			command =
				function()
					DoSubmit();
				end
		};
		
		Button
		{
			x=kCenter,
			y=kMax-50,
			name="resubmit",
			type = kPush,
			flags = kHAlignCenter + kVAlignCenter,
			label="resubmit",
			command =
				function()
					DoSubmit();
				end
		};

		Button
		{
			x=20,
			y=kMax-50,
			name="back",
			type = kPush,
			flags = kHAlignCenter + kVAlignCenter,
			label="back",
			command =
				function()
					PopModal("medalswindow"); 
				end
		};
	},
}

UpdateSubmit();