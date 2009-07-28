//------------------------------------------------------------------
// Copyright (c) 2005 PlayFirst, Inc.
//
// This material is the confidential trade secret and proprietary 
// information of PlayFirst, Inc. It may not be reproduced, used, 
// sold or transferred to any third party without the prior written    
// consent of PlayFirst, Inc. All rights reserved.
//------------------------------------------------------------------


// The following code is a port of the Bouncy Castle java blowfish
// implementation that is used on the server side.

/**
 * The Bouncy Castle License
 *
 * Copyright (c) 2000 The Legion Of The Bouncy Castle (http://www.bouncycastle.org)
 * <p>
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * <p>
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 * <p>
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */


#ifndef BLOWFISH_H_INCLUDED
#define BLOWFISH_H_INCLUDED
class TBlowfish {
public:
	TBlowfish();
	~TBlowfish();

	void Init(const unsigned char *key, int keyLen);
	char *EncryptString(const char *str, int inLength, int *outLength);
	char *DecryptString(const char *input, int inputLen, int *outLength);

private:
	void Encrypt(unsigned int *xl, unsigned int *xr);
	void Decrypt(unsigned int *xl, unsigned int *xr);
	unsigned int F(unsigned int x);
	void processTable(int xl, int xr, int *table, int size);
	
	int *S0, *S1, *S2, *S3;     // the s-boxes
    int *P;                  // the p-array

};



#endif //BLOWFISH_H_INCLUDED


