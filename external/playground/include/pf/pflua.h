//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef PFLUA_H_INCLUDED
#define PFLUA_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

#ifdef __cplusplus
};

#include "luapluscd.h"
#include "luaautoblock.h"

/**
 * Wrap a Lua object for use within C++ code. Keeps a reference
 * to the Lua object so it won't be garbage collected.
 *
 * \ifnot BOOK
 * 	\ingroup luadocs
 * \endif
 */
class PFLIB_API TLuaObjectWrapper
{
public:
	/**
	 * Initialize our function with the object found on top of the stack.
	 * Pops object from stack.
	 *
	 * @param state  Lua state.
	 */
	TLuaObjectWrapper( lua_State * state ) :
		mState(state)
	{
		// Push our key.
		lua_pushlightuserdata(mState,this);
		// Swap top two elements
		lua_insert(mState,-2) ;
		lua_settable(mState,LUA_REGISTRYINDEX);
	}

	/**
	 * Copy constructor.
	 *
	 * @param other  State to copy.
	 */
	TLuaObjectWrapper( TLuaObjectWrapper & other ) :
		mState(other.mState)
	{
		lua_pushlightuserdata(mState,this);
		other.Push();
		lua_settable(mState,LUA_REGISTRYINDEX);
	}
private:
	// No copying!
	TLuaObjectWrapper & operator=( const TLuaObjectWrapper & other );
public:
	/**
	 * Destructor.
	 */
	virtual ~TLuaObjectWrapper()
	{
		// Push our key
		lua_pushlightuserdata(mState,this);
		// Push nil to delete the key
		lua_pushnil(mState);
		// Set the value
		lua_settable(mState,LUA_REGISTRYINDEX);
	}

	/**
	 * Push our object onto the stack. Note a Lua object can only
	 * be pushed onto the stack of the Lua interpreter that it was
	 * extracted from and originally created in.
	 */
	inline void Push()
	{
		// Push our key
		lua_pushlightuserdata(mState,this);
		// Get the value
		lua_gettable(mState,LUA_REGISTRYINDEX);
	}

	/**
	 * Is this object a string?
	 *
	 * @return True if it is a string.
	 */
	bool IsString()
	{
		LuaAutoBlock lab(mState);
		Push();
		return lua_isstring(mState,-1)!=0;
	}

	/**
	 * Is the object a number?
	 *
	 * @return True if the object is a number.
	 */
	bool IsNumber()
	{
		LuaAutoBlock lab(mState);
		Push();
		return lua_isnumber(mState,-1)!=0;
	}

	/**
	 * Is the object a table?
	 *
	 * @return True if a table
	 */
	bool IsTable()
	{
		LuaAutoBlock lab(mState);
		Push();
		return lua_istable(mState,-1)!=0;
	}

	/**
	 * Convert the object to a string.
	 *
	 * @return A string representation of the object, if one is available.
	 */
	str AsString()
	{
		LuaAutoBlock lab(mState);
		Push();
		return lua_tostring(mState,-1);
	}

	/**
	 * Convert the object to a number.
	 *
	 * @return A numeric representation of the object, if one is available.
	 */
	lua_Number AsNumber()
	{
		LuaAutoBlock lab(mState);
		Push();
		return lua_tonumber(mState,-1);
	}

	/**
	 * Get the current state associated with this object.
	 *
	 * @return A Lua state.
	 */
	inline lua_State * GetState() { return mState; }
protected:
	lua_State * mState ;
};

typedef TLuaObjectWrapper * pTLuaObjectWrapper;

#if !defined(USER_DOCS)
/**
 * @nouser
 *
 * A wrapper for a Lua thread.
 *
 * \ifnot BOOK
 * 	\ingroup luadocs
 * \endif
 * @see http://www.lua.org/manual/5.0/manual.html#3.20
 */
class PFLIB_API TLuaThread
{
public:
	/**
	 * Create a thread in parent state and manage it.
	 *
	 * @param parentState
	 *               Parent of the thread.
	 */
	TLuaThread( lua_State * parentState )
	{
		// Create a new thread and push it on the stack
		mThreadState = lua_newthread(parentState);
		// Grab the thread from the stack and wrap it up
		mThread= new TLuaObjectWrapper(parentState);
	}
	/// Destructor
	virtual ~TLuaThread()
	{
		delete mThread ;
	}
	inline lua_State * GetState() { return mThreadState; }
protected:
	TLuaObjectWrapper	*mThread ;
	lua_State			*mThreadState ;
};
#endif

/**
 * A wrapper for a Lua function
 * \ifnot BOOK
 * 	\ingroup luadocs
 * \endif
 */
class PFLIB_API TLuaFunction : public TLuaObjectWrapper
{
public:
	/**
	 * Constructor.
	 *
	 * @param state  State to extract Lua function from. Pulls the top item off
	 *               the stack and binds it to this TLuaFunction.
	 */
	TLuaFunction( lua_State * state ) :
		TLuaObjectWrapper(state)
	{

	}

	/**
	 * Call the function.
	 */
	void Call()
	{
		Push();

		if (lua_isfunction(mState,-1))
		{
			lua_getglobal(mState,"_ALERT");
			lua_insert(mState,1); 				// move ALERT to 1
			lua_pcall(mState,0,1,1);
		}
		else
		{
			lua_pop(mState,1);
		}
	}
	/**
	 * Test to verify that we're actually bound to a function.
	 *
	 * @return True if our bound object is a Lua function.
	 */
	bool IsFunction()
	{
		LuaAutoBlock lab(mState);

		Push();
		if (lua_isfunction(mState,-1))
		{
			return true;
		}
		return false ;
	}
};

#endif

#endif // PFLUA_H_INCLUDED
