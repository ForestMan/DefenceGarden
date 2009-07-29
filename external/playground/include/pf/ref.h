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
 * Definitions for reference-counted pointer class.
 *
 * @see \ref shared_ptr_docs
 */

/**
 * @page shared_ptr_docs Reference-Counted Pointers
 *
 * @section refintro How to Share and Play Well With Others
 *
 * When writing a game, some objects have clear owners: In the
 * case of a button, it's fine to allow the enclosing window to
 * own it and clean it up when it's destroyed. But there are
 * some situations where an object or resource has no clear
 * owner. The texture to draw on the button is one simple
 * example: You wouldn't want the button to destroy the texture
 * when it's deleted for fear another button on the screen is
 * still using it.
 *
 * One solution to this problem is to use reference-counted
 * pointers: Smart containers that keep track of how many
 * references are currently held to them. The above example
 * would keep the button texture around until the last button
 * window holding the texture container was deleted. Additional
 * tricks can allow you to keep a list of currently loaded
 * textures, so that when each button requests its texture they
 * all get shared instances of the same texture.
 *
 * You can use reference-counted pointers to manage your own
 * game objects as well, but it's important to thoroughly
 * understand the implications of using smart pointers before
 * using them. It's not really that difficult, but there are a
 * few common traps that you need to be aware of in order to
 * avoid stumbling into them.
 *
 * @section refusing Using shared_ptr (TClassRef) Classes
 *
 * When you include pf/ref.h, you gain access to the
 * \c shared_ptr template class. Several Playground classes use
 * \c shared_ptr internally, and provide convenience typedefs of
 * the form TFooRef, indicating that it's a reference-counted
 * container for a Foo. A few common examples include:
 *
 * - \c TTextureRef
 * - \c TModelRef
 * - \c TSoundRef
 * - \c TSpriteRef
 *
 * The full set of Playground \c shared_ptr typedefs is defined
 * in pf/forward.h. In each case where Playground intends you to
 * use \c shared_ptrs, the associated class has a static member
 * factory named Get() (or prefixed by Get) when the class
 * manages sharing of instances of the asset, or Create() for
 * when you need a new, unique item. See TTexture::Get() or
 * TTexture::Create() for two examples.
 *
 * Part of the reason to have factories like this instead of
 * allowing you to \c new objects and assign them to
 * \c shared_ptrs is to help avoid common
 * reference-counted-pointer errors by never encouraging you to
 * operate on the raw pointers. If you always hold one of these
 * objects in an appropriate Ref (e.g., TTextureRef) container
 * for as long as you intend to use it, passing it from place to
 * place as a Ref, and never converting it to a pointer at all,
 * then it's really hard to make a mistake that would cause the
 * reference counting mechanism to fail.
 *
 * @section refmanaging Managing Assets
 *
 * Another common strategy to handle assets is to have an asset
 * pool that a particular portion of your game can draw assets
 * from. Sometimes that portion is the entire game, where you
 * can load up assets at the beginning during a loading screen.
 * Sometimes you want to load some assets at the start of a
 * level, and then release them when the next level starts.
 *
 * Playground also supports this pattern: Create a TAssetMap for
 * your level, or for the whole game, or both, and use it to
 * hold references to all of your game assets. When you're done
 * with that portion of the game, destroy the %TAssetMap and all
 * of those assets will be released. Here's the best part: If
 * you "load" the next section of your game before you delete
 * the old %TAssetMap, any assets in common will simply be
 * referenced and not reloaded!
 *
 * You can use a %TAssetMap in either a strict mode where it
 * requires that any asset you request through it must already
 * be referenced, or in a more forgiving mode where you can
 * request an asset it doesn't have yet and it will both load
 * and hold a reference to the asset. See
 * TAssetMap::SetAutoLoad() for details.
 *
 * @section refmistakes Common Mistakes
 *
 * So what are the common mistakes you need to be aware of?
 * Well, here's one:
 *
 * @code
 * // This is BAD! Don't do it!
 * TTexture* someTexture = TTexture::Get("foo.png").get();
 * @endcode
 *
 * Assuming the texture isn't referenced elsewhere, the code
 * above will assign a dangling pointer to someTexture. By the
 * time the assignment occurs, the texture object will have been
 * destroyed.
 *
 * @code
 * // Correct
 * TTextureRef someTexture = TTexture::Get("foo.png");
 * @endcode
 *
 * Keeping it in a TTextureRef ensures that you keep a
 * reference to it when the temporary on the right hand side
 * of the assignment operator is destroyed.
 *
 * @code
 * // This is also BAD! Don't do it either!
 * void Foo( TTexture* texture )
 * {
 *      TTextureRef textureRef(texture); // ERROR!
 * }
 *
 * TTextureRef someTexture = TTexture::Get("foo.png");
 * Foo( someTexture.get() ); // BAD BAD BAD
 *
 * @endcode
 *
 * In the example above, when someTexture leaves scope, it will
 * delete the texture. Funny thing is, when textureRef leaves
 * scope, it will <i>also</i> delete the texture. This is not
 * considered a good thing. If you have a good memory debug tool
 * in place, you'll find this right away because it will alert
 * you right when it happens. Without malloc debugging active,
 * some time later, in an unrelated part of your game, it will
 * probably crash when allocating or freeing memory. And where
 * it crashes may change between debug and release builds,
 * because the memory allocators work differently in debug and
 * release. In other words, this is a bug you never want to
 * lay the groundwork for by using raw pointers when you should
 * be using \c shared_ptrs.
 *
 * Again, simply passing the reference around as a reference solves
 * the problem:
 *
 * @code
 * // Correct
 * void Foo( TTextureRef texture )
 * {
 *      ...
 * }
 *
 * TTextureRef someTexture = TTexture::Get("foo.png");
 * Foo( someTexture ); // No pointer necessary
 *
 * @endcode
 *
 *
 * @section refcustom Your Very Own shared_ptr
 *
 * The \c shared_ptr template class can be used to manage game assets as
 * well.  If you're using \c shared_ptrs in your own classes, though, you
 * can end up with a cyclic reference that never gets freed: \c A holds a
 * reference to \c B and vice versa.  When all references to \c A and \c B
 * are freed, you still have these two referring to each other.  All is
 * not lost, however.  One or both of these references can be a "weak"
 * reference.  If \c A owns \c B, but \c B needs a reference back to \c A,
 * then you can make the pointer in \c A a \c shared_ptr and the one in \c
 * B a weak_ptr.  That way when the last reference to \c A is released, it
 * will destroy \c A.  If another object still holds a reference to \c B,
 * it won't be destroyed when \c A releases its reference--but \c B's weak
 * reference to \c A will magically NULL itself, so that \c B knows \c A
 * has been destroyed.
 *
 * @section refimpl Implementation Details
 *
 * Technical details for the current implementation can be found at
 * http://boost.org/libs/smart_ptr/smart_ptr.htm
 *
 * Feel free to look at the documentation there to understand
 * more about the philosophy and design decisions behind smart
 * pointers. However, we are using a subset of the full Boost
 * libraries, so don't expect all of the features they describe
 * to be available. We do use weak_ptr internally to handle our
 * Get() functions (so we know if an asset has already been
 * loaded, and to correctly return the existing \c shared_ptr),
 * but we reserve the right to reimplement the \c shared_ptr if
 * we decide there's a compelling reason.
 *
 */

#ifndef REF_H_INCLUDED
#define REF_H_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#if !USER_DOCS

/**
 * @internal
 *
 * @class shared_ptr
 * A reference-counted pointer container template.
 *
 */
using boost::shared_ptr ;

/**
 * @internal
 *
 * @class weak_ptr
 * A weak-reference-counted pointer container template. A weak
 * reference will not prevent an object from being deleted. When
 * the object is deleted, the weak reference will go "NULL".
 */
using boost::weak_ptr ;

/**
 * @internal
 *
 * @class scoped_ptr
 * An auto-delete of a pointer kept on the stack.
 */
using boost::scoped_ptr ;

/**
 * @fn static_pointer_cast
 * Static cast a shared_ptr to another kind of shared_ptr.
 *
 * The classes pointed to must be related.
 *
 * Example:
 *
 * \code
 * TAsset a = ... ;
 * TTextureRef t = static_pointer_cast<TTexture>(a) ;
 * \endcode
 */
using boost::static_pointer_cast ;

/**
 * @internal
 *
 * @class enable_shared_from_this
 *
 * An optional base class that you may derive your class from if you
 * need to be able to extract the current shared reference count
 * from a simple pointer to the class.
 *
 * In practice, this is necessary if you need to retrieve a shared_ptr
 * from a member function of a class. If a class will never need to do
 * this, there is no need to derive from enable_shared_from_this.
 *
 * See documenation at http://www.boost.org for details.
 */
using boost::enable_shared_from_this ;

#endif


#endif // REF_H_INCLUDED
