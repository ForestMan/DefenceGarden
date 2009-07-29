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
 * Interface for class TVertexSet
 */


#ifndef VERTEXSET_H_INCLUDED
#define VERTEXSET_H_INCLUDED

#ifndef PLATFORM_H_INCLUDED
#include "platform.h"
#endif

#ifndef TAGGING_IGNORE
class TVertexSetData;
#endif

/**
 * \addtogroup vertextypes Vertex Support for Triangle Rendering
 *
 * When rendering to triangles or lines using
 * TRenderer::DrawVertices(), you need to set up your vertices
 * using functionality provided in this section.
 *
 * @{
 */

/**
 * 3d untransformed, unlit vertex.
 */
struct TVert
{
	TVec3	pos ;		///< Position in 3d space.
	TVec3	normal ;	///< Vertex normal. Must <i>not</i> be (0,0,0), and must be normalized.
	TVec2	uv ;		///< Vertex texture coordinate.
};

/**
 * 3d untransformed, lit vertex.
 */
struct TLitVert
{
	TVec3	pos ;       ///< Position in 3d space.
	uint32_t RESERVED;	///< UNUSED (must be zero).
	TColor32 color ;	///< Vertex color.
	TColor32 specular ;	///< Vertex specular component.
	TVec2	uv ;        ///< Vertex texture coordinate.
};

/**
 * 2d Transformed and lit vertex. Should be used with Begin2d()
 * operations.
 */
struct TTransformedLitVert
{
	TVec3	pos ; 		///< Position in screen coordinates.
	TReal	rhw ; 		///< RESERVED; must set to 0.5F.
    TColor32 color;     ///< Vertex color.
    TColor32 specular;	///< Vertex specular component.
	TVec2	uv ;		///< Vertex texture coordinate.
};

#if !USER_DOCS

/**
 * Pre-scaled transformed and lit vertex.
 *
 * Unlike a TTransformedLitVert, the
 * TPreScaledTransformedLitVert has been prescaled to the actual
 * screen dimensions and coordinates. This includes scaling down
 * to take into account being in a reduced-window-size ActiveX
 * control.
 *
 * Primarily used for rendering text to the screen, since text
 * needs to know what pixels it's actually rendering to in order
 * to draw correctly.
 *
 * @internal
 * @nouser
 */
struct TPreScaledTransformedLitVert : public TTransformedLitVert
{

};

#endif

/**
 * A helper/wrapper for the \ref vertextypes "Vertex Types" which allows
 * TRenderer::DrawVertices to identify the vertex type being
 * passed in without making the vertex types polymorphic.
 */
class PFLIB_API TVertexSet
{
public:

#if !USER_DOCS
	/**
	 * @nouser
	 * @internal
	 */
	enum EVertexType { kVertex, kLitVertex, kTransformedLitVertex, kPreScaledTransformedLitVertex };

	/**
	 * @nouser
	 * @internal
	 *
	 * Create a vertex set from an existing external array of
	 * TTransformedLitVerts. These are treated as pre-scaled by
	 * the Platform viewport scale, which is used in ActiveX mode
	 * to scale the graphics to a smaller screen size.
	 *
	 * @param v      Pointer to array of vertices.
	 * @param count  Number of vertices in array.
	 */
	TVertexSet(TPreScaledTransformedLitVert * v, uint32_t count);
#endif

	/// A hard limit on the number of vertices that you can specify.
	static const uint32_t kMaxVertices = 65535;

	/**
	 * Create a vertex set from an existing external array of
	 * TTransformedLitVerts. Does NOT copy the vertices--only
	 * keeps a pointer to them.
	 *
	 * @param v      Pointer to array of vertices.
     * @param count  Number of vertices in array. Must not be more
     *               than kMaxVertices.
	 */
	TVertexSet(TTransformedLitVert * v, uint32_t count);
	/**
	 * Create a vertex set from an existing external array of
	 * TLitVerts. Does NOT copy the vertices--only
	 * keeps a pointer to them.
	 *
	 * @param v      Pointer to array of vertices.
     * @param count  Number of vertices in array. Must not be more
     *               than kMaxVertices.
	 */
	TVertexSet(TLitVert * v, uint32_t count);
	/**
	 * Create a vertex set from an existing, external array of
	 * TVerts. Does NOT copy the vertices--only
	 * keeps a pointer to them.
	 *
	 * @param v      Pointer to array of vertices.
     * @param count  Number of vertices in array. Must not be more
     *               than kMaxVertices.
	 */
	TVertexSet(TVert * v, uint32_t count);

	/**
	 * Copy construction: Needs to be implemented because of an
	 * inane rule in the ISO standard.
	 *
	 * See https://developer.playfirst.com/node/158
	 * for details.
	 */
	TVertexSet( const TVertexSet & );

	/**
	 * Assignment
	 *
	 * @return A reference to this
	 */
	TVertexSet & operator=( const TVertexSet & );

/// Destructor
	~TVertexSet();

	/**
	 * Change the vertex count. This allows you to create a single
	 * TVertexSet and reuse its data repeatedly without reconstructing it.
	 *
	 * Maximum count is kMaxVertices.
	 *
	 * @param count  Number of vertices in the set.
	 */
	void			SetCount(uint32_t count);

#if !USER_DOCS
    /**
     * @internal
     * Private internal data.
     */
	TVertexSetData * mData ;
#endif
};


/**
 * Create some vertices based on a rectangle and some transformation
 * information. This is a utility function that is used by several
 * parts of the library internally, but has been exposed because of
 * its general usefulness.
 *
 * The resulting vertices can be passed to
 * TRenderer::DrawVertices() with the rendering type of
 * TRenderer::kDrawTriFan.
 *
 * @param pos      Position on screen to anchor rectangle.
 * @param vertices An array of four vertices to fill.
 * @param corners  An array of four corners (first is upper left, then clockwise) relative to
 *                 x,y that define the rectangle to create vertices for.
 *                 Will be transformed by CreateVertsFromRect.
 * @param uv       An array of two uv coordinates (upper left/lower right).
 * @param matrix   A transformation matrix to apply to the rectangle.
 * @param alpha    An alpha value to encode in the vertices.
 * @param tint     A tint value to encode into the vertices.
 */
void PFLIB_API CreateVertsFromRect(const TVec2 & pos, TTransformedLitVert *vertices, TVec2 * corners, const TVec2 * uv, const TMat3 & matrix, float alpha, const TColor &tint);

/** @} */

#endif // VERTEXSET_H_INCLUDED
