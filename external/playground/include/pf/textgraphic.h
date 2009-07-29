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
 *
 * Interface for TTextGraphic class.
 */

#ifndef TEXTGRAPHIC_H_INCLUDED
#define TEXTGRAPHIC_H_INCLUDED

#include <vector>

//------------------------------------------------------------------
#ifndef WINDOW_H_INCLUDED
#include "window.h"
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

//------------------------------------------------------------------
// Forward declarations

#ifndef TAGGING_IGNORE

class TTextState;
class TTextLine;
class TTextRun;

#endif

//------------------------------------------------------------------
// Main class declaration

/**
 * Formatted text class.
 *
 * A class that allows you to format text using the following tags:
 *
 *   - \<br\>		Line break
 *   - \<p\>\</p\>  Paragraph
 *   - \<b\>\</b\>		Bold
 *   - \<i\>\</i\>		Italic
 *   - \<u\>\</u\>		Underline
 *   - \<center\>\</center\> Center
 *   - \<left\>\</left\> Left Justify
 *   - \<right\>\</right\> Right Justify
 *   - \<tab pos="100"\> Move the cursor to a specific offset,
 *     in pixels.
 *   - \<outline color="ff0000" size=2\> Outline text
 *   - \<font size="10" color="ff0000"\>\</font\>		Font characteristics
 *   - \<a id='buttonname'\>\</a\>	Trigger a button named 'buttonname' when clicking this text.
 *   - \<cursor\> Cursor icon
 *
 * Text can be rendered directly to screen. For text that is automatically
 * rendered in a window, use the TText class.
 *
 * Use the static function TTextGraphic::Create to create a new instance
 * of a TTextGraphic.
 *
 * @see TText
 */
class PFLIB_API TTextGraphic
{
private:
	TTextGraphic();
	// Destroy with TTextGraphic::Destroy()
	~TTextGraphic();
public:
	/// TTextGraphic window child flags
	enum EFlags
	{
		kHAlignLeft 	= 0x00, ///< Align horizontally with the left edge.
		kHAlignCenter 	= 0x01,	///< Align horizontally with the center.
		kHAlignRight 	= 0x02, ///< Align horizontally with the right edge.
		kVAlignTop 		= 0x00, ///< Align vertically with the top.
		kVAlignCenter 	= 0x04, ///< Align vertically with the center.
		kVAlignBottom 	= 0x08  ///< Align vertically with the bottom.
	};

#if !USER_DOCS
    /**
     * @nouser
     * <em>Preliminary and subject to change.</em>
     *
     * At relatively low-res (800x600 display) with anti-aliasing, a
     * lot of "thin" fonts end up looking pretty bad / not bold
     * enough against a background because often times they end up
     * with very few fully opaque pixels. "SetBoldOverride" allows
     * you to set 2 things about the specified font:
     *
     * - Force bold to always be on. This makes the bold tag do
     *   nothing.
     * - Set the bold scale.
     *
     * @param fontFilename  Font to modify.
     * @param bForceBold    Force this font to always render "bold".
     * @param boldScale     The default is 1.0.  If you want it
     *                      always bolded, then sometimes the normal
     *                      amount of bolding is too much / too
     *                      bold.  So you can set it to 0.5 or 0.75
     *                      to still make the font bolder, but less
     *                      so.  Or you can increase the amount of
     *                      bolding with values greater than 1.0.
     */
	static void SetBoldOverride(const char* fontFilename, bool bForceBold, TReal boldScale);

	static void SetLegacyLineWrap() { mLegacyLineWrap=true; }


#endif
	/**
     * Activate the sprite-render style for TTextGraphic. The legacy
     * style for TTextGraphic is for it to render as if the text
     * surface is a texture, and all alignment happens relative to
     * this "virtual" texture surface.
     *
     * In the newer "sprite" render mode, which was created to help
     * support the new TTextSprite class, rendering is done as if to
     * a constant virtual surface size. This has no effect on left
     * and top justified text, but centered and right or bottom
     * justified text are drawn relative to the original surface
     * offsets rather than scaling those offsets along with the text
     * size.
     *
     * @param activate True to activate sprite rendering for this
     *                 TTextGraphic.
     *
     * @see TPlatform::kTextGraphicSpriteRender
	 */
	void SetSpriteRender(bool activate=true) { mSpriteRender=activate; }

	/**
	 * Create a TTextGraphic.
	 *
	 * @param text       Initial text for graphic. Can be empty.
	 * @param w          Width of client rectangle. Must be non-zero.
	 * @param h          Height of client rectangle. Must be non-zero.
	 * @param flags      Flags from TTextGraphic::EFlags
	 * @param fontFilename
	 *                   Font name.
	 * @param lineHeight Font size.
	 * @param textColor  Font color.
	 *
	 * @return True on success.
	 */
	static TTextGraphic * Create(str text, uint32_t w, uint32_t h, uint32_t flags=0,const char* fontFilename="", uint32_t lineHeight=10, const TColor& textColor=TColor(0,0,0,1));

	/**
	 * Call to destroy a TTextGraphic. Deletes the object and releases
	 * all resources.
	 */
	void Destroy() ;

    /**
     * Draw the text to a rectangle on the screen.
     *
     * @deprecated This overload uses an older, more confusing,
     *             syntax, and doesn't allow for sub-pixel
     *             placement.
     *
     *  Must be called between a TRenderer::Begin2d/TRenderer::End2d
     *  pair.
     *
     * @param destRect Destination rectangle in screen coordinates.
     * @param scale    Scale factor - useful for zooming effects.
     * @param linePadding
     *                 Additional spacing between text lines.
     * @param alpha    Alpha multiplier. 1.0 is opaque.
     * @param target   [optional] Target texture to draw to. Leave as default to draw
     *                 to current context.
     */
	void Draw(const TRect & destRect, TReal scale=1.0, int32_t linePadding=0, TReal alpha=1.0, TTextureRef target = TTextureRef())
	{
		Draw( TVec2( (TReal)destRect.x1, (TReal)destRect.y1 ), destRect.GetHeight(),scale,linePadding,alpha,target );
	}

	/**
	 * Draw the text to the screen.
	 *
     * Must be called between a TRenderer::Begin2d/TRenderer::End2d pair.
	 *
	 * @param at     Location to draw the text.
	 * @param height Height of text region in pixels. Used to determine
	 *               how many lines of text to draw.
	 * @param scale  Scale factor - useful for zooming effects.
	 * @param linePadding
	 *               Additional spacing between text lines.
	 * @param alpha  Alpha multiplier. 1.0 is opaque.
	 * @param target [optional] Target texture to draw to. Leave as default to draw
	 *               to current context.
	 */
	void Draw(const TVec2 & at, uint32_t height, TReal scale=1.0, int32_t linePadding=0, TReal alpha=1.0, TTextureRef target = TTextureRef());

	/**
     * When drawing to an offscreen texture, copy pixels to the
     * texture, instead of alpha blending them. Does not work with
     * text outlines, since the text and outline need to be blended
     * with each other.
     *
     * The default state is to blend the pixels into the destination
     * texture, leaving alpha alone. If you call SetNoBlend(), then
     * pixels will be set directly to the colors and alpha values,
     * so that the texture can then be used blended with an
     * arbitrary background. TTextGraphic::SetAlphaBlend() is
     * similar, but uses a more complex (i.e., slower) blend
     * algorithm that supports text outlines.
     *
     * This setting has no effect when drawing to the screen.
     *
     * @see TTextGraphic::SetAlphaBlend()
	 */
	void		SetNoBlend();

	/**
     * When drawing to an offscreen texture, blend pixels into the
     * texture and accumulate alpha. SetAlphaBlend() uses a more
     * sophisticated (and slower) blend algorithm than
     * TTextGraphic::SetNoBlend() that causes it to work correctly
     * with text outlines, as well as blending the text in with
     * other translucent layers.
     *
     * This setting has no effect when drawing to the screen.
     *
     * @see TTextGraphic::SetNoBlend()
	 */
	void		SetAlphaBlend();

	/**
	 * Get the number of lines in the text output.
	 *
	 * @return Number of lines.
	 */
	uint32_t	GetLineCount();

	/**
	 * Set the first line in the text output.
	 *
     * @param startLine Line to display as the first line of text.
     *                  First line is 0.
	 */
	void		SetStartLine(TReal startLine=0) {mStartLine = startLine;}; // lines are 0 -> GetLineCount()-1

	/**
	 * Get the index of the first line of text output.
	 *
	 * @return An index between 0 and GetLineCount()-1.
	 */
	TReal GetStartLine() { return mStartLine; 	}

	/**
	 * Get the actual boundary of the rendered text.
	 *
     * @param bounds A rectangle in client coordinates.
     * @param scale  The scale you want the text to be at when you
     *               query the bounds.
	 */
	void		GetTextBounds(TRect* bounds, TReal scale = 1.0F);

	/**
     * Set the current text content. Will return immediately if the
     * text hasn't changed.
	 *
	 * @param text   Text to set.
	 */
	void SetText( str text );

	/**
	 * Get the current text content.
	 *
	 * @return A string containing the current text.
	 */
	str GetText() const { return mText; }

	/**
	 * Set the current text color.
	 *
	 * @param color  New text color.
	 */
	void SetColor(const TColor& color);

	/**
	 * Set a new line height for this text. Text is re-calculated
	 * as to line wraps and tags based on new height value.
	 *
	 * @param newHeight New text height.
	 */
	void SetLineHeight( uint32_t newHeight );

	/**
	 * Set the font to be used to draw this text. Can change
	 * text bounds.
	 *
	 * @param fontFilename
	 *               Font to use.
	 */
	void SetFont( const char * fontFilename );

	/**
	 * Get the current font.
	 * 
	 * @return The current font name.
	 */
	str GetFont();

	/**
	 * Set the flags used to render this text.
	 *
	 * @param flags  Font alignment flags.
	 */
	void SetFlags( uint32_t flags );

	/**
	 * Change the text rectangle
	 *
	 * @param w          Width of client rectangle. Must be non-zero.
	 * @param h          Height of client rectangle. Must be non-zero.
	 */
	void SetTextRect(uint32_t w, uint32_t h);

	/**
	 * Change the rotation - default is 0 degrees, 0,0 origin
	 *
	 * @param degrees   rotation angle in degrees (not radians because degrees are friendlier)
	 * @param originX	offset from left of text rect to use as center point of rotation
	 * @param originY	offset from top of text rect to use as center point of rotation
	 */
	void SetRotation(TReal degrees, uint32_t originX, uint32_t originY);

	/**
	 * Get the current text color
	 *
	 * @return The current default text color. Note this can be changed
	 *         by the text markup.
	 */
	const TColor & GetColor() const { return mTextColor ; }

    /**
     * Pick an anchor record within text.
     * ( &lt;a id="buttonname"&gt;link&lt;/a&gt; )
     *
     * @param point  Point to test within text.
     * @param linePadding
     *               Line padding to use when Picking.
     *
     * @return A button name to trigger, if one is found. An empty string if
     *         no button is found.
     */
	str Pick( const TPoint & point, int32_t linePadding=0 );

    /**
     * Handle rollover state for links.
     *
     * @param pPoint Point to test within text, or NULL to clear the state.
     * @param linePadding
     *               Line padding to use when handling roll-over.
     *
     * @return True if a rollover link was triggered.
     */
	bool Rollover( const TPoint* pPoint, int32_t linePadding=0 );

	/**
	 * Get the number of lines this text needs to be scrolled to
	 * fit in the current text region.
	 *
	 * @param scale  Scale of text.
	 * @param linePadding
	 *               Line padding.
	 *
	 * @return Number of lines to scroll to get to bottom.
	 */
	uint32_t GetMaxScroll( TReal scale, int32_t linePadding ) const;
private:
	/**
	 * Render the text. Internal function.
	 */
	void RenderText();

private: // No copying!
	TTextGraphic( const TTextGraphic& );
	TTextGraphic & operator=( const TTextGraphic& );

private: // Internal data.
	/// Internal string
	str			mText;

	TTextState*	mState;
	TRect		mBounds; 			///< Actual bounds of text
	TPoint		mRenderRegionSize;	///< Size of region text is rendered to
	TReal		mRotationCos;
	TReal		mRotationSin;
	int32_t		mRotationX;
	int32_t		mRotationY;
	bool		mbNoBlend;
	bool		mAlphaBlend;

	typedef std::vector< shared_ptr<TTextLine> >	LineList;
	LineList	mLines;

	str			mFont ;
	uint32_t	mLineHeight ;
	TColor		mTextColor ;
	TReal		mStartLine;		// Starting line (plus any fractional scroll)
	TReal		mYScaleOffset;
	str			mRolloverId;

	uint32_t	mTextFlags ;
	TReal		mAlpha ;

	static bool		mLegacyLineWrap ;

	bool		mSpriteRender ;
};

//------------------------------------------------------------------
#endif // TEXTGRAPHIC_H_INCLUDED
