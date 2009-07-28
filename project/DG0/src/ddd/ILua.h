#pragma once

#include <map>
#include <pf/luatable.h> 
#include <pf/pflua.h> 
#include <pf/script.h>

#include "ddd/LuaUtils.h"

namespace ddd
{

	class ILua
	{
	public:
		ILua();
		virtual ~ILua();

		void init( TLuaTable* const luaTable );
		void release();

		inline const bool isInited()const;

		inline TLuaTable& getLuaTable()const;

		inline void setID( const unsigned long id );
		inline const unsigned long getID()const;
	protected:
		
		virtual void onInit() = 0;
		virtual void onRelease() = 0;

		inline void initLuaFunction( const unsigned long functionID, const char* functionName );
		inline const bool hasLuaFunction( const unsigned long functionID );
		inline void releaseLuaFunction( const unsigned long functionID );
		void executeLuaFunction( const unsigned long functionID );
		void executeLuaFunction( const unsigned long functionID, TLuaTable* parameters );
	
	private:

		inline TLuaFunction* getLuaFunction( const unsigned long functionID );
	
	private:

		TLuaTable* luaTable_;
		std::map< unsigned long, TLuaFunction* > luaFunctions_;
	};

	//-------------------------------------------------------------------------

	inline const bool ILua::isInited()const
	{
		return (0 != luaTable_);
	}

	//-------------------------------------------------------------------------

	inline TLuaTable& ILua::getLuaTable()const
	{
		assert( isInited() );
		return *luaTable_;
	}

	//-------------------------------------------------------------------------

	inline void ILua::setID( const unsigned long id )
	{
		setULong( getLuaTable(), "ID_", id );
	}

	//-------------------------------------------------------------------------
	
	inline const unsigned long ILua::getID()const
	{
		return getULong( getLuaTable(), "ID_" );
	}

	//-------------------------------------------------------------------------

	inline void ILua::initLuaFunction( const unsigned long functionID, const char* functionName )
	{
		assert( functionID >= 0 );
		assert( 0!= functionName );
		assert( getLuaTable().GetFunction( functionName ) );
		luaFunctions_[ functionID ] = getLuaTable().GetFunction( functionName );
	}

	//-------------------------------------------------------------------------

	inline const bool ILua::hasLuaFunction( const unsigned long functionID )
	{
		return 0 != luaFunctions_[ functionID ];
	}

	//-------------------------------------------------------------------------

	inline void ILua::releaseLuaFunction( const unsigned long functionID )
	{
		assert( hasLuaFunction( functionID ) );
		luaFunctions_.erase( functionID );
	}

	//-------------------------------------------------------------------------

	inline TLuaFunction* ILua::getLuaFunction( const unsigned long functionID )
	{
		assert( hasLuaFunction( functionID ) );
		return luaFunctions_[ functionID ];
	}

	//-------------------------------------------------------------------------
}