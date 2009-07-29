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
 * Interface for class TTextEdit
 * \ingroup windowgroup
 */


#ifndef TEXTEDIT_H_INCLUDED
#define TEXTEDIT_H_INCLUDED

#include "text.h"
#include "clock.h"


/**
 * The TTextEdit class represents an editable text TWindow.
 *
 * When creating from Lua, in addition to the TWindow tags, you
 * can specify the following tags:
 *
 * @warning Once this class finds an ancestor modal window, it
 *          will register itself with the modal and link itself
 *          to any other TTextEdit windows it finds under that
 *          modal window. If you then remove it (or a
 *          parent) from the hierarchy without destroying it or
 *          calling Unregister(), the resulting behavior is
 *          <b>undefined</b>.
 *
 * \ingroup windowgroup
 */
class PFLIB_API TTextEdit : public TText
{
	PFTYPEDEF_DC(TTextEdit,TText);
public:
	/// Default Constructor
	TTextEdit();

	/// Destructor
	virtual ~TTextEdit();

    /// Key category.
	enum eKeyType
	{
		kKeyChar,   ///< Key is a normal character.
		kKeyMove,   ///< Key is a cursor or backspace character.
		kKeyEnter,  ///< Key is enter or return.
		kKeyTab,    ///< Key is the tab character.
		kKeyPaste,  ///< Key is the "Paste" character.
		kKeyIllegal ///< Key is illegal.
	};

	/**
	 * Virtual function to notify child that a key was pressed.
	 *
	 * @param type   key type
	 * @param key	 key that was hit when appropriate
	 *
	 * @return True to accept the key. False to ignore.
	 */
	virtual bool KeyHit( eKeyType type, char key=0 );

	/**
     * New virtual function that supports UTF-8 characters. Will
     * return false for any UTF-8 character that is not in the
     * current font.
	 *
	 * @param type   Type of key event.
	 * @param key    Actual key hit.
	 *
	 * @return True to accept the key. False to ignore.
	 */
	virtual bool KeyHitUTF8( eKeyType type, uint32_t key=0 );

	virtual bool OnNewParent();
	virtual bool OnKeyDown(char key,uint32_t flags);
    virtual void Init(TWindowStyle &style);
	virtual bool OnChar(char key);
	virtual bool OnUTF8Char(str key);

	/**
	 * Tell this TTextEdit that it should unlink itself from its
     * parent modal window and any related TTextEdit windows in its
     * tab ring.
	 */
	void Unregister();

	/**
	 * Sets the textedit field into password mode, meaning that
	 * the displayed text will be all asterisks.
	 *
	 * @param bPassword - true to enable, false to disable
	 *
	 *
	 */
	void SetPassword(bool bPassword);

	/**
	 * Set this field to be editable. Enables the cursor, which
	 * will display when the TTextEdit is focused.
	 *
	 * @param bEditable True to make the field editable.
	 */
	void SetEditable(bool bEditable);

	/**
	 * Query as to whether this TTextEdit is currently editable.
	 *
	 * @return True if editable.
	 */
	bool	GetEditable() { return mEditable; }

	/**
	 * Update the text in the text field.
	 */
	void UpdateText();

	/**
	 * Call this function to simulate pressing "Backspace" in the
	 * text field.
	 */
	void Backspace();

	/**
	 * Any character in ignoreStr will be ignored and not entered
	 * into the textedit field.
	 *
	 * @param ignoreStr A string of characters to ignore.
	 */
	void SetIgnoreChars(str ignoreStr);

	/**
	 * Get the characters this TTextEdit is ignoring.
	 *
	 * @return A string that contains the characters to ignore.
	 */
	str GetIgnoreChars(){ return mIgnoreChars; }

	/**
	 * Get the current editable text.
	 *
	 * @return The text in the edit box.
	 */
	virtual str GetText();

	/**
	 * Set the current editable text.
	 *
	 * @param newText Text to set to.
	 */
	virtual void SetText( str newText );

	/**
	 * Function to draw dynamic elements.
	 */
	virtual void Draw();

    /// Speed of the cursor flash.
	static const int kCursorFlashMS=800 ;

    /// Default length if none given.
    static const int kDefaultMaxLength=10 ;

    /**
     * Set the maximum length of the input field. Current field
     * is not evaluated to test the new length.
     *
     * @param maxLength Maximum number of characters you can type.
     */
	void	SetMaxLength(uint32_t maxLength) {mMaxLength = maxLength;};

	/**
	 * Turn on UTF-8 key entry mode. This allows the TTextEdit
	 * field to take any arbitrary UTF-8 character. At present
	 * only characters that have glyphs will be displayed.
	 *
	 * @param utf8mode True to enable UTF-8 mode.
	 */
	void SetUTF8Mode( bool utf8mode );
protected:

	/**
	 * Animate the cursor flashing.
	 *
	 * @return True to continue animating. False to stop.
	 */
	virtual bool OnTaskAnimate();

    /// Maximum number of characters in a string.
    uint32_t mMaxLength ;

	/// Retrieve the current cursor position
	uint32_t GetCursor() { return mCursor; 	}

	/**
	 * Set the current cursor position.
	 *
	 * @param cursor New cursor position.
	 */
	void SetCursor(uint32_t cursor) { if (cursor<=mText.length()) mCursor=cursor; else mCursor=mText.length(); }

private:
	bool 			DispatchKey( eKeyType type, str key );
	void CheckRegistration();
   	TTextEdit( const TTextEdit & );
	TTextEdit & operator=( const TTextEdit & );

	typedef std::list<TTextEdit*> TextRing ;
	typedef shared_ptr<TextRing> TextRingRef ;
	friend class TextSpider;

	TextRingRef mTextRing ;

	str		mText ;
	uint32_t mCursor ;
	bool	mEditable;
	bool	mUTF8 ;
	bool	mPassword;
	bool	mCursorVisible;
	str		mIgnoreChars;
	TClock	mCursorTimer ;
};

#endif // TEXTEDIT_H_INCLUDED
