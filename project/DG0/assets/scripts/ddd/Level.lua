
require( "scripts/ddd/ilua.lua" );
require( "scripts/ddd/Factory.lua" );
require( "scripts/ddd/Constants.lua" );

--------------------------------------------------------------------------------

Level = { owner = nil, gameID_=0 }

classInheritance( Level, ILua )

--------------------------------------------------------------------------------

function Level:onInit()
	createActor(0, "actor", GT_DEFENCE_GARDEN, LT_MAIN_LEVEL);
	return true;
end