
require( "scripts/ddd/functionality.lua" );

--------------------------------------------------------------------------------

function classInheritance( heir, parent )
	return concatTables( parent, heir, false )
end

--------------------------------------------------------------------------------

ILua = { ID_=0, type_="unknown" }

function ILua:new()
	return cloneTable( self )
end

--------------------------------------------------------------------------------