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
 * Interface for class TPoint
 */
#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#ifndef PFLIBCORE_H_INCLUDED
#include "pflibcore.h"
#endif

#ifndef PFTYPES_H_INCLUDED
#include "pftypes.h"
#endif

/**
 * The TPoint class is a 2d integer point representation.
 */
class PFLIB_API TPoint
{
public:
	/// Default constructor. Zeros all members.
	TPoint() : x(0),y(0) {}

    /// Initializing constructor.
    TPoint(int32_t x, int32_t y) : mX(x),mY(y) {}

#if DOXYGEN
    int32_t		x ; ///< Horizontal coordinate.
    int32_t		y ; ///< Vertical coordinate. 
#else
	union
	{
		int32_t		mX ;    // compatibility
		int32_t		x ;
	};
	union
	{
		int32_t		mY ;    // compatibility
		int32_t		y ;
	};
#endif

    /// Equality.
	bool	operator==( const TPoint & point ) const
	{
		return (x == point.x) && (y == point.y) ;
	}

    /// Inequality.
	bool	operator!=( const TPoint & point ) const
	{
		return !((*this)==point);
	}

    /// Add a point to this point.
	TPoint & operator+=( const TPoint & point )
	{
		x += point.x;
		y += point.y;
		return *this;
	}

    /// Memberwise addition.
	TPoint operator+( const TPoint & point ) const
	{
		TPoint newpoint= *this;
		return newpoint+=point ;
	}

    /// Subtract a point from this point.
	TPoint & operator-=( const TPoint & point )
	{
		x -= point.x;
		y -= point.y;
		return *this;
	}

    /// Memberwise subtraction.
	TPoint operator-( const TPoint & point ) const
	{
		TPoint newpoint= *this;
		return newpoint-=point ;
	}

    /// Negation.
	TPoint operator-()
	{
		return TPoint( -x, -y );
	}
};

#endif // POINT_H_INCLUDED
