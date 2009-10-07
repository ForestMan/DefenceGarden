
require( "scripts/ddd/ilua.lua" );
require( "scripts/ddd/Factory.lua" );
require( "scripts/ddd/Level.lua" );
require( "scripts/ddd/Constants.lua" );

--------------------------------------------------------------------------------

function createLevel(levelID, gameID)
	level=Level:new();
	level.ID_ = levelID;
	level.gameID_ = gameID;
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

function Application:onCreateLevelTable(levelID, gameID)
	createLevel(levelID, gameID);
	return true;
end
