
require( "scripts/ddd/ilua.lua" );

--------------------------------------------------------------------------------

Level = { owner = nil }

classInheritance( Level, ILua )

--------------------------------------------------------------------------------

function Level:onInit()
	return true;
end