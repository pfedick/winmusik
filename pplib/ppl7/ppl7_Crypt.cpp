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

#include "prolog_ppl7.h"
#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_OPENSSL
#include <openssl/evp.h>
#endif

#include "ppl7.h"
#include "ppl7-crypto.h"

namespace ppl7 {

#ifdef HAVE_OPENSSL
static const EVP_CIPHER *getCipher(Crypt::Algorithm algo, Crypt::Mode mode)
{
    switch (algo) {
#ifdef HAVE_EVP_AES_128_ECB
    case Crypt::Algo_AES_128:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_aes_128_ecb();
        case Crypt::Mode_CBC:
            return EVP_aes_128_cbc();
        case Crypt::Mode_CFB:
            return EVP_aes_128_cfb();
        case Crypt::Mode_OFB:
            return EVP_aes_128_ofb();
        }
#endif
#ifdef HAVE_EVP_AES_192_ECB
    case Crypt::Algo_AES_192:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_aes_192_ecb();
        case Crypt::Mode_CBC:
            return EVP_aes_192_cbc();
        case Crypt::Mode_CFB:
            return EVP_aes_192_cfb();
        case Crypt::Mode_OFB:
            return EVP_aes_192_ofb();
        }
#endif
#ifdef HAVE_EVP_AES_256_ECB
    case Crypt::Algo_AES_256:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_aes_256_ecb();
        case Crypt::Mode_CBC:
            return EVP_aes_256_cbc();
        case Crypt::Mode_CFB:
            return EVP_aes_256_cfb();
        case Crypt::Mode_OFB:
            return EVP_aes_256_ofb();
        }
#endif
#ifndef OPENSSL_NO_ARIA
#ifdef HAVE_EVP_ARIA_128_ECB
    case Crypt::Algo_ARIA_128:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_aria_128_ecb();
        case Crypt::Mode_CBC:
            return EVP_aria_128_cbc();
        case Crypt::Mode_CFB:
            return EVP_aria_128_cfb();
        case Crypt::Mode_OFB:
            return EVP_aria_128_ofb();
        }
#endif
#ifdef HAVE_EVP_ARIA_192_ECB
    case Crypt::Algo_ARIA_192:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_aria_192_ecb();
        case Crypt::Mode_CBC:
            return EVP_aria_192_cbc();
        case Crypt::Mode_CFB:
            return EVP_aria_192_cfb();
        case Crypt::Mode_OFB:
            return EVP_aria_192_ofb();
        }
#endif
#ifdef HAVE_EVP_ARIA_256_ECB
    case Crypt::Algo_ARIA_256:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_aria_256_ecb();
        case Crypt::Mode_CBC:
            return EVP_aria_256_cbc();
        case Crypt::Mode_CFB:
            return EVP_aria_256_cfb();
        case Crypt::Mode_OFB:
            return EVP_aria_256_ofb();
        }
#endif
#endif
#ifndef OPENSSL_NO_BF
#ifdef HAVE_EVP_BF_ECB
    case Crypt::Algo_BLOWFISH:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_bf_ecb();
        case Crypt::Mode_CBC:
            return EVP_bf_cbc();
        case Crypt::Mode_CFB:
            return EVP_bf_cfb();
        case Crypt::Mode_OFB:
            return EVP_bf_ofb();
        }
#endif
#endif
#ifndef OPENSSL_NO_CAMELLIA
#ifdef HAVE_EVP_CAMELLIA_128_ECB
    case Crypt::Algo_CAMELLIA_128:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_camellia_128_ecb();
        case Crypt::Mode_CBC:
            return EVP_camellia_128_cbc();
        case Crypt::Mode_CFB:
            return EVP_camellia_128_cfb();
        case Crypt::Mode_OFB:
            return EVP_camellia_128_ofb();
        }
#endif
#ifdef HAVE_EVP_CAMELLIA_192_ECB
    case Crypt::Algo_CAMELLIA_192:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_camellia_192_ecb();
        case Crypt::Mode_CBC:
            return EVP_camellia_192_cbc();
        case Crypt::Mode_CFB:
            return EVP_camellia_192_cfb();
        case Crypt::Mode_OFB:
            return EVP_camellia_192_ofb();
        }
#endif
#ifdef HAVE_EVP_CAMELLIA_256_ECB
    case Crypt::Algo_CAMELLIA_256:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_camellia_256_ecb();
        case Crypt::Mode_CBC:
            return EVP_camellia_256_cbc();
        case Crypt::Mode_CFB:
            return EVP_camellia_256_cfb();
        case Crypt::Mode_OFB:
            return EVP_camellia_256_ofb();
        }
#endif
#endif
#ifndef OPENSSL_NO_CAST
#ifdef HAVE_EVP_CAST5_ECB
    case Crypt::Algo_CAST5:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_cast5_ecb();
        case Crypt::Mode_CBC:
            return EVP_cast5_cbc();
        case Crypt::Mode_CFB:
            return EVP_cast5_cfb();
        case Crypt::Mode_OFB:
            return EVP_cast5_ofb();
        }
#endif
#endif
#ifndef OPENSSL_NO_DES
#ifdef HAVE_EVP_DES_ECB
    case Crypt::Algo_DES:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_des_ecb();
        case Crypt::Mode_CBC:
            return EVP_des_cbc();
        case Crypt::Mode_CFB:
            return EVP_des_cfb();
        case Crypt::Mode_OFB:
            return EVP_des_ofb();
        }
#endif
#ifdef HAVE_EVP_DES_EDE3_ECB
    case Crypt::Algo_TRIPLE_DES:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_des_ede3_ecb();
        case Crypt::Mode_CBC:
            return EVP_des_ede3_cbc();
        case Crypt::Mode_CFB:
            return EVP_des_ede3_cfb();
        case Crypt::Mode_OFB:
            return EVP_des_ede3_ofb();
        }
#endif
#endif
#ifndef OPENSSL_NO_IDEA
#ifdef HAVE_EVP_IDEA_ECB
    case Crypt::Algo_IDEA:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_idea_ecb();
        case Crypt::Mode_CBC:
            return EVP_idea_cbc();
        case Crypt::Mode_CFB:
            return EVP_idea_cfb();
        case Crypt::Mode_OFB:
            return EVP_idea_ofb();
        }
#endif
#endif
#ifndef OPENSSL_NO_RC2
#ifdef HAVE_EVP_RC2_ECB
    case Crypt::Algo_RC2:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_rc2_ecb();
        case Crypt::Mode_CBC:
            return EVP_rc2_cbc();
        case Crypt::Mode_CFB:
            return EVP_rc2_cfb();
        case Crypt::Mode_OFB:
            return EVP_rc2_ofb();
        }
#endif
#endif
#ifndef OPENSSL_NO_RC5
#ifdef HAVE_EVP_RC5_32_12_16_ECB
    case Crypt::Algo_RC5:
        switch(mode) {
        case Crypt::Mode_ECB:
            return EVP_rc5_32_12_16_ecb();
        case Crypt::Mode_CBC:
            return EVP_rc5_32_12_16_cbc();
        case Crypt::Mode_CFB:
            return EVP_rc5_32_12_16_cfb();
        case Crypt::Mode_OFB:
            return EVP_rc5_32_12_16_ofb();
        }
#endif
#endif
    }
    return nullptr;
}
#endif


Crypt::Crypt()
{
    ctx=nullptr;
}

Crypt::~Crypt()
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (ctx) EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
#endif
}

int Crypt::keyLength() const
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    return EVP_CIPHER_CTX_key_length(static_cast<EVP_CIPHER_CTX*>(ctx));
#endif
}

int Crypt::maxKeyLength() const
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    return EVP_MAX_KEY_LENGTH;
#endif
}

int Crypt::ivLength() const
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    return EVP_CIPHER_CTX_iv_length(static_cast<EVP_CIPHER_CTX*>(ctx));
#endif
}

int Crypt::blockSize() const
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    return EVP_CIPHER_CTX_block_size(static_cast<EVP_CIPHER_CTX*>(ctx));
#endif
}

void Crypt::setPadding(bool enabled)
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    if (enabled) EVP_CIPHER_CTX_set_padding(static_cast<EVP_CIPHER_CTX*>(ctx),1);
    else EVP_CIPHER_CTX_set_padding(static_cast<EVP_CIPHER_CTX*>(ctx),0);
#endif
}

void Crypt::setKeyLength(int keylen)
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    int ret=EVP_CIPHER_CTX_set_key_length(static_cast<EVP_CIPHER_CTX*>(ctx),keylen);
    if (ret!=1) throw InvalidKeyLengthException("%d",keylen);
#endif
}


/***********************************************************************************************************
 * Enrypt
 ***********************************************************************************************************/

Encrypt::Encrypt(Algorithm algo, Mode mode)
{
    ctx=nullptr;
    setAlgorithm(algo, mode);
}


void Encrypt::setAlgorithm(Algorithm algo, Mode mode)
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (ctx) EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
    ctx=EVP_CIPHER_CTX_new();
    if (!ctx) throw ppl7::InitializationFailedException("OpenSSL: EVP_CIPHER_CTX_new");
    const EVP_CIPHER *cipher=getCipher(algo, mode);
    if (!cipher) {
        EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
        ctx=nullptr;
        throw UnsupportedAlgorithmException();
    }
    int ret=EVP_EncryptInit_ex(static_cast<EVP_CIPHER_CTX*>(ctx),
                               cipher,nullptr,nullptr,nullptr);
    if (ret!=1) {
        EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
        ctx=nullptr;
        throw ppl7::InitializationFailedException("OpenSSL: EVP_EncryptInit_ex");
    }
#endif
}

void Encrypt::setKey(const ByteArrayPtr &key)
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    int ret=EVP_EncryptInit_ex(static_cast<EVP_CIPHER_CTX*>(ctx),
                               nullptr,nullptr,
                               static_cast<const unsigned char*>(key.ptr()),
                               nullptr);
    if (ret!=1) throw ppl7::InitializationFailedException("OpenSSL: EVP_EncryptInit_ex, setKey");
#endif
}

void Encrypt::setIV(const ByteArrayPtr &iv)
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    int ret=EVP_EncryptInit_ex(static_cast<EVP_CIPHER_CTX*>(ctx),
                               nullptr,nullptr,nullptr,
                               static_cast<const unsigned char*>(iv.ptr()));
    if (ret!=1) throw ppl7::InitializationFailedException("OpenSSL: EVP_EncryptInit_ex, setIV");
#endif
}

void Encrypt::update(const ByteArrayPtr &in, ByteArray &out)
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    unsigned char *outbuf=static_cast<unsigned char *>(malloc(in.size()+
                                            static_cast<size_t>(EVP_CIPHER_CTX_block_size(static_cast<EVP_CIPHER_CTX*>(ctx)))));
    if (!outbuf) throw ppl7::OutOfMemoryException();
    int inlen=static_cast<int>(in.size());
    int outlen=0;
    if (!EVP_EncryptUpdate(static_cast<EVP_CIPHER_CTX*>(ctx), outbuf, &outlen,
                           static_cast<const unsigned char*>(in.ptr()), inlen)) {
        EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
        ctx=nullptr;
        throw EncryptionFailedException();
    }
    out.useadr(outbuf,static_cast<size_t>(outlen));
#endif
}

void Encrypt::final(ByteArray &out)
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    unsigned char *outbuf=static_cast<unsigned char *>(malloc(
                                            static_cast<size_t>(EVP_CIPHER_CTX_block_size(static_cast<EVP_CIPHER_CTX*>(ctx)))));
    if (!outbuf) throw ppl7::OutOfMemoryException();
    int outlen=0;
    if (!EVP_EncryptFinal_ex(static_cast<EVP_CIPHER_CTX*>(ctx), outbuf, &outlen)) {
        EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
        ctx=nullptr;
        throw EncryptionFailedException();
    }
    out.useadr(outbuf,static_cast<size_t>(outlen));
#endif
}

void Encrypt::encrypt(const ByteArrayPtr &in, ByteArray &out)
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    unsigned char *outbuf=static_cast<unsigned char *>(malloc(in.size()+
                                            2*static_cast<size_t>(EVP_CIPHER_CTX_block_size(static_cast<EVP_CIPHER_CTX*>(ctx)))));
    if (!outbuf) throw ppl7::OutOfMemoryException();
    int inlen=static_cast<int>(in.size());
    int outlen=0;
    if (!EVP_EncryptUpdate(static_cast<EVP_CIPHER_CTX*>(ctx), outbuf, &outlen,
                           static_cast<const unsigned char*>(in.ptr()), inlen)) {
        EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
        ctx=nullptr;
        throw EncryptionFailedException();
    }
    int outlen_final=0;

    if (!EVP_EncryptFinal_ex(static_cast<EVP_CIPHER_CTX*>(ctx), outbuf+outlen, &outlen_final)) {
        EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
        ctx=nullptr;
        throw EncryptionFailedException();
    }
    out.useadr(outbuf,static_cast<size_t>(outlen+outlen_final));
#endif
}

/***********************************************************************************************************
 * Decrypt
 ***********************************************************************************************************/

Decrypt::Decrypt(Algorithm algo, Mode mode)
{
    ctx=nullptr;
    setAlgorithm(algo, mode);
}


void Decrypt::setAlgorithm(Algorithm algo, Mode mode)
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (ctx) EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
    ctx=EVP_CIPHER_CTX_new();
    if (!ctx) throw ppl7::InitializationFailedException("OpenSSL: EVP_CIPHER_CTX_new");
    const EVP_CIPHER *cipher=getCipher(algo, mode);
    if (!cipher) {
        EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
        ctx=nullptr;
        throw UnsupportedAlgorithmException();
    }
    int ret=EVP_DecryptInit_ex(static_cast<EVP_CIPHER_CTX*>(ctx),
                               cipher,nullptr,nullptr,nullptr);
    if (ret!=1) {
        EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
        ctx=nullptr;
        throw ppl7::InitializationFailedException("OpenSSL: EVP_DecryptInit_ex");
    }
#endif
}

void Decrypt::setKey(const ByteArrayPtr &key)
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    int ret=EVP_DecryptInit_ex(static_cast<EVP_CIPHER_CTX*>(ctx),
                               nullptr,nullptr,
                               static_cast<const unsigned char*>(key.ptr()),
                               nullptr);
    if (ret!=1) throw ppl7::InitializationFailedException("OpenSSL: EVP_DecryptInit_ex, setKey");
#endif
}

void Decrypt::setIV(const ByteArrayPtr &iv)
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    int ret=EVP_DecryptInit_ex(static_cast<EVP_CIPHER_CTX*>(ctx),
                               nullptr,nullptr,nullptr,
                               static_cast<const unsigned char*>(iv.ptr()));
    if (ret!=1) throw ppl7::InitializationFailedException("OpenSSL: EVP_DecryptInit_ex, setIV");
#endif
}

void Decrypt::update(const ByteArrayPtr &in, ByteArray &out)
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    unsigned char *outbuf=static_cast<unsigned char *>(malloc(in.size()+
                                            static_cast<size_t>(EVP_CIPHER_CTX_block_size(static_cast<EVP_CIPHER_CTX*>(ctx)))));
    if (!outbuf) throw ppl7::OutOfMemoryException();
    int inlen=static_cast<int>(in.size());
    int outlen=0;
    if (!EVP_DecryptUpdate(static_cast<EVP_CIPHER_CTX*>(ctx), outbuf, &outlen,
                           static_cast<const unsigned char*>(in.ptr()), inlen)) {
        EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
        ctx=nullptr;
        throw DecryptionFailedException();
    }
    out.useadr(outbuf,static_cast<size_t>(outlen));
#endif
}

void Decrypt::final(ByteArray &out)
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    unsigned char *outbuf=static_cast<unsigned char *>(malloc(
                                            static_cast<size_t>(EVP_CIPHER_CTX_block_size(static_cast<EVP_CIPHER_CTX*>(ctx)))));
    if (!outbuf) throw ppl7::OutOfMemoryException();
    int outlen=0;
    if (!EVP_DecryptFinal_ex(static_cast<EVP_CIPHER_CTX*>(ctx), outbuf, &outlen)) {
        EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
        ctx=nullptr;
        throw DecryptionFailedException();
    }
    out.useadr(outbuf,static_cast<size_t>(outlen));
#endif
}

void Decrypt::decrypt(const ByteArrayPtr &in, ByteArray &out)
{
#ifndef HAVE_OPENSSL
    throw ppl7::UnsupportedFeatureException("OpenSSL");
#else
    if (!ctx) throw ppl7::NotInitializedException();
    unsigned char *outbuf=static_cast<unsigned char *>(malloc(in.size()+
                                            2*static_cast<size_t>(EVP_CIPHER_CTX_block_size(static_cast<EVP_CIPHER_CTX*>(ctx)))));
    if (!outbuf) throw ppl7::OutOfMemoryException();
    int inlen=static_cast<int>(in.size());
    int outlen=0;
    if (!EVP_DecryptUpdate(static_cast<EVP_CIPHER_CTX*>(ctx), outbuf, &outlen,
                           static_cast<const unsigned char*>(in.ptr()), inlen)) {
        EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
        ctx=nullptr;
        throw DecryptionFailedException();
    }
    int outlen_final=0;

    if (!EVP_DecryptFinal_ex(static_cast<EVP_CIPHER_CTX*>(ctx), outbuf+outlen, &outlen_final)) {
        EVP_CIPHER_CTX_free(static_cast<EVP_CIPHER_CTX*>(ctx));
        ctx=nullptr;
        throw DecryptionFailedException();
    }
    out.useadr(outbuf,static_cast<size_t>(outlen+outlen_final));
#endif
}


}	// EOF namespace ppl7

