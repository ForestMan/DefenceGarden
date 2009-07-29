//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef PREFSDB_H_INCLUDED
#define PREFSDB_H_INCLUDED

/**
 * Interface for TPrefsDB.
 */

#include "pflibcore.h"
#include "str.h"

#ifndef ENCRYPT_H_INCLUDED
#include <pf/encrypt.h>
#endif

class TPrefsDBData;

/**
 * The TPrefsDB class is designed to help with the saving of preferences for a game.
 * Preferences can be stored at either a global level or a user
 * level.
 *
 * See note about unique installs in the TPrefsDB() constructor
 * comments.
 *
 * TPrefsDB differs from TPrefs in the method of saving
 * the data; it uses a database format that can save part of the
 * file instead of rewriting the entire file each time, so if
 * your game has a large data save requirement, it will likely
 * be much faster to save if you switch from TPrefs to TPrefsDB.
 */

class PFLIB_API TPrefsDB
{
public:
	/**
     * Constructor.
     *
     * Prior to calling the constructor, you need to set the
     * application encryption key. See TPlatform::SetConfig().
     *
     * @param saveData  Whether or not data should be saved between
     *                  sessions, default is true. For example, in a
     *                  web game you might not want scores to
     *                  persist between sessions, in which case
     *                  saveData should be false.
	 */
	TPrefsDB(bool saveData = true);

	/**
	 * Destructor
	 */
	~TPrefsDB();

	/**
     * Get the number of users. Users must be numbered sequentially,
     * and there may not be more than 1023 users.
	 *
	 * @returns The number of currently created users in the preferences.
	 */
	uint32_t GetNumUsers();

	/**
     * Deletes the current user at the specified slot, and slides
     * all users above that slot down. If user 2 is deleted, then
     * user 3 becomes user 2, 4 becomes 3, etc.
	 *
     * @param userNum Which user to delete
	 */
	void DeleteUser(uint32_t userNum);

	/**
     * An index to pass in as a userIndex parameter to any get/set
     * function to get/set a global preference.
	 */
	static const int32_t kGlobalIndex = -1;

	/**
     * Gets an int from the preferences.
	 *
     * @param prefName Name of preference to get.
     * @param defaultValue If this preference has not been set, what
     *                     value should be returned.
     * @param userIndex Which user to get from, or if this is
     *                  kGlobalIndex then get from the global
     *                  preferences. Users must be numbered
     *                  sequentially, and there may not be more than
     *                  1023 users.
	 *
	 * @return Returns the stored int if it exists, or else defaultValue if it does not exist.
	 */
	int32_t GetInt(str prefName, int32_t defaultValue, int32_t userIndex = kGlobalIndex);

	/**
     * Sets an int in the preferences.
	 *
     * @param prefName Name of preference to set.
     * @param value Value to set preference to.
     * @param userIndex Which user to set, or if this is
     *                  kGlobalIndex then set a global preference.
     *                  Users must be numbered sequentially, and
     *                  there may not be more than 1023 users.
     * @param save If this is true, commits preferences to disk. if
     *               false, preferences are changed in memory only,
     *               until a different preference is set with save
     *               set to true, or until SavePrefsdb() is called.
	 */
	void SetInt(str prefName, int32_t value, int32_t userIndex = kGlobalIndex, bool save = true);

	/**
     * Gets a str from the preferences.
	 *
	 * @param prefName Name of preference to Get.
	 * @param defaultValue If this preference has not been set, what value should be returned.
     * @param userIndex Which user to get from, or if this is
     *                  kGlobalIndex then get from the global
     *                  preferences. Users must be numbered
     *                  sequentially, and there may not be more than
     *                  1023 users.
	 *
	 * @return Returns the stored str if it exists, or else defaultValue if it does not exist.
	 */
	str GetStr(str prefName, str defaultValue, int32_t userIndex = kGlobalIndex);

	/**
     * Sets a str in the preferences.
     *
     * String length is limited to 4Mb-16 bytes.
	 *
     * @param prefName Name of preference to set.
     * @param value Value to set preference to.
     * @param userIndex Which user to set, or if this is
     *                  kGlobalIndex then set a global preference.
     *                  Users must be numbered sequentially, and
     *                  there may not be more than 1023 users.
     * @param save If this is true, commits preferences to disk. if
     *               false, preferences are changed in memory only,
     *               until a different preference is set with save
     *               set to true, or until SavePrefsdb() is called.
	 */
	void SetStr(str prefName, str value, int32_t userIndex = kGlobalIndex, bool save = true);

	/**
     * Gets a binary block from the preferences.
	 *
     * @param prefName Name of preference to get.
     * @param buffer Address of location to store data in.
     * @param bufferLen Size of buffer in bytes.
     * @param userIndex Which user to get from, or if this is
     *                  kGlobalIndex then get from the global
     *                  preferences. Users must be numbered
     *                  sequentially, and there may not be more than
     *                  1023 users.
	 *
	 * @return If the preference does not exist, -1 is returned.
	 *		   If buffer is successfully filled in with the preference, 0 is returned.
	 *	       If the preference exists and buffer is NULL or bufferLen is too small to store the data, then the size
	 *		   that the buffer needs to be is returned.
	 */
	int32_t GetBinary(str prefName, void *buffer, uint32_t bufferLen, int32_t userIndex = kGlobalIndex);

    /**
     * Sets a block of binary data in the preferences.
     *
     * Binary data size is limited to 4Mb-16 bytes.
     *
     * @param prefName   Name of preference to set.
     * @param data       Address of data to set in preferences.
     * @param dataLength Size of data in bytes.
     * @param userIndex Which user to set, or if this is
     *                  kGlobalIndex then set a global preference.
     *                  Users must be numbered sequentially, and
     *                  there may not be more than 1023 users.
     * @param save       If this is true, commits preferences to disk. If false, preferences are changed in memory only, until a different
     *                   preference is set with save set to true, or until SavePrefsdb() is called.
     */
	void SetBinary(str prefName, const void *data, uint32_t dataLength, int32_t userIndex = kGlobalIndex, bool save = true);

	/**
     * Commit preferences to permanent storage.
	 */
	void SavePrefs() ;

private:
	typedef int8_t DataType ;

	static uint32_t GetEncryptedLength( uint32_t len )
	{
		return len+(8 - (len % 8));
	}

	TEncrypt					mEncrypt ;

	str					mEncryptionKey;
	str					mPrefsdbFileName;

	bool				mAllowSaves;

	uint32_t			mLifeTime;

	TPrefsDBData		* mData ;

};

#endif // PREFSDB_H_INCLUDED
