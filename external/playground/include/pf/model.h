//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include "str.h"

#ifndef ASSET_H_INCLUDED
#include "asset.h"
#endif

#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

#ifndef TAGGING_IGNORE
class TModelData;
struct TVert;
#endif
/**
 * @condfile
 * Interface for TModel.
 */

/**
 * A 3d model.
 */
class PFLIB_API TModel : public TAsset
{
	PFSHAREDTYPEDEF(TAsset);
private:
	/// Private constructor since we're using factory methods.
	TModel();
public:
	virtual ~TModel();

	/**
	 * Draw the model using the current texture, material, and
	 * transformation matrix.
	 *
	 * Any texture used with TModel::Draw must be square and have dimensions that are
	 * powers of two.
	 */
	void        Draw();

	/**
	 * Cast a ray at the model and determine whether it's been hit.
	 * Uses current transformation matrix to position model.
	 *
	 * @param iWndX      X in TScreen coordinates.
	 * @param iWndY      Y in TScreen coordinates.
	 * @param pfPickDist Pointer to a float that starts out initialized to the maximum
	 * 					 distance the cast ray should collide with polygons. On return
	 *					 contains the actual distance from the
	 *                   screen to the model.
	 * @param pvHit      The 3d point where the cast ray intersects the model.
	 *
	 * @return True if model hit; false otherwise.
	 */
	bool        Pick(uint32_t iWndX, uint32_t iWndY, TReal *pfPickDist, TVec3* pvHit);

	/**
	 * Get the name of the asset as it was created.
	 *
	 * @return The name of the asset.
	 */
	str			GetName();

	/**
     * Get the number of polygons (triangles) in the model
     * @deprecated This function will be replaced by
     *             GetTriangleCount().
	 * @return Number of triangles.
	 */
	long        GetPolyCount();

	/**
	 * Get the number of triangles in the triangle array.
	 *
	 * @return Number of triangles.
	 */
	uint32_t    GetTriangleCount() { return GetPolyCount();}

	/**
	 * Get a pointer to an array of uint16_t values triples that
	 * indicate vertex indices that define triangles.
	 *
	 * @return A pointer to triangle indices.
	 */
	const uint16_t* GetTriangles();

	/**
	 * Get an array of the model's vertices.
	 *
     * @return A pointer to an array of TVert vertices.
     * @see GetVertexCount
	 */
	const TVert	*GetVertices();

	/**
	 * Get the number of vertices in the model.
	 *
     * @return A count of vertices in the vertex array.
     * @see GetVertices
	 */
	uint32_t	GetVertexCount();

	TModelRef	GetRef()
	{
		return static_pointer_cast<TModel>( shared_from_this() );
	}

public:

	/**
	 * Factory
	 *
     * @param assetName Asset id of the model
	 *
	 * @return A TModelRef of the model
	 */
	static TModelRef	Get( str assetName );

public:
	/// Implemenation details.
	TModelData* mData;
private:
	void        Free();
	bool        Init(const char* filename);
protected:
	virtual void Restore();
	virtual void Release();
};

//------------------------------------------------------------------

#endif // MODEL_H_INCLUDED
