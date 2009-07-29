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
 * Interface for class TAnimatedSprite
 */


#ifndef ANIMATEDSPRITE_H_INCLUDED
#define ANIMATEDSPRITE_H_INCLUDED

#ifndef SPRITE_H_INCLUDED
#include "sprite.h"
#endif

#ifndef REF_H_INCLUDED
#include "ref.h"
#endif

#ifndef TAGGING_IGNORE
class TAnimatedSprite ;
class TAnimatedSpriteData ;
class TClock;
class TScript;
struct lua_State;
#endif

typedef shared_ptr<TAnimatedSprite> TAnimatedSpriteRef ;	///< A reference to a TAnimatedSprite

/**
 * A TSprite with an attached TScript. Similar to TSprite, a
 * TAnimatedSprite should only ever be stored as a reference, but
 * it will work to store one in either a TAnimatedSpriteRef or
 * a TSpriteRef.
 *
 * Typically you will assign a TAnimatedTexture to a
 * TAnimatedSprite; however, it is legal to assign a normal
 * TTexture to a TAnimatedSprite instead. Obviously the
 * animation script will be unable to change "frames" if
 * a normal TTexture is attached, however.
 */
class PFLIB_API TAnimatedSprite : public TSprite
{
	PFSHAREDTYPEDEF(TSprite);
protected:
	/// Default Constructor.
	/// @param layer The initial layer for the sprite.
	TAnimatedSprite(int32_t layer);
public:
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
	virtual bool HitTest(const TPoint &at, const TDrawSpec &parentContext, int32_t opacity=-1, int32_t depth=-1);
	/**
	 * Get the rect of this sprite.
	 *
	 * @param parentContext
	 *                The parent context to test within--where is this sprite being drawn,
	 *                and with what matrix? Alpha and color information is ignored.
	 * @param depth	 Depth of children to test
	 *
	 * @return Rectangle that includes this sprite.
	 */
	virtual TRect GetRect(const TDrawSpec &parentContext, int32_t depth=-1);
	/**
     * @name Initialization/Destruction
	 * @{
	 */

	/**
	 * Factory
	 *
	 * @param layer Layer of sprite.
	 *
	 * @return A reference to a new sprite.
	 */
	static TAnimatedSpriteRef Create( int32_t layer=0 );

	/**
	 * Set the animation to use the passed in clock as its timer. To be effective,
	 * must be called before a call to Play() is issued.
	 *
	 * @param clock  Clock to use for timing. If NULL, then the global timer is used.
	 */
	void SetClock(TClock *clock);

	/// Destructor
	virtual ~TAnimatedSprite();

	/** @} */

	/**
	 * @name Drawing
	 * @{
	 */

	/**
     * Draw the sprite and its children.
	 *
     * @param drawSpec The 'parent' drawspec--the frame of reference
     *                 that this sprite is to be rendered in.
     *                 Defaults to a default-constructed TDrawSpec.
     *                 See TDrawSpec for more details on what is
     *                 inherited.
     * @param depth     How many generations of children to draw; -1
     *                  means all children.
     *
     * @see TDrawSpec
	 */
	virtual void Draw(const TDrawSpec & drawSpec=TDrawSpec(), int32_t depth=-1);

	/** @} */

	/**
	 * @name Animation Control
	 * @{
	 */

	/**
	 * Plays an animation script
	 *
	 * @param functionName
	 *               Name of lua function to run. By default this is "DoAnim"
	 */
	void Play(str functionName = "DoAnim");

	/**
	 * Stops an animation script that's already playing.
	 *
	 * Scripts will retain global variable settings even after
	 * they're stopped.
	 */
	void Stop();

	/**
	 * Pauses/un-pauses a specific animation. An alternative to calling Pause() on several
	 * different TAnimatedTextures is to have them all use the same TClock, and pause the
	 * clock instead.
	 *
	 * @param pause true to pause/false to un-pause
	 * @param depth
	 *               How many levels to recurse in modifying child animations.
	 */
	void Pause(bool pause, int32_t depth=-1);

	/**
     * Stop and eradicate the script associated with an animation.
     *
     * Any variables saved in the global environment will be erased.
     * The next time you call Play() or GetScript() it will reload
     * any associated TAnimatedTexture() script.
     */
	void Die();

	/**
     * Test whether or not the animation is currently playing.
     *
	 * @param depth
     *               How many levels to recurse in testing child
     *               sprite animations to see if they're playing.
     *               Default is -1 which means to recurse with no
     *               limit.
	 *
	 * @return True if the animation or one of its children is playing.
	 */
	bool IsPlaying(int32_t depth=-1);

	/**
     * Return whether or not the animation has signalled it's really
     * done by calling die().
	 *
	 * @param depth
	 *               How many levels to recurse in testing child animations.
	 *
	 * @return true if the animation is done.
	 */
	bool IsDone(int32_t depth=-1);

	/** @} */

	/**
	 * @name Frame Access
	 * @{
	 */

	/**
	 * Set the current animation frame.
	 *
	 * Does not affect children.
	 *
	 * @param frame Frame number.
	 */
	void SetCurrentFrame(int32_t frame);

	/**
	 * Get the current animation frame.
	 *
	 * @return Current animation frame number.
	 */
	int32_t GetCurrentFrame();

	/** @} */

	/**
	 * @name Script and Texture Access
	 * @{
	 */

	/**
	 * Gets the current script associated with this animation.
     * Creates a script if one doesn't exist already.
     *
     * DO NOT CACHE this pointer: this script can change
     * over time. As long as you don't call Die() on this
     * animation or reload it from a file, it will copy its
     * environment across scripts, so you can set global variables
     * and be reasonably assured that when you hit "Play" it will
     * retain them--even if it's running in a different TScript
     * (technically a different thread).
     *
	 * @return The current script.
	 */
	TScript * GetScript();

	/**
	 * Reset the script to a virgin one that has been initialized with
	 * the proper animation functions.
	 *
	 * Not necessary to call prior to LoadScript or InitXML, as these
	 * functions will call it if no script has been loaded. However,
	 * if you want a clean interpreter state, you can call this function.
	 */
	void NewScript();

    /**
     * @internal
     *
     * @deprecated This function will be made private in a future
     * version of Playground.
     *
     * Create a new playback thread. Kills current playback thread,
     * if any.
	 */
	void NewThread();

	/**
	 * Set the texture of the sprite object. Can be a TAnimatedTexture or a
	 * regular TTexture; if it's a TTexture, the script and animation control
	 * functions will only be relevant to any child sprites that have TAnimatedTextures
	 * assigned.
	 *
	 * @param texture Texture to use.
	 */
	virtual void SetTexture(TTextureRef texture);

	/**
	 * Return an animated texture, if one is attached. If no
	 * texture is attached, or if a normal TTexture is attached,
	 * return an empty reference.
	 *
	 * @return A TAnimatedTextureRef, if one is bound to this sprite.
	 */
	TAnimatedTextureRef GetAnimatedTexture();

	/**
	 * Get the number of anchors in the bound animation.
	 *
	 * @return Number of anchors. Returns zero if there is no animated
	 *         texture attached.
	 */
	uint32_t GetNumAnchors();

	/**
	 * Get the number of frames in the bound animation.
	 *
	 * @return Number of frames in the animation. Returns one (1) if there
	 *         is no animated texture attached.
	 */
	uint32_t GetNumFrames();
	/** @} */

	/**
     * @name Utility
	 * @{
	 */

	/// Get the TAnimatedSpriteRef for this TAnimatedSprite.
	TAnimatedSpriteRef GetRef() { return static_pointer_cast<TAnimatedSprite>( shared_from_this() ); }
	/** @} */

private:
	/// Lua exposed call to pause to next frame.
	int32_t WaitLuaNextFrame(lua_State * L);

	/// Wrapper for TAnimatedTexture::Get, to be called from Lua.
	bool Init(str animationName, str imageOverride, str maskOverride);

	/// Local random number generator; to be removed from final version
	/// in favor of TRandom.
	static uint32_t Random( uint32_t min, uint32_t max );
	void RemoveSprite();

	void SetAlpha( float alpha );

	TAnimatedSpriteData * mData;
};

#endif // ANIMATEDSPRITE_H_INCLUDED
