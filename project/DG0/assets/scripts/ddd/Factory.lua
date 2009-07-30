
require( "scripts/ddd/Game.lua" );

--------------------------------------------------------------------------------

function createGame( gameType, gameID )
	game=Game:new();
	game.type_= gameType;
	game.ID_ = gameID;
	addGame( game );
end

--------------------------------------------------------------------------------