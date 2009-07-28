
require( "scripts/ddd/ilua.lua" );
require( "scripts/ddd/game.lua" );

--------------------------------------------------------------------------------

Application = {}

classInheritance( Application, ILua )

--------------------------------------------------------------------------------

function Application:onInit()
	game=Game:new();
	game.type_="game";
	addGame( game );
	return true;
end
