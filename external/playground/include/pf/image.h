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
 * @ifnot BOOK
 * @condfile
 * Interface for class TImage
 * \ingroup windowgroup
 * @endif
 */

#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#ifndef WINDOW_H_INCLUDED
#include "window.h"
#endif

#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

/**
 * The TImage class is a TWindow that contains and draws a TTexture.
 *
 * It is not intended that the windowing system be used to
 * render sprites in your game--that's what the sprite system is
 * for. Among other things, there is a limited flexibility in
 * rendering options.
 *
 * A TImage will automatically set its kOpaque flag based on
 * SetAlpha()--a SetAlpha() of less than 1 will reset the flag.
 * In Lua initialization you can also use "alpha=true" to
 * reset the flag.
 *
 * \ingroup windowgroup
 */
class PFLIB_API TImage : public TWindow
{
	PFTYPEDEF_DC(TImage,TWindow);
public:
	/**
	 * Default Constructor.
	 *
	 * @param staticImage
	 *               Default behavior assumes that the image will change
	 *               infrequently, and therefore sets kInfrequentChanges and
	 *               tries to cache the image.
	 *
	 *               Create a TImage with staticImage set to false for an image
	 *               that will change frequently.
	 */
	TImage(bool staticImage=true);

	/// Destructor
	virtual ~TImage();

	/**
	 * @name Event Handlers
	 *
	 *  Functions to override to handle events in a window.
	 *
	 * @{
	 */

	/**
	 * Draw the window. TImage can draw only a portion of the window when only
	 * a part of the image needs to be redrawn.
	 */
	virtual void Draw();

	/** @} */

	/**
	 * Set the image texture.
	 *
	 * @param texture New texture.
	 * @param scale Amount to scale texture by when drawing
	 */
	void SetTexture( TTextureRef texture, TReal scale = 1.0f );


	/**
	 * Get the current image texture.
	 *
	 * @return A reference to the bound image.
	 */
	TTextureRef GetTexture()
	{
		return mTexture ;
	}

	/**
	 * Set the alpha for this image
	 *
	 * @param alpha  Alpha value. 1.0==opaque.
	 */
	void SetAlpha( TReal alpha );

	/**
	 * Get the current alpha of this image.
	 * 
	 * @return A value from 0 (transparent) to 1 (opaque).
	 */
	TReal GetAlpha() { return mAlpha; }

	/**
	 * Get the current scale of this TImage
	 *
	 * @return Scale from 0 - 1
	 */
	TReal GetScale() { return mScale ; 	}

	/**
	 * Set this image to be rotated right (clockwise) by
	 * 90 degrees.
	 *
	 * @param rotate True to rotate.
	 */
	void SetRotate( bool rotate );

	/**
	 * Set the draw flags for this image (including flip
	 * horizontal and flip vertical).
     *
     * @param flags  Draw flags to use:  A combination of
     *               TDrawSpec::kFlipVertical and/or
     *               TDrawSpec::kFlipHorizontal.
	 */
	void SetDrawFlags( uint32_t flags );

	virtual void Init(TWindowStyle &style);
private:
	TTextureRef		mTexture ;
	TReal			mScale;
	TReal			mAlpha;
	uint32_t		mDrawFlags ;
	bool			mRotate;
};

#endif // IMAGE_H_INCLUDED
