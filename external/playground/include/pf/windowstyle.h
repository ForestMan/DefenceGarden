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
 * Interface for class TWindowStyle
 */

#ifndef WINDOWSTYLE_H_INCLUDED
#define WINDOWSTYLE_H_INCLUDED

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#ifndef TAGGING_IGNORE
class TLuaTable ;
class TLuaFunction ;
#endif

/**
 * An encapsulation of a Lua window style.
 */
class PFLIB_API TWindowStyle
{
public:
	/** \name Local Types
	 * @{
	 */

	/**
	 * Various window constants
	 */
	enum
	{
		/**
		 * Select center for a coordinate.
		 */
		kCenter=80000,
		/**
		 * Select max for a width or height
		 */
		kMax=160000,

		kHAlignLeft=0,		///< Align text to the left.
		kHAlignCenter=1,	///< Align text to the center.
		kHAlignRight=2,		///< Align text to the right.
		kVAlignTop=0,		///< Align text to the top.
		kVAlignCenter=4,	///< Align text vertically to the center.
		kVAlignBottom=8,	///< Align text to the bottom.
		kDefault=128,		///< Default text alignment.
		kPushButtonAlignment=kHAlignCenter+kVAlignCenter,
		kRadioButtonAlignment=kHAlignLeft+kVAlignCenter,
		kToggleButtonAlignment=kHAlignLeft+kVAlignCenter,
	};
	/** @} */
public:
	/// Construction
	TWindowStyle::TWindowStyle(TLuaTable * table);

	/// Destructor
	virtual ~TWindowStyle();

	/**
	 * Get a string parameter from the style.
	 *
	 * @param key    Name of the parameter to query.
	 * @param defaultValue
	 *               Default if parameter value not found.
	 *
	 * @return Value if found; defaultValue otherwise.
	 */
	str	 	GetString( str key, str defaultValue="" ) const ;
	/**
	 * Get a numeric parameter from the style.
	 *
	 * @param key    Name of the parameter to query.
	 * @param defaultValue
	 *               Default if parameter value not found.
	 *
	 * @return Value if found; defaultValue otherwise.
	 */
	double 	GetNumber( str key, double defaultValue=0 ) const ;
	/**
     * Get a boolean parameter from the style.
	 *
	 * @param key    Name of the parameter to query.
	 * @param defaultValue
	 *               Default if parameter value not found.
	 *
	 * @return Value if found; defaultValue otherwise.
	 */
	bool	GetBool( str key, bool defaultValue=false ) const ;
	/**
     * Get a color parameter from the style.
	 *
	 * @param key    Name of the parameter to query.
	 * @param defaultValue
	 *               Default if parameter value not found.
	 *
	 * @return Value if found; defaultValue otherwise.
	 */
	TColor	GetColor( str key, TColor defaultValue=TColor(0,0,0,1) ) const ;

	/**
     * Get a Lua function closure from the style.
     *
     * You will need to delete the function when you're done with
     * it.
	 *
	 * @param key    Name of the parameter to query.
	 *
     * @return Function if found; NULL otherwise.
	 */
	TLuaFunction * GetFunction( str key ) const;

	/**
     * Get a Lua table from the style.
     *
     * You will need to delete the table when you're done with it.
	 *
	 * @param key    Name of the parameter to query.
	 *
     * @return Table if found; NULL otherwise.
	 */
	TLuaTable * GetTable( str key ) ;

	/**
     * Get an integer parameter from the style.
	 *
	 * @param key    Name of the parameter to query.
	 * @param defaultValue
	 *               Default if parameter value not found.
	 *
	 * @return Value if found; defaultValue otherwise.
	 */
	int32_t GetInt(str key, int32_t defaultValue=0 ) const { return (int32_t)GetNumber(key,(double)defaultValue); }

private:

	// prevent copying
	TWindowStyle( TWindowStyle & );
	TWindowStyle & operator=( const TWindowStyle & );

	bool _GetTag(  const char * name, bool searchStyles=true ) const;

	TLuaTable * mTable ;
};

#endif // WINDOWSTYLE_H_INCLUDED
