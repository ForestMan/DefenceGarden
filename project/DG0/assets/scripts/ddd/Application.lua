
require( "scripts/ddd/ilua.lua" );
require( "scripts/ddd/Factory.lua" );
require( "scripts/ddd/Level.lua" );
require( "scripts/ddd/Constants.lua" );

--------------------------------------------------------------------------------

function createLevel(levelID)
	level=Level:new();
	level.ID_ = levelID;
	--level.gameID_ = gameID;
	addLevel( level );
end

--------------------------------------------------------------------------------

Application = {}

classInheritance( Application, ILua )

--------------------------------------------------------------------------------

function Application:onInit()
	createGame( "game", GT_DEFENCE_GARDEN );
	return true;
end

function Application:onCreateLevelTable(levelID)
	createLevel(levelID);
	return true;
end
