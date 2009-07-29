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
 * Interface for class TRenderer
 */

#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#ifndef PFLIBCORE_H_INCLUDED
#include "pflibcore.h"
#endif

#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

#ifndef VERTEXSET_H_INCLUDED
#include "vertexset.h"
#endif

#ifndef RECT_H_INCLUDED
#include "rect.h"
#endif

class TRendererData;
class TRendererImpl;

/**
 * The interface to the rendering subsystem. Available as a
 * singleton while the game is running using
 * TRenderer::GetInstance().
 */
class PFLIB_API TRenderer
{
public:
	/**
	 * Destructor
	 */
	~TRenderer() ;

private:
	/**
	 * Default constructor.
	 */
	TRenderer() ;

private:
	TRenderer( TRenderer & other ) ;
	TRenderer & operator =( TRenderer & other ) ;
public:

    /**
     * Accessor
     */
	static TRenderer * GetInstance();

	/**
	 * Set the current rendering viewport. Only affects the 3d rendering
	 * functions, as the viewport is overridden internally by the TTexture::Draw
	 * functions.
	 *
     * @param viewport  Viewport
	 */
	void SetViewport( const TRect & viewport );

	/**
	 * Get the current rendering viewport.
	 *
	 * @param viewport  Viewport
	 */
	void GetViewport( TRect * viewport );

	/**
	 * Set the current screen clipping rectangle.
	 *
	 * @param clip   Rectangle to clip to in screen coordinates.
	 */
	void SetClippingRectangle( const TURect & clip );

	/**
     * Get the current screen clipping rectangle.
	 */
	TRect GetClippingRectangle();

	/**
	 * Push a clipping rectangle onto the internal stack.
	 *
	 * @param clip   Clipping rectangle to push.
	 */
	void PushClippingRectangle( const TURect & clip );

	/**
	 * Pop a clipping rectangle from the internal stack.
	 */
	void PopClippingRectangle();

	/**
	 * Push a viewport on the internal viewport stack.
	 *
	 * @param viewport  Viewport
	 */
	void PushViewport( const TRect & viewport );

	/**
	 * Restore a viewport from the viewport stack.
	 */
	void PopViewport();

	/** \name Raw Primitive Drawing.
	 *
	 * Routines for drawing groups of lines or triangles, given either 2d or 3d
	 * vertex data. Must be called in a window Draw() function, or between
	 * BeginRenderTarget/EndRenderTarget.
	 *
	 * Respects the currently active 2d or 3d rendering environment. Functions are indifferent
	 * to Begin2d/Begin3d/End2d/End3d.
	 *
	 * @{
	 */

	/// Type for DrawVertices.
	enum EDrawType
	{
		kDrawPoints=1,	///< DrawVertices renders the vertices as a collection of isolated points.
		kDrawLines,		///< DrawVertices renders the vertices as a set of isolated line segments.
		kDrawLineStrip, ///< DrawVertices renders the vertices as a line strip.
		kDrawTriangles, ///< DrawVertices renders each group of 3 vertices as a triangle.
		kDrawTriStrip,	///< DrawVertices renders the vertices as a triangle strip.
		kDrawTriFan		///< DrawVertices renders the vertices as a triangle fan.
	};

	/**
	 * Draw a set of vertices using the current environment.
	 *
     * There is a hard limit on the number of vertices of
     * TVertexSet::kMaxVertices.
	 *
	 * @param type     Type of data to draw.
	 * @param vertices A set of vertices.
	 */
	void DrawVertices( EDrawType type, const TVertexSet & vertices );

	/**
	 * The maximum number of indices that can be passed into
	 * DrawIndexedVertices().
	 */
	static const int kMaxIndices = 65535;

	/**
	 * Draw a set of vertices using the current environment.
	 *
     * There is a hard limit on the number of vertices of
     * TVertexSet::kMaxVertices, and on the number of indices of
     * kMaxIndices.
     *
	 * @param type       Type of data to draw.
	 * @param vertices   A set of vertices.
	 * @param indices    An array of indices into the set of vertices.
	 * @param indexCount The number of indices.
	 */
	void DrawIndexedVertices( EDrawType type, const TVertexSet & vertices, uint16_t * indices, uint32_t indexCount );

	/** @} */

	/**
	 * \name 2d/3d Agnostic Code
	 *
	 * These functions allow you to modify the 2d or 3d rendering
	 * environment. Some of these states are cleared/reset by calling
	 * Begin2d or Begin3d, so don't expect any state to be preserved
	 * across those calls.
	 *
	 * @{
	 */

    /**
     * Shading modes.
     *
     * @see TRenderer::SetShadeMode
     */
	enum EShadeMode
	{
		kShadeFlat = 1,     ///< Flat shading with no shading across a polygon.
		kShadeGouraud,      ///< Gouraud shading with smooth shading across a polygon.
	};

	/// Set the shade mode
	void SetShadeMode( EShadeMode shadeMode);


	/// Query as to whether the current render target is the screen
	bool RenderTargetIsScreen();

	/**
	 * Set the current rendering texture.
	 *
	 * @param pTexture Texture to assign. Default parameter is NULL texture.
	 */
	void SetTexture(TTextureRef pTexture=TTextureRef());

    /**
     * Get the currently assigned texture.
     *
     * @return The current texture.
     */
    TTextureRef GetTexture();

	/**
	 * Blending modes for SetBlendMode()
	 */
	enum EBlendMode
	{
		kBlendNormal,		///< Normal alpha drawing.
		/**
		 * Draw with no alpha. This mode requires your texture to be
		 * square and each side be a power of two in order to work
		 * consistently.
		 */
		kBlendOpaque,
		kBlendAdditiveAlpha,///< Additive drawing, respecting source alpha. Useful for "glowing" effects.
		kBlendSubtractive,	///< Subtractive drawing; useful for shadows or special effects.
		kBlendMultiplicative, ///< Multiplicative drawing; can also be used for shadows or special effects.

		kBlendINVALID=-1,	///< Invalid blend mode.
	};

	/**
	 * Set the current blend mode. The blend mode is reset to "normal"
	 * when calling either Begin2d or Begin3d.
	 *
	 * @param blendMode The new blend mode.
	 */
	void SetBlendMode( EBlendMode blendMode );

    /**
     * Filtering modes.
     *
     * @see SetFilteringMode
     */
	enum EFilteringMode
	{
		kFilterPoint,       ///< Point-filtering: Select the nearest pixel.
		kFilterLinear       ///< Bilinear (or trilinear for MIPMAPs) filtering: Blend the nearest pixels.
	};

	/**
	 * Set the current filtering mode to use when scaling images.
	 *
	 * @param filteringMode
	 *               Mode to use.
	 */
	void SetFilteringMode( EFilteringMode filteringMode );

    /**
     * The various texture map modes.
     *
     * @see TRenderer::SetTextureMapMode()
     */
	enum ETextureMapMode
	{
		kMapClamp,      ///< Clamp the texture to 0,1.

        /// Repeat texture coordinates. Uses the fractional part of the texture coordinates only.
        /// Do not expect wrap to work with arbitrarily large numbers, as some video cards limit
        /// wrapping to as little as \f$\pm4\f$.
        kMapWrap,

		kMapMirror      ///< Mirror coordinate: 0->1->0->1.
	};

    /**
     * Set the texture map mode for use when texture coordinates extend
     * beyond the edge of the internal texture.
     *
     * NOTE that the internal texture is almost always square and a power of
     * two, so most textures should be limited to those constraints when using
     * wrapping modes.
     *
     * @param umap   Texture map mode for U coordinates.
     * @param vmap   Texture map mode for V coordinates.
     */
	void SetTextureMapMode( ETextureMapMode umap, ETextureMapMode vmap );

	/**
	 * Enable or disable writing to zbuffer.
	 *
	 * @param writeToZbuffer
	 *               True to write to zbuffer.
	 * @see SetZBufferTest
	 */
	void 	SetZBufferWrite( bool writeToZbuffer );

	/**
	 * Enable zbuffer test.
	 *
	 * @param testZbuffer
	 *               True to test zbuffer when drawing.
	 * @see SetZBufferWrite
	 */
	void	SetZBufferTest( bool testZbuffer );

	/**
	 * Enable writing of RGB color data. Default is on.
	 *
	 * If you disable writing of RGB color data, no visible
	 * drawing will happen, though if zbuffer writing is still
	 * active drawing will still occur to that plane.
	 *
	 * @param colorWrite
	 *               True to enable writing.
	 */
	void 	SetColorWrite( bool colorWrite );

	/**
	 * Get the current "color write" state.
	 *
     * @return True if color writing is enabled.
     * @see SetColorWrite
	 */
	bool 	GetColorWrite();

	/**
     * Clear the ZBuffer. Done automatically by BeginDraw() and
     * BeginRenderTarget().
	 */
	void ClearZBuffer();

	/**
     * Query ZBuffer-write state.
	 *
     * @return True if ZBuffer writing is enabled; false otherwise.
     * @see SetZBufferWrite
	 */
	bool GetZBufferWrite();

	/**
     * Query ZBuffer-test state.
	 *
     * @return True if ZBuffer testing is enabled; false otherwise.
     * @see SetZBufferTest
	 */
	bool GetZBufferTest();


	/** @} */

    /** \name Information
	 *
	 * @{
	 */

	/**
	 * Get data about the current system.
	 *
	 * @return A system data string.
	 */
	str GetSystemData();

	/**
     * Query whether textures on this computer will be created as
     * square internally. You can always create textures of any
     * shape, but internally a texture may be extended to the next
     * power-of-two, square size that will fit your requested
     * texture size.
     *
     * Note that texture dimensions will be rounded up to the next
     * power-of-two on cards that require it; always query
     * TTexture::GetInternalSize() if the exact texture dimensions
     * are relevant.
	 *
     * @return True if textures will always be square, internally.
	 */
	bool GetTextureSquareFlag();

	/** @} */

	/**
     * \name Scene Management Functions
     *
     * Enable drawing to a render target or the backbuffer, or set
     * up a 2d or 3d rendering context.
     *
     * Drawing to the backbuffer is usually handled by the system;
     * by the time a TWindow::Draw() function is called, you are
     * already in a rendering state.
	 *
	 * @{
	 */

    /// Modes for TRenderer::BeginRenderTarget
    enum ERenderTargetMode
    {
        /// Render RGB with an opaque alpha. Does not preserve the current
        /// texture contents.
        kFullRenderRGB1,

        /// Render RGB with an undefined alpha. Does not preserve the current
        /// texture contents. Alpha contents are <b>undefined</b>.
        kFullRenderRGBX,

        /// Render RGB with an opaque alpha, preserving the current texture
        /// contents; entire final surface (not just areas that are overdrawn)
        /// will get an opaque alpha. Note that this call is actually slower
        /// than kFullRenderRGB1, since the existing texture data needs to be
        /// copied to the render surface.
        kMergeRenderRGB1,

        /// Render RGB with an opaque alpha, preserving the current texture
        /// contents; final surface alpha is unchanged. Note that this call is
        /// actually slower than kFullRenderRGBX, since the existing texture
        /// data needs to be copied to the render surface.
        kMergeRenderRGBX,

        /// Render Alpha without modifying RGB pixels. RGB of original image will
        /// be preserved.
        kMergeRenderXXXA,
    };

	/**
	 * Start rendering to a texture. Must NOT be called during
     * a Draw() update. After being called, all rendering commands
     * without a texture target will be directed instead to the
     * texture passed in to BeginRenderTarget().
     *
     * If you are rendering to a texture that you will be completely
     * covering with content, use one of the "full" render modes
     * (kFullRenderRGB1 or kFullRenderRGBX). Note that in a full
     * mode that if you don't cover the surface you may get garbage
     * left over in the rendering buffer in any untouched areas of
     * the image.
     *
     * If your image already has content that you want to render on
     * top of, use one of the merge modes.
     *
     * If you want your resulting image to have an alpha component,
     * you'll need to render it in two passes. This is because a lot
     * of video cards don't have the ability to render alpha to a
     * buffer--but they do have the ability to redirect alpha to
     * the RGB channels. So that's what kMergeRenderXXXA does: It
     * sets up the rendering pipeline to render just the alpha to
     * the RGB channels, and then copies the resulting data back
     * into your texture alpha channel. The usage pattern looks like
     * this:
     *
     * @code
     *
	 * TTextureRef myTexture = .... ; // Create your texture
     * TRenderer * r = TRenderer::GetInstance();
     * if (r->BeginRenderTarget(myTexture,TRenderer::kFullRenderRGBX))
	 * {
	 * 		DoRendering();			// Paint the full texture
	 * 		r->EndRenderTarget();
	 * }
	 *
	 * if (r->BeginRenderTarget(myTexture,TRenderer::kMergeRenderXXXA))
	 * {
	 * 		DoRendering();			// Paint the full texture again, this time
	 * 		r->EndRenderTarget();	// to get alpha information
	 * }
     *
     * @endcode
     *
	 * @warning Will not render to areas of a surface beyond
     *          the extents of the screen surface (800x600 by
     *          default). This allows us to use the most compatible
     *          render-to-surface modes.
	 *
     *          Additionally, by using the most compatible modes,
     *          this is not a fast operation. It is best used to
     *          pre-render an image that will be used multiple times
     *          rather than attempting to execute it every frame.
     *
     * @param texture Texture to render to.
     * @param mode    Render target mode.
	 *
	 * @return True on success.
	 */
	bool 		BeginRenderTarget( TTextureRef texture, ERenderTargetMode mode );

	/**
	 * Complete the rendering to a texture.
	 *
	 * @see BeginRenderTarget
	 */
	void 		EndRenderTarget();

	/**
	 * Open an internal draw context. Called automatically by
	 * the system.
	 *
     * <i>Typically internal use only. This function is called by
     * the system prior to your TWindow::Draw() and
     * TWindow::PostDraw() calls.</i>
	 *
	 * @param needRefresh
	 *               True if we should refresh the screen (for support of dirty-
	 *               rectangle drawing). Usually false, meaning you're redrawing
	 *               the entire screen.
	 *
	 * @return True on success. If BeginDraw() returns false, do not
	 *         call EndDraw().
	 */
	bool BeginDraw(bool needRefresh);

	/**
	 * Close and complete an internal draw context. Also
	 * presents the completed rendered screen.
	 *
	 * @param flip     Flip or blit the back buffer to the screen.
	 */
	void EndDraw(bool flip=true);

	/**
	 * Begin 2d rendering. Any calls to TTexture::Draw*() functions
	 * should happen between a Begin2d() and End2d() call.
     *
     * It's a good idea to minimize renderer state changes, as they
     * can be expensive on some cards. Try to group most of your 2d
     * rendering together in as few Begin2d() groups as possible.
	 *
	 * Clears all 3d matrices when called. You can set the view and world
	 * matrix between Begin2d() and End2d(), but they will be cleared again
	 * on End2d().
	 *
     * You can use the helper class TBegin2d to automatically close
     * the block on exiting the scope.
     *
	 * @return True on success. False on failure, which can mean that
	 *         you are already in a Begin2d() state, you are in a
	 *         Begin3d() state, or some other aspect of the engine
	 *         has gotten into a bad state. Check log file messages
     *         for details.
     *
     * @see TBegin2d
	 */
	bool Begin2d();

	/**
     * Finish a 2d rendering set.
     *
     * @see Begin2d
	 */
	void End2d();

	/**
	 * Begin 3d rendering. Any calls to TModel::Draw*() functions
     * should happen between a Begin3d() and End3d() call. Also,
     * calls to DrawVertices() with vertex types TLitVert or TVert
     * should be within a Begin3d() block.
     *
     * It's a good idea to minimize renderer state changes, as they
     * can be expensive on some cards. Try to group most of your 3d
     * rendering together in as few Begin3d() groups as possible.
     *
     * You can use the helper class TBegin3d to automatically close
     * the block on exiting the scope.
	 *
	 * @return True on success. False on failure, which can mean that
	 *         you are already in a Begin3d() state, you are in a
	 *         Begin2d() state, or some other aspect of the engine
	 *         has gotten into a bad state. Check log file messages
     *         for details.
     *
     * @see TBegin3d
	 */
	bool Begin3d();

	/**
     * Finish a 3d rendering set.
     *
     * @see Begin3d
	 */
	void End3d();

	/**
	 * Query as to whether we're currently in a drawing mode.
	 *
	 * @return True if we're between BeginDraw() and EndDraw(), or
	 *         BeginRenderTarget() and EndRenderTarget().
	 */
	bool InDraw() const  ;

	/** @} */


	/** \name 2d-Related Functions
	 *
	 * @{
	 */

	/**
	 * Fill a rectangle, optionally specifying a destination texture.
	 *
	 * Unlike TTexture::Draw*() calls, FillRect can be used to draw
	 * a rectangle in any texture type, and does not depend on the
     * Begin2d/End2d state. <b>Will not blend</b> using the alpha value;
     * instead it writes the alpha value as part of the color into
     * the destination, when the destination texture supports alpha.
     *
     * In the case of screen fills, the alpha value <b>is never
     * used,</b> since to draw an alpha value to the screen is
     * meaningless (the screen surface is never used as a source
     * texture).
	 *
     * @param rect   Rectangle to fill; this rectangle is either
     *               relative to the upper left corner of the
     *               current viewport or window (when dst is NULL),
     *               or is relative to the upper left corner of the
     *               texture.
     * @param color  Color to draw--this color (RGBA) will be
     *               written verbatim into the surface across the
     *               rectangle with no blending, such that all
     *               pixels in the rectangle will exactly equal the
     *               RGBA color specified.
     * @param dst    Optional destination texture. Current viewport
     *               and window coordinates are ignored when dst is
     *               non-NULL.
	 */
	void FillRect(const TURect & rect, const TColor & color, TTextureRef dst=TTextureRef());

	/**
     * Query as to whether the previously set capture texture is
     * still waiting to be filled with the screen render.
	 *
     * @return True if the capture texture is still pending.
	 */
	bool IsCapturePending();

	/**
	 * Set a capture texture. This texture will be filled with the
     * results of the next screen update. Technically, the next
     * EndDraw() will trigger a copy of the back-buffer surface
     * to the given texture; EndDraw() typically is called at the
     * end of a screen update by TWindowManager.
     *
     * The capture texture MUST be exactly the size of the screen,
     * or SetCaptureTexture() will silently fail (or ASSERT in
     * debug builds).
	 *
	 * @param texture Texture to fill.
	 */
	void SetCaptureTexture( TTextureRef texture );

	/**
	 * Get the current capture texture from the renderer.
	 *
	 * After a texture is filled, this function will return an
	 * empty texture.
	 *
	 * @return The texture that's waiting to be filled with
	 *         the results of a screen render. An empty TTextureRef if
	 *         SetCaptureTexture() hasn't been called, or after the
	 *         texture has been filled.
	 */
	TTextureRef  GetCaptureTexture();

	/** @} */

	/** \name 3d-Related functions
	 *
	 * @{
	 */

	/**
	 * Set the world matrix.
	 *
	 * Playground uses a World/View/Projection transformation model.
	 * This method allows you to set the current world matrix.
	 *
	 * @param pMatrix New world matrix.
	 */
	void		SetWorldMatrix(TMat4* pMatrix);

	/**
	 * Set the view matrix.
	 *
	 * Playground uses a World/View/Projection transformation model.
	 * This method allows you to set the current view matrix.
	 *
	 * Helper function SetView() can be used to set the view matrix based on
	 * a location, a look-at target, and an up vector.
	 *
	 * @param pMatrix New view matrix.
	 */
	void		SetViewMatrix(TMat4* pMatrix);

	/**
	 * Set the projection matrix.
	 *
	 * Playground uses a World/View/Projection transformation model.
	 * This method allows you to set the current projection matrix.
	 *
	 * Projection can be set up automatically using
	 * SetPerspectiveProjection.
	 *
	 * @see SetPerspectiveProjection
	 *
	 * @param pMatrix New projection matrix.
	 */
	void		SetProjectionMatrix(TMat4* pMatrix);

	/**
	 * Set the view matrix based on the viewer location, a target
	 * location, and an up vector.
	 *
	 * @param eye    The viewer's location.
	 * @param at     The target location.
	 * @param up     Up vector.
	 */
	void		SetView( const TVec3 & eye, const TVec3 & at, const TVec3 & up );

	/**
	 * Set a perspective projection matrix.
	 *
	 * @param nearPlane Distance to near plane.
	 * @param farPlane  Distance to far plane.
	 * @param fov       Field of view in radians.
	 * @param aspect    Aspect ratio of view (0 to calculate the aspect ration from
	 *                  the viewport size).
	 */
	void		SetPerspectiveProjection(TReal nearPlane, TReal farPlane, TReal fov=PI/4.0f, TReal aspect=0);

	/**
	 * Set an orthogonal projection matrix.
	 *
	 * @param nearPlane Distance to the near plane.
	 * @param farPlane  Distance to the far plane.
	 */
	void		SetOrthogonalProjection(TReal nearPlane, TReal farPlane);

	/**
	 * Get the current world matrix.
	 *
	 * @param m      A matrix to fill with the current world matrix.
	 */
	void		GetWorldMatrix(TMat4* m);
	/**
	 * Get the current view matrix.
	 *
	 * @param m      A matrix to fill with the current view matrix.
	 */
	void		GetViewMatrix(TMat4* m);
	/**
	 * Get the current projection matrix.
	 *
	 * @param m      A matrix to fill with the current projection matrix.
	 */
	void		GetProjectionMatrix(TMat4* m);

	/**
	 * Set the scene's ambient color.
	 *
	 * @param color  New ambient color.
	 */
	void SetAmbientColor(const TColor & color);

	/// Possible cull modes.
	enum ECullMode
	{
		kCullNone = 1,		///< No culling.
		kCullCW,			///< Cull faces with clockwise vertices
		kCullCCW			///< Cull faces with counterclockwise vertices
	};

	/**
	 * Set the current cull mode of the 3d render.
	 *
	 * @param cullMode The new cull mode.
	 */
	void SetCullMode(ECullMode cullMode);

	/**
     * Set the current rendering material.
     *
     * @param mat Material to set.
	 */
	void SetMaterial(TMaterial* mat);

	/**
	 * Set up one of the scene lights.
	 *
	 * @param index  Index to light to initialize.
	 * @param light  Light data.
	 */
	void SetLight( uint32_t index, TLight * light );

	/**
	 * Toggle the HUD
	 *
	 * @return true if HUD was previously active.
	 */
	bool ToggleHUD();
#if !USER_DOCS
	static void ShutDown();
#endif
    /**
     * Currently in Begin2d mode.
     *
     * @return True if in Begin2d mode.
     */
	bool In2d() const;

    /**
     * Currently in Begin3d mode.
     *
     * @return True if in Begin3d mode.
     */
	bool In3d() const;

	/**
	 * Set a renderer option.
	 *
	 * @param option Option to set.
	 * @param value  Value to set.
	 */
	void SetOption( str option, str value );

	/**
	 * Get a renderer option.
	 *
	 * @param option Option to get.
	 *
	 * @return Value.
	 */
	str GetOption( str option );

	/** @} */

private:
	/// The global instance of the TRenderer
	static TRenderer * mInstance;

	/// The private (platform-agnostic) data
	TRendererData * mData ;

	/// The private (platform-specific) implementation and data
	TRendererImpl * mImpl ;
};

/**
 * Helper class to wrap 2d rendering.
 *
 * When you construct this class it activates a
 * TRenderer::Begin2d() state, and when it's destroyed, it calls
 * TRenderer::End2d(). You can also stop the state early by
 * calling TBegin2d::Done().
 *
 * This allows you to create a local stack variable that will
 * automatically disable TRenderer::Begin2d() mode when it
 * leaves scope.
 */
class TBegin2d
{
public:
	/// Default constructor
	TBegin2d()
	{
		success= TRenderer::GetInstance()->Begin2d();
	}
	/// Destructor
	~TBegin2d()
	{
		Done();
	}
	/// We're done with 2d for now!
	void Done()
	{
		if (success)
		{
			TRenderer::GetInstance()->End2d();
			success = false ;
		}
	}
private:
	bool success ;
};

/**
 * Helper class to wrap 3d rendering.
 *
 * When you construct this class it activates a
 * TRenderer::Begin3d() state, and when it's destroyed, it calls
 * TRenderer::End3d(). You can also stop the state early by
 * calling TBegin3d::Done().
 *
 * This allows you to create a local stack variable that will
 * automatically disable TRenderer::Begin3d() mode when it
 * leaves scope.
 */
class TBegin3d
{
public:
	/// Default constructor
	TBegin3d()
	{
		success= TRenderer::GetInstance()->Begin3d();
	}
	/// Destructor
	~TBegin3d()
	{
		Done();
	}
	/// We're done with 3d for now!
	void Done()
	{
		if (success)
		{
			TRenderer::GetInstance()->End3d();
			success = false ;
		}
	}
private:
	bool success ;
};


#endif // RENDERER_H_INCLUDED

