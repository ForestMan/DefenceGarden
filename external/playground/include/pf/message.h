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
 * Interface for class TMessage
 */

#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#ifndef DEBUG_H_INCLUDED
#include "debug.h"
#endif

#ifndef PFTYPEINFO_H_INCLUDED
#include "pftypeinfo.h"
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#ifndef PFLUA_H_INCLUDED
#include "pflua.h"
#endif

/**
 * Application message base class.  Actual messages being
 * passed around by the application will either use TMessage
 * directly or derive from TMessage, depending on whether they
 * need additional payload.
 *
 * @warning Never use C++ or C style casting to coerce a TMessage
 * to another type; always use GetCast<>(). When a message is sent
 * from Lua, it is wrapped in a TLuaMessageWrapper, which will report
 * the mType of the contained TMessage-derived message--and it has a
 * GetCast operator that will give you the actual contained message.
 * But casting it to your target object will result in undefined (and
 * certainly incorrect) behavior, so it's best to always use GetCast.
 */
class PFLIB_API TMessage
{
	PFTYPEDEFBASE_DC(TMessage);
public:

	/**
	 * System-level predefined message IDs. Start user message
	 * IDs at kUserMessageBase
	 */
	enum EMessageID
	{
		/// A generic message--the derived class determines the type.
		kGeneric=0,

		/// Close the current window if you get this message ID.
		kCloseWindow =1,

		/// Trigger the default action (done by pressing "enter" equivalent)
		kDefaultAction,

		/// Button pressed
		kButtonPress,

		/// Unhandled user input
		kPressAnyKey,

		/// Time to exit the application
		kQuitNow,

		/// A modal window closed.
		kModalClosed,

		/// New information has been typed into/removed from a text edit field
		kTextEditChanged,

		/// This message is empty; it's being sent to run the accompanying command.
		kCommandOnly,

		/// A slider value changed
		kSliderValChanged,

		/// The mouse has been released on a slider
		kSliderMouseUp,

		/// Someone clicked above the slider handle to create a virtual page-up.
		kSliderPageUp,

		/// Someone clicked below the slider handle to create a virtual page-down.
		kSliderPageDown,

		/**
		 * First ID available for client applications.
		 */
		kUserMessageBase=1000
	};

	/**
	 * Constructor.
	 *
	 * @param type   Message type.
	 * @param name   Message name.
	 * @param destination
	 *               Optional destination window.
	 */
	TMessage( int32_t type=kGeneric, str name="", TWindow * destination=NULL ) :
		mType(type),
		mName(name),
		mDestination(destination)
	{

	}

	/// Destructor
	virtual ~TMessage() ;

	/** \name Type Information and Casting
	 * @{
	 */

	/**
	 * @fn PFClassId ClassId()
	 * Get the ClassId.
	 * @return A ClassId that can be passed to IsKindOf.
     * @see @ref rtti
	 */
	/** @fn virtual bool IsKindOf( PFClassId type )
	 * Determine whether this message is derived from type.
	 * @param type ClassId() of type to test.
	 * @see @ref rtti
	 */
	/**
	 * @fn template<class TO> TO * GetCast()
     * Safely cast this message to another type.
	 * @return A cast pointer, or NULL.
	 * @see @ref rtti
	 */

#if DOXYGEN
	PFClassId ClassId();
	virtual bool IsKindOf( PFClassId type );
	template<class TO> TO * GetCast();
#endif

	/** @} */

	/// The EMessageID of this message, or a user defined type (starting at kUserMessageBase).
	int32_t		mType ;

	/**
	 * Name of this message.
	 *
	 * For a button message, this is the name of the button
	 * that is sending the message.
	 */
	str		mName ;

	/// Optional TWindow destination
	TWindow *mDestination ;
};

struct lua_State ;
#ifndef USER_DOCS
namespace LPCD
{
	void PFLIB_API Push(lua_State* L, TMessage * value);
#if INCLUDE_DEBUG_STATEMENTS
	bool PFLIB_API Match(TypeWrapper<TMessage*>, lua_State* L, int idx);
#endif
	PFLIB_API TMessage * Get(TypeWrapper<TMessage*>, lua_State* L, int idx);
};

class TLuaMessage : public TLuaObjectWrapper
{
public:
	TLuaMessage( lua_State * state ) :
		TLuaObjectWrapper(state)
    {

    }

    TMessage * Get()
    {
        Push();
        TMessage * message= LPCD::Get(LPCD::TypeWrapper<TMessage*>(),GetState(),-1);
        lua_pop( GetState(), 1 );
        return message ;
    }
};

/**
@nouser

A Wrapper for a Lua-owned TMessage. Has a member TLuaMessage that
extracts the top element of the Lua stack and wraps it with a TLuaObjectWrapper.

The TLuaMessageWrapper class is used to allow TMessages that are passed through
the message system on the C++ side to always be deletable; once a message is owned
by Lua, the Lua garbage collection will expect to be able to destroy it. With the
TLuaMessageWrapper, the message can be successfully destroyed by the C++ code without
interfering with Lua object management.

\ifnot BOOK
  \ingroup luadocs
\endif

 */
class PFLIB_API TLuaMessageWrapper : public TMessage
{
//	We have a custom implementation of PFTYPEDEF below.
//    PFTYPEDEF(TMessage);
public:
	static PFClassId ClassId();

	/// A delegating version of the standard GetCast; will return a pointer
	/// to the contained object if that object matches the cast destination.
	template<class TO>
	TO * GetCast()
	{
		if (IsKindOf(TO::ClassId()))
		{
			return (TO*)(this);
		}
		if (mLuaMessage.Get()->IsKindOf(TO::ClassId()))
		{
			return (mLuaMessage.Get()->GetCast<TO>());
		}
		return NULL;
	}
	virtual bool IsKindOf( PFClassId type )
	{
		if (type == ClassId())
		{
			return true;
		}

		return TMessage::IsKindOf(type);
	}
	GET_TYPE();
public:
    TLuaMessageWrapper( lua_State * state ) :
        mLuaMessage(state)
    {
		TMessage * m = mLuaMessage.Get() ;
		mType = m->mType ;
		mName = m->mName ;
		mDestination = m->mDestination ;
    }
    TLuaMessage mLuaMessage ;

private:
	TLuaMessageWrapper( const TLuaMessageWrapper & message );
	TLuaMessageWrapper & operator=( const TLuaMessageWrapper message );
};
#endif // !USER_DOCS

#endif // MESSAGE_H_INCLUDED
