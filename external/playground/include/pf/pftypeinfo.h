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
 * @file
 *
 * Runtime type information handling support macros. This file
 * contains a number of macros that add a more flexible
 * runtime-type information facility than is supported natively
 * by C++. For any class you want to decorate with additional
 * runtime information, you'll need one call in the class
 * definition, with a corresponding call in the implementation
 * file. The base class definition has its own call as well.
 *
 * The basic form is PFTYPEDEF() in the class, PFTYPEIMPL() in
 * the implementation file, with the base class using
 * PFTYPEDEFBASE(). This set would be used in classes that do
 * not need dynamic creation or to be stored in shared pointers.
 *
 * The PFSHAREDTYPEDEF() variation is used in classes that will
 * be held in shared pointers. It requires that you use
 * PFSHAREDTYPEDEFBASE() in the base class, and that the base
 * class be derived from
 * enable_shared_from_this&lt;BASECLASS&gt;. In the
 * implementation files you can use PFTYPEIMPL() with
 * PFSHAREDTYPEDEF() or PFSHAREDTYPEDEFBASE().
 *
 * The _DC variations are similar to the ones described above,
 * only they also support dynamic creation by defining a
 * function CreateFromId() that takes the ClassId() of a class
 * and creates a new instance of the class. PFTYPEDEF_DC()
 * matches PFTYPEIMPL_DC(). The PFTYPEIMPL_DCA() variation needs
 * to be used for any abstract class.
 *
 * @see \ref rtti
 */

#ifndef PFTYPEINFO_H_INCLUDED
#define PFTYPEINFO_H_INCLUDED

#ifndef REF_H_INCLUDED
#include "ref.h"
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#if USE_DEBUG_NEW && !defined(DEBUG_H_INCLUDED)
#define new DEBUG_NEW
#define THIS_FILE  __FILE__
#endif

#if !USER_DOCS

#define GET_TYPE() virtual const char* GetTypeName()
#define GET_TYPE_DEF(CLASS) const char* CLASS::GetTypeName() { return #CLASS; }

typedef ptrdiff_t PFClassId ;

/**
 * @internal Implementation details
 * Implementation details of PFTYPEDEF and PFTYPEDEFBASE.
 */
#define _TYPEDEF()										\
public:													\
	static PFClassId ClassId();							\
	template<class TO>									\
	TO * GetCast()										\
	{													\
		if (this==NULL)									\
		{                                               \
			return NULL ;                               \
		}                                               \
		if (IsKindOf(TO::ClassId()))					\
		{												\
			return (TO*)(this);							\
		}												\
		return NULL;									\
	}													\
	GET_TYPE();											\

#define _CREATEFROMID(THISCLASS)			\
typedef THISCLASS * (* DynCreator)() ;		\
static THISCLASS * CreateFromId( PFClassId id );	\

#define _SHAREDTYPEDEF()								\
public:													\
	static PFClassId ClassId();							\
	template<class TO>									\
	shared_ptr<TO> GetCast()							\
	{													\
		if (this==NULL)									\
		{                                               \
			return shared_ptr<TO>() ;                   \
		}                                               \
		if (IsKindOf(TO::ClassId()))					\
		{												\
			return static_pointer_cast<TO>(shared_from_this());	\
		}												\
		return shared_ptr<TO>();						\
	}													\
	GET_TYPE();											\


#define _GETREF(BASECLASS)								\
	shared_ptr<BASECLASS> GetRef()						\
	{	return shared_from_this(); }					\

#define _ISKINDOF(BASECLASS)				\
virtual bool IsKindOf( PFClassId type )		\
{											\
	if (type == ClassId())					\
	{										\
		return true;						\
	}										\
											\
	return BASECLASS::IsKindOf(type);		\
}											\

#define _ISKINDOFBASE()						\
virtual bool IsKindOf( PFClassId type )			\
{											\
	if (type == ClassId())					\
	{										\
		return true;						\
	}										\
	return false ;							\
}											\

#endif

/**
 * Additional definitions for a class that needs run time
 * type information.
 *
 * Putting this macro in a class will declare and define
 * IsKindOf(), GetCast() and ClassId() for that class.
 *
 * @see \ref rtti
 */
#if USER_DOCS

#define PFTYPEDEF(THISCLASS,BASECLASS)

#else
#define PFTYPEDEF(THISCLASS,BASECLASS)		\
_TYPEDEF()									\
_ISKINDOF(BASECLASS)						\
_CREATEFROMID(THISCLASS)					\
private:

#endif
/**
 * Additional definitions for a class that needs run time
 * type information and dynamic creation.
 *
 * Putting this macro in a class will declare and define
 * DynamicCreate(), IsKindOf(), GetCast() and ClassId() for that class.
 *
 * @see \ref rtti
 */
#if USER_DOCS
#define PFTYPEDEF_DC(THISCLASS,BASECLASS)
#else
#define PFTYPEDEF_DC(THISCLASS,BASECLASS)	\
_TYPEDEF()									\
_ISKINDOF(BASECLASS)						\
_CREATEFROMID(THISCLASS)					\
private:
#endif

/**
 * Additional definitions for a class that needs run time
 * type information, and that is controlled through shared
 * pointers.
 *
 * Putting this macro in a class will declare and define
 * IsKindOf(), GetCast() and ClassId() for that class.
 *
 * @param BASECLASS Our base class.
 *
 * @see \ref rtti
 */
#if USER_DOCS
#define PFSHAREDTYPEDEF(BASECLASS)
#else
#define PFSHAREDTYPEDEF(BASECLASS)	\
_SHAREDTYPEDEF()					\
_ISKINDOF(BASECLASS)				\
private:

#endif

/**
 * Additional definitions for a class that needs run time
 * type information and dynamic creation, and that is
 * controlled through shared pointers.
 *
 * Putting this macro in a class will declare and define
 * DynamicCreate(), IsKindOf(), GetCast() and ClassId() for that class.
 *
 * @param THISCLASS The class we're defining.
 * @param BASECLASS The (single) base class.
 *
 * @see \ref rtti
 */
#if USER_DOCS
#define PFSHAREDTYPEDEF_DC(THISCLASS,BASECLASS)
#else
#define PFSHAREDTYPEDEF_DC(THISCLASS,BASECLASS)	\
_SHAREDTYPEDEF()								\
_ISKINDOF(BASECLASS)							\
_CREATEFROMID(THISCLASS)						\
private:
#endif

/**
 * Additional definitions for the base class of a polymorphic
 * type that needs run time type information.
 *
 * Putting this macro in a class will declare and define
 * IsKindOf(), GetCast() and ClassId() for that class.
 */
#if USER_DOCS
#define PFTYPEDEFBASE()
#else
#define PFTYPEDEFBASE()				\
_TYPEDEF()							\
_ISKINDOFBASE()						\
private:
#endif

/**
 * Additional definitions for the base class of a polymorphic
 * type that needs run time type information and dynamic
 * creation.
 *
 * Putting this macro in a class definition file will declare
 * and define IsKindOf(), GetCast() and ClassId() for that
 * class.
 *
 * @param THISCLASS The current class to declare.
 */
#if USER_DOCS
#define PFTYPEDEFBASE_DC(THISCLASS)
#else
#define PFTYPEDEFBASE_DC(THISCLASS)	\
_TYPEDEF()							\
_ISKINDOFBASE()						\
_CREATEFROMID(THISCLASS)			\
private:
#endif

/**
 * Additional definitions for the base class of a polymorphic
 * type that needs run time type information, and that will be
 * stored in shared pointers (reference counted, like
 * TTextureRef).
 *
 * Putting this macro in a class will declare and define
 * IsKindOf(), GetCast() and ClassId() for that class.
 *
 * @param THISCLASS The current class to declare.
 */
#if USER_DOCS
#define PFSHAREDTYPEDEFBASE(THISCLASS)
#else
#define PFSHAREDTYPEDEFBASE(THISCLASS)	\
_SHAREDTYPEDEF()						\
_ISKINDOFBASE()							\
_GETREF(THISCLASS)						\
private:
#endif

/**
 * Additional definitions for the base class of a polymorphic
 * type that needs run time type information and dynamic
 * creation, and that will be stored in shared pointers
 * (reference counted, like TTextureRef).
 *
 * Putting this macro in a class will declare and define
 * IsKindOf(), GetCast() and ClassId() for that class.
 */
#if USER_DOCS
#define PFSHAREDTYPEDEFBASE_DC(THISCLASS)
#else
#define PFSHAREDTYPEDEFBASE_DC(THISCLASS)	\
_SHAREDTYPEDEF()							\
_ISKINDOFBASE()								\
_CREATEFROMID(THISCLASS)					\
_GETREF(THISCLASS)							\
private:
#endif

#ifndef DOXYGEN

#define _CREATEFROMIDIMPL(THISCLASS)		\
THISCLASS * THISCLASS::CreateFromId( PFClassId id )	\
{                                           \
	THISCLASS * p = ((DynCreator)id)();     \
	if (id==(PFClassId)p)                   \
	{                                       \
		return NULL ;                       \
	}                                       \
	ASSERT(p->IsKindOf(id));                \
	return p;                               \
}                                           \

#endif

/**
 * Implementation for run time type information.
 *
 * Use this macro in the implementation file.
 */
#if USER_DOCS
#define PFTYPEIMPL(THISCLASS)
#else
#define PFTYPEIMPL(THISCLASS)				\
PFClassId THISCLASS::ClassId()	   			\
{											\
	return (PFClassId)&ClassId;	   			\
}											\
GET_TYPE_DEF(THISCLASS);					\

#endif

/**
 * Implementation for run time type information for a class
 * with dynamic creation.
 *
 * Use this macro in the implementation file.
 */
#if USER_DOCS
#define PFTYPEIMPL_DC(THISCLASS)
#else
#define PFTYPEIMPL_DC(THISCLASS)			\
static THISCLASS * THISCLASS##DynamicCreate()	\
{ 											\
	return new THISCLASS ;                  \
}                                           \
PFClassId THISCLASS::ClassId()				\
{											\
	return (PFClassId)&THISCLASS##DynamicCreate;\
}											\
_CREATEFROMIDIMPL(THISCLASS)				\
GET_TYPE_DEF(THISCLASS);					\

#endif

/**
 * Implementation for run time type information for an abstract class,
 * descendents of which will require dynamic creation.
 *
 * Use this macro in the implementation file.
 */
#if USER_DOCS
#define PFTYPEIMPL_DCA(THISCLASS)
#else
#define PFTYPEIMPL_DCA(THISCLASS)			\
static THISCLASS * THISCLASS##DynamicCreate()	\
{ 											\
	return NULL ;			                 \
}                                           \
PFClassId THISCLASS::ClassId()				\
{											\
	return (PFClassId)&THISCLASS##DynamicCreate;\
}											\
_CREATEFROMIDIMPL(THISCLASS)				\
GET_TYPE_DEF(THISCLASS);					\

#endif

/**
 * \page rtti Type Information and Casting
 *
 * \lanchor{rtti}
 *
 * \section rttiintro Dynamic Casting
 *
 * C++ has support for runtime type information (RTTI), which includes
 * the ability to safely dynamically cast from one type to another,
 * but there are several shortcomings to the standard RTTI
 * support. For one, there is no dynamic creation support. For another,
 * when classes are contained in smart pointers, a straight
 * dynamic_cast won't work, because as far as C++ is concerned
 * the objects have no relationship to each other. Finally,
 * there is no reflection in C++ classes--you can't ask a class
 * what type it is and get a predictable response.
 *
 * The Playground SDK&trade; RTTI system supports dynamic creation of
 * classes, safe casting between smart pointers to related classes, and
 * identifying classes by name.
 *
 * \section rttiintwindow RTTI in TWindow
 *
 * The TWindow hierarchy doesn't require the use of the smart-pointer features,
 * but does use dynamic creation. Every %TWindow-derived type includes a static member function
 * ClassId(). %TButton::ClassId() will return the class identifier for the TButton class, for instance.
 *
 * To find out if a %TWindow* is a %TButton, use the following
 * pattern:
@code
	TWindow * window = ....

	if (window->IsKindOf( TButton::ClassId() ))
	{
		// Our window is a button
	}
@endcode
 * Most of the time you'll probably need to get a %TButton
 * directly. That's where %GetCast() comes in--it works like
 * dynamic_cast to cast our %TWindow* to an appropriate type,
 * returning a NULL pointer if the cast is inappropriate:
 *
@code
	TWindow * window = ....

	TButton * button = window->GetCast<TButton>();

	if (button)
	{
		// Our window is a button, and now we have a button to play with
	}
@endcode
 *
 * @if HTML
 * \ref userguidecontents <br>
 * \ref contents
 * @endif
 */

#if USE_DEBUG_NEW && !defined(DEBUG_H_INCLUDED)
#undef new
#undef THISCLASS
#endif

#endif // PFTYPEINFO_H_INCLUDED
