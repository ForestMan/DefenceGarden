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
 * Interface for class TSoundManager.
 *
 * Includes interface for plugging a dynamic reader into the sound
 * playback.
 */


#ifndef SOUNDMANAGER_H_INCLUDED
#define SOUNDMANAGER_H_INCLUDED

#include "str.h"
#include "clock.h"

#define SOUND_DEBUG 0
class TSoundStream;
typedef shared_ptr<TSoundStream> TSoundStreamRef;

/**
 * The TSoundManager class controls access to the sound subsystem.
 */
class PFLIB_API TSoundManager
{
public:
	/**
	 * Set the global sound volume.
	 *
	 * @param volume Global sound volume. 0.0 is off, 1.0 is full volume.
	 *
	 */
	void SetVolume(float volume);


	/**
	 * Set the volume for a specific group of sounds. This is multiplied
	 * with the global volume setting.
	 *
     * @param type id of sound group to set.
	 * @param volume Volume to set. 0.0 is off, 1.0 is full volume.
	 */
	void SetTypeVolume(int32_t type, float volume);



	/**
	 * Pause or unpause all sounds.
	 *
	 * @param bPause true to pause, false to resume.
     * @param type Pass -1 to pause all sounds, or specify a
     *              specific sound group ID
     *
	 */
	void PauseAllSounds(bool bPause, int32_t type = -1);

	/**
	 * Kill all sounds.
	 *
     * @param type Pass -1 to kill all sounds, or specify a specific
     *          sound group ID.
	 */
	void KillAllSounds(int32_t type = -1);

#if !USER_DOCS
	TSoundManager();
	virtual ~TSoundManager();

	bool Init();
	bool Initialized() { return mInitialized; }
	void AppActivate(bool activated);
	void HandleNotifications();
	float GetVolume(int32_t type);
	void InternalHandleNotifications(bool delayDelete);
	bool ShuttingDown() { return mShuttingDown; }
	bool SystemPaused() { return !mSoundActive; }
#endif
	friend class TSoundStream ;
private:

	enum
	{
		kConcurrentStreams = 32		    ///< Max number of concurrent streams?
	};

	enum
	{
        kUpdateDelay= 500
	};

	/**
	 * Streams can register themselves through this interface.
	 * (Streams are friends).
	 *
	 * @param stream Stream to register.
	 */
	void RegisterStream( TSoundStream* stream);
	void Update(bool delayDelete);
	void SetStreamVolume(TSoundStreamRef stream);
	void UpdateAllVolumes();
	void StopStream(TSoundStreamRef stream);
	void StopStreamDelayed(TSoundStreamRef stream);
	void KillAllSoundsDelayed();


	friend class TLockSoundMutex ;
	class TSoundManagerData 	*mData ;

	TClock	mUpdateClock ;
	bool	mInitialized ;
	bool    mShuttingDown ;

	bool	mSoundActive ;
};

#endif // SOUNDMANAGER_H_INCLUDED
