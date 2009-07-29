//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef SLIDER_H_INCLUDED
#define SLIDER_H_INCLUDED

#ifndef WINDOW_H_INCLUDED
#include "window.h"
#endif

#ifndef MESSAGE_H_INCLUDE
#include "message.h"
#endif

#ifndef TAGGING_IGNORE
class TImage;
struct TTransformedLitVert;
#endif

//------------------------------------------------------------------
// Main class declaration

/**
 * Slider class.
 *
 * A class that implements a UI slider that is scalable. It consists of two
 * parts, the rail and the slider. The rail consists of three graphics, the top,
 * middle, and bottom. The middle is stretched to be as long as it needs to match
 * the height or width of the slider. You can set the width or height of the slider
 * to be the long dimension, and the other dimension will be taken from the width of
 * the rail images.
 *
 * Attributes:
 *
 * - \c railtop - Top of the rail. Rail graphics are vertical.
 * - \c railmid - Middle of the rail. This part gets stretched.
 * - \c railbot - Bottom of the rail.
 * - \c sliderimage - Image to use as the slider handle.
 * - \c sliderrollimage - Image to use as the rollover for the slider handle.
 * - \c yoffset - An offset to use to adjust the centering of the slider handle.
 *
 */
class PFLIB_API TSlider : public TWindow
{
	PFTYPEDEF_DC(TSlider,TWindow);
public:
	/// The current state of the slider
	enum ESliderState {eIdle = 0, eMoving};

	TSlider();
	~TSlider();

	/**
	 * Show the slider widget. Defaults to being visible.
	 *
	 * @param show   True to show handle.
	 */
	void ShowHandle( bool show );

	/**
	 * Draw the window. Derived classes will override this
	 * function and provide the draw functionality.
	 *
	 * Optionally only redraw portions that have been
	 * invalidated since the previous draw.
	 *
	 */
	virtual void Draw();
    /**
     * Set the textures for the rail. The textures are oriented
     * vertically, as if for a vertical slider, and will be
     * rotated counter-clockwise for horizontal sliders.
     *
     * To ensure compatiblity across renderers, the height
     * each texture should be a power of two and greater than
     * its width.
     *
     * @param top    Top of the slider.
     * @param mid    Middle of the slider. This one gets stretched
     *               out to make the slider the right height or
     *               width.
     * @param bot    Bottom of the slider.
     */
	void SetRailTexture( TTextureRef top, TTextureRef mid, TTextureRef bot );

    /**
     * Set the textures for the slider knobs.
     *
     * @param texture  Normal state slider knob.
     * @param rollover Rollover state slider knob.
     */
	void SetSliderTexture( TTextureRef texture, TTextureRef rollover );

	/**
	 * Get the current slider handle position.
	 *
	 * @return The position as a value from 0 to 1.
	 */
	TReal GetValue();

    /**
     * Set the current slider handle position.
     *
     * @param value  Position from 0 to 1 inclusive.
     * @param silent True to prevent a "slider-changed" message from being sent.
     */
	void SetValue(TReal value, bool silent= false);

    /**
     * Set the slider scale.
     *
     * @param scale  The scale of the slider.
     */
    void SetScale(TReal scale)
    {
        mScale = scale;
    }

	/**
	 * Get the current slider scale.
	 *
	 * @return Slider scale.
	 */
	TReal GetScale() { return mScale; }

	/**
	 * Set the current slider alpha.
	 *
	 * @param alpha  Alpha of the slider.
	 */
	void SetAlpha( TReal alpha ) ;

	/**
	 * Get the current slider alpha.
	 *
	 * @return The slider alpha.
	 */
	TReal GetAlpha() { return mAlpha; }

    /**
     * Mouse handler.
     *
     * @param point  Mouse position.
     *
     * @return True if handled.
     */
	virtual bool OnMouseDown(const TPoint & point);
    /**
     * Mouse handler.
     *
     * @param point  Mouse position.
     *
     * @return True if handled.
     */
	virtual bool OnMouseUp(const TPoint & point);
    /**
     * Mouse handler.
     *
     * @param point  Mouse position.
     *
     * @return True if handled.
     */
	virtual bool OnMouseMove(const TPoint & point);
    /**
     * Mouse handler.
     *
     * @return True if handled.
     */
	virtual bool OnMouseLeave();

    /**
     * Register TSlider with the windowing system.
     */
	static void Register();

    /**
     * Window initialization handler.
     *
     * @param style  Window style.
     */
	virtual void Init(TWindowStyle & style);


	/**
	 * Get the current slider state (whether it's moving or idle).
	 *
	 * @return The state of the slider.
	 */
	ESliderState GetState() { return mState; }

private:
	int BuildTexture( TTextureRef tex, int offset, TTransformedLitVert * verts, bool stretch );
	void SetSliderSize();
	void UpdateSliderPos();

	ESliderState	mState;
	bool			mInitialized;
	//TImage			*mRailImage;
	TImage			*mSliderImage;
	TTextureRef		mSliderTextures[2];
	enum
	{
		kRailTop,
		kRailMid,
		kRailBot,
		kRailNumTextures
	};
	TTextureRef		mRailTextures[3];
	TReal			mSliderValue;
	TReal			mScale;
	TReal			mAlpha;

	int				mSliderOffset ;
	int				mSliderWidth ;

	bool			mHSlider ;
};

//------------------------------------------------------------------
#endif // SLIDER_H_INCLUDED
