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
 * Interface for TText class.
 * \ingroup windowgroup
 */

#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

//------------------------------------------------------------------
#ifndef WINDOW_H_INCLUDED
#include "window.h"
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#ifndef TEXTGRAPHIC_H_INCLUDED
#include "textgraphic.h"
#endif

//------------------------------------------------------------------
// Forward declarations

#ifndef TAGGING_IGNORE

class TTextGraphic;

#endif

//------------------------------------------------------------------
// Main class declaration

/**
 * A text window. Contains a TTextGraphic and renders it
 * transparently.
 *
 * @see TTextGraphic
 *
 * \ingroup windowgroup
 */
class PFLIB_API TText : public TWindow
{
	PFTYPEDEF_DC(TText,TWindow);
public:
	/// TText window child flags
	enum EFlags
	{
		kHAlignLeft 	= TTextGraphic::kHAlignLeft,	///< Align horizontally with the left edge.
		kHAlignCenter 	= TTextGraphic::kHAlignCenter,  ///< Align horizontally with the center.
		kHAlignRight 	= TTextGraphic::kHAlignRight,   ///< Align horizontally with the right edge.
		kVAlignTop 		= TTextGraphic::kVAlignTop,     ///< Align vertically with the top.
		kVAlignCenter 	= TTextGraphic::kVAlignCenter,  ///< Align vertically with the center.
		kVAlignBottom 	= TTextGraphic::kVAlignBottom   ///< Align vertically with the bottom.
	};

	/**
	 * Constructor.
	 *
	 * @param staticText True if this is a static text field.
	 */
	TText(bool staticText=false);

	/**
	 * Destructor.
	 */
	~TText();

	/**
	 * Create a TText window.
	 *
	 * @param text       Initial text for window.
	 * @param w          Width of client rectangle.
	 * @param h          Height of client rectangle.
	 * @param flags      Flags from TText::EFlags
	 * @param fontFilename
	 *                   Font name.
	 * @param lineHeight Font size.
	 * @param textColor  Font color.
	 *
	 * @return True on success.
	 */
	bool Create(str text, uint32_t w, uint32_t h, uint32_t flags,const char* fontFilename, uint32_t lineHeight, const TColor& textColor);

	/**
	 * TWindow::Draw handler.
	 */
	virtual void Draw();

	/**
	 * Get the number of lines in the text output.
	 *
	 * @return Number of lines.
	 */
	uint32_t	GetLineCount();

	/**
	 * Set the first line in the text output.
	 * Lines are 0 -> linecount-1.
	 *
	 * @param startLine Line to display as the first line of text.
	 */
	void		SetStartLine(TReal startLine=0) { mTextGraphic->SetStartLine(startLine); };

	/**
	 * Get the actual boundary of the rendered text.
	 *
	 * @param pBounds A rectangle in client coordinates.
	 */
	void		GetTextBounds(TRect* pBounds);

	/**
	 * Set the current text content
	 *
	 * @param text   Text to set.
	 */
	virtual void SetText( str text );

	/**
	 * Get the current text content.
	 *
	 * @return A string containing the current text.
	 */
	virtual str GetText() { return mTextGraphic->GetText(); }

	/**
	 * Get the associated TTextGraphic object.
	 *
	 * @return The TTextGraphic that draws this window's text.
	 */
	TTextGraphic * GetTextGraphic() { return mTextGraphic ; };

	/**
	 * Set the current text color.
	 *
	 * @param color  New text color.
	 */
	void SetColor(const TColor& color);

	/**
	 * Set the text alpha.
	 *
	 * @param alpha  Opacity of text. 1.0==opaque. Multiplied by alpha component of color.
	 */
	void SetAlpha( TReal alpha );

	/**
	 * Set the current text scale. Default is 1.0. Useful for zooming effects.
	 *
	 * @param scale  New text scale.
	 */
	void SetScale(TReal scale) {mScale = scale;};

	/**
	 * Set the current line padding. Default is 0. Can be positive or negative - extends/compresses a fonts natural line spacing
	 *
	 * @param linePadding  New Line Padding.
	 */
	void SetLinePadding(int32_t linePadding) { mLinePadding = linePadding; };

	/**
	 * Mouse up handler.
	 *
	 * Used to detect clicks on embedded text links. Returns false if
	 * no text link was previously clicked on.
	 *
	 * @param point      Location of mouse release in client coordinates.
	 *
	 * @return true if message was handled, false to keep searching
	 *         for a handler.
	 */
	virtual bool OnMouseUp(const TPoint &point);

	/**
	 * Mouse down handler.
	 *
	 * Used to detect clicks on embedded text links. Returns false if
	 * no text link is found.
	 *
	 * @param point      Location of mouse press in client coordinates.
	 *
	 * @return true if message was handled, false to keep searching
	 *         for a handler.
	 */
	virtual bool OnMouseDown(const TPoint &point);

	/**
	 * Mouse motion handler.
	 *
	 * @param point      Location of mouse in client coordinates.
	 *
	 * @return true if message was handled, false to keep searching
	 *         for a handler.
	 */
	virtual bool OnMouseMove(const TPoint & point);

	/**
	 * Notification that the mouse has left the window.
	 *
     * @warning This message is only sent if
     * TWindowManager::AddMouseListener() has been called for this
     * window previously.
	 *
	 * @return True if handled.
	 */
	virtual bool OnMouseLeave();

	/**
	 * Change the rotation - default is 0 degrees, 0,0 origin
	 *
	 * @param degrees   rotation angle in degress (not radians because degress are friendlier)
	 * @param originX	offset from left of text rect to use as center point of rotation
	 * @param originY	offset from top of text rect to use as center point of rotation
	 */
	void SetRotation(TReal degrees, int32_t originX, int32_t originY);

	/**
	 * Get the maximum line you need to scroll the text to in order
	 * to display the last line of text.
	 *
	 * @return The highest integer you need to set the top line to.
	 */
	uint32_t GetMaxScroll();

	/**
	 * A virtual function to override if your window can scroll.
	 *
	 * @param vScroll Vertical scroll percentage (0.0-1.0).
	 * @param hScroll Horizontal scroll percentage (0.0-1.0).
	 */
	virtual void SetScroll(TReal vScroll, TReal hScroll=0);

	/**
	 * This value controls how far past (in lines) the end
	 * of the text the scroll can go. The default value is 0.5f,
	 * so this means that if you called SetScroll(1.0f), then
	 * the text would scroll so it was 0.5 lines up off the
	 * bottom of the window.
	 *
	 * @param pad How many lines to pad the scrolling text
	 */
	void SetScrollPadding(TReal pad){mScrollPadding = pad;}

    virtual void Init(TWindowStyle &style);

private:

	TTextGraphic 	*mTextGraphic ;
	TReal			mScale;
	TReal 			mAlpha;
	int32_t			mLinePadding;
	TReal			mScrollPadding;
	str				mLinkPressed ;
};

//------------------------------------------------------------------
#endif // TEXT_H_INCLUDED
