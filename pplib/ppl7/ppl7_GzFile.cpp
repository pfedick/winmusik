/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 7 (PPL7).
 * Web: http://www.pfp.de/ppl/
 *
 *
 *******************************************************************************
 * Copyright (c) 2015, Patrick Fedick <patrick@pfp.de>
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "prolog_ppl7.h"
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
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
#ifdef HAVE_SYS_FILE_H
	#include <sys/file.h>
#endif
#ifdef HAVE_STDARG_H
	#include <stdarg.h>
#endif
#ifdef HAVE_ERRNO_H
	#include <errno.h>
#endif

#ifdef HAVE_LIBZ
#include <zlib.h>
#endif

#ifdef _WIN32
#include <io.h>
#define WIN32_LEAN_AND_MEAN		// Keine MFCs

#include <windows.h>
#ifdef DeleteFile
#undef DeleteFile
#endif

#ifdef CopyFile
#undef CopyFile
#endif


#ifdef MoveFile
#undef MoveFile
#endif

#endif
#include "ppl7.h"

namespace ppl7 {

/*!\class GzFile
 * \ingroup PPLGroupFileIO
 * \brief Zugriff auf eine mit gzip komprimierte Datei
 *
 * \header \#include <ppl7.h>
 * \desc
 * Mit dieser Klasse können mit gzip-komprimierte Dateien geladen, verändert und
 * gespeichert werden. Sie dient als Wrapper-Klasse für die Methoden aus der zlib-Bibliothek.
 *
 */


/*!\brief Konstruktor der Klasse
 *
 * \desc
 * Konstruktor der Klasse
 */
GzFile::GzFile ()
{
	ff=NULL;
}

/*!\brief Konstruktor der Klasse mit gleichzeitigem Öffnen einer Datei
 *
 * Konstruktor der Klasse, mit dem gleichzeitig eine Datei geöffnet wird.
 * @param[in] filename Name der zu öffnenden Datei
 * @param[in] mode Zugriffsmodus. Defaultmäßig wird die Datei zum binären Lesen
 * geöffnet (siehe \ref ppl7_File_Filemodi)
 */
GzFile::GzFile (const String &filename, File::FileMode mode)
{
	ff=NULL;
	open (filename,mode);
}

/*!\brief Konstruktor mit Übernahme eines C-Filehandles
 *
 * \desc
 * Konstruktor der Klasse mit Übernahme eines C-Filehandles einer bereits mit ::fopen geöffneten Datei.
 *
 * @param[in] handle File-Handle
 */
GzFile::GzFile (int fd)
{
	ff=NULL;
	open(fd);
}

/*!\brief Destruktor der Klasse
 *
 * \desc
 * Der Destruktor der Klasse sorgt dafür, dass eine noch geöffnete Datei geschlossen wird und
 * alle Systemresourcen wieder freigegeben werden.
 */

GzFile::~GzFile()
{
	if (ff!=NULL) {
		close();
	}
}

/*!\brief %Exception anhand errno-Variable werfen
 *
 * \desc
 * Diese Funktion wird intern verwendet, um nach Auftreten eines Fehlers, anhand der globalen
 * "errno"-Variablen die passende Exception zu werfen.
 *
 * @param e Errorcode aus der errno-Variablen
 * @param filename Dateiname, bei der der Fehler aufgetreten ist
 */
void GzFile::throwErrno(int e,const String &filename)
{
	throwExceptionFromErrno(e,filename);
}

/*!\brief Exception anhand errno-Variable werfen
 *
 * \desc
 * Diese Funktion wird intern verwendet, um nach Auftreten eines Fehlers, anhand der globalen
 * "errno"-Variablen die passende Exception zu werfen.
 *
 * @param e Errorcode aus der errno-Variablen
 */
void GzFile::throwErrno(int e)
{
	throwExceptionFromErrno(e,filename());
}

/*!\brief Datei öffnen
 *
 * \desc
 * Mit dieser Funktion wird eine Datei zum Lesen, Schreiben oder beides geöffnet.
 * @param[in] filename Dateiname
 * @param mode Zugriffsmodus
 *
 * \return Kein Rückgabeparameter, im Fehlerfall wirft die Funktion eine Exception
 */
void GzFile::open (const String &filename, File::FileMode mode)
{
	close();
	// fopen stuerzt ab, wenn filename leer ist
	if (filename.isEmpty()) throw IllegalArgumentException();
	if ((ff=gzopen((const char*)filename,File::fmode(mode)))==NULL) {
		throwErrno(errno,filename);
	}
	seek(0);
	setFilename(filename);
}

/*!\brief Datei zum Lesen oder Schreiben öffnen
 *
 * \desc
 * Mit dieser Funktion wird eine Datei zum Lesen, Schreiben oder beides geöffnet.
 *
 * \param filename Dateiname als C-String
 * \param mode String, der angibt, wie die Datei geöffnet werden soll (siehe \ref ppl7_File_Filemodi)
 *
 * \return Kein Rückgabeparameter, im Fehlerfall wirft die Funktion eine Exception
 */
void GzFile::open (const char * filename, File::FileMode mode)
{
	if (filename==NULL || strlen(filename)==0) throw IllegalArgumentException();
	close();
	if ((ff=gzopen(filename,File::fmode(mode)))==NULL) {
		throwErrno(errno,filename);
	}
	seek(0);
	setFilename(filename);
}

/*
 *!\brief Bereits geöffnete Datei übernehmen
 *
 * Mit dieser Funktion kann eine mit der C-Funktion \c fopen bereits geöffnete Datei
 * übernommen werden.
 *
 * @param[in] handle Das Filehandle
 * @return Kein Rückgabeparameter, im Fehlerfall wirft die Funktion eine Exception
 */
void GzFile::open (int fd, File::FileMode mode)
{
	if (fd==0) throw IllegalArgumentException();
	close();
	if ((ff=gzdopen(fd,File::fmode(mode)))==NULL) {
		throwErrno(errno,"FILE");
	}
	seek(0);
	setFilename("FILE");
}


/*!\brief Datei schließen
 *
 * \desc
 * Diese Funktion schließt die aktuell geöffnete Datei. Sie wird automatisch vom Destruktor der
 * Klasse aufgerufen, so dass ihr expliziter Aufruf nicht erforderlich ist.
 * \par
 * Wenn  der  Stream  zur  Ausgabe  eingerichtet  war,  werden  gepufferte  Daten  zuerst  durch
 * FileObject::flush
 * geschrieben. Der zugeordnete Datei-Deskriptor wird geschlossen, alle Systemressourcen werden
 * freigegeben.
 *
 * \return Kein Rückgabeparameter, im Fehlerfall wirft die Funktion eine Exception
 */
void GzFile::close()
{
	setFilename("");
	if (ff!=NULL) {
		if (buffer!=NULL) {
			free (buffer);
			buffer=NULL;
		}
		int ret=gzclose ((gzFile)ff);
		ff=NULL;
		if (ret==Z_OK) return;
		else if (ret==Z_ERRNO) throwErrno(errno,filename());
		else if (ret==Z_MEM_ERROR) throw ppl7::OutOfMemoryException();
		throw ppl7::CompressionFailedException();
	}
}

bool GzFile::isOpen() const
{
	if (ff!=NULL) return true;
	return false;
}

void GzFile::rewind ()
{
	if (ff!=NULL) {
		gzrewind((gzFile)ff);
		return;
	}
	throw FileNotOpenException();
}

void GzFile::seek(uint64_t position)
{
	seek(position,SEEKSET);
}


uint64_t GzFile::seek (int64_t offset, SeekOrigin origin )
{
	if (ff==NULL) throw FileNotOpenException();
	int o=0;
	switch (origin) {
		case File::SEEKCUR:
			o=SEEK_CUR;
			break;
		case File::SEEKSET:
			o=SEEK_SET;
			break;
		case File::SEEKEND:
			throw ppl7::UnsupportedFeatureException("GzFile::SEEKEND");
		default:
			throw IllegalArgumentException();
	}
	int suberr=::gzseek((gzFile)ff,(long)offset,o);
	if (suberr>=0) {
		return tell();
	}
	throwErrno(errno,filename());
	return 0;
}

uint64_t GzFile::tell()
{
	if (ff!=NULL) {
		return (uint64_t) gztell((gzFile)ff);
	}
	throw FileNotOpenException();
}

bool GzFile::eof() const
{
	if (ff==NULL) throw FileNotOpenException();
	if (gzeof((gzFile)ff)!=0) return true;
	return false;
}

size_t GzFile::fread(void * ptr, size_t size, size_t nmemb)
{
	if (ff==NULL) throw FileNotOpenException();
	if (ptr==NULL) throw IllegalArgumentException();
	int by=::gzread((gzFile)ff,ptr,(unsigned int)(size*nmemb));
	if (by>0) return by;
	if (by==0) throw ppl7::EndOfFileException();
	int err=0;
	const char *msg=gzerror((gzFile)ff,&err);
	throw ppl7::CompressionFailedException("gzread: %s [%d]",msg,err);
}

char * GzFile::fgets (char *buffer, size_t num)
{
	if (ff==NULL) throw FileNotOpenException();
	if (buffer==NULL) throw IllegalArgumentException();
	//int suberr;
	char *res;
	res=::gzgets((gzFile)ff,buffer, (int)num);
	if (res==NULL) {
		//suberr=::ferror((FILE*)ff);
		if (gzeof((gzFile)ff)) throw ppl7::EndOfFileException();
		else throwErrno(errno,filename());
	}
	return buffer;
}

int GzFile::fgetc()
{
	if (ff==NULL) throw FileNotOpenException();
	int ret=gzgetc((gzFile)ff);
	if (ret!=-1) {
		return ret;
	}
	throw ppl7::EndOfFileException();
}

#ifdef TODO
size_t File::fwrite(const void * ptr, size_t size, size_t nmemb)
{
	if (ff==NULL) throw FileNotOpenException();
	if (ptr==NULL) throw IllegalArgumentException();
	size_t by=::fwrite(ptr,size,nmemb,(FILE*)ff);
	pos+=(by*size);
	if (pos>this->mysize) this->mysize=pos;
	if (by<nmemb) throwErrno(errno,filename());
	return by;
}


void File::fputs (const char *str)
{
	if (ff==NULL) throw FileNotOpenException();
	if (str==NULL) throw IllegalArgumentException();
	if (::fputs(str,(FILE*)ff)!=EOF) {
		pos+=strlen(str);
		if (pos>mysize) mysize=pos;
		return;
	}
	throwErrno(errno,filename());
}

void File::fputc(int c)
{
	if (ff==NULL) throw FileNotOpenException();
	int	ret=::fputc(c,(FILE*)ff);
	if (ret!=EOF) {
		pos++;
		if (pos>mysize) mysize=pos;
		return;
	}
	throwErrno(errno);
}

void File::sync()
{
	if (ff==NULL) throw FileNotOpenException();
#ifdef HAVE_FSYNC
	int ret=fsync(fileno((FILE*)ff));
	if (ret==0) return;
	throwErrno(errno);
#else
	throw UnsupportedFeatureException("ppl7::File::sync: No fsync available");
#endif
}

void File::truncate(uint64_t length)
{
	if (ff==NULL) throw FileNotOpenException();
#ifdef HAVE_FTRUNCATE
	int fd=fileno((FILE*)ff);
	int ret=::ftruncate(fd,(off_t)length);
	if (ret==0) {
		mysize=length;
		if (pos>mysize) seek(mysize);
		return;
	}
	throwErrno(errno);
#else
	throw UnsupportedFeatureException("ppl7::File::truncate: No ftruncate available");
#endif
}


void File::lockExclusive(bool block)
{
	if (ff==NULL) throw FileNotOpenException();
#if defined HAVE_FCNTL
	int fd=fileno((FILE*)ff);
	int cmd=F_SETLK;
	if (block) cmd=F_SETLKW;
	struct flock f;
	f.l_start=0;
	f.l_len=0;
	f.l_whence=0;
	f.l_pid=getpid();
	f.l_type=F_WRLCK;
	int ret=fcntl(fd,cmd,&f);
	if (ret!=-1) return;
	throwErrno(errno);
#elif defined HAVE_FLOCK
	int fd=fileno((FILE*)ff);
	int flags=LOCK_EX;
	if (!block) flags|=LOCK_NB;
	int ret=flock(fd,flags);
	if (ret==0) return;
	throwErrno(errno);
#else
	throw UnsupportedFeatureException("ppl7::File::unlock: No file locking available");
#endif
}

void File::lockShared(bool block)
{
	if (ff==NULL) throw FileNotOpenException();
#if defined HAVE_FCNTL
	int fd=fileno((FILE*)ff);
	int cmd=F_SETLK;
	if (block) cmd=F_SETLKW;
	struct flock f;
	f.l_start=0;
	f.l_len=0;
	f.l_whence=0;
	f.l_pid=getpid();
	f.l_type=F_RDLCK;
	int ret=fcntl(fd,cmd,&f);
	if (ret!=-1) return;
	throwErrno(errno);

#elif defined HAVE_FLOCK
	int fd=fileno((FILE*)ff);
	int flags=LOCK_SH;
	if (!block) flags|=LOCK_NB;
	int ret=flock(fd,flags);
	if (ret==0) return;
	throwErrno(errno);
#else
	throw UnsupportedFeatureException("ppl7::File::unlock: No file locking available");

#endif
}

void File::unlock()
{
	if (ff==NULL) throw FileNotOpenException();
#if defined HAVE_FCNTL
	int fd=fileno((FILE*)ff);
	struct flock f;
	f.l_start=0;
	f.l_len=0;
	f.l_whence=0;
	f.l_pid=getpid();
	f.l_type=F_UNLCK;
	int ret=fcntl(fd,F_SETLKW,&f);
	if (ret!=-1) return;
	throwErrno(errno);

#elif defined HAVE_FLOCK
	int fd=fileno((FILE*)ff);
	int ret=flock(fd,LOCK_UN);
	if (ret==0) return;
	throwErrno(errno);
#else
	throw UnsupportedFeatureException("ppl7::File::unlock: No file locking available");
#endif
}





// ####################################################################
// Statische Funktionen
// ####################################################################


#endif //TODO

} // end of namespace ppl7
