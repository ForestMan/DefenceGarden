//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

/** @condfile */

#ifndef RECT_H_INCLUDED
#define RECT_H_INCLUDED

#ifndef POINT_H_INCLUDED
#include "point.h"
#endif

/**
 * A rectangle.
 *
 * Like the Windows RECT class, the right and bottom (X2/Y2)
 * coordinates are one past the edge of the rectangle.
 */
class PFLIB_API TRect
{
public:
	/// Default constructor. Zeros all members.
	TRect():
		x1(0), y1(0), x2(0), y2(0)
	{}

    /// Initializing constructor.
	TRect(int32_t X1,int32_t Y1,int32_t X2,int32_t Y2) :
		x1(X1), y1(Y1), x2(X2), y2(Y2)
	{}

	/**
	 * Construct a TRect from two points.
	 *
	 * @param topLeft Upper left corner of the TRect.
	 * @param bottomRight
	 *                Lower right corner of the TRect.
	 */
	TRect(const TPoint & topLeft, const TPoint & bottomRight) ;

	/**
	 * Build a TRect from the upper left corner and the
	 * desired width and height.
	 *
	 * @param x      X coordinate of the upper left corner.
	 * @param y      Y coordinate of the upper left corner.
	 * @param w      Width
	 * @param h      Height
	 *
	 * @return A new TRect calculated from X,Y,W,H
	 */
	static TRect FromXYWH( int32_t x, int32_t y, int32_t w, int32_t h )
	{
		return TRect( x, y, x+w, y+h );
	}

	/**
	 * Get the width of the rectangle.
	 *
	 * @return Exclusive width of the rectangle. (x2-x1)
	 */
	inline int32_t GetWidth() const {return x2-x1;};

	/**
	 * Get the height of the rectangle.
	 *
	 * @return Exclusive height of the rectangle. (y2-y1)
	 */
	inline int32_t GetHeight() const {return y2-y1;};

    /// Equality.
	bool	operator==( const TRect & r ) const
	{
		return (x1==r.x1) &&
			   (x2==r.x2) &&
			   (y1==r.y1) &&
			   (y2==r.y2) ;
	}

    /// Inequality.
	bool 	operator!=( const TRect & r ) const
	{
		return !( *this == r );
	}

	/**
	 * Move a rect by a point.
	 *
	 * @param p      Point to offset rect by.
	 *
	 * @return A reference to this.
	 */
	TRect & operator+=( const TPoint & p )
	{
		x1+=p.mX ;
		x2+=p.mX ;
		y1+=p.mY ;
		y2+=p.mY ;

		return *this;
	}

	/**
	 * Move a rect by a point.
	 *
	 * @param p      Point to offset rect by.
	 *
	 * @return A reference to this.
	 */
	TRect & operator-=( const TPoint & p )
	{
		x1-=p.mX ;
		x2-=p.mX ;
		y1-=p.mY ;
		y2-=p.mY ;

		return *this;
	}

	/**
     * Test to see whether a rectangle is inside this rectangle.
	 *
     * @param r      TRect to test.
	 *
     * @return True if r is inside this.
	 */
	bool	Contains( const TRect & r ) const
	{
		if ((r.x1<x1) || (r.x2>x2) ||
			(r.y1<y1) || (r.y2>y2) )
		{
			return false;
		}
		return true;
	}

	/**
	 * Test to see whether a point is inside this rectangle.
	 *
	 * @param p      Point to test.
	 *
	 * @return True if inside. If p.x==x2 or p.y==y2, the test
	 *         fails.
	 */
	bool Contains( const TPoint & p ) const
	{
		if ( (p.x<x1) || (p.x>=x2) ||
			 (p.y<y1) || (p.y>=y2) )
		{
			return false ;
		}
		return true ;
	}

    /**
     * Test to see whether a point is inside this rectangle.
     *
     * @deprecated  This function is going to be deleted in favor of
     *              the more clearly named TRect::Contains().
     *
     * @param p      Point to test.
     *
     * @return True if inside. If p.x==x2 or p.y==y2, the test
     *         fails.
     */
	bool	IsInside( const TPoint & p ) const
	{
		return Contains(p);
	}

    /**
     * Test to see if another rectangle overlaps this rectangle.
     *
     * @param rect   Rectangle to test.
     *
     * @return True on overlap.
     */
	bool	Overlaps( const TRect & rect ) const
	{
		if ( (x1>rect.x2) ||
			 (y1>rect.y2) ||
			 (rect.x1>x2) ||
			 (rect.y1>y2) )
		{
			return false ;
		}
		return true ;
	}

	/**
	 * Make this rectangle the union of the two parameter
	 * rectangles. Can pass this rectangle as either parameter.
	 *
	 * @param rect1  First
	 * @param rect2  Second
	 */
	void	Union( const TRect & rect1, const TRect & rect2 );

	/**
	 * Make this rectangle the intersection of the two parameter
	 * rectangles. Can pass this rectangle as either parameter.
	 *
	 * @warning Assumes the two rectangles overlap. Resulting value is
	 * 			undefined if the original rectangles do not overlap.
	 *
	 * @param rect1  First
	 * @param rect2  Second
	 */
	void 	Intersect( const TRect & rect1, const TRect & rect2 );

	/**
	 * Get a TPoint that represents the upper left corner
	 * of the rectangle.
	 *
	 * @return Corner point
	 */
	TPoint	& GetTopLeft() { return * (TPoint*)&x1 ; }

	/**
	 * Get a TPoint that represents the upper left corner
	 * of the rectangle.
	 *
	 * @return Corner point
	 */
	const TPoint	& GetTopLeft() const { return * (TPoint*)&x1 ; }

	/**
	 * Get a TPoint that represents the lower right corner
	 * of the rectangle.
	 *
	 * @return Corner point
	 */
	TPoint	& GetBottomRight() { return * (TPoint*)&x2 ; }

	/**
	 * Get a TPoint that represents the lower right corner
	 * of the rectangle.
	 *
	 * @return Corner point
	 */
	const TPoint	& GetBottomRight() const { return * (TPoint*)&x2 ; }

#if DOXYGEN
    int32_t	x1 ;	///< Upper left corner x coordinate
    int32_t	y1 ;	///< Upper left corner y coordinate
    int32_t	x2 ;	///< Lower right corner x coordinate
    int32_t	y2 ;	///< Lower right corner y coordinate
#else
    union
    {
        int32_t	x1 ;	///< Upper left corner x coordinate
        int32_t	mX1 ;	///< Upper left corner x coordinate
    };
    union
    {
        int32_t	y1 ;	///< Upper left corner y coordinate
        int32_t	mY1 ;	///< Upper left corner y coordinate
    };
    union
    {
        int32_t	x2 ;	///< Lower right corner x coordinate
        int32_t	mX2 ;	///< Lower right corner x coordinate
    };
    union
    {
        int32_t	y2 ;	///< Lower right corner y coordinate
        int32_t	mY2 ;	///< Lower right corner y coordinate
    };
#endif
};

/**
 * A TRect that's forced to be unsigned at all times.
 *
 * Will ASSERT in debug builds if you construct a TURect with
 * a TRect that has negative values.
 *
 * @see TRect
 */
class PFLIB_API TURect : public TRect
{
public:
	/**
	 * Default constructor.
	 */
    TURect()
    {}

	/**
	 * Construct from four values.
	 *
	 * @param X1     Left edge.
	 * @param Y1     Top edge.
	 * @param X2     One past right edge.
	 * @param Y2     One past bottom edge.
	 */
    TURect(uint32_t X1,uint32_t Y1,uint32_t X2,uint32_t Y2) :
		TRect(X1,Y1,X2,Y2)
    {
        Check();
    }

	/**
	 * Construct from a TRect
	 *
	 * @param rect   Source TRect to construct from.
	 */
    TURect(const TRect& rect) :
        TRect(rect)
    {
        Check();
    }

	/**
	 * Assign from a TRect.
	 *
	 * @param rect   Source rectangle.
	 */
	TURect& operator=( const TRect& rect )
	{
		TRect::operator=(rect);
		Check();
		return *this;
	}
	/**
	 * Assign from a TURect.
	 *
	 * @param rect   Source rectangle.
	 */
	TURect& operator=( const TURect& rect )
	{
		TRect::operator=(rect);
		return *this;
	}
    /**
     * Construct a TRect from two points.
     *
     * @param topLeft Upper left corner of the TRect.
     * @param bottomRight
     *                Lower right corner of the TRect.
	 *
	 * @see TPoint
     */
    TURect(const TPoint & topLeft, const TPoint & bottomRight) ;

#if !USER_DOCS
	/// Verify that this rect is properly unsigned.
	/// @nouser
	void Check();
#endif
};

#endif // RECT_H_INCLUDED

