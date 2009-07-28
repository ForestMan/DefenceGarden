#include "ddd/ILua.h"

#include <pf/windowmanager.h>
#include <pf/script.h>

namespace ddd
{

	//-------------------------------------------------------------------------

	ILua::ILua()
		: luaTable_( 0 )
	{
	}

	//-------------------------------------------------------------------------

	ILua::~ILua()
	{
	}

	//-------------------------------------------------------------------------

	void ILua::init( TLuaTable* const luaTable )
	{
		luaTable_ = new TLuaTable( *luaTable );
		setID( MAX_UNSIGN_LONG );
		luaFunctions_.clear();
		onInit();
	}

	//-------------------------------------------------------------------------

	void ILua::release()
	{
		onRelease();
		luaFunctions_.clear();
		delete luaTable_;
		luaTable_ = 0;
	}

	//-------------------------------------------------------------------------

	void ILua::executeLuaFunction( const unsigned long functionID )
	{
		assert( hasLuaFunction( functionID ) );

		getLuaFunction( functionID )->Push();
		getLuaTable().Push();
		TWindowManager::GetInstance()->GetScript()->RunFunction( 1, 0 );
	}

	//-------------------------------------------------------------------------

	void ILua::executeLuaFunction( const unsigned long functionID, TLuaTable* parameters )
	{
		assert( hasLuaFunction( functionID ) );
		assert( 0 != parameters );

		getLuaFunction( functionID )->Push();
		getLuaTable().Push();
		parameters->Push();
		TWindowManager::GetInstance()->GetScript()->RunFunction( 2, 0 );
	}
}