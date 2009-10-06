
require( "scripts/ddd/Game.lua" );
require( "scripts/ddd/Actor.lua" );

--------------------------------------------------------------------------------

function createGame( gameType, gameID )
	game=Game:new();
	game.type_= gameType;
	game.ID_ = gameID;
	addGame( game );
end

--------------------------------------------------------------------------------

function createActor(actorID, actorType, gameID, levelID)
	actor=Actor:new();
	actor.ID_ = actorID;
	actor.type_= actorType;
	addActor( actor, gameID, levelID );
end

--------------------------------------------------------------------------------