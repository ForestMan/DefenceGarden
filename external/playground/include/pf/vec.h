//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#if !defined(VEC_H_INCLUDED)
#define VEC_H_INCLUDED

#ifndef PFTYPES_H_INCLUDED
#include "pftypes.h"
#endif

// Forward declarations
class TVec2;
class TVec3;
class TVec4;

/**
 * A 2d vector class.
 *
 * This class is a POD (plain-old-data) type with public member data.
 */
class PFLIB_API TVec2
{
public:
#if !USER_DOCS
	// traits
	enum { kDIM = 2 };
#endif

	/// Public X dimension.
	TReal x;
	/// Public Y dimension.
	TReal y;

	/// Constructor.
	TVec2();

    /**
     * Initializing constructor.
     *
     * @param X      X value
     * @param Y      Y value
     */
	TVec2(TReal X, TReal Y);

	/// Copy construction.
	TVec2(const TVec2& rhs);

    /**
     * Conversion from a TVec3.
     *
     * @param rhs    Source TVec3. Constructor drops the z parameter.
     */
    explicit TVec2(const TVec3& rhs);

    /**
     * Assignment.
     *
     * @return A reference to this.
     */
	TVec2& operator=(const TVec2& rhs);

    /**
     * Member accessor.
     *
     * @param i      Zero-based index.
     *
     * @return A reference to the i'th member.
     */
	TReal& operator[](TIndex i) 			{ return (&x)[i]; }

    /**
     * Member accessor.
     *
     * @param i      Zero-based index.
     *
     * @return A reference to the i'th member.
     */
	const TReal& operator[](TIndex i) const { return (&x)[i]; }

    /**
     * The dimension of this vector (2).
     *
     * @return 2
     */
	TIndex Dim() const						{ return kDIM; }

    /**
     * The length of this vector squared.
     *
     * @return @f$x^2+y^2@f$
     */
    TReal LengthSquared() const;

    /**
     * The length of this vector.
     *
     * @return @f$\sqrt{x^2+y^2}@f$
     */
    TReal Length() const;

    /**
     * Addition-assignment operator.
     *
     * @return A reference to this.
     */
	TVec2& operator+=(const TVec2& rhs);
    /**
     * Subtraction-assignment operator.
     *
     * @return A reference to this.
     */
	TVec2& operator-=(const TVec2& rhs);

#if 0
    TVec2& operator*=(const TVec2& rhs);
	TVec2& operator/=(const TVec2& rhs);
#endif

    /**
     * Scaling operator.
     *
     * @param s Scale factor.
     *
     * @return A reference to this, scaled as @f$(x*s,y*s)@f$
     */
	TVec2& operator*=(TReal s);
    /**
     * Scaling operator.
     *
     * @param s Scale divisor.
     *
     * @return A reference to this, scaled as @f$(x/s,y/s)@f$
     */
	TVec2& operator/=(TReal s);

    /**
     * Normalize this vector.
     *
     * Changes this vector to @f$(x/Length(),y/Length())@f$
     *
     * @return A reference to this.
     */
	TVec2& Normalize();

    /**
     * Unary negation.
     *
     * @return @f$(-x,-y)@f$
     */
	TVec2 operator-() const;
};

/**
 * Equality operator.
 *
 * @relates TVec2
 *
 * @return True if equal.
 */
bool PFLIB_API operator==(const TVec2& lhs, const TVec2& rhs);
/**
 * Inequality operator.
 *
 * @relates TVec2
 *
 * @return True if not equal.
 */
bool PFLIB_API operator!=(const TVec2& lhs, const TVec2& rhs);

/**
 * Addition operator.
 *
 * @relates TVec2
 *
 * @return @f$(x_1+x_2,y_1+y_2)@f$
 */
TVec2 PFLIB_API operator+(const TVec2& lhs, const TVec2& rhs);
/**
 * Subtraction operator.
 *
 * @relates TVec2
 *
 * @return @f$(x_1-x_2,y_1-y_2)@f$
 */
TVec2 PFLIB_API operator-(const TVec2& lhs, const TVec2& rhs);
#if 0
TVec2 PFLIB_API operator*(const TVec2& lhs, const TVec2& rhs);
TVec2 PFLIB_API operator/(const TVec2& lhs, const TVec2& rhs);
#endif
/**
 * Scaling operator.
 *
 * @relates TVec2
 *
 * @return @f$(x*s,y*s)@f$
 */
TVec2 PFLIB_API operator*(const TVec2& lhs, TReal s);
/**
 * Scaling operator.
 *
 * @relates TVec2
 *
 * @return @f$(x*s,y*s)@f$
 */
TVec2 PFLIB_API operator*(TReal s, const TVec2& rhs); 			// make TReal * TVec2 commutative

/**
 * Scaling operator.
 *
 * @relates TVec2
 *
 * @return @f$(x/s,y/s)@f$
 */
TVec2 PFLIB_API operator/(const TVec2& lhs, TReal s);

/**
 * Dot product function.
 *
 * @relates TVec2
 * @param lhs    Left-hand side of the dot product.
 * @param rhs    Right-hand side of the dot product.
 *
 * @return The dot product of the two vectors: @f$(x_1*x_2+y_1*y_2)@f$.
 */
TReal PFLIB_API DotProduct(const TVec2& lhs, const TVec2& rhs);

/**
 * A 3d vector class.
 *
 * This class is a POD (plain-old-data) type with public member data.
 */
class PFLIB_API TVec3
{
public:
#if !USER_DOCS
	// traits
	enum { kDIM = 3 };
#endif

    // data members
	TReal x;    ///< X dimension.
	TReal y;    ///< Y dimension.
	TReal z;    ///< Z dimension.

	/// Constructor.
	TVec3();

    /**
     * Initializing constructor.
     *
     * @param X      X value.
     * @param Y      Y value.
     * @param Z      Z value.
     */
	TVec3(TReal X, TReal Y, TReal Z);

    /**
     * Conversion constructor.
     *
     * @param rhs    TVec2 to convert from.
     * @param z      Additional z component to add; defaults to zero.
     */
	explicit TVec3(const TVec2& rhs, TReal z=0);

    /**
     * Conversion constructor.
     *
     * @param rhs    TVec4 to convert from. Drops the w component.
     */
	explicit TVec3(const TVec4& rhs); // slices off w

	/// Copy construction.
	TVec3(const TVec3& rhs);

    /**
     * Assignment.
     *
     * @return A reference to this.
     */
	TVec3& operator=(const TVec3& rhs);

    /**
     * Member accessor.
     *
     * @param i      Zero-based index.
     *
     * @return A reference to the i'th member.
     */
	TReal& operator[](TIndex i) 			{ return (&x)[i]; }
    /**
     * Member accessor.
     *
     * @param i      Zero-based index.
     *
     * @return A reference to the i'th member.
     */
	const TReal& operator[](TIndex i) const	{ return (&x)[i]; }

    /**
     * The dimension of this vector (3).
     *
     * @return 3
     */
	TIndex Dim() const 						{ return kDIM; }
    /**
     * The length of this vector squared.
     *
     * @return @f$x^2+y^2+z^2@f$
     */
	TReal LengthSquared() const;
    /**
     * The length of this vector.
     *
     * @return @f$\sqrt{x^2+y^2+z^2}@f$
     */
	TReal Length() const;
    /**
     * Normalize this vector.
     *
     * Changes this vector to @f$(x/Length(),y/Length(),z/Length())@f$
     *
     * @return A reference to this.
     */
	TVec3& Normalize();

    /**
     * Addition-assignment operator.
     *
     * @return A reference to this.
     */
	TVec3& operator+=(const TVec3& rhs);
    /**
     * Subtraction-assignment operator.
     *
     * @return A reference to this.
     */
	TVec3& operator-=(const TVec3& rhs);
//	TVec3& operator*=(const TVec3& rhs);
//	TVec3& operator/=(const TVec3& rhs);
    /**
     * Scaling operator.
     *
     * @param s Scale factor.
     *
     * @return A reference to this, scaled as @f$(x*s,y*s,z*s)@f$
     */
	TVec3& operator*=(TReal s);
    /**
     * Scaling operator.
     *
     * @param s Scale divisor.
     *
     * @return A reference to this, scaled as @f$(x/s,y/s,z/s)@f$
     */
	TVec3& operator/=(TReal rhs);

    /**
     * Unary negation.
     *
     * @return @f$(-x,-y,-z)@f$
     */
	TVec3 operator-() const;
};

/**
 * Equality operator.
 *
 * @relates TVec3
 *
 * @return True if equal.
 */
bool PFLIB_API operator==(const TVec3& lhs, const TVec3& rhs);
/**
 * Inequality operator.
 *
 * @relates TVec3
 *
 * @return True if not equal.
 */
bool PFLIB_API operator!=(const TVec3& lhs, const TVec3& rhs);
/**
 * Addition operator.
 *
 * @relates TVec3
 *
 * @return @f$(x_1+x_2,y_1+y_2,z_1+z_2)@f$
 */
TVec3 PFLIB_API operator+(const TVec3& lhs, const TVec3& rhs);
/**
 * Subtraction operator.
 *
 * @relates TVec3
 *
 * @return @f$(x_1-x_2,y_1-y_2,z_1-z_2)@f$
 */
TVec3 PFLIB_API operator-(const TVec3& lhs, const TVec3& rhs);
/**
 * Scaling operator.
 *
 * @relates TVec3
 *
 * @return @f$(x*s,y*s,z*s)@f$
 */
TVec3 PFLIB_API operator*(const TVec3& lhs, TReal s);
/**
 * Scaling operator.
 *
 * @relates TVec3
 *
 * @return @f$(x*s,y*s,z*s)@f$
 */
TVec3 PFLIB_API operator*(TReal s, const TVec3& rhs);				// make TReal * TVec3 commutative

//TVec3 PFLIB_API operator*(const TVec3& lhs, const TVec3& rhs);
/**
 * Scaling operator.
 *
 * @relates TVec3
 *
 * @return @f$(x/s,y/s,z/s)@f$
 */
TVec3 PFLIB_API operator/(const TVec3& lhs, TReal s);
/**
 * Dot product function.
 *
 * @relates TVec3
 *
 * @return The dot product of the two vectors: @f$(x_1*x_2+y_1*y_2+z_1*z_2)@f$.
 */
TReal PFLIB_API DotProduct(const TVec3& lhs, const TVec3& rhs);
/**
 * Cross product function.
 *
 * @relates TVec3
 *
 * @return The cross product of the two vectors.
 */
TVec3 PFLIB_API CrossProduct(const TVec3& lhs, const TVec3& rhs);


/**
 * A 4d vector class.
 *
 * This class is a POD with public member data.
 */

class PFLIB_API TVec4
{
public:
#if !USER_DOCS
	// traits
	enum { kDIM = 4 };
#endif

	// data members
    TReal x;    ///< X dimension.
    TReal y;    ///< Y dimension.
    TReal z;    ///< Z dimension.
	TReal w;    ///< W dimension.

	/// Constructor.
	TVec4();

    /**
     * Initializing constructor.
     *
     * @param X      X value (v[0]).
     * @param Y      Y value (v[1]).
     * @param Z      Z value (v[2]).
     * @param W      W value (v[3]).
     */
    TVec4(TReal X, TReal Y, TReal Z, TReal W);

    /**
     * Conversion from a TVec3.
     *
     * @param rhs    Source TVec3.
     * @param W      W component to add. Defaults to zero.
     */
    explicit TVec4(const TVec3& rhs, TReal W = 0.0); 	// implied w = 0

	/// Copy construction.
	TVec4(const TVec4& rhs);

    /**
     * Assignment.
     *
     * @return A reference to this.
     */
	TVec4& operator=(const TVec4& rhs);

    /**
     * Member accessor.
     *
     * @param i      Zero-based index.
     *
     * @return A reference to the i'th member.
     */
	TReal& operator[](TIndex i) 				{ return (&x)[i]; }

    /**
     * Member accessor.
     *
     * @param i      Zero-based index.
     *
     * @return A reference to the i'th member.
     */
	const TReal& operator[](TIndex i) const 	{ return (&x)[i]; }

    /**
     * The dimension of this vector (4).
     *
     * @return 4
     */
	TIndex Dim() const 							{ return kDIM; }
    /**
     * The length of this vector squared.
     *
     * @return @f$x^2+y^2+z^2+w^2@f$
     */
	TReal LengthSquared() const;
    /**
     * The length of this vector.
     *
     * @return @f$\sqrt{x^2+y^2+z^2+w^2}@f$
     */
	TReal Length() const;
    /**
     * Normalize this vector. Changes vector to @f$(x/Length(),y/Length(),z/Length(),w/Length())@f$
     *
     * @return A reference to this.
     */
	TVec4& Normalize();
//	TVec3 Projection();

    /**
     * Addition-assignment operator.
     *
     * @return A reference to this.
     */
	TVec4& operator+=(const TVec4& rhs);
    /**
     * Subtraction-assignment operator.
     *
     * @return A reference to this.
     */
	TVec4& operator-=(const TVec4& rhs);
//	TVec4& operator*=(const TVec4& rhs);
//	TVec4& operator/=(const TVec4& rhs);
    /**
     * Scaling operator.
     *
     * @param s Scale factor.
     *
     * @return A reference to this, scaled as @f$(x*s,y*s)@f$
     */
	TVec4& operator*=(TReal rhs);
    /**
     * Scaling operator.
     *
     * @param s Scale divisor.
     *
     * @return A reference to this, scaled as @f$(x/s,y/s)@f$
     */
	TVec4& operator/=(TReal rhs);

    /**
     * Unary negation.
     *
     * @return @f$(-x,-y,-z,-w)@f$
     */
	TVec4 operator-() const;
};

/**
 * Equality operator.
 *
 * @relates TVec4
 *
 * @return True if equal.
 */
bool PFLIB_API operator==(const TVec4& lhs, const TVec4& rhs);
/**
 * Inequality operator.
 *
 * @relates TVec4
 *
 * @return True if not equal.
 */
bool PFLIB_API operator!=(const TVec4& lhs, const TVec4& rhs);
/**
 * Addition operator.
 *
 * @relates TVec4
 *
 * @return @f$(x_1+x_2,y_1+y_2,z_1+z_2,w_1+w_2)@f$
 */
TVec4 PFLIB_API operator+(const TVec4& lhs, const TVec4& rhs);
/**
 * Subtraction operator.
 *
 * @relates TVec4
 *
 * @return @f$(x_1-x_2,y_1-y_2,z_1-z_2,w_1-w_2)@f$
 */
TVec4 PFLIB_API operator-(const TVec4& lhs, const TVec4& rhs);
//TVec4 operator*(const TVec4& lhs, const TVec4& rhs);
/**
 * Scaling operator.
 *
 * @relates TVec4
 *
 * @return @f$(x*s,y*s)@f$
 */
TVec4 PFLIB_API operator*(const TVec4& lhs, TReal s);
/**
 * Scaling operator.
 *
 * @relates TVec4
 *
 * @return @f$(x*s,y*s)@f$
 */
TVec4 PFLIB_API operator*(TReal s, const TVec4& rhs);			// make TReal * TVec4 commutative
//TVec4 operator/(const TVec4& lhs, const TVec4& rhs);
/**
 * Scaling operator.
 *
 * @relates TVec4
 *
 * @return @f$(x/s,y/s,z/s,w/s)@f$
 */
TVec4 PFLIB_API operator/(const TVec4& lhs, TReal rhs);
/**
 * Dot product function.
 *
 * @relates TVec4
 *
 * @return The dot product of the two vectors: @f$(x_1*x_2+y_1*y_2+z_1*z_2+w_1*w*2)@f$.
 */
TReal PFLIB_API DotProduct(const TVec4& lhs, const TVec4& rhs);
/**
 * Cross product function.
 *
 * @relates TVec4
 *
 * @return The cross product of the two vectors.
 */
TVec4 PFLIB_API CrossProduct(const TVec4& a, const TVec4& b, const TVec4& c);



/**
 * Detects if a ray intersects a triangle.
 *
 * @param[in] pvOrig	Points to ray origin.
 * @param[in] pvDir		Points to ray direction from origin.
 * @param[in] pv0		Points to triangle vertex0.
 * @param[in] pv1		Points to triangle vertex1.
 * @param[in] pv2		Points to triangle vertex2.
 * @param[out] pfDist	Points to where the distance from vOrig to the
 *                      point of intersection gets stored.
 * @param[out] pvHit    Points to where the actual point of intersection
 *                      gets stored.
 * @relates TVec3
 *
 * @return true if ray intersects triangle.
 */
bool PFLIB_API IntersectTriangle(	const TVec3 & pvOrig,
                                const TVec3 & pvDir,
                                const TVec3 & pv0,
                                const TVec3 & pv1,
                                const TVec3 & pv2,
                                TReal* pfDist,
                                TVec3* pvHit);


#endif
