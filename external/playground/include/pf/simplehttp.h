//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------

#ifndef SIMPLEHTTP_H_INCLUDED
#define SIMPLEHTTP_H_INCLUDED

#if !defined(PFCONFIG_H_INCLUDED)
#include "pfconfig.h"
#endif

//------------------------------------------------------------------
#include "str.h"

//------------------------------------------------------------------
class THttpData;

/**
 * The TSimpleHttp class implements a basic HTTP connection.
 */
class PFLIB_API TSimpleHttp {
public:
	/**
	 * Construction.
	 */
    TSimpleHttp();
	/**
	 * Destruction.
	 */
    virtual ~TSimpleHttp();

	/**
	 * Initialize TSimpleHttp.
	 *
     * @param url   The URL to connect with. The underlying library
     *              will not accept extremely long URLs, so
     *              use POST requests if the request will be more
     *              than about 800 characters long.
     * @param bPost True for a POST request; otherwise GET.
     * @param path	Specify path name to download to; otherwise the
     *             data is kept in an internal buffer and retrieved
     *             using TSimpleHttp::GetContents().
	 */
	void			Init(const char* url, bool bPost = false, const char* path = NULL);

    /**
     * Add a POST or GET argument to the query.
     *
     * @param name   Name of argument.
     * @param value  Value of argument.
     */
    void            AddArg(const char* name, const char* value);
    /**
     * Add a POST or GET argument to the query.
     *
     * @param name   Name of argument.
     * @param value  Value of argument.
     */
	void            AddArg(const char* name, int32_t value);

	/**
	 * HTTP cleanup.
	 *
	 * @param unsafeString
	 *               String to clean.
	 *
	 * @return Newly cleaned string.
	 */
	static str HttpSafeString(const char *unsafeString);

    /// Request modifiers.
	enum ERequestFlags {
        /// No modifier.
		eNoFlag = 0x00000000,
        /// Do not write the request to the cache.
		eDoNotWriteCache = 0x00000001,
        /// Ignore an invalid certificate.
		eIgnoreCNInvalid = 0x00000002
	};

    /**
     * Start the request.
     *
     * @param flags  One or more ERequestFlags bitwise-ORed together.
     */
    void 		    DoRequest(int32_t flags);


	/**
	 * Status enumeration.
	 */
    enum EStatus {
        /// Waiting for a reply.
        eNetWait,
        /// Reply complete and successful.
        eNetDone,
        /// Error communicating with server.
        eNetError,
        /// Error writing file.
        eFileError
    };

    /**
     * Get the current status.
     *
     * @return The status.
     */
    EStatus	        GetStatus();

    /**
     * Returns the "content-length" field of the HTTP header, if
     * available.
     *
     * This value is the expected total download size.
     *
     * @return Expected size of the content.
     *
     * @see GetBytesReceived()
     */
	unsigned long	GetContentLengthHeader();

    /**
     * Get the total number of bytes received. If the download is
     * complete, this is the same as GetContentLengthHeader(); if
     * the download is still in progress, this will be a lower
     * number that you can use to display a progress bar.
     *
     * @return Bytes received.
     *
     * @see GetContentLengthHeader()
     */
	unsigned long	GetBytesReceived();

	/// Get the content of the reply.
    ///
    /// Available only if downloading to memory: Will fail until status is eNetDone.
	char*			GetContents();

private:
	str				mUrl;
	str				mArgs;
	str				mPath;
	str				mPostStr;
	bool            mbFirst;
	bool			mbPost;
	THttpData*		mData;
};

//------------------------------------------------------------------
#endif // SIMPLEHTTP_H_INCLUDED


