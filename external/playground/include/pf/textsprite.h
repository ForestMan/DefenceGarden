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
 * Interface for class TSprite
 */


#ifndef TEXTSPRITE_H_INCLUDED
#define TEXTSPRITE_H_INCLUDED

#ifndef SPRITE_H_INCLUDED
#include "sprite.h"
#endif

#ifndef TAGGING_IGNORE
class TTaskList;
class TTextGraphic;
#endif

/**
 * A 2d text sprite object. A sprite that's primary purpose is
 * to draw text at the sprite's location. Will also respect
 * simple rotation and scaling from its matrix, and from the
 * parent's sprite context. Any skew or non-symmetric scaling
 * will be ignored, however.
 *
 * TTextSprites are always stored in the client as
 * TTextSpriteRef--reference counted objects. To destroy a
 * TTextSprite, simply call reset() on the last TTextSpriteRef
 * that refers to that sprite.
 *
 * @see TSprite
 */
class PFLIB_API TTextSprite : public TSprite
{
	PFSHAREDTYPEDEF(TSprite);
protected:
	/// Internal Constructor. Use TTextSprite::Create() to get a new sprite.
	TTextSprite(uint32_t w, uint32_t h, int32_t layer);
public:
	/// TTextSprite text alignment flags.
	enum EFlags
	{
		/// Align text horizontally with the left edge of the virtual space; text
		/// will "grow" to the right from the drawing location.
		kHAlignLeft 	= 0x00,
		/// Align text horizontally with the center the virtual space; text
		/// will be centered on the drawing location horizontally.
		kHAlignCenter 	= 0x01,
		/// Align text horizontally with the right edge of the virtual space; text
		/// will "grow" to the left from the drawing location.
		kHAlignRight 	= 0x02,
		/// Align text vertically with the top edge of the virtual space; text
		/// will "grow" down from the drawing location.
		kVAlignTop 		= 0x00,
		/// Align text vertically with the center the virtual space; text
		/// will be centered on the drawing location vertically.
		kVAlignCenter 	= 0x04,
		/// Align text vertically with the bottom of the virtual space; text
		/// will "grow" up from the drawing location.
		kVAlignBottom 	= 0x08
	};

	/**
	 * @name Initialization/Destruction
	 * @{
	 */

	/**
     * Allocation of a new text sprite. Construction is restricted
     * to help "encourage" the use of TTextSpriteRefs to hold
     * your TTextSprites (as well as to encapsulate the
     * TTextSpriteRef creation pattern).
	 *
	 * @param width      The logical width of the text region, in pixels;
	 *                   determines text wrapping.
	 * @param height     The logical height of the text region, in
	 *                   pixels. Determines the maximum height of drawn
	 *                   text.
     * @param justify    The alignment of the text; see \ref EFlags.
	 * @param fontFilename
	 *                   The handle of the font to use.
	 * @param lineHeight The initial line height.
	 * @param textColor  The initial color of the text.
	 * @param layer      Initial sprite layer.
	 *
	 * @return A newly allocated TTextSprite wrapped in a
	 *         TTextSpriteRef.
	 * @see TSprite::SetLayer
	 */
	static TTextSpriteRef Create(uint32_t width, uint32_t height,
		uint32_t justify=0,
		const char* fontFilename="",
		uint32_t lineHeight=10,
		const TColor& textColor=TColor(0,0,0,1),
		int32_t layer=0 );

	/// Destructor.
	virtual ~TTextSprite();

	/** @} */

	/**
     * @name Drawing
	 * @{
	 */

	/**
     * Draw the sprite and its children.
     *
     * A common mistake is to assume that the incoming TDrawSpec
     * here is what you use to position and configure this sprite.
     * The way to position a sprite is to modify its internal
     * TDrawSpec, which you retrieve with GetDrawSpec(). The
     * environment parameter is combined with the local TDrawSpec to
     * determine the actual position of the sprite, though it only
     * inherits those features marked in TDrawSpec as inheritable.
     * See TDrawSpec::GetRelative() for more details.
     *
     * @param environmentSpec The 'parent' or environment
     *                 drawspec--the frame of reference that this
     *                 sprite is to be rendered in. In general you
     *                 shouldn't pass anything in for this parameter
     *                 and instead should modify the position of the
     *                 sprite using its GetDrawSpec() member.
     *
     *                 Defaults to a default-constructed TDrawSpec.
     *                 See TDrawSpec for more details on what is
     *                 inherited.
	 * @param depth     How many generations of children to draw; -1
	 *                  means all children.
	 * @see TDrawSpec
	 */
	virtual void Draw(const TDrawSpec & environmentSpec=TDrawSpec(), int32_t depth=-1);

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
     * @see TTextSprite::SetAlphaBlend()
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
     * @see TTextSprite::SetNoBlend()
	 */
	void		SetAlphaBlend();

	/** @} */

	/**
	 * @name Bounding Rectangles and Hit Tests.
	 * @{
	 */

	virtual TRect GetRect( const TDrawSpec & parentContext, int32_t depth=-1 );

	/**
	 * Get the logical origin of the text in the virtual text
	 * area.
	 *
	 * @return The point in the virtual text rectangle relative to
	 *         which text will grow.
	 */
	const TVec2 & GetOrigin();

	/**
	 * Test to see if a point is within our sprite.
	 *
	 * @param at      Point to test.
	 * @param parentContext
	 *                The parent context to test within--where is this sprite being drawn,
	 *                and with what matrix? Alpha and color information is ignored.
	 * @param opacity Level of opacity to test for; -1 for a simple
	 *                bounding box test, or 0-255 for alpha color
   	 *                value, where 0 is transparent (and will
      	 *                therefore always succeed).
	 * @param depth   Depth of children to test. Zero means only test this sprite.
	 * 		  -1 means test
	 *
	 * @return true if point hits us.
	 */
	virtual bool HitTest( const TPoint & at, const TDrawSpec & parentContext, int32_t opacity=-1, int32_t depth=-1 );

	/** @} */

	/**
     * @name Embedded Links
     */

	/**
     * Pick an anchor record within text.
     * ( &lt;a id="buttonname"&gt;link&lt;/a&gt; )
     *
     * @param point  Point to test within text.
     *
     * @return A button name to trigger, if one is found. An empty string if
     *         no button is found.
     */
	str Pick( const TPoint & point);

    /**
     * Handle rollover state for links.
     *
     * @param pPoint Point to test within text, or NULL to clear the state.
     *
     * @return True if a rollover link was triggered.
     */
	bool Rollover( const TPoint* pPoint );

    /** @} */

	/**
     * @name Text Parameters
	 * @{
	 */

	/**
     * Set the virtual text rectangle; used for determining text
     * wrapping and vertical alignment.
	 *
     * @param w     Width of virtual text region, in pixels.
     * @param h     Height of virtual text region, in pixels.
	 */
	void SetTextRect(uint32_t w, uint32_t h);

	/**
	 * Set the font to be used to draw this text. Can change
	 * text bounds.
	 *
	 * @param fontFilename
	 *               Font to use.
	 */
	void SetFont( const char * fontFilename );

	/**
	 * Set the flags used to render this text.
	 *
	 * @param flags  Font alignment flags.
	 */
	void SetFlags( uint32_t flags );

	/**
	 * Get the current sprite flags.
	 *
	 * @return Current flags.
	 */
	uint32_t GetFlags() { return mFlags; }

	/**
     * Set the default text color.
	 *
	 * @param color  New text color.
	 */
	void SetColor(const TColor& color);

	/**
     * Get the default text color.
	 *
	 * @return Text color.
	 */
	const TColor & GetColor() const;

	/**
	 * Get the current text of the sprite.
	 *
	 * @return Current text.
	 */
	str GetText() const;

	/**
     * Set the current text of the sprite. Will return immediately if the
     * text hasn't changed.
	 *
	 * @param text   New text.
	 */
	void SetText( str text );

	/**
	 * Set the rendered height of a line of text in pixels; this
     * changes the font size. Text line wraps are re-calculated,
     * and tag locations are adjusted, based on the new size of the
     * font.
	 *
	 * @param newHeight New height in pixels.
	 */
	void SetLineHeight( uint32_t newHeight );

	/**
     * Query the height of the virtual text region.
	 *
     * @return The height text region width.
	 */
	uint32_t GetTextHeight() const;

	/**
	 * Query the width of the virtual text region.
	 *
	 * @return The virtual text region width.
	 */
	uint32_t GetTextWidth() const;

	TRect GetTextBounds( const TDrawSpec & environmentSpec=TDrawSpec() ) const;

	/**
     * Get the number of lines this text needs to be scrolled so
     * that the final line fits in the current text region.
     *
     * @param environmentSpec The parent environment of this sprite;
     *                        typically empty. Only relevant if a
     *                        parent sprite contains any scaling.
	 *
	 * @return Number of lines to scroll to get to bottom.
	 */
	uint32_t GetMaxScroll( const TDrawSpec & environmentSpec=TDrawSpec()  ) const;

	/**
	 * Get the number of lines in the text output.
	 *
	 * @return Number of lines.
	 */
	uint32_t GetLineCount() const;

	/**
	 * Set the first line in the text output.
	 *
	 * @param startLine Line to display as the first line of text.
	 *                  First line is 0.
	 */
	void SetStartLine( TReal startLine );

	/**
	 * Get the index of the first line of text output.
	 *
	 * @return An index between 0 and GetLineCount()-1.
	 */
	TReal GetStartLine() const;

	/**
	 * Set any additional padding between text lines. If you want
	 * "double spaced" text, you'd put the line height in this
	 * field.
	 *
	 * @param padding The number of pixels to add between lines. Can be negative
	 *                to squeeze text lines together.
	 */
	void SetLinePadding( int32_t padding );

	/**
	 * Set the current drawing origin. This value is overwritten
     * internally if you call SetTextRect() or SetFlags().
     *
     * Rotation is performed around this point, and scaling is
     * performed relative to this point.
     *
     * The default value for the origin depends on the horizontal
     * and vertical alignment flags. When "center" is used, it
     * defaults to the center on that axis; when one side or the
     * other is used, the origin will be aligned with that side.
     *
     * So left-top justified text will default to an origin of
     * (0,0), while right-bottom justified text will default to
     * (width,height). Fully centered text will default to
     * (width/2,height/2).
	 *
	 * @param origin The point on the virtual text surface to draw relative to.
	 *               Typically this point is between 0,0 and width,height of your
     *               virtual drawing surface, but that isn't
     *               required.
	 */
	void SetDrawingOrigin( const TVec2 & origin ) { mOrigin=origin; }

	/** @} */
private: // No copying!
	TTextSprite( const TTextSprite& );
	TTextSprite & operator=( const TTextSprite& );
private:
	uint32_t 	mWidth;
	uint32_t 	mHeight;

	uint32_t 	mFlags;

	int32_t		mLinePadding;

	TVec2		mOrigin;

	TTextGraphic *mTextGraphic ;
};

#endif // TEXTSPRITE_H_INCLUDED
