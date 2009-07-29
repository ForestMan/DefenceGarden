//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef ANIMATEDTEXTURE_H_INCLUDED
#define ANIMATEDTEXTURE_H_INCLUDED

/** @condfile */

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#ifndef DEBUG_H_INCLUDED
#include "debug.h"
#endif

#ifndef ASSET_H_INCLUDED
#include "asset.h"
#endif

#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

#ifndef CLOCK_H_INCLUDED
#include "clock.h"
#endif

#ifndef TEXTURE_H_INCLUDED
#include "texture.h"
#endif

#ifndef TAGGING_IGNORE
struct lua_State ;	// A Lua script context.
class TAnimatedTextureData;
#endif

/**
 * This class encapsulates the concept of an animated texture.
 * This kind of texture is placed directly in video RAM, so be
 * conservative about how large you allow concurrent game
 * animations to become: Consider that a 1024x1024x32 bit
 * texture takes up 4Mb of video RAM, so if you are trying to
 * display more than 10 such textures simultaneously you may start
 * getting performance problems as textures are swapped in and
 * out of video RAM.
 *
 * A TAnimatedTexture loads in an xml (or anm) file describing
 * the animation, and an image file with multiple source frames.
 *
 * Additionally, the xml file can describe hot spots, where other textures can be attached.
 *
 * A Lua script can be embedded in the TAnimatedTexture which
 * can be played back by a TAnimatedSprite. The TAnimatedTexture
 * doesn't have playback capability itself because it represents
 * the raw texture object, and you can have multiple instances
 * of it on the screen at once.
 *
 * When you use TTexture::Lock() on a TAnimatedTexture, you will
 * lock the full texture surface--which means that
 * TAnimatedTexture::GetWidth() and
 * TAnimatedTexture::GetHeight() will return the wrong values.
 * Instead, you should call TTexture::GetWidth() and
 * TTexture::GetHeight(). For example:
 * @code
 *
 * TAnimatedTextureRef at = ...;
 *
 * uint32_t w = at->TTexture::GetWidth() ; // This gives you the real texture width
 *
 * @endcode
 */
class PFLIB_API TAnimatedTexture : public TTexture
{
	PFSHAREDTYPEDEF(TTexture);
protected:
	/// Construction is through the factory method
	TAnimatedTexture();

private:
	/// Hiding functions that wouldn't do what you wanted anyway
	static TTextureRef Create( uint32_t width, uint32_t height, bool alpha );
	static TTextureRef CreateSimple( uint32_t width, uint32_t height );
	static TTextureRef CreateRenderTargetTexture( uint32_t width, uint32_t height );
	static TTextureRef GetMerged( str colorAssetName, str alphaAssetName, uint32_t flags=eDefaultAlpha );

public:
	enum
	{
		kNoFrame = -1
	};

public :
	/**
	 * @name Factory Methods/Destruction
	 * @{
	 */

	/**
     * Get a TAnimatedTextureRef given an animation name. The
     * animation name should be an xml or anm file describing the
     * animation.
	 *
     * @param assetName Name of the xml or anm file which contains
     *                  the animation data. You can leave off the
     *                  extension and it will search first for .xml,
     *                  and then for .anm.
	 * @param imageOverride Optional image file name to load for this texture, instead of
	 *						using image named in the xml file
	 * @param maskOverride Optional mask file name to load for this texture, instead of
	 *						using mask named in the xml file
	 *
     * @return A TAnimatedTextureRef. This ref will be NULL if it is
     *         unable to load.
	 */
	static TAnimatedTextureRef Get( str assetName, str imageOverride = "", str maskOverride = "" );

	/**
	 * Get the handle that an animated texture will be registered in
	 * the global asset manager as.
	 *
	 * @param assetName Name of asset.
	 * @param imageOverride
	 *                  Image Override (if any)
	 * @param maskOverride
	 *                  Mask Override (if any)
	 *
	 * @return A string that represents the handle of the object.
	 */
	static str GetHandle( str assetName, str imageOverride="", str maskOverride="" );

	/// Destructor
	virtual ~TAnimatedTexture();
	/** @} */


public:
	/**
     * @name Drawing Methods
	 * @{
	 */

	/**
     * Delegates to TTexture::CopyPixels(), which means that it will
     * copy from the multi-frame source image, not from the
     * individual frame.
	 *
	 * @param x      Left side of resulting rectangle.
	 * @param y      Top edge of resulting rectangle.
	 * @param sourceRect Source rectangle to blit. NULL to blit the entire surface.
     * @param _dst   Destination texture. NULL to draw to back buffer.
     *
     * @see TTexture::CopyPixels
	 */
	virtual void CopyPixels(int32_t x, int32_t y, const TRect * sourceRect, TTextureRef _dst);

	/**
	 * Draw a normal texture to a render target surface as a sprite. This
	 * draws a texture with optional rotation and scaling. Only capable of
	 * drawing an entire surface--not a sub-rectangle.
	 *
	 * Will draw the sprite within the currently active viewport. X and Y are
	 * relative to the upper left corner of the current viewport.
	 *
	 * DrawSprite can be called inside TWindow::Draw() or a BeginRenderTarget/EndRenderTarget block.
	 *
     * @param x  	 x Position where center of sprite is to be
     *               drawn.
     * @param y  	 y Position where center of sprite is to be
     *               drawn.
	 * @param alpha  Alpha to apply to the entire texture. Set to a negative
	 *               value to entirely disable alpha during blit, including
	 *               alpha within the source TTexture.
	 * @param scale  Scaling to apply to the texture. 1.0 is no scaling.
	 * @param rotRad Rotation in radians.
	 * @param flags  Define how textures are drawn. Use ETextureDrawFlags for the
	 *               flags. Default behavior is eDefaultDraw.
	 */
	 virtual void DrawSprite(TReal x, TReal y, TReal alpha, TReal scale, TReal rotRad, uint32_t flags);

	/**
	 * Draw an animated texture. Draws the current frame of the animated texture.
     *
     * @param drawSpec The TDrawSpec to use to draw the sprite.
     *
     * @see TTexture::DrawSprite
	 */
	virtual void DrawSprite(const TDrawSpec & drawSpec);

	/** @} */

	/**
     * @name Frame Sequence Information/Update
	 * @{
	 */

	/**
	 * Get number of frames in this animation
	 *
	 * @return Number of frames in animation.
	 */
	uint32_t GetNumFrames();

	/**
     * Set the current frame of the animation.
	 *
     * @param frame	Frame to set animation to, from 0 to n-1, where
     *              n is the number of frames.
	 */
	void SetCurrentFrame(int32_t frame);

	/**
	 * Get the current frame of the animation
	 *
     * @return frame 	Current frame of animation, from 0 to n-1,
     *              where n is the number of frames.
	 */
	int32_t GetCurrentFrame();

    /**
     * Get a frame by the frame's name
     *
     * @param name   Name of frame to retrieve.
     *
     * @return frame    Frame number, or -1 if it doesn't exist;
     */
	int32_t GetFrameByName(str name);

	/**
	 * Get the bounding rectangle for this animation.
	 *
	 * @returns a TRect that would contain the entire animation.
	 */
	TRect GetAnimationBoundingBox();

    /**
     * Get the bounding rectangle for a specific animation frame
     *
     * @returns a TRect that would contain the current frame of the animation
     * @param frame  Frame to query.
     *
     * @return The bounding box of this frame.
     */
	TRect GetBoundingBox(int32_t frame);

	/**
     * Get the location of a specific frame inside the animation texture.
     *
     * @returns a TRect that would contain the frame inside the animation texture
     * @param frame  Frame to query.
     *
     * @return The location of this frame inside the animation texture.
     */
	TRect GetFrameRect(int32_t frame);

	/**
	 * Get the alpha value associated with a frame.
	 *
	 * @param frame  Frame to retrieve the alpha value from.
	 *
     * @return 0.0 (transparent) - 1.0 (opaque).
	 */
	TReal GetFrameAlpha( int32_t frame);

    /** @} */

	/**
     * @name Anchor Point Support (not needed for most animations).
     *
     * Anchor points are used to track animated offsets; for
     * instance, the hand of a character that needs to have an
     * object attached might get an animated anchor.
     *
     * @{
	 */

	/**
	 * Get anchor point information for the current frame of the animation
	 *
	 * @param name Name of anchor point to retrieve
	 * @param x	[out] Fills in with x coordinate of anchor point if it exists
	 * @param y	[out] Fills in with y coordinate of anchor point if it exists
	 *
	 * @return true if the anchor point exists, false otherwise
	 */
	bool GetAnchorPoint(str name, int32_t *x, int32_t *y);

    /**
     * Get anchor point information for the selected frame of the
     * animation
     *
     * @param frame Frame index of frame to query.
	 * @param name Name of anchor point to retrieve
     * @param x	[out] Fills in with x coordinate of anchor point if it exists
     * @param y	[out] Fills in with y coordinate of anchor point if it exists
     *
     * @return true if the anchor point exists, false otherwise
     */
	bool GetAnchorPoint(int32_t frame, str name, int32_t *x, int32_t *y);

	/**
	 * Get number of anchors in this frame of animation
	 *
	 * @return Number of anchors in this frame of animation.
	 */
	uint32_t GetNumAnchors();

	/**
	 * Get anchor information
	 *
	 * @param anchorNum Which anchor to fetch
     * @param x	[out] Fills in with x coordinate of anchor point if it
     *          exists
	 * @param y	[out] Fills in with y coordinate of anchor point if it exists
     * @param name [out] Fills in name of anchor
	 *
	 *
	 * @return true if anchorNum is valid, false otherwise
	 */
	bool GetAnchorInfo(int32_t anchorNum, int32_t *x, int32_t *y, str *name);

    /** @} */

	/**
     * @name Image and Frame Data
     *
     * The data that specifies how each frame needs to be rendered.
     *
     *  @{
	 */

	/**
	 * Get the initial image registration point.
	 *
	 * The registration point is point from which the image coordinates are
	 * calculated.
	 *
	 * @return Registration point
	 */
	TPoint GetRegistrationPoint();

	/**
	 * Get the registration point of a frame.
	 *
	 * @param frame  Frame number
	 * @return Registration point
	 */
	TPoint GetRegistrationPoint(int32_t frame);

	/**
	 * Get the width of the texture. This gets the size of the texture as requested at
	 * creation or load; the actual internal size of the texture may vary. If you're using this
     * texture as a source for TRenderer::DrawVertices, see
     * GetInternalSize.
	 *
	 * @return Width of the texture in pixels.
	 */
	virtual uint32_t GetWidth();
	/**
	 * Get the height of the texture. This gets the size of the texture as requested at
	 * creation or load; the actual internal size of the texture may vary. If you're using this
     * texture as a source for TRenderer::DrawVertices, see
     * GetInternalSize.
	 *
	 * @return Height of the texture in pixels.
	 */
	virtual uint32_t GetHeight();

	/** @} */

	/**
     * @name Utility Functions
	 * @{
	 */
	/**
     * Get the animation script that was embedded in the XML file.
	 *
	 * @return Animation script
	 */
	str GetScript();

	/**
	 * Get the path to the source data file.
	 *
     * @return A string that represents the path to the source data
     *         file.
	 */
	str GetPath();

	/**
	 * Get a shared pointer (TAnimatedTextureRef) to this texture.
	 *
	 * @return A TAnimatedTextureRef that shares ownership with other Refs
	 *         to this texture.
	 */
	TAnimatedTextureRef	GetRef()
	{
		return static_pointer_cast<TAnimatedTexture>( shared_from_this() );
	}

	/** @} */

protected:
	virtual void Restore();

private:

	void SetDefaultScript();

protected:
	/**
	 * Internal implementation data.
	 */
    TAnimatedTextureData* mATData;

private:
	TAnimatedTexture(str name);
	static TAnimatedTextureRef InternalGet( str handle );
	static TAnimatedTextureRef InternalNew( str handle="", str imageOverride="", str maskOverride="" );
	bool ParseAnimation(str imageOverride, str maskOverride);
	bool ParseFromXML(str assetName);
	bool ParseFromANM(str assetName);
	bool LoadTextures();

};


#endif // ANIMATEDTEXTURE_H_INCLUDED
