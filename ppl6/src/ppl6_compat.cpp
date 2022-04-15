/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 6 (PPL6).
 * Web: http://www.pfp.de/ppl/
 *
 * $Author$
 * $Revision$
 * $Date$
 * $Id$
 *
 *******************************************************************************
 * Copyright (c) 2010, Patrick Fedick <patrick@pfp.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
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

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Keine MFCs
#define _WIN32_WINNT 0x501
#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <windows.h>
#endif

#include "prolog.h"

#ifdef MINGW32
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif


#include "ppl6.h"


namespace ppl6 {
/*!\brief Findet das erste Auftauchen einer Zeichenkette in einem String
 */
const char *strcasestr(const char *haystack, const char *needle)
{
#ifdef HAVE_STRCASESTR
	return ::strcasestr(haystack,needle);
#else
    char c, sc;
    size_t len;

    if ((c = *needle++) != 0) {
        c = tolower((unsigned char)c);
        len = strlen(needle);
        do {
            do {
                if ((sc = *haystack++) == 0)
                    return (NULL);
            } while ((char)tolower((unsigned char)sc) != c);
        } while (strncasecmp(haystack, needle, len) != 0);
        haystack--;
    }
    return ((char *)haystack);
#endif
}

int strcasecmp(const char *s1, const char *s2)
{
#ifdef HAVE_STRCASECMP
	return ::strcasecmp(s1,s2);
#else
	const unsigned char
	*us1 = (const unsigned char *)s1,
	*us2 = (const unsigned char *)s2;

	while (tolower(*us1) == tolower(*us2++))
		if (*us1++ == '\0')
			return (0);
	return (tolower(*us1) - tolower(*--us2));
	#endif
}

int strncasecmp(const char *s1, const char *s2, size_t n)
{
#ifdef HAVE_STRNCASECMP
	return ::strncasecmp(s1,s2,n);
#else
    if (n != 0) {
        const unsigned char
                *us1 = (const unsigned char *)s1,
                *us2 = (const unsigned char *)s2;

        do {
            if (tolower(*us1) != tolower(*us2++))
                return (tolower(*us1) - tolower(*--us2));
            if (*us1++ == '\0')
                break;
        } while (--n != 0);
    }
    return (0);
#endif
}


int vasprintf(char **buff, const char *fmt, va_list args)
{
	#if defined HAVE_VASPRINTF
		return ::vasprintf(buff,fmt,args);
	#elif defined HAVE_WORKING_VSNPRINTF
		char tb[4];
		int size=vsnprintf(tb,1,fmt,args);
		char *b=(char*)malloc(size+2);
		if (b) {
			vsnprintf(b,size+1,fmt,args);
			*buff=b;
			return size;
		} else {
			*buff=NULL;
			return 0;
		}
	#elif defined _WIN32
		// Feststellen, wie groß der String werden würde
		int size=_vscprintf(fmt,args);
		// Buffer allocieren
		char *b=(char*)malloc(size+2);
		if (b) {
			_vsnprintf(b,size+1,fmt,args);
			*buff=b;
			return size;
		} else {
			*buff=NULL;
			return 0;
		}

	#else
		#pragma error No working vasprintf!!!
		*buff=NULL;
		return 0;
    #endif
}

int asprintf(char **buff, const char *format, ...)
{
	va_list args;
	va_start(args,format);
	char *b=NULL;
	vasprintf(&b,format,args);
	va_end(args);
	if (b) {
		*buff=b;
		return (int)strlen(b);
	}
	*buff=NULL;
	return -1;
}


long htol (const char * wert)
{
	long mp[]={0x1,0x10,0x100,0x1000,0x10000,0x100000,0x1000000,0x10000000};
	size_t i,l;
	long w;
	char * p,t;
	strtolower ((char*)wert);
	p=trim((char*)wert);
	w=0;
	l=strlen(p);
	for (i=0;i<l;i++) {
		t=p[l-i-1]-48;
		if (t>9) t=t-39;
		w+=t*mp[i];
	}
	return (w);
}

long long atoll (const char *wert)
{
	if(!wert) return 0;
#ifdef HAVE_ATOLL
	return ::atoll(wert);
#elif defined _WIN32
	return (long long) _atoi64(wert);
#elif defined HAVE_STRTOLL
	return strtoll(str, (char **)NULL, 10);
#else
	return 0;
#endif
}

int atoi(const char *wert)
{
	if (!wert) return 0;
	return ::atoi(wert);
}

long atol(const char *wert)
{
	if (!wert) return 0;
	return ::atol(wert);
}

double atof(const char *wert)
{
	if (!wert) return 0.0;
	return ::atof(wert);
}


char *strndup(const char *str, size_t len)
{
#ifdef HAVE_STRNDUP
	return ::strndup(str,len);
#else
	if (len<0) return NULL;
	if (!str) return NULL;
	size_t ll=strlen(str);
	if (ll<len) len=ll;
	char *buff=(char*)malloc(len+1);
	if (!buff) return NULL;
	strncpy(buff,str,len);
	buff[len]=0;
	return buff;
#endif
}

int strncmp(const char *s1, const char *s2, size_t len)
{
#ifdef HAVE_STRNCMP
	return ::strncmp(s1,s2,len);
#else
	if (len==0) return 0;
	do {
		if (*s1 != *s2++)
			return (*(const unsigned char *)s1 - *(const unsigned char *)(s2 - 1));
			if (*s1++ == 0) break;
	} while (--len != 0);
	return (0);
#endif
}

void bcopy(const void *src, void *dst, size_t len)
{
#ifdef HAVE_BCOPY
	::bcopy(src,dst,len);
#elif defined _WIN32
	CopyMemory(dst,src,len);
#elif defined HAVE_MEMMOVE
	::memmove(dst,src,len);

#endif
}

size_t strnlen(const char *str, size_t len)
/*!\brief Länge des Strings bis zu einer bestimmten Position
 *
 * \descr
 * Diese Funktion ist vergleichbar mit der Funktion \c strlen aus der libc, mit dem Unterschied,
 * dass sie einen zweiten Parameter erwartet, der die Länge des Buffers angibt.
 * \par
 * Diese Funktion sollte verwendet werden, wenn nicht bekannt ist, ob der String am Ende
 * ein 0-Byte enthält, aber der Buffer nur eine bestimmte Länge hat. \c strlen würde in diesem
 * Fall über das Ende des Buffers hinauslesen und entweder einen falschen Wert zurückliefern
 * oder eine Schutzverletzung verursachen
 *
 * \param[in] str Pointer auf den Buffer mit dem String
 * \param[in] len Maximale Länge des Buffers/String
 * \return Liefert die Position des ersten gefundenen Null-Bytes zurück oder \p len, falls
 * kein 0-Byte gefunden wurde, \p len=0 ist oder \p str=NULL ist.
 *
 * \since
 * Wurde mit Version 6.3.0 eingeführt
 */
{
	if (str==NULL || len==0) return 0;
	size_t p=0;
	while (p<len) {
		if (str[p]==0) return p;
		p++;
	}
	return p;
}

}	// EOF namespace ppl6


