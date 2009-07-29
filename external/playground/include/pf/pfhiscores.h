//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef PFHISCORES_H_INCLUDED
#define PFHISCORES_H_INCLUDED

#ifndef PFCONFIG_H_INCLUDED
#include "pf/pfconfig.h"
#endif

#ifndef PFTYPES_H_INCLUDED
#include "pftypes.h"
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

// Note - if you are using pfhiscores, but not using pflib, you need to compile
// this file with PFLIB_HISCORE_STANDALONE defined. Additionally, if you
// are using the .lib version of pfhiscores (as opposed to a .dll), you need
// to compile with PFLIB_STATIC_LINK defined.

#ifndef PFLIB_HISCORE_STANDALONE
	#ifndef PFLIBCORE_H_INCLUDED
		#include "pflibcore.h"
	#endif

	#ifdef PFHISCORE_EXPORTS
		#define PFHISCORE_API __declspec(dllexport)
	#else
		#define PFHISCORE_API PFLIB_API
	#endif
#else
	#ifdef PFLIB_STATIC_LINK
		#define PFHISCORE_API
	#else
		#ifdef PFHISCORE_EXPORTS
			#define PFHISCORE_API __declspec(dllexport)
		#else
			#define PFHISCORE_API __declspec(dllimport)
		#endif
	#endif
#endif // PFLIB_HISCORE_STANDALONE

//------------------------------------------------------------------
class TPfHiscoresData;

/**
 * TPfHiScores - class that manages local and global hiscore saving and viewing.
 *
 * This is the backend class for managing hiscores. It has the ability to save and load local hiscores
 * as well as submit and retrieve hiscores from the server.
 *
 * For information about how to test your hiscore implementation against a debug server,
 * please see the @ref hiscoresystem information.
 *
 *
 *  Localization - the hiscore system can be localized by including a file called "hiscore.xml" in
 *  the same data directory as the .dll or the .exe if you are using a static lib. The contents of
 *  this xml file should be:
 *
 * @xmlcode
 * 	<hiscore>
 * 		<language>en</language>
 * 		<defaulterror>Unable to connect to server. Please try again later.</defaulterror>
 * 	</hiscore>
 * @endxmlcode
 *
 * 	The language parameter will override any setting used with eLanguage property in SetProperty().
 * 	Language should be ISO-639 (e.g. "en", "jp", "fr", "en_CA").
 * 	The default error string is what is displayed in the case of not being able to connect to the server.
 *
 */
class PFHISCORE_API TPfHiscores
{
public:

    /**
     * Default constructor
     *
     * This initalizes the hiscores system. Among other things, it
     * loads in a preference file off disk which contains all the
     * stored local scores.
     *
     * The game name and encryption key must be set in the global
     * configuration prior to constructing. See
     * TPlatform::SetConfig() for details. For standalone builds,
     * see the standalone hiscore documentation.
     *
     * @param saveData Whether or not data should be saved between
     *                 sessions, default is true. For example, in
     *                 a web game you might not want scores to
     *                 persist between sessions, in which case
     *                 saveData should be false.
     */
	TPfHiscores(bool saveData = true);

    /**
     * Default destructor
     */
	~TPfHiscores();

    /**
     * After sending a request to the hiscore server, this
     * is the status of the request.
     */
	enum EStatus { eSuccess = 0, ePending, eError };


    /**
     * The various properties that can
     * be set for the hiscore module.
     */
	enum EProperty { eLanguage = 0, ePlayerName, eGameMode };

    /**
     * Sets a given property
     *
     * @param property Which property to set.
     *                 eLanguage - which language to set (ISO-639 (e.g. "en",
     *                 "jp", "fr", "en_CA")). The default language
     *                 is "en". Note that this property will have
     *                 no effect if you are using the "hiscore.xml"
     *                 localization file described above.
     *                 ePlayerName - This is the local player name. This will be
     *                 used for logging and returning local scores.
     *                 eGameMode - Once the game mode is set, it will be used
     *                 for all score submission and retreivals.
     * @param value    Value to set the property to (for example, if
     *                 property is eLanguage, value might be "en").
     */
	void SetProperty(EProperty property, str value);

    /**
     * Log a score for the current player.
     *
     * This stores a score for the current player into the local
     * score table. A score must be logged before it can be
     * submitted. The score is logged for the player name set with
     * SetPlayerName()
     *
     * @param score      the score the user is submitting
     * @param replaceExisting
     *                   should this score submission
     *                   replace an existing user score, or create
     *                   a new one? Typical usage is that
     *                   story/career mode games replace an
     *                   existing score, whereas arcade modes allow
     *                   a new score with each submission.
     * @param gameData   game specific scoring information (up to
     *                   60 chars supported by server) - it is
     *                   important that this be data that is
     *                   additional scoring information (i.e. last
     *                   level reached) and not scoring
     *                   interpretation information (i.e. awarded
     *                   the "Best Ever" trophy) so that the server
     *                   can adjust awards later on. Also, this
     *                   game data should not include any text that
     *                   would need to be localized. Therefore, it
     *                   should just be numbers if at all possible.
     */
	void KeepScore(int32_t score, bool replaceExisting, const char *gameData);


	 /**
     * The various medal types for use in KeepMedal()
     */
	enum EMedalType
	{
		eMedalType_Game=0, ///< Medals that are awarded for the entire game, regardless of what mode the user is in
		eMedalType_Mode    ///< Medals that are awarded per game mode (the user can earn this medal once in each game mode)
	};


	/**
     * Log a medal for the current player.
	 *
	 * This stores a medal for the current player. A medal must be logged
	 * before it can be submitted. The medal is logged for the
	 * player name set with SetPlayerName()
	 *
	 * If a player has already earned this particular medal,
	 * then this call does nothing. It is possible to tell if this
	 * call changes the number of medals the player has
	 * by calling GetNumMedalsToSubmit() before and after
	 * this call.
	 *
	 * @param medalName     The name of the medal the user
	 *						earned. The same names used to fill
	 *						in the TPlatform::kPFGameMedalName
	 *						config settings should be used here.
	 * @param type			The type of medal earned
	 * @param gameData		This string can be used to keep game specific information about how this medal
	 *						was earned. This data will not be transmitted to the server, and is just kept
	 *						for retrieval with GetEarnedMedalInfo(). An example use might be a time stamp
	 *						so that when GetEarnedMedalInfo() is called to display a UI, a date can be
	 *						displayed along with the medal.
     */
	void KeepMedal(const char *medalName, EMedalType type, str gameData);


    /**
     *  Retreives a username and password if one has been saved.
     *
     *  If the user submits their score with the bRemember flag set to true, then
     *  the username and password are saved, so that next time the user does not have
     *  to type them again.
     *
     * @param userName str to hold user name
     * @param password str to hold password
     *                 @return - if the user/pass have been saved, this returns true and fills in the user/pass. if
     *                 it has not been saved, it returns false and does nothing to username and password.
     *
     * @return If the user/pass have been saved, this returns tru and fills in
     *         the user/pass. If it has not been saved, it returns false and
     *         does nothing to the username and password.
     */
	bool GetRememberedUserInfo( str * userName, str * password );

    /**
     * Saves a user name and password.
     *
     * Saves a user name and password without logging any score information.
     *
     * @param userName If NULL or "" will delete any previously
     *                 saved information.
     * @param password If NULL or "" will delete any previously
     *                 saved information.
     */
	void SetRememberedUserInfo( str userName, str password);

    /**
     * Submit a request to the server to retreive category
     * information.
     *
     * After calling this function, GetServerRequestStatus() must be polled until a result is ready.
     */
	void	RequestCategoryInformation();

    /**
     * Return the number of categories available for hiscores.
     *
     * You must have retrieved the category information with RequestCategoryInformation() first.
     *
     * @return Number of categories available.
     */
	int32_t		GetCategoryCount();

    /**
     * Fill in a table name.
     *
     * You must have retrieved the category information with RequestCategoryInformation() first.
     *
     * @param n         Which category to fetch.
     * @param[out] name Fills in category's name
     * @param bufSize   Size of name buffer.
     *
     * @return Returns false if category information has not been properly initialized.
     */
	bool	GetCategoryName(int32_t n, char* name, uint32_t bufSize);

    /**
     * Submit a request to the server to retreive hiscores.
     *
     * After calling this function, GetServerRequestStatus() must be polled until a result is ready.
     *
     * @param categoryIndex
     *               Which category is requested (use GetCategoryCount() to see how many categories are available)
     */
	void	RequestScores(int32_t categoryIndex);

    /**
     * Return the status of the last server request.
     *
     * After calling any functiont that contacts the server, this function must be polled until a result
     * is ready.
     *
     * @param msg        If EStatus is eError, an error message will be placed inside msg
     * @param bufLen     Buffer length of msg.
     * @param pQualified For SubmitData(), if EStatus is eSuccess, this fills in whether or not the score
     *                   qulaified for a global record.
     *
     * @return The current status of the request. If it is eError, the request failed and the message
     *         inside msg should be displayed to the user.
     */
	EStatus	GetServerRequestStatus(char *msg, uint32_t bufLen, bool* pQualified);

    /**
     * Retrieve the number of scores currently downloaded from the
     * server.
     *
     * To retrieve a server score, you must have retrieved the score information with RequestScores() first.
     *
     * @param local True to retrieve for local scores.
     * @return Number of scores.
     */
	int32_t		GetScoreCount(bool local);

    /**
     * Fill in all the various score information for a given score.
     *
     * To retrieve a server score, you must have retrieved the score information with RequestScores() first.
     *
     * @param local      True for local high scores, false for
     *                   global.
     * @param n          Index into the score table.
     * @param[out] pRank      Fills in score's rank in current table.
     * @param[out] name        Fills in player's name.
     * @param bufSize    Size of name buffer.
     * @param[out] pAnonymous
     *                   Fills in whether score is anonymous or not.
     * @param[out] pScore      Fills in score
     * @param[out] gameData    Fills in game specifc data
     * @param gameDataBufferSize
     *                   Size of gameData buffer that can be filled
     *                   in.
     *
     * @return Returns false if scores are not properly intialized.
     */
	bool	GetScore(bool local, int32_t n, int32_t* pRank, char* name, uint32_t bufSize, bool* pAnonymous, int32_t* pScore, char *gameData, uint32_t gameDataBufferSize);

    /**
     * The various modes for calling GetUserBestScore()
     */
	enum EUserScore {eLocalEligible=0, eGlobalBest, eLocalBest};

    /**
     * Fill in all the various score information for a given user.
     *
     * Depending on the userScore type passed in, different score information will be filled in.
     *
     * @param userScore Type of score to fetch:
     *                  eLocalEligible - the user's best local score eligible for submission (one that has not already been submitted).
     *                  eGlobalBest - the user's best score on the current global score table (obtained from RequestScores()).
     * @param[out] pScore    Fills in the user's score.
     * @param[out] pRank     Fills in score's rank in current table.
     * @param[out] gameData  Fills in game specifc data.
     * @param gameDataBufferSize
     *                  Size of gameData buffer that can be filled in.
     *
     * @return Returns false if the user has no eligible score, or if the game cannot find a user score
     *         in the current table.
     */
    bool GetUserBestScore(EUserScore userScore, int32_t* pScore, int32_t* pRank, char* gameData, uint32_t gameDataBufferSize);

	/**
     * Returns the number of medals that a user has earned but has not submitted to the server with SubmitData().
	 *
	 * This call does not return the total number of medals a user has earned, only the number of medals that
	 * have not yet been submitted.
	 *
	 * This result of this call is not dependent on the current game mode - it returns a total number of
	 * medals across all game modes.
     */
	int GetNumMedalsToSubmit();

	/**
     * Returns the number of medals that a user has earned, regardless of whether or not they have been submitted.
	 *
	 * This only reflects the number of medals that the user has earned according to locally saved data.
	 * It does not fetch anything from the server.
	 *
	 * This result of this call is not dependent on the current game mode - it returns a total number of
	 * medals across all game modes.
	 *
	 * The result from this call can be used to index into GetEarnedMedalInfo().
     */
	int GetNumMedalsEarned();

	/**
     * Returns stored medal info for the current user. Calling GetNumMedalsEarned() will let you know
	 * how many medals can be retrieved.
	 *
	 * @param index Index of medal to be retrieved.
     * @param[out] medalName    Fills in the name of the medal (if medalName is not NULL).
     * @param[out] type     Fills in the type of the medal (if type is not NULL)
     * @param[out] gameMode  Fills in the gameMode that the medal was earned in (if gameMode is not NULL).
	 * @param[out] gameData  Fills in the gameData associated with the medal (if gameData is not NULL).
	 *
	 * @return Returns false if the passed in index is out of range. Otherwise returns true
     */
	bool GetEarnedMedalInfo(int index, str *medalName, EMedalType *type, str *gameMode, str *gameData);


	/** Submit modes
	 *
	 * Controls what type of data is submitted
	 */
	enum ESubmitMode
	{
		kSubmitScore = 0x00000001,	///< Submit the user's best score to the server
		kSubmitMedal = 0x00000002,	///< Submit the user's medals to the server
		kSubmitAll = 0xFFFFFFFF,	///< Submit all available data to the server. It is recommended
									///  that this mode be used whenever the UI permits - that way
									///  all of the user's data is submitted with one call.
	};
    /**
     * Submit the current user's best score and medals to the server.
     *
     * After calling this, you should poll GetServerRequestStatus()
     * to check for errors or successful submission.
	 *
	 * Only one call to SubmitData() can be made at a time. You can not issue a second call until
	 * GetServerRequestStatus() has returned a result. Issuing a second call before GetServerRequestStatus()
	 * returns a result may result in undefined behavior.
	 *
	 * Medals will only be submitted if a password is used. If a password is not used, then
	 * medals will be saved until the next time a password-enabled submission is made.
	 *
	 * All of a user's available medals are submitted - the types of medals are not filtered
	 * by the current game mode.
     *
     * @param username  Name to submit the score under.
     * @param password  The user's playfirst password. If this is NULL or "", then an anonymous submission is issued.
     * @param bRemember If this is true, then the module saves the
     *                  username and password for future use. If it
     *                  is false, it deletes any previously saved
     *                  username and password.
	 * @param submitMode What data to submit. When possible kSubmitAll should be used so that all user data
	 *					 is submitted. However, in certain UI presentations, it may only be appropriate to submit
	 *					 medals but not scores, or vice versa.
     *
     * @return If score submission is not possible, this returns false (i.e. the player has already submitted
     *         their best score, or if they do not have any scores or medals
     *         for this game mode, etc.).
     */
	bool SubmitData(str username, str password, bool bRemember, ESubmitMode submitMode);




    /**
     * Clear the local scores for the current game
     * mode.
     *
     * This will delete the current local scores for the current game mode. It cannot be undone.
     */
	void	ClearScores();

	/**
     * Clear the local scores and medal for a specified player name
	 *
	 * This will remove any scores associated with the player from the local score table, and
	 * will also remove their local medals. This operation cannot be undone.
     *
     * @param playername - name to erase. 
     */
	void	ClearPlayerData(str playername);



    /**
     * Encrypt a byte stream.
     *
     * This will encrypt the passed in byte stream, and returns a string encoded in
     * base64 (so it can be passed around like a string).
     *
     * @param toEncrypt The bytestream that is to be encrypted.
     * @param len       The length of data to encrypt - note that if you are encrypting a text string you
     *                  will want to encrypt the null terminator too, so you should pass in strlen(toEncrypt) + 1.
     * @param[out] buf       The buffer to fill in with the
     *                  encrypted string.
     * @param bufLen    The size of the buffer. If this is too small, the function will return the size
     *                  needed to encrypt the string, and will not fill in buf at all.
     *
     * @return 0 on success, or else returns the length of the buffer needed to encrypt this string.
     */
	uint32_t		EncryptData(const void *toEncrypt, uint32_t len, char *buf, uint32_t bufLen);

    /**
     * Decrypt a string.
     *
     * This will decrypt a null terminated Base64 string into a byte stream.
     *
     * @param toDecrypt A null terminated Base64 string to decrypt.
     * @param[out] buf       The buffer to fill in with the decrypted
     *                  string.
     * @param bufLen    The size of the buffer. If this is too small, the function will return the size
     *                  needed to decrypt the string, and will not fill in buf at all.
     *
     * @return 0 on success, or else returns the length of the buffer needed to encrypt this string.
     */
	uint32_t		DecryptData(const char *toDecrypt, void *buf, uint32_t bufLen);


	/**
	 * @deprecated This function has been replaced by KeepScore(). Please use that instead.
	 *
     * Log a score for the current player.
     *
     * This stores a score fore the current player into the local
     * score table. A score must be logged before it can be
     * submitted. The score is logged for the player name set with
     * SetPlayerName()
     *
     * @param score      the score the user is submitting
     * @param replaceExisting
     *                   should this score submission
     *                   replace an existing user score, or create
     *                   a new one? Typical usage is that
     *                   story/career mode games replace an
     *                   existing score, whereas arcade modes allow
     *                   a new score with each submission.
     * @param gameData   game specific scoring information (up to
     *                   60 chars supported by server) - it is
     *                   important that this be data that is
     *                   additional scoring information (i.e. last
     *                   level reached) and not scoring
     *                   interpretation information (i.e. awarded
     *                   the "Best Ever" trophy) so that the server
     *                   can adjust awards later on. Also, this
     *                   game data should not include any text that
     *                   would need to be localized. Therefore, it
     *                   should just be numbers if at all possible.
     * @param serverData an optional XML formatted string that
     *                   will enable certain hiscore-related
     *                   features on the hiscore server. The XML
     *                   must be well-formed.
     *
     *                   Currently available features are:
     *
     *                   Medals: Medals are awards given to users
     *                   that complete certain tasks in the game. A
     *                   medal has two parameters. The "name"
     *                   parameter is the identification name of the
     *                   medal. The "per" parameter can either be
     *                   "type" or "game" - "type" means that this
     *                   medal is specific to the current game mode,
     *                   whereas "game" means that this medal is a
     *                   global medal awarded across all game modes.
     *
     *                   Examples: To submit 2 medals for this
     *                   score:
	 * @xmlcode
     *                  <medal name="medal1" per="game"/>
     *                  <medal name="medal2" per="type"/>
     * @endxmlcode
     */
	void	LogScore(int32_t score, bool replaceExisting, const char *gameData, const char *serverData = NULL );

    /**
	 * @deprecated This function has been replaced by SubmitData(). Please use that instead.
	 *
     * Submit a new score to the server.
     *
     * After calling this, you should poll GetServerRequestStatus()
     * to check for errors or successful submission.
     *
     * @param username  Name to submit the score under.
     * @param password  The user's playfirst password. If this is NULL or "", then an anonymous submission is issued.
     * @param bRemember If this is true, then the module saves the
     *                  username and password for future use. If it
     *                  is false, it deletes any previously saved
     *                  username and password.
     *
     * @return If score submission is not possible, this returns false (i.e. the player has already submitted
     *         their best score, or if they do not have any scores
     *         for this game mode, etc.).
     */
	bool	SubmitScore(str username, str password, bool bRemember);

	/**
	 * @deprecated This function has been replaced by SubmitData(). Please use that instead.
	 *
     * Submit medal information for the current player to the
     * server.
     *
     * After calling this, you should poll GetServerRequestStatus() to check for errors or successful submission
     *
     * This stores the medals information for the current player into the local score table.
     * The medals information must be logged before it can be submitted.
     * The medals are logged for the player name set with SetPlayerName()
     *
     * @param medalsData A well-formed XML string that specifies medal information to be stored on the server.
     * @param username   Name to submit the score under.
     * @param password   The user's playfirst password.
     * @param bRemember  If this is true, then the module saves the username and password for future use. if it
     *                   is false, it deletes any previously saved username and password.
     *
     * @return If the medals submission is not possible, this returns false.
     *
     *         Medals are awards given to users that complete certain tasks in the game.
     *         A medal has two parameters. The "name" parameter is the identification name of the
     *         medal. The "per" parameter can either be "type" or "game" - "type" means that this
     *         medal is specific to the current game mode, whereas "game" means that this medal
     *         is a global medal awarded across all game modes.
     *
     *         Examples:
     *         @xmlcode
     *         <medal name="dinerdash7_frag50" per="type" type="dinerdash7_counterstrike"/>
     *         <medal name="dinerdash7_frag50" per="type" type="dinerdash7_fragfest"/>
     *         <medal name="dinerdash7_frag100" per="type" type="dinerdash7_fragfest"/>
     *         <medal name="dinerdash7_killedRonaldMcDonald" per="game"/>
     *         <medal name="dinerdash7_killedBurgerKing" per="game"/>
     *         @endxmlcode
     */
	bool	SubmitMedals(const char *medalsData,str username, str password, bool bRemember);


private:

	bool	ParseScoreData(const char *buffer);
	bool	ParseCategoryData(const char *buffer);
	bool	ParseReturnForErrors(const char *buffer);
	void	ParseRememberedUserPreferences(const char *inBuffer);
	void	ParseSavedLocalScores(const char *inBuffer);
	void	ParseSavedMedals(const char *inBuffer);
	void	SavePreferences();
	void	LoadPreferences();
	void	LoadSettings();
	void	DeleteCurrentHiscores();
	void	DeleteCurrentHiscoreCategories();
	bool	GetUserScore(bool bEligible, int32_t* pScore, int32_t* pRank, char* gameData, uint32_t gameDataBufferSize);
	bool	GetUserGlobalScore(int32_t* pScore, int32_t* pRank, char* gameData, uint32_t gameDataBufferSize);
	void	RecordScore(int32_t score, bool replaceExisting, const char *gameData, const char *serverData = NULL );
	bool	SubmitDataToServer(const char* username, const char* password, bool bRemember, bool checkMedals);
	bool	SubmitMedalsToServer(const char *medalsData,const char* username, const char* password, bool bRemember);


	TPfHiscoresData* mData;
};

//------------------------------------------------------------------
#endif // PFHISCORES_H_INCLUDED

