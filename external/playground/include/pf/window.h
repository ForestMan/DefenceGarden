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
 * Interface for class TWindow
 *
 * \ingroup windowgroup
 */

#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

#ifndef PLATFORM_H_INCLUDED
#include "platform.h"
#endif

#ifndef PFTYPES_H_INCLUDED
#include "pftypes.h"
#endif

#include <list>

#ifndef RECT_H_INCLUDED
#include "rect.h"
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#ifndef PFTYPEINFO_H_INCLUDED
#include "pftypeinfo.h"
#endif

#ifndef TAGGING_IGNORE

class TEvent ;
class TWindowSpider ;
class TWindowStyle ;
class TMessage ;
class TModalWindow ;
class TAnimTask ;

#endif // TAGGING_IGNORE (this prevents the above forward declarations from showing up in tag files)

// Worthless Windows #define
#undef GetFirstChild


/**
 * The TWindow class is the base class of any object that needs to
 * draw to the screen.
 *
 * As you create your own custom TWindow-derived classes, you should be aware that a TWindow "owns" its children: When a TWindow is
 * destroyed, it expects to be able to delete its children. Therefore a TWindow should never be allocated on the stack. If you want a
 * window to persist longer than its parent, you need to ensure that it is removed from the parent prior to the parent's destruction.
 *
 * \ingroup windowgroup
 */
class PFLIB_API TWindow
{
	PFTYPEDEFBASE_DC(TWindow);
public:
	/** \name Locally Defined Types
	 *
	 * Types defined in the TWindow scope.
	 *
	 *  @{
	 */

	/// A list of owned windows. Used for children.
	typedef std::list<TWindow*> WindowList ;

public:
	/**
	 * List of rectangles
	 */
	typedef std::list<TRect> RectList ;

	/** Static Window Types
	 *
	 * Flags that define what type and/or class a window is. These
	 * do not change after window creation.
	 */
	enum ETypeFlags
	{
		kModal 				= 0x00000001,	///< Flag that this window is modal
		kFocusTarget 		= 0x00000002,	///< Flag that this window can accept focus
		kInfrequentChanges	= 0x00000004,	///< Hint that we could cache this window. Only works if all ancestors are also flagged.
		kStartGroup			= 0x00000008,	///< This window is the start of a group of siblings (i.e. for radio buttons)
		kTypeMask			= 0x000000FF,	///< A flag mask that isolates the window types
	};

	/**
	 * Position constants for SetWindowDepth
	 *
	 * @see TWindow::SetWindowDepth
	 */
	enum EDepth
	{
		kBackMost,	///< Set this window to be the backmost window
		kFrontMost,	///< Set this window to be the frontmost window
		kOneHigher,	///< Set this window to be one higher than its current position
		kOneLower,	///< Set this window to be one lower than its current position
		kDepthCount,///< Number of depth options
	};

	/** Dynamic Window States
	 *
	 * States that may change frequently after window creation.
	 */
	enum EStateFlags
	{
		/// This window is enabled, and therefore can be rendered to
		/// and clicked upon.
		kEnabled 			= 0x00000100,
		kChecked 			= 0x00000200,	///< This window is in its "selected" or "checked" state.
		kCached 			= 0x00000400,	///< This window is rendered to the cache

/**
 * This window uses no alpha blending when it draws itself, and
 * covers its rectangle completely. It's important to set this
 * flag on a window when it's full screen and should completely
 * obscure the windows behind it--this will allow Playground
 * to prevent the deeper window from drawing.
 */
		kOpaque				= 0x00000800,
		kStateMask			= 0x0000FF00,	///< A flag mask that isolates the window states
	};

	/**
	 * Window drawing mode.
	 */
	enum EDrawMode {
		/**
		 * Draw all layers.
		 */
		eAll = 0,
		/**
		 * Draw only cacheable layers.
		 */
		eCached,
		/**
		 * Draw only dynamic layers.
		 */
		eDynamic
	};
	/** @} */
public:
	/** \name Construction and Initialization
	 * @{
	 */
	/**
	 * Default Constructor.
	 */
	TWindow();

	/// Destructor
	virtual ~TWindow();

	/**
     * Handle any initialization or setup that is required when
     * this window is assigned to a new parent. No initialization
     * of the window has happened prior to this call.
     *
     * @return True on success; false on failure.
     *
     * @see Init
	 * @see PostChildrenInit
	 */
	virtual bool OnNewParent();

    /**
     * Initialize the Window. Called by the system only in
     * Lua initialization.
     *
     * When you create your own custom window, this is where you put
     * your own custom initialization that needs to happen before
     * children are created. Fundamental window initialization is
     * handled in every class by this function, so <b>when you
     * override this function you almost always want to call your
     * base class to handle base class initialization.</b>
     *
     * @param style  The Lua style that was in effect when this window was
     *               created. This style contains all parameters specified
     *               explicitly for the window as well as parameters defined
     *               in the current style. Parameters set locally override
     *               ones in the style.
     */
	virtual void Init(TWindowStyle & style);

	/**
	 * Do post-children-added initialization when being created
	 * from Lua.
	 *
	 * Any initialization that needs to happen after a window's
	 * children have been added can be placed in a derived version
	 * of this function.
	 *
	 * @warning Remember to always call the base class if you're overriding
	 * this function.
	 *
	 * @param style  Current style environment that this window was created in.
	 */
	virtual void PostChildrenInit(TWindowStyle & style);
protected:
	/**
	 * A function that takes the default window parameters and
	 * applies them to the window's position and size.
	 *
	 * @advanced
	 *
	 * Called by TWindow::Init() <i>and</i> by TWindow::PostChildrenInit().
	 *
	 * Has no effect if it can't calculate the position and size
	 * based on current information available: If a position is
	 * set to kCenter, or a size set to kMax, but the parent window
	 * size hasn't yet been calculated, this function won't do
	 * anything.
	 *
	 * @note Implementation details.
	 *
	 * Since some windows set their size based on their calculated
	 * children's sizes (using TWindow::FitToChildren),
	 * TWindow::PostChildrenInit() needs to call this to adjust
	 * the position after the size has been calculated. Since
	 * other windows must have their full position and size
	 * specified in order to properly initialize, TWindow::Init()
	 * needs to call this function.
	 *
	 * @param style  The style of the window to apply.
	 */
	void SizeAndPositionFromStyle( TWindowStyle & style );
public:
	/** @} */

	/** \name Type Information and Casting
	 * @{
	 */

	/**
	 * @fn PFClassId ClassId()
	 * Get the ClassId.
	 * @return A ClassId that can be passed to IsKindOf.
	 * @see @ref rtti
	 */
	/** @fn virtual bool IsKindOf( PFClassId type )
	 * Determine whether this window is derived from type.
	 * @param type ClassId() of type to test.
	 * @see @ref rtti
	 */
	/**
	 * @fn template<class TO> TO * GetCast()
	 * Safely cast this window to another type.
	 * @return A cast pointer, or an empty reference.
	 * @see @ref rtti
	 */
#if DOXYGEN
	PFClassId ClassId();
	virtual bool IsKindOf( PFClassId type );
	template<class TO> TO * GetCast();
#endif

	/** @} */

	/** \name Update Functions
	 *
	 * Functions related to the drawing of windows.
	 *
	 *  @{
	 */

	/**
	 * Draw the window. Derived classes will override this
	 * function and provide the draw functionality.
	 */
	virtual void Draw();

	/**
     * Draw any overlays that should appear on top of this window's
     * children.
	 */
	virtual void PostDraw();

	/** @} */

	/** \name Child and Parent Window Functions.
	 *
	 * Functions to find child windows and to retreive and set
	 * parent windows.
	 *
	 *  @{
	 */

	/**
	 * Add a child to this window
	 *
	 * @warning If you override this in a derived class, be sure to call the
	 * base class to actually add the child from the list of children.
	 *
	 * @param child      Child that's being added.
	 * @param initWindow True to call OnNewParent().
	 *
	 * @return True if successful. On false, the window has NOT been adopted
	 *         and the calling class still has responsibility for destruction.
	 */
    virtual bool AdoptChild( TWindow * child, bool initWindow=true );

	/**
	 * Remove a child from this window.
	 *
	 * @warning If you override this in a derived class, be sure to call the
	 * base class to actually remove the child from the list of children.
	 *
     * @param child  Child that's being removed.
     *
	 */
	virtual void OrphanChild( TWindow * child );

	/**
	 * Destroy (delete) all child windows. Those windows will
	 * destroy their own children. Actual deletion is deferred
	 * using TWindowManager::SafeDestroyWindow(), so the windows
	 * will be actually deleted in the next event loop.
	 */
	void DestroyAllChildren();

	/**
	 * Fit this window to its childrens' sizes.
	 */
	void FitToChildren();

	/**
	 * Recursively find a child window from the given point.
     * The point is assumed to be inside <i>this</i> window.
     *
     * When searching for children, ChildWindowFromPoint() will
     * ignore any windows that are disabled (kEnabled is not
     * set).
	 *
	 * @param point  Point to test in client coordinates.
	 * @param depth  The number of times to recurse. 1 gives you only immediate
	 *               children. -1 gives you the deepest child. Defaults to 1.
	 *
	 * @return A pointer to the window containing the point.
	 *         If the point is not inside any of the
	 *         child windows, the function will return this.
	 */
	TWindow * ChildWindowFromPoint( const TPoint & point, int32_t depth=1 ) ;

	/**
	 * Find the nearest direct-ancestor modal window.
	 *
	 * @return A pointer to a modal window, or NULL if none is found.
	 */
	TModalWindow * FindParentModal();

	/**
	 * This method is called when this window's parent modal has been
	 * removed from the window stack. Because window deletion is delayed
	 * until it is safe to delete the window, this method can be used
	 * to detect immediately when a window has been removed from the
	 * stack, whereas the destructor will only be called when the window
	 * is actually deleted.
	 *
	 */
	virtual void OnParentModalPopped(){;}

#if _DEBUG
    str       GetWindowHierarchy( int32_t indent ) ;
#endif
private:
	/**
	 * Find an immediate child window from a point.
	 *
	 * @param point  Point to test in client coordinates.
	 *
	 * @return A pointer to the window. If the point is inside the
	 *         window but not in any child windows, the function will
	 *         return a TWindow* to this.
	 */
	TWindow * InternalChildWindowFromPoint( const TPoint & point ) ;

	/**
	 * Find the next child window from the point given.
	 *
	 * @param point  Point given.
	 * @param window Window iterator to start from
	 *
	 * @return The window found.
	 */
	TWindow * NextChildWindowFromPoint( const TPoint & point, WindowList::const_iterator window ) ;
public:

	/**
	 * Iterate through all children and call the Process()
	 * member function of TWindowSpider. Iteration happens in
	 * front-to-back order by default, or back-to-front if reverse is true
	 *
	 * @param spider The derived class which contains a Process() function
	 *               to be called on each child window.
	 * @param reverse If this is true, back-to-front order is used, default
	 *				 is false
	 */
	void ForEachChild( TWindowSpider * spider, bool reverse = false );

	/**
	 * Return true if this window has children.
	 *
	 * @return True if we're parents; false otherwise.
	 */
	bool HasChildren() { return mChildren.size()!=0 ;}

	/**
	 * Return the descendant window with the given name, if one
	 * exists.
	 *
	 * @param name		Window name to search for.
	 * @param depth  	Number of levels deep to look. Set to -1 for no limit.
	 * 					Defaults to -1.
	 *
	 * @return A TWindow to the descendant with the given name. Children
	 *         are searched recursively up to the level indicated in depth.
	 */
	TWindow * GetChildWindow( str name, int32_t depth=-1 ) ;

	/**
	 * Get the current window parent
	 *
	 * @return A shared pointer to the current parent window.
	 *
	 * @note This will return NULL if the current window has no
	 *         parent.
	 */
	TWindow * GetParent() const { return mParent ; }

	/**
	 * Reposition this window to be immediately in front of a
	 * given sibling.
	 *
	 * Pass the results of GetParent()->GetFirstChild()
	 * to bring this window to the front.
	 *
	 * @param inFrontOf  Sibling we should be visually in front of.
	 * 				     If NULL, will place this window in the back.
	 */
	void SetWindowDepth( TWindow * inFrontOf=NULL );

	/**
	 * Reposition this window to be in the position specified by the
	 * given constant.
	 *
     * @param depth  Enumeration that specifies a logical window
     *               depth.
     *
     * @see EDepth
	 */
	void SetWindowDepth( EDepth depth );

private:
	void _SetWindowDepth( EDepth depth, TWindow * inFrontOf );

public:

	/** @} */

	/** \name Window Coordinates and Rectangles.
	 *
	 *  Functions to calculate window point conversions relative
	 *  to two windows, and to acquire the window and client rectangles.
	 *
	 *  @{
	 */

	/**
	 * Get the rectangle that specifies the current window in
	 * top-level TScreen coordinates.
	 *
	 * @param rect   TRect to fill with resulting rectangle.
	 */
	void GetWindowRect(TRect * rect) const;

	/**
     * Set the position of the upper left corner of the window in
     * parent client coordinates.
     *
     * @param point New window position.
	 *
	 * @see SetWindowSize
	 * @see SetWindowDepth
	 */
	void SetWindowPos( const TPoint & point ) { SetWindowPos(point.mX,point.mY); }

	/**
     * Set the position of the upper left corner of the window in
     * parent client coordinates.
	 *
     * @param x New window x coordinate.
     * @param y New window y coordinate.
	 *
	 * @see SetWindowSize
	 * @see SetWindowDepth
	 */
	void SetWindowPos( int32_t x, int32_t y );

	/**
	 * Get the current window position.
	 *
	 * @return The current window position relative to its parent.
	 */
	const TPoint & GetWindowPos() const { return mRect.GetTopLeft(); }

	/**
	 * Set the size of the window.
	 *
	 * @param width  New window width.
	 * @param height New window height.
	 */
	void SetWindowSize( uint32_t width, uint32_t height );

	/**
	 * Get the "client" rectangle of the current window. This
	 * mimics the Windows functionality of getting a rect that
	 * has top and left set to 0, with right and bottom set to
	 * width and height, respectively.
	 *
	 * @param rect   The TRect to fill with the client rectangle.
	 */
	void GetClientRect( TRect * rect ) const ;

	/**
	 * Get the width of the client area of this window.
	 *
	 * @return Window client width in pixels.
	 */
	uint32_t GetWindowWidth() const { return mRect.GetWidth(); }

	/**
	 * Get the height of the client area of this window.
	 *
	 * @return Window client height in pixels.
	 */
	uint32_t GetWindowHeight() const { return mRect.GetHeight(); }

	/**
	 * Convert between top-level screen and client coordinates.
	 *
	 * @param point  in: Screen coordinates, out:client coordinates.
	 */
	void ScreenToClient( TPoint * point );

	/**
	 * Convert between top-level screen and client coordinates.
	 *
	 * @param rect  in: Screen coordinates, out:client coordinates.
	 */
	void ScreenToClient( TRect * rect )
	{
		ScreenToClient(&rect->GetTopLeft());
		ScreenToClient(&rect->GetBottomRight());
	}

	/**
	 * Convert between client and top-level screen coordinates.
	 *
	 * @param point  in: client coordinates, out: screen coorditates.
	 */
	void ClientToScreen( TPoint * point );

	/**
	 * Convert between client and top-level screen coordinates.
	 *
	 * @param rect  in: client coordinates, out: screen coorditates.
	 */
	void ClientToScreen( TRect * rect )
	{
		ClientToScreen(&rect->GetTopLeft());
		ClientToScreen(&rect->GetBottomRight());
	}

	/**
	 * Convert between parent and client coordinates.
	 *
	 * @param point  in: a point in parent's coordinate system, out:client coordinates
	 */
	void ParentToClient( TPoint * point ) const;

	/**
	 * Convert between parent and client coordinates.
	 *
	 * @param rect  in: a rect in parent's coordinate system, out:client coordinates
	 */
	void ParentToClient( TRect * rect ) const
	{
		ParentToClient(&rect->GetTopLeft());
		ParentToClient(&rect->GetBottomRight());
	}

	/**
	 * Convert between client and parent coordinates.
	 *
	 * @param point  in: client coordinates, out:a point in parent's coordinate system.
	 */
	void ClientToParent( TPoint * point );

	/**
	 * Convert between parent and client coordinates.
	 *
	 * @param rect  in: a rect in parent's coordinate system, out:client coordinates
	 */
	void ClientToParent( TRect * rect )
	{
		ClientToParent(&rect->GetTopLeft());
		ClientToParent(&rect->GetBottomRight());
	}

	/**
	 * Get the rectangle that represents this window in the client
	 * space of its parent window.
	 *
	 * @param rect   A rectangle to fill with the window's rectangle in
	 *               it's parent's coordinate system.
	 */
	void GetParentRelativeRect( TRect * rect ) const { *rect = mRect; }

	/**
	 * Get the rectangle that represents this window in the client
	 * space of its parent window.
	 *
	 * @return A reference to a TRect that describes this window in its
	 *         parents coordinates.
	 */
	const TRect & GetParentRelativeRect() const { return mRect; }

	/** @} */

	/** \name Window Information Accessors.
	 *
	 *  Functions to get or set information about a window.
	 *
     *  @{
	 */

	/**
	 * A virtual function to override if your window can scroll.
	 *
	 * @param vScroll Vertical scroll ratio (0.0-1.0).
	 * @param hScroll Horizontal scroll percentage (0.0-1.0).
	 */
	virtual void SetScroll( float vScroll, float hScroll=0 ) { (void)vScroll; (void)hScroll; }

	/**
	 * Get the window's state and style flags.
	 *
	 * @return Current state and style of the window.
	 */
	uint32_t	GetFlags()	const { return mFlags; }

	/**
	 * Set the state flags of the window. Does not change the
	 * "type" or style flags of the window.
	 *
	 * @param flags  Complete set of flags to update.
	 */
	void	SetFlags(uint32_t flags);

	/**
	 * Get the window name, if any.
	 *
	 * @return A str containing the window's name.
	 */
	str GetName() const { return mName; }

	/**
	 * Set the window name.
	 *
	 * @param name   New name for the window.
	 */
	void SetName( str name ) { mName = name; }

	/**
	 * Query this window's opacity.
	 *
	 * @return true if the window is "opaque": When it draws, none of the
	 *         background will show through. If any part of the window is
	 *         transparent, it should not have the kOpaque style set.
	 */
	bool IsOpaque() { return (mFlags& kOpaque)!=0; }

	/**
	 * Query this window's modal status.
	 *
	 * A modal window blocks further event handling by its parent
	 * and receives DoModalProcess() calls.
	 *
	 * @return Return true if this is a modal window,
	 *         false if it is not.
	 *
	 */
	bool IsModal() { return (mFlags & kModal)!=0 ; }

	/**
     * Return whether this window and all of its ancestors are
     * enabled.
	 *
     * @return True if this window is really enabled and (eventual)
     *         child of a TModalWindow.
	 */
	bool IsEnabled()
	{
		TWindow * w = this;

		// See if we (or a parent) were disabled.
		while (!w->IsModal())
		{
			// Are we disabled?
			if ( ((w->mFlags & kEnabled)==0) || (!w->GetParent()) )
			{
				// Then return false
				return false;
			}
			w= w->GetParent();
		}
		// OK, return whether the last one is enabled.
		return (w->mFlags & kEnabled)!=0;
	}

	/** @} */

	/** \name Event Handlers
	 *
	 * Functions to override to handle events in a window, and functions
	 * to trigger events on a window.
	 *
     *  @{
	 */

	/**
	 * Send a message to a window (or its ancestor). Takes ownership of
     * message and will delete it after it has been delivered.
	 *
	 * Calls OnMessage for this window and its parents until one
	 * returns "true", indicating the message has been handled. Stops
	 * searching at the first modal window.
	 *
	 * @param message   Message to send, including potential payload. Will be deleted after delivery.
	 */
	void SendWindowMessage( TMessage * message );

	/**
	 * Start a window animation. Can be called to reset an animation
     * delay. The virtual function OnTaskAnimate() will be called at
     * the frequency given by the parameters to StartWindowAnimation
     * until StopWindowAnimation is called or this window is
     * destroyed.
     *
     * This window must already be in a hierarchy and have a parent
     * TModalWindow to attach its animation to, or
     * StartWindowAnimation() will ASSERT (or crash in release
     * build). In other words, you cannot call this function in a
     * constructor.
	 *
	 * @param delay      Delay, in ms., before OnTaskAnimate will be called.
	 * @param autoRepeat True to cause delay to be auto-reset, i.e., to call
	 *                   OnTaskAnimate every delay ms. instead of just once.
	 * @param resetTime  Reset the time after each call. See TAnimTask::SetDelay for details.
	 * @param forceFrequency
     *                   Force the animation frequency. See TAnimTask::SetDelay for details.
     *
     * @see TWindow::StopWindowAnimation
     * @see TWindow::OnTaskAnimate
     */
	void StartWindowAnimation( int32_t delay, bool autoRepeat=true, bool resetTime=true, bool forceFrequency=false );

	/**
	 * Stop a window from receiving OnTaskAnimate calls.
	 * @see TWindow::StartWindowAnimation
	 * @see TWindow::OnTaskAnimate
	 */
	void StopWindowAnimation();

	/**
	 * Handle a message.
	 *
	 * @param message   Payload of message.
	 *
	 * @return True if message handled; false otherwise.
	 */
	virtual bool OnMessage(TMessage * message) { (void)message; return false ; }

	/**
	 * Called if you have initiated a window animation with
	 * TWindow::StartWindowAnimation.
	 *
	 * @return True to continue animating. False to stop.
	 */
	virtual bool OnTaskAnimate() { return false ; }

	/**
	 * Mouse down handler.
	 *
	 * @param point      Location of mouse press in client coordinates.
	 *
	 * @return true if message was handled, false to keep searching
	 *         for a handler.
	 */
	virtual bool OnMouseDown(const TPoint & point) { (void)point; return false ; };

	/**
	 * Extended mouse button handler.
	 *
	 * @param point  Location of mouse event in client coordinates.
	 * @param event Event that happened.
	 *
	 * @return true if message was handled, false to keep searching
	 *         for a handler.
	 */
	virtual bool OnExtendedMouseEvent(const TPoint & point, TPlatform::ExtendedMouseEvents event) { (void)point; (void)event; return false ; };

	/**
	 * Mouse up handler.
	 *
	 * @param point      Location of mouse release in client coordinates.
	 *
	 * @return true if message was handled, false to keep searching
	 *         for a handler.
	 */
	virtual bool OnMouseUp(const TPoint & point) { (void)point; return false ; };

	/**
	 * Mouse motion handler.
	 *
	 * @param point      Location of mouse in client coordinates.
	 *
	 * @return true if message was handled, false to keep searching
	 *         for a handler.
	 */
	virtual bool OnMouseMove(const TPoint & point) { (void)point; return false ; };

	/**
	 * Notification that the mouse has left the window.
	 *
     * @warning This message is only sent if TWindowManager::AddMouseListener()
	 * has been called for this window previously.
	 *
	 * @return True if handled.
	 */
	virtual bool OnMouseLeave() { return false ; };

	/**
	 * Returns true if this window can accept the keyboard
	 * focus.
	 *
	 * Override to return true if your derived window
	 * can accept focus.
	 *
	 * @return 	Return true if this window can accept keyboard
	 * 			focus. If it can accept keyboard focus, it should
	 * 			respond to the On*Focus() message to update its
	 * 			appearance when its focus state changes.
	 */
	virtual bool CanAcceptFocus() { return false; }

	/**
	 * Translated character handler.
     *
     * In UTF-8 mode, this function will only be called if
     * OnUTF8Char() returns false, or there is no OnUTF8Char()
     * handler.
     *
	 * @param key    Key hit on keyboard, along with shift translations.
	 * @return true if message was handled, false to keep searching
     *         for a handler.
     *
	 * @see TPlatform::kUTF8Mode
	 */
	virtual bool OnChar(char key) { (void)key; return false ; };

	/**
     * UTF-8 Translated character handler.
	 *
     * This function is ONLY active when UTF-8 support has been
	 * enabled.
	 *
     * @param key   Key hit on keyboard, along with shift
     *              translations.
     * @return true if message was handled, false to keep searching
     *         for a handler.
     *
     * @see TPlatform::kUTF8Mode
	 */
	virtual bool OnUTF8Char(str key) { (void)key; return false ; };

	/**
	 * Raw key hit on keyboard.
	 *
     * @param key    Key pressed on keyboard. Returns either a
     *               (low-ASCII) key name, or one of the constants
     *               defined in the \ref keycodes "Key Codes"
     *               section.
     *
     * @param flags TEvent::EKeyFlags mask representing the state
     *               of other keys on the keyboard when this key was
     *               hit.
	 *
	 * @return true if message was handled, false to keep searching
	 *         for a handler.
	 */
	virtual bool OnKeyDown(char key,uint32_t flags) { (void)key;(void)flags; return false ; };

	/**
	 * Raw key released on keyboard.
	 *
	 * @param key    Key released.
	 * @return true if message was handled, false to keep searching
	 *         for a handler.
	 */
	virtual bool OnKeyUp(char key) { (void)key; return false ; };

	/**
	 * Called if the mouse hovers over a point on the window.
	 *
	 * @param point  Point the mouse was last hovering over.
	 *
	 * @return True if processed; false to keep looking.
	 */
	virtual bool OnMouseHover( const TPoint & point );

	/**
	 * This window is receiving the keyboard
	 * focus.
	 *
	 * @param previous The window that was previously focused.
	 *                 Can be NULL.
	 */
	virtual void OnSetFocus( TWindow * previous ) { (void)previous; }

	/**
	 * This window is losing the keyboard focus.
	 *
	 * @param newFocus	The window that's receiving focus.
	 */
	virtual void OnKillFocus( TWindow * newFocus ) { (void)newFocus; }

	/** @} */

	/** \name Internal Utility Functions
	 *
	 *  Functions that are only called internally by the library.
	 *
	 *  @internal Documentation
	 *
     *  @{
	 */

	/**
	 * Internal event handling function.
     *
	 * @warning This function should normally only be called by the library.
	 *
     * @warning NOTE it's currently not virtual. If it's ever made
     *              virtual, it will likely be VERY important for
     *              derived classes to call the base class function.
	 *
	 * @param eIn				Event to process.
	 * @param focusChanged
	 *               true if this event has already caused a focus change, so that this
	 *							event will only cause one focus change total.
	 *
	 * @return True if the event is handled; false otherwise.
	 */
	bool HandleEvent(const TEvent* eIn, bool focusChanged = false);

	/**
	 * Get a unique window identifier.
	 *
	 * @return The ID of the window.
	 */
	str GetID() const { return str::getFormatted("%8x",mID); }

#if !USER_DOCS
	/**
     * @nouser
     *
	 * Internal Window drawing function.
	 *
	 * @warning This function should normally only be called by the library to
	 * 			trigger a rendering pass.
	 */
	bool DoDraw(TWindow::EDrawMode mode);

	/**
     * @nouser
     *
	 * Function that calculates proper position and size given
	 * center and right/bottom flags.
	 *
	 * @param pos     Pointer to the position (x or y)
	 * @param size    Pointer to the size (w or h)
	 * @param getsize Pointer to TWindow::GetWindowWidth or TWindow::GetWindowHeight.
	 * @param center  True to center.
	 * @param faredge True to right/bottom align.
	 */
	void FixSize( int32_t * pos, int32_t * size, uint32_t (TWindow::*getsize)() const, bool center=false, bool faredge=false, bool alignPresent=false );
#endif
protected:

	/**
	 * Set the current parent window.
	 *
	 * @param newParent A TWindow * to the new parent window.
	 *
	 * @return A TWindow * to the old parent window.
	 */
	TWindow * SetParent(TWindow *newParent);

	/** @} */

	/**
	 * Return whether this window is enabled, looking at parents,
	 * but ignoring whether it descends from a TModalWindow.
	 *
	 * @return True if enabled.
	 */
	bool IsEnabledSimple()
	{
		TWindow * w = this;

		// See if we (or a parent) were disabled.
		while (!w->IsModal() && w->GetParent())
		{
			// Are we disabled?
			if ( ((w->mFlags & kEnabled)==0) )
			{
				// Then return false
				return false;
			}
			w= w->GetParent();
		}
		// OK, return whether the last one is enabled.
		return (w->mFlags & kEnabled)!=0;
	}


protected:
	/**
	 * Function that allows a derived window to add type flags
	 * to the TWindow. This isn't public because in most circumstances a
	 * window's type should be invariant once created.
	 *
	 * @param type   Flags to add.
	 */
	void	AddWindowType( uint32_t type ) { mFlags |= type; }

#ifndef TAGGING_IGNORE
	friend class TWindowAnim ;		///< A TWindowAnim can poke inside us.
#endif

	/**
	 * Get the associated TAnimTask. A TAnimTask is associated with
	 * this TWindow when StartWindowAnimation() is called.
	 *
	 * Will create a new TAnimTask if StartWindowAnimation() hasn't
	 * been called yet. In that case, you may want to call
	 * SetDelay() on it, or call StartWindowAnimation() with the
	 * parameters you want.
	 *
     * @return A pointer to a TAnimTask, or NULL if none exists.
     * @see StartWindowAnimation
	 */
	TAnimTask		*GetWindowAnim();
	// Window data
private:
	TWindow 		*mParent ;		///< Our parent
protected:
	/// We own our children. Our descendants can play with our children, though.
	WindowList		mChildren ;

private:
	TRect			mRect ;			///< Current window rectangle in PARENT coordinates.

	str				mName ;			///< Name of window (can be empty/NULL).
	uint32_t		mFlags ;		///< Window flags (styles and states).

	uint32_t		mID ;			///< Window ID (for Lua tracking)

	TWindowAnim		*mWindowAnim;	///< A window-attached animation.
private:
	/// Disabled copy constructor
	TWindow(const TWindow & wnd);
	/// Disabled assignment operator
	TWindow operator=( const TWindow & wnd);

	virtual bool Init(){ return false; };

};

/**
 * A class used with TWindow::ForEachChild to iterate over the children
 * of a window with a single "callback" function.
 *
 * \ingroup windowgroup
 */
class PFLIB_API TWindowSpider
{
public:
	/**
	 * Virtual destructor
	 */
	virtual ~TWindowSpider();

	/**
	 * The function called once for each window.
	 *
	 * @param window The window being iterated.
	 *
	 * @return True to continue the traversal.
	 */
	virtual bool Process( TWindow * window )=0 ;
};

#endif // WINDOW_H_INCLUDED
