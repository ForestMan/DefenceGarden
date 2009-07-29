//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef PREFS_H_INCLUDED
#define PREFS_H_INCLUDED

/**
 * @condfile
 * Interface for TPrefs.
 */

#include "pflibcore.h"
#include "simplexml.h"
#include "str.h"

#include <map>
#include <vector>

#ifndef ENCRYPT_H_INCLUDED
#include <pf/encrypt.h>
#endif

/**
 * The TPrefs class is designed to help with the saving of preferences for a game.
 * Preferences can be stored at either a global level or a user level.
 *
 * See note about unique installs in the TPrefs() constructor comments.
 */

class PFLIB_API TPrefs
{
public:
	/**
     * Constructor.
     *
     * Prior to calling the constructor, you need to set the
     * application encryption key. See TPlatform::SetConfig().
     *
     * @warning Be sure to save the game periodically during play.
     * Any time the user has, e.g., gained a new level, or any other
     * significant event the game should remember, you should tell
     * it to write a save file. The reason is that, when your game
     * is wrapped in DRM, when the DRM expires it will often
     * directly send a kQuit message, which terminates the (default)
     * game loop.
     *
     * @par
     * If there are performance reasons you don't want to write the
     * file frequently, you can use the option in SetInt() and
     * SetStr() to NOT write the file immediately; as long as the
     * setting has been given to TPrefs, it will be written on game
     * exit.
     *
     * @par Selecting a Specific Configuration
     *
     * In order to prevent games from being downloaded multiple
     * times and being able to use the same saved games, a TPrefs
     * object attempts to distinguish between unique installs of the
     * game. It does this by looking at the directory the game is
     * running from.
     *
     * Therefore, for debugging purposes, if you want your game to
     * always be treated as the same install, you can hand create an
     * install.txt file in the assets folder. This file can do two
     * things:
     *
     *          1) If the file is empty, TPrefs will load/save
     *             preferences as if it was running from the most
     *             recently created preferences location (or create
     *             a new location if one doesn't exist).
     *
     *          2) If the file is not empty, it will treat the
     *             contents of this file as the install path, and
     *             read/save games as if it was that install.
     *             (example: putting C:/game/mygame in the
     *             install.txt file will make the TPrefs object
     *             pretend the game is running from c:/game/mygame
     *
     * @param saveData  Whether or not data should be saved between
     *                  sessions, default is true. For example, in a
     *                  web game you might not want scores to
     *                  persist between sessions, in which case
     *                  saveData should be false.
	 */
	TPrefs(bool saveData = true);

	/**
	 * Destructor
	 */
	~TPrefs();

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
     *               set to true, or until SavePrefs() is called.
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
     *               set to true, or until SavePrefs() is called.
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
     *                   preference is set with save set to true, or until SavePrefs() is called.
     */
	void SetBinary(str prefName, const void *data, uint32_t dataLength, int32_t userIndex = kGlobalIndex, bool save = true);

	/**
     * Commit preferences to permanent storage.
	 */
	void SavePrefs();

	/**
	 * Gets all the user data as a str.
	 *
     * @param userIndex Which user to get data from.
	 *
     * @return A str containing all the user's data.
	 */
	str GetUserStr(int32_t userIndex);

    /**
     * Sets a user data from a passed in str. In order to maintain proper functionality
     * this should only be called with a str return from GetUserStr.
     *
     * @param userIndex Which user to set data.
     * @param data      String to set.
     */
	void SetUserStr(int32_t userIndex, str data);

#ifndef USER_DOCS
	/**
	 * @nouser
	 *
	 */
	void PlayedTimes(std::map<str, uint32_t> &playedTimes);
#endif

private:
	void LoadXMLPrefs();
	void LoadPrefs();

	typedef int8_t DataType ;
	typedef std::vector<DataType> DataVec ;
	typedef std::map<str,DataVec> StrVecMap;
	typedef std::pair<str,DataVec> StrVecPair ;
	typedef std::map<str,str> StrStrMap;
	typedef std::vector<StrVecMap> PrefsArray ;
	typedef std::map<str,PrefsArray> AllPrefsMap ;

	static inline str ToStr( const DataVec & data )
	{
		return str((char*)&data[0],data.size());
	}

	static inline DataVec ToVec( const str & s )
	{
		return DataVec(s.c_str(),s.c_str()+s.length()+1);
	}

	bool ReadChunk( struct TFile & file, DataVec & chunk, char * header );
	void GetChunk( const StrVecMap & svm, DataVec & data );
	void ParseUserData( uint32_t user, DataType * data, uint32_t size );

	void Append( DataVec & data, const void * src, uint32_t len );
	template<typename T> void Append( DataVec & data, T src );
	void AppendEncrypted( DataVec & data, DataVec & src );
	static uint32_t GetEncryptedLength( uint32_t len )
	{
		return len+(8 - (len % 8));
	}

//	TXmlNode *FindUserNode(int32_t index);
//	TXmlNode *GetGlobalNode();

//	TXmlNode			mPrefs;

	TEncrypt					mEncrypt ;

	AllPrefsMap					mAllPrefs ;
	PrefsArray					*mCurrentPrefs ;

	str					mEncryptionKey;
	str					mPrefsFileName;
	str					mInstallKey;

	bool				mAllowSaves;

	uint32_t			mLifeTime;
};

#endif // PREFS_H_INCLUDED
