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
 * Copyright (c) 2013, Patrick Fedick <patrick@pfp.de>
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

#ifndef _PPL6_INCLUDE
#define _PPL6_INCLUDE

#define PPL_VERSION_MAJOR	6
#define PPL_VERSION_MINOR	4
#define PPL_VERSION_BUILD	22
#define PPL_RELEASEDATE		20180729

// Inlcude PPL6 configuration file
#ifndef _PPL6_CONFIG
	#ifdef PPL6LIB
		#ifdef __MINGW32__
			#include "config.h"
		#elif defined _WIN32
			#include "ppl6-config.h"
		#else
			#include "config.h"
		#endif
	#else
		#include <ppl6-config.h>
	#endif
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef  HAVE_STRINGS_H
#include <strings.h>
#endif

#include <string>
#include <iostream>
#include <list>


#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif

#ifdef WITH_QT
	#include <QString>
	#include <QVariant>
	#include <QByteArray>
#endif


#ifdef _M_IX86
#ifndef i386
#define i386
#endif
#endif

#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifdef PPL6LIB
	#include "ppl6-exceptions.h"
#else
	#include <ppl6-exceptions.h>
#endif

namespace ppl6 {

//#define PPL6_ENABLE_STRING_BUFFERPOOL
#undef PPL6_ENABLE_STRING_BUFFERPOOL

#define ppldb	ppluint8
#define ppldw	ppluint16
#define ppldd	ppluint32
#define ppld64	ppluint64
#define ppldbs	pplint8
#define ppldws	pplint16
#define ppldds	pplint32
#define ppld64s	pplint64

class Bits {
	enum BITMASKEN {
		Bit0	=0x00000001,
		Bit1	=0x00000002,
		Bit2	=0x00000004,
		Bit3	=0x00000008,
		Bit4	=0x00000010,
		Bit5	=0x00000020,
		Bit6	=0x00000040,
		Bit7	=0x00000080,
		Bit8	=0x00000100,
		Bit9	=0x00000200,
		Bit10	=0x00000400,
		Bit11	=0x00000800,
		Bit12	=0x00001000,
		Bit13	=0x00002000,
		Bit14	=0x00004000,
		Bit15	=0x00008000,
		Bit16	=0x00010000,
		Bit17	=0x00020000,
		Bit18	=0x00040000,
		Bit19	=0x00080000,
		Bit20	=0x00100000,
		Bit21	=0x00200000,
		Bit22	=0x00400000,
		Bit23	=0x00800000,
		Bit24	=0x01000000,
		Bit25	=0x02000000,
		Bit26	=0x04000000,
		Bit27	=0x08000000,
		Bit28	=0x10000000,
		Bit29	=0x20000000,
		Bit30	=0x40000000,
		Bit31	=0x80000000,
	};
};


class datatype {
	public:
		enum {
			UNKNOWN		=0, /*!< unbekannter Datentyp */
			CHAR		=1,	/*!< char-pointer */
			INT			=2, /*!< Integer */
			FLOAT		=3, /*!< Float */
			CSTRING		=4, /*!< CString-Klasse */
			ARRAY		=5, /*!< CAssocArray-Klasse */
			BINARY		=6, /*!< CBinary-Klasse */
			POINTER		=7, /*!< void-pointer */
			CWSTRING	=8, /*!< CWString-Klasse */
			CARRAY		=9,	/*!< CArray-Klasse */
			DATETIME	=10,/*!< CDateTime-Klasse */
			BOOLEAN		=11 /*!< Boolean */
		};
};


// Directory
#define CPPLDIR_MAXFILENAMELEN	1024
#define CPPLDIR_NORMAL			0x00
#define CPPLDIR_READONLY		0x01
#define CPPLDIR_DIR				0x02
#define CPPLDIR_LINK			0x04
#define CPPLDIR_HIDDEN			0x08
#define CPPLDIR_SYSTEM			0x10
#define CPPLDIR_ARCHIV			0x20
#define CPPLDIR_FILE			0x40

//!\brief File Attribute
namespace FILEATTR {
	enum {
		ISUID		= 0x4000,
		ISGID		= 0x2000,
		ISVTX		= 0x1000,
		STICKY		= 0x1000,
		USR_READ	= 0x0400,
		USR_WRITE	= 0x0200,
		USR_EXECUTE	= 0x0100,
		GRP_READ	= 0x0040,
		GRP_WRITE	= 0x0020,
		GRP_EXECUTE	= 0x0010,
		OTH_READ	= 0x0004,
		OTH_WRITE	= 0x0002,
		OTH_EXECUTE	= 0x0001,
		CHMOD_755	= 0x0755,
		CHMOD_644	= 0x0644
	};
}

typedef struct {    // rc
	int left;
	int top;
	int right;
	int bottom;
} RECT;

typedef struct {
	ppldds x;
	ppldds y;
} POINT;

typedef struct {
	int cx;
	int cy;
} SIZE;

typedef struct {
	int left;
	int top;
	int right;
	int bottom;
} intRECT;


//! \brief Header von PFP-Dateien
typedef struct {
	ppldb header[8];
	ppldb header_version;
	ppldb header_length;
	ppldb file_id[4];
	ppldb subversion;
	ppldb mainversion;
	ppldw offset_name;
	ppldw offset_autor;
	ppldd date_creation;
	ppldd time_creation;
	ppldd date_update;
	ppldd time_update;
	ppldw compression;
} PFPSTRUCT;

//! \brief Eine Struktur zum Erfassen von Uhrzeit und Datum
typedef struct tagTime {
	ppluint64	epoch;
	int			year;
	int			month;
	int			day;
	int			hour;
	int			min;
	int			sec;
	int			day_of_week;
	int			day_of_year;
	int			summertime;
	int			gmt_offset;
	int			have_gmt_offset;
} PPLTIME;

//! \brief Datentyp für Unix-Time
typedef ppluint64 ppl_time_t;

typedef struct tagWinRect {
	int left;
	int top;
	int width;
	int height;
} WINRECT;

class MemoryHeap
 {
 	private:
 		void		*blocks;
 		size_t		myElementSize, increaseSize;
 		size_t		myGrowPercent;
 		size_t		blocksAllocated, blocksUsed;
 		size_t		mem_allocated;
 		size_t		mem_used;
 		size_t		freeCount;

 		void		increase(size_t num);

 	public:
 		PPLNORMALEXCEPTION(NotInitializedException);
 		PPLNORMALEXCEPTION(AlreadyInitializedException);
 		PPLNORMALEXCEPTION(HeapCorruptedException);
 		PPLNORMALEXCEPTION(ElementNotInHeapException);



 		MemoryHeap();
 		MemoryHeap(size_t elementsize, size_t startnum, size_t increase, size_t growpercent=30);
 		~MemoryHeap();
 		void clear();
 		void init(size_t elementsize, size_t startnum, size_t increase, size_t growpercent=30);
 		void *malloc();
 		void *calloc();
 		void free(void *element);
 		size_t memoryUsed() const;
 		size_t memoryAllocated() const;
 		void dump() const;
 		size_t capacity() const;
 		size_t count() const;
 		size_t elementSize() const;
 		void reserve(size_t num);
 		void cleanup();
 };

}	// EOF namespace ppl6
// Inlcude PPL6 Algorithms
#ifdef PPL6LIB
#include "ppl6-algorithms.h"
#else
#include <ppl6-algorithms.h>
#endif

namespace ppl6 {

class CArray;
class CAssocArray;
class CCallback;
class CString;
class CWString;
class CMemMan;
class CDirEntry;
class Cppl6Core;
class CFile;
class CFileObject;
class CMemFile;
class CResource;
class CHeap;
class CBinary;

/*
namespace Exception {
	class InvalidAddress {};
	class OutOfMemory {};
	class FunctionUnavailable {};
	class InvalidFormat {};
	class ExistingCGrafixInstance {};
	class FileOpenFailed {};
	class ImageLoadFailed {};
	class InitialisationFailed {};
	class CopyFailed {};
	class InvalidParameter {};
	class FunctionFailed {};
};
*/


class CMemoryReference
{
	friend class CMemory;
	private:
		void *ptr;
		size_t s;
	public:
		CMemoryReference();
		CMemoryReference(const CMemoryReference &other);
		CMemoryReference(const CBinary &bin);
		CMemoryReference(void *adr, size_t size);
		bool isNull() const;
		bool isEmpty() const;
		size_t size() const;
		void *adr() const;
		void set(void *adr, size_t size);
		void set(const CMemoryReference &other);
		void set(const CBinary &bin);
		CMemoryReference &operator=(const CMemoryReference &other);
		CMemoryReference &operator=(const CBinary &bin);
		operator void*() const;
		operator CBinary() const;
		CString toHex() const;
		unsigned char operator[](size_t pos) const;
};

class CMemory : public CMemoryReference
{
private:

public:
	CMemory();
	CMemory(const CMemoryReference &other);
	CMemory(const CBinary &bin);
	CMemory(void *adr, size_t size);
	CMemory(size_t size);
	~CMemory();
	void set(void *adr, size_t size);
	void set(const CMemoryReference &other);
	void set(const CBinary &bin);
	void *copy(void *adr, size_t size);
	void *copy(const CMemoryReference &other);
	void *copy(const CBinary &bin);
	void *fromHex(const CString &hex);
	void *malloc(size_t size);
	void *calloc(size_t size);
	void free();
	CMemory &operator=(const CMemoryReference &other);
	CMemory &operator=(const CBinary &bin);
	operator void*() const;
	unsigned char operator[](size_t pos) const;
};

//! \brief Basisklasse für verschiedene Datentypen
class CVar
{
	public:

		enum PPLDataType {
			UNKNOWN		=0,
			CINT		=2,
			CSTRING		=4,
			CASSOCARRAY	=5,
			CBINARY		=6,
			CWSTRING	=8,
			CARRAY		=9,
			CDATETIME	=10,
			CBOOL		=11,
		};
	protected:
		ppluint8		type;

	public:
		CVar();
		CVar(const CVar &copy);
		int DataType() const;
		int IsType(int type) const;
		CVar& operator=(const CVar& var);
};

//! \brief Integer-Klasse
class CInt : public CVar
{
	private:
		int	value;
	public:
		CInt();
		CInt(const int val);
		CInt(const char *str);
		CInt(const CInt &val);
		CInt(const CString &str);
		~CInt();

		operator int() const;
		operator unsigned int() const;

		operator long() const;
		operator unsigned long() const;

		operator short() const;
		operator unsigned short() const;

		operator char() const;
		operator unsigned char() const;

		int GetVal();

		CInt& operator=(const char* str);
		CInt& operator=(const CString& str);
		CInt& operator=(const int val);
		CInt& operator=(const CInt &val);
		CInt& operator+=(const int val);
		CInt& operator|=(const int val);
		CInt operator+(const int val);
		CInt operator+(const CInt &val);
		CInt& operator+=(const CInt& val);
		CInt& operator-=(const int val);
		CInt& operator-=(const CInt& val);
		CInt& operator++();
		CInt& operator--();
		//CString operator+(const char* str) const;
		//CString operator+(const CString& str) const;


		bool operator<(const int val) const;
		bool operator<=(const int val) const;
		bool operator==(const int val) const;
		bool operator!=(const int val) const;
		bool operator>=(const int val) const;
		bool operator>(const int val) const;
};

//! \brief Boolean-Klasse
class CBool : public CVar
{
	private:
		bool value;
	public:
		CBool();
		CBool(const int val);
		CBool(const bool val);
		CBool(const char *str);
		CBool(CString *str);
		CBool(const CString &str);
		CBool(CInt *str);
		CBool(const CInt &str);
		~CBool();

		operator int() const;
		operator unsigned int() const;
		operator bool() const;

		CBool& operator=(const char* str);
		CBool& operator=(const CString& str);
		CBool& operator=(const int val);
		CBool& operator=(const bool val);
		CBool& operator=(const CInt &val);
		CBool& operator=(const CBool &val);
		bool	IsTrue();
		bool	IsFalse();

		bool operator==(bool val) const;
		bool operator!=(bool val) const;

};

class CString : public CVar
{
	private:
		char		*buffer;
		CArray		*matches;
		ppluint32	len,buffersize;
		void Init();
		void ReCalcLen();

	public:
		CString();
		CString(const char *str);
		CString(const char *str, size_t size);
		CString(const wchar_t *str);
		CString(CString *str);
		CString(const CString &str);
		CString(const std::string &str);
		CString(const CWString &str);
		CString(const CBinary &bin);

		~CString();

#ifdef PPL6_ENABLE_STRING_BUFFERPOOL
		void *operator new (size_t size);
		void operator delete (void *ptr, size_t size);
#endif

		void ImportBuffer(char *buffer, size_t bytes);

		// Liefert die Laenge des Strings zurueck
		ppluint32 Size() const;
		ppluint32 Length() const;
		ppluint32 Len() const;


		void Clear();

		// Reserviert Speicher fuer den String. Ein eventuell schon vorhandener
		// String geht dabei nicht verloren
		int Reserve(ppluint32 size);
		// Liefert die maximale Groesse des Strings zurueck, ohne dass neuer
		// Speicher reallokiert werden muss
		int Capacity() const;
		static void setInitialBuffersize(ppluint32 size);


		int IsNumeric() const;
		int IsInteger() const;
		int IsEmpty() const;
		int NotEmpty() const;


		void Print(bool attach_newline=false) const;
		void HexDump() const;
		void HexDump(const void *buffer, ppldd bytes, bool skipheader=false);

		void Set(const char *text, int bytes=-1);
		void Set(const CString *str, int bytes=-1);
		void Set(const CString &str, int bytes=-1);
		void Set(const CWString &str, int size=-1);
		void Set(const std::string &str, int bytes=-1);
		void Set(const std::wstring &str, int size=-1);
		void Set(const wchar_t *str, int size=-1);
		void Setf(const char *fmt, ...);
		void SetChar(char c);
		void SetChar(char c, int pos);



		void Sprintf(const char *fmt, ...);
		void VaSprintf(const char *fmt, va_list args);
		void Vasprintf(const char *fmt, va_list args);

		void Concat(const char *text, int bytes=-1);
		void Concat(CString &str, int bytes=-1);
		void Concatf(const char *fmt, ...);
		void Copy(const char *str, int bytes=-1);
		void Copy(CString &str, int bytes=-1);
		void Strcpy(const CString &str);
		void Strcpy(const char *str);
		void Strncpy(const char *str, size_t bytes);
		void Strncpy(const CString &str, size_t bytes);
		void Add(const char *str, int bytes=-1);
		const char *GetPtr() const;
		char Get(int pos) const;
		void AddChar(char c);

		void StripSlashes();
		void Trim();
		void LTrim();
		void RTrim();
		void Trim(const char *str);
		void LTrim(const char *str);
		void RTrim(const char *str);
		void Chomp();
		void Chop(int chars=1);
		void LCase();
		void UCase();
		void UCWords();
		void TrimL(ppluint32 chars);
		void TrimR(ppluint32 chars);
		void Cut(int position);
		void Cut(const char *letter);
		void Shr(char c, int size);
		void Shl(char c, int size);
		int StrCmp(const char *str, int size=0) const;
		int StrCmp(const CString &str, int size=0) const;
		int StrCaseCmp(const char *str, int size=0) const;
		int StrCaseCmp(const CString &str, int size=0) const;
		int Instr(const char *str, int pos=0) const;
		int Instr(const CString &str, int pos=0) const;
		int InstrCase(const CString &str, int pos=0) const;

		CString& Repeat(int num);
		CString& Repeat(const char *str, int num);
		CString& Repeat(char ascii, int num);
		CString& Repeat(const CString& str, int num);

		int ToInt() const;
		pplint64 ToInt64() const;
		bool ToBool() const;
		long ToLong() const;
		long long ToLongLong() const;
		float ToFloat() const;
		double ToDouble() const;
		bool IsTrue() const;
		bool IsFalse() const;

		CString& Replace(const char* str, const char* byStr);
		int Find(const char* str, int pos) const;

		int PregMatch(const char *expression, CArray *res=NULL);
		int PregMatch(const CString &expression, CArray &res) const;
		const char *GetMatch(int index) const;
		int PregReplace(const char *expression, const char *replace, int maxreplace=0);
		int PregReplace(const char *expression, CString &replace, int maxreplace=0);
		void PregEscape();

		CString Left(ppluint32 len) const;
		CString Right(ppluint32 len) const;
		CString Mid(ppluint32 start, ppluint32 len = 0xffffffff) const;
		CString SubStr(ppluint32 start, ppluint32 len = 0xffffffff) const;
		CString GetMD5() const;
		int	MD5(const CString &str);
		int	MD5(const char *str);
		int	MD5(const char *buffer, int size);
		int	MD5();
		int	Transcode(const char *fromcharset, const char *tocharset="UTF-8");
		int Transcode(const char *fromcharset, const char *tocharset, CBinary &Target) const;
		int	TranscodeText(const CString &text, const char *fromcharset, const char *tocharset="UTF-8");
		int	TranscodeText(const char *text, const char *fromcharset, const char *tocharset="UTF-8");
		int	TranscodeText(const char *text, int size, const char *fromcharset, const char *tocharset="UTF-8");

		operator const char *() const;
		operator int() const;
		operator bool() const;
		operator unsigned int() const;

		char operator[](int pos) const;
		CString& operator=(char* str);
		CString& operator=(const char* str);
		CString& operator=(const wchar_t* str);
		CString& operator=(const CString *str);
		CString& operator=(const std::string &str);
		CString& operator=(const CString& str);
		CString& operator=(const CWString& str);
		CString& operator+=(const char* str);
		CString& operator+=(int c);
		CString& operator+=(const CString& str);
		CString& operator=(const CBinary& str);

		bool operator<(const char* str) const;
		bool operator<=(const char* str) const;
		bool operator==(const char* str) const;
		bool operator!=(const char* str) const;
		bool operator>=(const char* str) const;
		bool operator>(const char* str) const;

		bool operator<(const CString &str) const;
		bool operator<=(const CString &str) const;
		bool operator==(const CString &str) const;
		bool operator!=(const CString &str) const;
		bool operator>=(const CString &str) const;
		bool operator>(const CString &str) const;


#ifdef WITH_QT
		CString(const QString &q) {
			Init();
			QByteArray a=q.toUtf8();
			Set((const char*)a);
		}
		CString(QString *q) {
			Init();
			QByteArray a=q->toUtf8();
			Set((const char*)a);
		}
		operator const QString() const {
			QString q=GetPtr();
			return q;
		}

		operator const QVariant() const {
			QString q=GetPtr();
			QVariant v=q;
			return v;
		}

		CString& operator=(const QString& q) {
			QByteArray a=q.toUtf8();
			Set((const char*)a);
			return *this;
		}
		CString& operator=(const QString *q) {
			QByteArray a=q->toUtf8();
			Set((const char*)a);
			return *this;
		}
		CString& operator=(QString& q) {
			QByteArray a=q.toUtf8();
			Set((const char*)a);
			return *this;
		}
		CString& operator=(QString *q) {
			QByteArray a=q->toUtf8();
			Set((const char*)a);
			return *this;
		}

#endif


//		int operator==(const char *str);

};
CString operator+(const CString &str1, const CString& str2);
CString operator+(const char *str1, const CString& str2);
CString operator+(const CString &str1, const char *str2);

std::ostream& operator<<(std::ostream& s, const CString &str);


int	SetGlobalEncoding(const char *encoding);
const char *GetGlobalEncoding();

class CWString : public CVar
{
	private:
		CArray		*matches;
		wchar_t		*buffer;
		char		*extbuffer;
		size_t		extbuffersize;
		size_t		len,buffersize,bufferused;
		void		*iconvimport;
		void		*iconvexport;
		char		extencoding[32];
		void 		Init();
		void 		ReCalcLen();

		int			InitImportEncoding(const char *encoding);
		int			InitExportEncoding(const char *encoding);

	public:
#ifdef PPL6_ENABLE_STRING_BUFFERPOOL
		void *operator new (size_t size);
		void operator delete (void *ptr, size_t size);
#endif

		CWString();
		CWString(const char *str);
		CWString(const wchar_t *str);
		CWString(const CString *str);
		CWString(const CString &str);
		CWString(const CWString *str);
		CWString(const CWString &str);
		CWString(const std::wstring &str);
		CWString(const wchar_t c);


		~CWString();
		void ImportBuffer(wchar_t *buffer, size_t bytes);

		int	SetExtEncoding(const char *encoding);

		// Liefert die Laenge des Strings zurueck
		size_t Size() const;
		size_t Length() const;
		size_t Len() const;

		void Clear();

		int Reserve(int chars);


		int IsNumeric() const;
		int IsInteger() const;
		// liefert true (!=0) zurueck, wenn der String leer ist
		int IsEmpty() const;
		int NotEmpty() const;

		int Capacity() const;

		void Print(bool attach_newline=false) const;
		void HexDump() const;
		void HexDump(const void *buffer, int bytes, bool skipheader=false);
		int Set(const char *text, int size=-1);
		int Set(const wchar_t *text, int size=-1);
		int Set(const CString *str, int size=-1);
		int Set(const CString &str, int size=-1);
		int Set(const CWString *str, int chars=-1);
		int Set(const CWString &str, int chars=-1);
		int SetChar(wchar_t c);
		int SetChar(wchar_t c, int pos);


		int Setf(const char *fmt, ...);
		int Setf(const wchar_t *fmt, ...);
		int Sprintf(const char *fmt, ...);
		int VaSprintf(const char *fmt, va_list args);
		int Vasprintf(const char *fmt, va_list args);

		int Strcpy(const char *str);
		int Strncpy(const char *str, int size);
		int Copy(const char *str, int size=-1);
		int Copy(const CString &str, int size=-1);
		int Add(const char *str, int size=-1);
		int AddChar(wchar_t c);

		int Concat(const char *text, int size=-1);
		int Concat(const wchar_t *text, int size=-1);
		int Concat(CString *str, int size=-1);
		int Concat(CWString *str, int size=-1);
		int Concat(CString str, int size=-1);
		int Concat(CWString str, int size=-1);
		int Concatf(const char *fmt, ...);
		int Concatf(const wchar_t *fmt, ...);
		int ConcatChar(wchar_t c);

		int Import(const char *encoding, void *buffer, int bytes=-1);

		int InsertChar(wchar_t c, int pos);
		int Delete(int pos, int num);


		void StripSlashes();
		void Trim();
		void LTrim();
		void RTrim();
		void Chomp();
		void Chop(int chars=1);
		void TrimL(int chars);
		void TrimR(int chars);
		void Cut(int position);
		void LCase();
		void UCase();
		void UCWords();
		void Trim(wchar_t c);
		void LTrim(wchar_t c);
		void RTrim(wchar_t c);
		void Trim(const char *str);
		void LTrim(const char *str);
		void RTrim(const char *str);
		CWString SubStr(int start, int len = -1) const;
		CWString Mid(int start, int len = -1) const;
		CWString Left(int len) const;
		CWString Right(int len) const;

		int StrCmp(const char *str, int size=0) const;
		int StrCmp(const wchar_t *str, int size=0) const;
		int StrCmp(const CString &str, int size=0) const;
		int StrCmp(const CWString &str, int size=0) const;
		int StrCaseCmp(const char *str, int size=0) const;
		int StrCaseCmp(const wchar_t *str, int size=0) const;
		int StrCaseCmp(const CString &str, int size=0) const;
		int StrCaseCmp(const CWString &str, int size=0) const;

		int Instr(const char *str, int pos=0) const;
		int Instr(const wchar_t *str, int pos=0) const;
		int Instr(const CWString &str, int pos=0) const;
		int Instr(const CString &str, int pos=0) const;


		void Shr(wchar_t c, int size);
		void Shl(wchar_t c, int size);

		CWString& Repeat(int num);
		CWString& Repeat(const char *str, int num);
		CWString& Repeat(const wchar_t *str, int num);
		CWString& Repeat(const wchar_t ascii, int num);
		CWString& Repeat(const CWString& str, int num);
		CWString& Repeat(const CString& str, int num);

		int Find(const char* str, int pos) const;
		int Find(const CWString &search, int pos) const;
		CWString& Replace(const char* str, const char* byStr);
		CWString& Replace(const CWString &str, const CWString &byStr);
		CWString& ReplaceLetterList(const CWString &letters, wchar_t replacement);

		int ToInt() const;
		ppluint32 ToUInt() const;
		pplint64 ToInt64() const;
		ppluint64 ToUInt64() const;
		bool ToBool() const;
		long ToLong() const;
		long long ToLongLong() const;
		float ToFloat() const;
		double ToDouble() const;
		CString ToCString() const;
		CBinary ToCBinary() const;

		int IsTrue() const;
		int IsFalse() const;

		const void *GetBuffer() const;
		const char *GetPtr();
		const wchar_t *GetWPtr() const;
		wchar_t	GetChar(int pos) const;

		operator const char *();
		operator const wchar_t*() const;

		wchar_t operator[](int pos);
		CWString& operator=(char* str);
		CWString& operator=(const char* str);
		CWString& operator=(const CWString& str);
		CWString& operator=(const CWString* str);
		CWString& operator=(const CString& str);
		CWString& operator=(const CString* str);
		CWString& operator=(const CBinary& str);
		CWString& operator=(const wchar_t c);
		CWString& operator=(const wchar_t *str);

		CWString& operator+=(const char* str);
		CWString& operator+=(const wchar_t* str);
		CWString& operator+=(const wchar_t c);
		CWString& operator+=(const CWString& str);
		CWString& operator+=(const CString& str);

		bool operator<(const CWString &str) const;
		bool operator<=(const CWString &str) const;
		bool operator==(const CWString &str) const;
		bool operator!=(const CWString &str) const;
		bool operator>=(const CWString &str) const;
		bool operator>(const CWString &str) const;

		operator int() const;
		operator unsigned int() const;

		CWString GetMD5() const;
		int	MD5(CWString &str);
		int	MD5(const char *str, int size=-1);
		int	MD5();


		void PregEscape();
		int PregMatch(const char *expression, CArray *res=NULL);
		const char *GetMatch(int index) const;
		int PregReplace(const char *expression, const char *replace, int maxreplace=0);
		int PregReplace(const char *expression, CString &replace, int maxreplace=0);
		int PregReplace(const char *expression, CWString &replace, int maxreplace=0);

#ifdef WITH_QT
		//! @name Operatoren zur Verwendung mit QT
		//@{
		CWString(QString q) {
			Init();
			QByteArray a=q.toUtf8();
			Set((const char*)a);
		}
		CWString(QString *q) {
			Init();
			QByteArray a=q->toUtf8();
			Set((const char*)a);
		}
		operator const QString() {
			QString q=GetPtr();
			return q;
		}
		CWString& operator=(const QString& q) {
			QByteArray a=q.toUtf8();
			Set((const char*)a);
			return *this;
		}
		CWString& operator=(const QString *q) {
			QByteArray a=q->toUtf8();
			Set((const char*)a);
			return *this;
		}
		CWString& operator=(QString& q) {
			QByteArray a=q.toUtf8();
			Set((const char*)a);
			return *this;
		}
		CWString& operator=(QString *q) {
			QByteArray a=q->toUtf8();
			Set((const char*)a);
			return *this;
		}
		//@}
#endif


};

const CWString operator+(const CWString &str1, const CWString& str2);
const CWString operator+(const char *str1, const CWString& str2);
const CWString operator+(const CWString &str1, const char *str2);
const CWString operator+(const wchar_t *str1, const CWString& str2);
const CWString operator+(const CWString &str1, const wchar_t *str2);

std::ostream& operator<<(std::ostream& s, const CWString &str);



//! \brief Klasse für Binäre Daten
class CBinary : public CVar
{
	private:
		void		*data;
		void		*ptr;
		size_t		size;

		void	ClearInternal();
		void	InitInternal();
	public:
		CBinary();
		~CBinary();
		CBinary(CFileObject &file);
		CBinary(const CBinary &bin);
		CBinary(const CString &str);
		CBinary(const CWString &str);
		CBinary(const CMemoryReference &mem);
		CBinary(const char *str);

		void	Clear();
		void *	Malloc(size_t size);
		void *	Calloc(size_t size);

		int		Set(void *ptr, size_t size);
		int		Set(CFileObject &file);
		int		Set(const CVar& object);
		int		Set(const CMemoryReference &memory);
		int		Copy(const CMemoryReference &memory);
		int		Copy(const void *ptr, size_t size);
		int		Copy(const char *str);
		int		Copy(CFileObject& file);
		int		Copy(const CVar& bin);

		int		Concat(void *ptr, size_t size);
		CString	GetMD5Sum() const;
		int		CheckMD5(const CString &md5) const;
		int		Load(const CString &filename);
		int		Save(const CString &filename) const;
		int		Loadf(const char *filename, ...);
		int		Savef(const char *filename, ...) const;
		int		ToFile(CMemFile *File);
		void	ManageMemory();

		size_t	GetSize() const;
		size_t	Size() const;
		size_t	Len() const;
		void	*GetPtr() const;
		char	Get(size_t pos) const;
		void	HexDump() const;
		void	HexDump(CString &s) const;
		CString ToHex() const;
		int		FromHex(const CString &s);

		CBinary& operator=(CFileObject& file);
		CBinary& operator=(const CBinary& bin);
		CBinary& operator=(const CBinary* bin);
		CBinary& operator=(const CString& str);
		CBinary& operator=(const CMemoryReference& mem);
		CBinary& operator=(const CWString& str);
		CBinary& operator=(const char *str);
		char operator[](size_t pos) const;
		operator CMemoryReference() const;

		int memcmp(const CBinary &other) const;
		bool operator<(const CBinary &other) const;
		bool operator<=(const CBinary &other) const;
		bool operator==(const CBinary &other) const;
		bool operator!=(const CBinary &other) const;
		bool operator>=(const CBinary &other) const;
		bool operator>(const CBinary &other) const;

};


class CDateTime : public CVar
{
	private:
		ppluint32 us;
		ppluint16 yy;
		ppluint8 mm;
		ppluint8 dd;
		ppluint8 hh;
		ppluint8 ii;
		ppluint8 ss;

	public:
		CDateTime();
		CDateTime(const CString &datetime);
		CDateTime(const CDateTime &other);
		CDateTime(ppluint64 t);

		void setTime_t(ppluint64 t);
		void setLongInt(ppluint64 i);
		int set(const CString &datetime);
		void set(const CDateTime &other);
		int set(const CString &date, const CString &time);
		int setDate(const CString &date);
		int setTime(const CString &time);
		void set(int year, int month, int day, int hour=0, int minute=0, int sec=0, int msec=0, int usec=0);
		void setCurrentTime();
		void print() const;
		void clear();
		bool notEmpty() const;
		bool isEmpty() const;
		bool isLeapYear() const;

		CString get(const CString &format="%Y-%m-%d %H:%M:%S") const;
		CString getDate(const CString &format="%Y-%m-%d") const;
		CString getTime(const CString &format="%H:%M:%S") const;
		CString getISO8601() const;
		CString getISO8601withMsec() const;
		CString getISO8601withUsec() const;
		ppluint64 time_t() const;
		ppluint64 longInt() const;

		int year() const;
		int month() const;
		int day() const;
		int hour() const;
		int minute() const;
		int second() const;
		int millisecond() const;
		int microsecond() const;
		int week() const;
		int weekISO8601() const;

		pplint64 diffSeconds(const CDateTime &other) const;
		int compareSeconds(const CDateTime &other, int tolerance=0) const;

		CDateTime& operator=(const CString &datetime);
		CDateTime& operator=(const CDateTime &other);


		static bool isLeapYear(int year);
		static CDateTime currentTime();

		operator CString() const;

		bool operator<(const CDateTime &other) const;
		bool operator<=(const CDateTime &other) const;
		bool operator==(const CDateTime &other) const;
		bool operator!=(const CDateTime &other) const;
		bool operator>=(const CDateTime &other) const;
		bool operator>(const CDateTime &other) const;

};

std::ostream& operator<<(std::ostream& s, const CDateTime &dt);


//! \brief Synchronisation von Threads
class CMutex
{
	private:
		void *handle;

	public:
		CMutex();
		~CMutex();
		int Lock();
		int Unlock();
		int TryLock();
		int Wait(int milliseconds=0);
		int Signal();
};

extern CMutex	GlobalMutex;
/*
 * Error-Funktionen
 * CpplError.cpp
 */

//! \brief Struktur zum Speichern von Fehlern
typedef struct {
	pplint32	err;
	pplint32	suberr;
	pplint32	flags;
	CString 	ErrorText;
} PPL_ERROR;

int		TranslateErrno(int e);
int		TranslateSocketError();
int		TranslateGaiError(int e);
void	SetSocketError();
void	SetError(ppluint32 errorcode);
void	SetError(ppluint32 errorcode, CString &msg);
void	ExtendError(ppluint32 errorcode);
void	SetError(ppluint32 errorcode, const char *msg, ...);
void	SetErrorFromErrno(const char *msg=NULL, ...);
void	SetError(ppluint32 errorcode, ppldd suberrorcode, const char *msg=NULL, ...);
const pplchar *GetError();
const pplchar *GetError(pplint32 code);
const pplchar *GetExtendedError();
void		Error2String(CString &str);
CString		Error2String();
CString		Error();
pplint32	GetErrorCode();
pplint32	GetSubError();
PPL_ERROR *StoreError();
int		StoreError(PPL_ERROR *err);
void	RestoreError(PPL_ERROR *err, bool clear=false);
void	ClearError(PPL_ERROR *err);
//void	CopyError(CpplError *err);
void	PrintError();
pplint32	AttachErrorHandler(const char *geterro(pplint32 code),pplint32 first, pplint32 last);

int DisplayError();
void PushError();
void PopError();



//! \brief strtok-Klasse
class CTok
{
	private:
		CArray	*token;
		int			ptr;
	public:
		CTok();
		CTok(const CString &string, const char *suchstring);
		CTok(const char *string, const char *suchstring);
		~CTok();
		int Split(const char *string, const char *suchstring);
		int Split(const CString &string, const char *suchstring);
		void Reset();
		const char *GetFirst(const CString &string, const char *suchstring);
		const char *GetFirst(const char *string, const char *suchstring);
		const char *GetFirst();
		const char *GetNext();
		const char *GetLast(const CString &string, const char *suchstring);
		const char *GetLast(const char *string, const char *suchstring);
		const char *GetLast();
		const char *GetPrevious();
		const char *Get(int index);
		CString GetRest(int index, const char *trenn=" ");
		CString GetRest(const char *trenn=" ");
		int	GetRest(CString &rest, int index, const char *trenn=" ");
		int	GetRest(CString &rest, const char *trenn=" ");
		void Clear();
		void List() const;
		const char *operator[](int index);
		int Num() const;
};

namespace FILEMODE {
	enum {
		READ	= 1,
		WRITE	= 2,
		APPEND	= 3
	};
}



// cpu.cpp
namespace CPUCAPS {
	enum {
		NONE			= 0x00000000,
		HAVE_CPUID		= 0x00000001,
		HAVE_MMX		= 0x00000002,
		HAVE_MMX2		= 0x00000004,
		HAVE_3DNow		= 0x00000008,
		HAVE_3DNow2		= 0x00000010,
		HAVE_SSE		= 0x00000020,
		HAVE_SSE2		= 0x00000040,
		HAVE_AMD64		= 0x00000080
	};
}

typedef struct {
	ppluint32	caps;							// Struktur kann um weitere Informationen erweitert werden
	ppluint32	bits;

} CPUCaps;

ppluint32 GetCPUCaps (CPUCaps *cpu);				// Wenn cpu=NULL ist, werden nur die Caps zur�ckgegeben


class CFileObject
{
	private:
		CString MyFilename;

	public:
		CFileObject();
		virtual ~CFileObject();

		void				SetFilenamef(const char *filename, ...);
		void				SetFilename(const CString &filename);
		const char *		GetFilename() const;
		CString				Filename() const;

		size_t				Write (const void * source, size_t bytes, ppluint64 fileposition);
		size_t				Write (const void * source, size_t bytes);
		size_t				Write (const CVar &object, size_t bytes=0);
		size_t				Read (void * target, size_t bytes, ppluint64 fileposition);
		size_t				Read (void * target, size_t bytes);
		size_t				Read (CVar &target, size_t bytes);
		ppluint64			Copy (CFileObject &quellfile, ppluint64 quelloffset, ppluint64 bytes, ppluint64 zieloffset);
		ppluint64			Copy (CFileObject &quellfile, ppluint64 bytes);
		int					Gets (CString &buffer, size_t num=1024);
		int					Getws (CWString &buffer, size_t num=1024);
		char *				Gets (char *buffer, size_t num=1024);
		wchar_t*			Getws (wchar_t *buffer, size_t num=1024);
		CString				Gets (size_t num=1024);
		CWString			Getws (size_t num=1024);
		int					Putsf (const char *fmt, ... );
		int					Puts (const CString &str);
		int					Putws (const CWString &str);
		pplint64			Lof() const;
		const char			*Map();
		char				*Load();
		int					Load(CVar &target);
		CString				MD5Sum();

		// Virtuelle Funktionen
		virtual int			Close ();
		virtual int			Seek (ppluint64 position);
		virtual	int			Fseek (ppluint64 offset, int origin);
		virtual pplint64	Ftell();
		virtual size_t		Fread(void * ptr, size_t size, size_t nmemb);
		virtual size_t		Fwrite(const void * ptr, size_t size, size_t nmemb);
		virtual ppluint64	DoCopy (CFileObject &quellfile, ppluint64 bytes);
		virtual char *		Fgets (char *buffer, size_t num=1024);
		virtual wchar_t*	Fgetws (wchar_t *buffer, size_t num=1024);
		virtual	int			Putc (int c);
		virtual	int			Getc();
		virtual	int			Putwc (wchar_t c);
		virtual	int			Getwc();
		virtual int			Puts (const char *str);
		virtual int			Putws (const wchar_t *str);
		virtual bool		Eof() const;
		virtual pplint64	Size() const;
		virtual const char	*Map(ppluint64 position, size_t size);
		virtual char		*MapRW(ppluint64 position, size_t size);
		virtual	void		Unmap();
		virtual void		SetMapReadAhead(size_t bytes);
		virtual int			GetFileNo() const;
		virtual int			Flush();
		virtual int			Sync();
		virtual int			Truncate(ppluint64 length);
		virtual bool		IsOpen() const;
		virtual int			LockShared(bool block=true);
		virtual int			LockExclusive(bool block=true);
		virtual int			Unlock();
};

class CFile : public CFileObject
{
	private:
		void * ff;
		char * MapBase;
		ppluint64	LastMapStart;
		ppluint64	LastMapSize;
		int			LastMapProtection;
		ppluint32	ReadAhead;
		char * buffer;
		ppluint64	size;
		ppluint64	pos;
		bool isPopen;

		int munmap(void *addr, size_t len);
		void *mmap(ppluint64 position, size_t size, int prot, int flags);


	public:
		CFile ();
		CFile (const CString &filename, const char * mode="rb");
		CFile (const char * filename, const char * mode="rb");
		CFile (FILE * handle);
		virtual ~CFile();

		int			Open (const CString &filename, const char * mode="rb");
		int			Openf (const char * format, const char * mode, ...);
		int			Open (FILE * handle);
		int			OpenTempf(const char *filetemplate, ...);
		int			OpenTemp(const CString &filetemplate);
		int			Popenf(const char *command, const char *mode, ...);
		int			Popen(const CString &command, const CString &mode);
		int			Delete();

		// Virtuelle Funktionen
		virtual int			Close ();
		virtual int			Seek (ppluint64 position);
		virtual	int			Fseek (ppluint64 offset, int origin);
		virtual pplint64	Ftell();
		virtual size_t		Fread(void * ptr, size_t size, size_t nmemb);
		virtual size_t		Fwrite(const void * ptr, size_t size, size_t nmemb);
		virtual ppluint64	DoCopy (CFileObject &quellfile, ppluint64 bytes);
		virtual char *		Fgets (char *buffer, size_t num=1024);
		virtual wchar_t*	Fgetws (wchar_t *buffer, size_t num=1024);
		virtual	int			Putc (int c);
		virtual	int			Getc();
		virtual	int			Putwc (wchar_t c);
		virtual	int			Getwc();
		virtual int			Puts (const char *str);
		virtual int			Putws (const wchar_t *str);
		virtual bool		Eof() const;
		virtual pplint64	Size() const;
		virtual const char	*Map(ppluint64 position, size_t size);
		virtual char		*MapRW(ppluint64 position, size_t size);
		virtual	void		Unmap();
		virtual int			GetFileNo() const;
		virtual void		SetMapReadAhead(size_t bytes);
		virtual int			Flush();
		virtual int			Sync();
		virtual int			Truncate(ppluint64 length);
		virtual bool		IsOpen() const;
		virtual int			LockShared(bool block=true);
		virtual int			LockExclusive(bool block=true);
		virtual int			Unlock();


		// Static Functions
		static int Truncate(const CString &filename, ppluint64 bytes);
		static int Exists(const CString &filename);
		static int Existsf(const char *fmt, ...);
		static int CopyFile(const CString &oldfile, const CString &newfile, size_t buffersize=0);
		static int MoveFile(const CString &oldfile, const CString &newfile);
		static int LoadFile(CVar &object, const CString &filename);
		static void *LoadFile(const CString &filename, size_t *size=NULL);
		static int DeleteFile(const CString &filename);
		static int DeleteFilef(const char *filename, ...);
		static int TouchFile(const CString &filename);
		static int TouchFilef(const char *filename, ...);
		static int WriteFilef(const void *content, size_t size, const char *filename, ...);
		static int WriteFile(const void *content, size_t size, const CString &filename);
		static int WriteFilef(const CVar &object, const char *filename, ...);
		static int WriteFile(const CVar &object, const CString &filename);
		static int RenameFile(const CString &oldfile, const CString &newfile);
		static int FileAttrf(int attr, const char *filename, ...);
		static int FileAttr(int attr, const CString &filename);
		static int Chmod(const CString &filename, int attr);
		static int Stat(const CString &filename, CDirEntry &result);
		static CString MD5(const CString &filename);
};


class CMemFile : public CFileObject
{
	private:
		char * buffer;
		ppluint64	size;
		ppluint64	pos;

		char * MemBase;

	public:

		CMemFile ();
		CMemFile (void * adresse, size_t size);
		CMemFile (const CMemoryReference &memory);
		~CMemFile();

		int			Open(void * adresse, ppluint32 size);
		int			Open(const CMemoryReference &memory);
		char		*Adr(ppluint32 adresse);

		// Virtuelle Funktionen
		virtual int			Close ();
		virtual int			Seek (ppluint64 position);
		virtual	int			Fseek (ppluint64 offset, int origin);
		virtual pplint64	Ftell();
		virtual ppluint64	DoCopy (CFileObject &quellfile, ppluint64 bytes);
		virtual size_t		Fread(void * ptr, size_t size, size_t nmemb);
		virtual size_t		Fwrite(const void * ptr, size_t size, size_t nmemb);
		virtual char *		Fgets (char *buffer, size_t num=1024);
		//virtual wchar_t*	Getws (wchar_t *buffer, size_t num=1024);
		virtual	int			Putc (int c);
		virtual	int			Getc();
		//virtual	int		Putwc (wchar_t c);
		//virtual	int		Getwc();
		virtual int			Puts (const char *str);
		//virtual int			Putws (const wchar_t *str);
		virtual bool		Eof() const;
		virtual pplint64	Size() const;
		virtual const char	*Map(ppluint64 position, size_t size);
		virtual char		*MapRW(ppluint64 position, size_t size);
		virtual	void		Unmap();
		//virtual void		SetMapReadAhead(size_t bytes);
		//virtual int			GetFileNo() const;
		virtual int			Flush();
		virtual int			Sync();
		//virtual int			Truncate(ppluint64 length);
		virtual bool		IsOpen() const;
		//virtual int			LockShared(bool block=true);
		//virtual int			LockExclusive(bool block=true);
		//virtual int			Unlock();


};

// compression.cpp

class CCompression
{
	public:
		enum Algorithm {
			Algo_NONE=0,
			Algo_ZLIB,
			Algo_BZIP2,
			Unknown=256
		};

		enum Level {
			Level_Fast=0,
			Level_Normal,
			Level_Default,
			Level_High
		};

		enum Prefix {
			Prefix_None=0,
			Prefix_V1,
			Prefix_V2,
		};
	private:
		void *buffer;
		void *uncbuffer;
		Algorithm aaa;
		Level lll;
		Prefix prefix;

		int DoNone(void *dst, size_t *dstlen, const void *src, size_t size);
		int DoZlib(void *dst, size_t *dstlen, const void *src, size_t size);
		int DoBzip2(void *dst, size_t *dstlen, const void *src, size_t size);

		int UnNone (void *dst, size_t *dstlen, const void *src, size_t srclen);
		int UnZlib (void *dst, size_t *dstlen, const void *src, size_t srclen);
		int UnBzip2 (void *dst, size_t *dstlen, const void *src, size_t srclen);


	public:

		CCompression();
		CCompression(Algorithm method, Level level);
		~CCompression();
		int Init(Algorithm method, Level level);
		void UsePrefix(Prefix prefix);

		int Compress(CBinary &out, const void *ptr, size_t size, bool copy=true);
		int Compress(CBinary &out, const CVar &object, bool copy=true);
		int Compress(CMemory &out, const CMemoryReference &in);
		int Compress(void *dst, size_t *dstlen, const void *src, size_t size);
		int Uncompress(CBinary &out, const CBinary &data, bool copy=true);
		int Uncompress(CMemory &out, const CMemoryReference &in);
		int Uncompress(CBinary &out, const void *data, size_t size=0, bool copy=true);
		int Uncompress(void *dst, size_t *dstlen, const void *src, size_t srclen, Algorithm method=Unknown);
};

int Compress(CBinary &out, const CVar &in, CCompression::Algorithm method, CCompression::Level level=CCompression::Level_Default);
int Compress(CBinary &out, const void *buffer, size_t size, CCompression::Algorithm method, CCompression::Level level=CCompression::Level_Default);
int Compress(CMemory &out, const CMemoryReference &in, CCompression::Algorithm method, CCompression::Level level=CCompression::Level_Default);
int CompressZlib(CBinary &out, const CVar &in, CCompression::Level level=CCompression::Level_High);
int CompressZlib(CBinary &out, const void *buffer, size_t size, CCompression::Level level=CCompression::Level_High);
int CompressZlib(CMemory &out, const CMemoryReference &in, CCompression::Level level=CCompression::Level_Default);
int CompressBZip2(CBinary &out, const CVar &in, CCompression::Level level=CCompression::Level_High);
int CompressBZip2(CBinary &out, const void *buffer, size_t size, CCompression::Level level=CCompression::Level_High);
int CompressBZip2(CMemory &out, const CMemoryReference &in, CCompression::Level level=CCompression::Level_Default);
int Uncompress(CBinary &out, const CBinary &in);
int Uncompress(CBinary &out, const void *buffer, size_t size);
int Uncompress(CMemory &out, const CMemoryReference &in);


//! \brief Signals
class CSignal
{
	private:
	public:
		CSignal();
		virtual ~CSignal();
		int CatchSignal(int sig);
		int IgnoreSignal(int sig);
		int ClearSignal(int sig);

		virtual void Signal(int sig);
};

// Threads/Multitasking

//! \brief Thread-Klassen
namespace THREAD_CLASS {
	enum {
		IDLE=1,
		NORMAL,
		HIGH,
		REALTIME
	};
}

//! \brief Thread-Priorität
namespace THREAD_PRIORITY {
	enum {
		UNKNOWN=0,
		LOWEST,
		BELOW_NORMAL,
		NORMAL,
		ABOVE_NORMAL,
		HIGHEST
	};
}

int ThreadSetPriority(int priority);
int ThreadGetPriority();

ppluint64	StartThread(void (*start_routine)(void *),void *data=NULL);

void CleanupThreadData();

//! \brief Thread-Klasse
class CThread
{
	private:
		CMutex	threadmutex;
		ppluint64	threadid;
		void *threaddata, *param;
		int flags;
		int IsRunning;
		int IsSuspended;
		int deleteMe;
		int myPriority;

	public:
		CThread();
		virtual ~CThread();
		void ThreadSuspend();
		void ThreadResume();
		void ThreadStop();
		void ThreadSignalStop();
		int  ThreadStart(void *param=NULL);
		void ThreadStartUp();
		int	 ThreadIsRunning();
		int  ThreadIsSuspended();
		int  ThreadGetFlags();
		ppluint64  ThreadGetID();
		int  ThreadShouldStop();
		void ThreadWaitSuspended(int msec=0);
		void ThreadDeleteOnExit(int flag=1);
		int  ThreadShouldDeleteOnExit();
		int ThreadSetPriority(int priority);
		int ThreadGetPriority();
		void ThreadIdle();
		int ThreadSetStackSize(size_t size=0);
		size_t ThreadGetStackSize();
		size_t ThreadGetMinimumStackSize();
		virtual void ThreadMain(void *param);
};

ppluint64 GetThreadID();
PPL_ERROR *GetErrorThreadPtr();

//! \brief Verwaltung von mehreren Threads
class CThreadPool
{
	private:
		CMutex mutex;
		void *first, *last;
		void *walk;
		int count;
	public:
		CThreadPool();
		~CThreadPool();
		void Clear();
		int AddThread(CThread *thread);
		void DeleteThread(CThread *thread);
		void Suspend();
		void Resume();
		void Stop();
		void SignalStop();
		int  Count();
		void DestroyThreads();
		int Lock();
		int Unlock();
		void Reset();
		void ChangePriority(int prio);
		CThread *GetFirst();
		CThread *GetNext();

};


//! \brief PPL-Archiv-File
class CArchiv
{
	private:
		CFileObject	*ff;
		void		*first_chunk, *last_chunk;
		ppluint64	size_uncompressed;
		ppluint64	size_compressed;
		ppluint64	nextptr;
		ppluint32	filecount;


		int			WriteHeader();
		int			ReadHeader();
		void		*FindChunk(int chunkid, char *filename=NULL);
		int			SaveChunk(void *data);
		int			ParseFile();
		int			SetText(char *text, int chunkid);

	public:
		CArchiv();
		~CArchiv();

		int			Open(char *filename);
		int			Open(CFileObject *file);
					// Open oeffnet ein bereits vorhandenes File. Existiert dieses nicht,
					// wird ein Fehler zur�ckgegeben
		int			Create(char *filename);
					// Create erstellt ein neues Archiv. Ist die Datei bereits vorhanden,
					// wird sie �berschrieben!
		int			Close();
		int			Add(char *filename, bool includepath, int compression, int level);
		int			Add(CFileObject *file, char *filename, bool includepath, int compression, int level);
		int			Add(void *adr, ppluint32 size, char *filename, bool includepath, int compression, int level);
		int			SetName(char *name);
		int			SetAutor(char *autor);
		int			SetDescription(char *descr);
		int			Delete(char *filename);
		int			Cleanup();
					// Mit Cleanup werden als gel�scht gekennzeichnete Dateien
					// endg�ltig entfernt
		CFileObject	*Load(char *filename, bool uncompress=true);
					// Laed die gew�nschte Datei in den Hauptspeicher

};

//! \brief Log-Handler
class CLogHandler
{
	public:
		virtual ~CLogHandler() {}
		virtual void LogMessage(int facility, int level, const char *msg);
};

#define NUMFACILITIES	9
#ifdef ERROR
#undef ERROR
#endif

namespace LOG {
	enum {
		EMERG		= 1,
		ALERT		= 2,
		CRIT		= 3,
		ERR			= 4,
		ERROR		= 4,
		WARNING		= 5,
		NOTICE		= 6,
		INFO		= 7,
		DEBUG		= 8
	};
}

//! \brief Allgemeine Logging-Klasse
class CLog
{
	public:
		enum SYSLOG_FACILITY {
			SYSLOG_AUTH=1,
			SYSLOG_AUTHPRIV,
			SYSLOG_CONSOLE,
			SYSLOG_CRON,
			SYSLOG_DAEMON,
			SYSLOG_FTP,
			SYSLOG_KERN,
			SYSLOG_LPR,
			SYSLOG_MAIL,
			SYSLOG_NEWS,
			SYSLOG_NTP,
			SYSLOG_SECURITY,
			SYSLOG_SYSLOG,
			SYSLOG_USER,
			SYSLOG_UUCP,
			SYSLOG_LOCAL0,
			SYSLOG_LOCAL1,
			SYSLOG_LOCAL2,
			SYSLOG_LOCAL3,
			SYSLOG_LOCAL4,
			SYSLOG_LOCAL5,
			SYSLOG_LOCAL6,
			SYSLOG_LOCAL7
		};
	private:
		CMutex	mutex;
		CAssocArray	*FilterModule, *FilterFile;
		int			debuglevel[NUMFACILITIES];
		bool		console_enabled;
		int			console_facility;
		int			console_level;
		CFile		logff[NUMFACILITIES];
		CString		logfilename[NUMFACILITIES];
		void		*firsthandler, *lasthandler;
		bool		logconsole;
		bool		logThreadId;
		int			rotate_mechanism;
		ppluint64	maxsize;
		int			generations;
		bool		inrotate;
		bool			useSyslog;
		SYSLOG_FACILITY	syslogFacility;
		CString			syslogIdent;


		bool		shouldPrint(const char *module, const char *function, const char *file, int line, int facility, int level);
		int			IsFiltered(const char *module, const char *function, const char *file, int line, int level);
		void		Output(int facility, int level, const char *module, const char *function, const char *file, int line, const char *buffer, bool printdate=true);
		void		OutputArray(int facility, int level, const char *module, const char *function, const char *file, int line, const CAssocArray *a, const char *prefix, CString *Out=NULL);
		void		CheckRotate(int facility);

	public:
		CLog();
		virtual ~CLog();
		void Terminate();
		int	AddLogHandler(CLogHandler *handler);
		int DeleteLogHandler(CLogHandler *handler);
		int SetLogfile(int facility, const char *filename);
		void SetLogLevel(int facility, int level=1);
		int  GetLogLevel(int facility);
		int		SetLogRotate(ppluint64 maxsize, int generations);
		void SetConsole(bool flag=true, int facility=LOG::DEBUG, int level=1);
		void	openSyslog(const CString &ident, SYSLOG_FACILITY facility=SYSLOG_USER);
		void	closeSyslog();
		void Print (int facility, int level, const char *text);
		void Print (int facility, int level, const char *file, int line, const char *text);
		void Print (int facility, int level, const char *module, const char *function, const char *file, int line, const char *text);
		void Print (int level, const char *text);
		void PrintArray (int facility, int level, const CAssocArray *a, const char *fmt, ...);
		void PrintArray (int facility, int level, const char *module, const char *function, const char *file, int line, const CAssocArray *a, const char *fmt, ...);
		void PrintArraySingleLine (int facility, int level, const char *module, const char *function, const char *file, int line, const CAssocArray *a, const char *fmt, ...);
		void Printfs (int level, const char *fmt, ... );
		void Printfs (int facility, int level, const char *fmt, ... );
		void Printf (int facility, int level, const char *file, int line, const char *fmt, ... );
		void Printf (int facility, int level, const char *module, const char *function, const char *file, int line, const char *fmt, ... );
		void HexDump (int facility, int level, const void * address, int bytes);
		void HexDump (const void * address, int bytes);
		void LogErrorCode (int level, ppluint32 errorcode);
		void LogErrorCode (ppluint32 errorcode);
		void LogError (int level);
		void LogError ();
		void LogError(const char *file, int line);
		void LogError(const char *module, const char *function, const char *file, int line);
		void LogError(int facility, int level, const char *file, int line);
		void LogError(int facility, int level, const char *module, const char *function, const char *file, int line);
		int SetFilter(const char *module, const char *function, int level);
		int SetFilter(const char *file, int line, int level);
		void DeleteFilter(const char *module, const char *function);
		void DeleteFilter(const char *file, int line);


};

//! \brief Debug-Klasse
class CDebug : public CLog
{
	private:

	public:
		CDebug();
		~CDebug();

		void SetLogfile(const char *filename);
		void SetDebugLevel(int level);
		int  GetDebugLevel();
		void Log (const char *text);
		void Log (int level, const char *text);
		void Logf (const char *fmt, ... );
		void Logf (int level, const char *fmt, ... );
		void Print (const char *text);
		void Print (int level, const char *text);
		void Printf (const char *fmt, ... );
		void Printf (int level, const char *fmt, ... );

		void SetConsole(bool flag=true);
};

//! \brief PPL-Core-Klasse
class Cppl6Core
{
	private:
		CMutex mutex;

	public:
		Cppl6Core();
		~Cppl6Core();
		ppluint32 GetCPUCaps(CPUCaps *cpu=NULL);
		const CPUCaps *GetCPUCaps();
		CString GetCopyright();
		CString GetVersion();
		void GetVersion(int *major, int *minor, int *build);
		int MinVersion (int major, int minor, int build);
		//ppluint32 GetReleaseDate();
		CString GetReleaseDate();
		ppluint32 GetReleaseDateInt();

		CResource *GetPPLResource();

		CHeap *GetHeap_CString();
		CHeap *GetHeap_CWString();
		CHeap *GetHeap_CArrayItem();
		CHeap *GetHeap_TREEITEMS();

		static int haveMCrypt();
		static int haveMHash();
		static int haveIconv();
		static int haveZlib();
		static int haveBzip2();
		static int havePNG();
		static int haveJPEG();
		static int haveFreetype2();
		static int haveSDL();
		static int haveLame();
		static int haveLibmad();
		static int havePCRE();
		static int haveMySQL();
		static int haveSybase();
		static int havePostgres();
		static int haveThreads();
		static int haveCurl();
		static int haveOpenSSL();
		static int haveDirectX9();
		static void GetFeatures(CAssocArray &list);


		//void EnableMemoryDebug(bool flag);
		//void MemoryLog();
		//void MemoryDump();
		int AtExit(int (*PPLExitFunction)(void *data), void *data);
};

Cppl6Core *PPLInit();
//Cppl6Core *PPLInit(bool MemoryDebug);

void PPLExit();

} // EOF namespace ppl6

#ifndef _IS_PPL6_CORE
extern ppl6::Cppl6Core *PPLCore;
#endif

#ifdef DEBUGLOG
    #define DLOG                if(PPLCore) PPLCore->Printf
    #define DLOGLEVEL(value)    if(PPLCore) PPLCore->Printf
    #define RETURN              { if(PPLCore) PPLCore->Printf("==> RETURN"); return;}
    #define RETURN_VALUE(value) { if(PPLCore) PPLCore->Printf("==> RETURN (%u)",value); return value;}
    #define RETVAL(value)       { if(PPLCore) PPLCore->Printf("==> RETURN (%u)",value); return value;}
    #define RETVALERR(value,err){ if(PPLCore) PPLCore->Printf("==> RETURN (%u), err: %u = %s",value,err,ppl6::errortext(err)); return value;}
#else
    #define DLOG                if(!1) printf
    #define DLOGLEVEL(value)    if(!1) printf
    #define RETURN              return;
    #define RETURN_VALUE(value) return (value);
    #define RETVAL(value)       return (value);
    #define RETVALERR(value,err)    return (value);
#endif

namespace ppl6 {

#define PPLDebug PPLCore

//! \brief Notify-Handler
class CNotifyHandler
{
	private:
		void (*NotifyHandler)(void *object, void *data);
		void *NotifyObject;


	public:
		CNotifyHandler();
		~CNotifyHandler();

		void SetHandler(void (*NotifyHandler)(void *object, void *data),void *object=NULL);
		void Notify(void *data=NULL);
		void Unset();
};

//! \brief Callback-Handler
class CCallback
{
	private:
		void *callbackdata;
	public:
		CCallback();
		virtual ~CCallback();
		void SetData(void *data=NULL);
		virtual void Callback(void *data);
};


class CHeap
{
	private:
		CMutex	mutex;
		void		*blocks;
		int			elementsize, increase, inccount;
		bool		IsInit;
		size_t		mem_allocated;
		size_t		mem_used;

		int			Increase(int num);
		void		Cleanup();

	public:
		CString	Name;
		CHeap();
		CHeap(size_t elementsize, int startnum, int increase, CString name=CString());
		~CHeap();
		void Clear();
		int Init(size_t elementsize, int startnum, int increase);
		void *Malloc();
		void *Calloc();
		int Free(void *element);
		void Dump();
};

class CMemSpace
{
	private:
		void *first, *last;
		ppluint32 size,blocks;
	public:
		CMemSpace();
		~CMemSpace();

		void Clear();
		void *Malloc(ppldd size, bool clear=false);
		void *Calloc(ppldd size);
		void Free(void *adr);
		char *StrDup(const char *string);

		ppluint32 GetNumBlocks();
		ppluint32 GetUsedMem();
};

class CMemMan
{
	private:
		//CMutex	mutex;
		void		*first, *last;
		ppluint32	growth;
		ppluint64	size, size_used, size_free;
		ppluint32	sizeof_memslot;
		ppluint32	sizeof_memman;
		ppluint32	sig;

		void *AllocateSlot(void *block, void *slot, ppldd size, bool clear);
		void FindBiggestSlot(void *block);
		ppluint32 CRC(void *slot);

	public:
		CMemMan();
		~CMemMan();
		void SetDefaultGrow(ppluint32 bytes);
		//
		void Clear();
		void *Malloc(ppldd size, bool clear=false);
		void *Calloc(ppldd size);
		void *Calloc(size_t nmemb, size_t size);
		char *Strdup(const char *str);
		char *Strndup(const char *str, size_t size);
		void Free(void *adr);

		ppluint64 GetUsedMem();
		ppluint64 GetFreeMem();
};

// ***************************************************************************
// AVL-Trees
// ***************************************************************************
#ifndef AVL_MAX_HEIGHT
#define AVL_MAX_HEIGHT 32
#endif

typedef struct tagTREEITEM {
	const void *data;
	struct tagTREEITEM *left, *right, *parent;
	signed char balance;
} TREEITEM;

class CTreeController
{
	public:
		virtual ~CTreeController() {};
		virtual int	Compare(const void *value1, const void *value2) const = 0;
		virtual int GetValue(const void *item, CString &buffer) const = 0;
		virtual int DestroyValue(void *item) const = 0;
};

class CAVLTree : public CTreeController
{
	public:
		class Walker
		{
			friend class CAVLTree;
			private:
				TREEITEM	*current;
				TREEITEM	*stack[AVL_MAX_HEIGHT];
				size_t		stack_height;
			public:
				Walker();
		};

	private:
		TREEITEM	*root;
		size_t		count;
		TREEITEM	*current;
		TREEITEM	*stack[AVL_MAX_HEIGHT];
		size_t		stack_height;
		CTreeController	*controller;
		bool		dupes;

		void		UpInsert(TREEITEM *node);
		void		UpDelete(TREEITEM *node);
		TREEITEM	*Rotate(TREEITEM *kn);
		void		SwapNodes(TREEITEM *item1, TREEITEM *item2);
		//void		ListNodes(CCallback *callback, TREEITEM *node);
		int			DeleteNode(TREEITEM *item);

	public:
		CAVLTree();
		virtual ~CAVLTree();
		virtual int	Compare(const void *value1, const void *value2) const;
		virtual int GetValue(const void *item, CString &buffer) const;
		virtual int DestroyValue(void *item) const;

		void		SetTreeController(CTreeController *c);
		void		AllowDupes(bool allow);
		int			Num() const;
		void		Clear();
		int			Add(const void *value);
		int			Delete(const void *value);
		int			Remove(const void *value);
		void		*Find(const void *value) const;
		TREEITEM	*FindNode(const void *value) const;
		void		*FindOrAdd(const void *item);
		void		Reset();
		void		*GetFirst();
		void		*GetNext();
		void		*GetLast();
		void		*GetPrevious();
		void		*GetCurrent();
		//int			ListNodes(CCallback *callback);
		void		PrintNodes(const TREEITEM *node=NULL) const;

		void		Reset(Walker &walk) const;
		void		*GetFirst(Walker &walk) const;
		void		*GetNext(Walker &walk) const;
		void		*GetLast(Walker &walk) const;
		void		*GetPrevious(Walker &walk) const;
		void		*GetCurrent(Walker &walk) const;

};

class CTree;

class CTreeItem
{
	friend class CTree;
	private:
		CTree		*tree;
		CTreeItem	*left;
		CTreeItem	*right;
		CTreeItem	*parent;
		signed char	balance;
	public:
		CTreeItem();
		virtual ~CTreeItem();
		virtual int CompareNode(CTreeItem *item);
		int				IsTrue(const char *key) const;
		int				IsFalse(const char *key) const;

		virtual int CompareValue(void *value);
		virtual int GetValue(CString *buffer);
		CTreeItem *Left();
		CTreeItem *Right();
		CTreeItem *Parent();
};

class CTreeWalker
{
	friend class CTree;
	private:
		CTreeItem	*current;
		CTreeItem	*stack[AVL_MAX_HEIGHT];
		size_t		stack_height;
	public:
		CTreeWalker();
};

class CTree
{
	private:
		CTreeItem	*root;
		size_t		count;
		CTreeItem	*current;
		CTreeItem	*stack[AVL_MAX_HEIGHT];
		size_t		stack_height;

		void UpInsert(CTreeItem *node);
		void UpDelete(CTreeItem *node);
		CTreeItem *Rotate(CTreeItem *kn);
		void SwapNodes(CTreeItem *item1, CTreeItem *item2);
		void ListNodes(CCallback *callback, CTreeItem *node) const;
		void Graphviz(CString &s, CTreeItem *node, int tiefe) const;

	public:
		CTree();
		virtual ~CTree();

		int			Num() const;
		void		Clear(bool deleteitems=false);
		int			Add(CTreeItem *item);
		int			Delete(CTreeItem *item);
		int			Delete(void *value);
		CTreeItem	*Find(void *value) const;
		CTreeItem	*Find(CTreeItem *item) const;
		CTreeItem	*FindOrAdd(CTreeItem *item);
		CTreeItem	*GetRootNode() const;
		void		Reset();
		CTreeItem	*GetFirst();
		CTreeItem	*GetNext();
		CTreeItem	*GetLast();
		CTreeItem	*GetPrevious();

		void		Reset(CTreeWalker &walk) const;
		CTreeItem	*GetFirst(CTreeWalker &walk) const;
		CTreeItem	*GetNext(CTreeWalker &walk) const;
		CTreeItem	*GetLast(CTreeWalker &walk) const;
		CTreeItem	*GetPrevious(CTreeWalker &walk) const;
		CTreeItem	*GetCurrent(CTreeWalker &walk) const;

		CTreeItem	*GetCurrent() const;
		int			ListNodes(CCallback *callback) const;
		void		PrintNodes(CTreeItem *node=NULL) const;
		int			Validate();
		void		Graphviz(CString &s,CTreeItem *node=NULL) const;

};


// Core-Functions

#define peek8 Peek8
#define peek16 Peek16
#define peek32 Peek32
#define peek64 Peek64
#define poke8 Poke8
#define poke16 Poke16
#define poke32 Poke32
#define poke64 Poke64

	void PrintDebug(const char *format, ...);
	void PrintDebugTime(const char *format, ...);
	void SetGlobalOutput(int type);

	pplchar * trim (pplchar * text);
	pplchar * trimchar (pplchar * text, ppluchar chr);


	void strxchg (char * str1, char *alt, char *neu);
	void strxchg (char * str1, const char *alt, const char *neu);
	void hex (char * ziel, ppldd ziffer,ppldd groesse);
	void dumphex (char *adresse,ppldd zeilen);
	char * getargv (int argc, char * argv[], const char * argument);
	CString GetPath(const CString &Path);
	CString GetPath(const char *path);
	CString GetFilename(const CString &Path);
	CString GetFilename(const char *path);
	CString FileSuffix(const CString &Filename);

	int countbyte (char * text, char *byte);
	char *strtolower (char * text);
	char *strtoupper (char * text);
	int instr (const char * string, const char * such, unsigned int start=0);
	int instrcase (const char * string, const char * such, unsigned int start=0);
	ppldd getstrr(char * buffer, char * adresse);
	int getstring (char * ziel, ppldd laenge, char * quelle);
	void pokeb (char *Adresse, ppldd Wert);
	void pokew (char *Adresse, ppldd Wert);
	void poked (char *Adresse, ppldd Wert);
	void Poke8 (char *Adresse, ppluint32 Wert);
	void Poke16 (char *Adresse, ppluint32 Wert);
	void Poke24 (char *Adresse, ppluint32 Wert);
	void Poke32 (char *Adresse, ppluint32 Wert);
	void Poke64 (char *Adresse, ppluint64 Wert);
	ppluint32 Peek8 (const char *Adresse);
	ppluint32 Peek16 (const char *Adresse);
	ppluint32 Peek24 (const char *Adresse);
	ppluint32 Peek32 (const char *Adresse);
	ppluint64 Peek64 (const char *Adresse);
	float	PeekFloat(const void *Adresse);
	void poked64 (char *Adresse, ppld64 Wert);
	void poket (char *Adresse, ppldd Wert);
	ppldd peekb (const char *Adresse);
	ppldd peekw (const char * Adresse);
	ppldd peekt (const char *Adresse);
	ppldd peekd (const char *Adresse);
	ppld64 peekd64 (const char *Adresse);
	void pokes (char * adresse, const char * string);
	void pokesz (char * adresse, const char * string);
	void pokesn (char * adresse, const char * string, long laenge);
	void pokesnz (char * adresse, const char * string, long laenge);
	// Network-Byte-Order
	void PokeN8 (char *Adresse, ppluint32 Wert);
	void PokeN16 (char *Adresse, ppluint32 Wert);
	void PokeN32 (char *Adresse, ppluint32 Wert);
	void PokeN64 (char *Adresse, ppluint64 Wert);
	void PokeFloat(void *Adresse, float Wert);
	ppluint32 PeekN8 (const char *Adresse);
	ppluint32 PeekN16 (const char *Adresse);
	ppluint32 PeekN32 (const char *Adresse);
	ppluint64 PeekN64 (const char *Adresse);

	int WritePFPHeader (char * adresse, PFPSTRUCT * pfp);
	int SavePFPHeader (CFileObject *ff, PFPSTRUCT *pfp);
	int PresetPFPHeader (PFPSTRUCT *pfp);
	char * resizebuffer(char *buffer, ppldd oldsize, ppldd newsize);
	int IsDir(const char *path);
	int IsFile(const char *file);
	int IsTrue(const char *value);
	int IsTrue(const char *value);
	int FileExists(const char *fmt, ...);
	void SetRect(RECT *r, int left, int top, int right, int bottom);
	void CopyRect(RECT *dest, RECT *src);
	int InsideRect(RECT *r, POINT *p);
	void HexDump(const void *address, ppldd bytes, bool skipheader);
	void HexDump(CString *s, const void *address, ppldd bytes, bool skipheader);
	void HexDump(const void *address, ppldd bytes);
	int MessageBox(int type, const char *caption, const char * fmt, ...);

	int MkDir(const char *path);					// Nicht rekursiv!
	int MkDir(const char *path, int recursive);		// recursive=0 oder 1
	int MkDir(const char *path, ppluint32 mode, int recursive);

	int StringToRect(CString *s, WINRECT *r);
	int StringToRect(CString *s, RECT *r);
	int RectToString(WINRECT *r, CString *s);
	int RectToString(RECT *r, CString *s);

// Strings
	CString Trim(const CString &str);
	CString Trim(const char *str);
	CString UCase(const CString &str);
	CString UCase(const char *str);
	CString LCase(const CString &str);
	CString LCase(const char *str);
	CWString Trim(const CWString &str);
	CWString UCase(const CWString &str);
	CWString LCase(const CWString &str);
	int StrCmp(const CString &s1, const CString &s2);
	int StrCmp(const CWString &s1, const CWString &s2);
	int StrCaseCmp(const CString &s1, const CString &s2);
	int StrCaseCmp(const CWString &s1, const CWString &s2);
	int Instr (const CString &string, const CString &such, size_t start=0);
	int InstrCase (const CString &string, const CString &such, size_t start=0);
	int Instr (const CWString &string, const CWString &such, size_t start=0);
	int InstrCase (const CWString &string, const CWString &such, size_t start=0);

	CString Left(const CString &str, size_t num);
	CString Right(const CString &str, size_t num);
	CString Mid(const CString &str, size_t start, size_t num=(size_t)-1);
	CString SubStr(const CString &str, size_t start, size_t num=(size_t)-1);
	CWString Left(const CWString &str, size_t num);
	CWString Right(const CWString &str, size_t num);
	CWString Mid(const CWString &str, size_t start, size_t num=(size_t)-1);
	CWString SubStr(const CWString &str, size_t start, size_t num=(size_t)-1);

	CString ToString(const char *fmt, ...);
	CString Replace(const CString &string, const CString &search, const CString &replace);
	int InWstr (const wchar_t * string, const wchar_t * such, unsigned int start);
	int IsTrue(const wchar_t *value);

	int PregMatch(const CString &expression, const CString &subject);
	int PregMatch(const CString &expression, const CString &subject, CArray &matches);

	CBinary Hex2Binary(const CString &hex);
	CMemory Hex2Memory(const CString &hex);
	CString ToHex(const CBinary &bin);
	CString ToHex(const CMemoryReference &bin);

	CBinary FromBase64(const CString &str);
	CString ToBase64(const CString &str);
	CString ToBase64(const CBinary &bin);
	CString EscapeHTMLTags(const CString &html);
	CString UnescapeHTMLTags(const CString &html);

	CString UrlDecode(const ppl6::CString &text);
	CString UrlEncode(const ppl6::CString &text);

	CString XmlEscape(const CString &text);
	CString XmlUnEscape(const CString &text);

// compatibility functions

	const char *strcasestr(const char *haystack, const char *needle);
	int strcasecmp(const char *s1, const char *s2);
	int strncasecmp(const char *s1, const char *s2, size_t n);
	int vasprintf(char **buff, const char *fmt, va_list args);
	int asprintf(char **buff, const char *format, ...);
	long htol (const char * wert);
	long long atoll (const char * wert);
	int atoi(const char *wert);
	long atol(const char *wert);
	double atof(const char *wert);
	char *strndup(const char *str, size_t len);
	int strncmp(const char *s1, const char *s2, size_t len);
	size_t strnlen(const char *str, size_t len);
	void bcopy(const void *src, void *dst, size_t len);



// time.cpp
	ppluint64 GetTime(PPLTIME *t);
	ppluint64 GetTime(PPLTIME *t, ppluint64 tt);
	ppluint64 GetTime(PPLTIME &t, ppluint64 tt);
	ppluint64 GetTime();
	ppluint64 GetMilliSeconds();
	int datum (char *str1);
	ppldd longdatum ();
	int usdatum (char *str1);
	int zeit (char *str1);
	ppldd longzeit ();
	ppldd zeit2long (char * zeitstr);
	void long2zeit (char *zeitstr,ppldd z);
	ppldd sec2long (ppldd z);
	ppldd dat2long (pplchar *dat);
	void long2dat (pplchar *datstr,ppluint32 dat);
	void datumsauswertung (pplchar * d, pplchar * dat);
	ppluint64 Datum2Sekunden(char *datum, char *zeit);
	ppluint64 GetUTC(char *datum, char *zeit);
	const char * UTC2Date(ppluint64 sec, char *datum, char *uhrzeit);
	ppldd Sekunden2Datum(ppluint64 sec, ppldd *datum, ppldd *uhrzeit);
	double getmicrotime();
	double GetMicrotime();
	int USleep(ppluint64 microseconds);		// 1 sec = 1000000 microseconds
	int MSleep(ppluint64 milliseconds);		// 1 sec = 1000 milliseconds
	int SSleep(ppluint64 seconds);
	ppluint64 MkTime(const char *year, const char *month, const char *day, const char *hour=NULL, const char *min=NULL, const char *sec=NULL);
	ppluint64 MkTime(int year, int month, int day, int hour=0, int min=0, int sec=0);
	ppluint64 MkTime(CString *year, CString *month, CString *day, CString *hour, CString *min, CString *sec);
	ppluint64 MkTime(const CString &year, const CString &month, const CString &day, const CString &hour="0", const CString &min="0", const CString &sec="0");
	ppluint64 MkTime(const char *iso8601date, PPLTIME *t=NULL);
	ppluint64 MkTime(CString *iso8601date, PPLTIME *t=NULL);

	const char *MkISO8601Date (CString &buffer, ppluint64 sec);
	const char *MkISO8601Date (CString &buffer, PPLTIME *t=NULL);
	char *MkISO8601Date (char *buffer, size_t size, ppluint64 sec);
	char *MkISO8601Date (char *buffer, size_t size, PPLTIME *t=NULL);
	CString MkISO8601Date (ppluint64 sec);
	CString MkISO8601Date (PPLTIME *t=NULL);

	CString MkRFC822Date (PPLTIME &t);
	CString MkRFC822Date (ppluint64 sec=0);
	/*
	char *MkRFC822Date (char *buffer, size_t size_t, ppluint64 sec);
	const char *MkRFC822Date (CString &buffer, ppluint64 sec);
	const char *MkRFC822Date (CString &buffer, PPLTIME *t=NULL);
	*/

	const char *MkDate (CString &buffer, const char *format, ppluint64 sec);
	CString MkDate(const char *format, ppluint64 sec);
	CString Long2Date(const char *format, int value);
	char *MkDate (char *buffer, int size, const char *format, ppluint64 sec);



// random.cpp
	void	srand(ppldd seed);
	ppldd	rand(ppldd min, ppldd max);
	char * rand_initstate(unsigned long seed, char *arg_state, long n);
	char * rand_setstate(char *arg_state);
	CBinary Random(size_t bytes);
	CBinary &Random(CBinary &buffer, size_t bytes);


// md5.c
	/* MD5 context. */
	typedef struct MD5Context {
		ppldd state[4];   /* state (ABCD) */
		ppldd count[2];   /* number of bits, modulo 2^64 (lsb first) */
		unsigned char buffer[64]; /* input buffer */
	} MD5_CTX;
	void   MD5Init (MD5_CTX *);
	void   MD5Update (MD5_CTX *, const unsigned char *, unsigned int);
	void   MD5Pad (MD5_CTX *);
	void   MD5Final (unsigned char [16], MD5_CTX *);
	char * MD5End(MD5_CTX *, char *);
	char * MD5File(const char *, char *);
	char * MD5Data(const unsigned char *data, unsigned int size, char *buffer33);
	CString MD5(const CString &str);
	CString MD5(const CWString &str);
	CString MD5(const CBinary &bin);
	CString MD5(const void *data, size_t size);

// math.cpp
	pplint32	sinus1024 (pplint32 wert);
	pplint32	cosinus1024 (pplint32 wert);
	pplint32	lowest (pplint32 wert1, pplint32 wert2);
	pplint32	highest (pplint32 wert1, pplint32 wert2);
	pplint32	saturate(pplint32 wert, pplint32 min, pplint32 max);
	ppluint32	crc32(const char* text);
	ppluint32	crc32(const void* buffer, int size);
	int			Calc(const CString &str, double &result);
	int			Calc(const char *str, double &result);


// ident.cpp
	const char	*identstring (int id);
	int		is_pfp (const char * adresse, PFPSTRUCT *pfs);
	int		ident (const char * adresse, PFPSTRUCT * pfp);
	int		ident (CFileObject &file, PFPSTRUCT * pfp);


/*
 * einfaches nummerisches Array
 */

//! \brief Einfaches zweidimensionales Array mit Zahlen als Schlüssel
class CArray : public CVar
{
	private:
		ppldd num;
		ppldd maxnum;
		ppldd pos;
		void *rows;

	public:
		CArray();
		CArray(const CArray &other);
		CArray(const CString &str, const CString &delimiter=CString("\n"), size_t limit=0, bool skipemptylines=false);
		~CArray();

		void Clear();
		void Reserve (ppldd size);
		int Set(ppldd index, const char *value, int bytes=-1);
		int GetNumRows() const;
		int Copy(const CArray &a);
		int Copy(const CArray *a);
		int Copy(const CAssocArray &a);
		int Copy(const CAssocArray *a);
		int Num() const;
		size_t Size() const;
		int Add(const char *value, int bytes=-1);
		int Addf(const char *fmt, ...);
		int Add(int value);
		int Add(const CString &value, int bytes=-1);
		int Add(const CWString &value, int chars=-1);
		int Add(const CArray &a);
		int Explode(const char *text, const char *trenn, int limit=0, bool skipemptylines=false);
		int Explode(const CString &text, const char *trenn, int limit=0, bool skipemptylines=false);
			// Der String "text" wird anhand des Trennzeichens "trenn" aufgesplittet und in das
			// Array gespeichert. Ist "limit" gesetzt, besteht das Array anschliessend aus "limit"
			// Elementen, wobei das letzte Element den Rest des Strings enthaelt.
		CString Implode(const CString &trenn=CString("\n")) const;
		int Text2Array(const char *text, const char *trenn);
		int Text2Array(const CString &text, const char *trenn);
		int File2Array(CFileObject &ff, const char *trenn);
		int File2Array(const char *filename, const char *trenn);
		const char *Get(ppldd index) const;
		CString GetString(ppldd index) const;
		const char *GetRandom() const;
		void List() const;
		const char *operator[](ppldd index) const;
		CArray& operator=(const CArray &a);
		CArray& operator=(const CArray *a);
		CArray& operator=(const CAssocArray &a);
		CArray& operator=(const CAssocArray *a);
		void Reset();
		const char *GetFirst();
		const char *GetNext();
		const char *Shift();		// Holt das erste Element aus dem Array heraus
		const char *Pop();		// Holt das letzte Element aus dem Array heraus
		int  Push(const char *value, int bytes=-1);
		int  Unshift(const char *value, int bytes=-1);
		void Sort();
		void MakeUnique();
		CArray &fromArgs(int argc, const char **argv);
		CArray &fromArgs(const CString &args);
};

CArray *Explode(const char *text, const char *trenn, int limit=0, bool skipemptylines=false);
CArray *Explode(const CString &text, const char *trenn, int limit=0, bool skipemptylines=false);
CArray *Text2Array(const char *text, const char *trenn);
CArray *Text2Array(const CString &text, const char *trenn);
CArray *File2Array(const char *filename, const char *trenn);
CArray *File2Array(CFileObject &file, const char *trenn);

CArray Sort(const CArray &a);

#define PPL_ARRAY_CHAR			1
#define PPL_ARRAY_INT			2
#define PPL_ARRAY_FLOAT			3
#define PPL_ARRAY_CSTRING		4
#define PPL_ARRAY_CPPLSTRING	4
#define PPL_ARRAY_ARRAY			5
#define PPL_ARRAY_BINARY		6

#define PPL_CHAR				1
#define PPL_INT					2
#define PPL_FLOAT				3
#define PPL_STRING				4
#define PPL_ASSOCARRAY			5
#define PPL_BINARY				6
#define PPL_POINTER				7
#define PPL_UTF16STRING			8
#define PPL_UTF16				8


typedef struct tagArrayResult {
		void *item;
} ARRAYRESULT;

typedef const ARRAYRESULT * ARRAY_RESULT;

class CAssocArray : public CVar
{
	private:
		CTree			Tree;
		ARRAY_RESULT	FindInternal(const char *key) const;
		ARRAY_RESULT	CreateTree(const char *key);
		ppluint64		maxint;
		int				num;

	public:
		//!\name Konstruktoren und Destruktoren
		//@{
		CAssocArray();
		CAssocArray(const CAssocArray &a);
		CAssocArray(const CArray &a);
		CAssocArray(const CAssocArray *a);
		~CAssocArray();
		//@}

		//!\name Informationen ausgeben/auslesen
		//@{
		void	List(const char *prefix=NULL) const;
		int		Count(bool recursive=false) const;
		int		Count(const char *key, bool recursive=false) const;
		int		Validate(const char *prefix=NULL);
		//@}

		//!\name Werte setzen
		//@{
		int		Add (const char *key, const char *value, int size=-1);
        int		Add (const char *key, ppldds value);
		int		Set(const char *key, const char *value, int size=-1);
		int		Set(const char *key, const CString &value, int size=-1);
		int		Set(const char *key, const CWString &value, int size=-1);
		int		Setf(const char *key, const char *fmt, ...);
		int		Set(const char *key, const CAssocArray &array);
		int		Set(const char *key, CAssocArray *array, bool copy=true);
		int		Set(const char *key, const CArray &array);
		int		Set(const char *key, const CDirEntry &d);
		int		Set(const char *key, const CBinary &bin);
		int		Set(const char *key, CBinary *bin, bool copy=true);
		int		Set(const char *key, const CDateTime &date);
		int		SetPointer(const char *key, const void *ptr);
		int		Copy(const CAssocArray *a);
		int		Copy(const CAssocArray &a);
		int		Copy(const CDirEntry &d);
		int		Copy(const CArray &a);
		int		Copy (const char *key, const CAssocArray &a, const char *sourcekey=NULL);
		int		Merge(const char *key, const CAssocArray &a);
		//@}

		//!\name Werte erweitern (nur Strings)
		//@{
		int		Concat(const char *key, const char *value, const char *concat="\n", int size=-1);
		int		Concatf(const char *key, const char *concat, const char *fmt, ...);
		//@}

		//!\name Werte löschen
		//@{
		void	Clear();
		int		Delete(const char *key);
		int		Deletef(const char *fmt, ...);
		//@}


		//!\name Import und Export von Daten
		//@{
		int		Size() const;
		int		CreateFromTemplate(const char *templ, const char *linedelimiter="\n", const char *splitchar="=", const char *concat="\n", bool dotrim=false);
		int		CreateFromConfig(const char *content, const char *splitchar=":", const char *concat="\n", bool dotrim=false);
		void	ToTemplate(CString &s, const char *prefix=NULL, const char *linedelimiter="\n", const char *splitchar="=");
		int		ExportBinary(void *buffer, int buffersize, int *realsize) const;
		CBinary *ExportBinary() const;
		int		ImportBinary(const void *buffer, int buffersize);
		int		ImportBinary(const CBinary &bin);

		//@}

		//!\name Werte direkt auslesen
		//@{
		ARRAY_RESULT	GetValue(const char *key) const;
		const char		*GetChar(const char *key) const;
        const char		*Get (const char *key) const;
        const char		*Getf (const char *fmt, ...) const;
		CString			*GetString(const char *key) const;
		CDateTime		GetDateTime(const char *key) const;
		CString			*GetStringf(const char *fmt, ...) const;
		CString			*GetCString(const char *key) const;
		CString			*GetCStringf(const char *fmt, ...) const;
		CWString		*GetCWString(const char *key) const;
		CWString		*GetCWStringf(const char *fmt, ...) const;
		int				GetCArray(CArray &a, const char *key=NULL) const;
		CAssocArray		*GetArray(const char *key) const;
		CAssocArray		*GetArrayf(const char *fmt, ...) const;
		CBinary			*GetBinary(const char *key) const;
		CBinary			*GetBinaryf(const char *fmt, ...) const;

		int				ToInt(const char *key) const;
		long			ToLong(const char *key) const;
		long long		ToLongLong(const char *key) const;
		bool			ToBool(const char *key) const;
		CString			ToCString(const char *key) const;
		CWString		ToCWString(const char *key) const;
		CAssocArray		ToCAssocArray(const char *key) const;
		CArray			ToCArray(const char *key) const;
		CBinary			ToCBinary(const char *key) const;
		bool			HaveKey(const char *key) const;

		void			*GetPointer(const char *key) const;
		void			*GetPointerf(const char *fmt, ...) const;
		int 			GetKey (CString &key, const char *value, bool recursive=false);

		//@}

		//!\name Werte prüfen
		//@{
		int				IsTrue(const char *key) const;
		int				IsFalse(const char *key) const;
		int				IsString(ARRAY_RESULT res) const;
		int				IsCString(ARRAY_RESULT res) const;
		int				IsCWString(ARRAY_RESULT res) const;
		int				IsArray(ARRAY_RESULT res) const;
		int				IsPointer(ARRAY_RESULT res) const;
		int				IsBinary(ARRAY_RESULT res) const;
		int				GetDatatype(ARRAY_RESULT res) const ;
		//@}

		//!\name Array durchwandern
		//@{
		void			Reset();
		ARRAY_RESULT	GetFirst();
		ARRAY_RESULT	GetNext();
		ARRAY_RESULT	GetLast();
		ARRAY_RESULT	GetPrevious();

		void			Reset(CTreeWalker &walk) const;
		ARRAY_RESULT	GetFirst(CTreeWalker &walk) const;
		ARRAY_RESULT	GetNext(CTreeWalker &walk) const;


		CAssocArray		*GetFirstArray();
		CAssocArray		*GetNextArray();
		CAssocArray		*GetLastArray();
		CAssocArray		*GetPreviousArray();

		CAssocArray		*GetFirstArray(CTreeWalker &walk) const;
		CAssocArray		*GetNextArray(CTreeWalker &walk) const;
		CAssocArray		*GetLastArray(CTreeWalker &walk) const;
		CAssocArray		*GetPreviousArray(CTreeWalker &walk) const;

		int				GetFirstArray(CAssocArray &res);
		int				GetNextArray(CAssocArray &res);
		int				GetLastArray(CAssocArray &res);
		int				GetPreviousArray(CAssocArray &res);


		int				GetFirst(char **key, char **value);
		int				GetFirst(CString *key, CString *value);
		int				GetFirst(CString &key, CString &value);
		int				GetNext(char **key, char **value);
		int				GetNext(CString &key, CString &value);
		int				GetNext(CString *key, CString *value);

		int				GetLast(char **key, char **value);
		int				GetLast(CString &key, CString &value);
		int				GetLast(CString *key, CString *value);
		int				GetPrevious(char **key, char **value);
		int				GetPrevious(CString &key, CString &value);
		int				GetPrevious(CString *key, CString *value);

		CString			*GetString(ARRAY_RESULT res) const;
		CString			*GetCString(ARRAY_RESULT res) const;
		CWString		*GetCWString(ARRAY_RESULT res) const;
		const char		*GetChar(ARRAY_RESULT res) const;
		CAssocArray		*GetArray(ARRAY_RESULT res) const;
		void			*GetPointer(ARRAY_RESULT res) const;
		CBinary			*GetBinary(ARRAY_RESULT res) const;
		const char		*GetKey(ARRAY_RESULT res, CString *str=NULL) const;
		const char		*GetKey(ARRAY_RESULT res, CString &key) const;
		const char		*GetCurrentKey(CString *str=NULL) const;
		const char		*GetCurrentKey(CString &key) const;

		//@}

		//!\name Operatoren
		//@{
		const char *operator[](const char *key) const;
		CAssocArray& operator=(const CAssocArray& a);
		CAssocArray& operator=(const CArray& a);
		CAssocArray& operator=(const CAssocArray *a);
		CAssocArray& operator=(CDirEntry& d);
		CAssocArray& operator=(CDirEntry *d);
		//@}
};

int Count(CAssocArray *a, bool recursive=false);
CAssocArray *Ini2Array(const char *content, const char *splitchar=":", const char *concat="\n", CAssocArray *res=NULL, bool dotrim=true);


/***************************************************************************
 * Configure                                                               *
 ***************************************************************************/

//! \brief Klasse zum Verarbeiten von Config-Files
class CConfig
{
	private:
		char	*separator;
		size_t	separatorLength;
		char	*currentsection;
		bool	isused;
		void	*first, *last, *section;
		CAssocArray sections;

		void Init();
		void *FindSection(const char *sectionname);

	public:
		//ppldd	err, suberr;

		CConfig();
		CConfig(const char *filename);
		CConfig(CFileObject *file);
		~CConfig();
		int Load(const char *filename, ...);
		int LoadFromString(CString &string);
		int LoadFromMemory(void *buffer, int bytes);
		int Load (CFileObject *file);
		int Save(const char *filename);
		int Save(CFileObject *file);
		void Unload();
		void SetSeparator(const char *string);
		int  SelectSection(const char *section);
		int  FirstSection();
		int  NextSection();
		const char *GetSectionName();
		const char *GetSection(const char *section);
		CAssocArray *ReadSection(const char *section, CAssocArray *a=NULL);
		int  CreateSection (const char * section);
		int  DeleteSection (const char *section);
		int  Add(const char *section, const char *key, const char *value);
		int  Add(const char *key, const char *value);
		int  Add(const char *key, int value);
		int  Add(const char *section, const char *key, int value);
		int  Add(const char *section, const char *key, bool value);
		int  Delete(const char * key);
		int  Delete(const char * section, const char * key);
		const char *Get(const char *key, const char *defaultvalue=NULL);
		const char *Get(const char *section, const char *key, const char *defaultvalue=NULL);
		bool	GetBool(const char *section, const char *key, bool defaultvalue=false);
		bool	GetBool(const char *key, bool defaultvalue=false);
		int		GetInt(const char *section, const char *key, int defaultvalue=0);
		int		GetInt(const char *key, int defaultvalue=0);
		int  	IsTrue(const char *value);

		void Reset();								// Zum Auslesen einer kompletten Section
		int  GetFirst(char **key, char **value);	// Zum Auslesen einer kompletten Section
		int  GetNext(char **key, char **value);
		int  GetFirst(CString *key, CString *value);
		int  GetNext(CString *key, CString *value);

		void String2Rect (const char * string, RECT * r);
		int String2Point (const char * string, POINT * p);

		void Print();
};

class CList;

//! \brief Klasse für Listen-Elemente
class CListItem
{
	friend class CList;
	private:
		CListItem *next, *previous;
		CList *list;
	public:
		CListItem();
		virtual ~CListItem();
};

//! \brief Listen-Klasse für CListItem Elemente
class CList
{
	private:
		CListItem *first, *last;
		CListItem *pointer;
		int counter;

	public:
		CList();
		~CList();
		void		Clear(bool deleteitems=false);
		int			Add(CListItem *item);
		int			Delete(CListItem *item);
		CListItem	*GetFirst();
		CListItem	*GetNext();
		CListItem	*GetLast();
		CListItem	*GetPrevious();
		void		Reset();
		int			Num() const;
		int			CheckConsistency();
};


//! \brief Listen-Klasse für beliebige Elemente
class CGenericList
{
	public:
		class Walker
		{
			friend class CGenericList;
			private:
				void		*pointer;
			public:
				Walker();
		};

	private:
		void *first, *last;
		void *pointer;
		void *Find(const void *data);
		ppldd counter;
		int (*DestroyFunction)(void *item,void *data);
		void *DestroyData;

	public:
		CGenericList();
		virtual ~CGenericList();

		void Clear(bool deleteitems=false);
		void Reset();
		int Num() const;
		void *Add(const void *data);
		void *Copy(const void *data, size_t size);
		int Delete(const void *data);
		int HasObject(const void *data);
		void *GetFirst();
		void *GetNext();
		void *GetLast();
		void *GetPrevious();

		void Reset(Walker &walk) const;
		void *GetFirst(Walker &walk) const;
		void *GetNext(Walker &walk) const;
		void *GetLast(Walker &walk) const;
		void *GetPrevious(Walker &walk) const;

		virtual int Destroy(void *data);
		int SetDestroyFunction(int DestroyFunction(void *item,void *data),void *data);
};


class CQueue
{
	private:
		CMutex Mutex;
		void *first, *last;
		int count;
		int max;
		bool usemutex;

	public:
		CQueue();
		CQueue(int max);
		~CQueue();
		void Clear();
		void UseMutex(bool flag);
		void SetMaximum(int max);
		int Put(const void *item);
		int Add(const void *item);
		const void *Get();
		int Count();


};

class CDirEntry
{
	public:
		CDirEntry();

		CString		Filename;
		CString		Path;
		CString		File;
		ppluint64	Size;
		ppluint32	Attrib;
		ppluint32	Uid;
		ppluint32	Gid;
		ppluint32	Blocks;
		ppluint32	BlockSize;
		ppluint32	NumLinks;
		char		AttrStr[11];
		ppluint64	ATime, CTime, MTime;
		int			IsDir() const;
		int			IsFile() const;
		int			IsLink() const;
		int			IsHidden() const;
		int			IsReadOnly() const;
		int			IsArchiv() const;
		int			IsSystem() const;
		int			Copy(CDirEntry *dir);
		CDirEntry& operator=(const CDirEntry& dir);
		CDirEntry& operator=(const CDirEntry *dir);
		int			ToArray(CAssocArray *a) const;
		void		Print(char *label=NULL) const;
};


class CDir
{
	public:
		enum Sort {
			Sort_None,
			Sort_Filename,
			Sort_Filename_IgnoreCase,
			Sort_Date,
			Sort_ATime,
			Sort_CTime,
			Sort_MTime,
			Sort_Size
		};
	private:
		ppl6::List<CDirEntry> Files;
		ppl6::List<const CDirEntry*> SortedFiles;
		ppl6::List<const CDirEntry*>::Iterator myit;

		Sort sort;
		CString Path;

		void resortMTime();
		void resortCTime();
		void resortATime();
		void resortSize();
		void resortFilename();
		void resortFilenameIgnoreCase();
		void resortNone();

	public:
		CDir(const CString &path=CString(), Sort s=Sort_None);
		~CDir();
		int Open(const CString &path, Sort s=Sort_None);
		void Resort(Sort s);
		void Reset();
		void Clear();
		int Num() const;
		const CDirEntry *GetFirst();
		const CDirEntry *GetNext();
		const CDirEntry *GetFirstPattern(const char *pattern, bool ignorecase=false);
		const CDirEntry *GetNextPattern(const char *pattern, bool ignorecase=false);
		const CDirEntry *GetFirstRegExp(const char *regexp);
		const CDirEntry *GetNextRegExp(const char *regexp);

		void Print();
		void Print(const CDirEntry *de);

		static CString CurrentPath();
		static CString homePath();
		static CString tempPath();
		//static CString HomePath();
		//static CString TempPath();

};

/***************************************************************************************************
 * Socket-Klassen
 ***************************************************************************************************/


class CResolver
{
	private:
		void *res;
		void *packet;

		int InitInternal();

	public:
		// Quelle: http://en.wikipedia.org/wiki/List_of_DNS_record_types
		enum Type {
			A=1,
			NS=2,
			CNAME=5,
			MX=15,
			SOA=6,
			PTR=12,
			TXT=16,
			AAAA=28,
			NAPTR=35,
			SRV=33,
			DS=43,
			DNSKEY=48,
			NSEC=47,
			NSEC3=50,
			RRSIG=46,
			OPT=41,
			TSIG=250
		};

		enum Class {
			/** the Internet */
			CLASS_IN    = 1,
			/** Chaos class */
			CLASS_CH    = 3,
			/** Hesiod (Dyer 87) */
			CLASS_HS    = 4,
			/** None class, dynamic update */
			CLASS_NONE      = 254,
			/** Any class */
			CLASS_ANY   = 255,
			CLASS_FIRST     = 0,
			CLASS_LAST      = 65535,
			CLASS_COUNT     = CLASS_LAST - CLASS_FIRST + 1
		};

		CResolver();
		//CResolver(const CResolver &other);
		~CResolver();
		static CString typeName(Type t);
		static CString className(Class c);

		void setTimeout(int seconds, int microseconds);
		void setDnssecEnabled(bool flag);
		void setEdnsUdpSize(size_t s);
		size_t ednsUdpSize() const;
		bool dnssecEnabled() const;
		int setNameservers(const CArray &list);
		int setNameserver(const CString &server);
		int setDNSSECAnchor(const CString &anchor);
		int setDNSSECAnchor(const CArray &anchors);
		int setDNSSECAnchorFromFile(const CString &filename);
		int setDNSSECAnchorFromFile(CFileObject &file);

		int query(CAssocArray &r, const CString &label, Type t=A, Class c=CLASS_IN);
};


int GetHostByName(const char *name, CAssocArray *Result);
int GetHostByAddr(const char *addr, CAssocArray *Result);
CString GetHostname();


class CTCPSocket;

//! \brief SSL-Verschlüsselung
class CSSL
{
	friend class CTCPSocket;
	private:
		CMutex	Mutex;
		void		*ctx;
		void		*first_ref, *last_ref;
		int			references;
		void 		Clear();
		void *		RegisterSocket(CTCPSocket *socket);
		int			ReleaseSocket(CTCPSocket *socket, void *data);
	public:
		enum SSL_METHOD {
			TLS,				// uses TLS on openssl version >=1.1.1 and SSLv23 on older versions
			TLSclient,
			TLSserver,
			// The following Methods are obsolete!
			SSLv2,
			SSLv2client,
			SSLv2server,
			SSLv3,
			SSLv3client,
			SSLv3server,
			TLSv1,
			TLSv1client,
			TLSv1server,
			SSLv23,				// Unterstuetzt SSLv2, v3 und TLSv1.x
			SSLv23client,		// Unterstuetzt SSLv2, v3 und TLSv1.x
			SSLv23server,		// Unterstuetzt SSLv2, v3 und TLSv1.x
			TLSv1_1,
			TLSv1_1client,
			TLSv1_1server,
			TLSv1_2,
			TLSv1_2client,
			TLSv1_2server,
			DTLSv1,
			DTLSv1client,
			DTLSv1server
		};

		CSSL();
		~CSSL();
		int 	Init(int method=0);
		int		IsInit();
		int 	Shutdown();
		void 	*NewSSL();
		int		LoadTrustedCAfromFile(const char *filename);
		int		LoadTrustedCAfromPath(const char *path);
		// LoadCertificate wird benoetigt, wenn ein SSL-Server gestartet werden soll.
		// LoadCertificate laed ein Zertifikat im PEM-Format oder eine komplette Trustchain im
		// PEM-Format aus dem File "certificate". Wird "privatekey" angegeben, wird daraus der
		// Private Key geladen. Wenn nicht, wird der Private Key ebenfalls in der "certificate"-
		// Datei erwartet. Ist der Key durch ein Passwort geschuetzt, muss dieses als "password"
		// angegeben werden.
		int		LoadCertificate(const char *certificate, const char *privatekey=NULL, const char *password=NULL);
		int		SetCipherList(const char *cipherlist);		// "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"
		int		SetTmpDHParam(const CString &dh_param_file);
		void	*GetSSLContext();
};

class SSLError
{
	public:
		SSLError() {
			Code=0;
			Line=Flags=0;
		}
		CString			Text;
		CString			Filename;
		CString			Data;
		unsigned long	Code;
		int				Line;
		int				Flags;
};

int GetSSLError(SSLError &e);
int GetSSLErrors(std::list<SSLError> &e);
int GetSSLErrors(CString &e);
void ClearSSLErrorStack();



//! \brief TCP-Socket-Nachrichten
class CSocketMessage
{
	friend class CTCPSocket;
	private:
		int			datatype;
		void		*data;
		void		*incoming_data;
		int			size;
		int			incoming_size;
		int			incoming_type;
		int			commandId;
		int			Id;
		int			Version;
		bool		UseCompression;
		bool		SupportMsgChannel;

	public:
		bool		ClientSupportsCompression;

		CSocketMessage();
		CSocketMessage(const CSocketMessage &other);

		virtual ~CSocketMessage();
		void Copy(const CSocketMessage &other);
		void Copy(const CSocketMessage *other);

		void Clear();
		void SetCommandId(int id);
		void SetId(int id);
		int SetData(const CString &msg);
		int SetData(const char *msg);
		int SetData(const CAssocArray &msg);
		int GetData(CString &msg);
		const char*GetData();
		int GetData(CAssocArray &msg);
		int GetId();
		int GetCommandId();
		int GetType();
		void Dump(CString &buffer);
		void Dump();
		void Dump(CLog *Log, int facility=LOG::DEBUG, int level=1);
		int SetVersion(int version);
		void EnableCompression();
		void DisableCompression();
		void EnableMsgChannel();
		void DisableMsgChannel();
		bool isCompressionSupported() const;
		bool isMsgChannelSupported() const;
};

//! \brief TCP-Socket-Klasse
class CTCPSocket
{
	private:
		CMutex	mutex;
		CLog		*log;
		CSSL		*sslclass;
		CThread	*thread;
		void		*sslreference;
		void *socket;
		void *ssl;
		int connected;
		int islisten;
		int		BytesWritten;
		int		BytesRead;
		int		connect_timeout_sec;
		int		connect_timeout_usec;
        bool    stoplisten;
        CString	HostName;
        int			PortNum;
        CString	SourceHost;
        int			SourcePort;

        int		SSL_Write(const void *buffer, int size);
        int		SSL_Read(void *buffer, int bytes);

	public:
		CTCPSocket();
		virtual ~CTCPSocket();
		void SetLogfile(CLog *log);
		void SetConnectTimeout(int seconds, int useconds);
		int SetReadTimeout(int seconds, int useconds);
		int SetWriteTimeout(int seconds, int useconds);
		void SetSource(const char *host, int port=0);
		int Connect(const char *host_and_port);
		int Connect(const char *host, int port);
		int ConnectSSL(const char *host_and_port, CSSL *ssl=NULL);
		int ConnectSSL(const char *host, int port, CSSL *ssl=NULL);
		void DispatchErrno();
		int GetBytesWritten();
		int GetBytesRead();
		const CString& getHostname() const;
		int getPort() const;

        int Bind(const char *ip, int port);
		int Disconnect();
		int Write(const CString &str);
		int Write(const CBinary &bin);
		int Write(const CString *str);
		int Write(const void *buffer, int bytes);
		int WriteBuffer(const void *buffer, int bytes);
		int Writef(const char *fmt, ...);
		int Write(CSocketMessage &msg);
		int Read(void *buffer, int bytes);
		int Read(CString &buffer, int bytes);
		int Read(CString *buffer, int bytes);
		int Read(CBinary &buffer, int bytes);
		char *Read(int bytes);
		int ReadOnce(void *buffer, int bytes);
		int ReadOnce(CString &buffer, int bytes);
		int ReadOnce(CString *buffer, int bytes);
		char *ReadOnce(int bytes);

		int IsConnected();
		int IsListening();
        int StopListen();
		int SignalStopListen();
        int Listen(int timeout=100);
        int Shutdown();
        int	WaitForMessage(CSocketMessage &msg, int timeout=0);
        int WatchThread(CThread *thread);
		int SetBlocking(bool value);
		int GetDescriptor();
		int IsWriteable();
		int IsReadable();
		int WaitForIncomingData(int seconds, int useconds);
		int WaitForOutgoingData(int seconds, int useconds);

        virtual int ReceiveConnect(CTCPSocket *socket, const char *host, int port);

		static ppluint32 Ntohl(ppluint32 net);
		static ppluint32 Htonl(ppluint32 host);
		static ppluint16 Ntohs(ppluint16 net);
		static ppluint16 Htons(ppluint16 host);

		int SSL_Init(CSSL *ssl);
		int SSL_Shutdown();
		int SSL_Init_Client();
		int SSL_Init_Server();
		int SSL_Start();
		int SSL_Stop();
		int SSL_CheckCertificate(const char *hostname, bool AcceptSelfSignedCert=false);
		int SSL_Accept();
		int SSL_WaitForAccept(int timeout=0);
		void SSL_Info();

		bool SSL_IsEncrypted() const;
		CString SSL_GetCipherName() const;
		CString SSL_GetCipherVersion() const;
		int SSL_GetCipherBits() const;

};

int SSL_Init();
int SSL_Exit();

class CUDPSocket
{
	private:
		int		timeout_sec;
		int		timeout_usec;
		void	*socket;

	public:
		CUDPSocket();
		~CUDPSocket();
		void SetTimeout(int seconds, int useconds);
		int SendTo(const char *host, int port, const void *buffer, int bytes);
		int SendTo(const char *host, int port, const CString &buffer);
		int RecvFrom(void *buffer, int maxlen);
		int RecvFrom(void *buffer, int maxlen, CString &host, int *port);
		int RecvFrom(CString &buffer, int maxlen);
		int SetReadTimeout(int seconds, int useconds);
		int Bind(const char *host, int port);
		int GetDescriptor();

};

class Webserver
{
	private:
		CSSL		SSL;
		void		*daemon;
		CLog		*log;
		int			port;
		CAssocArray	res;
		CTCPSocket	Socket;
		bool		basicAuthentication;
		bool		SSLEnabled;
		CString		realm;
		CString		sslkey, sslcert;

	public:
		class Request {
			friend class Webserver;
			private:

			public:
				void *connection;
				CString url;
				CString method;
				CString version;
				CAssocArray header;
				CAssocArray data;
				CAssocArray auth;
		};
		Webserver();
		virtual ~Webserver();
		void setLogfile(CLog *log);
		void bind(const CString &adr, int port);
		const CString & getHostname() const;
		int getPort() const;
		void loadCertificate(const CString &certificate, const CString &privatekey, const CString &password);
		void enableSSL(bool enable);
		bool sslEnabled() const;
		void start();
		void stop();
		void requireBasicAuthentication(bool enable, const CString &realm);
		bool useBasicAuthentication() const;
		void queueResponse(const Request &req, const CString &text, int httpStatus=200);
		void queueBasicAuthFailedResponse(const Request &req);
		void queueResponseError(const Request &req);
		void queueResponseException(const Request &req, const ppl6::Exception &e);

		virtual int request(Request &req);
		virtual int authenticate(const CString &username, const CString &password, Request &req);
		virtual CString getDenyMessage();




		PPLPARAMETERISEDEXCEPTION(CouldNotBindToSocket);
		PPLPARAMETERISEDEXCEPTION(InvalidSSLCertificate);
		PPLNORMALEXCEPTION(NoAddressSpecified);
		PPLPARAMETERISEDEXCEPTION(CouldNotStartDaemon);
		PPLNORMALEXCEPTION(SSLInitializationFailed);
		PPLPARAMETERISEDEXCEPTION(CouldNotCreateResponse);
		PPLPARAMETERISEDEXCEPTION(CouldNotQueueResponse);

};


class CCurl;

typedef struct {
    CCurl        *curl;
    size_t          size;           // Groesse der kompletten Datei
    size_t          bytes;
    size_t          currentsize;    // Bisher downgeloadet
    ppldd           bps;
    ppldd           bps_average;
    const char *    filename;
} PPL_CURL_NOTIFYDATA;

//! \brief CURL-Klasse
class CCurl
{
	private:
		void *handle;
		void *post, *last;
		void *headers;
		char *resultbuffer;
		CLog	*log;
		size_t resultbuffer_size;
		CString Header;
		CString Browser, Url, UserPassword, Referer, Proxy;
		CString GetCall;
		char *errorbuffer;
		bool	aboard;

		int SetOptOk(int ret);



	public:

		enum HTTPVERSION {
			HTTP_1_0=1,
			HTTP_1_1,
		};

		static bool isSupported();

		CCurl();
		~CCurl();

		CNotifyHandler call_receive;
		CNotifyHandler call_send;
		CNotifyHandler call_done;

		void DebugHandler(int type, const char *data, size_t size);
		size_t StoreResult(void *ptr, size_t bytes, int type);
		void	SetLogfile(CLog *log);

		int		SetBrowser(const char *browser);
		int		SetURL(const char *url);
		int		SetReferer(const char *url);
		int		SetUserPassword(const char *username, const char *password);
		int		SetUsername(const CString &username);
		int		SetPassword(const CString &password);
		int		SetUserPassword(const char *userpassword);	// Format: username:password
		int		SetProxy(const char *proxy, int port);
		int		SetHttpVersion(CCurl::HTTPVERSION version);
		int		SetTimeout(int seconds);
		int		SetHeader(const char *name, const char *value);
		int		SetMaximumPersistantConnects(int value);
		int		EnableSignals(bool enable);
		int		Reset();
		int		ClearHeader();
		int		Get();
		int		Get(const char *parameter);
		int		Get(CAssocArray &param);
		int		GetResultBuffer(void **buffer, size_t *size);
		int		GetResultBuffer(CBinary &bin);
		int		CopyResultBuffer(CBinary &bin);
		CString	GetResultBuffer();
		int		GetHeader(CString &str);
		CString GetHeader();
		CString GetLastURL();
		const char *GetURL();
		int		Escape(CString &target, CAssocArray &source);
		int		Escape(CString &string);
		int		AddPostVar(const char *name, const char *data, const char *contenttype=NULL);
		int		AddPostVar(const char *name, int val, const char *contenttype=NULL);
		//int		AddPostFile(const char *name, const char *filename);
		int		AddPostVar(CAssocArray &param, const char *prefix=NULL);
		int		Post();
		int		Post(CAssocArray &param);
		int		PostFields(CAssocArray &param);
		void	Clear();

		int		AddCAFile(const char *filename);
		int		VerifyPeer(bool verify);

		void *getCurlHandle() const;
		/*
		int  StoreToFile(char *filename);
		int  POST();
		void StopTransfer();
		const char *GetErrorBuffer();
		const char *GetFilename();
		size_t	GetFilesize();
		CFile *GetResultFile();
		*/
};

//!\brief Klasse zum Verschicken von Emails
class CSendMail
{
	private:
		CAssocArray	Header;
		CString		SMTPServer;
		CString		MyHostname;
		CString		Sender;
		CString		CompleteMail;

		int Talk(CTCPSocket &socket,CString &Reply, CString &Msg);
		int Talk(CTCPSocket &socket,CString &Reply, const char *msg, ...);
		int ParseAnswer(CString &Answer, CString &Text);


	public:
		CString		Message;

		CSendMail();
		~CSendMail();
		void Clear();
		int SetHeader(const char *name, const char *content, ...);
		int CopyHeader(CAssocArray &header);
		int DeleteHeader(const char *name);
		int SetServer(const char *hostport);
		int SetLocalHostname(const char *name);
		int SetSender(const char *name);
		int SetMessage(const char *message);
		int SetMessage(CString &message);
		int Send();
		const char *GetMail();
};

class CReadMail
{
	private:
		CAssocArray HeaderLines;
		CString		Mail;
		CString		Header, Body, Decoded, Transcoded;
		bool		Loaded;

		int ParseHeader();
		int RemoveCR();
		int SplitHeaderAndBody();
		int DecodeBody();
		int Decode_QuotedPrintable();
		int Decode_Base64();


	public:
		CReadMail();
		~CReadMail();
		void Clear();
		int LoadFile(const char *filename, ...);
		int LoadFile(CFileObject *file);
		int LoadFile(CFileObject &file);
		int LoadString(const char *string);
		int LoadString(const CString &string);
		const char *GetMail();
		int GetMail(CString &Buffer);
		CAssocArray *GetHeaderArray(CAssocArray *Buffer=NULL);
		int GetHeaderArray(CAssocArray &Buffer);
		const char *GetHeader(const char *key);
		const char *GetHeader();
		const char *GetBody();
		const char *GetDecodedBody();
		int GetDecodedBody(CString &Buffer);
		const char *GetTranscodedBody(const char *charset="UTF-8");
		int GetTranscodedBody(CString &Buffer, const char *charset="UTF-8");
		int GetBody(CString &Buffer);
		int GetHeader(CString &Buffer);
		int GetHeader(CString &Buffer, const char *key);

};

class CHostsAllow
{
	private:
		CMutex		Mutex;
		CAssocArray		Hosts;
		CLog			*Log;
		bool 			enabled;


	public:
		CHostsAllow();
		~CHostsAllow();
		void Clear();
		int LoadFromFile(const char *filename, ...);
		int LoadFromFile(const CString &filename);
		int LoadFromFile(CFileObject &file);
		int Load(const char *text);
		int Count();
		int Num();
		int IsEnabled();
		void Enable(bool flag=true);
		int IsAllowed(const char *ip);
		void SetLogfile(CLog *log=NULL);
};

ppluint32 IP2Int(CString &ip);

//! \brief PPL-Resourcen
class CResource
{
	private:
		CHeap	heap;
		int count;
		int maxid, minid;
		int major, minor;
		CMemory memory;
		CMemoryReference memref;
		void *firstchunk;

		int CheckResource(const CMemoryReference &memory);
		int Parse();
		int Uncompress(void *resource);

		void *Find(int id);
		void *Find(const char *name);

	public:
		CResource();
		~CResource();
		void Clear();
		int Load(const char *filename, ...);
		int Load(CFileObject *file);
		int Load(CFileObject &file);
		int Load(const CMemoryReference &memory);
			/* Bei Load wird die gesamte Datei in den Speicher geladen und erst
			 * beim Loeschen der Resource wieder freigegeben. Die angegebene Datei
			 * kann vorher geschlossen werden
			 */
		int UseMemory(const CMemoryReference &memory);
		int UseMemory(void *ptr, size_t size);
			/* Wird UseMemory benutzt, muss ein Pointer auf einen Speicherbereich
			 * angegeben werden, der seine Gueltigkeit nicht verlieren darf,
			 * solange die Instanz von CResource existiert.
			 */

		CFileObject *GetFile(int id);
		CFileObject *GetFile(const char *name);
		CMemoryReference GetMemory(int id);
		CMemoryReference GetMemory(const char *name);

		static CResource *GetPPLResource();
		static int GenerateResourceHeader(const char *basispfad, const char *configfile, const char *targetfile, const char *label);
};

CResource *GetPPLResource();

//! \brief Sprachdateien verwalten
class CLang
{
	private:
		char *values;
		char *header;
		char *data;
		void *table;

		char *val[10];
		ppldd count,maxid;
		char *country;
		char *language;
		char *international;
		char *choose;

		CAssocArray replace;

		void Replace(char *alt, char *neu, char *buffer, ppldd buffersize);

	public:
		CLang();
		~CLang();

		void Unload();

		int Load(const char *file);
		int Load(CFileObject &file);

		// Pruefen, ob eine bestimmte Sprache in der Datei vorkommt
		bool HasCountry(char *country);

		// Strings auslesen:
		const char *GetRaw(ppldd id);
		char *Get(ppldd id, char *buffer=NULL, ppldd buffersize=0);

		// Platzhalter definieren:
		void SetValue(ppldd id, char *value);
		void SetValue(ppldd id, ppldd value);
		void SetValue(ppldd id, float value, int dezimalstellen=2);
		char *GetValue(ppldd id);

		// Zeichenketten definieren, die durch die Get-Funktion
		// ausgetauscht werden sollen:
		void SetReplace(char *str, char *replacement);

		// Sprachtemplate kompilieren:
		int Compile(char *lang_template, char *file);
};

//! \brief Timer-Klasse
class CTimer
{
	private:
		double startzeit, endzeit, duration;
	public:
		CTimer();
		~CTimer();
		double Start();
		double Stop();
		double GetCurrentDuration();
		double GetDuration();
};

//! \brief Character-Konvertierung
class CIconv
{
	private:
		void *cd;
		CString From, To;

	public:
		CIconv();
		CIconv(const char *fromcode, const char *tocode);
		~CIconv();
		int Init(const char *fromcode, const char *tocode);
		CString Transcode(CString &str);
		int		TranscodeString(CString &str);
		char *Transcode(const char *str, size_t size=0);
		char *Transcode(const char *source, size_t size_source, size_t *size_target);
		int Transcode(const char *source, size_t size, CBinary &target);

		static int Enumerate(CArray &list);
		static int Enumerate(CAssocArray &list);
};


class PFPChunk : public CListItem
{
	friend class PFPFile;
	private:
		char chunkname[5];
		void *data;
		int size;
	public:
		PFPChunk();
		~PFPChunk();
		int SetName(const char *chunkname);
		int SetData(const void *ptr, int size);
		int SetData(const CString &s);
		int SetData(const char *s);
		int Size();
		const void *Data();
		const char *Name();
};

class PFPFile
{
	private:
		CList Chunks;
		char id[5];
		ppluint8 mainversion, subversion;
		CCompression::Algorithm comp;
		CString findchunk;

		int SetParam(const char *chunkname, const char *data);


	public:
		CMutex	Mutex;

		PFPFile();
		virtual ~PFPFile();
		void Clear();
		int SetAuthor(const char *author);
		int SetCopyright(const char *copy);
		int SetDescription(const char *descr);
		int SetName(const char *name);
		int SetVersion(int main=0, int sub=0);
		int SetId(const char *id);
		int Save(const char *filename);
		int AddChunk(PFPChunk *chunk);
		int DeleteChunk(PFPChunk *chunk);
		int DeleteChunk(const char *chunkname);
		PFPChunk *FindFirstChunk(const char *chunkname);
		PFPChunk *FindNextChunk(const char *chunkname=NULL);
		virtual void List();
		int SetCompression(CCompression::Algorithm type);

		int Load(CFileObject *ff);
		int Load(const char *file);
		bool Ident(CFileObject &ff);
		bool Ident(const CString &file);


		virtual int LoadRequest(const char *id, int mainversion ,int subversion);

		const char *GetName();
		const char *GetDescription();
		const char *GetAuthor();
		const char *GetCopyright();
		void GetVersion(int *main, int *sub);
		const char *GetID();
		int GetMainVersion();
		int GetSubVersion();
		CCompression::Algorithm GetCompression();

		void Reset();
		PFPChunk *GetFirst();
		PFPChunk *GetNext();

};

class CCrontab;

class CCronjob : private CListItem
{
	friend class CCrontab;
	private:
		double intervall;
		double nextevent;

		CCrontab *crontab;
		CCallback	*callback;
		void			*callback_data;

	public:
		CCronjob();
		virtual ~CCronjob();
		void SetCallback(CCallback *c, void *data=NULL);
		void StartIntervall(int ms);
		void StartOnce(int ms);

};

class CCrontab : private CThread
{
	private:
		CMutex	Mutex;
		CList jobs;

	public:
		CCrontab();
		~CCrontab();

		int Start();
		int Stop();

		int Add(CCronjob *job);
		int Delete(CCronjob *job);
		virtual void ThreadMain(void *param);


};


} // namespace ppl6

namespace ppl6 {
	class CApplication;

int PPL6Main (int argc, char **argv, CApplication *app);
#ifdef _WIN32
#ifndef __MINGW32__
	int PPL6MainWin (HINSTANCE hInstance, HINSTANCE hPrevInstance, char * lpCmdLine, int nCmdShow, CApplication *app);
#endif
#endif
}

#ifdef __MINGW32__
	#define PPL6APP(APP) int main(int argc, char **argv) { APP _ppl6app; return ppl6::PPL6Main(argc, argv, &_ppl6app); }
#elif defined _WIN32
	#ifdef _CONSOLE
		#define PPL6APP(APP) int main(int argc, char **argv) { APP _ppl6app; return ppl6::PPL6TKMain(argc, argv, &_ppl6app); }
	#else
		#define PPL6APP(APP) int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char * lpCmdLine, int nCmdShow) { APP _ppl6app; return ppl6::PPL6MainWin(hInstance, hPrevInstance, lpCmdLine,nCmdShow, &_ppl6app); }
	#endif
#else
	#define PPL6APP(APP) int main(int argc, char **argv) { APP _ppl6app; return ppl6::PPL6Main(argc, argv, &_ppl6app); }
#endif

namespace ppl6 {

typedef struct {
	int	argc;
	char **argv;
#ifndef __MINGW32__
#ifdef _WIN32
	HINSTANCE	hInstance;
	HINSTANCE	hPrevInstance;
#endif
#endif
	int			nCmdShow;
} PPL6APPPARAMS;

class CApplication
{
	private:
		PPL6APPPARAMS	*startparam;
		int				argc;
		char			**argv;

	public:
		CApplication();
		virtual ~CApplication();
		Cppl6Core *PPLCore;
		int Init(PPL6APPPARAMS *params);
		const char *GetArg(const char *argument);
		bool HaveArg(const char *argument);
		const char *GetArg(int num);
		int  GetArgc();
		const char **GetArgv();
		PPL6APPPARAMS	*GetSystemParameter();

		virtual int Main();
};


class CWikiParser
{
	private:
		int ispre;
		int ullevel;
		int ollevel;
		int indexcount;
		int intable;
		int inrow;
		int indentlevel;
		bool doxyparamsStarted;
		bool indexenabled;

		CAssocArray index;
		CString incol;
		CString ret;
		CString BaseURI;
		CArray nowiki;
		int nowikicount;
		bool nobr;

		int precount;
		CAssocArray pre;
		int sourcecount;
		CArray source;

		CArray diagrams;


		void init();
		int renderInternal(const CString &Source, CString &Html);
		void extractNoWiki(CString &Text);
		void extractSourcecode(CString &Text);
		void extractDiagrams(CString &Text);
		void parseHeadlines(CString &Line);
		int parseUL(CString &Line);
		int parseOL(CString &Line);
		int parseIndent(CString &Line);

		void parseDoxygen(CString &Line);
		void doxygenChapter(CString &Line, const CString &Name);
		void parseAutoPRE(CString &Line);
		void parseTable(CString &Line);
		void parseLinks(CString &Line);
		void buildIndex(CString &Html);
		void finalize();
		void finalizeNoWiki();
		void finalizePRE();
		void finalizeSource();
		void finalizeDiagrams();

	protected:
		virtual void customParseLinks(CString &Line);

	public:

		CWikiParser();
		virtual ~CWikiParser();
		int render(const CString &Source, CString &Html);
		int renderBody(const CString &Source, CString &Html);
		CString render(const CString &Source);
		CString renderBody(const CString &Source);
		CString header();
		void setIndexEnabled(bool enabled);
		void setBaseURI(const CString &Uri);
		static CString xmlDiagram2HTML(const CString &xml);
		virtual int getHeader(CString &Html);
};

class PerlHelper
{
	public:
		static CString escapeString(const CString &s);
		static CString escapeRegExp(const CString &s);
		static CString toHash(const CAssocArray &a, const CString &name);

};

class PythonHelper
{
	public:
		static CString escapeString(const CString &s);
		static CString escapeRegExp(const CString &s);
		static CString toHash(const CAssocArray &a, const CString &name, int indention=0);

};

};	// EOF namespace ppl6


#endif	// #ifndef _PPL6_INCLUDE
