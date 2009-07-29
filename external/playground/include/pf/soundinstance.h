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
 * Interface for class TSoundInstance
 */

#ifndef SOUNDINSTANCE_H_INCLUDED
#define SOUNDINSTANCE_H_INCLUDED

#ifndef FORWARD_H_INCLUDED
#include "forward.h"
#endif

#ifndef TAGGING_IGNORE
class TStreamNotify ;
class TSoundCallBack ;
class TSoundStream ;
#endif


/**
 * An instance of a sound. Returned as a TSoundInstanceRef
 * from TSound::Play(), you can then control the playback
 * of that instance using this class.
 *
 * @see TSound
 */
class PFLIB_API TSoundInstance : public enable_shared_from_this<TSoundInstance>
{
private:
    /// Private constructor; these are only built by TSound!
	TSoundInstance(TSoundStream* ss) ;

#ifndef TAGGING_IGNORE
    friend class TSound;
    friend class TSoundData;
    friend class TSoundStream;
#endif
public:
    /// Destructor
	virtual ~TSoundInstance();

    /**
     * Play the sound!
     */
    void Play();

    /**
     * Pause the sound!
     *
     * @param bPause True to pause the sound; false to resume.
     */
    void Pause(bool bPause);

    /**
     * Kill the sound instance.  Sound instances normally live until
     * they complete or until they are killed. Simply releasing the
     * TSoundInstanceRef will not itself kill a sound.
     */
    void Kill();

   	/**
	 * Set a sound to a specific play position
	 *
     * @param seconds The position, in seconds, to set the play
     *                position of the sound.
	 */
	void SetPosition(TReal seconds);

    /**
     * Set the volume of the sound.
     *
     * @param volume Volume level 0.0-1.0f
     */
    void SetVolume( float volume );

    /**
     * Get the group this stream belongs to.
     *
     * @return The current group id.
     */
    int GetGroupId();

    /**
     * Setup a sound callback - the sound will call the passed in callback class when the
     *        sound has finished playing.
     *        The client maintains ownership of the callback, and it is their responsibility
     *	      to delete it when no longer needed. Deleting the callback automatically
     *        unregisters it.
     *        A TSoundCallBack contains references to the current sound and the playNext
     *        sound. Therefore, you do not need to keep a reference around to a sound
     *        used in a sound callback, and the sound can be killed by deleting the sound
     *        callback (However - in practice it is wise to keep around your own sound
     *        reference so you can pause it, kill it, etc.)
     *        This call can also be used to setup a sound to play immediately
     *        after this sound is done. Note that because a callback is called during the main
     *        thread, in order to play a sound seamlessly, it is better to
     *        setup a 2nd sound with playNext instead of having the pCallback play
     *        another sound.
     *
     * @warning When queueing up sounds with the next parameter, the sound must be of the
     *			same number of channels (i.e. mono or stereo).
     *
     *			If you queue up a sound with a different number of channels, an ASSERT will
     *			trigger in a debug build. In a release build, the resulting sound will likely
     *			be incorrect.
     *
     * @param pCallback Pointer to callback object that will be called when the sound is done.
     *              This parameter can be NULL if the client just wants to setup a playNext sound.
     * @param playNext - what sound to play after the current sound finishes. Default is a
     *              NULL TSoundRef(). (See note above about playNext restrictions)
     *
     */
	void SetCompleteAction(TSoundCallBack* pCallback, TSoundRef playNext = TSoundRef());

	/**
	 * Get a reference to the original sound that spawned this instance.
	 *
	 * @return A sound reference, or NULL if the sound has been deleted.
	 */
	TSoundRef GetSound() { return mSound.lock(); }

	/**
	 * Get a reference to the next sound to be streamed after the current one completes.
	 */
	 TSoundRef GetNextSound() { return mNextSound.lock() ; }
	 
private:
    TSoundStream    *mStream;

    boost::weak_ptr<TSound>	    mSound;
	boost::weak_ptr<TSound>		mNextSound;

	TSoundCallBack  *mCallback;

};

#endif // SOUNDINSTANCE_H_INCLUDED
