//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------
#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER
#ifndef LUAAUTOBLOCK_H_INCLUDED
#define LUAAUTOBLOCK_H_INCLUDED

#ifndef USER_DOCS

/**
 * @condfile 
 * Interface for LuaAutoBlock.
 */	

/**
 * 	\addtogroup luadocs 
 *	Lua related documentation
 * @{
 */

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
/**
	A helper class for automatically setting the stack state back to point of
	LuaAutoBlock creation.
*/
class LuaAutoBlock
{
public:

	LuaAutoBlock(lua_State* state) :
		m_state(state),
		m_stackTop(lua_gettop(m_state))
	{
	}

	~LuaAutoBlock()
	{
		lua_settop(m_state, m_stackTop);
	}

	bool Balanced() { return lua_gettop(m_state) == m_stackTop ; }
private:
	LuaAutoBlock(const LuaAutoBlock& src);					// Not implemented
	const LuaAutoBlock& operator=(const LuaAutoBlock& src);	// Not implemented

	lua_State* m_state;
	int32_t m_stackTop;
};


/*@}*/

#endif // USERDOCS

#endif // LUAAUTOBLOCK_H
