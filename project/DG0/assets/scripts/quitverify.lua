require "scripts/style.lua"

if gQuitVerifyActive then
	return;
end

gQuitVerifyActive = true;
DisplayDialog{
	"scripts/yesno.lua",
	yes=function()
			PostMessage( CreateNamedMessage( kQuitNow, "quit" ) );
	   end,
	title = "quittitle",
	body = "quitconfirm",
};
gQuitVerifyActive = false;

