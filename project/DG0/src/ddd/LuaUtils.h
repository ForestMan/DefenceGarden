#pragma once

#include <pf/luatable.h> 
#include "ddd/Types.h"

namespace ddd
{
	
	inline void setULong( TLuaTable& luaTable, const char* key, const unsigned long num )
	{
		assert(0 != key);
		luaTable.Assign( key, static_cast< lua_Number >( num ) );
	}

	inline const unsigned long getULong( const TLuaTable& luaTable, const char* key )
	{
		assert(0 != key);
		return static_cast< unsigned long >( const_cast< TLuaTable & >( luaTable ).GetNumber( key ) );
	}

}