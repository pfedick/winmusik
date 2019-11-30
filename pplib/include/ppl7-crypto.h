/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 7 (PPL7).
 * Web: http://www.pfp.de/ppl/
 *
 * $Author$
 * $Revision$
 * $Date$
 * $Id$
 *
 *******************************************************************************
 * Copyright (c) 2013, Patrick Fedick <patrick@pfp.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright notice, this
 *       list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#ifndef PPL7CRYPTO_H_
#define PPL7CRYPTO_H_

#ifndef _PPL7_INCLUDE
    #ifdef PPL7LIB
        #include "ppl7.h"
    #else
        #include <ppl7.h>
    #endif
#endif

namespace ppl7 {

PPL7EXCEPTION (UnsupportedAlgorithmException, Exception);
PPL7EXCEPTION (InvalidAlgorithmException, Exception);
PPL7EXCEPTION (NoAlgorithmSpecifiedException, Exception);
PPL7EXCEPTION (InvalidBlocksizeException, Exception);
PPL7EXCEPTION (HashFailedException, OperationFailedException);
PPL7EXCEPTION (NoKeySpecifiedException, Exception);
PPL7EXCEPTION (NoIVSpecifiedException, Exception);
PPL7EXCEPTION (EncryptionFailedException, OperationFailedException);
PPL7EXCEPTION (DecryptionFailedException, OperationFailedException);
PPL7EXCEPTION (InvalidKeyLengthException, Exception);

class Crypt
{
    friend class Encrypt;
    friend class Decrypt;
private:
    void *ctx;

public:
    enum Mode {
        Mode_ECB,
        Mode_CBC,
        Mode_CFB,
        Mode_OFB
    };

    enum Algorithm {
        Algo_AES_128,
        Algo_AES_192,
        Algo_AES_256,
        Algo_ARIA_128,
        Algo_ARIA_192,
        Algo_ARIA_256,
        Algo_BLOWFISH,
        Algo_CAMELLIA_128,
        Algo_CAMELLIA_192,
        Algo_CAMELLIA_256,
        Algo_CAST5,
        Algo_DES,
        Algo_TRIPLE_DES,
        Algo_IDEA,
        Algo_RC2,
        Algo_RC5,
    };
    Crypt();
    ~Crypt();
    int keyLength() const;
    int maxKeyLength() const;
    int ivLength() const;
    int blockSize() const;
    void setPadding(bool enabled);
    void setKeyLength(int keylen);
};

class Encrypt : public Crypt
{
public:
    Encrypt(Algorithm algo, Mode mode);
    void setAlgorithm(Algorithm algo, Mode mode);
    void setKey(const ByteArrayPtr &key);
    void setIV(const ByteArrayPtr &iv);
    void update(const ByteArrayPtr &in, ByteArray &out);
    void final(ByteArray &out);
    void encrypt(const ByteArrayPtr &in, ByteArray &out);
};

class Decrypt : public Crypt
{
public:
    Decrypt(Algorithm algo, Mode mode);
    void setAlgorithm(Algorithm algo, Mode mode);
    void setKey(const ByteArrayPtr &key);
    void setIV(const ByteArrayPtr &iv);
    void update(const ByteArrayPtr &in, ByteArray &out);
    void final(ByteArray &out);
    void decrypt(const ByteArrayPtr &in, ByteArray &out);
};

class Digest
{
	private:
		const void *m;
		void *ctx;
		unsigned char *ret;
		uint64_t bytecount;

	public:
		enum Algorithm {
			Algo_MD4,
			Algo_MD5,
			Algo_SHA1,
			Algo_SHA224,
			Algo_SHA256,
			Algo_SHA384,
			Algo_SHA512,
			Algo_WHIRLPOOL,
			Algo_RIPEMD160
		};


		Digest();
		Digest(const String &name);
		Digest(Algorithm algorithm);
		~Digest();

		void setAlgorithm(Algorithm algorithm);
		void setAlgorithm(const String &name);
		void addData(const void *data, size_t size);
		void addData(const ByteArrayPtr &data);
		void addData(const String &data);
		void addData(const WideString &data);
		void addData(FileObject &file);
		void addFile(const String &filename);
		ByteArray getDigest();
		void saveDigest(ByteArray &result);
		void saveDigest(String &result);
		void saveDigest(WideString &result);

		void reset();
		uint64_t bytesHashed() const;

		static ByteArray hash(const ByteArrayPtr &data, Algorithm algorithm);
		static ByteArray hash(const ByteArrayPtr &data, const String &algorithmName);
		static ByteArray md4(const ByteArrayPtr &data);
		static ByteArray md5(const ByteArrayPtr &data);
		static ByteArray sha1(const ByteArrayPtr &data);
		static ByteArray sha224(const ByteArrayPtr &data);
		static ByteArray sha256(const ByteArrayPtr &data);
		static ByteArray sha384(const ByteArrayPtr &data);
		static ByteArray sha512(const ByteArrayPtr &data);
		static uint32_t crc32(const ByteArrayPtr &data);
		static uint32_t adler32(const ByteArrayPtr &data);

};


} // EOF namespace ppl7

#endif /* PPL7CRYPTO_H_ */
