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
 * Interface for class TLayeredWindow
 * \ingroup windowgroup
 */

#ifndef LAYEREDWINDOW_H_INCLUDED
#define LAYEREDWINDOW_H_INCLUDED

#ifndef WINDOW_H_INCLUDED
#include "window.h"
#endif

/**
 * A TLayeredWindow is a TWindow with multiple layers which can be
 * switched between. This can be used for animation, or for button presses,
 * for instance.
 * \ingroup windowgroup
 */
class PFLIB_API TLayeredWindow : public TWindow
{
	PFTYPEDEF(TLayeredWindow,TWindow);
public:
	enum
	{
		kAll = -1
	};

	/**
	 * Default Constructor
	 *
	 * @param numberOfLayers
	 *               Number of layers to create.
	 */
	TLayeredWindow(uint32_t numberOfLayers);

	/// Destructor
	virtual ~TLayeredWindow();

	/**
     * Select the layer of this window to display or add children
     * to. Children added when one layer is selected will be added
     * only to that layer, and therefore only displayed when that
     * layer is selected.
	 *
	 * @param layer  The new active layer. Set to kAll to add children to all layers;
	 *               layer 0 will be displayed in that state.
	 */
	void SetCurrentLayer( int32_t layer=kAll );

	/**
	 * Get the current active layer.
	 *
	 * @return An zero-based index to the current active layer.
	 */
	int32_t GetCurrentLayer() { return mCurrentLayer; }

	/**
	 * Get the number of layers.
	 *
	 * @return The number of layers associated with this window.
	 */
	uint32_t GetNumLayers() { return mNumberOfLayers ; }

	/**
	 * Remove a child from this window, releasing ownership.
	 *
	 * Removes this child from <I>all</I> window layers.
	 *
	 * @param child  child to remove.
	 *
	 * @see TWindow::OrphanChild
	 */
	virtual void OrphanChild(TWindow * child);

    /**
     * Add a child to this window. Adds it to the current
     * window layer only.
     *
     * @param child      Child to add.
     * @param initWindow Whether to run the init function OnNewParent() on this window.
     *
     * @return True if successful. False if OnNewParent() returned
     *         false, in which case child was NOT added.
     *
     * @see TWindow::AdoptChild
     */
	virtual bool AdoptChild(TWindow * child, bool initWindow=true);
private:
	int32_t			    mCurrentLayer ;
	uint32_t		    mNumberOfLayers ;
	WindowList 			*mLayers ;

};

#endif // LAYEREDWINDOW_H_INCLUDED
