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
 * Interface for TEncrypt.
 */

#ifndef ENCRYPT_H_INCLUDED
#define ENCRYPT_H_INCLUDED

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#ifndef PFTYPES_H_INCLUDED
#include "pftypes.h"
#endif

/**
 * The TEncrypt class is used to encrypt/decrypt data.
 */
class TEncryptData;

/**
 * A class that encapsulates an encryption engine.
 */
class PFLIB_API TEncrypt
{
public:

	/**
	 * Constructor
	 *
	 * @param encryptionKey Key to use for encrypting/decrypting. The key should consist of valid Base64 characters(0-9,a-z,A-Z,+,/) and be
	 *						 correctly formatted (i.e. user proper '=' suffix).
	 */
	TEncrypt(str encryptionKey);

	/**
	 * Destructor
	 */
	~TEncrypt();

	/**
	 * @param input Str to encrypt.
	 *
	 * @return returns the input encrypted as a Str.
	 */
	str EncryptStr(str input);

	/**
	 * @param input Str to decrypt. This str must have been created with EncryptStr to ensure proper decryption.
	 *
	 * @return returns the decrypted str.
	 */
	str DecryptStr(str input);

    /**
	 * @param input Data to encrypt
	 * @param inLength Length of input in bytes
	 * @param output Buffer to place encrypted data into.
	 * @param outLength Size of output buffer.
	 * @param base64 true to fill the buffer with base64 characters, so it can be passed around as a string.
	 *				 false will fill the buffer with binary data.
	 *
	 * @return returns 0 if successful. Otherwise, if output is NULL or outLength is too small for the resulting data,
	 *		   then returns the size in bytes that the output buffer must be to hold the encrypted data.
	 */
    uint32_t EncryptBinary(const void *input, uint32_t inLength, void *output, uint32_t outLength, bool base64);

	 /**
	 * @param input Data to decrypt. This data must have been encrypted with EncryptBinary to ensure proper decryption.
	 * @param inLength Length of input in bytes
	 * @param output Buffer to place decrypted data into.
	 * @param outLength Size of output buffer.
	 * @param base64 true if the incoming buffer is in base64, false if the incoming buffer is in binary
	 *
	 * @return returns 0 if successful. Otherwise, if output is NULL or outLength is too small for the resulting data,
	 *		   then returns the size in bytes that the output buffer must be to hold the decrypted data.
	 */
    uint32_t DecryptBinary(const void *input, uint32_t inLength, void *output, uint32_t outLength, bool base64);

	/**
	 * Get the last decrypted or encrypted data size.
	 *
	 * @return Size of last encrypted or decrypted data.
	 */
	uint32_t GetLastSize();
private:
	TEncrypt();
	TEncryptData *mData;


};

#endif
