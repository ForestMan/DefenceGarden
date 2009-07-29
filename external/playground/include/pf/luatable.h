//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------


/**
 * @condfile
 * Interface for class TLuaTable
 */

#ifndef LUATABLE_H_INCLUDED
#define LUATABLE_H_INCLUDED

#ifndef PFLUA_H_INCLUDED
#include "pflua.h"
#endif

#ifndef DEBUG_H_INCLUDED
#include "debug.h"
#endif

/**
 * A wrapper for Lua table access in C++.
 *
 * 	\ingroup luadocs
 */
class PFLIB_API TLuaTable : public TLuaObjectWrapper
{
public:

	/**
	 * Constructor
	 *
	 * @param state  The lua_State pointer of the table. Use GetState() on a
	 *               TScript to get the current state.
	 */
	TLuaTable( lua_State * state );

	/**
	 * Construct from a TLuaObjectWrapper
	 *
	 * @param lobj   Object wrapper that presumably wraps a table.
	 */
	TLuaTable( TLuaObjectWrapper & lobj ) :
		TLuaObjectWrapper( lobj )
	{
		ASSERT(lobj.IsTable());
	}

	/**
	 * Create a new table, and wrap it in a TLuaTable.
	 *
	 * @param state  State of Lua interpreter to use.
	 *
	 * @return A new (empty) TLuaTable. You're responsible for deleting it.
	 */
	static TLuaTable * Create( lua_State * state )
	{
		lua_newtable(state) ;
		return new TLuaTable(state);
	}

	/**
	 * How many elements in the indexed portion of the table?
	 *
     * @return Number of elements in the array portion of the table.
	 */
	uint32_t GetSize()
	{
		Push();
		uint32_t size = lua_getn(mState,-1);
		lua_pop(mState,1);
		return size;
	}

    /**
     * Get a string from the table.
     *
     * @param key    The key to the string.
     * @param defaultValue
     *               Default value if key isn't found.
     *
     * @return A copy of the string, if one exists at that key. Otherwise an empty string.
     */
	str	GetString( const char * key, str defaultValue="" );

    /**
     * Get a string from the table.
     *
     * @param key    The key to the string.
     * @param defaultValue
     *               Default value if key isn't found.
     *
     * @return A copy of the string, if one exists at that key. Otherwise an empty string.
     */
	str	GetString( lua_Number key, str defaultValue="" );

    /**
     * Get a lua_Number from a key in the table.
     *
     * @param key    Key to look up.
     * @param defaultValue
     *               Default value if key isn't found.
     *
     * @return A lua_Number, if one is found. Zero otherwise.
     */
	lua_Number	GetNumber( const char * key, lua_Number defaultValue=0 );

    /**
     * Get a lua_Number from a key in the table.
     *
     * @param key    Key to look up.
     * @param defaultValue
     *               Default value if key isn't found.
     *
     * @return A lua_Number, if one is found. Zero otherwise.
     */
	lua_Number	GetNumber( lua_Number key, lua_Number defaultValue=0 );

	/**
     * Assign a string to a string key.
	 *
	 * @param key    Key to set.
	 * @param value  Value to set.
	 */
	void Assign( str key, str value );

	/**
	 * Assign a string to a numeric key.
	 *
	 * @param key    Key to set.
	 * @param value  Value to set.
	 */
	void Assign( lua_Number key, str value );

	/**
     * Assign a number to a string key.
	 *
	 * @param key    Key to set.
	 * @param value  Value to set.
	 */
	void Assign( str key, lua_Number value );

	/**
     * Assign a number to a numeric key.
	 *
	 * @param key    Key to set.
	 * @param value  Value to set.
	 */
	void Assign( lua_Number key, lua_Number value );

	/**
     * Assign a boolean value to a string key.
	 *
	 * @param key    Key to set.
	 * @param value  Value to set.
	 */
	void Assign( str key, bool value );

	/**
     * Assign a boolean value to a numeric key.
	 *
	 * @param key    Key to set.
	 * @param value  Value to set.
	 */
	void Assign( lua_Number key, bool value );

	/**
	 * Assign a TLuaObjectWrapper-wrapped Lua object to a numeric
	 * key.
	 *
	 * @param key       Key to assign object to in table.
	 * @param luaObject Object to assign.
	 */
	void Assign( lua_Number key,TLuaObjectWrapper * luaObject );

	/**
     * Assign a TLuaObjectWrapper-wrapped Lua object to a string
	 * key.
	 *
	 * @param key       Key to assign object to in table.
	 * @param luaObject Object to assign.
	 */
	void Assign( str key,TLuaObjectWrapper * luaObject );

	/**
	 * Erase the key/value pair located at key in the table.
	 *
	 * @param key    Key to erase.
	 */
	void Erase( str key );

	/**
	 * Erase the key/value pair located at key in the table.
	 *
	 * @param key    Key to erase.
	 */
	void Erase( lua_Number key );

    /**
     * Get a boolean value from a key in the table.
     *
     * @param key    Key to look up.
     * @param defaultValue
     *               Default value if key isn't found.
     *
     * @return The value of the key as a boolean (using
     *         lua_toboolean()) if the key is found. False
     *         otherwise.
     */
	bool GetBoolean( const char * key, bool defaultValue=false );

	/**
	 * Get a TLuaFunction from a key in the table.
	 *
	 * @param key    Key to look up.
	 *
     * @return A TLuaFunction, if that slot in the table has a lua
     *         function. NULL otherwise. You should eventually
     *         delete this pointer.
	 */
	TLuaFunction * GetFunction( const char * key );

	/**
	 * Acquire an embedded Lua table from within this table.
	 *
	 * @param key    Key where table is stored.
	 *
	 * @return A new TLuaTable pointer which you must eventually delete.
	 */
	TLuaTable * GetTable( const char * key );

	/**
	 * Acquire an embedded Lua table from within this table.
	 *
	 * @param key    Key where table is stored.
	 *
	 * @return A new TLuaTable pointer which you must eventually delete.
	 */
	TLuaTable * GetTable( lua_Number key );

	/**
     * Get a TColor from a table of four values.
     *
     * In Lua, if you define a color using the Color() function, you
     * define it using values from 0-255. If you define using
     * FColor(), the values are from 0-1.
	 *
	 * @param key    Key of table.
	 * @param defaultValue
	 *               Default value to return if no table found.
	 *
	 * @return A TColor.
	 */
	TColor GetColor( str key, const TColor & defaultValue=TColor(0,0,0,0) );

	/**
	 * Get a TColor from a table of four values.
	 *
	 * In Lua, if you define a color using the Color() function, you
	 * define it using values from 0-255. If you define using
	 * FColor(), the values are from 0-1.
	 *
	 * @param key    Key of table.
	 * @param defaultValue
	 *               Default value to return if no table found.
	 *
	 * @return A TColor.
	 */
	TColor GetColor( lua_Number key, const TColor & defaultValue=TColor(0,0,0,0) );

	/**
	 * Takes a key and returns the key/value pair for the next
	 * key in the table. The key you pass in should start out NULL
	 * to start the iteration.
	 *
	 * @warning If you stop an iteration in the middle, you're
	 * responsible for deleting the last key you've received in
	 * addition to the normal deletion of the last value.
	 *
     * @par Example Usage
     *
     * @code
     *  TLuaObjectWrapper *key=NULL;
     *  TLuaObjectWrapper *value;
     *  while ( (value=options->GetNext(&key))!=NULL)
     *  {
     * 		// Here we have a key/value pair.
     * 		str keyString = key->GetString(); str valueString =
     * 		value->GetString(); // this gets it as a string; you can also GetNumber()
	 * 		// if you need a table, you can TLuaTable table(value);
	 *
	 * 		delete value; // Delete the value once you've extracted it.
     *  }
     * @endcode
     *
     * @param key    Pointer to variable to receive next key.
     *               Initialize it to NULL to start the iteration,
     *               and leave the previous key in place to iterate.
	 *
	 * @return A pointer to a TLuaObjectWrapper. You must delete this pointer
	 *         when you're done with it. Returns NULL after the last table item.
	 */
	TLuaObjectWrapper* GetNext( TLuaObjectWrapper**key );

	/**
	 * Push the value at a particular string key onto the Lua
	 * stack.
	 *
	 * @param key    Key of value to retrieve.
	 */
	bool PushValue( const char * key );

	/**
	 * Push the value at a particular numeric key onto the Lua
	 * stack.
	 *
	 * @param key    Key of value to retrieve.
	 */
	bool PushValue( lua_Number key );

	/**
	 * Internal function to defer the delete of a table until the next
	 * event loop.
	 *
	 * @param table  Table to delete later.
	 */
	static void DeferDelete( TLuaTable * table );

#if INCLUDE_DEBUG_STATEMENTS

	/**
	 * Dump the table to the debug output stream.
	 */
	void Dump();
#endif
};

#if !USER_DOCS
/// LuaPlus Call Dispatcher
namespace LPCD
{
	inline void Push(lua_State* L, TLuaTable * value)
	{
		if (value)
		{
			// Make sure we're from the same Lua interpreter
			ASSERT(value->GetState()==L);
			value->Push();
		}
		else
			lua_newtable(L) ;
	}
#if INCLUDE_DEBUG_STATEMENTS
	inline bool	Match(TypeWrapper<TLuaTable *>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TTABLE;  }
#endif
	inline TLuaTable * Get(TypeWrapper<TLuaTable *>, lua_State* L, int idx)
	{
		// Push a copy onto the Lua stack
		lua_pushvalue(L,idx);

		// Create the TLuaTable, consume the copy, and return.
		TLuaTable * table = new TLuaTable(L);

		TLuaTable::DeferDelete(table);

		return table ;
	}

};
#endif // USER_DOCS

#endif // LUATABLE_H_INCLUDED
