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
 *  \defgroup luadocs Lua-Related Documentation
 *
 * Documentation on predefined Lua constants and functions, as well as
 * C++ interfaces to Lua.
 *
 * See the section on \ref luascripts for more information.
 *
 * @{
 */

/**
 * @file
 *
 * Playfirst-modified LuaPlus Call Dispatcher.
 *
 * <b>The functionality added by this file is only going to be useful for
 * advanced Lua interactions and extensions. For a simpler approach, see the macros
 * ScriptRegisterFunctor, ScriptRegisterMemberFunctor, ScriptRegisterDirect, and
 * ScriptRegisterMemberDirect.
 *
 * If the information below seems mysterious or opaque, then you probably don't need to
 * know or use any of it.
 * </b>
 *
 * If you're curious or would like to learn more, you should read about
 * the Lua API at http://www.lua.org/manual/5.0/manual.html#3 and the documentation
 * for the original version of these Lua extensions at
 * http://wwhiz.com/LuaPlus/LuaPlusCallDispatcher.html
 *
 * PlayFirst extensions include Lua mapping for Lua-strings to str and changes to
 * the behavior of lua_pushobjectdirectclosure and lua_pushobjectfunctorclosure to
 * take a pointer to this rather than a reference. Internally, the behavior has changed
 * to keep a copy of the pointer rather than a copy of the original structure.
 *
 */

///////////////////////////////////////////////////////////////////////////////
// This source file is part of the LuaPlus source distribution and is Copyright
// 2002-2004 by Joshua C. Jensen (jjensen@workspacewhiz.com).
//
// The latest version may be obtained from http://wwhiz.com/LuaPlus/.
//
// The code presented in this file may be used in any environment it is
// acceptable to use Lua.
///////////////////////////////////////////////////////////////////////////////
#ifndef LUAPLUSCD_H_INCLUDED
#define LUAPLUSCD_H_INCLUDED

#ifndef __cplusplus
#error "This file can only be used in C++ files as it relies heavily on templates."
#endif

#ifndef PFLUA_H_INCLUDED
#include "pflua.h"
#endif

#ifndef DEBUG_H_INCLUDED
#include "debug.h"
#endif

#include <string.h>

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#pragma once

#if !USER_DOCS
/// LuaPlus Call Dispatcher
namespace LPCD
{
	/// A Lua light user data wrapper
	struct LuaLightUserData
	{
		LuaLightUserData(const void* value) :
			m_value(value)
		{
		}

		const void* m_value;
	};


	/// A Lua user data wrapper
	struct LuaUserData
	{
		LuaUserData(const void* value) :
			m_value(value)
		{
		}

		const void* m_value;
	};

	/// A Lua nil-type wrapper
	struct LuaNil
	{
	};
	inline void Push(lua_State* L, bool value)				{  lua_pushboolean(L, value);  }
	inline void Push(lua_State* L, char value)				{  lua_pushnumber(L, value);  }
	inline void Push(lua_State* L, unsigned char value)		{  lua_pushnumber(L, value);  }
	inline void Push(lua_State* L, short value)				{  lua_pushnumber(L, value);  }
	inline void Push(lua_State* L, unsigned short value)	{  lua_pushnumber(L, value);  }
	inline void Push(lua_State* L, int value)				{  lua_pushnumber(L, value);  }
	inline void Push(lua_State* L, unsigned int value)		{  lua_pushnumber(L, value);  }
	inline void Push(lua_State* L, long value)				{  lua_pushnumber(L, value);  }
	inline void Push(lua_State* L, unsigned long value)		{  lua_pushnumber(L, value);  }
	inline void Push(lua_State* L, lua_Number value)		{  lua_pushnumber(L, value);  }
	inline void Push(lua_State* L, float value)				{  lua_pushnumber(L, (lua_Number)value);  }
	inline void Push(lua_State* L, const char* value)		{  lua_pushstring(L, value);  }
	inline void Push(lua_State* L, str value)				{  lua_pushstring(L, value.c_str());  }
	inline void Push(lua_State* L, const LuaNil&)			{  lua_pushnil(L);  }
	inline void Push(lua_State* L, lua_CFunction value)		{  lua_pushcclosure(L, value, 0);  }
	inline void Push(lua_State* L, const void* value)		{  lua_pushlightuserdata(L, (void*)value);  }
	inline void Push(lua_State* L, const LuaLightUserData& value)	{  lua_pushlightuserdata(L, (void*)value.m_value);  }

	template<class T> struct TypeWrapper {};

#if INCLUDE_DEBUG_STATEMENTS
	inline bool	Match(TypeWrapper<bool>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TBOOLEAN;  }
	inline bool	Match(TypeWrapper<char>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<unsigned char>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<short>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<unsigned short>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<int>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<unsigned int>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<long>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<unsigned long>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<float>, lua_State* L, int idx)
		{  int type = lua_type(L, idx);  return type == LUA_TNUMBER  ||  type == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<double>, lua_State* L, int idx)
		{  int type = lua_type(L, idx);  return type == LUA_TNUMBER  ||  type == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<const char*>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TSTRING;  }
	inline bool	Match(TypeWrapper<str>, lua_State* L, int idx)
		{  return (lua_type(L, idx) == LUA_TSTRING) || (lua_type(L, idx) == LUA_TNONE);  }
#endif

	inline void				Get(TypeWrapper<void>, lua_State*, int)
		{  }
	inline bool				Get(TypeWrapper<bool>, lua_State* L, int idx)
		{  return lua_toboolean(L, idx) != 0;  }
	inline char				Get(TypeWrapper<char>, lua_State* L, int idx)
		{  return static_cast<char>(lua_tonumber(L, idx));  }
	inline unsigned char	Get(TypeWrapper<unsigned char>, lua_State* L, int idx)
		{  return static_cast<unsigned char>(lua_tonumber(L, idx));  }
	inline short			Get(TypeWrapper<short>, lua_State* L, int idx)
		{  return static_cast<short>(lua_tonumber(L, idx));  }
	inline unsigned short	Get(TypeWrapper<unsigned short>, lua_State* L, int idx)
		{  return static_cast<unsigned short>(lua_tonumber(L, idx));  }
	inline int				Get(TypeWrapper<int>, lua_State* L, int idx)
		{  return static_cast<int>(lua_tonumber(L, idx));  }
	inline unsigned int		Get(TypeWrapper<unsigned int>, lua_State* L, int idx)
		{  return static_cast<unsigned int>(lua_tonumber(L, idx));  }
	inline long				Get(TypeWrapper<long>, lua_State* L, int idx)
		{  return static_cast<long>(lua_tonumber(L, idx));  }
	inline unsigned long	Get(TypeWrapper<unsigned long>, lua_State* L, int idx)
		{  return static_cast<unsigned long>(lua_tonumber(L, idx));  }
	inline float			Get(TypeWrapper<float>, lua_State* L, int idx)
		{  return static_cast<float>(lua_tonumber(L, idx));  }
	inline double			Get(TypeWrapper<double>, lua_State* L, int idx)
		{  return static_cast<double>(lua_tonumber(L, idx));  }
	inline const char*		Get(TypeWrapper<const char*>, lua_State* L, int idx)
		{  return static_cast<const char*>(lua_tostring(L, idx));  }
	inline str				Get(TypeWrapper<str>, lua_State* L, int idx)
		{  return str(lua_tostring(L, idx));  }
	inline LuaNil			Get(TypeWrapper<LuaNil>, lua_State* , int )
		{  return LuaNil();  }
	inline lua_CFunction	Get(TypeWrapper<lua_CFunction>, lua_State* L, int idx)
		{  return static_cast<lua_CFunction>(lua_tocfunction(L, idx));  }

	//////////////////////////////////////////////////////////////////////////

#if INCLUDE_DEBUG_STATEMENTS
	#define luaL_argassert(arg, _index_) if (!Match(TypeWrapper<P##arg>(), L, _index_)) \
				luaL_argerror(L, _index_, "bad argument")
#else
	#define luaL_argassert(arg, _index_)
#endif

	template<class RT>
	struct ReturnSpecialization
	{
		static int Call(RT (*func)(), lua_State* L, int /*index*/)
		{
			RT ret = func();
			Push(L, ret);
			return 1;
		}


		template <typename P1>
		static int Call(RT (*func)(P1), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2>
		static int Call(RT (*func)(P1, P2), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3>
		static int Call(RT (*func)(P1, P2, P3), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4>
		static int Call(RT (*func)(P1, P2, P3, P4), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5>
		static int Call(RT (*func)(P1, P2, P3, P4, P5), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6>
		static int Call(RT (*func)(P1, P2, P3, P4, P5, P6), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6, typename P7>
		static int Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6)
			);
			Push(L, ret);
			return 1;
		}

		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		template <typename Callee>
		static int Call(Callee& callee, RT (Callee::*func)(), lua_State* L, int /*index*/)
		{
			RT ret = (callee.*func)();
			Push(L, ret);
			return 1;
		}


		template <typename Callee, typename P1>
		static int Call(Callee& callee, RT (Callee::*func)(P1), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0)
			);
			Push(L, ret);
			return 1;
		}


		template <typename Callee, typename P1, typename P2>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6)
			);
			Push(L, ret);
			return 1;
		}
	};


	template<>
	struct ReturnSpecialization<void>
	{
		static int Call(void (*func)(), lua_State* L, int /*index*/)
		{
			L;
			func();
			return 0;
		}


		template <typename P1>
		static int Call(void (*func)(P1), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);

			func(
				Get(TypeWrapper<P1>(), L, index + 0)
			);
			return 0;
		}


		template <typename P1, typename P2>
		static int Call(void (*func)(P1, P2), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1)
			);
			return 0;
		}


		template <typename P1, typename P2, typename P3>
		static int Call(void (*func)(P1, P2, P3), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4>
		static int Call(void (*func)(P1, P2, P3, P4), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4,
				 typename P5>
		static int Call(void (*func)(P1, P2, P3, P4, P5), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static int Call(void (*func)(P1, P2, P3, P4, P5, P6), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static int Call(void (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6)
			);
			return 0;
		}

		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		template <typename Callee>
		static int Call(Callee& callee, void (Callee::*func)(), lua_State* /*L*/, int /*index*/)
		{
			(callee.*func)();
			return 0;
		}


		template <typename Callee, typename P1>
		static int Call(Callee& callee, void (Callee::*func)(P1), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0)
			);
			return 0;
		}


		template <typename Callee, typename P1, typename P2>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, int index)
		{
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6)
			);
			return 0;
		}
	};


	template <typename RT>
	int Call(RT (*func)(), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(func, L, index);
	}


	template <typename RT, typename P1>
	int Call(RT (*func)(P1), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(func, L, index);
	}


	template <typename RT, typename P1, typename P2>
	int Call(RT (*func)(P1, P2), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3>
	int Call(RT (*func)(P1, P2, P3), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(func, L, index);
	}

	template <typename RT, typename P1, typename P2, typename P3,
		typename P4>
	int Call(RT (*func)(P1, P2, P3, P4), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(func, L, index);
	}

	template <typename RT, typename P1, typename P2, typename P3,
		typename P4, typename P5>
	int Call(RT (*func)(P1, P2, P3, P4, P5), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(func, L, index);
	}

	template <typename RT, typename P1, typename P2, typename P3,
		typename P4, typename P5, typename P6>
	int Call(RT (*func)(P1, P2, P3, P4, P5, P6), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(func, L, index);
	}

	template <typename RT, typename P1, typename P2, typename P3,
		typename P4, typename P5, typename P6, typename P7>
	int Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(func, L, index);
	}

	template <typename Callee, typename RT>
	int Call(Callee& callee, RT (Callee::*func)(), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1>
	int Call(Callee& callee, RT (Callee::*func)(P1), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5, typename P6>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5, typename P6, typename P7>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, int index)
	{
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	inline unsigned char* GetFirstUpValueAsUserData(lua_State* L)
	{
		void* buffer;

#ifndef FAST_DISPATCH
		buffer = lua_touserdata(L, lua_upvalueindex(1));
#else FAST_DISPATCH
#endif FAST_DISPATCH

		return (unsigned char*)buffer;
	}


	// The Helper class is needed for less fortunate compiler template implementations.
	template <typename Func>
	class DirectCallFunctionDispatchHelper
	{
	public:
		static inline int DirectCallFunctionDispatcher(lua_State* L)
		{
 			unsigned char* buffer = GetFirstUpValueAsUserData(L);
			return Call(*(Func*)(buffer), L, 1);
		}
	};


	template <typename Callee, typename Func>
	class DirectCallMemberDispatcherHelper
	{
	public:
		static inline int DirectCallMemberDispatcher(lua_State* L)
		{
 			unsigned char* buffer = GetFirstUpValueAsUserData(L);
			return Call(**(Callee**)buffer, *(Func*)(buffer + sizeof(Callee*)), L, 1);
		}
	};

	inline int lua_StateFunctionDispatcher(lua_State* L)
	{
		typedef int (*Functor)(lua_State*);
 		unsigned char* buffer = GetFirstUpValueAsUserData(L);
		Functor& func = *(Functor*)(buffer);
		return (*func)(L);
	}


	template <typename Callee>
	inline int lua_StateMemberDispatcher(lua_State* L)
	{
		typedef int (Callee::*Functor)(lua_State*);
 		unsigned char* buffer = GetFirstUpValueAsUserData(L);
		Callee& callee = **(Callee**)buffer;
		Functor& func = *(Functor*)(buffer + sizeof(Callee*));
		return (callee.*func)(L);
	}
} // namespace LPCD
#endif //USER_DOCS

/**
 * Push a function on the Lua stack that will be called "directly"
 * with a custom parameter list and return value ("directly").
 *
 * See \ref ScriptRegisterDirect() for a simplified wrapper macro.
 *
 * <b>Advanced user function; can be safely ignored by most users.
 * @ifnot BOOK
 * See the disclaimer on the LuaPlusCD.h description page.
 * @endif
 * </b>
 *
 * With this function you can expose <i>any</i> class member function
 * that uses the supported parameter and return value types to a Lua script.
 *
 * See ScriptRegisterDirect() for supported parameter types.
 *
 * @param L         Lua state.
 * @param func      Function to call.
 * @param nupvalues Number of upvalues (usually 0; see Lua docs)
 * @see ScriptRegisterDirect
 */
template <typename Func>
inline void lua_pushdirectclosure(lua_State* L, Func func, unsigned int nupvalues)
{
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(func));
	memcpy(buffer, &func, sizeof(func));
	lua_pushcclosure(L, LPCD::DirectCallFunctionDispatchHelper<Func>::DirectCallFunctionDispatcher, nupvalues + 1);
}

/**
 * Push a function on the Lua stack that will be called "directly"
 * with a custom parameter list and return value ("directly"). More details
 * are available in lua_pushdirectclosure(lua_State* L, Func func, unsigned int nupvalues)
 *
 * See \ref ScriptRegisterMemberDirect() for a simplified wrapper to this function.
 *
 * <b>Advanced user function; can be safely ignored by most users.
 * @ifnot BOOK
 * See the disclaimer on the LuaPlusCD.h description page.
 * @endif
 * </b>
 *
 * @param L         Lua state.
 * @param callee	A pointer to the class instance you want to bind your caller to.
 * @param func      Member function to call.
 * @param nupvalues Number of upvalues (usually 0; see Lua docs)
 *
 * @see lua_pushdirectclosure(lua_State* L, Func func, unsigned int nupvalues)
 * @see ScriptRegisterMemberDirect
 */
template <typename Callee, typename Func>
inline void lua_pushdirectclosure(lua_State* L, Callee* callee, Func func, unsigned int nupvalues)
{
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(Callee*) + sizeof(func));
	*((Callee**)buffer)= callee ;
	memcpy(buffer + sizeof(Callee*), &func, sizeof(func));
	lua_pushcclosure(L, LPCD::DirectCallMemberDispatcherHelper<Callee, Func>::DirectCallMemberDispatcher, nupvalues + 1);
}

#if !USER_DOCS
inline void lua_pushfunctorclosure(lua_State* L, int (*func)(lua_State*), unsigned int nupvalues)
{
	lua_pushcclosure(L, func, nupvalues);
}
#endif //USER_DOCS

/**
 * Push a member function on the Lua stack that will be called as a standard
 * Lua callback function. The standard function signature takes a parameter
 * lua_State* and returns int.
 *
 * This will allow you to create a member function and have Lua call it directly.
 *
 * <b>Advanced user function; can be safely ignored by most users.
 * @ifnot BOOK
 * See the disclaimer on the LuaPlusCD.h description page.
 * @endif
 * </b>
 *
 * @param L         Lua state.
 * @param callee    A pointer to the class that your function is a member of.
 * @param func      Function to call.
 * @param nupvalues Number of upvalues (usually 0; see Lua docs)
 */
template <typename Callee>
inline void lua_pushfunctorclosure(lua_State* L, Callee * callee, int (Callee::*func)(lua_State*), unsigned int nupvalues)
{
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(Callee*) + sizeof(func));
	*((Callee**)buffer)= callee ;
	memcpy(buffer + sizeof(Callee*), &func, sizeof(func));
	lua_pushcclosure(L, LPCD::lua_StateMemberDispatcher<Callee>, nupvalues + 1);
}


///////////////////////////////////////////////////////////////////////////////

#if !USER_DOCS
namespace LPCD
{
	inline void* GetObjectUserData(lua_State* L)
	{
		int type = lua_type(L, 1);
		if (type == LUA_TUSERDATA)
			return lua_unboxpointer(L, 1);
		else if (type == LUA_TTABLE)
		{
			lua_pushvalue(L, 1);
			lua_pushstring(L, "__object");
			lua_gettable(L, -2);
			if (!lua_isuserdata(L, -1))
				luaL_error(L, "The table does not have a userdata member called __object.");
			void* ret = lua_touserdata(L, -1);
			lua_pop(L, 1);
			return ret;
		}
		else
		{
			luaL_argerror(L, 1, "must be userdata or a table with a userdata member called __object");
		}

		return NULL;
	}

	template <typename Callee>
	inline int Object_MemberDispatcher(lua_State* L)
	{
		typedef int (Callee::*Functor)(lua_State*);
 		unsigned char* buffer = GetFirstUpValueAsUserData(L);
		Functor& func = *(Functor*)(buffer);
		Callee& callee = *(Callee*)GetObjectUserData(L);
		return (callee.*func)(L);
	}

	template <typename Callee, typename Func, int startIndex>
	class DirectCallObjectMemberDispatcherHelper
	{
	public:
		static inline int DirectCallMemberDispatcher(lua_State* L)
		{
 			unsigned char* buffer = GetFirstUpValueAsUserData(L);
			Callee& callee = *(Callee*)GetObjectUserData(L);
			return Call(callee, *(Func*)buffer, L, startIndex);
		}
	};
} // namespace LPCD

/**
 * @nouser
 *
 * Push a function that will be attached to a Lua object as a method.
 *
 * The function would be called using the object::method() syntax, where
 * object is either a table with an __object member, or a userdata that
 * contains a boxed pointer to the relevant C++ object.
 *
 * This is an advanced method and is not currently supported by PlayFirst.
 *
 */
template <typename Callee>
inline void lua_pushobjectfunctorclosure(lua_State* L, int (Callee::*func)(lua_State*), unsigned int nupvalues)
{
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(func));
	memcpy(buffer, &func, sizeof(func));
	lua_pushcclosure(L, LPCD::Object_MemberDispatcher<Callee>, nupvalues + 1);
}

/**
 * @nouser
 *
 * Push a function that will be attached to a Lua object as a method.
 *
 * The function would be called using the object::method() syntax, where
 * object is either a table with an __object member, or a userdata that
 * contains a boxed pointer to the relevant C++ object.
 *
 * This is an advanced method and is not currently supported by PlayFirst.
 *
 */
template <typename Callee, typename Func>
inline void lua_pushobjectdirectclosure(lua_State* L, Callee* callee, Func func, unsigned int nupvalues)
{
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(func));
	memcpy(buffer, &func, sizeof(func));
	lua_pushcclosure(L, LPCD::DirectCallObjectMemberDispatcherHelper<Callee, Func, 2>::DirectCallMemberDispatcher, nupvalues + 1);
}

#endif // USER_DOCS

/**
 * @}
 */

#endif LUAPLUS_LPCD_H

