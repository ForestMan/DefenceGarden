
require( "scripts/ddd/ilua.lua" );
require( "scripts/ddd/Level.lua" );

--------------------------------------------------------------------------------

function createLevel( levelType, levelID, gameID )
	level=Level:new();
	level.type_= levelType;
	level.ID_ = levelID;
	addLevel( level, gameID );
end

--------------------------------------------------------------------------------

Game = {}

classInheritance( Game, ILua )

--------------------------------------------------------------------------------

function Game:onInit()
	createLevel( "level", 0, self.ID_ );
	return true;
end