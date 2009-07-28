require( "scripts/style.lua" );

function ShowSubmitOptions( show )
	EnableWindow( "delete", show )
	EnableWindow( "chooseok", show )
	EnableWindow( "editname", show )
end

function ShowCreateOptions( show )
	EnableWindow( "newplayer", show )
end

function UpdatePlayerChoices( curUser, numUsers, userTable )

	if (numUsers >= kMaxUsers) then
		ShowCreateOptions(false)
	else
		ShowCreateOptions(true)
	end

	if (numUsers==0) then
		ShowSubmitOptions( false )
	else
		ShowSubmitOptions( true )
	end

	for i = 0,numUsers-1 do
		SetLabel( "button"..i, userTable[ i+1 ] );
		EnableWindow( "button"..i, true ) ;
	end

	for i = numUsers, kMaxUsers do
		EnableWindow( "button"..i, false ) ;
	end

	if numUsers>0 then
		SetButtonToggleState( "button"..curUser, true );
	end

--	DebugOut("UpdatePlayerChoices");
end

NameFont = {
  standardFont,
  24,
  BlackColor
};

HeaderFont = {
  standardFont,
  48,
  BlackColor
};

ButtonX = kCenter;
ButtonY = 90;
ButtonYSpace = 30;

kMaxUsers = 10

NameButtonGraphics = {
	"buttons/choosenameover.png",
	"buttons/choosenamedown.png",
	"buttons/choosenameover.png",
	"buttons/choosenamedown.png"
};

NameStyle= {
	type = kRadio,
	flags = kHAlignCenter + kVAlignCenter,
	font = StandardButtonFont,
	graphics = NameButtonGraphics,
	sound = kDefaultButtonSound
};

--gSelected = -1 ;

function AddButtons( N )

	t = { SetStyle(NameStyle), BeginGroup() };

	for i = 0,N-1 do
		table.insert(t,
			Button{
				name="button"..i,
--				command=function() gSelected = i; end,
				label="#name",
				x=ButtonX,
				y=ButtonY+(ButtonYSpace*i),
			}
		);
	end

	return Group(t);
end

MakeDialog
{
	-- Name the modal dialog
	name="chooseplayer",
	Bitmap
	{
		image="backgrounds/largedialog",
		x=kCenter,
		y=kCenter,

		ChoosePlayerWindow
		{
			x = 0, y = 0, w=kMax, h=kMax,

			Text
			{
				font = HeaderFont,
				name = "selectaplayer",
				x=0,y=40,w=kMax,h=kMax,
				flags = kHAlignCenter + kVAlignTop,
				label = "selectaplayer"
			};

			AddButtons( kMaxUsers );

			SetStyle(ButtonStyle);

			Button
			{
				x=60,
				y=410,
				name="editname",
				label="",
				label="editname",
				command =
					function()
						gEnterNameValue = GetSelectedPlayerName();
						name = DoModal("scripts/entername.lua");
						if (not(name == nil)) then
							SetCurrentUser(GetCurrentSelectedUser());
							ChangeCurrentUserName(name);
							SetPlayerChoices();
							SetWelcomeName();
						end
					end
			};

			Button
			{
				x=220,
				y=450,
				name="chooseok",
				label="ok",
				default=true,
				cancel=true,	-- ALSO allow them to exit the dialog by hitting escape
				command =
					function()
						SetCurrentUser(GetCurrentSelectedUser());
						SetWelcomeName();
						PopModal("chooseplayer");
					end

			};

			Button
			{
				x=220,
				y=410,
				name="newplayer",
				label="newplayer",
				command =
					function()
						gEnterNameValue = "";
						name = DoModal("scripts/entername.lua");
						if (not(name == nil)) then
							CreateNewUser(name);
							SetPlayerChoices()
						end
					end
			};

			Button
			{
				x=60,
				y=450,
				name="delete",
				label="deleteplayer",
				command =
					function()
						result = DisplayDialog {
							"scripts/yesno.lua",
							body ="deleteconfirm",
							title = "deletetitle"
						};
						if (result=='yes') then
							DeleteUser(GetCurrentSelectedUser())
							SetPlayerChoices()
						end
					end

			};
		}
	}
}


--UpdatePlayerChoices( 0,0, {} );

--SetPlayerChoices();

