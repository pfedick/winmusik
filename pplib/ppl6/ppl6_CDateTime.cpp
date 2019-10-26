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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
       The glibc version of struct tm has additional fields
              long tm_gmtoff;           // Seconds east of UTC
              const char *tm_zone;      // Timezone abbreviation

       defined  when _BSD_SOURCE was set before including <time.h>.  This is a
       BSD extension, present in 4.3BSD-Reno.
*/
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#ifdef MINGW32
#define _POSIX_THREAD_SAFE_FUNCTIONS
#endif
#include <time.h>
#ifdef HAVE_SYS_TIME_H
	#include <sys/time.h>
#endif
#ifdef HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#include "ppl6.h"


namespace ppl6 {

#ifndef HAVE_LOCALTIME_R
// In diesem Fall ist ein Workaround für localtime_r in core/time.cpp definiert
struct tm * localtime_r(const time_t *clock, struct tm *result);
#endif

#ifndef HAVE_GMTIME_R
struct tm *gmtime_r(const time_t *timep, struct tm *result);
#endif


/*!\class CDateTime
 * \ingroup PPLGroupDataTypes
 * \brief Datenobjekt zum Speichern von Datum und Uhrzeit
 *
 * \desc
 * Dies ist eine Klasse zum Speichern von Datum und Uhrzeit. Mit den Funktionen
 * \ref CDateTime::set(const ppl6::CString &datetime) "set", \ref CDateTime::setDate "setDate"
 * und \ref CDateTime::setTime "setTime" können Datum und/oder Uhrzeit gesetzt werden,
 * mit \ref CDateTime::get "get", \ref CDateTime::getDate "getDate",
 * \ref CDateTime::getTime "getTime" und \ref CDateTime::getISO8601 "getISO8601" kann der Wert ausgelesen werden.
 * Alternativ kann mit \ref CDateTime::setTime_t "setTime_t" und \ref CDateTime::time_t "time_t"
 * ein Unix-Timestamp gesetzt oder gelesen werden (Sekunden seit 1970),
 * oder mit \ref CDateTime::setLongInt "setLongInt" und \ref CDateTime::longInt "longInt"
 * ein 64-Bit Wert gesetzt oder gelesen werden, in dem die einzelnen Bestandteile bitweise kodiert sind.
 *
 * \since
 * Die Klasse wurde mit Version 6.4.1 eingeführt.
 */


/*!\var CDateTime::yy
 * \brief Jahr
 */

/*!\var CDateTime::mm
 * \brief Monat
 */

/*!\var CDateTime::dd
 * \brief Tag
 */

/*!\var CDateTime::hh
 * \brief Stunden
 */

/*!\var CDateTime::ii
 * \brief Minuten
 */

/*!\var CDateTime::ss
 * \brief Sekunden
 */

/*!\var CDateTime::us
 * \brief Mikrosekunden
 */


//!\name Konstruktoren

/*!\brief Konstruktor mit Initialisierung auf 0
 *
 * \desc
 * Mit diesem Konstruktor ohne Parameter wird der Wert der Datumsklasse auf 0 gesetzt. Die Funktion
 * CDateTime::isEmpty "isEmpty" würde \c true zurückliefern.
 */
CDateTime::CDateTime()
{
	type=CVar::CDATETIME;
	clear();
}


/*!\brief Konstruktor mit Datumsinitialisierung aus einem String
 *
 * \desc
 * Über diesen Konstruktor wird ein CDateTime Objekt anhand des im String \p datetime enthaltenen
 * Datums und Uhrzeit erstellt. Die unterstützten Formate sind in der Funktion
 * \ref CDateTime::set(const ppl6::CString &datetime) "set" beschrieben.
 *
 * @param[in] datetime String mit Datum und Uhrzeit
 *
 * \exception IllegalArgumentException: Wird geworfen, wenn der String \p datetime
 * ein ungültiges oder unbekanntes Datumsformat hat.
 * Ausnahmen: Ist der String leer oder enthält nur den
 * Buchstaben "T" oder den Wert "0" wird keine Exception geworfen, sondern der Datumswert auf 0 gesetzt.
 */
CDateTime::CDateTime(const ppl6::CString &datetime)
{
	type=CVar::CDATETIME;
	if (!set(datetime)) throw IllegalArgumentException();
}

/*!\brief Copy-Konstruktor
 *
 * \desc
 * Über diesen Konstruktor wird das Datum eines anderen CDateTime-Wertes übernommen.
 *
 * @param[in] other Referenz auf den zu kopierenden CDateTime-Wert
 */
CDateTime::CDateTime(const ppl6::CDateTime &other)
{
	yy=other.yy;
	us=other.us;
	mm=other.mm;
	dd=other.dd;
	hh=other.hh;
	ii=other.ii;
	ss=other.ss;
}

/*!\brief Konstruktor mit Angabe von Unix-Timestamp
 *
 * \desc
 * Mit dieser Konstruktor werden Datum und Uhrzeit aus einem Unix-Timestamp übernommen (Sekunden seit 1970),
 * wie ihn Beispielsweise die C-Funktion "time()" zurückliefert. Es ist daher nicht möglich ein Datum vor
 * 1970 zu setzen.
 *
 * @param t 64-Bit Integer mit den Sekunden seit 1970.
 */
CDateTime::CDateTime(ppluint64 t)
{
	setTime_t(t);
}

//@}


//!\name Datum setzen

/*!\brief Datum auf 0 setzen
 *
 * \desc
 * Mit dieser Funktion wird der Datumswert der Klasse auf 0 gesetzt. Die Funktion
 * \ref CDateTime::isEmpty "isEmpty" würde \c true zurückliefern. Die Klasse wird somit
 * wieder in den Ausgangszustand versetzt.
 */
void CDateTime::clear()
{
	yy=0;
	us=0;
	mm=0;
	dd=0;
	hh=0;
	ii=0;
	ss=0;
}


/*!\brief Datum anhand eines Strings setzen
 *
 * \desc
 * Mit dieser Funktion wird das Datum anhand des Strings \p datetime gesetzt. Dabei versucht die Funktion
 * anhand mehrerer Regular Expressions zu erkennen, in welchem Format die Datumsangabe vorliegt. Es werden
 * folgende Formate erkannt:
 * - yyyy-mm-dd hh:ii:ss[.mms]
 * - yyyy.mm.dd hh:ii:ss[.mms]
 * - dd-mm-yyyy hh:ii:ss[.mms]
 * - dd.mm.yyyy hh:ii:ss[.mms]
 * - yyyy-mm-ddThh:ii:ss[.mms]+oo:oo (ISO 8601-Format)
 * - yyyy-mm-dd
 * - yyyy.mm.dd
 * - dd-mm-yyyy
 * - dd.mm.yyyy
 * - T: wird als Leerstring interpretiert und setzt das Datum auf 0
 * - Leerstring: setzt das Datum auf 0
 * \par Legende:
 * - yyyy: 4-Stellige Jahreszahl. Muss zwingend 4-stellig sein, da sonst nicht erkannt wird ob die Jahreszahl
 *   an erster oder dritter Stelle steht. Jahreszahlen < 1000 müssen daher mit führenden Nullen aufgefüllt
 *   werden, z.B. "0500" statt "500". Es werden keine negativen Jahreszahlen unterstützt.
 * - mm: Monatszahl zwischen 1 und 12. Kann ein- oder zweistellig sein
 * - dd: Monatstag zwischen 1 und 31, kann ein- oder zweistellig sein.
 * - hh: Stunden zwischen 0 und 23, kann ein- oder zweistellig sein
 * - ii: Minuten zwischen 0 und 59, kann ein- oder zweistellig sein
 * - ss: Sekunden zwischen 0 und 59, kann ein- oder zweistellig sein
 * - mms: Millisekunden oder Mikrosekunden: Ein dreistelliger Wert wird als Millisekunden
 *   interpretiert, ein sechstelliger Wert als Mikrosekunden. Wert ist optional und kann statt mit einem Punkt
 *   auch mit einem Doppelpunkt von den Sekunden der Uhrzeit getrennt sein.
 * \par
 * Bei der Datumsangabe kann als Trennzeichen wahlweise Punkt oder Minus verwendet werden. Es muss mindestens ein
 * vollständiges Datum angegeben werden und optional eine vollständige Uhrzeit (hh:ii:ss), wobei die Millisekunden
 * optional sind.
 *
 * @param[in] datetime String mit dem zu setzenden Datum und optional der Uhrzeit
 * @return Wurde der String erfolgreich erkannt, liefert die Funktion 1 zurück, andernfalls 0. Ferner wird der
 * Fehlercode 558 gesetzt.
 */
int CDateTime::set(const ppl6::CString &datetime)
{
	ppl6::CString d=ppl6::UCase(ppl6::Trim(datetime));
	ppl6::CArray m;
	d.Replace(","," ");
	if (d.IsEmpty()==true || d=="T" || d=="0") {
		clear();
		return 1;
	}
	if (d.PregMatch("/^([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})T([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})[\\.:]([0-9]{3})([0-9]{3})/",&m)) {
		// yyyy-mm-ddThh:ii:ss.msecusec[[+-]oo:00]
		set(m.GetString(1).ToInt(),
				m.GetString(2).ToInt(),
				m.GetString(3).ToInt(),
				m.GetString(4).ToInt(),
				m.GetString(5).ToInt(),
				m.GetString(6).ToInt(),
				m.GetString(7).ToInt(),
				m.GetString(8).ToInt());
	} else if (d.PregMatch("/^([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})T([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})[\\.:]([0-9]{1,3})/",&m)) {
		// yyyy-mm-ddThh:ii:ss.msec[[+-]oo:00]
		set(m.GetString(1).ToInt(),
				m.GetString(2).ToInt(),
				m.GetString(3).ToInt(),
				m.GetString(4).ToInt(),
				m.GetString(5).ToInt(),
				m.GetString(6).ToInt(),
				m.GetString(7).ToInt());

	} else if (d.PregMatch("/^([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})T([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})/",&m)) {
		// yyyy-mm-ddThh:ii:ss[[+-]oo:00]
		set(m.GetString(1).ToInt(),
				m.GetString(2).ToInt(),
				m.GetString(3).ToInt(),
				m.GetString(4).ToInt(),
				m.GetString(5).ToInt(),
				m.GetString(6).ToInt());
	} else if (d.PregMatch("/^([0-9]{4})[\\.-]([0-9]{1,2})[\\.-]([0-9]{1,2})\\s+([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})[\\.:]([0-9]{3})([0-9]{3})$/",&m)) {
		// yyyy.mm.dd hh:ii:ss.msecusec
		set(m.GetString(1).ToInt(),
				m.GetString(2).ToInt(),
				m.GetString(3).ToInt(),
				m.GetString(4).ToInt(),
				m.GetString(5).ToInt(),
				m.GetString(6).ToInt(),
				m.GetString(7).ToInt(),
				m.GetString(8).ToInt());
	} else if (d.PregMatch("/^([0-9]{4})[\\.-]([0-9]{1,2})[\\.-]([0-9]{1,2})\\s+([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})[\\.:]([0-9]{1,3})$/",&m)) {
		// yyyy.mm.dd hh:ii:ss.msec
		set(m.GetString(1).ToInt(),
				m.GetString(2).ToInt(),
				m.GetString(3).ToInt(),
				m.GetString(4).ToInt(),
				m.GetString(5).ToInt(),
				m.GetString(6).ToInt(),
				m.GetString(7).ToInt());
	} else if (d.PregMatch("/^([0-9]{1,2})[\\.-]([0-9]{1,2})[\\.-]([0-9]{4})\\s+([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})[\\.:]([0-9]{3})([0-9]{3})$/",&m)) {
		// dd.mm.yyyy hh:ii:ss.msecusec
		set(m.GetString(3).ToInt(),
				m.GetString(2).ToInt(),
				m.GetString(1).ToInt(),
				m.GetString(4).ToInt(),
				m.GetString(5).ToInt(),
				m.GetString(6).ToInt(),
				m.GetString(7).ToInt(),
				m.GetString(8).ToInt());
	} else if (d.PregMatch("/^([0-9]{1,2})[\\.-]([0-9]{1,2})[\\.-]([0-9]{4})\\s+([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})[\\.:]([0-9]{1,3})$/",&m)) {
		// dd.mm.yyyy hh:ii:ss.msec
		set(m.GetString(3).ToInt(),
				m.GetString(2).ToInt(),
				m.GetString(1).ToInt(),
				m.GetString(4).ToInt(),
				m.GetString(5).ToInt(),
				m.GetString(6).ToInt(),
				m.GetString(7).ToInt());
	} else if (d.PregMatch("/^([0-9]{4})[\\.-]([0-9]{1,2})[\\.-]([0-9]{1,2})\\s+([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})$/",&m)) {
		// yyyy.mm.dd hh:ii:ss
		set(m.GetString(1).ToInt(),
				m.GetString(2).ToInt(),
				m.GetString(3).ToInt(),
				m.GetString(4).ToInt(),
				m.GetString(5).ToInt(),
				m.GetString(6).ToInt());
	} else if (d.PregMatch("/^([0-9]{1,2})[\\.-]([0-9]{1,2})[\\.-]([0-9]{4})\\s+([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})$/",&m)) {
		// dd.mm.yyyy hh:ii:ss
		set(m.GetString(3).ToInt(),
				m.GetString(2).ToInt(),
				m.GetString(1).ToInt(),
				m.GetString(4).ToInt(),
				m.GetString(5).ToInt(),
				m.GetString(6).ToInt());
	} else if (d.PregMatch("/^([0-9]{1,2})[\\.-]([0-9]{1,2})[\\.-]([0-9]{4})$/",&m)) {
		// dd.mm.yyyy
		set(m.GetString(3).ToInt(),
				m.GetString(2).ToInt(),
				m.GetString(1).ToInt());
	} else if (d.PregMatch("/^([0-9]{4})[\\.-]([0-9]{1,2})[\\.-]([0-9]{1,2})$/",&m)) {
		// yyyy.mm.dd
		set(m.GetString(1).ToInt(),
				m.GetString(2).ToInt(),
				m.GetString(3).ToInt());
	} else if (d.PregMatch("/^null$/i",&m)) {
		clear();
		return 1;
	} else {
		clear();
		ppl6::SetError(558,"%s",(const char*)datetime);
		return 0;
	}
	return 1;
}

/*!\brief Datum aus einer anderen CDateTime-Variablen übernehmen
 *
 * \desc
 * Mit dieser Funktion wird der Wert einer anderen CDateTime-Variablen übernommen.
 *
 * @param[in] other Referenz auf eine andere CDateTime-Variable, dessen Wert kopiert werden soll.
 */
void CDateTime::set(const ppl6::CDateTime &other)
{
	yy=other.yy;
	us=other.us;
	mm=other.mm;
	dd=other.dd;
	hh=other.hh;
	ii=other.ii;
	ss=other.ss;
}

/*!\brief Datum und Uhrzeit aus unterschiedlichen Strings importieren
 *
 * \desc
 * Mit dieser Funktion kann das Datum und die Uhrzeit aus zwei unterschiedlichen Strings
 * übernommen werden. Dazu werden beide Strings einfach mit Space getrennt hintereinander
 * gehangen und dann die \ref CDateTime::set(const ppl6::CString &datetime) "set-Funktion"
 * aufgerufen, die nur einen String-Parameter erwartet.
 *
 * @param[in] date Referenz auf den String mit dem Datum. Dieses kann folgende Formate haben:
 * - yyyy-mm-dd
 * - yyyy.mm.dd
 * - dd-mm-yyyy
 * - dd.mm.yyyy
 * - Die Jahreszahl muss 4-stellig sein, Tag und Monat können ein- oder zweistellig sein. Statt Punkt oder Minus
 *   kann auch noch Doppelpunkt oder Komma als Trennzeichen verwendet werden.
 * @param[in] time Referenz auf den String mit der Uhrzeit. Diese muss folgendes Format haben:
 * - hh:ii:ss[.mms]
 * - Stunde, Minute und Sekunde können ein- oder zweistellig sein, Statt Doppelpunkt kann auch Komma, Punkt oder
 *   Minus als Trennzeichen verwendet werden. Die
 * @return Wurden Datum und Uhrzeit erfolgreich erkannt, liefert die Funktion 1 zurück, andernfalls 0. Ferner wird der
 * Fehlercode 558 gesetzt.
 * \see
 * Eine genauere Beschreibung der Formate samt Legende ist \ref CDateTime::set(const ppl6::CString &datetime) "hier"
 * zu finden.
 */
int CDateTime::set(const ppl6::CString &date, const ppl6::CString &time)
{
	ppl6::CString d,dd=ppl6::Trim(date),tt=ppl6::Trim(time);
	dd.Replace(",",".");
	dd.Replace(":",".");
	tt.Replace(",",":");
	tt.Replace(".",":");
	tt.Replace("-",":");

	d=dd+" "+tt;
	return set(d);
}

/*!\brief Datum setzen, Uhrzeit bleibt unverändert
 *
 * \desc
 * Mit dieser Funktion wird nur das Datum der Klasse verändert, die Uhrzeit bleibt erhalten.
 *
 * @param[in] date Referenz auf den String mit dem zu setzenden Datum. Das Format wird bei der
 * \ref CDateTime::set(const ppl6::CString &date, const ppl6::CString &time) "set-Funktion" genauer beschrieben.
 * @return Wurde das Datum erfolgreich erkannt, liefert die Funktion 1 zurück, andernfalls 0. Ferner wird der
 * Fehlercode 558 gesetzt.
 */
int CDateTime::setDate(const ppl6::CString &date)
{
	ppl6::CString time=getTime();
	return set(date,time);
}

/*!\brief Uhrzeit setzen, Datum bleibt unverändert
 *
 * \desc
 * Mit dieser Funktion wird nur die Uhrzeit der Klasse verändert, das Datum bleibt erhalten.
 *
 * @param[in] time Referenz auf den String mit der zu setzenden Uhrzeit. Das Format wird bei der
 * \ref CDateTime::set(const ppl6::CString &date, const ppl6::CString &time) "set-Funktion" genauer beschrieben.
 * @return Wurde die Uhrzeit erfolgreich erkannt, liefert die Funktion 1 zurück, andernfalls 0. Ferner wird der
 * Fehlercode 558 gesetzt.
 */
int CDateTime::setTime(const ppl6::CString &time)
{
	ppl6::CString date=getDate();
	return set(date,time);
}

/*!\brief Datum und Uhrzeit anhand einzelner Integer-Wert setzen
 *
 * \desc
 * Mit dieser Funktion wird das Datum anhand einzelner Integer-Werten gesetzt.
 *
 * @param[in] year Jahreszahl zwischen 0 und 9999
 * @param[in] month Monat zwischen 1 und 12
 * @param[in] day Tag zwischen 1 und 31
 * @param[in] hour Stunde zwischen 0 und 23. Optionaler Wert, Default ist 0.
 * @param[in] minute Minute zwischen 0 und 59. Optionaler Wert, Default ist 0.
 * @param[in] sec Sekunde zwischen 0 und 59. Optionaler Wert, Default ist 0.
 * @param[in] msec Millisekunde zwischen 0 und 999. Optionaler Wert, Default ist 0.
 * @param[in] usec Mikrosekunde zwischen 0 und 999999. Optionaler Wert, Default ist 0.
 * \attention
 * Gegenwärtig werden Werte ausserhalb des Gültigkeitsbereiches abgeschnitten! Aus dem Monat 0 oder -10 würde 1
 * werden, aus 13 oder 12345 würde 12 werden. Dieses Verhalten wird sich in einer späteren Version noch ändern!
 * Geplant ist, dass bei Überlauf eines Wertes die anderen automatisch angepasst werden, so dass z.B. aus
 * dem 32.12.2010 automatisch der 01.01.2011 wird.
 * \par
 * Wird bei \p year, \p month und \p day der Wert "0" angegeben, wird der Timestamp auf 0 gesetzt.
 *
 * \note
 * Millisekunden und Mikrosekunden werden intern nach der Formel msec*1000+usec zusammengerechnet.
 * Die Werte sollten daher entweder alternativ verwendet werden oder es muss sichergestellt sein,
 * dass die Mikrosekunden den Millisekundenanteil nicht enthalten.
 */
void CDateTime::set(int year, int month, int day, int hour, int minute, int sec, int msec, int usec)
{
	if (year==0 && month==0 && day==0) {
		clear();
		return;
	}
	yy=year;
	if (year<0) yy=0;
	if (year>9999) yy=9999;
	mm=month;
	if (month<1) mm=1;
	if (month>12) mm=12;
	dd=day;
	if (day<1) dd=1;
	if (day>31) dd=31;
	hh=hour;
	if (hour<0) hh=0;
	if (hour>23) hh=23;
	ii=minute;
	if (minute<0) ii=0;
	if (minute>59) ii=59;
	ss=sec;
	if (sec<0) ss=0;
	if (sec>59) ss=59;
	if (msec<0) msec=0;
	if (msec>999) msec=999;
	if (usec<0) usec=0;
	if (usec>999999) usec=999999;
	us=msec*1000+usec;
}

/*!\brief Datum aus Unix-Timestamp übernehmen
 *
 * \desc
 * Mit dieser Funktion werden Datum und Uhrzeit aus einem Unix-Timestamp übernommen (Sekunden seit 1970),
 * wie ihn Beispielsweise die C-Funktion "time()" zurückliefert. Es ist daher nicht möglich ein Datum vor
 * 1970 zu setzen.
 *
 * @param t 64-Bit Integer mit den Sekunden seit 1970.
 */
void CDateTime::setTime_t(ppluint64 t)
{
	struct tm tt, *r;
	if (t==0) {
		clear();
		return;
	}
	::time_t tp=(::time_t)t;
	r=localtime_r(&tp,&tt);
	if (!r) throw InvalidDateException();
	ss=tt.tm_sec;
	ii=tt.tm_min;
	hh=tt.tm_hour;
	dd=tt.tm_mday;
	mm=tt.tm_mon+1;
	yy=tt.tm_year+1900;
	us=0;
}

/*!\brief Aktuelles Datum und Uhrzeit übernehmen
 *
 * \desc
 * Mit dieser Funktion wird die Variable auf das aktuelle Datum und die aktuelle Uhrzeit gesetzt.
 * Es gibt sie auch als statische Funktion \ref CDateTime::currentTime "currentTime".
 */
void CDateTime::setCurrentTime()
{
	struct tm tt, *r;
	::time_t tp=time(NULL);
	r=localtime_r(&tp,&tt);
	if (!r) throw InvalidDateException();
	ss=tt.tm_sec;
	ii=tt.tm_min;
	hh=tt.tm_hour;
	dd=tt.tm_mday;
	mm=tt.tm_mon+1;
	yy=tt.tm_year+1900;
	us=0;
#ifdef HAVE_GETTIMEOFDAY
	struct timeval tv;
	if (gettimeofday(&tv,NULL)==0) {
		us=tv.tv_usec;
	}

#endif
}



//@}

//!\name Datum auslesen


/*!\brief Datum als String im angegebenen Format zurückgeben
 *
 * \desc
 * Datum als String im angegebenen Format zurückgeben
 *
 * @param[in] format Formatierungsstring. Wird dieser nicht angegeben, wird das Datum in folgendem Format zurückgegeben:
 * "%Y-%m-%d %H:%M:%S"
 *
 * @return String mit dem Datum im gewünschten Format
 * \par Formatierung
 *  Erlaubt sind folgende Formatzeichen:
 * - \%Y: Das Jahr als 4-stellige Angabe (z.B. 2010)
 * - \%y: Das Jahr als 2-stellige Angabe ohne Jahrhundert (z.B. 10)
 * - \%m: Der Monat als zweistellige Zahl (01 bis 12)
 * - \%d: Der Tag als zweistellige Zahl (01 bis 31)
 * - \%H: Stunden als zweistellige Zahl (00 bis 23)
 * - \%M: Minuten als zweistellige Zahl (00 bis 59)
 * - \%S: Sekunden als zweistellige Zahl (00 bis 59)
 * - \%*: Millisekunden als dreistellige Zahl (000 bis 999)
 * - \%u: Mikrosekunden als sechstellige Zahl (000000 bis 999999)
 * \par
 * Falls das im Objekt enthaltene Datum > 1900 ist, können weitere Formatanweisungen verwendet werden.
 * \par
 * \copydoc strftime.dox
 *
 */
ppl6::CString CDateTime::get(const ppl6::CString &format) const
{
	ppl6::CString Tmp;
	ppl6::CString r=format;
	Tmp.Setf("%03i",(us/1000));
	r.Replace("%*",Tmp);
	Tmp.Setf("%06i",us);
	r.Replace("%u",Tmp);

	if (yy<1900) {
		Tmp.Setf("%04i",yy);
		r.Replace("%Y",Tmp);
		Tmp.Setf("%02i",yy%100);
		r.Replace("%y",Tmp);

		Tmp.Setf("%02i",mm);
		r.Replace("%m",Tmp);

		Tmp.Setf("%02i",dd);
		r.Replace("%d",Tmp);

		Tmp.Setf("%02i",hh);
		r.Replace("%H",Tmp);

		Tmp.Setf("%02i",ii);
		r.Replace("%M",Tmp);

		Tmp.Setf("%02i",ss);
		r.Replace("%S",Tmp);

		return r;
	}

	struct tm t;
	memset(&t,0,sizeof(struct tm));
	t.tm_sec=ss;
	t.tm_min=ii;
	t.tm_hour=hh;
	t.tm_mday=dd;
	t.tm_mon=mm-1;
	t.tm_year=yy-1900;
	t.tm_isdst=-1;
	mktime(&t);

	size_t size=r.Len()*2+32;
	char *b=(char*)malloc(size);
	if (!b) {
		ppl6::SetError(2);
		return r;
	}
	if (strftime(b, size,(const char*)r, &t)==0) {
		ppl6::SetError(348,"CDateTime::get(\"%s\")",(const char*)r);
		free(b);
		return r;
	}
	r.Set(b);
	free(b);
	return r;
}

/*!\brief Datum als String zurückgeben
 *
 * \desc
 * Diese Funktion ist identisch zu CDateTime::get, hat aber einen anderen Default für den optionalen
 * Formatstring.
 *
 * @param[in] format Formatierungsstring. Wird dieser nicht angegeben, wird das Datum in folgendem Format zurückgegeben:
 * "%Y-%m-%d"
 *
 * @return String mit dem Datum im gewünschten Format
 *
 * \see
 * Siehe CDateTime::get
 */
ppl6::CString CDateTime::getDate(const ppl6::CString &format) const
{
	return get(format);
}

/*!\brief Uhrzeit als String zurückgeben
 *
 * \desc
 * Diese Funktion ist identisch zu CDateTime::get, hat aber einen anderen Default für den optionalen
 * Formatstring.
 *
 * @param[in] format Formatierungsstring. Wird dieser nicht angegeben, wird die Uhrzeit in folgendem Format zurückgegeben:
 * "%H-%M-%S"
 *
 * @return String mit der Uhrzeit im gewünschten Format
 *
 * \see
 * Siehe CDateTime::get
 */
ppl6::CString CDateTime::getTime(const ppl6::CString &format) const
{
	return get(format);
}

/*!\brief Datum als String im ISO8601-Format zurückgeben
 *
 * \desc
 * Diese Funktion gibt das Datum als String im ISO8601-Format zurück, das folgenden Aufbau hat:
 * "yyyy-mm-ddThh:ii:ss+zz:zz"
 * \par
 * Der Wert "+zz:zz" gibt den Offset zu GMT in Stunden und Minuten an und kann auch negativ sein.
 * Er wird allerdings nur ergänzt, wenn das Jahr >=1900 ist und das Betriebssystem den Wert "tm_gmtoff" in
 * seiner tm-Structure hat (siehe "man ctime").
 *
 * @return String mit dem Datum im ISO8601-Format
 */
ppl6::CString CDateTime::getISO8601() const
{
	ppl6::CString r;
	r.Setf("%04i-%02i-%02iT%02i:%02i:%02i",yy,mm,dd,hh,ii,ss);

#ifdef STRUCT_TM_HAS_GMTOFF
	if (yy>=1900) {
		struct tm t;
		t.tm_sec=ss;
		t.tm_min=ii;
		t.tm_hour=hh;
		t.tm_mday=dd;
		t.tm_mon=mm-1;
		t.tm_year=yy-1900;
		t.tm_isdst=-1;
		mktime(&t);

		int s=abs((int)(t.tm_gmtoff/60));
		if (t.tm_gmtoff>=0) {
			r.Concatf("+%02i:%02i",(int)(s/60),(int)(t.tm_gmtoff%60));
		} else {
			r.Concatf("-%02i:%02i",(int)(s/60),(int)(t.tm_gmtoff%60));
		}
	}
#elif defined WIN32
	/*
	 * Leider: error: 'TIME_ZONE_INFORMATION' was not declared in this scope
	 * error: 'GetTimeZoneInformation' was not declared in this scope
	 * ...
	TIME_ZONE_INFORMATION tzi;
	memset(&tzi,0,sizeof(TIME_ZONE_INFORMATION));
	GetTimeZoneInformation(&tzi);
	printf ("Bias: %i\n",tzi.Bias);
	printf ("StandardBias: %i\n",tzi.StandardBias);
	printf ("DaylightBias: %i\n",tzi.DaylightBias);
	*/

#endif
	return r;
}

/*!\brief Datum als String im ISO8601-Format mit Millisekunden zurückgeben
 *
 * \desc
 * Diese Funktion gibt das Datum als String im ISO8601-Format mit Millisekunden zurück, das folgenden Aufbau hat:
 * "yyyy-mm-ddThh:ii:ss.xxx+zz:zz"
 * \par
 * Der Wert "xxx" stellt die Millisekunden dar.
 * \par
 * Der Wert "+zz:zz" gibt den Offset zu GMT in Stunden und Minuten an und kann auch negativ sein.
 * Er wird allerdings nur ergänzt, wenn das Jahr >=1900 ist und das Betriebssystem den Wert "tm_gmtoff" in
 * seiner tm-Structure hat (siehe "man ctime").
 *
 * @return String mit dem Datum im ISO8601-Format
 */
ppl6::CString CDateTime::getISO8601withMsec() const
{
	ppl6::CString r;
	r.Setf("%04i-%02i-%02iT%02i:%02i:%02i.%03i",yy,mm,dd,hh,ii,ss,us/1000);

#ifdef STRUCT_TM_HAS_GMTOFF
	if (yy>=1900) {
		struct tm t;
		t.tm_sec=ss;
		t.tm_min=ii;
		t.tm_hour=hh;
		t.tm_mday=dd;
		t.tm_mon=mm-1;
		t.tm_year=yy-1900;
		t.tm_isdst=-1;
		mktime(&t);

		int s=abs((int)(t.tm_gmtoff/60));
		if (t.tm_gmtoff>=0) {
			r.Concatf("+%02i:%02i",(int)(s/60),(int)(t.tm_gmtoff%60));
		} else {
			r.Concatf("-%02i:%02i",(int)(s/60),(int)(t.tm_gmtoff%60));
		}
	}
#endif
	return r;
}

/*!\brief Datum als String im ISO8601-Format mit Mikrosekunden zurückgeben
 *
 * \desc
 * Diese Funktion gibt das Datum als String im ISO8601-Format mit Mikrosekunden zurück, das folgenden Aufbau hat:
 * "yyyy-mm-ddThh:ii:ss.xxxxxx+zz:zz"
 * \par
 * Der Wert "xxxxxx" stellt die Mikrosekunden dar.
 * \par
 * Der Wert "+zz:zz" gibt den Offset zu GMT in Stunden und Minuten an und kann auch negativ sein.
 * Er wird allerdings nur ergänzt, wenn das Jahr >=1900 ist und das Betriebssystem den Wert "tm_gmtoff" in
 * seiner tm-Structure hat (siehe "man ctime").
 *
 * @return String mit dem Datum im ISO8601-Format
 */
ppl6::CString CDateTime::getISO8601withUsec() const
{
	ppl6::CString r;
	r.Setf("%04i-%02i-%02iT%02i:%02i:%02i.%06i",yy,mm,dd,hh,ii,ss,us);

#ifdef STRUCT_TM_HAS_GMTOFF
	if (yy>=1900) {
		struct tm t;
		t.tm_sec=ss;
		t.tm_min=ii;
		t.tm_hour=hh;
		t.tm_mday=dd;
		t.tm_mon=mm-1;
		t.tm_year=yy-1900;
		t.tm_isdst=-1;
		mktime(&t);

		int s=abs((int)(t.tm_gmtoff/60));
		if (t.tm_gmtoff>=0) {
			r.Concatf("+%02i:%02i",(int)(s/60),(int)(t.tm_gmtoff%60));
		} else {
			r.Concatf("-%02i:%02i",(int)(s/60),(int)(t.tm_gmtoff%60));
		}
	}
#endif
	return r;
}

/*!\brief Datum in Unix-Timestamp umrechnen
 *
 * \desc
 * Mit dieser Funktion wird das in der Variablen enthaltene Datum und Uhrzeit in einen
 * Unix-Timestamp umgerechnet (Sekunden seit 1970).
 *
 * @return Sekunden seit 1970 oder 0, wenn das Datum sich nicht umrechnen läßt, z.B. wenn das Jahr vor 1970 liegt.
 */
ppluint64 CDateTime::time_t() const
{
	if (yy<1900) return 0;
	struct tm t;
	t.tm_sec=ss;
	t.tm_min=ii;
	t.tm_hour=hh;
	t.tm_mday=dd;
	t.tm_mon=mm-1;
	t.tm_year=yy-1900;
	t.tm_isdst=-1;
	return (ppluint64)mktime(&t);
}

/*!\brief Datum aus einem 64-Bit-Integer übernehmen
 *
 * \desc
 * Mit dieser Funktion werden Datum, Uhrzeit und Millisekunden aus einem Long Integer (64 Bit) übernommen,
 * wie ihn die Funktion CDateTime::longInt zurückgibt. Der Aufbau des Integer-Wertes ist intern und kann
 * sich von Version zu Version ändern.
 *
 * @param i 64-Bit Integer
 */
void CDateTime::setLongInt(ppluint64 i)
{
	us=i%1000000;
	i=i/1000000;
	ss=i%60;
	i=i/60;
	ii=i%60;
	i=i/60;
	hh=i%24;
	i=i/24;
	dd=(i%31)+1;
	i=i/31;
	mm=(i%12)+1;
	yy=i/12;
}


/*!\brief Datum als 64-Bit-Integer auslesen
 *
 * Mit dieser Funktion werden Datum, Uhrzeit und Millisekunden als Long Integer (64 Bit) zurückgegeben,
 * wie er von der Funktion CDateTime::setLongInt eingelesen werden kann. Der Aufbau des Integer-Wertes ist intern und kann
 * sich von Version zu Version ändern.
 *
 * @return 64-Bit-Integer mit dem Timestamp
 */
ppluint64 CDateTime::longInt() const
{
	ppluint64 r=yy*12+(mm-1);
	r=r*31+(dd-1);
	r=r*24+hh;
	r=r*60+ii;
	r=r*60+ss;
	r=r*1000000+us;
	/*
	r=(ppluint64)(yy&16383)<<42;
	r|=(ppluint64)(mm&15)<<37;
	r|=(ppluint64)(dd&31)<<31;
	r|=(ppluint64)(hh&31)<<25;
	r|=(ppluint64)(ii&63)<<18;
	r|=(ppluint64)(ss&63)<<11;
	r|=(ppluint64)((us/1000)&1023);
	*/
	return r;
}



/*!\brief Das Jahr als Integer auslesen
 *
 * \desc
 * Diese Funktion gibt die Jahreszahl als Integer zurück.
 *
 * @return Integer-Wert mit dem Jahr
 */
int CDateTime::year() const
{
	return yy;
}

/*!\brief Den Monat als Integer auslesen
 *
 * \desc
 * Diese Funktion gibt den Monat als Integer zurück.
 *
 * @return Integer-Wert mit dem Monat
 */
int CDateTime::month() const
{
	return mm;
}

/*!\brief Den Tag als Integer auslesen
 *
 * \desc
 * Diese Funktion gibt den Tag als Integer zurück.
 *
 * @return Integer-Wert mit dem Tag
 */
int CDateTime::day() const
{
	return dd;
}

/*!\brief Die Stunde als Integer auslesen
 *
 * \desc
 * Diese Funktion gibt die Stunde als Integer zurück.
 *
 * @return Integer-Wert mit der Stunde
 */
int CDateTime::hour() const
{
	return hh;
}

/*!\brief Die Minute als Integer auslesen
 *
 * \desc
 * Diese Funktion gibt die Minute als Integer zurück.
 *
 * @return Integer-Wert mit der Minute
 */
int CDateTime::minute() const
{
	return ii;
}

/*!\brief Die Sekunde als Integer auslesen
 *
 * \desc
 * Diese Funktion gibt die Sekunde als Integer zurück.
 *
 * @return Integer-Wert mit der Sekunde
 */
int CDateTime::second() const
{
	return ss;
}

/*!\brief Die Millisekunden als Integer auslesen
 *
 * \desc
 * Diese Funktion gibt die Millisekunden als Integer zurück.
 *
 * @return Integer-Wert mit den Millisekunden
 */
int CDateTime::millisecond() const
{
	return us/1000;
}

/*!\brief Die Mikrosekunden als Integer auslesen
 *
 * \desc
 * Diese Funktion gibt die Mikrosekunden als Integer zurück.
 *
 * @return Integer-Wert mit den Mikrosekunden
 */
int CDateTime::microsecond() const
{
	return us;
}

/*!\brief Die Wochennummer als Integer auslesen, Berechnung nach ISO 8601
 *
 * \desc
 * Diese Funktion berechnet anhand des Datums die Wochennummer innerhalb
 * des Jahres und gibt diese als Integer zurück. Die Zählweise richtet sich
 * dabei nach ISO 8601:
 *   - Jeden Montag und nur montags beginnt eine neue Kalenderwoche.
 *   - Die erste Kalenderwoche ist diejenige, die mindestens vier Tage des neuen Jahres enthält.
 * \par
 * Diese Zählweise ist die in Europa gebräuchliche.
 *
 * \see CDateTime::week
 *
 * @return Integer-Wert mit dem Jahr
 */
int CDateTime::weekISO8601() const
{
	if (yy<1900) throw DateOutOfRangeException("year < 1900 [%i]",yy);
	struct tm t;
	t.tm_sec=0;
	t.tm_min=0;
	t.tm_hour=12;
	t.tm_mday=dd;
	t.tm_mon=mm-1;
	t.tm_year=yy-1900;
	t.tm_isdst=-1;
	::time_t clock=mktime(&t);
	gmtime_r(&clock, &t);
	char buffer[10];
	if (strftime(buffer, 10, "%V", &t)==0) {
		throw InvalidDateException();
	}
	return atoi(buffer);
}

/*!\brief Die Wochennummer als Integer auslesen
 *
 * \desc
 * Diese Funktion berechnet anhand des Datums die Wochennummer innerhalb
 * des Jahres und gibt diese als Integer zurück. Die Zählweise entspricht der in
 * den USA, Australien und vielen weiteren Ländern, in der sich die Tradition des
 * Judentums, Christentums und Islams erhalten hat. Dabei gilt folgende Regel:
 * - Jeden Sonntag beginnt eine neue Kalenderwoche
 * - Am 1. Januar beginnt stets – unabhängig vom Wochentag – die 1. Kalenderwoche
 *
 * @return Integer-Wert mit dem Jahr
 */
int CDateTime::week() const
{
	if (yy<1900) throw DateOutOfRangeException("year < 1900 [%i]",yy);
	struct tm t;
	t.tm_sec=0;
	t.tm_min=0;
	t.tm_hour=12;
	t.tm_mday=dd;
	t.tm_mon=mm-1;
	t.tm_year=yy-1900;
	t.tm_isdst=-1;
	::time_t clock=mktime(&t);
	gmtime_r(&clock, &t);
	char buffer[10];
	if (strftime(buffer, 10, "%U", &t)==0) {
		throw InvalidDateException();
	}
	return atoi(buffer);
}

//@}

//!\name Verschiedenes

/*!\brief Datum und Uhrzeit auf STDOUT ausgeben
 *
 * \desc
 * Mit dieser Funktion wird Datum und Uhrzeit auf der Konsole (STDOUT) ausgegeben. Sie ist
 * nur zu Debug-Zwecken gedacht.
 */
void CDateTime::print() const
{
	printf ("%04i-%02i-%02i %02i:%02i:%02i\n",yy,mm,dd,hh,ii,ss);
}

/*!\brief Prüft, ob ein Datum oder Uhrzeit vorhanden ist
 *
 * \desc
 * Diese Funktion liefert \c true zurück, wenn ein Datum oder Uhrzeit gesetzt ist, der Wert also \b nicht Null ist.
 * Sie ist somit das Gegenteil zu CDateTime::isEmpty.
 * @return \c true oder \c false
 *
 */
bool CDateTime::notEmpty() const
{
	if (yy>0) return 1;
	if (mm>0) return 1;
	if (dd>0) return 1;
	if (hh>0) return 1;
	if (ii>0) return 1;
	if (ss>0) return 1;
	if (us>0) return 1;
	return 0;
}

/*!\brief Prüft, ob ein Datum oder Uhrzeit vorhanden ist
 *
 * \desc
 * Diese Funktion liefert \c true zurück, wenn \b kein Datum und \b keine Uhrzeit gesetzt ist,
 * der Wert also Null ist. Sie ist somit das Gegenteil zu CDateTime::notEmpty.
 *
 * @return \c true oder \c false
 */
bool CDateTime::isEmpty() const
{
	if (yy>0) return 0;
	if (mm>0) return 0;
	if (dd>0) return 0;
	if (hh>0) return 0;
	if (ii>0) return 0;
	if (ss>0) return 0;
	if (us>0) return 0;
	return 1;
}




/*!\brief Datum auf Schaltjahr prüfen
 *
 * \desc
 * Mit dieser Funktion wird geprüft, ob es sich bei dem in der Variable gespeicherten Jahr um ein
 * Schaltjahr handelt oder nicht.
 *
 * @return Liefert \c true zurück, wenn es sich um ein Schaltjahr handelt, andernfalls \c false.
 */
bool CDateTime::isLeapYear() const
{
	return isLeapYear(yy);
}

/*!\brief Jahreszahl auf Schaltjahr prüfen
 *
 * \desc
 * Mit dieser statischen Funktion kann geprüft werden, ob es sich bei dem angegebenen Jahr \p year um ein
 * Schaltjahr handelt oder nicht.
 *
 * @param[in] year Das zu prüfende Jahr
 * @return Liefert \c true zurück, wenn es sich um ein Schaltjahr handelt, andernfalls \c false.
 */
bool CDateTime::isLeapYear(int year)
{
	if (year%4!=0) return 0;
	if (year%400==0) return 1;
	if (year%100==0) return 0;
	return 1;
}

/*!\brief Aktuelles Datum zurückgeben
 *
 * \desc
 * Diese statische Funktion liefert das aktuelle Datum und die aktuelle Uhrzeit in Form einer
 * CDateTime-Variablen zurück.
 * @return CDateTime-Variable mit dem aktuellen Datum und Uhrzeit.
 */
CDateTime CDateTime::currentTime()
{
	CDateTime d;
	d.setCurrentTime();
	return d;
}


/*!\brief Differenz in Sekunden
 *
 * \desc
 * Diese Funktion gibt die Differenz dieses CDateTime zu dem angegebenen CDateTime \p other in
 * Sekunden zurück. Liegt der Zeitpunkt von \p other vor diesem, ist der Rückgabewert negativ.
 *
 * Vor dem Vergleich werden beide Zeitwerte in UTC umgewandelt.
 * @param[in] other Zu vergleichender Zeitwert
 * @return Differenz in Sekunden
 */
pplint64 CDateTime::diffSeconds(const CDateTime &other) const
{
	pplint64 mySecs=(pplint64)time_t();
	pplint64 otherSecs=(pplint64)other.time_t();
	return otherSecs-mySecs;
}

/*!\brief Differenz in Sekunden mit Toleranz vergleichen
 *
 * \desc
 * Mit dieser Funktion wird die Differenz des Zeitwerts dieses CDateTime mit der angegebenen CDateTime \p other
 * auf Sekundenbasis berechnet und anschließend mit der angegebenen Toleranz \p tolerance verglichen.
 *
 * @param[in] other Zu vergleichender Zeitwert
 * @param[in] tolerance Optionaler Wert, der die akzeptable Toleranz beider Werte in Sekunden angibt
 * @return Sind beide Zeitwerte identisch oder liegen im Bereich der angegebenen Toleranz, gibt die Funktion
 * 1 zurück, andernfalls 0. Es wird kein Fehlercode gesetzt.
 */
int CDateTime::compareSeconds(const CDateTime &other, int tolerance) const
{
	pplint64 mySecs=(pplint64)time_t();
	pplint64 otherSecs=(pplint64)other.time_t();
	pplint64 diff=otherSecs-mySecs;
	if (diff<0) diff=mySecs-otherSecs;
	if (diff<=tolerance) return 1;
	return 0;
}
//@}



//!\name Operatoren


/*!\brief Datum aus einem String übernehmen
 *
 * \desc
 * Mit diesem Operator werden Datum und Uhrzeit aus dem String \p datetime übernommen.
 * Die unterstützten Formate sind in der Funktion
 * \ref CDateTime::set(const ppl6::CString &datetime) "set" beschrieben.
 *
 * @param[in] datetime String mit Datum und Uhrzeit
 * @return Gibt eine Referenz auf den CDateTime-Wert zurück
 *
 * \exception
 * Enthält der String \p datetime ein ungültiges oder unbekanntes Datumsformat, wird eine
 * "InvalidFormat" Exception geworfen. Ausnahmen: Ist der String leer oder enthält nur den
 * Buchstaben "T" oder den Wert "0" wird keine Exception geworfen, sondern der Datumswert auf 0 gesetzt.
 */
CDateTime& CDateTime::operator=(const ppl6::CString &datetime)
{
	if (!set(datetime)) throw InvalidDateException(datetime);
	return *this;
}

/*!\brief Datum aus einem anderen CDateTime-Wert übernehmen
 *
 * \desc
 * Mit diesem Operator wird der Wert eines anderen CDateTime-Wertes übernommen.
 *
 * @param[in] other Referenz auf den zu kopierenden CDateTime-Wert
 * @return Gibt eine Referenz auf den CDateTime-Wert zurück
 */
CDateTime& CDateTime::operator=(const ppl6::CDateTime &other)
{
	set(other);
	return *this;
}

/*!\brief Operator, der einen CString zurückliefert
 *
 * \desc
 * Dieser Operator liefert den Inhalt der Variablen als String in folgendem Format zurück:
 * "yyyy-mm-dd hh:ii:ss.micses".
 * @return Datums-String
 */
CDateTime::operator CString() const
{
	CString r;
	r.Setf("%04i-%02i-%02i %02i:%02i:%02i.%06i",yy,mm,dd,hh,ii,ss,us);
	return r;
}


/*!\brief Vergleichsoperator "kleiner": <
 *
 * \desc
 * Mit diesem Operator werden zwei CDateTime Werte miteinander verglichen. Die Funktion gibt \c true
 * zurück, wenn der erste Wert kleiner ist als der Zweite.
 *
 * @param other Der zweite Wert, mit dem der Vergleich durchgeführt werden soll
 * @return Gibt \c true zurück, wenn der erste Wert kleiner ist als der Zweite.
 */
bool CDateTime::operator<(const CDateTime &other) const
{
	if (yy<other.yy) return true;
	else if (yy>other.yy) return false;

	if (mm<other.mm) return true;
	else if (mm>other.mm) return false;

	if (dd<other.dd) return true;
	else if (dd>other.dd) return false;

	if (hh<other.hh) return true;
	else if (hh>other.hh) return false;

	if (ii<other.ii) return true;
	else if (ii>other.ii) return false;

	if (ss<other.ss) return true;
	else if (ss>other.ss) return false;

	if (us<other.us) return true;
	return false;
}

/*!\brief Vergleichsoperator "kleiner oder gleich": <=
 *
 * \desc
 * Mit diesem Operator werden zwei CDateTime Werte miteinander verglichen. Die Funktion gibt \c true
 * zurück, wenn der erste Wert kleiner oder gleich groß ist, wie der Zweite.
 *
 * @param other Der zweite Wert, mit dem der Vergleich durchgeführt werden soll
 * @return Gibt \c true zurück, wenn der erste Wert kleiner oder gleich gross ist wie der Zweite.
 */
bool CDateTime::operator<=(const CDateTime &other) const
{
	if (yy<other.yy) return true;
	else if (yy>other.yy) return false;

	if (mm<other.mm) return true;
	else if (mm>other.mm) return false;

	if (dd<other.dd) return true;
	else if (dd>other.dd) return false;

	if (hh<other.hh) return true;
	else if (hh>other.hh) return false;

	if (ii<other.ii) return true;
	else if (ii>other.ii) return false;

	if (ss<other.ss) return true;
	else if (ss>other.ss) return false;

	if (us<other.us) return true;
	else if (ss>other.ss) return false;
	return true;
}

/*!\brief Vergleichsoperator "gleich": ==
 *
 * \desc
 * Mit diesem Operator werden zwei CDateTime Werte miteinander verglichen. Die Funktion gibt \c true
 * zurück, wenn beide Werte identisch sind.
 *
 * @param other Der zweite Wert, mit dem der Vergleich durchgeführt werden soll
 * @return Gibt \c true zurück, wenn beide Werte identisch sind.
 */
bool CDateTime::operator==(const CDateTime &other) const
{
	if (yy!=other.yy) return false;
	if (mm!=other.mm) return false;
	if (dd!=other.dd) return false;
	if (hh!=other.hh) return false;
	if (ii!=other.ii) return false;
	if (ss!=other.ss) return false;
	if (us!=other.us) return false;
	return true;
}

/*!\brief Vergleichsoperator "ungleich": !=
 *
 * \desc
 * Mit diesem Operator werden zwei CDateTime Werte miteinander verglichen. Die Funktion gibt \c true
 * zurück, wenn die Werte nicht übereinstimmen.
 *
 * @param other Der zweite Wert, mit dem der Vergleich durchgeführt werden soll
 * @return Gibt \c true zurück, wenn die Werte nicht übereinstimmen.
 */
bool CDateTime::operator!=(const CDateTime &other) const
{
	if (yy!=other.yy) return true;
	if (mm!=other.mm) return true;
	if (dd!=other.dd) return true;
	if (hh!=other.hh) return true;
	if (ii!=other.ii) return true;
	if (ss!=other.ss) return true;
	if (us!=other.us) return true;
	return false;
	/*
	ppluint64 v1=longInt();
	ppluint64 v2=other.longInt();
	if (v1!=v2) return true;
	return false;
	*/
}

/*!\brief Vergleichsoperator "größer oder gleich": >=
 *
 * \desc
 * Mit diesem Operator werden zwei CDateTime Werte miteinander verglichen. Die Funktion gibt \c true
 * zurück, wenn der erste Wert größer oder gleich groß ist, wie der Zweite.
 *
 * @param other Der zweite Wert, mit dem der Vergleich durchgeführt werden soll
 * @return Gibt \c true zurück, wenn der erste Wert größer oder gleich groß ist, wie der Zweite.
 */
bool CDateTime::operator>=(const CDateTime &other) const
{
	if (yy>other.yy) return true;
	else if (yy<other.yy) return false;

	if (mm>other.mm) return true;
	else if (mm<other.mm) return false;

	if (dd>other.dd) return true;
	else if (dd<other.dd) return false;

	if (hh>other.hh) return true;
	else if (hh<other.hh) return false;

	if (ii>other.ii) return true;
	else if (ii<other.ii) return false;

	if (ss>other.ss) return true;
	else if (ss<other.ss) return false;

	if (us>other.us) return true;
	else if (us<other.us) return false;
	return true;
}

/*!\brief Vergleichsoperator "größer": >
 *
 * \desc
 * Mit diesem Operator werden zwei CDateTime Werte miteinander verglichen. Die Funktion gibt \c true
 * zurück, wenn der erste Wert größer ist als der Zweite.
 *
 * @param other Der zweite Wert, mit dem der Vergleich durchgeführt werden soll
 * @return Gibt \c true zurück, wenn der erste Wert größer ist als der Zweite.
 */
bool CDateTime::operator>(const CDateTime &other) const
{
	if (yy>other.yy) return true;
	else if (yy<other.yy) return false;

	if (mm>other.mm) return true;
	else if (mm<other.mm) return false;

	if (dd>other.dd) return true;
	else if (dd<other.dd) return false;

	if (hh>other.hh) return true;
	else if (hh<other.hh) return false;

	if (ii>other.ii) return true;
	else if (ii<other.ii) return false;

	if (ss>other.ss) return true;
	else if (ss<other.ss) return false;

	if (us>other.us) return true;
	return false;
}

std::ostream& operator<<(std::ostream& s, const CDateTime &dt)
{
	CString str=dt.get("%Y-%m-%d %H:%M:%S.%u");
	return s.write((const char*)str,str.Size());
}


//@}

}		// EOF namespace ppl6
