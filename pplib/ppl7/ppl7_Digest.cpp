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

#ifdef HAVE_LIBZ
#include <zlib.h>
#endif

#include "ppl7.h"
#include "ppl7-crypto.h"

namespace ppl7 {

bool __OpenSSLDigestAdded = false;

Mutex __OpenSSLGlobalMutex;

void InitOpenSSLDigest()
{
#ifndef HAVE_OPENSSL
	throw UnsupportedFeatureException("OpenSSL");
#else
	__OpenSSLGlobalMutex.lock();
	if (!__OpenSSLDigestAdded) {
		::OpenSSL_add_all_digests();
		__OpenSSLDigestAdded=true;
	}
	__OpenSSLGlobalMutex.unlock();
#endif
}


Digest::Digest()
{
	bytecount=0;
	m=NULL;
	ret=NULL;
	ctx=NULL;
#ifndef HAVE_OPENSSL
	throw UnsupportedFeatureException("OpenSSL");
#else
	if (!__OpenSSLDigestAdded) {
		InitOpenSSLDigest();
	}
#endif
}

Digest::~Digest()
{
#ifdef HAVE_OPENSSL
	free(ret);
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
	if (ctx) EVP_MD_CTX_free((EVP_MD_CTX*)ctx);
#else
	if (ctx) EVP_MD_CTX_destroy((EVP_MD_CTX*)ctx);
#endif

#endif
}

Digest::Digest(const String &name)
{
	bytecount=0;
	m=NULL;
	ret=NULL;
	ctx=NULL;
#ifndef HAVE_OPENSSL
	throw UnsupportedFeatureException("OpenSSL");
#else
	if (!__OpenSSLDigestAdded) {
		InitOpenSSLDigest();
	}
	setAlgorithm(name);
#endif
}

Digest::Digest(Algorithm algorithm)
{
	bytecount=0;
	m=NULL;
	ret=NULL;
	ctx=NULL;
#ifndef HAVE_OPENSSL
	throw UnsupportedFeatureException("OpenSSL");
#else
	if (!__OpenSSLDigestAdded) {
		InitOpenSSLDigest();
	}
	setAlgorithm(algorithm);
#endif
}

void Digest::setAlgorithm(const String &name)
{
#ifndef HAVE_OPENSSL
	throw UnsupportedFeatureException("OpenSSL");
#else
	m=EVP_get_digestbyname((const char*)name);
	if (!m) {
		throw InvalidAlgorithmException("%s",(const char*)name);
	}
	if (!ctx) {
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
		ctx=EVP_MD_CTX_new();
#else
		ctx=EVP_MD_CTX_create();
#endif
		if (!ctx) throw OutOfMemoryException();
	} else {
		reset();
	}
	EVP_DigestInit_ex((EVP_MD_CTX*)ctx,(const EVP_MD*)m, NULL);
#endif
}

void Digest::setAlgorithm(Algorithm algorithm)
{
#ifndef HAVE_OPENSSL
	throw UnsupportedFeatureException("OpenSSL");
#else

	switch(algorithm) {
#ifndef OPENSSL_NO_MD4
		case Algo_MD4: m=EVP_md4(); break;
#endif
#ifndef OPENSSL_NO_MD5
		case Algo_MD5: m=EVP_md5(); break;
#endif
#ifndef OPENSSL_NO_SHA
		case Algo_SHA1: m=EVP_sha1(); break;
#endif
#ifndef OPENSSL_NO_SHA256
		case Algo_SHA224: m=EVP_sha224(); break;
		case Algo_SHA256: m=EVP_sha256(); break;
#endif
#ifndef OPENSSL_NO_SHA512
		case Algo_SHA384: m=EVP_sha384(); break;
		case Algo_SHA512: m=EVP_sha512(); break;
#endif
#ifndef OPENSSL_NO_WHIRLPOOL
#if OPENSSL_VERSION_NUMBER >= 0x10001000L
		case Algo_WHIRLPOOL: m=EVP_whirlpool(); break;
#endif
#endif
#ifndef OPENSSL_NO_RIPEMD
		case Algo_RIPEMD160: m=EVP_ripemd160(); break;
#endif

		default: throw InvalidAlgorithmException();
	}
	if (!m) {
		throw InvalidAlgorithmException("%i",algorithm);
	}
	if (!ctx) {
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
		ctx=EVP_MD_CTX_new();
#else
		ctx=EVP_MD_CTX_create();
#endif
		if (!ctx) throw OutOfMemoryException();
	} else {
		reset();
	}
	EVP_DigestInit_ex((EVP_MD_CTX*)ctx,(const EVP_MD*)m, NULL);
#endif
}

void Digest::addData(const void *data, size_t size)
{
#ifndef HAVE_OPENSSL
	throw UnsupportedFeatureException("OpenSSL");
#else
	if (!m) throw NoAlgorithmSpecifiedException();
	EVP_DigestUpdate((EVP_MD_CTX*)ctx,data,size);
	bytecount+=size;
#endif
}

void Digest::addData(const ByteArrayPtr &data)
{
	addData(data.ptr(),data.size());
}

void Digest::addData(const String &data)
{
	addData(data.getPtr(),data.size());
}

void Digest::addData(const WideString &data)
{
	addData(data.getPtr(),data.size());
}


void Digest::addData(FileObject &file)
{
	file.seek(0);
	size_t bsize=1024*1024*1;		// We allocate 1 MB maximum
	uint64_t fsize=file.size();
	if (fsize<bsize) bsize=fsize;	// or filesize if file is < 1 MB
	void *buffer=malloc(bsize);
	if (!buffer) {
		throw OutOfMemoryException();
	}
	uint64_t rest=fsize;
	try {
		while(rest) {
			size_t bytes=rest;
			if (bytes>bsize) bytes=bsize;
			if (!file.read(buffer,bytes)) {
				throw ReadException();
			}
			addData(buffer,bytes);
			rest-=bytes;
		}
	} catch (...) {
		free(buffer);
		throw;
	}
	free(buffer);

}

void Digest::addFile(const String &filename)
{
	File ff;
	ff.open(filename,File::READ);
	addData(ff);
}

uint64_t Digest::bytesHashed() const
{
	return bytecount;
}

void Digest::saveDigest(ByteArray &result)
{
#ifndef HAVE_OPENSSL
	throw UnsupportedFeatureException("OpenSSL");
#else
	result=getDigest();
#endif
}

void Digest::saveDigest(String &result)
{
#ifndef HAVE_OPENSSL
	throw UnsupportedFeatureException("OpenSSL");
#else
	ByteArray ba=getDigest();
	result=ba.toHex();
#endif
}

void Digest::saveDigest(WideString &result)
{
#ifndef HAVE_OPENSSL
	throw UnsupportedFeatureException("OpenSSL");
#else
	ByteArray ba=getDigest();
	result=ba.toHex();
#endif
}


ByteArray Digest::getDigest()
{
#ifndef HAVE_OPENSSL
	throw UnsupportedFeatureException("OpenSSL");
#else
	unsigned int len;
	if (!ret) {
		ret=(unsigned char*)malloc(EVP_MAX_MD_SIZE);
		if (!ret) throw OutOfMemoryException();
	}
	EVP_DigestFinal((EVP_MD_CTX*)ctx,ret,&len);
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
	EVP_MD_CTX_reset((EVP_MD_CTX*)ctx);
#else
	EVP_MD_CTX_cleanup((EVP_MD_CTX*)ctx);
#endif
	EVP_DigestInit_ex((EVP_MD_CTX*)ctx,(const EVP_MD*)m, NULL);
	bytecount=0;
	return ByteArray(ret,len);
#endif
}

void Digest::reset()
{
#ifndef HAVE_OPENSSL
	throw UnsupportedFeatureException("OpenSSL");
#else
	if (!m) throw NoAlgorithmSpecifiedException();
	if (!ctx) throw NoAlgorithmSpecifiedException();
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
	EVP_MD_CTX_reset((EVP_MD_CTX*)ctx);
#else
	EVP_MD_CTX_cleanup((EVP_MD_CTX*)ctx);
#endif
	EVP_DigestInit((EVP_MD_CTX*)ctx,(const EVP_MD*)m);
	bytecount=0;
#endif
}


ByteArray Digest::hash(const ByteArrayPtr &data, Algorithm algorithm)
{
	Digest dig(algorithm);
	dig.addData(data);
	return dig.getDigest();
}

ByteArray Digest::hash(const ByteArrayPtr &data, const String &algorithmName)
{
	Digest dig(algorithmName);
	dig.addData(data);
	return dig.getDigest();
}

ByteArray Digest::md4(const ByteArrayPtr &data)
{
	return Digest::hash(data,Algo_MD4);
}

ByteArray Digest::md5(const ByteArrayPtr &data)
{
	return Digest::hash(data,Algo_MD5);
}

ByteArray Digest::sha1(const ByteArrayPtr &data)
{
	return Digest::hash(data,Algo_SHA1);
}

ByteArray Digest::sha224(const ByteArrayPtr &data)
{
	return Digest::hash(data,Algo_SHA224);
}

ByteArray Digest::sha256(const ByteArrayPtr &data)
{
	return Digest::hash(data,Algo_SHA256);
}

ByteArray Digest::sha384(const ByteArrayPtr &data)
{
	return Digest::hash(data,Algo_SHA384);
}

ByteArray Digest::sha512(const ByteArrayPtr &data)
{
	return Digest::hash(data,Algo_SHA512);
}

uint32_t Digest::crc32(const ByteArrayPtr &data)
{
#ifdef HAVE_LIBZ
	uLong crc=::crc32(0L,Z_NULL,0);
	return ::crc32(crc,(const Bytef*)data.ptr(),(uInt)data.size());
#endif
	return Crc32(data.ptr(),data.size());
}

uint32_t Digest::adler32(const ByteArrayPtr &data)
{
     const unsigned char *buffer = (const unsigned char *)data.ptr();
     size_t buflength=data.size();
     uint32_t s1 = 1;
     uint32_t s2 = 0;

     for (size_t n = 0; n < buflength; n++) {
        s1 = (s1 + buffer[n]) % 65521;
        s2 = (s2 + s1) % 65521;
     }
     return (s2 << 16) | s1;
  }
}
