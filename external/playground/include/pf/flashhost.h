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
 * Interface for TFlashHost.
 */

#ifndef FLASHHOST_H_INCLUDED
#define FLASHHOST_H_INCLUDED

#ifndef PFLIBCORE_H_INCLUDED
#include "pflibcore.h"
#endif

struct IUnknown;

//------------------------------------------------------------------
class TFlashHostData;

/**
 * An embedded Flash-playback routine.
 *
 * Can be used in-game for cut-scenes. Flash animation will play
 * until it's "done", at which point control will return to the
 * game.
 *
 * From within Lua, you can call DisplaySplash() to display a
 * Flash animation and an optional replacement bitmap.
 *
 * To ensure that the Flash file plays correctly on the Mac, you
 * should:
 *
 * -# Have an 800x600 stage in your Flash content (assuming your
 *    display resolution is 800x600).
 * -# Have an empty first frame, and then place an 800x10
 *    rectangle shape cast member at (0,-30). (Again, assuming
 *    that 800 is the display width.)
 *
 * This will guarantee that the Flash stage size is calculated
 * correctly by Playground. The Mac version of Playground uses
 * WebKit to load an HTML file that loads an SWF file that loads the
 * Flash content (it's complicated, but necessary). Flash does
 * not have a method to determine the stage size of the loaded
 * clip, however. You can get the dimensions of the movie clip,
 * but these are computed as the bounding box of the objects
 * onstage, and this is what we are using when we decide how
 * much we need to scale the loaded content to fit 800x600.
 *
 * This is why the rectangle shape is necessary: So that
 * Playground can retrieve the 800 width and determine that it
 * doesn't need to scale the resulting Flash file.
 */
class PFLIB_API TFlashHost
{
public:
    /**
     * Constructor.
     *
     * @param filename Flash file to play.
     */
	TFlashHost(const char* filename);
    /**
     * Destructor.
     */
	~TFlashHost();

    /**
     * Start Flash file.
	 *
	 * Flash movies need to be in Flash 4 format to provide the most
	 * compatibility across all systems.
	 * Translatable movies need to be Flash 6.
	 *
	 * It is possible to translate a Flash movie by setting the
	 * bTranslate parameter to true. When translate mode is turned
	 * on, any string in the string table that begins with "FLASH_"
	 * will be sent to the Flash movie for translation.
	 * If all of the text in the Flash movie is set up as dynamic text,
	 * and each text box references a variable that is named after an
	 * entry in your strings.xml file (minus the "FLASH_"),
	 * then translation will succeed.
	 * You will also want to embed the font you want inside of your
	 * movie, so that if the user doesn't have the font installed on
	 * their system the font will still show up correctly.
	 *
	 * For example, if you have a dynamic text variable in your movie
	 * named "text_box_1", and have an entry in your strings.xml
	 * table named "FLASH_text_box_1", then at run time, "text_box_1"
	 * in your flash movie will be set to whatever the value of
	 * "FLASH_text_box_1" is in your strings.xml file.
	 *
     * It is recommended that inside the Flash movie itself,
	 * incomplete text is used when doing layouts. That way it will be obvious if
	 * it has been forgotten to add any text to the translation pipeline.
	 * If the text inside the movie is correct, it is not clear whether
	 * or not the movie is using the original text or the translated
	 * text until someone actually goes in to translate the
	 * strings.xml file.
	 *
     * @param bLoop  Loop file.
	 * @param bTranslate Send translated strings to the movie.
	 * @param bAllowInput If this is true, the system cursor will be shown
	 *                    and mouse clicks will be sent to the flash movie.
	 *                    If it is false, the cursor will be hidden and clicks
	 *                    will be sent to the game only.
     *
     * @return True on success.
     */
	bool		Start(bool bLoop = false, bool bTranslate = false, bool bAllowInput = false);
    /**
     * Stop playing file.
     */
	void		Stop();

    /**
     * Get the total number of frames.
     *
     * @return Number of frames.
     */
	long		GetTotalFrames();

    /**
     * Get the current frame number.
     *
     * @return Frame number.
     */
	long		GetFrameNum();

    /**
     * Test to see whether the animation is playing.
     *
     * @return True if playing.
     */
	bool		IsPlaying();

private:
	bool		StartMovie(IUnknown* control, bool bLoop, bool bTranslate);

	char*		mFilename; // not str, so easy to use in pfXtra
	TFlashHostData* mData;
};

//------------------------------------------------------------------
#endif // FLASHHOST_H_INCLUDED
