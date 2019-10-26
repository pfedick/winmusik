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

#include "prolog.h"
#ifdef HAVE_STDIO_H
	#include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
	#include <string.h>
#endif
#ifdef HAVE_TIME_H
	#include <time.h>
#endif
#ifdef HAVE_FCNTL_H
	#include <fcntl.h>
#endif
#ifdef HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif
#ifdef HAVE_STDARG_H
	#include <stdarg.h>
#endif
#ifdef HAVE_DIRENT_H
	#include <dirent.h>
#endif
#ifdef HAVE_FNMATCH_H
	#include <fnmatch.h>
#endif
#ifdef HAVE_SYS_TIME_H
	#include <sys/time.h>
#endif
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#ifdef HAVE_CTYPE_H
	#include <ctype.h>
#endif

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN		// Keine MFCs
	#include <time.h>
	#include <windows.h>
	#include <io.h>
	#include <tchar.h>
	#include <memory.h>
	#include <direct.h>
#endif

#ifdef HAVE_PCRE
#define PCRE_STATIC
#include <pcre.h>
#endif

#include "ppl6.h"


namespace ppl6 {

CString Trim(const CString &str)
{
	CString ret=str;
	ret.Trim();
	return ret;
}

CString Trim(const char *str)
{
	CString ret=str;
	ret.Trim();
	return ret;
}

CWString Trim(const CWString &str)
{
	CWString ret=str;
	ret.Trim();
	return ret;
}

CString UCase(const CString &str)
{
	CString ret=str;
	ret.UCase();
	return ret;
}

CString UCase(const char *str)
{
	CString ret=str;
	ret.UCase();
	return ret;
}

CWString UCase(const CWString &str)
{
	CWString ret=str;
	ret.UCase();
	return ret;
}

CString LCase(const CString &str)
{
	CString ret=str;
	ret.LCase();
	return ret;
}

CString LCase(const char *str)
{
	CString ret=str;
	ret.LCase();
	return ret;
}


CWString LCase(const CWString &str)
{
	CWString ret=str;
	ret.LCase();
	return ret;
}

int StrCmp(const CString &s1, const CString &s2)
{
	int cmp=s1.StrCmp(s2);
	if (cmp<0) return -1;
	if (cmp>0) return 1;
	return 0;
}

int StrCmp(const CWString &s1, const CWString &s2)
{
	int cmp=s1.StrCmp(s2);
	if (cmp<0) return -1;
	if (cmp>0) return 1;
	return 0;
}

int StrCaseCmp(const CString &s1, const CString &s2)
{
	int cmp=s1.StrCaseCmp(s2);
	if (cmp<0) return -1;
	if (cmp>0) return 1;
	return 0;
}

int StrCaseCmp(const CWString &s1, const CWString &s2)
{
	int cmp=s1.StrCaseCmp(s2);
	if (cmp<0) return -1;
	if (cmp>0) return 1;
	return 0;
}

int Instr (const CString &string, const CString &such, size_t start)
{
	return string.Instr(such,start);
}

int InstrCase (const CString &string, const CString &such, size_t start)
{
	CString str=LCase(string);
	CString su=LCase(such);
	return str.Instr(su,start);
}

int Instr (const CWString &string, const CWString &such, size_t start)
{
	return string.Instr(such,start);
}

int InstrCase (const CWString &string, const CWString &such, size_t start)
{
	CWString str=LCase(string);
	CWString su=LCase(such);
	return str.Instr(su,start);
}

CString Left(const CString &str, size_t num)
{
	return str.Left(num);
}

CString Right(const CString &str, size_t num)
{
	return str.Right(num);
}

CString Mid(const CString &str, size_t start, size_t num)
{
	return str.Mid(start,num);
}

CString SubStr(const CString &str, size_t start, size_t num)
{
	return str.Mid(start,num);
}

CWString Left(const CWString &str, size_t num)
{
	return str.Left(num);
}

CWString Right(const CWString &str, size_t num)
{
	return str.Right(num);
}

CWString Mid(const CWString &str, size_t start, size_t num)
{
	return str.Mid(start,num);
}

CWString SubStr(const CWString &str, size_t start, size_t num)
{
	return str.Mid(start,num);
}

CString Replace(const CString &string, const CString &search, const CString &replace)
{
	CString Tmp=string;
	Tmp.Replace(search,replace);
	return Tmp;
}

CString ToString(const char *fmt, ...)
{
	CString String;
	va_list args;
	va_start(args, fmt);
	String.VaSprintf(fmt,args);
	va_end(args);
	return String;
}



int PregMatch(const CString &expression, const CString &subject)
{
	CArray matches;
	return PregMatch(expression,subject,matches);
}

int PregMatch(const CString &expression, const CString &subject, CArray &matches)
{
#ifndef HAVE_PCRE
	SetError(276);
	return 0;
#else
	matches.Clear();
	if (expression.IsEmpty()) return 0;
	if (subject.IsEmpty()) return 0;
	int ret=0;
	char *r=strdup(expression.GetPtr()+1);
	int flags=0;
	// letzten Slash in regex finden
	char *options=strrchr(r,'/');
	if (options) {
		options[0]=0;
		options++;
		if (strchr(options,'i')) flags|=PCRE_CASELESS;
		if (strchr(options,'m')) flags|=PCRE_MULTILINE;
		if (strchr(options,'x')) flags|=PCRE_EXTENDED;
		if (strchr(options,'s')) flags|=PCRE_DOTALL;
		if (strchr(options,'8')) flags|=PCRE_UTF8;
		if (strchr(options,'a')) flags|=PCRE_ANCHORED;
		if (strchr(options,'u')) flags|=PCRE_UNGREEDY;
	}
	// Wenn das System auf UTF-8 eingestellt ist, setzen wir das UTF8-Flag automatisch
	if (instrcase(setlocale(LC_CTYPE,NULL),"UTF-8",0)>=0) flags|=PCRE_UTF8;
	const char *perr;
	const char *buffer=subject.GetPtr();
	int len=subject.Len();
	char *tmp;
	int re,erroffset, ovector[32];
	int perrorcode;
	pcre *reg;
	//printf ("r=%s, flags=%i\n",r,flags);
	CString__PregMatch_Restart:
	reg=pcre_compile2(r,flags,&perrorcode,&perr, &erroffset, NULL);
	if (reg) {
		memset(ovector,0,30*sizeof(int));
		if ((re=pcre_exec(reg, NULL, buffer,len,0, 0, ovector, 30))>=0) {
			ret=1;
			for (int i=0;i<14;i++) {
				tmp=NULL;
				pcre_get_substring(buffer,ovector,30,i,(const char**)&tmp);
				if (tmp) {
					//printf("tmp[%i]=%s\n",i,tmp);
					matches.Set(i,tmp);
					pcre_free_substring(tmp);
				}
			}
		} else if ((flags&PCRE_UTF8)==PCRE_UTF8 && (re==PCRE_ERROR_BADUTF8 || re==PCRE_ERROR_BADUTF8_OFFSET)) {
			// Wir haben ungültiges UTF_8
			//printf ("ungültiges UTF-8");
			// Vielleicht matched es ohne UTF-8-Flag
			flags-=PCRE_UTF8;
			free(reg);
			goto CString__PregMatch_Restart;
		}
		free(reg);
	}
	free(r);
	return ret;
#endif

}



/*
** Translation Table as described in RFC1113
*/
static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
** Translation Table to decode (created by author)
*/
static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

/*
** encodeblock
**
** encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
void encodeblock( unsigned char in[3], unsigned char out[4], int len )
{
    out[0] = cb64[ in[0] >> 2 ];
    out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
}

/*
** encode
**
** base64 encode a stream adding padding and line breaks as per spec.
*/
CString ToBase64(const CBinary &bin)
{
	CString res;
    unsigned char in[3], out[4];
    int i, len;
    size_t p=0, filelen=bin.Len();

    while( p<filelen ) {
        len = 0;
        for( i = 0; i < 3; i++ ) {
            in[i] = (unsigned char) bin.Get(p++);
            if( p<=filelen) {
                len++;
            }
        }
        if( len ) {
            encodeblock( in, out, len );
            for( i = 0; i < 4; i++ ) {
            	res.Concatf("%c",out[i]);
            }
        }
    }
    return res;
}

CString ToBase64(const CString &str)
{
	CString res;
    unsigned char in[3], out[4];
    int i, len;
    size_t p=0, filelen=str.Len();

    while( p<filelen ) {
        len = 0;
        for( i = 0; i < 3; i++ ) {
            in[i] = (unsigned char) str.Get(p++);
            if( p<=filelen) {
                len++;
            }
        }
        if( len ) {
            encodeblock( in, out, len );
            for( i = 0; i < 4; i++ ) {
            	res.Concatf("%c",out[i]);
            }
        }
    }
    return res;
}
/*
** decodeblock
**
** decode 4 '6-bit' characters into 3 8-bit binary bytes
*/
static void decodeblock( unsigned char in[4], unsigned char out[3] )
{
    out[ 0 ] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
    out[ 1 ] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
    out[ 2 ] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}

/*
** decode
**
** decode a base64 encoded stream discarding padding, line breaks and noise
*/
CBinary FromBase64(const CString &str)
{
	CBinary res;
    unsigned char in[4], out[3], v;
    int i, len;
    size_t p=0, filelen=str.Len();
    while( p<filelen) {
        for( len = 0, i = 0; i < 4 && p<filelen; i++ ) {
            v = 0;
            while( p<filelen+1 && v == 0 ) {
                v = (unsigned char) str.Get(p++);
                v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
                if( v ) {
                    v = (unsigned char) ((v == '$') ? 0 : v - 61);
                }
            }
            if( p<filelen+1 ) {
                len++;
                if( v ) {
                    in[ i ] = (unsigned char) (v - 1);
                }
            }
            else {
                in[i] = 0;
            }
        }
        if( len) {
            decodeblock( in, out );
            res.Concat(out,len-1);
            len=0;
        }
    }
    return res;
}

CString EscapeHTMLTags(const ppl6::CString &html)
{
	ppl6::CString s;
	s=html;
	s.Replace("&","&amp;");
	s.Replace("<","&lt;");
	s.Replace(">","&gt;");
	return s;
}

CString UnescapeHTMLTags(const ppl6::CString &html)
{
	ppl6::CString s;
	s=html;
	s.Replace("&amp;","&");
	s.Replace("&lt;","<");
	s.Replace("&gt;",">");
	return s;
}

CBinary Hex2Binary(const CString &hex)
{
	CBinary b;
	b.FromHex(hex);
	return b;
}

CMemory Hex2Memory(const CString &hex)
{
	CMemory b;
	b.fromHex(hex);
	return b;
}

CString ToHex(const CBinary &bin)
{
	return bin.ToHex();
}

CString ToHex(const CMemoryReference &bin)
{
	return bin.toHex();
}


/*!\brief String zur Verwendung in einer URL umwandeln
 *
 * \desc
 * Mit dieser statischen Funktion kann ein beliebiger String so umkodiert werden, dass er als
 * Parameter in einer URL verwendet werden kann. Dabei werden alle Spaces durch "+" ersetzt
 * und alle nicht alphanummerischen Zeichen mit Ausnahme von "-_.!~*'()" in ihre Hex-Werte
 * mit vorangestelltem Prozentzeichen.
 *
 * @param text Der zu kodierende Text
 * @return Der URL-kodierte Text
 *
 * \example
 * \code
 * ppl6::CString text="Hallo Welt! 1+1=2";
 * printf("%s\n",(const char*)ppl6::urlEncode(text));
 * \endcode
 * ergibt:
 * \verbatim
Hallo+Welt!+1%2B1%3D2
\endverbatim
 * \see
 * Mit urlDecode kann der Kodierte String wieder dekodiert werden
 */
CString UrlEncode(const CString &text)
{
	const char *source=text.GetPtr();
	ppl6::CString ret;
	static const char *digits = "0123456789ABCDEF";
	unsigned char ch;
	while (*source)
	{
		ch = (unsigned char)*source;
		if (*source == ' ') {
			ret+="+";
		}
		else if (
				(ch>='a' && ch<='z')
				|| (ch>='A' && ch<='Z')
				|| (ch>='0' && ch<='9')
				|| (strchr("-_.!~*'()", ch))
				) {
			ret+=ch;
		}
		else {
			ret+="%";
			ret+= digits[(ch >> 4) & 0x0F];
			ret+= digits[       ch & 0x0F];
		}
		source++;
	}
	return ret;
}

static int HexPairValue(const char * code) {
  int value = 0;
  const char * pch = code;
  for (;;) {
    int digit = *pch++;
    if (digit >= '0' && digit <= '9') {
      value += digit - '0';
    }
    else if (digit >= 'A' && digit <= 'F') {
      value += digit - 'A' + 10;
    }
    else if (digit >= 'a' && digit <= 'f') {
      value += digit - 'a' + 10;
    }
    else {
      return -1;
    }
    if (pch == code + 2)
      return value;
    value <<= 4;
  }
}

/*!\brief URL-kodierten String dekodieren
 *
 * \desc
 * Mit dieser statischen Funktion kann ein URL-kodierter String dekodiert werden.
 *
 * @param text Der zu URL-kodierte String
 * @return Der dekodierte String
 *
 * \example
 * \code
 * ppl6::CString text="Hallo+Welt!+1%2B1%3D2";
 * printf("%s\n",(const char*)ppl6::urlDecode(text));
 * \endcode
 * ergibt:
 * \verbatim
Hallo Welt! 1+1=2";
\endverbatim
 * \see
 * Mit urlEncode kann ein unkodierter String kodiert werden.
 */
CString UrlDecode(const CString &text)
{
	const char *source=text.GetPtr();
	ppl6::CString ret;

	while (*source) {
		switch (*source) {
			case '+':
				ret+=" ";
				break;
			case '%':
				if (source[1] && source[2]) {
					int value = HexPairValue(source + 1);
					if (value >= 0) {
						ret+=value;
						source += 2;
					}
					else {
						ret+="?";
					}
				}
				else {
					ret+="?";
				}
				break;
			default:
				ret+=*source;
		}
		source++;
	}
	return ret;
}


CString XmlEscape(const CString &text)
{
	ppl6::CString Value=text;
	Value.Replace("&","&amp;");
	Value.Replace("\"","&quot;");
	Value.Replace("'","&#039;");
	Value.Replace("<","&lt;");
	Value.Replace(">","&gt;");
	return Value;
}

CString XmlUnEscape(const CString &text)
{
	ppl6::CString Value=text;
	Value.Replace("&amp;","&");
	Value.Replace("&lt;","<");
	Value.Replace("&gt;",">");
	Value.Replace("&quot;","\"");
	Value.Replace("&#039;","'");
	return Value;
}



}	// EOF namespace ppl6
