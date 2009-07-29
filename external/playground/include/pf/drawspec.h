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
 * Definition of class TDrawSpec
 */

#ifndef DRAWSPEC_H_INCLUDED
#define DRAWSPEC_H_INCLUDED

#ifndef VEC_H_INCLUDED
#include "vec.h"
#endif

#ifndef MAT_H_INCLUDED
#include "mat.h"
#endif

#ifndef RENDERER_H_INCLUDED
#include "renderer.h"
#endif

/**
 * 2d drawing parameters for use in DrawSprite.
 */
class TDrawSpec
{
public:
	static const int32_t kFlipHorizontal = 1<<0 ;	///< Flip texture horizontally.
	static const int32_t kFlipVertical   = 1<<1 ;	///< Flip texture vertically.
	static const int32_t kUseSourceRect  = 1<<2 ;	///< Use the source rectangle field.
	/// Use the center field.
	/// @warning If you set kUseCenter on the TDrawSpec of a TAnimatedSprite,
	/// you will disable the automatic positioning of the TAnimatedSprite, and the
	/// animation will no longer work as expected.
	static const int32_t kUseCenter		 = 1<<3 ;

	/**
	 * Default constructor with optional arguments for convenience.
	 *
	 * @param at     Position to draw the object.
	 * @param alpha  Alpha level of the object, 0-1.
	 * @param scale  The scale of the object, where 1.0 is its actual size.
     * @param flags  Options flags. See mFlags.
	 */
	TDrawSpec( const TVec2 & at=TVec2(), TReal alpha=1, TReal scale=1, uint32_t flags=0 ) :
		mCenter(0,0),
		mTint(1,1,1,1),
		mAlpha(alpha),
		mSourceRect(0,0,0,0),
		mFlags(flags),
		mBlendMode(TRenderer::kBlendINVALID)
	{
		if (scale!=1)
		{
			mMatrix.Scale(scale);
		}
		// Don't scale the position!
		mMatrix[2] = TVec3(at,1);
	}

	/**
	 * Create a new TDrawSpec that is a combination of this TDrawSpec
     * and a parent reference frame.
     *
     * The new TDrawSpec will have a combined matrix, combined
     * alpha, and inherited relative flip flags (kFlipVertical +
     * kFlipVertical = no flip, same for kFlipHorizontal). Blend
     * modes are inherited only if this TDrawSpec is set to
     * kBlendINVALID.
     *
     * Other parameters stay the same as this class and are NOT
     * inherited.
	 *
	 * @param parent Parent to combine
	 *
	 * @return New combined TDrawSpec
	 */
	TDrawSpec GetRelative( const TDrawSpec & parent ) const
	{
		TDrawSpec drawSpec(*this);

		drawSpec.mMatrix = drawSpec.mMatrix * parent.mMatrix  ;
		drawSpec.mAlpha *= parent.mAlpha ;
		drawSpec.mFlags ^= (parent.mFlags)&(kFlipVertical|kFlipHorizontal);
		if (drawSpec.mBlendMode==TRenderer::kBlendINVALID)
		{
			drawSpec.mBlendMode = parent.mBlendMode ;
		}

		return drawSpec ;
	}

	// Comments:
	// * Non-experts have a hard time with bitwise flags, so we should really use bools.
	// * Center and source rect are likely to be done wrong; they require careful
	//   reading of the docs to get right. Therefore, I think those should be private
	//   and have accessors (Get/Set/Clear).
	//
public:

    /**
     *  3x3 matrix including the offset and relative orientation
     *  (and scaling) of the sprite.
	 *
     * Use mMatrix[2] to set the position where the center of sprite is to be drawn.
     *
     * Use mMatrix.Scale() to set the scale of the sprite.
	 */
	TMat3 	mMatrix;

	/**
     * Logical center of the texture in pixels; can be outside of
     * the texture. Drawing of the texture will be done relative to
     * this point: If this point is (0,0), it will draw relative to
     * the upper left corner of the image, for instance.
     *
     * This is <b>not</b> the parameter to use to set the position
     * of an object; for that, see TDrawSpec::mMatrix. If you misuse
     * the mCenter field to position a sprite, you will likely run
     * into problems with inherited positions and TAnimatedTexture
     * drawing. The mCenter field isn't inherited (as it's intended
     * to specify the reference point of <b>this</b> sprite), and
     * TAnimatedTexture uses mCenter internally to properly position
     * the animation after sidewalk has cropped all of the frames.
     *
     * This parameter is <b>not</b> inherited when used with
     * TSprite::Draw(), so if you want to modify the center of a
     * sprite you need to modify the sprite's TSprite::GetDrawSpec()
     * and not the environment passed in to TSprite::Draw().
	 *
     * mFlags must have kUseCenter set to cause this field to be
     * valid. Otherwise the actual center of the texture is used.
     *
     * @see kUseCenter
	 */
	TVec2	mCenter;

	/**
     * Vertex coloring for the texture. The alpha channel of
     * this color is ignored. Default value on construction is pure
     * white.
     *
     * Applies only to this object--for sprites, does NOT get
     * inherited by child sprites.
	 */
	TColor 	mTint;

    /**
     *  Alpha to use to draw sprite. Zero is completely transparent
     *  and one is completely opaque (subject to blend mode and
     *  texture alpha values).
     *
     *  This value is multiplicatively inherited in child sprites
     *  using GetRelative().
	 */
	TReal	mAlpha;

	/**
     * Source rectangle to extract image from.
     *
     * This parameter is NOT inherited when used with
     * TSprite::Draw(), so if you want to modify the source
     * rectangle of a sprite you need to modify the sprite's
     * TSprite::GetDrawSpec() and not the environment passed in to
     * TSprite::Draw().
     *
     * mFlags must have kUseSourceRect set to activate.
	 */
	TRect	mSourceRect;

	/**
     * Options for drawing. Inherits flip flags with
     * an XOR, so if a parent and child are, e.g., both horizontally
     * flipped, the child will be drawn normally.
     *
     * Options include \ref kFlipHorizontal, \ref kFlipVertical,
     * \ref kUseSourceRect, and \ref kUseCenter.
	 */
	uint32_t	mFlags;

	/**
     * Blend mode to use, if any; set to TRenderer::kBlendINVALID to
     * use current mode (default). Inherited when parent mode set
     * and child is kBlendINVALID.
	 */
	TRenderer::EBlendMode mBlendMode;

};

#endif
