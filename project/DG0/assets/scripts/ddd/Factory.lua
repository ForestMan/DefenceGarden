
require( "scripts/ddd/Game.lua" );
require( "scripts/ddd/Level.lua" );

--------------------------------------------------------------------------------

function createGame( gameType, gameID )
	game=Game:new();
	game.type_= gameType;
	game.ID_ = gameID;
	addGame( game );
end

--------------------------------------------------------------------------------

function createLevel(levelID)
	level=Level:new();
	level.ID_ = levelID;
	addLevel( level );
end

--------------------------------------------------------------------------------