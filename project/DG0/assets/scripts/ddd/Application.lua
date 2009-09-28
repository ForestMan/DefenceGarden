
require( "scripts/ddd/ilua.lua" );
require( "scripts/ddd/Factory.lua" );

--------------------------------------------------------------------------------

Application = {}

classInheritance( Application, ILua )

--------------------------------------------------------------------------------

GT_DEFENCE_GARDEN = 0;

function Application:onInit()
	createGame( "game", GT_DEFENCE_GARDEN );
	return true;
end
