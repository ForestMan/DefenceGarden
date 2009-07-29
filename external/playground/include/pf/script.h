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
 * Interface for class TScript
 */


#ifndef SCRIPT_H_INCLUDED
#define SCRIPT_H_INCLUDED

#ifndef PFLIBCORE_H_INCLUDED
#include "pflibcore.h"
#endif

#ifndef PLATFORM_H_INCLUDED
#include "platform.h"
#endif

#ifndef PFLUA_H_INCLUDED
#include "pflua.h"
#endif

#ifndef RECT_H_INCLUDED
#include "rect.h"
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#ifndef ANIMTASK_H_INCLUDED
#include "animtask.h"
#endif

#ifndef ASSET_H_INCLUDED
#include "asset.h"
#endif

#ifndef PFTYPEINFO_H_INCLUDED
#include "pftypeinfo.h"
#endif

#include <vector>

#ifndef TAGGING_IGNORE
struct lua_State ;	// A Lua script context.
class TLuaTable ;	// A Lua function wrapper.
#endif

/**
 * 	\addtogroup luadocs
 * @{
 */

/**
 * Register a member function with the standard Lua signature.
 *
 * The function signature should match:
 *
 * int FN( lua_State * L );
 *
 * @param script 	Script to add functor to.
 * @param name   	Lua name of function.
 * @param ptr	 	Pointer to "this" in the class we're binding to.
 * @param functor 	The member function to call.
 *
 * @see \ref luagamescript
 */
#if DOXYGEN
#define ScriptRegisterMemberFunctor(script,name,ptr,functor)
#else
#define ScriptRegisterMemberFunctor(script,name,ptr,functor) \
	do { lua_pushstring(script->GetState(), name ); \
    lua_pushfunctorclosure(script->GetState(), ptr, &functor, 0); \
    lua_settable(script->GetState(), LUA_GLOBALSINDEX); } while (0)
#endif

/**
 * Register a function with the standard Lua signature.
 *
 * The function signature should match:
 *
 * int FN( lua_State * L );
 *
 * @param script 	Script to add functor to.
 * @param name   	Lua name of function.
 * @param functor   The function to call.
 *
 * @see \ref luagamescript
 */
#if DOXYGEN
#define ScriptRegisterFunctor(script,name,functor)
#else
#define ScriptRegisterFunctor(script,name,functor) \
	do { lua_register(script->GetState(), name, functor); } while (0)
#endif

/**
 * Register a "Direct" called function: A function that will be
 * called by Lua directly with appropriate parameters.
 *
 * @param script 	Script to add functor to.
 * @param name   	Lua name of function.
 * @param ptr	 	Pointer to "this" in the class we're binding to.
 * @param directfunctor  The member function to call.
 *
 * See ScriptRegisterDirect for supported parameter types.
 * @see ScriptRegisterDirect
 * @see \ref luagamescript
 */
#if DOXYGEN
#define ScriptRegisterMemberDirect(script,name,ptr,directfunctor)
#else
#define ScriptRegisterMemberDirect(script,name,ptr,directfunctor) \
	do { lua_pushstring(script->GetState(), name ); lua_pushdirectclosure(script->GetState(), ptr, &directfunctor, 0); lua_settable(script->GetState(), LUA_GLOBALSINDEX); } while (0)
#endif
/**
 * Register a "Direct" called function: A function that will be
 * called by Lua directly with appropriate parameters. This can be
 * contrasted with ScriptRegisterMemberFunctor(), which calls a function with
 * the standard Lua function signature.
 *
 * @par Supported parameter types include:
 * - bool
 * - [unsigned] char
 * - [unsigned] [short] int (unsigned and short are optional)
 * - [unsigned] long
 * - lua_Number
 * - float
 * - const char*
 * - str
 * - const LuaNil&
 * - lua_CFunction
 * - TLuaTable * (see below for important notes)
 * - TMessage*
 * - const void* (return from Lua only)
 * - const LuaLightUserData& (return from Lua only)
 *
 * For \c %TLuaTable * as parameter, the pointer will exist for
 * the duration of your function, but will be deleted in the
 * next application event loop, so don't keep it around.
 *
 * For \c %TLuaTable * as a return type, you will need to return
 * a TLuaTable pointer that persists past the end of the
 * function; you are still responsible for deleting the pointer.
 *
 * @see \ref luagamescript
 */
#if DOXYGEN
#define ScriptRegisterDirect(script,name,directfunctor)
#else
#define ScriptRegisterDirect(script,name,directfunctor) \
	do { lua_pushstring(script->GetState(), name ); lua_pushdirectclosure(script->GetState(), directfunctor, 0); lua_settable(script->GetState(), LUA_GLOBALSINDEX); } while (0)
#endif

/**
 * Unregister a function that was previously registered using
 * ScriptRegisterDirect(), ScriptRegisterMemberDirect(),
 * ScriptRegisterFunctor() or ScriptRegisterMemberFunctor().
 */
#if DOXYGEN
#define ScriptUnregisterFunction(script,name)
#else
#define ScriptUnregisterFunction(script,name) \
	do { lua_pushstring(script->GetState(), name ); lua_pushnil(script->GetState()); lua_settable(script->GetState(), LUA_GLOBALSINDEX); } while (0)
#endif

/**
 * An encapsulation for a Lua script context.
 *
 * A TScript can be used in multiple ways. If you create a thread that
 * yields, then you can either use TScript::Resume to explicitly resume
 * the thread, or you can register it as a TAnimTask with an appropriate
 * dispatch (commonly TPlatform::AdoptTask or TModalWindow::AdoptTask). If
 * you're calling functions that exit normally, then you don't need to do
 * anything beyond calling the function or executing Lua code using DoLuaString(),
 * RunScript(), or RunFunction().
 *
 * The script syntax is Lua-standard. There are a few predefined
 * functions that are useful to know:
 *
 * - DebugOut() will print a string to the debug log.
 * - DumpTable() will dump a table recursively to the debug log.
 * - toparams() will take a table as a parameter and return the
 *   indexed members as multiple return values. You can use this
 *   to pass the members of a table to a vararg function, for
 *   instance.
 *
 * @see @ref luascripts
 * @see http://www.lua.org
 */
class PFLIB_API TScript : public TAnimTask
{
public:
	/** \name Construction/Destruction
	 *
	 *  @{
	 */

	/**
	 * Default Constructor
	 *
	 * @param luaState This one is really for internal use only.
	 *                 TScript really only "plays well" with Lua scripts created
	 *                 by Playground.
	 */
	TScript(lua_State * luaState=NULL);

	/// Destructor
	virtual ~TScript();


	/** @} */

	/** \name Lua Data Constructors
	 *
	 * Functions that are used to push new data on the Lua stack.
	 *
	 *  @{
	 */

	/**
	 * Set a global variable to a Lua light userdata value.
	 *
	 * @param name     Name of global.
	 * @param userData Value of userdata.
	 */
	void SetGlobalLightUserData( const char * name, void * userData );

#ifndef USER_DOCS
	void PushLightUserData( const char * name, void * userData )
	{
		SetGlobalLightUserData(name,userData);
	}
#endif

	/** @} */

	/** \name Lua Data Accessors
	 *
	 * Functions that are used to access parameters from the Lua table on
	 * the top of the stack.
	 *
	 *  @{
	 */

	/// Get the current Lua state
	lua_State * GetState() { return mLuaState; }

	/**
	 * Pop a TColor from the stack
	 *
	 * @return A TColor from the top of the Lua stack.
	 */
	TColor PopColor();

	/**
	 * Pop a TFont from the stack
	 *
	 * @return A TFont from the top of the Lua stack.
	 */
	class TFont PopFont();

	/**
	 * Pop a string from the stack
	 *
	 * @return A string from the top of the Lua stack.
	 */
	str PopString() ;

	/**
	 * Pop a TRect off the top of the stack.
	 *
	 * @return A TRect
	 */
	TRect PopRect();

	/**
	 * Pop a number from the stack
	 *
	 * @return A number from the top of the Lua stack.
	 */
	inline lua_Number PopNumber()
	{
		lua_Number f = lua_tonumber(mLuaState,-1);
		lua_pop(mLuaState, 1);	// Pop the number
		return f;
	}

	/**
	 * Pop a boolean from the stack.
	 *
	 * @return A bool from the top of the Lua stack.
	 */
	inline bool PopBool()
	{
		switch (lua_type(mLuaState,-1))
		{
			case LUA_TBOOLEAN :
				{
					int b = lua_toboolean(mLuaState,-1);
					lua_pop(mLuaState, 1);	// Pop the bool
					return b!=0;
				}
			case LUA_TNUMBER :
				{
					return PopNumber()!=0 ;
				}
			case LUA_TNIL :
				{
					lua_pop(mLuaState, 1);	// Pop the nil
					return false ;
				}
			case LUA_TSTRING :
				{
					str s = PopString() ;
					ERROR_WRITE(("PopBool used on lua string %s",s.c_str()));
					ASSERT(false && "PopBool used on a string! Check your log for what string.");
					return false ;
				}
		}
		ERROR_WRITE(("PopBool used on a type that isn't a number, a boolean, or nil. (type: %d)",lua_type(mLuaState,-1)));
		ASSERT(false && "PopBool used on an invalid type.");
		return false ;
	}
	/** @} */
	/** \name Client Interface
	 *
	 * Functions that are commonly used by a client.
	 *
	 * @{
	 */

	/**
	 * Run a Lua script in the current environment.
     *
     * If the Lua script returns a value at the top level, that
     * value is assumed to be a function that is to be run as a
     * thread.
	 *
	 * RunScript loads the script from the resource and then runs it
	 * using RunFunction.  See RunFunction for important restrictions
	 * and limitations.
	 *
	 * @param filename Filename to read
	 *
	 * @return true on success.
	 *
	 * @see RunFunction
	 */
	bool RunScript( str filename );

	/**
	 * Create a new TScript-derived class of the same type
	 * as this class, but running in a new Lua thread.
	 *
	 * Lua threading is cooperative multithreading: It is non-preemptive, and as
	 * such requires that you "yield" to pause processing.
	 *
	 * @return A TScript-derived class constructed with similar
	 *         parameters as the host class.
	 */
	TScript * NewThread() ;

    /**
	 * Execute a string in the Lua interpreter.
	 *
	 * Is not executed as a thread, so will not interfere with
	 * an existing thread that has yielded.
	 *
	 * @param luaCommand Command to execute.
	 * @param length     Optional length, for binary buffers.
	 * @param nresults   Optional number of results.
	 */
	void DoLuaString( str luaCommand, int32_t length=-1, int nresults=0 );

	/**
	 * Inject a function into a running Lua script. Default
	 * implementation is empty, but the TWindowManager::GetScript()
	 * script has a derived implementation.
	 *
	 * Attempts to inject the function on the top of the Lua stack
	 * into the current coroutine.
	 *
	 * The Lua function can take parameters, but is assumed to not
	 * return any results.
	 *
	 * @return True on success, false on failure.
	 *
	 * @see TScript::RunScript
	 */
	virtual bool InjectFunction() { lua_pop(mLuaState,1); return false ; }

	/**
	 * Resume a thread that has been suspended by a coroutine yield.
	 *
	 * @param narg   Number of arguments being passed in on the stack; these
	 *               arguments are returned as the results of the previous
	 *               yield.
	 *
	 * @return 0 if there are no errors running the coroutine, or an error
	 *         code. See http://www.lua.org/manual/5.0/manual.html#lua_pcall
	 * 		   for error codes.
	 */
	int32_t Resume( int32_t narg= 0 );

	/**
	 * Test to see if a coroutine can currently be successfully
	 * resumed.
	 *
	 * @param narg   Number of arguments you were planning to pass in to the
	 *               coroutine.
	 *
	 * @return True if a coroutine exists and is ready to be resumed. False
	 *         otherwise.
	 */
	bool CoroutineActive( int32_t narg= 0 );

	/** @} */

	/** \name Lua Global Data Accessors
	 *
	 * Functions that read global data from the Lua state. These are
	 * used both inside a creator and globally.
	 *
	 *  @{
	 */

	/**
	 * Get a string from a global Lua variable.
	 *
	 * @param name   Name of the Lua variable.
	 *
	 * @return The string, if found. An empty string otherwise.
	 */
	str GetGlobalString( str name );

	/**
	 * Get a string from a global Lua table.
	 *
	 * @param name   Name of the Lua table.
	 * @param index  Index of item to extract.
	 *
	 * @return The string, if table and index found. An empty string otherwise.
	 */
	str GetGlobalTableString( str name, int32_t index );

	/**
	 * Get a global table from a Lua state. The table is created with new, and
	 * must be deleted when you are done with it.
	 *
	 * @param name   Name of table to retrieve.
	 *
	 * @return A TLuaTable wrapping the global table.
	 */
	TLuaTable * GetGlobalTable( str name );

	/**
	 * Set a string global in a Lua environment.
	 *
	 * @param name   Name of the string variable.
	 * @param value  Value to set the string variable.
	 */
    void SetGlobalString( str name, str value );

	/**
	 * Set a numeric global in a Lua environment.
	 *
	 * @param name   Name of the lua_Number variable.
	 * @param value  Value to set the variable.
	 */
    void SetGlobalNumber( str name, lua_Number value );

    /**
	 * Get a number from a global Lua variable.
	 *
	 * @param name   Name of the Lua variable.
	 *
	 * @return The corresponding number, or 0 if not found.
	 */
	lua_Number GetGlobalNumber( str name );
    /** @} */

	/** \name TTask Overrides
	 *
	 * Functions that handle TTask behavior.
	 *
	 *  @{
	 */

	/**
	 * This function is called when it's time to execute this task.
	 *
	 * @return True to keep the task alive. False to destroy the task.
	 */
	virtual bool Animate();

	/** @} */

	/** \name Global Script Settings
	 *
	 * Set the path that all Lua scripts will search.
	 *
	 * @{
	 */

	/**
	 * Get the global Lua search path.
	 */
	static str GetLuaPath() { return mLuaPath; }

	/**
	 * Set the global Lua search path.
	 *
	 * @param luaPath New path for Lua.
	 */
	void SetLuaPath( str luaPath )
	{
		mLuaPath = luaPath ;
	}
	/*}*/

	/**
	 * Run the function with parameters on the Lua stack.
	 *
	 * This is different than the Lua APIs lua_call and lua_pcall in
	 * that it relies on InjectFunction to pass a function into a currently
	 * paused coroutine. InjectFunction is implemented in the TWindowManager
	 * script, but not in the base class. See Implementation Details below.
	 *
	 * Example usage:
	 *
	 * @code
TScript * s = TWindowManager::GetInstance()->GetScript();
// Push the function on the stack.
lua_getglobal( s->GetState(), "MyLuaFunction" );
// Run the function with no parameters or results.
s->RunFunction(0,0);
	 * @endcode
	 *
	 * The above will call MyLuaFunction() in Lua with no parameters.
	 *
	 * Example with parameters and a return value:
	 *
	 * @code
TScript * s = TWindowManager::GetInstance()->GetScript();
// Push the function on the stack.
lua_getglobal( s->GetState(), "StopGame" );
// Push the number 25 onto the stack.
lua_pushnumber( s->GetState(), 25 );
// Run the function with one parameter and one result.
s->RunFunction(1,1);
// Get the return value from the top of the stack.
int32_t result = (int32_t)lua_tonumber( s->GetState(), -1 );
		@endcode
	 *
	 * See Lua API documentation from http://www.lua.org for more information on lua_getglobal, lua_pushnumber, and
	 * the rest of the internal Lua API.
	 *
	 * @warning If you need more than one return value from your function, it
	 *          cannot yield control, nor call any function that yields control
	 *          of the coroutine. A single return value is supported by the current
	 *          API.
	 *
	 * @section Implementation Details
	 *
	 * When Lua has paused a script using the "coroutine.yield" function or the C call
	 * lua_yield, it remains in a suspended state until one calls lua_resume or
	 * coroutine.resume. In this state you can actually still use the same interpreter to
	 * execute Lua functions, but those functions may not themselves yield, nor can a Lua
	 * function that was called via C resume the previous Lua coroutine.
	 *
     * To allow arbitrary function execution from C, the Playground
     * Lua "message loop" takes an extra parameter which is a
     * function to call. In other words, whenever the Lua message
     * loop has yielded to wait for a message, you can pass it in a
     * message and/or a command to execute. The TWindowManager
     * version of InjectFunction calls Resume with that command as a
     * parameter, and it's executed as part of the main thread--so
     * it can therefore enter its own wait loops, call other script
     * functions, etc. However, that function that's passed in as a
     * parameter can take no parameters of its own; RunFunction uses
     * a Playground Lua call GetClosure to wrap your function plus
     * any parameters in a Lua closure, and then passes that closure
     * in to be executed.
	 *
	 * In the case that a coroutine is not currently active, RunFunction does the trivial thing
	 * and calls lua_pcall with the standard error handler.
	 *
	 * Note that the base class implementation of InjectFunction does nothing, and it's only the derived
	 * window UI script that InjectFunction will work.
	 *
	 * This is not to hide the implementation, but instead because the injection
	 * relies on how the Lua script that yielded treats the return values of the
	 * yield statement. We can't make any assumptions about how your own custom
	 * scripts will process yield results; if you want to create your own version of
	 * InjectFunction, derive from TScript and add your own implementation. As an example:
	 * @code
	 * bool InjectFunction()
	 * {
	 *      return Resume(1)==0;
	 * }
	 * @endcode
	 *
	 * This would assume that your Lua code interpreted the first return value from yield as
	 * a function, and then ran the function:
	 *
	 * @luacode
	 *      f = yield();
	 *      if (f) then
	 *          f();
	 *      end
	 * @endluacode
	 *
	 * That way, the function is executed as part of your thread, rather than outside of it. If you
	 * need multiple threads, see TScript::NewThread().
	 *
	 * @param nargs    Number of arguments.
	 * @param nresults Number of results. If there's a chance your function will
	 *                 be executed during a paused coroutine, <i>and</i> your function
	 *                 needs to be able to yield, then this number should be zero
	 *                 or one.
	 *
	 * @return 0 on success. Lua error code otherwise.
	 */
	int32_t RunFunction(int32_t nargs=0,int32_t nresults=1);

public:
#ifndef USER_DOCS
	/** @nouser
	 * Register a table for deletion.
	 */
	void DeferDeleteTable( TLuaTable * table );
#endif
private:

	/**
	 * Internal initialization function.
	 */
	void LuaInit();

	/**
	 * Current Lua state.
	 */
	lua_State 		*mLuaState ;

	/**
	 * The thread that this script is running in, if any.
	 */
	TLuaThread 		*mThread ;

	typedef std::vector<TLuaTable*> TableList;
	TableList		mTablesToDelete ;

	// The path that Lua should search for files
	static str mLuaPath ;
};

/**
 * An encapsulation of a compiled Lua source file.
 */
class PFLIB_API TScriptCode : public TAsset
{
	PFSHAREDTYPEDEF(TAsset);
private:
	TScriptCode(const char * s, uint32_t size) :
		mScript(s,size),
		mLength(size)
	{

	}
public:

    /**
     * Accessor function for Lua script-code asset.
     *
     * Internally, when loading a Lua script from a file, the
     * internal routines use TScriptCode::Get() to load it. If you
     * keep a reference to the TScriptCode, then when the routine
     * calls TScriptCode::Get(), it will retrieve the cached (and
     * precompiled) copy rather than reloading it from disk.
     *
     * @param handle  File handle to load.
     * @param luaPath Any additional Lua search path necessary.
	 * @param error   An optional str that gets set with an error string.
     *
     * @return A reference to the compiled code object.
     */
    static TScriptCodeRef Get( str handle, str luaPath="", str * error=NULL );

    /**
     * Get the (compiled) script code.
     *
     * @return A pre-compiled Lua block. Can have embedded null characters;
     *         use TScriptCode::GetCodeLength() to determine the length.
     */
	str GetCode()
	{
		return mScript ;
	}
    /**
     * Get the length of the compiled code block.
     *
     * @return Code block length.
     */
	uint32_t GetCodeLength()
	{
		return mLength ;
	}
private:
	str	mScript;
	uint32_t mLength ;
};

/*@}*/

#endif // SCRIPT_H_INCLUDED
