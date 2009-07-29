//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#ifndef ASSET_H_INCLUDED
#include "asset.h"
#endif

#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

#ifndef PLATFORM_H_INCLUDED
#include "platform.h"
#endif

#ifndef DRAWSPEC_H_INCLUDED
#include "drawspec.h"
#endif

#ifndef TAGGING_IGNORE
class TTextureData ;
#endif

/**
 * This class encapsulates the concept of a texture.
 *
 * A TTexture can be used to:
 *
 *   - Texture a 3d object on the screen
 *   - Draw a sprite
 *   - Draw a screen widget (background, button, etc.)
 *   - Be a target for 3d rendering <i>(which will restrict it to
 *     being a 2d blit source)</i>
 *
 * Texture size is limited to 1024x1024 for textures that are in
 * video RAM (i.e., any texture that isn't created as "slow").
 *
 * Additionally, some textures are created from bitmaps,
 * and those bitmaps can be dynamically MIPMAPped. Bitmaps can
 * be read from JPG or PNG files, and the PNG files will correctly
 * read the transparency information, if present.
 *
 * The image extension can be omitted to allow the decision between
 * JPG and PNG to be made on a an image-by-image basis without needing
 * to change code.
 */
class PFLIB_API TTexture : public TAsset
{
	PFSHAREDTYPEDEF(TAsset);
protected:
#if !USER_DOCS
	/// Construction is through the factory method
	TTexture();
#endif
public:
	virtual ~TTexture();

    /// Texture loading flags used with TTexture::Get and TTexture::GetMerged.
	enum ETextureCreateFlags
	{
		/// Select alpha based on the image content. In the case of a PNG
		/// image that contains alpha information, alpha will be enabled;
		/// otherwise it will be disabled.
		eDefaultAlpha=0,
		eGenerateMipmaps=1,			///< Generate MIPMAPs for the image.
		eForceAlpha=2,				///< Force the image to have an alpha channel.
		eForceNoAlpha=4,			///< Force the image to not have an alpha channel.
	};


public :
	/**
	 * @name Factory Methods
	 * @{
	 */

	/**
	 * Get a texture from a handle with optional alpha information and optional auto-generated mipmaps.
	 * The handle refers to a file in the assets folder.
	 *
	 * When passing in an assetName, flags can be passed following a "?" and seperated by commas.
	 * Example "texture.png?slow"
	 *
	 * Maximum texture size for non-slow textures is 1024x1024.
	 *
	 * Available flags are:
	 *  - slow - Load this texture into slow system RAM. Allows for non-square, non-power-of-two textures,
	 * 			 as well as textures larger than 1024x1024.
	 * 			This flag implies "simple"--no direct drawing from this surface is allowed.
	 *  - simple - Create this texture as a simple surface. See GetSimple() for an explanation.
	 *  - alpha  - Create this texture with an alpha layer.
	 *  - mipmap - Create this texture with mipmaps.
	 *
	 * @param assetName Name of the asset. File extension (.jpg, .png) is not necessary.
	 * @param flags 	ETextureCreateFlags
	 *
	 * @return A TTextureRef to the texture.
	 */
	static TTextureRef Get( str assetName, uint32_t flags=eDefaultAlpha );

	/**
	 * Get a texture from 2 assets, one which specifies the color map and
	 * one which specifies the alpha map.  So that the alpha map can be a
	 * highly compressed image, the alpha value will be pulled from the
	 * "red" channel of the alpha asset.
	 *
	 * See TTexture::Get() for information on passing flags inside of asset names.
	 *
	 * Note that eForceNoAlpha has no effect, since the point of this function is
	 * to add an alpha channel.
	 *
	 * @warning The two assets must be the same dimensions, or the alpha map
	 * will not show up correctly.
	 *
     * @param colorAssetName    Name of the asset used for the color
     *                          map.File extension (.jpg, .png) is
     *                          not necessary. Supports "?" flags
     *                          like TTexture::Get(), but only
     *                          include those options on the
     *                          colorAssetName parameter.
     *
     * @param alphaAssetName    Name of the asset used for the
     *                            alpha map. File extension (.jpg,
     *                            .png) is not necessary. Do not use
     *                            "?" flags on alphaAssetName.
	 * @param flags 	ETextureCreateFlags
	 *
	 * @return A TTextureRef to the texture.
	 */
	static TTextureRef GetMerged( str colorAssetName, str alphaAssetName, uint32_t flags=eDefaultAlpha );


	/**
	 * Create a texture at a particular size.
	 *
     * Maximum texture size for a created texture is 1024x1024.
     *
     * @warning Cannot be called within a BeginDraw/EndDraw or
     * BeginRenderTarget/EndRenderTarget block.
	 *
	 * @param width      Width
	 * @param height      Height
	 * @param alpha  Create with alpha channel
	 *
	 * @return A TTextureRef to the texture.
	 */
	static TTextureRef Create( uint32_t width, uint32_t height, bool alpha );

	/**
	 * Get a simple texture from an asset name
	 *
	 * Simple textures are not usable as 3d textures, and in fact can only be copied with
	 * CopyPixels(). Advantages include speed and decreased memory usage: A "Simple" texture
	 * can be created without the common restrictions of 3d textures, which often need to be
	 * powers of two in size and square.
	 *
	 * Maximum texture size for non-slow textures is 1024x1024; slow textures have problems
	 * with either dimension larger than 4096.
	 *
	 * Look in the documentation for TTexture::Get for information on flags that can be appended
	 * to the asset name.
	 *
	 * @param assetName Name of the asset to load or acquire a reference to.
	 * 							File extension (.jpg, .png) is not necessary.
	 *
	 * @see TTexture::Get()
	 * @return A TTextureRef to the texture.
	 */
	static TTextureRef GetSimple( str assetName );

	/**
	 * Create a simple texture surface. See GetSimple for more information on
	 * "simple" textures.
	 *
     * Maximum texture size for non-slow textures is 1024x1024;
     * slow textures have problems with either dimension larger than
     * 4096.
	 *
     * @warning Cannot be called within a BeginDraw/EndDraw or
     * BeginRenderTarget/EndRenderTarget block.
     *
	 * @param width     Width
	 * @param height    Height
     * @param slow		True to create a slow (RAM based) texture.
     * @param alpha     True to have this texture include alpha.
	 *
	 * @see TTexture::GetSimple
	 * @return A TTextureRef to the created texture.
	 */
	static TTextureRef CreateSimple( uint32_t width, uint32_t height, bool slow=false, bool alpha=false );

	/** @} */

	/**
	 * @name Drawing Methods
	 * @{
	 */

	/**
	 * Draw a normal texture to a render target surface as a sprite. This
	 * draws a texture with optional rotation and scaling. Only capable of
     * drawing an entire surface--not a sub-rectangle. See
     * TTexture::DrawSprite(const TDrawSpec&) for more drawing
     * control.
	 *
	 * Will draw the sprite within the currently active viewport. X and Y are
	 * relative to the upper left corner of the current viewport.
	 *
	 * DrawSprite can be called inside TWindow::Draw() or a BeginRenderTarget/EndRenderTarget block.
	 *
	 * @param x      X of Center.
	 * @param y      Y of Center.
	 * @param alpha  Alpha to apply to the entire texture. Set to a negative
	 *               value to entirely disable alpha during blit, including
	 *               alpha within the source TTexture.
	 * @param scale  Scaling to apply to the texture. 1.0 is no scaling.
	 * @param rotRad Rotation in radians around center point.
	 * @param flags Define how textures are drawn. Use ETextureDrawFlags for the
	 *              flags. Default behavior is eDefaultDraw.
	 */
	 virtual void DrawSprite( TReal x, TReal y, TReal alpha=1, TReal scale=1, TReal rotRad=0, uint32_t flags = 0 );

	/**
	 * Draw a normal texture to a render target surface or
	 * backbuffer as a sprite. Uses the %TDrawSpec to decide where to
	 * put the texture and how to draw it. See TDrawSpec for
	 * details.
	 *
	 * Will draw the sprite within the currently active viewport. %TDrawSpec
	 * position is relative to the upper left corner of the current
	 * viewport.
	 *
	 * %DrawSprite can be called inside TWindow::Draw() or a
	 * TRenderer::BeginRenderTarget()/%EndRenderTarget block.
	 *
	 * @param drawSpec The %TDrawSpec to use to draw the sprite.
	 */
	virtual void DrawSprite( const TDrawSpec & drawSpec );

	/**
	 * Draw a normal or simple texture to any target TTexture surface with the same alpha as
	 * this surface. In other words, CopyPixels can go from an alpha surface to another alpha
	 * surface, or to a non-alpha to another non-alpha surface, but not between the two.
	 *
	 * There are no restrictions on the blit source rectangle or the destination
	 * of the blit within the target surface.
	 *
	 * CopyPixels() does not respect alpha in its copy; it performs a bitwise, opaque copy only.
	 *
	 * Unlike the other texture calls, CopyPixels() can be called outside of a TWindow::Draw() or
	 * a BeginRenderTarget/EndRenderTarget block.
	 *
	 * @param x      Left side of resulting rectangle.
	 * @param y      Top edge of resulting rectangle.
	 * @param sourceRect Source rectangle to blit. NULL to blit the entire surface.
	 * @param _dst   Destination texture. NULL to draw to back buffer.
	 */
	virtual void CopyPixels(int32_t x, int32_t y, const TRect * sourceRect=NULL, TTextureRef _dst=TTextureRef());

	/** @} */

	/**
	 * @name Surface access.
	 * @{
	 */

	/**
	 * Lock a surface for reading and writing pixel data.
	 *
	 * Surface will be stored in 32 bit pixels, in the binary order
	 * defined by TColor32.
	 *
	 * @param data       [out] Receives a pointer to the locked data.
	 * @param pixelPitch Number of pixels to add to a pointer to advance one row.
	 *
	 * @return True on success.
	 */
	bool Lock( TColor32 ** data, uint32_t * pixelPitch );

	/**
	 * Unlock a surface
	 */
	void Unlock();

	/** @} */

	/**
	 * @name Information Query.
	 * @{
	 */

	/**
	 * Get the width of the texture. This gets the width of the texture as requested at
	 * creation or load; the actual internal width of the texture may vary. If you're using this
     * texture as a source for TRenderer::DrawVertices, see
     * GetInternalSize().
	 *
	 * @return Width of the texture in pixels.
	 */
	virtual uint32_t GetWidth();

	/**
	 * Get the height of the texture. This gets the height of the texture as requested at
	 * creation or load; the actual internal height of the texture may vary. If you're using this
	 * texture as a source for TRenderer::DrawVertices, see GetInternalSize().
	 *
	 * @return Height of the texture in pixels.
	 */
	virtual uint32_t GetHeight();

	/**
	 * Gets the internal width and height of the texture in pixels, rather
	 * than the requested width and height.  Relevant if you're using the
	 * texture as a texture source for TRenderer::DrawVertices.
	 *
	 * The texture coordinates that you set in vertices that refer
	 * to this texture need to be calculated based on the internal
	 * representation size, and not the "logical" size that GetWidth()
	 * and GetHeight() return. In other words, if the original width
	 * is 800, and the internal width is 1024, then your U coordinate
	 * for the right edge would be 800.0F/1024.0F.
	 *
	 * If your textures are always powers of two and square in
	 * size (equal width and height), GetInternalSize should always
	 * return the same values as GetWidth() and GetHeight().
	 *
     * @return  A point with width and height of the internal
     *          texture size, in pixels.
	 */
	TPoint GetInternalSize();

	/**
	 * Get the name of the texture.
	 *
	 * @return The handle of the image used to create the texture, along with any alpha texture handle.
	 */
	str GetName();

	/**
	 * Query whether this texture is a "simple" type; simple textures
	 * can not be used in DrawSprite(), but can be locked and can be used
	 * in CopyPixels().
	 *
	 * @see TTexture::GetSimple
	 * @return True if simple
	 */
	bool IsSimple();

	/**
	 * Query whether this texture is a "slow" texture, i.e. one
	 * that's been allocated in system RAM.
	 *
	 * @return True if slow.
	 */
	bool IsSlow();

	/**
	 * Query whether this texture has an alpha channel.
	 *
	 * @return True if the texture has an alpha channel, false otherwise.
	 */
	bool HasAlpha();

	/**
	 * Clear the texture to black and transparent alpha.
	 */
	void Clear();

	/**
	 * Save the texture to a file.
	 *
	 * @param fileName		path of file to create including extension that
	 *						defines what format to save the file as. Currently
	 *						supported file extensions are: ".jpg" and ".png".
	 * @param quality		For file formats that use image compression, this
	 *						specifies what level of compression to use
	 *						(1.0 means highest quality, 0.0 means lowest quality)
	 *						.png files currently ignore the quality parameterT
	 *
	 * @return true if file was successfully saved, false otherwise.
	 *
	 */
	bool Save(str fileName, TReal quality=1.0f);

	/**
	 * Get a shared pointer (TTextureRef) to this texture.
	 *
	 * @return A TTextureRef that shares ownership with other Refs
	 *         to this texture.
	 */
	TTextureRef	GetRef()
	{
		return static_pointer_cast<TTexture>( shared_from_this() );
	}
	/** @} */

	/**
	 * Internal implementation data.
	 * @nouser
	 */
    TTextureData* mData;

#if !DOXYGEN
protected:
	void MarkMemoryUsed();
	virtual void Restore();
	virtual void Release();
#endif

private:
	/**
	 * Internal function that gets an asset from the asset
	 * manager, if one exists.
	 *
	 * @param handle Handle to the asset.
	 *
	 * @return A TTextureRef of the asset; can be NULL.
	 */
	static TTextureRef InternalGet( str handle );

protected:
	/**
	 * Internal function to create a TTexture.
	 *
	 * @param handle (optional) Handle of asset to add to asset manager.
	 *               Empty to create a unique TTexture.
	 *
	 * @return A TTextureRef to the new texture.
	 */
	static TTextureRef InternalNew( str handle="" );

private:
	/**
	 * Unload all texture resources.
	 */
	void        Free();
};

//------------------------------------------------------------------

#endif // TEXTURE_H_INCLUDED
