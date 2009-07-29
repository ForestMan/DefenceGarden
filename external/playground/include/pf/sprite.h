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


#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#ifndef DEBUG_H_INCLUDED
#include "debug.h"
#endif

#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

#ifndef DRAWSPEC_H_INCLUDED
#include "drawspec.h"
#endif

#ifndef PFTYPEINFO_H_INCLUDED
#include "pftypeinfo.h"
#endif

#ifndef PFTYPES_H_INCLUDED
#include "pftypes.h"
#endif

#include <list>

#ifndef TAGGING_IGNORE
class TTaskList;
#endif

/**
 * A 2d sprite object. A TSprite functions both as an individual
 * display object and a container for child TSprite objects. Typically
 * a TSprite with no TTexture is used as a container for all of the
 * TSprites in one screen of a game.
 * 
 * @par How to Position a Sprite
 * 
 * Using TSprite::GetDrawSpec(), you retreive a TDrawSpec class
 * that contains an mMatrix that governs how this sprite is
 * drawn. It contains a position (mMatrix[2]), and a
 * rotation/scale (see TMat3 for more information). The
 * TDrawSpec also allows you to tint a sprite, flip it in the x
 * or y axis, or change the logical center (the point it scales
 * from and rotates around).
 * 
 * See the TDrawSpec documentation for more information.
 * 
 * @par TSprite References
 * A TSprite is always stored in the client as a
 * TSpriteRef&mdash;it's a reference counted object. To destroy
 * a TSprite, simply call reset() on the last TSpriteRef that
 * refers to that sprite.
 *
 * It's worth noting that a TSprite holds a reference to any of its
 * children, so you don't need to worry about keeping an external reference
 * to a child sprite that can be passively attached to an object. An
 * example of this usage would be a shadow that stays at a constant offset
 * from the parent sprite. To make the shadow appear behind the parent you
 * can give it a negative "layer" when you create it.
 * 
 * @par Design Concerns
 * Because it's convenient, many people tend to want to use
 * TSprite as a game object. It's not designed that way,
 * however, and you'll quickly run into limitations if you try.
 * For instance, there's no way to enumerate children of a
 * TSprite. There's also no way to name a TSprite, and then look
 * up a child by name.
 * 
 * This is intentional. TSprite was designed as a very light
 * class, and adding strings and the ability to iterate would
 * weigh it down.
 * 
 * Another option you might consider is deriving your own class
 * from TSprite. It has been requested by developers so often,
 * that it's used as an example in the docs. I've come to
 * believe that creating a game object that way is categorically
 * a bad idea. As Playground has grown, new child classes of
 * TSprite have been created, so if you derive a game object
 * from TSprite, you won't be able to have a game object that
 * is, say, a TFxSprite or a TTextSprite. Aside from that, a
 * TSprite really is all about rendering a TTexture, and mixing
 * game code with your rendering code is poor design when it can
 * be avoided.
 * 
 * If your game objects exist in their own hierarchy, however,
 * and they just own a reference to the TSprite that represents
 * them in the game, then your game objects can be represented
 * by whatever TSprite-derived object we (or you) devise, and
 * the design of your game will now be cleaner, since the
 * details of the rendering of your game objects will be
 * properly encapsulated in its own class and not mixed
 * willy-nilly in the game object behavior classes.
 * 
 * So that's why you should create your own hierarchy of game
 * objects distinct from the TSprite hierarchy. Having two
 * parallel hierarchies is not ideal, but weighing down TSprite
 * with the plumbing required to promote it to a full-fledged
 * game object is even less desirable.
 * 
 * @par How Sprites are Drawn
 * A TSprite has a concept of a layer which indicates how it will be
 * rendered relative to its siblings.  For the top sprite in a hierarchy
 * (the one you're calling Draw on yourself), the layer parameter is
 * meaningless&mdash;it only applies to sprites with siblings or
 * a parent.  The layer determines the relative order of drawing
 * of a sprite with its siblings and parent.
 *
 * Here are the rules that determine the order of drawing:
 *
 *  - When siblings have the same layer, they will render in an arbitrary
 *    order&mdash;it is assumed not to matter what order they're
 *    in.
 *
 *  - When siblings have different layers, the higher layered siblings will
 *    be rendered above the lower layered siblings.
 *
 *  - When a child sprite has a negative layer, it will be rendered <em>behind</em>
 *    its parent; otherwise it will be rendered in front of its parent.
 *
 * Changing the layer of a sprite requires that the parent's child list be resorted.
 * In other words,  it's a relatively heavy operation if there are a lot of siblings,
 * so try not to do it frequently.
 *
 * @see TAnimatedSprite
 */
class PFLIB_API TSprite : public enable_shared_from_this<TSprite>
{
	PFSHAREDTYPEDEFBASE(TSprite);
public:
    /**
     * A list of sprites.
     */
	typedef std::list<TSpriteRef> SpriteList ;
protected:
	/// Internal Constructor. Use TSprite::Create() to get a new sprite.
	TSprite(int32_t layer);
public:
	/**
	 * @name Initialization/Destruction
	 * @{
	 */

	/**
	 * Allocation of a new sprite. Construction is restricted
	 * to help "encourage" the use of TSpriteRefs to hold
	 * your TSprites (as well as to encapsulate the TSpriteRef
	 * creation pattern).
	 *
	 * @param layer Initial sprite layer.
	 * @param texture  Initial sprite texture.
	 *
	 * @return A newly allocated TSprite wrapped in a TSpriteRef.
	 *
	 * @see SetLayer
	 * @see SetTexture
	 */
	static TSpriteRef Create(int32_t layer=0,TTextureRef texture=TTextureRef());

	/// Destructor.
	virtual ~TSprite();

	/** @} */

	/**
	 * @name Drawing and Layers
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
     *                 drawspec&mdash;the frame of reference that
     *                 this sprite is to be rendered in. In general
     *                 you shouldn't pass anything in for this
     *                 parameter and instead should modify the
     *                 position of the sprite using its
     *                 GetDrawSpec() member.
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
	 * Set the layer of the sprite. A relatively expensive call;
	 * use with care.
	 *
	 * The sprite's layer determines the order in which it will be rendered
	 * relative to its immediate parent and siblings. A negative layer will be
	 * rendered behind its parent, while a positive layer will be rendered in front.
	 * Higher layer numbers are rendered in front of lower layer numbers.
	 *
	 * @param layer New sprite layer.
	 */
	void SetLayer( int32_t layer );

	/**
     * Get the current sprite layer.
     */
	int32_t GetLayer() { return mLayer; }

	/** @} */

	/**
	 * @name Parent/Child Access and Management
	 * @{
	 */

	/**
	 * Add a child sprite. Children are drawn relative to
	 * the parent sprite: When you set the position, it will
	 * be added to the position of the parent.
	 *
	 * The child should <i>not</i> be added independently to the
	 * sprite manager.
	 *
	 * Children with a negative sprite layer are drawn behind the
	 * parent sprite. Zero or positive layers are drawn after the
     * parent sprite.
	 *
	 * @param child  The sprite to add as a child of this sprite.
	 */
	void AddChild( TSpriteRef child );

    /**
     * Remove a child sprite. Returns true if child found.
     *
     * @param sprite Child to remove.
     *
     * @return True if child found.
     */
	virtual bool RemoveChild(TSpriteRef sprite);

	/**
	 * Release all children of the sprite.
	 *
	 * "If you love them, set them free."
	 */
	void RemoveChildren();

	/** @} */

	/**
	 * @name Bounding Rectangles and Hit Tests.
	 * @{
	 */

	/**
	 * Get the bounding rectangle of this sprite.
	 *
	 * @param parentContext
     *                The parent context to test within&mdash;where
     *                is this sprite being drawn, and with what
     *                matrix? Alpha and color information is
     *                ignored.
     * @param depth	 Depth of children to test
     *
     * @return Rectangle that includes this sprite.
	 */
	virtual TRect GetRect( const TDrawSpec & parentContext, int32_t depth=-1 );

	/**
     * Test to see if a point is within our sprite. Will test
     * children as well, unless depth is set to zero.
     * 
     * By default will <i>not</i> test children unless the TPlatform
     * setting TPlatform::kSpriteAlwaysTestChildren is enabled with
     * TPlatform::SetConfig().
     * 
	 * @param at      Point to test.
	 * @param parentContext
     *                The parent context to test within&mdash;where
     *                is this sprite being drawn, and with what
     *                matrix? Alpha and color information is
     *                ignored.
	 * @param opacity Level of opacity to test for; -1 for a simple
	 *                bounding box test, or 0-255 for alpha color
     *                value, where 0 is transparent (and will
     *                therefore always succeed).
	 * @param depth   Depth of children to test. Zero means only test this sprite.
     *                -1 means recurse children as deep as they go.
	 *
	 * @return true if point hits us.
	 */
	virtual bool HitTest( const TPoint & at, const TDrawSpec & parentContext, int32_t opacity=-1, int32_t depth=-1 );

	/** @} */

	/**
	 * Set the texture of the sprite object.
	 *
	 * @param texture Texture to use.
	 */
	virtual void SetTexture( TTextureRef texture );

	/**
	 * Get the current texture.
	 *
	 * @return A reference to the current texture.
	 */
	TTextureRef GetTexture() { return mTexture; }

	/**
	 * Get the associated TDrawSpec.
	 *
	 * @return A modifiable reference to the TDrawSpec associated with this sprite.
	 */
	TDrawSpec & GetDrawSpec() { return mDrawSpec; }

	/**
     * Get a const reference to the associated TDrawSpec.
	 *
     * @return A non-modifiable reference to the TDrawSpec
     *         associated with this sprite.
	 */
	const TDrawSpec & GetDrawSpec() const { return mDrawSpec; }

	/**
	 * Set a sprite to be visible and enabled. Sprites are initially visible.
	 *
     * @param visible True to set visible.
	 */
	void SetVisible( bool visible );

	/**
	 * Is the sprite visible/enabled?
	 *
	 * @return True if it's enabled.
	 */
	bool IsVisible() { return mEnabled; }

    /**
     * Comparison operator for layer sorting.
     *
     * @param first  Left hand item to compare.
     * @param second Right hand item to compare.
     *
     * @return True if first sprite has a lower layer than the second.
     */
	friend bool operator<( TSpriteRef first, TSpriteRef second ) { return first->mLayer < second->mLayer ; }

	/**
	 * Get the current parent of this sprite.
	 *
	 * @return A pointer to the current parent, or NULL if this sprite is
	 *         free.
	 */
	TSprite * GetParent() { return mSpriteParent; }
protected:
    /**
     * Resort my children because one of them has changed
     * layer (priority).
     */
	virtual void ResortSprites();
private:
	friend class TSpriteManager ;

	/// The sprite that owns us, if any.
	TSprite * mSpriteParent ;

	/// True when a sprite is enabled
	bool	mEnabled ;

	/// Sprite layer.
	int32_t		mLayer ;

	TTextureRef mTexture ;

	TDrawSpec	mDrawSpec ;

protected:
	/// Sprite children
	SpriteList  mChildren ;
};

#endif // SPRITE_H_INCLUDED
