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
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#ifdef HAVE_FXNTL_H
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

#include "ppl7.h"

#ifdef HAVE_LIBZ
#include <zlib.h>
#endif

#ifdef HAVE_BZIP2
#include <bzlib.h>
#endif


namespace ppl7 {

/*!\class Compression
 * \ingroup PPL7_COMPRESSION
 * \brief Komprimierung und Dekomprimierung von Daten
 *
 * \descr
 * Mit dieser Klasse können Daten komprimiert und dekomprimiert werden. Zur Zeit werden zwei
 * verschiedene Komprimierungsmethoden unterstüzt:
 * - ZLib (siehe http://www.zlib.net/)
 * - BZip2 (siehe http://www.bzip.org/)
 * \par
 * Um die gewünschte Methode auszuwählen, muss diese entweder im Konstruktor übergeben werden, oder durch
 * Aufruf von Compression::Init, was den Vorteil hat, das man hier auch gleich einen Fehlercode
 * gemeldet bekommt, wenn die gewünschte Methode nicht einkompiliert ist.
 * \par
 * Anschließend können durch Aufrufe von Compress und Uncompress Daten komprimiert bzw. entpackt
 * werden.
 *
 * \example
\code
int main (int argc, char **argv)
{
	ppl6::Compression comp;
	ppl6::CBinary uncompressed;				// Wir verwenden einfach die Quelldatei des
	if (!uncompressed.Load("main.cpp")) {	// Beispiels zum Komprimieren
		ppl6::PrintError();
		return 0;
	}
	printf ("Größe der Datei unkomprimiert: %u Bytes\n",uncompressed.Size());
	// Komprimierungsmethode und Level auswählen
	if (!comp.Init(ppl6::Compression::Algo_ZLIB, ppl6::Compression::Level_High)) {
		ppl6::PrintError();
		return 0;
	}
	// Die komprimierten Daten speichern wir in einem CBinary Objekt
	ppl6::CBinary compressed;
	if (!comp.Compress(compressed,data,false)) {
		ppl6::PrintError();
		return 0;
	}
	printf ("Größe der Datei komprimiert ohne Prefix: %u Bytes\n",compressed.Size());

	// Jetzt wieder dekomprimieren
	ppl6::CBinary recovered;
	if (!comp.Uncompress(recovered,compressed,false)) {
		ppl6::PrintError();
		return 0;
	}
	printf ("Größe nach Dekomprimierung:      %u Bytes, MD5: %s\n",recovered.Size(),(char*)chk2);
	return 1;
}
\endcode

 * \section Compression_Prefix Komprimierungsprefix
 *
 * Über die Funktion Compression::UsePrefix kann eingestellt werden, ob bei der Komprimierung noch ein
 * Header vorangestellt werden soll oder nicht. Der Header hat den Vorteil, dass man ihm die Komprimierungs-
 * Methode und die Länge der ursprünglichen unkomprimierten Daten entnehmen kann. Nicht alle Variationen
 * von Compress und Uncompress unterstützen den Prefix, daher ist bei der jeweiligen Funktion vermerkt,
 * ob der Prefix beachtet wird oder nicht.
 *
 * Es gibt zwei Versionen des Headers:
 *
 * \par Version 1 Prefix
 * Bei Version 1 gibt es einen 9-Byte großen Header mit folgendem Aufbau:
 *
\verbatim
Byte 0: Kompressions-Flag (siehe oben)
        Bits 0-2: Kompressionsart
                  0=keine
                  1=Zlib
                  2=Bzip2
        Bits 3-7: unbenutzt, müssen 0 sein
Byte 1: Bytes Unkomprimiert (4 Byte)
Byte 5: Bytes Komprimiert (4 Byte)
\endverbatim
 * Der erste Wert gibt an, wieviele Bytes der Datenblock unkomprimiert benötigt, der zweite gibt an,
 * wie gross er komprimiert ist. Nach dem Header folgen dann soviele Bytes, wie in "Bytes Komprimiert"
 * angegeben ist.
 *
 * \par Version 2 Prefix
 * Die Länge des Version 2 Headers ist variabel. Er beginnt wieder mit dem Kompressionsflag, diesmal
 * ist jedoch Bit 3 gesetzt und die Bits 4-7 werden ebenfalls verwendet:
 *
\verbatim
Byte 0: Kompression-Flag
        Bits 0-2: Kompressionsart
                  0=keine
                  1=Zlib
                  2=Bzip2
        Bit 3:    Headerversion
        Bits 4-5: Bytezahl Uncompressed Value
                  0=1 Byte, 1=2 Byte, 2=3 Byte, 3=4 Byte
        Bits 6-7: Bytezahl Compressed Value
                  0=1 Byte, 1=2 Byte, 2=3 Byte, 3=4 Byte
Byte 1: Bytes Unkomprimiert (1-4 Byte)
Byte n: Bytes Komprimiert (1-4 Byte)
\endverbatim
 * Bei Version 2 folgen eine variable Anzahl von Bytes für die beiden Werte "Bytes Unkomprimiert" und
 * "Bytes Komprimiert". Wieviele Bytes das sind, ist jeweils den Bits 4-5 und 6-7 des
 * Kompressions-Flags zu entnehmen. Bei kleinen Datenblöcken, die unkomprimiert weniger als 255 Bytes
 * benötigen, schrumpft der Prefix somit von 9 auf 3 Byte im Vergleich zum Version 1 Prefix.
 *
 */


/*!\enum Compression::Algorithm
 * \brief Unterstütze Komprimierungsmethoden
 *
 * Die Klasse unterstützt folgende Komprimierungsmethoden:
 */

/*!\var Compression::Algorithm Compression::Algo_NONE
 * Keine Komprimierung. Bei Verwendung dieser Methode werden die Daten einfach nur unverändert kopiert.
 */

/*!\var Compression::Algorithm Compression::Algo_ZLIB
 * Zlib ist eine freie Programmbibliothek von Jean-Loup Gailly und Mark Adler (http://www.zlib.net/).
 * Sie verwendet wie gzip den Deflate-Algorithmus um den Datenstrom blockweise zu komprimieren.
 * Die ausgegebenen Blöcke werden durch Adler-32-Prüfsummen geschützt.
 * Das Format ist in den RFC 1950, RFC 1951 und RFC 1952 definiert und gilt quasi als defakto
 * Standard im Unix- und Netzwerkbereich.
 */

/*!\var Compression::Algorithm Compression::Algo_BZIP2
 * bzip2 ist ein frei verfügbares Komprimierungsprogramm zur verlustfreien Kompression
 * von Dateien, entwickelt von Julian Seward. Es ist frei von jeglichen patentierten
 * Algorithmen und wird unter einer BSD-ähnlichen Lizenz vertrieben.
 * Die Kompression mit bzip2 ist oft effizienter, aber meist erheblich langsamer als
 * die Kompression mit Zlib.
 */

/*!\var Compression::Algorithm Compression::Unknown
 * Wird als Defaulteinstellung beim Dekomprimieren verwendet und hat keine eigentliche
 * Funktion.
 */

/*!\enum Compression::Level
 * \brief Kompressionsrate
 *
 * Es werden verschiedene Einstellungen unterstützt, die Einfluß auf die Kompressionsrate
 * aber auch Speicherverbrauch und Geschwindigkeit haben:
 */

/*!\var Compression::Level Compression::Level_Fast
 * Niedrige Kompressionsrate, dafür aber in der Regel sehr schnell
 */

/*!\var Compression::Level Compression::Level_Normal
 * Ausgewogene Kompressionsrate und Geschwidigkeit.
 */

/*!\var Compression::Level Compression::Level_Default
 * \copydoc Compression::Level_Normal
 */

/*!\var Compression::Level Compression::Level_High
 * Hohe Kompressionsrate, dafür aber auch langsamer als die anderen Einstellungen
 */

/*!\enum Compression::Prefix
 * \brief Prefix voranstellen
 *
 * Verwendung eines Prefix, der den komprimierten Daten vorangestellt wird.
 * Siehe dazu auch \ref Compression_Prefix
 */

/*!\var Compression::Prefix Compression::Prefix_None
 * Es wird kein Prefix vorangestellt. Die Anwendung muß sich selbst darum
 * kümmern, dass die Information über Größe der komprimierten und
 * unkomprimierten Daten erhalten bleibt.
 */

/*!\var Compression::Prefix Compression::Prefix_V1
 * Es wird ein 9-Byte langer Version 1 Prefix vorangestellt.
 */

/*!\var Compression::Prefix Compression::Prefix_V2
 * Es wird ein Version 2 Prefix mit variabler Länge vorangestellt.
 */


/*!\var Compression::buffer
 * \brief Interner Speicher, der nach Aufruf von Compress die komprimierten Daten enthält
 *
 * Interner Speicher, der nach Aufruf von Compress die komprimierten Daten enthält
 */

/*!\var Compression::uncbuffer
 * \brief Interner Speicher, der nach Aufruf von Uncompress die entpackten Daten enthält
 *
 * Interner Speicher, der nach Aufruf von Uncompress die entpackten Daten enthält
 */

/*!\var Compression::aaa
 * \brief Enthält die durch Init oder den Konstruktor eingestellten Kompressionsmethode
 *
 * Enthält die durch Init oder den Konstruktor eingestellten Kompressionsmethode
 */

/*!\var Compression::lll
 * \brief Enthält den durch Init oder den Konstruktor eingestellten Komprimierungslevel
 *
 * Enthält den durch Init oder den Konstruktor eingestellten Komprimierungslevel
 */

/*!\var Compression::prefix
 * \brief Flag, ob und welcher Prefix beim Komprimieren vorangestellt wird
 *
 * Flag, ob und welcher Prefix beim Komprimieren vorangestellt wird
 */



Compression::Compression()
/*!\brief Konstruktor der Klasse
 *
 * \descr
 * Der parameterlose Konstruktor initialisiert die Klasse mit dem Zlib-Algorithmus und
 * dem Default-Level für die Komprimierungsrate.
 */
{
	buffer=NULL;
	uncbuffer=NULL;
	aaa=Algo_ZLIB;
	lll=Level_Default;
	prefix=Prefix_None;
}

Compression::Compression(Algorithm method, Level level)
/*!\brief Konstruktor mit Initialisierung der Komprimierungsmethode
 *
 * \descr
 * Mit diesem Konstruktor kann gleichzeitig bestimmt werden, welche Komprimierungsmethode verwendet werden soll,
 * und wie stark die Komprimierung sein soll. Hier gilt: je höher die Komprimierung, desto langsamer.
 *
 * @param method Komprimierungsmethode (siehe Compression::Algorithm)
 * @param level Komprimierungslevel (siehe Compression::Level)
 */
{
	buffer=NULL;
	uncbuffer=NULL;
	aaa=method;
	lll=level;
	prefix=Prefix_None;
}

Compression::~Compression()
/*!\brief Destruktor der Klasse
 *
 * \descr
 * Der Destruktor sorgt dafür, dass intern allokierter Speicher freigegeben wird.
 * Falls Ergebnisse aus Compress oder Uncompress Aufrufen in einem CBinary-Objekt
 * gespeichert wurden, ohne "copy"-Flag, so ist der darin enthaltene Speicher
 * ebenfalls ungültig und darf nicht mehr verwendet werden.
 */
{
	if(buffer) free(buffer);
	if(uncbuffer) free(uncbuffer);
}

void Compression::usePrefix(Prefix prefix)
/*!\brief Verwendung eines Prefix beim Komprimieren
 *
 * \descr
 * Durch Aufruf dieser Funktion kann festgelegt werden, ob beim Komprimieren
 * den komprimierten Daten ein Prefix vorangestellt wird.
 *
 * Compression::Prefix
 *
 * @param prefix Der gewünschte Prefix
 *
 * \see Compression_Prefix
 */
{
	this->prefix=prefix;
}

void Compression::init(Algorithm method, Level level)
/*!\brief Gewünschte Komprimierungsmethode einstellen
 *
 * \descr
 * Mit dieser Funktion wird eingestellt, welche Komprimierungsmethode verwendet werden soll,
 * und wie stark die Komprimierung sein soll. Hier gilt: je höher die Komprimierung, desto langsamer.
 *
 * @param method Komprimierungsmethode (siehe Compression::Algorithm)
 * @param level Komprimierungslevel (siehe Compression::Level)
 * @return Bei Erfolg liefert die Funktion 1 zurück, im Fehlerfall 0
 */
{
#ifndef HAVE_LIBZ
	if (method==Algo_ZLIB) {
		throw UnsupportedFeatureException("Zlib");
	}
#endif
#ifndef HAVE_BZIP2
	if (method==Algo_BZIP2) {
		throw UnsupportedFeatureException("Bzip2");
	}
#endif

	aaa=method;
	lll=level;
}

void Compression::doNone(void *dst, size_t *dstlen, const void *src, size_t size)
/*!\brief Keine Komprimierung verwenden
 *
 * \descr
 * Diese interne Funktion wird aufgerufen, wenn die Daten garnicht komprimiert werden sollen.
 * Sie ruft daher nun memcpy auf, um die Quelldaten von \p src nach \p dst zu kopieren.
 *
 * @param[in,out] dst Pointer auf den Speicherbereich, in dem die komprimierten Daten abgelegt werden sollen
 * @param[in,out] dstlen Pointer auf eine Variable, die bei Aufruf die Größe des Zielspeicherbereichs \p dst
 * enthält und nach erfolgreichem Aufruf Anzahl tatsächlich benötigter Bytes
 * @param[in] src Pointer auf den Speicherbereich, den komprimiert werden soll
 * @param[in] size Länge des zu komprimierenden Speicherbereichs
 * @exception BufferTooSmallException Der Puffer \p dst ist zu klein, um die komprimierten Daten aufzunehmen.
 * Der Parameter \p dstlen enthält nach Auftreten der Exception die tatsächlich benötigten Bytes.
 */
{
	if (*dstlen<size) {
		*dstlen=size;
		throw BufferTooSmallException();
	}
	memcpy(dst,src,size);
	*dstlen=size;
}

void Compression::doZlib(void *dst, size_t *dstlen, const void *src, size_t size)
/*!\brief Zlib-Komprimierung verwenden
 *
 * \descr
 * Mit dieser internen Funktion werden die Quelldaten aus \p src mit Zlib komprimiert und
 * in \p dst abgelegt.
 *
 * @param[in,out] dst Pointer auf den Speicherbereich, in dem die komprimierten Daten abgelegt werden sollen
 * @param[in,out] dstlen Pointer auf eine Variable, die bei Aufruf die Größe des Zielspeicherbereichs \p dst
 * enthält und nach erfolgreichem Aufruf Anzahl tatsächlich benötigter Bytes
 * @param[in] src Pointer auf den Speicherbereich, den komprimiert werden soll
 * @param[in] size Länge des zu komprimierenden Speicherbereichs
 * @exception UnsupportedFeatureException Zlib wird nicht unterstützt
 * @exception OutOfMemoryException Nicht genug Speicher verfügbar
 * @exception BufferTooSmallException Der Puffer \p dst ist zu klein, um die komprimierten Daten aufzunehmen.
 * Der Parameter \p dstlen enthält nach Auftreten der Exception die tatsächlich benötigten Bytes.
 * @exception CompressionFailedException Ein unerwarteter Fehler ist aufgetreten, die Daten konnten nicht komprimiert werden
 */
{
#ifndef HAVE_LIBZ
	throw UnsupportedFeatureException("Zlib");
#else
	uLongf dstlen_zlib;
	int zcomplevel;
	switch (lll) {			// Kompressionslevel festlegen
		case Level_Fast:
			zcomplevel=Z_BEST_SPEED;
			break;
		case Level_Normal:
			zcomplevel=5;
			break;
		case Level_High:
			zcomplevel=Z_BEST_COMPRESSION;
			break;
		default:
			zcomplevel=Z_DEFAULT_COMPRESSION;
			break;
	}
	dstlen_zlib=(uLongf)*dstlen;
	int res=::compress2((Bytef*)dst,(uLongf *)&dstlen_zlib,(const Bytef*)src,(uLong)size,zcomplevel);
	if (res==Z_OK) {
		*dstlen=(uint32_t)dstlen_zlib;
		return;
	} else if (res==Z_MEM_ERROR) {
		throw OutOfMemoryException();
	} else if (res==Z_BUF_ERROR) {
		*dstlen=(uint32_t)dstlen_zlib;
		throw BufferTooSmallException();
	} else if (res==Z_STREAM_ERROR) throw CompressionFailedException();
	throw CompressionFailedException();
#endif
}

void Compression::doBzip2(void *dst, size_t *dstlen, const void *src, size_t size)
/*!\brief Bzip2-Komprimierung verwenden
 *
 * \descr
 * Mit dieser internen Funktion werden die Quelldaten aus \p src mit BZip2 komprimiert und
 * in \p dst abgelegt.
 *
 * @param[in,out] dst Pointer auf den Speicherbereich, in dem die komprimierten Daten abgelegt werden sollen
 * @param[in,out] dstlen Pointer auf eine Variable, die bei Aufruf die Größe des Zielspeicherbereichs \p dst
 * enthält und nach erfolgreichem Aufruf Anzahl tatsächlich benötigter Bytes
 * @param[in] src Pointer auf den Speicherbereich, den komprimiert werden soll
 * @param[in] size Länge des zu komprimierenden Speicherbereichs
 * @exception UnsupportedFeatureException Bzip2 wird nicht unterstützt
 * @exception OutOfMemoryException Nicht genug Speicher verfügbar
 * @exception BufferTooSmallException Der Puffer \p dst ist zu klein, um die komprimierten Daten aufzunehmen.
 * Der Parameter \p dstlen enthält nach Auftreten der Exception die tatsächlich benötigten Bytes.
 * @exception CompressionFailedException Ein unerwarteter Fehler ist aufgetreten, die Daten konnten nicht komprimiert werden
 */
{
#ifndef HAVE_BZIP2
	throw UnsupportedFeatureException("Bzip2");
#else
	int zcomplevel;
	switch (lll) {
		case Level_Fast:
			zcomplevel=1;
			break;
		case Level_Normal:
			zcomplevel=5;
			break;
		case Level_High:
			zcomplevel=9;
			break;
		default:
			zcomplevel=5;
			break;
	}
	int ret=BZ2_bzBuffToBuffCompress((char*)dst,(unsigned int *)dstlen,(char*)src,
		(int)size,zcomplevel,0,30);
	if (ret==BZ_OK) {
		return;
	} else if (ret==BZ_MEM_ERROR) {
		throw OutOfMemoryException();
	} else if (ret==BZ_OUTBUFF_FULL) {
		throw BufferTooSmallException();
	}
	throw CompressionFailedException();
#endif
}

void Compression::unNone (void *dst, size_t *dstlen, const void *src, size_t srclen)
/*!\brief Speicherbereich ohne Dekompression kopieren
 *
 * \descr
 * Diese interne Funktion wird aufgerufen, wenn die zu dekomprimierenden Daten garnicht
 * komprimiert sind. Sie führt daher lediglich ein memcpy aus.
 *
 * @param[in,out] dst Pointer auf den Speicherbereich, in dem die dekomprimierten Daten abgelegt werden sollen
 * @param[in,out] dstlen Pointer auf eine Variable, die bei Aufruf die Größe des Zielspeicherbereichs \p dst
 * enthält und nach erfolgreichem Aufruf Anzahl tatsächlich benötigter Bytes
 * @param[in] src Pointer auf den Anfang des Speicherbereichs, der die komprimierten Daten enthält
 * @param[in] srclen Länge der komprimierten Daten
 * @exception BufferTooSmallException Der Puffer \p dst ist zu klein, um die komprimierten Daten aufzunehmen.
 * Der Parameter \p dstlen enthält nach Auftreten der Exception die tatsächlich benötigten Bytes.
 */
{
	if (*dstlen<srclen) {
		*dstlen=srclen;
		throw BufferTooSmallException();
	}
	memcpy(dst,src,srclen);
	*dstlen=srclen;
}

void Compression::unZlib (void *dst, size_t *dstlen, const void *src, size_t srclen)
/*!\brief Zlib-Komprimierte Daten entpacken
 *
 * \descr
 * Diese interne Funktion wird aufgerufen, wenn die zu dekomprimierenden Daten mit Zlib
 * komprimiert sind.
 *
 * @param[in,out] dst Pointer auf den Speicherbereich, in dem die dekomprimierten Daten abgelegt werden sollen
 * @param[in,out] dstlen Pointer auf eine Variable, die bei Aufruf die Größe des Zielspeicherbereichs \p dst
 * enthält und nach erfolgreichem Aufruf Anzahl tatsächlich benötigter Bytes
 * @param[in] src Pointer auf den Anfang des Speicherbereichs, der die komprimierten Daten enthält
 * @param[in] srclen Länge der komprimierten Daten
 * @exception UnsupportedFeatureException Zlib wird nicht unterstützt
 * @exception OutOfMemoryException Nicht genug Speicher verfügbar
 * @exception BufferTooSmallException Der Puffer \p dst ist zu klein, um die komprimierten Daten aufzunehmen.
 * Der Parameter \p dstlen enthält nach Auftreten der Exception die tatsächlich benötigten Bytes.
 * @exception CorruptedDataException Die zu dekomprimierenden Daten sind korrupt, unvollständig
 * oder nicht mit erwarteten Algorithmus komprimiert.
 * @exception DecompressionFailedException Ein unerwarteter Fehler ist aufgetreten, die Daten konnten nicht dekomprimiert werden
 */
{
#ifndef HAVE_LIBZ
	throw UnsupportedFeatureException("Zlib");
#else
	size_t d;
	uLongf dstlen_zlib;
	d=*dstlen;
	dstlen_zlib=(uLongf)d;
	int ret=::uncompress((Bytef*)dst,&dstlen_zlib,(const Bytef*) src,(uLong)srclen);
	if (ret==Z_OK) {
		*dstlen=(uint32_t)dstlen_zlib;
		return;
	} else if (ret==Z_MEM_ERROR) {
		throw OutOfMemoryException();
	} else if (ret==Z_BUF_ERROR) {
		*dstlen=(uint32_t)dstlen_zlib;
		throw BufferTooSmallException();
	} else if (ret==Z_DATA_ERROR) {
		throw CorruptedDataException("Z_DATA_ERROR");
	}
	throw DecompressionFailedException();
#endif
}

void Compression::unBzip2 (void *dst, size_t *dstlen, const void *src, size_t srclen)
/*!\brief Bzip2-Komprimierte Daten entpacken
 *
 * \descr
 * Diese interne Funktion wird aufgerufen, wenn die zu dekomprimierenden Daten mit Bzip2
 * komprimiert sind.
 *
 * @param[in,out] dst Pointer auf den Speicherbereich, in dem die dekomprimierten Daten abgelegt werden sollen
 * @param[in,out] dstlen Pointer auf eine Variable, die bei Aufruf die Größe des Zielspeicherbereichs \p dst
 * enthält und nach erfolgreichem Aufruf Anzahl tatsächlich benötigter Bytes
 * @param[in] src Pointer auf den Anfang des Speicherbereichs, der die komprimierten Daten enthält
 * @param[in] srclen Länge der komprimierten Daten
 * @exception UnsupportedFeatureException Bzip2 wird nicht unterstützt
 * @exception OutOfMemoryException Nicht genug Speicher verfügbar
 * @exception BufferTooSmallException Der Puffer \p dst ist zu klein, um die komprimierten Daten aufzunehmen.
 * Der Parameter \p dstlen enthält nach Auftreten der Exception die tatsächlich benötigten Bytes.
 * @exception CorruptedDataException Die zu dekomprimierenden Daten sind korrupt, unvollständig
 * oder nicht mit erwarteten Algorithmus komprimiert.
 * @exception DecompressionFailedException Ein unerwarteter Fehler ist aufgetreten, die Daten konnten nicht dekomprimiert werden
 */
{
#ifndef HAVE_BZIP2
	throw UnsupportedFeatureException("Bzip2");
#else
	int ret=BZ2_bzBuffToBuffDecompress((char*)dst,(unsigned int*)dstlen,(char*)src,(int)srclen,0,0);
	if (ret==BZ_OK) {
		return;
	} else if (ret==BZ_MEM_ERROR) {
		throw OutOfMemoryException();
	} else if (ret==BZ_OUTBUFF_FULL) {
		throw BufferTooSmallException();
	} else if (ret==BZ_DATA_ERROR || ret==BZ_DATA_ERROR_MAGIC || ret==BZ_UNEXPECTED_EOF) {
		throw CorruptedDataException();
	}
	throw DecompressionFailedException();
#endif
}





void Compression::compress(void *dst, size_t *dstlen, const void *src, size_t srclen, Algorithm a)
/*!\brief Komprimierung eines Speicherbereiches in einen anderen
 *
 * \descr
 * Mit dieser Version der Compress-Funktion wird ein Speicherbereich \p src mit einer Länge
 * von \p srclen Bytes komprimiert und das Ergebnis mit einer maximalen Länge von
 * \p dstlen Bytes ab der Speicherposition \p dst gespeichert. Der Zielspeicher \p dst
 * muss vorab allokiert worden sein und groß genug sein, um die komprimierten Daten aufzunehmen.
 * Wieviel Bytes tatsächlich verbraucht wurden, ist nach erfolgreichem Aufruf der Variablen
 * \p dstlen zu entnehmen.
 * \par
 * Diese Funktion führt nur die reine Komprimierung durch und unterstützt keinen Prefix.
 *
 * @param[in,out] dst Pointer auf den Speicherbereich, in dem die komprimierten Daten abgelegt werden sollen
 * @param[in,out] dstlen Pointer auf eine Variable, die bei Aufruf die Größe des Zielspeicherbereichs \p dst
 * enthält und nach erfolgreichem Aufruf Anzahl tatsächlich benötigter Bytes
 * @param[in] src Pointer auf den Speicherbereich, den komprimiert werden soll
 * @param[in] srclen Länge des zu komprimierenden Speicherbereichs
 * @exception NullPointerException Einer der übergebenen Parameter (\p dst, \p dstlen oder \p src) zeigt auf NULL
 * @exception UnsupportedFeatureException Der eingestellte Komprimier-Algorithmus wird nicht unterstützt
 * @exception OutOfMemoryException Nicht genug Speicher verfügbar
 * @exception BufferTooSmallException Der Puffer \p dst ist zu klein, um die komprimierten Daten aufzunehmen.
 * Der Parameter \p dstlen enthält nach Auftreten der Exception die tatsächlich benötigten Bytes.
 * @exception CompressionFailedException Ein unerwarteter Fehler ist aufgetreten, die Daten konnten nicht komprimiert werden
 *
 * \note
 * Die Funktion prüft lediglich welche Komprimierungsmethode eingestellt wurde und ruft dann eine
 * der privaten Funktionen Compression::doNone, Compression::doZlib oder Compression::doBzip2 auf.
 */
{
	if ((!src) || (!dst)) throw NullPointerException();
	if (dstlen==NULL) throw NullPointerException();
	if (a==Unknown) a=aaa;
	switch (a) {
		case Algo_NONE:
			doNone(dst,dstlen,src,srclen);
			return;
		case Algo_ZLIB:
			doZlib(dst,dstlen,src,srclen);
			return;
		case Algo_BZIP2:
			doBzip2(dst,dstlen,src,srclen);
			return;
		default: throw UnsupportedFeatureException();
	}
}

ByteArrayPtr Compression::compress(const void *ptr, size_t size)
/*!\brief Komprimierung eines Speicherbereiches
 *
 * \descr
 * Mit dieser Version der Compress-Funktion wird ein Speicherbereich \p ptr mit einer Länge
 * von \p size Bytes komprimiert und das Ergebnis als ByteArrayPtr-Objekt zurückgegeben.
 * Dieses enthält eine Referenz auf Speicherbereich der Compression-Klasse, die nur solange
 * gültig ist, wie die Compression-Klasse existiert und keine neue (De-)Komprimierung
 * durchgeführt wurde.
 * \par
 * Diese Funktion unterstützt das Prefix-Flag (siehe Compression::usePrefix).
 *
 * @param[in] ptr Pointer auf den Speicherbereich, den komprimiert werden soll
 * @param[in] size Länge des zu komprimierenden Speicherbereichs
 * @return Bei Erfolg wird ein ByteArrayPtr mit einer Referenz auf den komprimierten
 * Speicher zurückgegeben. Im Fehlerfall wird eine Exception geworfen.
 * @exception UnsupportedFeatureException Der eingestellte Komprimier-Algorithmus wird nicht unterstützt
 * @exception OutOfMemoryException Nicht genug Speicher verfügbar
 * @exception BufferTooSmallException Der intern zum komprimieren verwendete Puffer
 * ist zu klein. Sollte dieser Fall auftreten, handelt es sich um einen Bug oder die
 * zu komprimierenden Daten lassen sich nicht komprimieren.
 * @exception CompressionFailedException Ein unerwarteter Fehler ist aufgetreten, die Daten konnten nicht komprimiert werden
 *
 */
{
	if (buffer) free(buffer);
	size_t dstlen=size+64;
	buffer=malloc(dstlen+9);
	if (!buffer) throw OutOfMemoryException();
	char *tgt=(char*)buffer+9;
	compress(tgt,&dstlen,ptr,size);
	if (prefix==Prefix_None) {
		return ByteArrayPtr(tgt,dstlen);
	} else if (prefix==Prefix_V1) {
		char *prefix=(char*)buffer;
		Poke8(prefix,(aaa&7));	// Nur die unteren 3 Bits sind gültig, Rest 0
		Poke32(prefix+1,(int)size);	// Größe Unkomprimiert
		Poke32(prefix+5,(int)dstlen);// Größe Komprimiert
		return ByteArrayPtr(prefix,dstlen+9);
	} else if (prefix==Prefix_V2) {
		// Zuerst prüfen wir, wieviel Bytes wir für die jeweiligen Blöcke brauchen
		int b_unc=4, b_comp=4;
		int flag=aaa&7;					// Nur die unteren 3 Bits sind gültig, Rest 0
		flag|=8;						// Version 2-Bit setzen

		if (size<=0xff) b_unc=1;
		else if (size<=0xffff) b_unc=2;
		else if (size<=0xffffff) b_unc=3;
		if (dstlen<=0xff) b_comp=1;
		else if (dstlen<=0xffff) b_comp=2;
		else if (dstlen<=0xffffff) b_comp=3;
		int bytes=1+b_unc+b_comp;
		char *prefix=tgt-bytes;
		char *p2=prefix+1;

		// Daten unkomprimiert
		if (b_unc==1) {
			Poke8(prefix+1,(int)size);
			p2=prefix+2;
		} else if (b_unc==2) {
			Poke16(prefix+1,(int)size);
			p2=prefix+3;
			flag|=16;
		} else if (b_unc==3) {
			Poke24(prefix+1,(int)size);
			p2=prefix+4;
			flag|=32;
		} else {
			Poke32(prefix+1,(int)size);
			p2=prefix+5;
			flag|=(16+32);
		}

		// Daten komprimiert
		if (b_comp==1) {
			Poke8(p2,(int)dstlen);
		} else if (b_comp==2) {
			Poke16(p2,(int)dstlen);
			flag|=64;
		} else if (b_comp==3) {
			Poke24(p2,(int)dstlen);
			flag|=128;
		} else {
			Poke32(p2,(int)dstlen);
			flag|=(128+64);
		}
		Poke8(prefix,flag);
		/*
		printf ("DEBUG\n");
		printf ("b_unc=%d, b_comp=%d, bytes=%d, flag=%d\n", b_unc, b_comp, bytes, flag);
		printf ("size unc=%zd, size_comp=%zd\n", size, dstlen);
		*/
		return ByteArrayPtr(prefix,dstlen+bytes);
	}
	// Bis hierhin sollte es nicht kommen
	throw UnknownException();
}

ByteArrayPtr Compression::compress(const ByteArrayPtr &in)
/*!\brief Komprimierung eines Speicherbereiches
 *
 * \descr
 * Mit dieser Version der Compress-Funktion wird der von \p in referenzierte Speicherbereich
 * komprimiert und das Ergebnis als ByteArrayPtr-Objekt zurückgegeben.
 * Dieses enthält eine Referenz auf Speicherbereich der Compression-Klasse, die nur solange
 * gültig ist, wie die Compression-Klasse existiert und keine neue (De-)Komprimierung
 * durchgeführt wurde.
 * \par
 * Diese Funktion unterstützt das Prefix-Flag (siehe Compression::usePrefix).
 *
 * @param[in] ptr Pointer auf den Speicherbereich, den komprimiert werden soll
 * @param[in] size Länge des zu komprimierenden Speicherbereichs
 * @return Bei Erfolg wird ein ByteArrayPtr mit einer Referenz auf den komprimierten
 * Speicher zurückgegeben. Im Fehlerfall wird eine Exception geworfen.
 * @exception UnsupportedFeatureException Der eingestellte Komprimier-Algorithmus wird nicht unterstützt
 * @exception OutOfMemoryException Nicht genug Speicher verfügbar
 * @exception BufferTooSmallException Der intern zum komprimieren verwendete Puffer
 * ist zu klein. Sollte dieser Fall auftreten, handelt es sich um einen Bug oder die
 * zu komprimierenden Daten lassen sich nicht komprimieren.
 * @exception CompressionFailedException Ein unerwarteter Fehler ist aufgetreten, die Daten konnten nicht komprimiert werden
 *
 */
{
	return compress(in.ptr(),in.size());
}

void Compression::compress(ByteArray &out, const void *ptr, size_t size)
/*!\brief Komprimierung eines Speicherbereiches in ein ByteArray Objekt
 *
 * \descr
 * Mit dieser Version der Compress-Funktion wird ein Speicherbereich \p ptr mit einer Länge
 * von \p size Bytes komprimiert und das Ergebnis im ByteArray-Objekt \p out gespeichert.
 * Der optionale Parameter \p copy bestimmt, ob in CBinary eine Kopie der komprimierten
 * Daten abgelegt wird oder nur ein Pointer auf den internen Buffer der Compression-Klasse.
 * \par
 * Diese Funktion unterstützt das Prefix-Flag (siehe Compression::usePrefix).
 *
 * @param[out] out ByteArray-Objekt, in dem die komprimierten Daten gespeichert werden sollen
 * @param[in] ptr Pointer auf den Speicherbereich, den komprimiert werden soll
 * @param[in] size Länge des zu komprimierenden Speicherbereichs
 * @exception UnsupportedFeatureException Der eingestellte Komprimier-Algorithmus wird nicht unterstützt
 * @exception OutOfMemoryException Nicht genug Speicher verfügbar
 * @exception BufferTooSmallException Der intern zum komprimieren verwendete Puffer
 * ist zu klein. Sollte dieser Fall auftreten, handelt es sich um einen Bug oder die
 * zu komprimierenden Daten lassen sich nicht komprimieren.
 * @exception CompressionFailedException Ein unerwarteter Fehler ist aufgetreten, die Daten konnten nicht komprimiert werden
 *
 */
{
	ByteArrayPtr r=compress(ptr,size);
	out.copy(r);
}

void Compression::compress(ByteArray &out, const ByteArrayPtr &in)
/*!\brief Komprimierung eines Speicherbereichs in ein CMemory-Objekt
 *
 * \descr
 * Mit dieser Version der Compress-Funktion wird der durch \p in referenzierte
 * Speicher komprimiert und das Ergebnis in \p out gespeichert.
 * \par
 * Diese Funktion unterstützt das Prefix-Flag (siehe Compression::UsePrefix).
 *
 * @param[out] out ByteArray-Objekt, in dem die komprimierten Daten gespeichert werden sollen
 * @param[in] in Ein von ByteArray oder ByteArrayPtr abgeleitetes Objekt, das den zu
 * komprimierenden Speicherbereich repräsentiert.
 * @exception UnsupportedFeatureException Der eingestellte Komprimier-Algorithmus wird nicht unterstützt
 * @exception OutOfMemoryException Nicht genug Speicher verfügbar
 * @exception BufferTooSmallException Der intern zum komprimieren verwendete Puffer
 * ist zu klein. Sollte dieser Fall auftreten, handelt es sich um einen Bug oder die
 * zu komprimierenden Daten lassen sich nicht komprimieren.
 * @exception CompressionFailedException Ein unerwarteter Fehler ist aufgetreten, die Daten konnten nicht komprimiert werden
 *
 */
{
	compress(out,in.adr(),in.size());
}

void Compression::uncompress(void *dst, size_t *dstlen, const void *src, size_t srclen, Algorithm a)
/*!\brief Dekomprimierung eines Speicherbereiches in einen anderen
 *
 * \descr
 * Mit dieser Version der Compress-Funktion wird ein komprimierter Speicherbereich \p src mit einer Länge
 * von \p srclen Bytes dekomprimiert und das entpackte Ergebnis mit einer maximalen Länge von
 * \p dstlen Bytes ab der Speicherposition \p dst gespeichert. Der Zielspeicher \p dst
 * muss vorab allokiert worden sein und groß genug sein, um die unkomprimierten Daten aufzunehmen.
 * Wieviel Bytes tatsächlich verbraucht wurden, ist nach erfolgreichem Aufruf der Variablen
 * \p dstlen zu entnehmen.
 * \par
 * Diese Funktion führt nur die reine Dekomprimierung durch und unterstützt keinen Prefix.
 *
 * @param[in,out] dst Pointer auf den Speicherbereich, in dem die dekomprimierten Daten abgelegt werden sollen
 * @param[in,out] dstlen Pointer auf eine Variable, die bei Aufruf die Größe des Zielspeicherbereichs \p dst
 * enthält und nach erfolgreichem Aufruf Anzahl tatsächlich benötigter Bytes
 * @param[in] src Pointer auf den Anfang des Speicherbereichs, der die komprimierten Daten enthält
 * @param[in] srclen Länge der komprimierten Daten
 * @exception NullPointerException Einer der übergebenen Parameter (\p dst, \p dstlen oder \p src) zeigt auf NULL
 * @exception UnsupportedFeatureException Der eingestellte Komprimier-Algorithmus wird nicht unterstützt
 * @exception OutOfMemoryException Nicht genug Speicher verfügbar
 * @exception BufferTooSmallException Der Puffer \p dst ist zu klein, um die dekomprimierten Daten aufzunehmen.
 * Der Parameter \p dstlen enthält nach Auftreten der Exception die tatsächlich benötigten Bytes.
 * @exception DecompressionFailedException Ein unerwarteter Fehler ist aufgetreten, die Daten konnten nicht dekomprimiert werden
 *
 * \note
 * Die Funktion prüft lediglich welche Komprimierungsmethode eingestellt wurde und ruft dann eine
 * der privaten Funktionen Compression::unNone, Compression::unZlib oder Compression::unBzip2 auf.
 */
{
	if ((!src) || (!dst)) throw NullPointerException();
	if (dstlen==NULL) throw NullPointerException();
	if (a==Unknown) a=aaa;
	switch (a) {
		case Algo_NONE:
			unNone(dst,dstlen,src,srclen);
			return;
		case Algo_ZLIB:
			unZlib(dst,dstlen,src,srclen);
			return;
		case Algo_BZIP2:
			unBzip2(dst,dstlen,src,srclen);
			return;
		default:
			throw UnsupportedFeatureException();
	}
}

ByteArrayPtr Compression::uncompress(const void *ptr, size_t size)
/*!\brief Dekomprimierung eines Speicherbereichs in ein CBinary Objekt
 *
 * \descr
 * Mit dieser Version der Compress-Funktion wird der durch \p ptr angegebene
 * Speicherbereich mit einer Länge von \p size Bytes dekomprimiert und die entpackten
 * Daten als ByteArrayPtr zurückgegeben.
 * \par
 * Diese Funktion unterstützt das Prefix-Flag (siehe Compression::UsePrefix).
 *
 * @param[in] ptr Pointer auf den Beginn des zu entpackenden Speicherbereichs
 * @param[in] size Größe des komprimierten Speicherbereichs
 * @return Bei Erfolg wird ein ByteArrayPtr mit einer Referenz auf den dekomprimierten
 * Speicher zurückgegeben. Im Fehlerfall wird eine Exception geworfen.
 *
 * @exception UnsupportedFeatureException Der eingestellte Komprimier-Algorithmus wird nicht unterstützt
 * @exception OutOfMemoryException Nicht genug Speicher verfügbar
 * @exception BufferTooSmallException Der intern zum komprimieren verwendete Puffer
 * ist zu klein. Sollte dieser Fall auftreten, handelt es sich um einen Bug.
 * @exception DecompressionFailedException Ein unerwarteter Fehler ist aufgetreten, die Daten konnten nicht dekomprimiert werden
 *
 */
{
	if (uncbuffer) free(uncbuffer);
	uncbuffer=NULL;
	if (prefix==Prefix_None) {
		size_t bsize=size*3;
		while (1) {
			if (uncbuffer) free(uncbuffer);
			uncbuffer=malloc(bsize);
			if (!uncbuffer) throw OutOfMemoryException();
			// Wir prüfen, ob das Ergebnis in den Buffer passt
			size_t dstlen=bsize;
			try {
				uncompress(uncbuffer,&dstlen,ptr,size);
				return ByteArrayPtr(uncbuffer,dstlen);
			} catch (BufferTooSmallException &) {
				// Der Buffer war nicht gross genug, wir vergrößern ihn
				bsize+=size;
			} catch (...) {
				free(uncbuffer);
				uncbuffer=NULL;
				throw;
			}
		}
	} else if (prefix==Prefix_V1) {
		char *buffer=(char*)ptr;
		int flag=Peek8(buffer);
		size_t size_unc=Peek32(buffer+1);
		size_t size_comp=Peek32(buffer+5);
		//printf ("Flag: %i, unc: %u, comp: %u\n",flag,size_unc, size_comp);
		if (uncbuffer) free(uncbuffer);
		uncbuffer=malloc(size_unc);
		if (!uncbuffer) throw OutOfMemoryException();
		size_t dstlen=size_unc;
		try {
			uncompress(uncbuffer,&dstlen,buffer+9,size_comp,(Algorithm)(flag&7));
			return ByteArrayPtr(uncbuffer,dstlen);
		} catch (...) {
			free(uncbuffer);
			uncbuffer=NULL;
			throw;
		}
	} else if (prefix==Prefix_V2) {
		char *buffer=(char*)ptr;
		int flag=Peek8(buffer);
		Algorithm a=(Algorithm)(flag&7);
		if ((flag&8)==0) {	// Bit 3 muss aber gesetzt sein
			throw CorruptedDataException("wrong flag");
		}
		int b_unc=4, b_comp=4;
		if ((flag&48)==0) b_unc=1;
		else if ((flag&48)==16) b_unc=2;
		else if ((flag&48)==32) b_unc=3;
		else b_unc=4;
		if ((flag&192)==0) b_comp=1;
		else if ((flag&192)==64) b_comp=2;
		else if ((flag&192)==128) b_comp=3;
		else b_comp=4;

		size_t size_unc=0;
		if (b_unc==1) size_unc=Peek8(buffer+1);
		else if (b_unc==2) size_unc=Peek16(buffer+1);
		else if (b_unc==3) size_unc=Peek24(buffer+1);
		else size_unc=Peek32(buffer+1);

		if (uncbuffer) free(uncbuffer);
		uncbuffer=malloc(size_unc);
		if (!uncbuffer) throw OutOfMemoryException();
		size_t dstlen=size_unc;
		size_t bytes=1+b_unc+b_comp;
		/*
		printf ("b_unc=%d, b_comp=%d, bytes=%d, dstlen=%zd, size=%zd\n",
				b_unc, b_comp, bytes, dstlen,size);
		if (size==804) {
			HexDump(buffer,size);
		}
		*/
		try {
			uncompress(uncbuffer,&dstlen,buffer+bytes,size-bytes,a);
			return ByteArrayPtr(uncbuffer,dstlen);
		} catch (...) {
			free(uncbuffer);
			uncbuffer=NULL;
			throw;
		}
	}
	throw DecompressionFailedException();
}

ByteArrayPtr Compression::uncompress(const ByteArrayPtr &in)
/*!\brief Dekomprimierung eines Speicherbereichs in ein CBinary Objekt
 *
 * \descr
 * Mit dieser Version der Compress-Funktion wird der durch \p ptr angegebene
 * Speicherbereich mit einer Länge von \p size Bytes dekomprimiert und die entpackten
 * Daten als ByteArrayPtr zurückgegeben.
 * \par
 * Diese Funktion unterstützt das Prefix-Flag (siehe Compression::UsePrefix).
 *
 * @param[in] in Referenz auf den zu dekomprimierenden Speicher
 * @return Bei Erfolg wird ein ByteArrayPtr mit einer Referenz auf den dekomprimierten
 * Speicher zurückgegeben. Im Fehlerfall wird eine Exception geworfen.
 *
 * @exception UnsupportedFeatureException Der eingestellte Komprimier-Algorithmus wird nicht unterstützt
 * @exception OutOfMemoryException Nicht genug Speicher verfügbar
 * @exception BufferTooSmallException Der intern zum komprimieren verwendete Puffer
 * ist zu klein. Sollte dieser Fall auftreten, handelt es sich um einen Bug.
 * @exception DecompressionFailedException Ein unerwarteter Fehler ist aufgetreten, die Daten konnten nicht dekomprimiert werden
 *
 */
{
	return uncompress(in.ptr(),in.size());
}

void Compression::uncompress(ByteArray &out, const void *ptr, size_t size)
/*!\brief Dekomprimierung eines Speicherbereichs in ein CBinary Objekt
 *
 * \descr
 * Mit dieser Version der Compress-Funktion wird der durch \p ptr angegebene
 * Speicherbereich mit einer Länge von \p size Bytes dekomprimiert und die entpackten
 * Daten im CBinary-Objekt \p out gespeichert.
 * \par
 * Diese Funktion unterstützt das Prefix-Flag (siehe Compression::UsePrefix).
 *
 * @param[out] out CBinary-Objekt, in dem die entpackten Daten gespeichert werden sollen
 * @param[in] ptr Pointer auf den Beginn des zu entpackenden Speicherbereichs
 * @param[in] size Größe des komprimierten Speicherbereichs
 *
 * @exception UnsupportedFeatureException Der eingestellte Komprimier-Algorithmus wird nicht unterstützt
 * @exception OutOfMemoryException Nicht genug Speicher verfügbar
 * @exception BufferTooSmallException Der intern zum komprimieren verwendete Puffer
 * ist zu klein. Sollte dieser Fall auftreten, handelt es sich um einen Bug.
 * @exception DecompressionFailedException Ein unerwarteter Fehler ist aufgetreten, die Daten konnten nicht dekomprimiert werden
 *
 */
{
	ByteArrayPtr b=uncompress(ptr, size);
	out.copy(b);
}

void Compression::uncompress(ByteArray &out, const ByteArrayPtr &object)
/*!\brief Dekomprimierung eines ByteArrayPtr Objektes
 *
 * \descr
 * Mit dieser Version der Compress-Funktion wird Speicher des Objektes \p object
 * entpackt und das Ergebnis im ByteArray-Objekt \p out gespeichert.
 * \par
 * Diese Funktion unterstützt das Prefix-Flag (siehe Compression::usePrefix).
 *
 * @param[out] out ByteArray-Objekt, in dem die entpackten Daten gespeichert werden sollen
 * @param[in] object ByteArrayPtr-Objekt, das auf die komprimierten Daten zeigt.
 *
 * @exception UnsupportedFeatureException Der eingestellte Komprimier-Algorithmus wird nicht unterstützt
 * @exception OutOfMemoryException Nicht genug Speicher verfügbar
 * @exception BufferTooSmallException Der intern zum komprimieren verwendete Puffer
 * ist zu klein. Sollte dieser Fall auftreten, handelt es sich um einen Bug.
 * @exception DecompressionFailedException Ein unerwarteter Fehler ist aufgetreten, die Daten konnten nicht dekomprimiert werden
 *
 */
{
	uncompress(out,object.ptr(), object.size());
}



/*!\ingroup PPL7_COMPRESSION
 * \brief Speicherbereich komprimieren
 *
 * \desc
 * Mit dieser Funktion wird der durch \p in referenzierte Speicher
 * mit der Komprimierungsmethode \p method und dem Komprimierungslevel \p level komprimiert
 * und das Ergebnis im CMemory-Objekt \p out gespeichert.
 *
 * Speicherbereich komprimieren
 *
 * @param[out] out ByteArray-Objekt, in dem die komprimierten Daten gespeichert werden sollen
 * @param[in] in Ein von ByteArrayPtr abgeleitetes Objekt mit den zu komprimierenden Daten
 * @param[in] method Die gewünschte Komprimierungsmethode (siehe Compression::Algorithm)
 * @param[in] level Der gewünschte Komprimierungslevel (siehe Compression::Level)
 *
 * \see Compression
 * @return
 */
void Compress(ByteArray &out, const ByteArrayPtr &in, Compression::Algorithm method, Compression::Level level)
{
	Compression comp;
	comp.init(method,level);
	comp.usePrefix(Compression::Prefix_V2);
	comp.compress(out,in);
}

/*!\ingroup PPL7_COMPRESSION
 * \relatesalso Compression
 * \brief Daten dekomprimieren
 *
 * \descr
 * Mit dieser Funktion werden die in \p in enthaltenen komprimierten Daten
 * entpackt und das Ergebnis im CBinary-Objekt \p out gespeichert.
 * \par
 * Die Funktion geht davon aus, dass die komprimierten Daten mit einem
 * Version 2 Prefix beginnen (siehe \ref Compression_Prefix). Ist dies nicht der
 * Fall, sollte statt dieser Funktion die Klasse Compression verwendet werden,
 * deren Compression::Uncompress-Funktionen auch Dekomprimierung ohne Prefix
 * unterstützen.
 *
 * @param[out] out CBinary-Objekt, in dem die entpackten Daten gespeichert werden sollen
 * @param[in] in Das CBinary-Objekt, das die komprimierten Daten enthält
 * @return Bei Erfolg gibt die Funktion 1 zurück, im Fehlerfall 0
 *
 * \see Compression
 */
void Uncompress(ByteArray &out, const ByteArrayPtr &in)
{
	Compression comp;
	comp.usePrefix(Compression::Prefix_V2);
	comp.uncompress(out,in);
}


void CompressZlib(ByteArray &out, const ByteArrayPtr &in, Compression::Level level)
/*!\ingroup PPL7_COMPRESSION
 * \relatesalso Compression
 * \brief Daten mit ZLib komprimieren
 *
 * \descr
 * Mit dieser Funktion wird der durch \p in referenzierte Speicherbereich
 * mit der Komprimierungsmethode ZLib und dem Komprimierungslevel \p level komprimiert
 * und das Ergebnis im CMemory-Objekt \p out gespeichert.
 * \par
 * Die Funktion stellt den komprimierten Daten automatisch einen Version 2 Prefix voran (siehe
 * \ref Compression_Prefix), so dass die komprimierten Daten durch Aufruf der Funktion
 * Uncompress ohne Angabe der Kompressionsmethod wieder entpackt werden kann.
 *
 * @param[out] out ByteArray-Objekt, in dem die komprimierten Daten gespeichert werden sollen
 * @param[in] in Ein ByteArrayPtr-Objekt mit den zu komprimierenden Daten.
 * @param[in] level Der gewünschte Komprimierungslevel (siehe Compression::Level). Der Default ist
 * Compression::Level_High
 *
 * \see Compression
 */
{
	Compress(out,in,Compression::Algo_ZLIB,level);
}


void CompressBZip2(ByteArray &out, const ByteArrayPtr &in, Compression::Level level)
/*!\ingroup PPL7_COMPRESSION
 * \relatesalso Compression
 * \brief Daten mit BZip2 komprimieren
 *
 * \descr
 * Mit dieser Funktion wird der durch \p in referenzierte Speicherbereich
 * mit der Komprimierungsmethode BZip2 und dem Komprimierungslevel \p level komprimiert
 * und das Ergebnis im CMemory-Objekt \p out gespeichert.
 * \par
 * Die Funktion stellt den komprimierten Daten automatisch einen Version 2 Prefix voran (siehe
 * \ref Compression_Prefix), so dass die komprimierten Daten durch Aufruf der Funktion
 * Uncompress ohne Angabe der Kompressionsmethod wieder entpackt werden kann.
 *
 * @param[out] out CMemory-Objekt, in dem die komprimierten Daten gespeichert werden sollen
 * @param[in] in Ein CMemoryReference-Objekt mit den zu komprimierenden Daten.
 * @param[in] level Der gewünschte Komprimierungslevel (siehe Compression::Level). Der Default ist
 * Compression::Level_High
 * @return Bei Erfolg gibt die Funktion 1 zurück, im Fehlerfall 0. Die Länge der
 * komprimierten Daten kann \p out entnommen werden.
 *
 * \see Compression
 */
{
	Compress(out,in,Compression::Algo_BZIP2,level);
}


} // EOF namespace ppl7
