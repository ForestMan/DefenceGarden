//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#ifndef ASSET_H_INCLUDED
#include "asset.h"
#endif

#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

//------------------------------------------------------------------
#ifndef TAGGING_IGNORE
class TSoundData;
class TSoundCallBack;
class TSoundStream;
class TSoundCallBackData;
#endif

typedef shared_ptr<TSoundData> TSoundDataRef ;
typedef boost::weak_ptr<TSoundData> TSoundDataWeakRef ;

/**
 * TSoundCallBack --a class that you override and attach to a TSound
 * if you want to know when the sound has finished playing.
 *
 */
class PFLIB_API TSoundCallBack
{
public:
	/// Constructor
	TSoundCallBack();

	/// Destructor
	virtual ~TSoundCallBack();

	/**
     * Function called when a sound has finished playing. This
     * function is guaranteed to be called during the main thread,
     * but may be called up to one second later due to buffering.
	 *
	 * @param soundInstance	TSoundInstanceRef of sound that just finished
	 * @param nextSound TSoundRef of sound that has been queued up to play next with
	 *			   TSound::SetCompleteAction
	 *
	 */
	virtual void OnComplete(TSoundInstanceRef soundInstance, TSoundRef nextSound) = 0;

	friend class TSound;
	friend class TSoundData;

private:
	TSoundCallBackData *mData;

};

/**
 * The TSound class represents an object that can play a sound asset.
 *
 * By default, TSound only knows how to load Ogg/Vorbis files,
 * but Playground supports the loading of Ogg/Speex files as
 * well. The Speex compression format is optimized for voice
 * compression. For the fullest Playground support of Ogg/Speex
 * files, you must use the encoder that ships with Playground,
 * bin/speexenc.exe.
 *
 * To activate Speex decoding, you need to add the following
 * code to main.cpp:
 *
 * @code
 * #include <pf/speex.h>
 *
 * ...
 *
 * // In PlaygroundInit()
 * TSpeex::Register();
 * @endcode
 *
 * This will cause TSound::Get() to handle .spx files correctly.
 * Speex files <em>only</em> support streamed playback, and do
 * not support seeking to an arbitrary offset. You can, however,
 * pause, resume, and reset a Speex sound to the beginning, and
 * you can loop a Speex sound.
 */
class PFLIB_API TSound : public TAsset
{
	PFSHAREDTYPEDEF(TAsset);
private:
	/// Construction is through the factory method
	TSound();
public:
	virtual ~TSound();

	enum
	{
		kButtonSound = 0,
		kUserSoundBase = 1000
	};

	/**
	 * @name Factory Methods
	 * @{
	 */

	/**
	 * Get a sound from a name, associating it with a
	 * particular sound group.
	 *
	 * You can also specify the
	 * sound group by appending "?group=###" to the filename
	 * instead of passing the group number as the third
	 * parameter. This allows you to specify a group number
	 * when referencing a sound in Lua (in the TButton sound tags,
	 * for example), or when add a group of files using
	 * TAssetMap::AddAssets().
	 *
	 * @param filename  Name of the sound file.
	 * @param bInMemory True to load entire sound into memory, false to stream it
	 * @param type      Type of sound, or -1 to be part of a global group. Sounds with the same type
	 *                  will all play at the same volume. User defined groups should start
	 *                  at kUserSoundBase. Anything below that value is reserved
	 *                  for internal library use.
	 *
	 * @return A TSoundRef to the sound.
	 */
	static TSoundRef Get(str filename, bool bInMemory=true, int32_t type=-1);

	/** @} */

	/**
	 * Play a sound
	 *
	 * @param bLoop	Whether or not to loop a sound
	 *
     * @return A reference to the spawned instance of the sound. To
     *         modify or update that instance once it's spawned,
     *         keep a reference to the TSoundInstance and use it to
     *         modify the instance.
	 */
     TSoundInstanceRef Play(bool bLoop = false);

	/**
	 * Pause all instances of this sound.
	 *
	 * @param bPause	True to pause, false to unpause

	 * @return true on success, false on failure
	 */
	bool Pause(bool bPause);

	/**
	 * Kill all instances of this sound - once a sound is killed it must be restarted with Play(), it cannot be unpaused.
	 *
	 * If a sound is set as a "continuation" of a sound that is killed, it will also be killed, even if it hasn't started yet.
	 *
	 * @return true if any sounds were killed, false otherwise.
	 */
	bool Kill();

    /**
     * Get the length of the sound in seconds.
     *
     * @return Number of seconds.
     */
    TReal GetSoundLength();

	/**
	 * Get a reference to this sound.
	 *
	 * @return A reference to this.
	 */
	TSoundRef	GetRef()
	{
		return static_pointer_cast<TSound>( shared_from_this() );
	}

	/**
	 * Get the file handle used to create the sound.
	 *
	 * @return The sound file handle.
	 */
	str GetName();

private:
#if !TAGGING_IGNORE
	friend class TSoundInstance;
#endif
	void SetCompleteAction(TSoundInstanceRef sir, TSoundCallBack* pCallback, TSoundRef playNext = TSoundRef());
	bool KillInstance(TSoundInstanceRef sir);

	/**
	 * Internal function that gets an asset from the asset
	 * manager, if one exists.
	 *
	 * @param handle Handle to the asset.
	 *
	 * @return A TSoundRef of the asset; can be NULL.
	 */
	static TSoundRef InternalGet( str handle );

	/**
	 * Internal function to create a TSound.
	 *
	 * @param handle (optional) Handle of asset to add to asset manager.
	 *               Empty to create a unique TSound.
	 *
	 * @return A TSound to the new sound.
	 */
	static TSoundRef InternalNew( str handle="" );

	TSoundDataRef mData;

};

//------------------------------------------------------------------

#endif // TEXTURE_H_INCLUDED
