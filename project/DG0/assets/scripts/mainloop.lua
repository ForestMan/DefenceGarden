require( "scripts/ddd/Application.lua" );

-- Main game loop
function Main()

	--DisplaySplash(
	--	"splash/playfirst_animated_logo.swf",
	--	"splash/playfirst_logo",4000
	--	);
	--DisplaySplash("", "splash/distributor_logo",4000);
	
	app = Application:new();
	initApplication( app );

	-- Push the game selection screen
	while true do
		DoMainWindow("scripts/mainmenu.lua");
		-- DoMainWindow will exit only if there are NO windows pushed on the stack, so
		-- a PopModal()/PushModal() combination will not cause this to loop.
	end
end

-- Return a function to be executed in a thread
return Main