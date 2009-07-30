
require( "scripts/ddd/ilua.lua" );
require( "scripts/ddd/Factory.lua" );

--------------------------------------------------------------------------------

Application = {}

classInheritance( Application, ILua )

--------------------------------------------------------------------------------

GAME_DEFENCE_GARDEN = 1;

function Application:onInit()
	createGame( "game", GAME_DEFENCE_GARDEN );
	return true;
end
