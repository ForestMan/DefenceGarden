//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef PLATFORM_H_INCLUDED
#define PLATFORM_H_INCLUDED

//------------------------------------------------------------------
#ifndef PFLIBCORE_H_INCLUDED
#include "pflibcore.h"
#endif

#ifndef PFTYPES_H_INCLUDED
#include "pftypes.h"
#endif

#ifndef RECT_H_INCLUDED
#include "rect.h"
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

#ifndef TASKLIST_H_INCLUDED
#include "tasklist.h"
#endif

#ifndef MAT_H_INCLUDED
#include "mat.h"
#endif

#include <map>

#ifndef TAGGING_IGNORE
class TTask ;
class TPlatformData;
class TEvent;
class TMatrixStack;
class TVertexSet ;
class TRenderer;

struct TLight ;
#endif

#include <float.h>
#include <math.h>

//------------------------------------------------------------------
#ifndef PI
#define PI 3.141592654f
#endif


#define MAX_FILENAME 512


//------------------------------------------------------------------
/**
 * The platform-specific functionality encapsulation class.
 *
 * This class is created within the library and exists in one
 * global instance that can be acquired anywhere in the application
 * using the static function TPlatform::GetInstance().
 */
class PFLIB_API TPlatform
{
private:
	static TPlatform	* mPlatform ;
public:

	/**
	 * These mouse button constants are here to allow you to respond
	 * to the use of other mouse buttons. PlayFirst game design
	 * constraints forbid the use of any other than the left mouse
	 * button as a "necessary" part of the user interface. In other
	 * words, any other buttons on the mouse or mouse wheel needs to
	 * be a supplemental interface for convenience of power users.
	 */
	enum ExtendedMouseEvents
	{
		kMouseRightUp,		///< Mouse right-button-up event.
		kMouseRightDown,	///< Mouse right-button-down event.
		kMouseMiddleUp,		///< Mouse middle-button-up event.
		kMouseMiddleDown,	///< Mouse middle-button-down event.
		kMouseScrollLeft,	///< Mouse scroll-left-button event.
		kMouseScrollRight,	///< Mouse scroll-right-button event.
		kMouseScrollUp,		///< Mouse scroll-up-button event.
		kMouseScrollDown,	///< Mouse scroll-down-button event.
	};

	/**
	 * @name Configuration values
	 *
     * Use these values in GetConfig() to get the related setting,
     * or with SetConfig() to change the related setting (for
     * writable values).
     *
     * @see TPlatform::GetConfig
     * @see TPlatform::SetConfig
	 *
	 * @{
	 */

	/// Unique Computer Identifier
	/// @see GetConfig
	static const char * kComputerId;

	/**
	 * Cheat mode. Be sure to set your kEncryptionKey before
	 * querying this value.
	 *
	 * @see GetConfig
	 * @see kEncryptionKey
	 */
	static const char * kCheatMode;

	/**
	 * In a Web game, this is the URL that the browser should be
	 * set to when you click the "Download" button in the game.
	 */
	static const char * kDownloadURL;

	/**
	 * A key unique to the install of this application.
	 *
	 * Used to detect which portal the game was installed from.
	 */
	static const char * kInstallKey;

	/**
	 * The application's encryption key.
	 *
	 * The application MUST set this key to a key assigned by
	 * PlayFirst to function correctly with preferences, high scores,
	 * and the cheat enabler.
	 *
	 * @see kCheatMode
	 */
	static const char * kEncryptionKey;

	/**
	 * Query as to whether hiscore mode is local-only.
	 */
	static const char * kHiscoreLocalOnly;

	/**
	 * Query as to whether hiscore mode is anonymous only.
	 */
	static const char * kHiscoreAnonymous;

	/**
	 * Query how this particular build has been tagged. Build tagging
	 * may be used to enable/disable certain feature sets.
	 */
	static const char * kBuildTag;

	/**
     * Query whether this build is a "first peek" build:  A
     * limited-functionality public beta version.
	 */
	static const char * kFirstPeek ;

    /**
     * What is the name of this game? Defined to be the string
     * "gamename".
     *
     * Set this in your "Main" routine to set the name your game
     * should report to the high score server. If you want to change
     * the name your game gets in the data folder, you must call
     * this function from PlaygroundInit().
     *
	 * @see PlaygroundInit
	 */
	static const char * kGameName;

    /**
	 * What is the name of the publisher of this game?  Defaults to
	 * "PlayFirst". This value is used to determine part of the path
	 * for the the user: and common: folders.
     *
     * In order for this setting to have an effect on the user
     * folders for the application, it must be set in the special
     * initialization routine PlaygroundInit().
     *
     * @see PlaygroundInit
	 */
	static const char * kPublisherName;

	/**
     * Call TPlatform::SetConfig( TPlatform::kTextGraphicSpriteRender, 1 ) to
     * default all new TTextGraphic instances to "sprite rendering"
     * mode.
     *
	 * @see TTextGraphic::SetSpriteRender
	 */
	static const char * kTextGraphicSpriteRender;

	/**
     * What version/build is this EXE? Defined to be the string
     * "version". Call TPlatform::GetConfig(
     * TPlatform::kGameVersion) to query the version number of your
     * game. Do not read version.h directly.
     */
	static const char * kGameVersion;

	/**
     * This value is the game handle that the game uses to communicate
	 * with any PlayFirst services, such as the hiscore system.
	 * This value should be set with the PFGAMEHANDLE value in key.h
	 * on application startup. See the Playground Skeleton sample for
	 * code that does this.
     */
	static const char * kPFGameHandle;

	/**
     * This value is the prefix to the game mode names used to
	 * communicate with the PlayFirst Hiscore system. To get the
	 * game mode name, you need to add on the number for the game
	 * mode you want, starting with index 1. So for example, to get
	 * the first game mode, you could ask for str(kPFGameModeName) + "1".
	 * This value should be set with the PFGAMEMODENAMES value in key.h
	 * on application startup. See the Playground Skeleton sample for
	 * code that does this.
     */
	static const char * kPFGameModeName;

	/**
     * This value is the prefix to the medal names used to
	 * communicate with the PlayFirst Hiscore system. To get the
	 * medal name, you need to add on the number for the game
	 * mode you want, starting with index 1. So for example, to get
     * the first medal, you could ask for str(kPFGameMedalName) +
     * "1". This value should be set with the PFGAMEMEDALNAMES value
     * in key.h on application startup. See the Playground Skeleton
     * sample for code that does this.
     */
	static const char * kPFGameMedalName;

	/**
     * Instruct Playground to wait for the vertical blanking period
     * to start prior to drawing to the screen in windowed mode. Has
     * no effect on full-screen mode, which always flips its buffer
     * at the vertical refresh.
	 *
	 * Can minimize "tearing" when enabled. Does cause the game
	 * to pause to wait for the screen vertical refresh to happen,
     * which can seriously slow down the frame rate of your game by
     * causing it to skip frames when the time to compute core
     * logic and render a frame takes slightly longer than one video
     * frame to calculate.
	 *
	 * For instance, if the game takes 1/60 of a second (16.66ms)
	 * to perform all calculations and finish rendering,
	 * but the monitor is set to a 70Hz update, setting this
	 * flag will cause the screen to update at only 35FPS
     * (half of 70FPS), since by the time one frame is complete,
     * it will have already missed the next video synchronization.
     * As a result, the game will need to wait for the following
     * frame vertical refresh to draw. If you can only draw once
     * every two frames at 70Hz, you'll get a 35Hz (35FPS) update.
	 *
	 * Set the value to "1" to enable, or "0" to disable this
     * feature. Defaults to being disabled.
	 */
	static const char * kVsyncWindowedMode ;

	/**
	 * Enable UTF-8 support mode.
	 *
     *  - Enables TWindow::OnUTF8Char() dispatch.
     *  - When enabled, OnChar() will only be called for low-ASCII
     *    characters, and only if no OnUTF8Char() has already
     *    handled the character.
     *
	 * @warning This mode has not yet been approved for use in
     * PlayFirst-developed games.
     *
     * @warning Must be enabled in PlaygroundInit() section.
	 */
	static const char * kUTF8Mode ;

	/**
	 * Automatically merge any .mask.png files when loading a
	 * .jpg file. When you call:
	 *
	 * TTexture::Get("foo.jpg");
	 *
     * ...then if kAutoMergeMask is enabled, it will look for
     * foo.mask.png and, if found, automatically perform a
     * TTexture::GetMerged("foo.jpg", "foo.mask.png").
	 */
	static const char * kAutoMergeMask ;

	/**
	 * Enable mouse event routing to overlay windows.
	 */
	static const char * kOverlayWindowMouseEvents ;

	/**
	 * Enable verbose debug info from Playground.
	 */
	static const char * kVerboseDebug ;

	/**
	 * Query the current monitor frequency. Returns "0" if the
     * monitor frequency is not available. Can change when the
     * computer switches between full screen and windowed mode.
	 */
	static const char * kMonitorFrequency ;

	/**
	 * Raise the minimum major Flash version that your game
	 * requires.
	 *
	 * The value should be a version number in quotes. For instance,
	 * to require the Flash 8 control to be installed on a system,
	 * set the value to "8".
	 */
	static const char * kFlashMajorVersion ;


	/**
	 * Always do HitTest() on sprite children, even if the parent
	 * TSprite doesn't have a texture.
	 */
	static const char * kSpriteAlwaysTestChildren ;

	/**
	 * When using animated textures for particle systems, automatically
	 * adjust the position of each particle so that the animation
	 * frames play in the correct positions.
	 */
	static const char * kAdjustAnimatedParticles ;

    /**
     * Fix button mask scaling to properly scale a mask
     * to the size of the button.
	 */
	static const char * kFixButtonMaskScaling ;

	/**
	 * Enable this in PlaygroundInit() to enable high-precision
	 * floating point.
	 */
	static const char * kFPUPrecision ;

	/**
	 * Enabling this will decompress any non-streamed .ogg sounds into memory,
	 * rather than keeping them compressed. This will reduce the time it takes
	 * to decompress the sound while it is playing, but comes at a cost of
	 * increased memory usage.
	 */
	static const char * kKeepOGGUncompressed ;

	/**
	 * If this is enabled, Playground will software cull any TTexture draws that occur
	 * outside of TRenderer::GetClippingRectangle
	 */
	static const char * kSoftwareTextureCulling ;

	/**
	 * Certain video cards do not handle VRAM paging well. If your game uses more than
	 * 128MB VRAM, you might consider setting this flag, which will force the game
	 * to run in 16-bit mode on certain cards and certain OS's that have exhibited
	 * VRAM paging problems.
	 */
	static const char * kLargeVRAMUsage ;

	/** @} */


#if !USER_DOCS
    /// Valid TRenderer::SetLight indices are 0 .. kMaximumLights - 1.
	static const unsigned int	kMaximumLights = 4;

	/**
	 * @internal
	 * Constructor. For internal use only.
	 *
	 * @param pData  Internal data structure.
	 */
	TPlatform(TPlatformData* pData);

	/**
	 * @internal
	 * Destructor.
	 */
	~TPlatform();

	/**
	 * @nouser
	 * Initialize the sound subsystem.
	 *
	 * @return true if sound was successfully initialized.
	 */
	bool 		InitSound();
	/**
	 * @nouser
	 *
	 * Internal function that notifies us that we've run out of
	 * video memory.
	 */
	void OutOfVideoMemory();

#endif

	/** \name Platform Environment and Information
	 *
	 * @{
	 */

	/**
	 * Get the singleton TPlatform.
	 *
	 * @return The one-and-only TPlatform.
	 *
	 *         During normal game operation, it can be assumed that
	 *         TPlatform always exists.
	 */
	static TPlatform * GetInstance();
#if !USER_DOCS
	/**
	 * Set the global user data value.
	 *
	 * Any client application global variables should ideally be
	 * in one struct or class. Store a pointer to that struct or
	 * class in TPlatform::SetUserData() and you can retrieve it
	 * from anywhere using TPlatform::GetUserData().
	 *
	 * Some platforms actually support only one global variable
	 * in an entire program. This feature will allow your application
	 * to be portable to such a platform.
	 *
	 * @param data   Pointer to user data.
	 */
	void SetUserData( void * data );

	/**
	 * Get the user data pointer. See TPlatform::SetUserData for
	 * more information.
	 *
	 * @return A pointer to the user data.
	 */
	void * GetUserData();
#endif
	/**
	 * Query for a configuration setting. Settings are collected from settings.xml,
	 * and can be overridden by command line options.
	 *
	 * Options are set on the command line using one command line parameter, <b>options</b>.
	 * Options are concatenated using HTTP-GET semantics: Multiple options are appended using
	 * &.
	 *
	 * For example, to set option "first" to 1 and option "second" to 2, you would use the
	 * command line:
	 *
	 * options=first=1&second=2
	 *
	 * This somewhat odd syntax allows us to pass in a stream of arbitrary options from
	 * HTML through the ActiveX wrapper code.
	 *
	 * @param setting Setting to query.
	 * @param defaultSetting
	 *                The value to return if the setting is not already configured.
	 *
	 * @return Value of that setting, or the value of defaultSetting if the
     *         setting is not found.
     *
	 * @see kCheatMode
	 * @see kComputerId
	 * @see kInstallKey
	 * @see kBuildTag
	 * @see kGameName
	 * @see kGameVersion
	 * @see kEncryptionKey
	 * @see kHiscoreLocalOnly
	 * @see kHiscoreAnonymous
	 * @see kPublisherName
	 */
	static str GetConfig( str setting, str defaultSetting="" );

	/**
	 * Query as to whether a setting is enabled. Uses the same settings
	 * as GetConfig().
	 *
	 * @param setting Setting to query.
	 *
	 * @return True if setting is enabled.
	 *
	 * @see GetConfig()
	 */
	static bool IsEnabled( str setting ) { return GetConfig(setting).to_int()!=0; }

	/**
	 * Set a client configuration value.
	 *
	 * Use this to set the encryption key, or to store a value to later be retrieved by
	 * GetConfig().
	 *
	 * @param setting Setting to modify
	 * @param value   New value for setting.
	 *
	 * @see GetConfig()
	 */
	static void SetConfig( str setting, str value );

	/**
	 * Get the sound manager.
	 *
	 * @return The application sound manager.
	 */
	class TSoundManager * GetSoundManager();

	/**
	 * Get the application window manager.
	 *
	 * @return The TWindowManager created by TPlatform.
	 */
	class TWindowManager * GetWindowManager();

	/**
	 * Get the application task list.
	 *
     * @return The TTaskList that holds the system's tasks.
	 */
	TTaskList* GetTaskList();

	/**
	 * Get the string table
	 *
	 * @return The TStringTable created by TPlatform
	 */
	class TStringTable * GetStringTable();

	/** @} */


	/** \name System Commands
	 *
	 * Commands that interact with the operating system.
	 *
	 * @{
	 */

	/**
	 * Set the window application title.
	 *
	 * @param title  Title of application.
	 */
	void		SetWindowTitle(str title);

	/**
	 * Open a URL in a Web browser on the target system.
	 *
	 * @param url    URL to open.
	 */
	void 		OpenBrowser( const char * url );

	/**
	 * Get an event from the application event queue.
	 *
	 * @see TWindowManager::HandleEvent()
	 *
	 * @param pEvent Event to process.
	 */
	void		GetEvent(TEvent* pEvent);

	/**
	 * Send a string to the system clipboard.
	 *
	 * @param copyString String to copy.
	 */
	bool StringToClipboard( str copyString );

	/**
	 * Retrieve a string, if any, from the current clipboard.
	 *
	 * @return A string representation of the current copy buffer.
	 */
	str StringFromClipboard();

	/**
	 * Exit the program.
	 *
	 * This function does return, but the program will exit on its next pass
	 * through the main event loop.
	 *
	 * @param exitValue Exit code.
	 */
	void Exit( int32_t exitValue=0 );

	/** @} */

	/** \name Display Related Functions
	 *
	 * @{
	 */

	/**
	 * Initialize the current display mode.
	 *
	 * @param width  Width of target display
	 * @param height Height of target display
     * @param fullscreen
	 *               True for fullscreen.
	 */
	void		SetDisplay(uint32_t width, uint32_t height, bool fullscreen);

	/**
	 * Get the current display parameters.
	 *
	 * @param pWidth  Width of display.
	 * @param pHeight Height of display.
	 * @param pbFullscreen
	 *                True for fullscreen.
	 */
	void		GetDisplay(uint32_t* pWidth, uint32_t* pHeight, bool* pbFullscreen);

	/**
     * Set a mouse cursor to an image. Use 100% magenta
     * (RGB=255,0,255) for transparent color in a software cursor.
     * For hardware cursors, a normal image with alpha is
     * appropriate.
     *
     * Playground retains both the hardware and software cursor, and
     * uses the one appropriate to the current hardware and
     * windowing state. Because of this, you should always provide
     * both a software and hardware cursor to Playground when you
     * want to be sure the custom cursor will be visible.
	 *
	 * @param texture Texture to use. Set to TTextureRef() (i.e., NULL) to
     *                disable software cursor. A software cursor
     *                texture needs to be a power-of-two in size and
     *                square. A hardware texture must be exactly
     *                32x32 pixels, must <i>not</i> be simple, and
     *                uses normal alpha transparency (magenta will
     *                be magenta!).
     *
     *                You should always supply both a software
     *                cursor and a hardware cursor if you want a
     *                custom cursor to be available.
     *
	 * @param hotSpot Point within texture that the hot spot should be (i.e., the
     *                point where the clicking happens).
     * @param hardware True to set the hardware cursor, false to set
     *                 the software cursor.      */
	void SetCursor(TTextureRef texture, TPoint hotSpot, bool hardware=false);

	/**
	 * Show or hide the cursor.
	 *
	 * @param show True to show the cursor.
	 */
	void ShowCursor(bool show);

   /**
     * Set the cursor position.
     *
     * @deprecated   This API doesn't quite work as expected on the
     *               Mac, and so will be removed from a future
     *               version of Playground. To achieve relative
     *               cursor functionality, please use
     *               SetCursorMode(TPlatform::kCursorModeDelta)
     *               instead.
     *
     * @param at     Position to set mouse cursor, in application
     *               window coordinates.
	 */
	void SetCursorPos( const TPoint & at );

	/**
	 * These mouse button constants are here to allow you to respond
	 * to the use of other mouse buttons. PlayFirst game design
	 * constraints forbid the use of any other than the left mouse
	 * button as a "necessary" part of the user interface. In other
	 * words, any other buttons on the mouse or mouse wheel needs to
	 * be a supplemental interface for convenience of power users.
	 */
	enum ECursorMode
	{
		kCursorModeAbsolute,
		kCursorModeDelta,
	};

   /**
	 * Set the cursor mode.
	 *
     * @param mode     Set the data mode for kMouseMove events.
	 */
	void SetCursorMode( ECursorMode mode );

   /**
	 * Get the cursor mode.
	 */
	ECursorMode GetCursorMode() const;

	/**
	 * Return true if the application is currently the foreground
	 * window.
	 *
	 * @return True if application is in the foreground.
	 */
	bool        IsForeground();

	/**
	 * Set the game window to the foreground. On some systems, this is more of a request than an action, but it will get the user's attention.
	 *
	 */
	void		SetForeground();

	/**
	 * Convenience function for toggling fullscreen.
	 *
	 * @param bFullscreen
	 *               True for full-screen.
	 *
	 * @return True on success.
	 */
	bool		SetFullscreen(bool bFullscreen);

	/**
	 * True if the window is full screen.
	 *
	 * @return True on full screen.
	 */
	bool		IsFullscreen();
#if !USER_DOCS
    /**
     * @nouser
     *
     * Get the internal asset manager. Not for external use;
	 * use the Get() method for the asset type you need.
	 *
     * @return A pointer to the asset manager.
     */
	class TAssetManager * GetAssetManager();

    /**
     * @nouser
     * Create the application window.
     */
    void CreateApplicationWindow();
#endif

	/**
	 * Add a Texture Refresh Listener: On some platforms (DirectX)
	 * there are situations that cause all textures to be
	 * destroyed. If you have a texture that was created by loading
	 * it from a file or resource, it will be automatically rebuilt
	 * by the library. If your texture is created programmatically,
	 * however, you will need to recreate it manually when a
	 * texture-loss event occurs.
	 *
	 * The library internally maintains a list of Texture Refresh
	 * Listeners that all get called when system textures need to
	 * be rebuilt. Add a TTask listener to the list using this
	 * function.
	 *
	 * Note that the TTask::DoTask() return value is respected, so
	 * be sure to return true unless you want your refresh listener
	 * to self-destruct.
	 *
	 * @param rn     A Texture Refresh listener to add to the internal list of
	 *               functions to call when all textures (surfaces) are lost.
	 */
	void AdoptTextureRefreshListener( TTask * rn );

	/**
	 * Remove a Texture Refresh Listener.
	 *
	 * @param rn     The Listener to remove.
	 * @return true if listener was removed, false if listener was not found
	 * @see AddTextureRefreshListener()
	 */
	bool OrphanTextureRefreshListener( TTask * rn );

	/** @} */


	/** \name Timer and user event functions
	 *
	 * Functions that can be used to set up and cancel timers,
	 * query elapsed time, and pause the application.
	 *
	 * @{
	 */

	/**
	 * A count in milliseconds since the program has initialized,
	 *
	 * @return Time value in milliseconds.
	 */
	uint32_t		Timer();

	/**
	 * Sleep the program for a number of milliseconds.
	 *
	 * @param ms     Number of milliseconds to sleep.
	 */
	void		Sleep(uint32_t ms);

	/**
	 * Release a task from the global task list. Releases ownership
	 * of the TTask pointer to the calling function.
	 *
	 * @param task   Task to release.
	 * @return true if task was removed, false if task was not found
	 */
	bool OrphanTask( TTask * task );

	/**
	 * Add a task to the global task list. Transfers ownership to
	 * the global task list, which will expect to destroy the task
	 * when it is complete.
	 *
	 * @param task   Task to adopt.
	 */
	void AdoptTask( TTask * task );

	/** @} */

    /** \name Randomness
	 *

	/**
	 * Return a random integer. Clients are encouraged to use
	 * this as opposed to the stdlib version for maximum
	 * compatibility, and this random number generator
	 * is randomly seeded at application startup.
	 *
     * @return A random integer from 0 to 0xFFFFFFFF.
	 */
	uint32_t Rand( );

    /** @} */

#if !USER_DOCS
    /** *
     * @nouser
	 * Expects an array of four longs, which it fills with a
	 * unique array of four values.
	 *
	 * @param pGuid  Pointer to an array of four longs.
	 */
	static void GenerateGUID( long* pGuid );

	/**
	 * @nouser
	 *
	 * Return true if the display is ready to be updated.
	 *
	 * @return True when ready to draw.
	 */
	bool GetDisplayReady();

	/**
	 * @nouser
	 *
	 * Registers files with TFile. This is used to add any
	 * inertnally used Playground files that always need to be visible,
	 * even after a user calls AddFileMask().
	 */
	static void RegisterFiles();

#endif

#if !USER_DOCS
	/**
	 * @nouser
	 * Internal data for TPlatform.
	 */
	TPlatformData* mData;

	/**
	 * @nouser
	 * Internal data for TPlatform.
	 */
	TTaskList		mRefreshNotify ;
#endif

};

//------------------------------------------------------------------

#if !USER_DOCS

#ifdef _WINDOWS
#define IDI_PLAYGROUND_APP	99
PFLIB_API TPlatformData * CreatePlatformData(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     char *     lpCmdLine,
                     int       nCmdShow,
					 bool		bAllowMultipleInstances=false);
#endif
#ifdef MACINTOSH
PFLIB_API TPlatformData * CreatePlatformData(int argc, char** argv,  bool bAllowMultipleInstances=false);
#endif
#endif

//------------------------------------------------------------------
#endif // PLATFORM_H_INCLUDED
