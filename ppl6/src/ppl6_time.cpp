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
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN		// Keine MFCs
#include <windows.h>
#endif
#include "ppl6.h"



namespace ppl6 {

/*
ppl_time_t time(ppl_time_t *timer)
{
#ifdef _WIN32
	return (ppl_time_t) _time64((__time64_t*)timer);
#else
	if (!timer) return (ppl_time_t) ::time(NULL);
	time_t a,b;
	a=(time_t)*timer;
	b=::time(&a);
	*timer=a;
	return b;
#endif
}
*/

#if ! defined (HAVE_LOCALTIME_R) || ! defined (HAVE_GMTIME_R)
static CMutex LocalTimeMutex;
#endif

#ifndef HAVE_LOCALTIME_R
struct tm * localtime_r(const time_t *clock, struct tm *result)
{
	LocalTimeMutex.Lock();
#ifdef MINGW32
	struct tm *ttt=localtime(clock);
#elif defined _WIN32
	__time64_t a=*clock;
	struct tm *ttt=_localtime64(&a);
#else
	struct tm *ttt=localtime(clock);
#endif
	if (ttt) {
		memcpy(result,ttt,sizeof(struct tm));
		LocalTimeMutex.Unlock();
		return result;
	}
	LocalTimeMutex.Unlock();
	return NULL;
}
#endif

#ifndef HAVE_GMTIME_R
struct tm *gmtime_r(const time_t *timep, struct tm *result)
{
	LocalTimeMutex.Lock();
#ifdef MINGW32
	struct tm *ttt=gmtime(timep);
#elif defined _WIN32
	__time64_t a=*timep;
	struct tm *ttt=_gmtime64(&a);
#else
	struct tm *ttt=localtime(timep);
#endif
	if (ttt) {
		memcpy(result,ttt,sizeof(struct tm));
		LocalTimeMutex.Unlock();
		return (result);
	}
	LocalTimeMutex.Unlock();
	return (NULL);
}


#endif


int datum (char *str1)
{
	time_t now;
	struct tm tmstruct;
	time(&now);
	if (localtime_r(&now,&tmstruct)) {
		sprintf (str1,"%02d.%02d.%04d",tmstruct.tm_mday,tmstruct.tm_mon+1,tmstruct.tm_year+1900);
		return 1;
	}
	return 0;
}

/*!\ingroup PPLGroupDateTime
 * \brief Liefert die aktuelle Unixtime in einer Struktur zurück
 *
 * Liefert die aktuelle Unix-Zeit als Return-Wert zurück, sowie aufgeschlüsselt in der
 * Struktur PPLTIME.
 *
 * \param t ist ein Pointer auf eine Struktur vom Typ PPLTIME oder NULL.
 * \returns Bei Erfolg wird die Zeit seit 1.1.1970, 00:00 Uhr in Sekunden zurückgegeben,
 * sowie die Struktur PPLTIME gefüllt, sofert der Parameter \a t nicht NULL ist.
 * Tritt ein Fehler auf, wird ((ppluint64)-1) zurückgegeben und errno entsprechend gesetzt.
 *
 * \see ppl6::GetTime()
 * \see ppl6::GetTime(PPLTIME *t, ppluint64 now)
 *
 */
ppluint64 GetTime(PPLTIME *t)
{
	time_t now;
	time(&now);
	if (t) GetTime(t,now);
	return (ppluint64) now;
}

/*!\ingroup PPLGroupDateTime
 * \brief Liefert die aktuelle Unixtime in einer Struktur zurück
 *
 * Liefert die aktuelle Unix-Zeit als Return-Wert zurück, sowie aufgeschlüsselt in der
 * Struktur PPLTIME.
 *
 * \param t Referenz aif eine Struktur vom Typ PPLTIME.
 * \returns Bei Erfolg wird die Zeit seit 1.1.1970, 00:00 Uhr in Sekunden zurückgegeben,
 * sowie die Struktur PPLTIME gefüllt.
 * Tritt ein Fehler auf, wird ((ppluint64)-1) zurückgegeben und errno entsprechend gesetzt.
 *
 */
ppluint64 GetTime(PPLTIME &t)
{
	time_t now;
	time(&now);
	return GetTime(t,now);
}

/*! \fn ppl6::GetTime (PPLTIME *t, ppluint64 now)
 * \ingroup PPLGroupDateTime
 * \brief Wandelt Unix-Zeit in die Struktur PPLTIME um
 *
 * Wandelt die angegebene Unix-Zeit in eine Struktur vom Typ PPLTIME um.
 *
 * \param t ist ein Pointer auf eine Struktur vom Typ PPLTIME oder NULL.
 * \param now enthält die Sekunden seit 1970, die in die PPLTIME-Struktur umgewandelt werden
 * sollen.
 * \returns Bei Erfolg werden die über den Parameter \a now angegebenen Sekunden
 * zurückgeliefert und die Struktur PPLTIME wird gefüllt, Bei Auftreten eines Fehlers
 * wird 0 zurückgegeben.
 *
 * \see ppl6::GetTime()
 * \see ppl6::GetTime(PPLTIME *t)
 *
 */
ppluint64 GetTime(PPLTIME *t, ppluint64 now)
{
	struct tm tmstruct;
	time_t n=(time_t)now;
	if (!t) return now;
	memset(t,0,sizeof(PPLTIME));
	if (!localtime_r(&n,&tmstruct)) {
		return 0;
	}
	t->year=tmstruct.tm_year+1900;
	t->month=tmstruct.tm_mon+1;
	t->day=tmstruct.tm_mday;
	t->hour=tmstruct.tm_hour;
	t->min=tmstruct.tm_min;
	t->sec=tmstruct.tm_sec;
	t->epoch=now;
	t->day_of_week=tmstruct.tm_wday;
	t->day_of_year=tmstruct.tm_yday;
	t->summertime=tmstruct.tm_isdst;
	#ifdef STRUCT_TM_HAS_GMTOFF
		t->gmt_offset=tmstruct.tm_gmtoff;
		t->have_gmt_offset=1;
	#else
		t->gmt_offset=0;
		t->have_gmt_offset=0;
	#endif
	return now;
}

/*!\ingroup PPLGroupDateTime
 * \brief Wandelt Unix-Zeit in die Struktur PPLTIME um
 *
 * Wandelt die angegebene Unix-Zeit in eine Struktur vom Typ PPLTIME um.
 *
 * \param t Referenz auf Eine PPLTIME-Struktur
 * \param now enthält die Sekunden seit 1970, die in die PPLTIME-Struktur umgewandelt werden
 * sollen.
 * \returns Bei Erfolg werden die über den Parameter \a now angegebenen Sekunden
 * zurückgeliefert und die Struktur PPLTIME wird gefüllt, Bei Auftreten eines Fehlers
 * wird 0 zurückgegeben.
 *
 * \see ppl6::GetTime()
 * \see ppl6::GetTime(PPLTIME *t)
 *
 */
ppluint64 GetTime(PPLTIME &t, ppluint64 now)
{
	struct tm tmstruct;
	time_t n=(time_t)now;
	memset(&t,0,sizeof(PPLTIME));
	if (!localtime_r(&n,&tmstruct)) {
		return 0;
	}
	t.year=tmstruct.tm_year+1900;
	t.month=tmstruct.tm_mon+1;
	t.day=tmstruct.tm_mday;
	t.hour=tmstruct.tm_hour;
	t.min=tmstruct.tm_min;
	t.sec=tmstruct.tm_sec;
	t.epoch=now;
	t.day_of_week=tmstruct.tm_wday;
	t.day_of_year=tmstruct.tm_yday;
	t.summertime=tmstruct.tm_isdst;
	#ifdef STRUCT_TM_HAS_GMTOFF
		t.gmt_offset=tmstruct.tm_gmtoff;
		t.have_gmt_offset=1;
	#else
		t.gmt_offset=0;
		t.have_gmt_offset=0;
	#endif
	return now;
}

/*! \brief Liefert die aktuelle Unixtime zurück
 * \ingroup PPLGroupDateTime
 *
 * Diese Funktion gibt die Zeit seit Beginn der "Computer-Epoche" (00:00:00 UTC, January 1, 1970)
 * in Sekunden zurück.
 * \returns Bei Erfolg wird die Zeit seit 1.1.1970, 00:00 Uhr in Sekunden zurückgegeben.
 * Tritt ein Fehler auf, wird ((ppluint64)-1) zurückgegeben und errno entsprechend gesetzt.
 *
 * \see ppl6::GetTime(PPLTIME *t)
 * \see ppl6::GetTime(PPLTIME *t, ppluint64 now)
 *
 */
ppluint64 GetTime()
/*!\ingroup PPLGroupDateTime
 */
{
	time_t now;
	now=time(NULL);
	return (ppluint64) now;
}


ppldd longdatum ()
/*!\ingroup PPLGroupDateTime
 */

{
	char tmpc[20];
	datum (tmpc);
	return (dat2long(tmpc));
}

int usdatum (char *str1)
/*!\ingroup PPLGroupDateTime
 */
{
	time_t now;
	struct tm tmstruct;
	time(&now);
	if (!localtime_r(&now,&tmstruct)) return 0;
	sprintf (str1,"%02d/%02d/%04d",tmstruct.tm_mon+1,tmstruct.tm_mday,tmstruct.tm_year+1900);
	return 1;
}

int zeit (char *str1)
/*!\ingroup PPLGroupDateTime
 */
{
	time_t now;
	struct tm tmstruct;
	time(&now);
	if (!localtime_r(&now,&tmstruct)) return 0;
	sprintf (str1,"%02d:%02d:%02d",tmstruct.tm_hour,tmstruct.tm_min,tmstruct.tm_sec);
	return 1;
}

ppldd longzeit ()
/*!\ingroup PPLGroupDateTime
 */
{
	char tmpc[20];
	zeit (tmpc);
	return (zeit2long(tmpc));
}

ppldd zeit2long (char * zeitstr)
/*!\ingroup PPLGroupDateTime
 */
{
	CString Str=zeitstr;
	ppldd d=0;
	if (Str.PregMatch("/^([0-9]+):([0-9]+)\\.([0-9]+)$/")) {
		d=10000*atol(Str.GetMatch(1));
		d+=100*atol(Str.GetMatch(2));
		d+=atol(Str.GetMatch(3));
	}
	return d;
}

void long2zeit (char *zeitstr,ppldd z)
/*!\ingroup PPLGroupDateTime
 */
{
	char d1 [11];
	sprintf(d1,"%06u",z);
	//strxchg (d1," ","0");
	zeitstr[0]=d1[0];
	zeitstr[1]=d1[1];
	zeitstr[2]=':';
	zeitstr[3]=d1[2];
	zeitstr[4]=d1[3];
	zeitstr[5]=':';
	zeitstr[6]=d1[4];
	zeitstr[7]=d1[5];
	zeitstr[8]=0;
}

ppldd sec2long (ppldd z)
/*!\ingroup PPLGroupDateTime
 */
{
	ppldd hour,min,sec;
	hour=(z/3600);
	z-=hour*3600;
	min=(z/60);
	sec=z-min*60;
	return (hour*10000+min*100+sec);
}

ppldd dat2long (pplchar *dat)
/*!\ingroup PPLGroupDateTime
 */
{
	CString Str=dat;
	ppldd d=0;
	if (Str.PregMatch("/^([0-9]+)\\.([0-9]+)\\.([0-9]+)$/")) {
		d=10000*atol(Str.GetMatch(3));
		d+=100*atol(Str.GetMatch(2));
		d+=atol(Str.GetMatch(1));
	}
	return d;
}

void long2dat (pplchar *datstr,ppluint32 dat)
/*!\ingroup PPLGroupDateTime
 */
{
	char d1 [11];
	sprintf(d1,"%u",dat);
	datstr[0]=d1[6];
	datstr[1]=d1[7];
	datstr[2]='.';
	datstr[3]=d1[4];
	datstr[4]=d1[5];
	datstr[5]='.';
	datstr[6]=d1[0];
	datstr[7]=d1[1];
	datstr[8]=d1[2];
	datstr[9]=d1[3];
	datstr[10]=0;
}

void datumsauswertung (pplchar * d, pplchar * dat)
/*!\ingroup PPLGroupDateTime
 */
{
	CTok Tok;
	int z,l,p,m;
	char t [15], ad [11];
	const char *tokptr;
	char *strptr = t;

	strcpy (t,dat);
	datum(ad);						/* Aktuelles Datum holen                  */
	if (strlen(t)==0)				/* Ist ein Datum vorhanden?               */
		strcpy(d,ad);				/* Wenn nein, dann aktuelles Datum nehmen */
	else {
		strxchg (t,",",".");		/* Zuerst die Trennzeichen in Punkte      */
		strxchg (t,"-",".");		/* umwandeln                              */
		strxchg (t,"/",".");
		strcat  (t,"..");
		z=0;
		p=0;
		strcpy (d,"");
		Tok.Split(strptr,".");
		while (z<3) {
			tokptr = Tok.GetNext();
			if (tokptr!=NULL) {
				l=strlen(tokptr);
				if (l<2 && z<2)
					strcat (d,"0");
				else if (l<4 && z==2)
					strncat (d,&ad[p],4-l);
				strcat (d,tokptr);
				if (z<2) strcat (d,".");
			} else {
				if (z<2)
					strncat (d,&ad[p],3);
				else
					strncat (d,&ad[p],4);
			}
			p=p+3;
			z++;
		}
	}
	/* Datum auf Plausibilitaet pruefen */
	strncpy (t,d,2);
	z=atoi(t);
	strncpy (t,d+3,2);
	m=atoi(t);
	if (z<1) {
		d[0]='0';
		d[1]='1';
	}
	if (m<0) {
		d[3]='0';
		d[4]='1';
		m=1;
	}
	if (m>12) {
		d[3]='1';
		d[4]='2';
		m=12;
	}
	if ((m==1||m==3||m==5||m==7||m==8||m==10||m==12) && z>31) {
		d[0]='3';
		d[1]='1';
	}
	if ((m==4||m==6||m==9||m==11) && z>30) {
		d[0]='3';
		d[1]='0';
	}
	if (m==2 && z>28) {
		strncpy (t,d+6,4);
		m=atoi(t);
		if ((m&3)==0) {					// Ein Schaltjahr
			if (z>29) {
				d[0]='2';
				d[1]='9';
			}
		} else {						// Kein Schaltjahr
			if (z>28) {
				d[0]='2';
				d[1]='8';
			}
		}
	}
}

ppluint64 Datum2Sekunden(char *datum, char *zeit)
/*!\ingroup PPLGroupDateTime
 */
{
  int jahr,monat,tag,hour=0,min=0,sec=0;
  time_t LTime;
  struct tm Time;
  memset((char *) &Time, 0,sizeof(Time));

  sscanf (datum,"%4d%2d%2d",&jahr,&monat,&tag);
  if (zeit!=NULL) {
    sscanf (zeit,"%2d%2d%2d",&hour,&min,&sec);
  }
  monat--;
  Time.tm_mday = tag;
  Time.tm_mon  = monat;
  Time.tm_year = jahr-1900;
  Time.tm_hour = hour;
  Time.tm_min  = min;
  Time.tm_sec  = sec;

//	printf ("jahr=%u, monat=%u, tag=%u\n",jahr,monat,tag);

  LTime = mktime(&Time);
  return((ppluint64)LTime);
}

ppluint64 MkTime(CString *year, CString *month, CString *day, CString *hour, CString *min, CString *sec)
/*!\ingroup PPLGroupDateTime
 */
{
	struct tm Time;
	if ((!year) || (!month) || (!day) || (!hour) || (!min) || (!sec)) return 0;
	Time.tm_mday = day->ToInt();
	Time.tm_mon  = month->ToInt()-1;
	Time.tm_year = year->ToInt()-1900;
	Time.tm_hour = hour->ToInt();
	Time.tm_min  = min->ToInt();
	Time.tm_sec  = sec->ToInt();
	time_t LTime=mktime(&Time);
	return (ppluint64) LTime;
}

ppluint64 MkTime(const CString &year, const CString &month, const CString &day, const CString &hour, const CString &min, const CString &sec)
/*!\ingroup PPLGroupDateTime
 */
{
	struct tm Time;
	Time.tm_mday = day.ToInt();
	Time.tm_mon  = month.ToInt()-1;
	Time.tm_year = year.ToInt()-1900;
	Time.tm_hour = hour.ToInt();
	Time.tm_min  = min.ToInt();
	Time.tm_sec  = sec.ToInt();
	time_t LTime=mktime(&Time);
	return (ppluint64) LTime;
}


ppluint64 MkTime(const char *year, const char *month, const char *day, const char *hour, const char *min, const char *sec)
/*!\ingroup PPLGroupDateTime
 */
{
	struct tm Time;
	if ((!year) || (!month) || (!day) || (!hour) || (!min) || (!sec)) return 0;
	memset(&Time,0,sizeof(Time));
	Time.tm_mday = atoi(day);
	Time.tm_mon  = atoi(month)-1;
	Time.tm_year = atoi(year)-1900;
	Time.tm_hour = atoi(hour);
	Time.tm_min  = atoi(min);
	Time.tm_sec  = atoi(sec);
	time_t LTime=mktime(&Time);

	return (ppluint64) LTime;
}

ppluint64 MkTime(int year, int month, int day, int hour, int min, int sec)
/*!\ingroup PPLGroupDateTime
 */
{
	struct tm Time;
	if (year<1900 || month<1) return 0;
	memset(&Time,0,sizeof(Time));
	Time.tm_mday = day;
	Time.tm_mon  = month-1;
	Time.tm_year = year-1900;
	Time.tm_hour = hour;
	Time.tm_min  = min;
	Time.tm_sec  = sec;
	time_t LTime=mktime(&Time);
	return (ppluint64) LTime;
}


ppluint64 GetUTC(char *datum, char *zeit)
/*!\ingroup PPLGroupDateTime
 */
{
  int jahr,monat,tag,hour=0,min=0,sec=0;
  time_t LTime;
  struct tm Time;
  memset((char *) &Time, 0,sizeof(Time));

  sscanf (datum,"%2d.%2d.%4d",&tag,&monat,&jahr);
  if (zeit!=NULL) {
    sscanf (zeit,"%2d.%2d.%2d",&hour,&min,&sec);
  }
  monat--;
  Time.tm_mday = tag;
  Time.tm_mon  = monat;
  Time.tm_year = jahr-1900;
  Time.tm_hour = hour;
  Time.tm_min  = min;
  Time.tm_sec  = sec;

  //  printf ("jahr=%u, monat=%u, tag=%u\n",jahr,monat,tag);

  LTime = mktime(&Time);
  return((ppluint64)LTime);
}


static char utcdatebuff[16];

/*! \fn ppl6::UTC2Date(ppluint64 sec, char *datum, char *uhrzeit)
 *\ingroup PPLGroupDateTime
 * \brief Wandelt Unixtime in einen String um
 *
 * Die angegebene Unixtime wird in einen String gewandelt und das Ergebnis getrennt in Datum und
 * Uhrzeit zurückgegeben
 * \param sec Die Unixtime (Sekunden seit 1970)
 * \param datum Ein Pointer auf einen String, der groß genug sein muß, um ein Datum aufzunehmen (11 Byte), oder NULL
 * \param uhrzeit Ein Pointer auf einen String, der groß genug sein muß, um eine Uhrzeit aufzunehmen (9 Byte), oder NULL
 * \returns Sofern die Parameter nicht NULL sind, wird das Ergebnis in \a datum und \a uhrzeit
 * gespeichert. Das Format dabei ist:
 * - Datum: dd.mm.yyyy (Tag.Monat.Jahr)
 * - Uhrzeit; hh:mm:ss (Stunden:Minuten:Sekunden)
 *
 * Ferner wird ein Pointer auf einen String zurückgeliefert, der nur das Datum enthält
 * \warning Wenn der Parameter \p datum NULL ist, wird für den Rückgabewert ein statischer
 * Buffer verwendet, der nicht nicht Threadsafe ist!
 * \note Mit der Funktion ppl6::MkTime kann ein Datumsstring im beliebigen Format erzeugt werden.
 * \see ppl6::MkTime
 */
const char * UTC2Date(ppluint64 sec, char *datum, char *uhrzeit)
{
	struct tm tm_tmp;
	if (!localtime_r((time_t*)&sec,&tm_tmp)) return NULL;

	if (uhrzeit!=NULL) {
		strftime(uhrzeit,127,"%H.%M.%S",&tm_tmp);
	}
	if (datum!=NULL) {
		strftime(datum,127,"%d.%m.%Y",&tm_tmp);
		return datum;
	}
	strftime(utcdatebuff,127,"%d.%m.%Y",&tm_tmp);
	return utcdatebuff;
}

ppldd Sekunden2Datum(time_t sec, ppldd *datum, ppldd *uhrzeit)
/*!\ingroup PPLGroupDateTime
 */
{
	char buff[128];
	struct tm tm_tmp;
	if (!localtime_r((time_t*)&sec,&tm_tmp)) return 0;

	if (datum!=NULL) {
		strftime(buff,127,"%Y%m%d",&tm_tmp);
		*datum=atol(buff);
	}
	if (uhrzeit!=NULL) {
		strftime(buff,127,"%H%M%S",&tm_tmp);
		*uhrzeit=atol(buff);
	}
	return *datum;
}

double getmicrotime()
/*!\ingroup PPLGroupDateTime
 */
{
	double t=0.0;
	#ifdef _WIN32
		static double time_frequency=0.0;
		LARGE_INTEGER gettime;
		if (time_frequency==0) {
			QueryPerformanceFrequency(&gettime);
			time_frequency=(double)gettime.QuadPart;
		}
		QueryPerformanceCounter(&gettime);
		t=(double)gettime.QuadPart/time_frequency;
		return t;
	#else
		struct timeval tp;
		if (gettimeofday(&tp,NULL)==0) {
			t=(double)tp.tv_sec+(double)tp.tv_usec/1000000.0;
		}
		return t;
	#endif
}

double GetMicrotime()
/*!\ingroup PPLGroupDateTime
 */
{
	double t=0.0;
	#ifdef _WIN32
		static double time_frequency=0.0;
		LARGE_INTEGER gettime;
		if (time_frequency==0) {
			QueryPerformanceFrequency(&gettime);
			time_frequency=(double)gettime.QuadPart;
		}
		QueryPerformanceCounter(&gettime);
		t=(double)gettime.QuadPart/time_frequency;
		return t;
	#else
		struct timeval tp;
		if (gettimeofday(&tp,NULL)==0) {
			t=(double)tp.tv_sec+(double)tp.tv_usec/1000000.0;
		}
		return t;
	#endif
}

ppluint64 GetMilliSeconds()
/*!\ingroup PPLGroupDateTime
 * \brief Aktuelle Zeit in Millisekunden
 *
 * \desc
 * Diese Funktion liefert die Anzahl Millisekunden, die seit dem 1.1.1970 0 Uhr vergangen
 * sind. (1000 Millisekunden = 1 Sekunde).
 *
 * \return Anzahl Millisekunden seit 1970.
 *
 */
{
	ppluint64 t=0;
	#ifdef _WIN32
	static ppluint64 time_frequency=0;
	LARGE_INTEGER gettime;
	if (time_frequency==0) {
		QueryPerformanceFrequency(&gettime);
		time_frequency=(ppluint64)gettime.QuadPart;
	}
	QueryPerformanceCounter(&gettime);
	t=(ppluint64)gettime.QuadPart*1000/time_frequency;
	return t;
	#else
		struct timeval tp;
		if (gettimeofday(&tp,NULL)==0) {
			t=(ppluint64)tp.tv_sec*1000+(ppluint64)(tp.tv_usec/1000);
		}
		return t;
	#endif
}




int USleep(ppluint64 microseconds)
/*!\ingroup PPLGroupDateTime
 */

{		// 1 sec = 1000000 microseconds
	#ifdef _WIN32
		Sleep(DWORD((microseconds+999)/1000));
		return 1;
	#elif defined HAVE_USLEEP
		if (usleep(microseconds)==0) return 1;
		return 0;
	#endif
	return 0;

}

int MSleep(ppluint64 milliseconds)
/*!\ingroup PPLGroupDateTime
 */
{		// 1 sec = 1000 milliseconds
	#ifdef _WIN32
		Sleep((DWORD)milliseconds);
		return 1;
	#elif defined HAVE_USLEEP
		if (usleep(1000*milliseconds)==0) return 1;
		return 0;
	#endif
	return 0;
}

int SSleep(ppluint64 seconds)
/*!\ingroup PPLGroupDateTime
 */
{
	#ifdef _WIN32
		Sleep((DWORD)seconds*1000);
		return 1;
	#elif defined HAVE_USLEEP
		if (usleep(1000000*seconds)==0) return 1;
		return 0;
	#else
		sleep(seconds);
	#endif
	return 0;
}


const char *MkISO8601Date (CString &buffer, ppluint64 sec)
/*!\ingroup PPLGroupDateTime
 */
{
	PPLTIME tt;
	GetTime(&tt,sec);
	return MkISO8601Date(buffer,&tt);
}

const char *MkISO8601Date (CString &buffer, PPLTIME *t)
/*!\ingroup PPLGroupDateTime
 */
{
	PPLTIME tt;
	if (!t) {
		memset(&tt,0,sizeof(PPLTIME));
		t=&tt;
		GetTime(t);
	}
	char offset[11];
	if (t->have_gmt_offset) {
		int off=abs(t->gmt_offset)/60;
		int h=(off/60);
		int m=(off%60);
		if (t->gmt_offset<0) sprintf(offset,"-%02i:%02i",h,m);
		else sprintf(offset,"+%02i:%02i",h,m);
	} else {
		if (t->summertime) sprintf(offset,"+01:00");
		else sprintf(offset,"+02:00");
	}
	// "2005-01-20T17:03:45+01:00"
	buffer.Setf("%04i-%02i-%02iT%02i:%02i:%02i%s",
		t->year, t->month, t->day, t->hour, t->min, t->sec, offset);
	return (const char*)buffer;
}

char *MkISO8601Date (char *buffer, size_t size, ppluint64 sec)
/*!\ingroup PPLGroupDateTime
 */
{
	CString b;
	if (!size) {
		SetError(194);
		return NULL;
	}
	MkISO8601Date(b,sec);
	if (size>b.Size()) size=b.Size();
	strncpy(buffer,(const char*)b,size);
	buffer[size-1]=0;
	return buffer;
}

char *MkISO8601Date (char *buffer,size_t size, PPLTIME *t)
/*!\ingroup PPLGroupDateTime
 */
{
	CString b;
	if (!size) {
		SetError(194);
		return NULL;
	}
	MkISO8601Date(b,t);
	if (size>b.Size()) size=b.Size();
	strncpy(buffer,(const char*)b,size);
	buffer[size-1]=0;
	return buffer;
}

CString MkISO8601Date (ppluint64 sec)
/*!\ingroup PPLGroupDateTime
 */
{
	CString s;
	PPLTIME tt;
	GetTime(&tt,sec);
	MkISO8601Date(s,&tt);
	return s;
}

CString MkISO8601Date (PPLTIME *t)
/*!\ingroup PPLGroupDateTime
 */
{
	CString s;
	MkISO8601Date(s,t);
	return s;
}

/*
 * date-time   =  [ day "," ] date time        ; dd mm yy
                                                 ;  hh:mm:ss zzz

     day         =  "Mon"  / "Tue" /  "Wed"  / "Thu"
                 /  "Fri"  / "Sat" /  "Sun"

     date        =  1*2DIGIT month 2DIGIT        ; day month year
                                                 ;  e.g. 20 Jun 82

     month       =  "Jan"  /  "Feb" /  "Mar"  /  "Apr"
                 /  "May"  /  "Jun" /  "Jul"  /  "Aug"
                 /  "Sep"  /  "Oct" /  "Nov"  /  "Dec"

     time        =  hour zone                    ; ANSI and Military

     hour        =  2DIGIT ":" 2DIGIT [":" 2DIGIT]
                                                 ; 00:00:00 - 23:59:59

     zone        =  "UT"  / "GMT"                ; Universal Time
                                                 ; North American : UT
                 /  "EST" / "EDT"                ;  Eastern:  - 5/ - 4
                 /  "CST" / "CDT"                ;  Central:  - 6/ - 5
                 /  "MST" / "MDT"                ;  Mountain: - 7/ - 6
                 /  "PST" / "PDT"                ;  Pacific:  - 8/ - 7
                 /  1ALPHA                       ; Military: Z = UT;
                                                 ;  A:-1; (J not used)
                                                 ;  M:-12; N:+1; Y:+12
                 / ( ("+" / "-") 4DIGIT )        ; Local differential
                                                 ;  hours+min. (HHMM)
 *
 */


CString MkRFC822Date (PPLTIME &t)
/*!\ingroup PPLGroupDateTime
 * \brief Datumstring nach RFC-822 (Mailformat) erzeugen
 *
 * \desc
 * Mit dieser Funktion wird ein Datummstring nach RFC-822 erzeugt, wie er im Header einer Email verwendet wird.
 * Das Format lautet:
 * \code
 * weekday, day month year time zone
 * \endcode
 * und hat folgende Bedeutung:
 * - weekday: Name des Wochentags ("Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat")
 * - day: Tag des Monats mit ein oder zwei Ziffern
 * - month: Name des Monats ("Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec")
 * - year: Das Jahr mit 4 Ziffern
 * - time: Stunde:Minute:Sekunde (hh:mm:ss), jeweils mit zwei Ziffern und Doppelpunkt getrennt
 * - zone: Offset zu UTC in Stunden und Minuten (+|-HHMM)
 *
 * \param[in] t Eine PPLTIME-Struktur, der die Datumsinformationen entnommen werden
 *
 * \exception Exception::FunctionFailed Die Funktion wirft eine Exception, wenn die Datumsinformation in der PPLTIME-Struktur ungültig ist.
 */
{
	CString s;
	const char *day[]={ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	const char *month[]={ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	// PPLTIME prüfen
	if (t.day_of_week<0 || t.day_of_week>6) throw IllegalArgumentException("MkRFC822Date: week<0 order week>6");
	if (t.month<1 || t.month>12) throw IllegalArgumentException("MkRFC822Date: month<0 order month>12");

	s=day[t.day_of_week];
	s+=", ";
	s.Concatf("%i ",t.day);
	s+=month[t.month-1];
	s.Concatf(" %04i %02i:%02i:%02i ",t.year,t.hour,t.min,t.sec);
	if (t.have_gmt_offset) {
		if (t.gmt_offset>=0) s.Concatf("+%02i%02i",abs(t.gmt_offset/3600),abs(t.gmt_offset%3600));
		else s.Concatf("-%02i%02i",abs(t.gmt_offset/3600),abs(t.gmt_offset%3600));
	}
	return s;
}

CString MkRFC822Date (ppluint64 sec)
/*!\ingroup PPLGroupDateTime
 * \brief Datumstring nach RFC-822 (Mailformat) erzeugen
 *
 * \desc
 * Mit dieser Funktion wird ein Datummstring nach RFC-822 erzeugt, wie er im Header einer Email verwendet wird.
 * Das Format lautet:
 * \code
 * weekday, day month year time zone
 * \endcode
 * und hat folgende Bedeutung:
 * - weekday: Name des Wochentags ("Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat")
 * - day: Tag des Monats mit ein oder zwei Ziffern
 * - month: Name des Monats ("Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec")
 * - year: Das Jahr mit 4 Ziffern
 * - time: Stunde:Minute:Sekunde (hh:mm:ss), jeweils mit zwei Ziffern und Doppelpunkt getrennt
 * - zone: Offset zu UTC in Stunden und Minuten (+|-HHMM)
 *
 * \param[in] sec Ein Optionaler Parameter mit Sekunden seit 1970. Ist er 0 oder wird nicht angegeben, wird die
 * aktuelle Zeit verwendet.
 *
 * \exception Exception::FunctionFailed Die Funktion wirft eine Exception, wenn die Datumsinformation in der PPLTIME-Struktur ungültig ist.
 */
{
	PPLTIME t;
	memset(&t,0,sizeof(t));
	if (!sec) sec=GetTime();
	if (GetTime(t,sec)!=sec) throw OperationFailedException();
	return MkRFC822Date(t);
	/*
	if (localtime_r(&now,&tmstruct)) {
		char buffer[60];
		if (strftime(buffer,60,"%a, %d %b %Y %H:%M:%S %z",&tmstruct)) {
			return buffer;
		}
	}
	throw Exception::FunctionFailed();
	*/
}


ppluint64 MkTime(const char *iso8601date, PPLTIME *t)
/*!\ingroup PPLGroupDateTime
 */
{
	if (!iso8601date) {
		SetError(194,"const char *iso8601date");
		return 0;
	}
	CString str=iso8601date;
	return MkTime(&str,t);
}

ppluint64 MkTime(CString *iso8601date, PPLTIME *t)
/*!\ingroup PPLGroupDateTime
 */
{
	if (!iso8601date) {
		SetError(194,"CString *iso8601date");
		return 0;
	}
	if (!iso8601date->PregMatch("/^([0-9]{4})-([0-9]{2})-([0-9]{2})T([0-9]{2}):([0-9]{2}):([0-9]{2})\\+([0-9]{2}):([0-9]{2})$/i")) {
		SetError(326);
		return 0;
	}
	struct tm Time;
	memset(&Time,0,sizeof(Time));
	Time.tm_mday = atoi(iso8601date->GetMatch(3));
	Time.tm_mon  = atoi(iso8601date->GetMatch(2))-1;
	Time.tm_year = atoi(iso8601date->GetMatch(1))-1900;
	Time.tm_hour = atoi(iso8601date->GetMatch(4));
	Time.tm_min  = atoi(iso8601date->GetMatch(5));
	Time.tm_sec  = atoi(iso8601date->GetMatch(6));
	time_t LTime=mktime(&Time);
	if (t) GetTime(t,(ppluint64)LTime);
	return (ppluint64) LTime;
}

/*!\brief Datum/Zeit formatieren
 * \ingroup PPLGroupDateTime
 *
 * \header \#include <ppl6.h>
 * \desc
 * Die Funktion MkDate wandelt einen Unix-Timestamp in einen String um.
 *
 * \param buffer
 * \param format ist ein beliebiger String, der verschiedene  Platzhalter
 * entahlten darf (siehe unten)
 * \param sec
 *
 * \par Syntax-Formatstring
 * \copydoc strftime.dox
 */
const char *MkDate (CString &buffer, const char *format, ppluint64 sec)
{
	if (!format) {
		SetError(194,"const char *format");
		return NULL;
	}
	size_t size=strlen(format)*2+32;
	char *b=(char*)malloc(size);
	if (!b) {
		SetError(2);
		return NULL;
	}
	char *ret=MkDate(b,size,format,sec);
	if (ret) {
		buffer.Set(b);
	} else {
		free(b);
		return NULL;
	}
	free(b);
	return buffer;
}

/*!\brief Datum/Zeit formatieren
 * \ingroup PPLGroupDateTime
 *
 * \header \#include <ppl6.h>
 * \desc
 * Die Funktion MkDate wandelt einen Unix-Timestamp in einen String um.
 *
 * \param format ist ein beliebiger String, der verschiedene  Platzhalter
 * entahlten darf (siehe unten)
 * \param sec
 * \return Bei Erfolg gibt die Funktion einen neuen String mit dem formatierten
 * Zeitpunkt zurück, im Fehlerfall einen leeren String.
 *
 * \par Syntax-Formatstring
 * \copydoc strftime.dox
 */
CString MkDate(const char *format, ppluint64 sec)
{
	CString buffer;
	if (!format) {
		SetError(194,"const char *format");
		return buffer;
	}
	size_t size=strlen(format)*2+32;
	char *b=(char*)malloc(size);
	if (!b) {
		SetError(2);
		return buffer;
	}
	struct tm t;
	const time_t tt=(const time_t)sec;

	localtime_r(&tt, &t);
	if (strftime(b, size,format, &t)==0) {
		SetError(348,"%s",format);
		free(b);
		return buffer;
	}
	buffer.Set(b);
	free(b);
	return buffer;
}

CString Long2Date(const char *format, int value)
{
	int day=value%100;
	value=value/100;
	int month=value%100;
	int year=value/100;
	ppluint64 t=MkTime(year,month,day);
	return MkDate(format,t);
}

char *MkDate (char *buffer, int size, const char *format, ppluint64 sec)
/*!\brief Datum/Zeit formatieren
 * \ingroup PPLGroupDateTime
 *
 * \header \#include <ppl6.h>
 * \desc
 * Die Funktion MkDate wandelt einen Unix-Timestamp in einen String um.
 *
 * \param buffer
 * \param size
 * \param format ist ein beliebiger String, der verschiedene  Platzhalter
 * entahlten darf (siehe unten)
 * \param sec
 *
 * \par Syntax-Formatstring
 * \copydoc strftime.dox
 */
{
	if (!buffer) {
		SetError(194,"char *buffer");
		return NULL;
	}
	if (!format) {
		SetError(194,"const char *format");
		return NULL;
	}
	struct tm t;
	const time_t tt=(const time_t)sec;

	localtime_r(&tt, &t);
	if (strftime(buffer, size,format, &t)==0) {
		SetError(348,"%s",format);
		return NULL;
	}
	return buffer;
}



/*
 * CTimer-Klasse
 */

/*!\class CTimer
 * \ingroup PPLGroupDateTime
 */

CTimer::CTimer()
{
	startzeit=getmicrotime();
	endzeit=0.0;
	duration=0.0;
}

CTimer::~CTimer()
{
}

double CTimer::Start()
{
	startzeit=getmicrotime();
	return startzeit;
}

double CTimer::Stop()
{
	endzeit=getmicrotime();
	duration=endzeit-startzeit;
	return duration;
}

double CTimer::GetCurrentDuration()
{
	return getmicrotime()-startzeit;
}


double CTimer::GetDuration()
{
	return duration;
}





}		// EOF namespace ppl6

