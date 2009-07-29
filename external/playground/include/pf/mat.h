//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#if !defined(MAT_H_INCLUDED)
#define MAT_H_INCLUDED

#include "vec.h"

#include <cmath>

/**
 * 2d Matrix with 2x2 rotation component and TVec2 offset component.
 */
class PFLIB_API TMat3
{
public:
#if !USER_DOCS
	/// Dimensions
	enum { kDIM = 3 };
#endif

	/** @name Construction and Initialization.
	 * @{
	 */

	/// Default constructor.
	/// Initializes to identity.
	TMat3();

	/// Construct from individual values.
	TMat3(TReal m00, TReal m01, TReal m02,
		TReal m10, TReal m11, TReal m12,
		TReal m20, TReal m21, TReal m22);

	/// Construct from three vectors.
	TMat3(TVec3 v0, TVec3 v1, TVec3 v2);

	/// Copy construction.
	TMat3(const TMat3& rhs);

	/// Assignment.
	TMat3& operator=(const TMat3& rhs);

	/// Initialize to an identity.
	TMat3& Identity();
	/** @} */

	/** @name Accessors
	 * @{
	 */

	/// Array Accessor
	/// @param i The row number, from 0-2.
	/// @return a reference to the TVec3 that represents the row.
	TVec3& operator[](TIndex i)				{ return mData[i]; }
    /// Array Accessor
    /// @param i The row number, from 0-2.
    /// @return a reference to the TVec3 that represents the row.
	const TVec3& operator[](TIndex i) const	{ return mData[i]; }

	/// Dimensions of this array
	TIndex Dim() const 						{ return kDIM; }

	/** @} */

    /** @name Assignment Operators
	 * @{
	 */

    /**
     *  Assignment operator.
     *  @param rhs The right-hand-side of the assignment.
     *   @return a reference to this.
     */
	TMat3& operator+=(const TMat3& rhs);
    /**
     *  Assignment operator.
     *  @param rhs The right-hand-side of the assignment.
     *   @return a reference to this.
     */
	TMat3& operator-=(const TMat3& rhs);
    /**
     *  Assignment operator.
     *  @param rhs The right-hand-side of the assignment.
     *   @return a reference to this.
     */
	TMat3& operator*=(const TMat3& rhs);
    /**
     *  Assignment operator.
     *  @param rhs The right-hand-side of the assignment.
     *   @return a reference to this.
     */
	TMat3& operator*=(TReal rhs);
    /**
     *  Assignment operator.
     *  @param rhs The right-hand-side of the assignment.
     *   @return a reference to this.
     */
	TMat3& operator/=(TReal rhs);

	/** @} */

	/** @name Scaling and rotation
	 * @{
	 */

    /**
     * Scale this matrix in two dimensions. Assumes that this matrix
     * is used as a 2d matrix.
     *
     * @param x Scale in X dimension.
     * @param y Scale in Y dimension.
     *
     * @return this matrix.
     */
	TMat3& Scale( TReal x, TReal y );

    /**
     * Scale this matrix. Assumes this matrix is used as a 2d
     * matrix.
     *
     * @param s Amount to scale X and Y axes.
     *
     * @return this matrix.
     */
	TMat3& Scale( TReal s );

	/**
	 * Get a rotation matrix.
	 *
	 * @param radians Rotation in radians.
	 *
	 * @return A new matrix that represents the given rotation frame.
	 */
	static TMat3 GetRotation( TReal radians )
	{
		const TReal s = std::sin(radians);
		const TReal c = std::cos(radians);

		return TMat3(
			c, s,	0,
			-s, c,  0,
			0,	0,	1
		);
	}

	/**
	 * Rotate this matrix in place.
	 *
	 * @param radians Radians to rotate the matrix by. Only changes the orientation
	 *                portion of the matrix; position is unchanged.
	 *
	 * @return A reference to this matrix.
	 */
	TMat3& Rotate( TReal radians );

	/** @} */

private:
	TVec3 mData[kDIM];
};

/**
 * Equality.
 *
 * @relates TMat3
 *
 * @return True on equal.
 */
bool PFLIB_API operator==(const TMat3& lhs, const TMat3& rhs);
/**
 * Inequality.
 *
 * @relates TMat3
 *
 * @return True on not equal.
 */
inline bool PFLIB_API operator!=(const TMat3& lhs, const TMat3& rhs)
{
	return !(lhs == rhs);
}

/**
 * Matrix multiplication.
 *
 * @relates TMat4
 *
 * @return @f$M_1*M_2@f$
 */
TMat3 PFLIB_API operator*(const TMat3& lhs, const TMat3& rhs);

/**
 * Matrix multiplication with a vector.
 *
 * @relates TMat3
 *
 * @return @f$M*v@f$
 */
TVec3 PFLIB_API operator*(const TMat3& lhs, const TVec3& rhs);
/**
 * Matrix multiplication with a vector.
 *
 * @relates TMat3
 *
 * @return @f$M*v@f$
 */
TVec2 PFLIB_API operator*(const TMat3& lhs, const TVec2& rhs);
/**
 * Matrix multiplication with a vector.
 *
 * @relates TMat3
 *
 * @return @f$v^T*M@f$
 */
TVec3 PFLIB_API operator*(const TVec3& lhs, const TMat3& rhs);
/**
 * Matrix multiplication with a vector.
 *
 * @relates TMat3
 *
 * @return @f$v^T*M@f$
 */
TVec2 PFLIB_API operator*(const TVec2& lhs, const TMat3& rhs);

/**
 * Member-wise scaling of the vector. Note this won't necessarily
 * do what you want if you're trying to produce a scale
 * vector. See TMat3::Scale().
 *
 * @relates TMat3
 *
 * @return @f[
 * \left|
 * \begin{array}{ccc}
 * a_{00}*s & a_{10}*s & a_{20}*s \\
 * a_{01}*s & a_{11}*s & a_{21}*s \\
 * a_{02}*s & a_{12}*s & a_{22}*s
 * \end{array}
 * \right|
 * @f]
 */
TMat3 PFLIB_API operator*(const TMat3& lhs, TReal rhs);

/**
 * A restricted 2x2 matrix vector multiply.
 * Does the rotation/scaling but no translation of the vector.
 *
 * @relates TMat3
 *
 * @param lhs    Matrix left-hand-side of the multiply
 * @param rhs    Vector right-hand-side.
 *
 * @return Matrix[2x2]*Vector
 */
TVec2 PFLIB_API Multiply2x2(const TMat3& lhs,const TVec2& rhs);

/**
 * A restricted 2x2 matrix vector multiply.
 * Does the rotation/scaling but no translation of the vector.
 *
 * @param lhs    Vector left-hand-side of the multiply
 * @param rhs    Matrix right-hand-side.
 *
 * @return Vector*Matrix[2x2]
 *
 * @relates TMat3
 */
TVec2 PFLIB_API Multiply2x2(const TVec2& lhs,const TMat3& rhs);

/**
 * A restricted 2x2 matrix-matrix multiply.
 *
 * @param lhs    Matrix left-hand-side.
 * @param rhs    Matrix right-hand-side.
 *
 * @relates TMat3
 *
 * @return Matrix[2x2]*Matrix[2x2], with lhs[2] as the
 *         translation component.
 */
TMat3 PFLIB_API Multiply2x2(const TMat3& lhs,const TMat3& rhs);

/**
 * Restricted-multiply operator.
 *
 * @see Multiply2x2()
 * @relates TMat3
 *
 * @return Multiply2x2(lhs,rhs)
 */
inline TVec2 PFLIB_API operator%(const TVec2& lhs,const TMat3& rhs)
{
	return Multiply2x2(lhs,rhs);
}

/**
 * Restricted-multiply operator.
 *
 * @see Multiply2x2()
 * @relates TMat3
 *
 * @return Multiply2x2(lhs,rhs)
 */
inline TVec2 PFLIB_API operator%(const TMat3& lhs,const TVec2& rhs)
{
	return Multiply2x2(lhs,rhs);
}

/**
 * Restricted-multiply operator.
 *
 * @see Multiply2x2()
 * @relates TMat3
 *
 * @return Multiply2x2(lhs,rhs)
 */
inline TMat3 PFLIB_API operator%(const TMat3& lhs,const TMat3& rhs)
{
	return Multiply2x2(lhs,rhs);
}



/**
 * 3d Matrix with 3x3 rotation component and TVec3 offset component.
 */
class PFLIB_API TMat4
{
public:
#if !USER_DATA
    // traits
	enum { kDIM = 4 };
#endif

	/** @name Construction and Initialization.
	 * @{
	 */

	/**
	 * Default constructor. Initializes to identity.
	 */
	TMat4();

	/// Construct from individual values.
	TMat4(TReal m00, TReal m01, TReal m02, TReal m03,
		TReal m10, TReal m11, TReal m12, TReal m13,
		TReal m20, TReal m21, TReal m22, TReal m23,
		TReal m30, TReal m31, TReal m32, TReal m33);

	/// Construct from vectors.
	TMat4(TVec4 v0, TVec4 v1, TVec4 v2, TVec4 v3);

	/// Copy construction
    /// @param rhs  Copy source.
	TMat4(const TMat4& rhs);

    /// Assignment operator.
    /// @param rhs  Right hand side of the equation.
	TMat4& operator=(const TMat4& rhs);

	/// Destruction
	~TMat4();

    /** @} */

    /** @name Accessors
     * @{
     */

    /// Array Accessor
    /// @param i The row number, from 0-2.
    /// @return a reference to the TVec4 that represents the row.
	TVec4& operator[](TIndex i)				{ return mData[i]; }
    /// Array Accessor
    /// @param i The row number, from 0-2.
    /// @return a reference to the TVec4 that represents the row.
	const TVec4& operator[](TIndex i) const	{ return mData[i]; }
#if !USER_DOCS
    TIndex Dim() const 						{ return kDIM; }
#endif
    /** @} */

    /** @name Assignment operators.
     * @{
     */

	/// Assignment
	TMat4& operator+=(const TMat4& rhs);
	/// Assignment
	TMat4& operator-=(const TMat4& rhs);
	/// Assignment
	TMat4& operator*=(const TMat4& rhs);
	/// Assignment
	TMat4& operator*=(TReal rhs);
	/// Assignment
	TMat4& operator/=(TReal rhs);

    /** @} */

    /** @name Initializers.
     * @{
     */

    /// Make this matrix the identity.
    /// @return A reference to this.
    TMat4& Identity();

    /**
     * Change this matrix to be a view matrix that's oriented to
     * "look at" a point.
     *
     * @param pos    Viewer position.
     * @param at     Point we're looking at.
     * @param up     Local "up" vector.
     *
     * @return A reference to this.
     */
	TMat4& LookAt(const TVec3& pos, const TVec3& at, const TVec3& up);

    /**
     * Make this matrix a perspective matrix.
     *
     * @param nearPlane Distance from viewer to the near plane.
     * @param farPlane  Distance from viewer to the far plane. The smaller your ratio
     *                  of far to near, the higher Z-buffer resolution you get.
     * @param fov       The field of view in radians.
     * @param aspect    The aspect ratio of the resulting view.
     *
     * @return A reference to this.
     */
    TMat4& Perspective(TReal nearPlane, TReal farPlane, TReal fov, TReal aspect);

	/**
	 * Make this matrix an offset perspective matrix.
	 *
	 * In order to display a partially clipped 3d window (say
	 * you wanted to allow a window to scroll off the screen),
	 * you need to have a perspective matrix that has been
	 * skewed to display part of a larger projection.
	 *
	 * @param nearPlane Distance from viewer to the near plane.
	 * @param farPlane  Distance from viewer to the far plane. The smaller your ratio
	 *                  of far to near, the higher Z-buffer resolution you get.
	 * @param fov       The field of view in radians.
	 * @param aspect    The aspect ratio of the resulting view.
     * @param offsets   The amounts to skew the x and y portions of
     *                  the matrix. If you want to clip 20% of the
     *                  left edge, you'd set offsets.x to 0.2.
     *                  Similarly if you want to clip 20% of the
     *                  bottom edge, you'd set y to -0.2.
	 *
	 * @return A reference to this.
	 */
	TMat4& OffsetPerspective(TReal nearPlane, TReal farPlane, TReal fov, TReal aspect, const TVec2 & offsets);

    /**
     * Make this matrix an orthogonal projection matrix that
     * transforms objects within the given box into view
     * coordinates.
     *
     * @param left   Left side of box.
     * @param right  Right side of box.
     * @param bottom Bottom of box.
     * @param top    Top of box.
     * @param zNear  Near side of box.
     * @param zFar   Far side of box.
     *
     * @return A reference to this.
     */
    TMat4& Orthogonal(TReal left, TReal right, TReal bottom, TReal top, TReal zNear, TReal zFar);

    /** @} */

    /** @name Manipulators
     * @{
     */

    /**
     * Rotate this matrix around an axis.
     *
     * @param axis  A unit vector that defines an axis to rotate
     *              around.
     * @param a     Number of radians to rotate matrix.
     *
     * @return A reference to this.
     */
	TMat4& RotateAxis(const TVec3& axis, TReal a);

    /**
     * Rotate this matrix around the X axis.
     * @param a     Number of radians to rotate matrix.
     *
     * @return A reference to this.
     */
	TMat4& RotateX(TReal a);

    /**
     * Rotate this matrix around the Y axis.
     * @param a     Number of radians to rotate matrix.
     *
     * @return A reference to this.
     */
	TMat4& RotateY(TReal a);

    /**
     * Rotate this matrix around all three axes, given as Yaw,
     * Pitch, and Roll.
     *
     * @param yaw   Number of radians to rotate matrix around Y
     *              axis.
     * @param pitch Number of radians to rotate matrix around X
     *              axis.
     * @param roll  Number of radians to rotate matrix around Z
     *              axis.
     *
     * @return A reference to this.
     */
    TMat4& RotateYPR(TReal yaw, TReal pitch, TReal roll);

    /**
     * Rotate this matrix around the Z axis.
     * @param a     Number of radians to rotate matrix.
     *
     * @return A reference to this.
     */
	TMat4& RotateZ(TReal a);

    /**
     * Scale this matrix
     *
     * @param x     Scale to apply to X axis.
     * @param y     Scale to apply to Y axis.
     * @param z     Scale to apply to Z axis.
     *
     * @return      A reference to this.
     */
    TMat4& Scale(TReal x, TReal y, TReal z);

    /**
     * Translate this matrix by a vector.
     *
     * @param v      Amount to translate.
     *
     * @return A reference to this.
     */
	TMat4& Translate( const TVec3 & v );

    /** @} */

    /** @name Utility Mmbers
     * @{
     */

    /**
     * Return a transposed matrix.
     *
     * @return A new transposed matrix.
     */
	TMat4 Transpose() const;

    /// Matrix adjoint function.
	TMat4 Adjoint() const;

    /**
     * Return an inverse of the matrix.
     *
     * @return An inverse copy of the matrix.
     */
	TMat4 Inverse() const;

    /**
     * Calculate the determinant of the matrix.
     *
     * @return The determinant.
     */
	TReal Determinant() const;

    /** @} */

private:
	TVec4 mData[kDIM];
};

typedef TMat4 TMatrix;	//rls temporary


/**
 * Equality.
 *
 * @relates TMat4
 *
 * @return True on equal.
 */
bool PFLIB_API operator==(const TMat4& lhs, const TMat4& rhs);
/**
 * Inequality.
 *
 * @relates TMat4
 *
 * @return True on not equal.
 */
inline bool PFLIB_API operator!=(const TMat4& lhs, const TMat4& rhs)
{
	return !(lhs == rhs);
}

/**
 * Matrix multiplication.
 *
 * @relates TMat4
 *
 * @return @f$M_1*M_2@f$
 */
TMat4 PFLIB_API operator*(const TMat4& lhs, const TMat4& rhs);
/**
 * Matrix multiplication with a vector.
 *
 * @relates TMat4
 *
 * @return @f$M*v@f$
 */
TVec4 PFLIB_API operator*(const TMat4& lhs, const TVec4& rhs);
/**
 * Matrix multiplication with a vector.
 *
 * @relates TMat4
 *
 * @return @f$M*v@f$
 */
TVec3 PFLIB_API operator*(const TMat4& lhs, const TVec3& rhs);
/**
 * Matrix multiplication with a vector.
 *
 * @relates TMat4
 *
 * @return @f$M*v@f$
 */
TVec4 PFLIB_API operator*(const TVec4& lhs, const TMat4& rhs);
/**
 * Matrix scaling.
 *
 * @relates TMat4
 *
 * @return @f[
 * \left|
 * \begin{array}{cccc}
 * a_{00}*s & a_{10}*s & a_{20}*s & a_{30}*s \\
 * a_{01}*s & a_{11}*s & a_{21}*s & a_{31}*s \\
 * a_{02}*s & a_{12}*s & a_{22}*s & a_{32}*s \\
 * a_{03}*s & a_{13}*s & a_{23}*s & a_{33}*s
 * \end{array}
 * \right|
 * @f]
 */
TMat4 PFLIB_API operator*(const TMat4& lhs, TReal s);
/**
 * Matrix scaling.
 *
 * @relates TMat4
 *
 * @return @f$ (1/s)*M @f$
 */
TMat4 PFLIB_API operator/(const TMat4& lhs, TReal s);
/**
 * Matrix multiplication with a vector.
 *
 * @relates TMat3
 *
 * @return @f$M*v@f$
 */
TVec3 PFLIB_API operator*(const TMat4& lhs, const TVec3& rhs);
/**
 * Matrix multiplication with a vector.
 *
 * @relates TMat3
 *
 * @return @f$v^T*M@f$
 */
TVec3 PFLIB_API operator*(const TVec3& lhs, const TMat4& rhs);

TReal PFLIB_API DegreesToRadians(TReal a);
TReal PFLIB_API RadiansToDegrees(TReal a);

inline TReal DegreesToRadians(TReal a)
{
	return ((a * 3.141592654f) / 180.0f);
}

inline TReal RadiansToDegrees(TReal a)
{
	return ((a * 180.0f) / 3.141592654f);
}



#endif
