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


#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_DIRENT_H
#include <dirent.h>
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
#include <io.h>
#include <direct.h>
#endif

#include <map>

#include "ppl6.h"

namespace ppl6 {

/*!\class CDirEntry
 * \ingroup PPLGroupFileIO
 * \brief Dateiverzeichnis-Eintrag
 *
 *
 * Die Klasse CDirEntry wird zum Abbilden von Dateien innerhalb eines Verzeichnisses
 * verwendet und ist die Basisklasse für alle Directory-Funktionen. Die Klasse CDir
 * setzt darauf auf.
 */


/*!\var CDirEntry::Filename
 * \brief Beinhaltet den Dateinamen
 *
 * Diese Variable beinhaltet nur den Dateinamen, ohne Pfadangaben
 */

/*!\var CDirEntry::File
 * \brief Beinhaltet Pfad und Dateinamen
 *
 * Diese Variable beinhaltet den vollständigen Pfad und Dateinamen.
 */

/*!\var CDirEntry::Path
 * \brief Beinhaltet nur den Pfad
 *
 * Diese Variable beinhaltet den vollständigen Pfad, aber nicht den Dateinamen.
 */

/*!\var CDirEntry::Size
 * \brief Größe der Datei in Bytes
 *
 * Größe der Datei in Bytes als vorzeichenloser 64-Bit-Wert (ppluint64).
 */

/*!\var CDirEntry::Attrib
 * \brief Attribute der Datei
 *
 * Enthält die Dateiattribute:
 * - CPPLDIR_NORMAL
 * - CPPLDIR_READONLY
 * - CPPLDIR_DIR
 * - CPPLDIR_LINK
 * - CPPLDIR_HIDDEN
 * - CPPLDIR_SYSTEM
 * - CPPLDIR_ARCHIV
 * - CPPLDIR_FILE
 *
 * Die Attribute können auch einzeln mit den Memberfunktionen IsDir, IsFile,
 * IsLink, IsHidden, IsReadOnly, IsArchiv und IsSystem abgefragt werden.
 *
 */

/*!\var CDirEntry::AttrStr
 * \brief Lesbare Darstellung der Datei-Attribute und Rechte
 *
 * Dieser String enthält eine lesbare Darstellung der Datei-Attribute und
 * Zugriffsrechte, wie man es vom UNIX-Befehl "ls -l" kennt.
 */

/*!\var CDirEntry::ATime
 * \brief Zeit des letzten Dateizugriffs
 *
 * Enthält das Datum des letzten Dateizugriffs in UNIX-Time.
 */

/*!\var CDirEntry::CTime
 * \brief Zeit der Datei-Erstellung
 *
 * Enthält das Datum der Erstellung der Datei in UNIX-Time.
 */

/*!\var CDirEntry::MTime
 * \brief Zeit der letzten Modifizierung
 *
 * Enthält das Datum der letzten Modifizierung der Datei in UNIX-Time.
 */



CDirEntry::CDirEntry()
/*!\brief Konstruktor der Klasse
 *
 * Dies ist der Konstruktor der Klasse.
 *
 * Hier werden alle Variablen der Klasse in den Ausgangszustand gebracht. Die
 * Strings werden geleert, alle Integer-Variablen werden auf 0 gesetzt. Da die
 * Klasse selbst keinen Speicher allokiert, gibt es keinen Destruktor.
 */
{
	Filename="";
	Path="";
	File="";
	Size=0;
	Attrib=0;
	AttrStr[0]=0;
	ATime=CTime=MTime=0;
	Uid=0;
	Gid=0;
	Blocks=0;
	BlockSize=0;
	NumLinks=0;
}

int CDirEntry::IsDir() const
/*!\brief Ist aktueller Eintrag ein Verzeichnis?
 *
 * Die Funktion prüft, ob der aktuelle Directory-Eintrag ein Verzeichnis ist.
 *
 * \return Liefert true (1) oder false (0) zurück.
 */
{
	if (Attrib&CPPLDIR_DIR) return 1;
	return 0;
}

int CDirEntry::IsFile() const
/*!\brief Ist aktueller Eintrag eine Datei?
 *
 * Die Funktion prüft, ob der aktuelle Directory-Eintrag eine Datei ist.
 *
 * \return Liefert true (1) oder false (0) zurück.
 */
{
	if (Attrib&CPPLDIR_FILE) return 1;
	return 0;
}

int CDirEntry::IsLink() const
/*!\brief Ist aktueller Eintrag ein Link?
 *
 * Die Funktion prüft, ob der aktuelle Directory-Eintrag ein Link ist.
 *
 * \return Liefert true (1) oder false (0) zurück.
 */
{
	if (Attrib&CPPLDIR_LINK) return 1;
	return 0;
}

int CDirEntry::IsHidden() const
/*!\brief Ist aktueller Eintrag versteckt?
 *
 * Die Funktion prüft, ob der aktuelle Directory-Eintrag eine versteckte Datei
 * oder ein versteckzes Verzeichnis ist.
 *
 * \return Liefert true (1) oder false (0) zurück.
 */
{
	if (Attrib&CPPLDIR_HIDDEN) return 1;
	return 0;
}

int CDirEntry::IsReadOnly() const
/*!\brief Ist aktueller Eintrag nur lesbar?
 *
 * Die Funktion prüft, ob der aktuelle Directory-Eintrag nur lesbar ist (Read-Only).
 *
 * \return Liefert true (1) oder false (0) zurück.
 */

{
	if (Attrib&CPPLDIR_READONLY) return 1;
	return 0;
}

int CDirEntry::IsArchiv() const
/*!\brief Hat aktueller Eintrag Archiv-Flag?
 *
 * Die Funktion prüft, ob der aktuelle Directory-Eintrag ein gesetztes
 * Archiv-Flag hat.
 *
 * \return Liefert true (1) oder false (0) zurück.
 */
{
	if (Attrib&CPPLDIR_ARCHIV) return 1;
	return 0;
}

int CDirEntry::IsSystem() const
/*!\brief Ist aktueller Eintrag eine Systemdatei?
 *
 * Die Funktion prüft, ob der aktuelle Directory-Eintrag eine Systemdatei ist.
 *
 * \return Liefert true (1) oder false (0) zurück.
 */
{
	if (Attrib&CPPLDIR_SYSTEM) return 1;
	return 0;
}

int CDirEntry::Copy(CDirEntry *dir)
/*!\brief Eintrag kopieren
 *
 * Die Funktion kopiert den Inhalt einer anderen CDirEntry- oder CDir-Klasse
 * in diese. Wird von den Zuweisungsoperatoren verwendet.
 *
 * \param dir Ein Pointer auf die zu kopierende CDirEntry- oder CDir-Klasse
 *
 * \return Liefert true (1) zurück, wenn der Eintrag erfolgreich kopiert wurde,
 * oder false (0) im Fehlerfall zurück. Ein Fehler kann nur dann auftreten, wenn
 * der übergebene /i dir Parameter NULL war.
 */
{
	if (!dir) {
		SetError(194,"int CDirEntry::Copy(CDirEntry *dir)");
		return 0;
	}
	Filename=dir->Filename;
	Path=dir->Path;
	File=dir->File;
	Size=dir->Size;
	Attrib=dir->Attrib;
	strncpy(AttrStr,(const char*)dir->AttrStr,sizeof(AttrStr));
	ATime=dir->ATime;
	CTime=dir->CTime;
	MTime=dir->MTime;
	Uid=dir->Uid;
	Gid=dir->Gid;
	Blocks=dir->Blocks;
	BlockSize=dir->BlockSize;
	NumLinks=dir->NumLinks;
	return 1;
}


CDirEntry& CDirEntry::operator=(const CDirEntry& dir)
/*!\brief Kopieroperator
 *
 * Kopiert den Inhalt einer anderen CDirEntry oder davon abgeleiteten Klasse
 */
{
	Copy((CDirEntry*)&dir);
	return *this;
}

CDirEntry& CDirEntry::operator=(const CDirEntry *dir)
/*!\brief Kopieroperator
 *
 * Kopiert den Inhalt einer anderen CDirEntry oder davon abgeleiteten Klasse
 */
{
	Copy((CDirEntry*)dir);
	return *this;
}

int CDirEntry::ToArray(CAssocArray *a) const
/*!\brief Eintrag in einem Array speichern
 *
 * Mit dieser Funktion können alle Informationen des aktuellen Verzeichnis-Eintrags
 * in einem Assoziativen Array gespeichert werden.
 *
 * \param a Pointer auf das Assoziative Array, in dem die Daten gespeichert werden
 * sollen.
 *
 * \return Liefert true (1) zurück, wenn die Daten erfolgreich gespeichert wurden,
 * ansonsten false (0).
 */
{
	if (!a) {
		SetError(194,"int CDirEntry::ToArray(CAssocArray *a)");
		return 0;
	}
	a->Set("filename",Filename);
	a->Set("path",Path);
	a->Set("file",File);
	a->Setf("size","%llu",Size);
	a->Setf("attrib","%u",Attrib);
	a->Set("attribstr",AttrStr);
	a->Setf("atime","%llu",ATime);
	a->Setf("ctime","%llu",CTime);
	a->Setf("mtime","%llu",MTime);
	return 1;
}

void CDirEntry::Print(char *label)  const
/*!\brief Verzeichniseintrag ausgeben
 *
 * Gibt den Inhalt des aktuellen Verzeichnis-Eintrags auf STDOUT aus.
 *
 * \param label ist ein optionaler String, der bei der Ausgabe jeder Zeile
 * vorangestellt wird.
 */
{
	if (!label) label=(char*)"CDirEntry";
	CAssocArray a;
	ToArray(&a);
	a.List(label);
}


/*!\class CDir
 * \ingroup PPLGroupFileIO
 * \brief Klasse zum Durchsuchen von Verzeichnissen
 *
 * \desc
 * Die Klasse CDir wird zum Lesen von Verzeichnissen verwendet. Dazu muss man zunächst mit
 * CDir::Open ein Verzeichnis öffnen oder das gewünschte Verzeichnis gleich im Konstruktor
 * angeben. Anschließend kann man mit den Funktionen CDir::GetFirst und CDir::GetNext die
 * einzelnen Dateien auslesen. Diese sind bereits nach Dateiname sortiert. Mit dem
 * Befehl CDir::Resort kann man aber jederzeit eine andere Sortierung einstellen oder diese
 * ganz abschalten.
 * \par
 * Möchte man nur Dateien, die einem bestimmten Muster (Pattern) entsprechen, kann man
 * statt CDir::GetFirst und CDir::GetNext auch CDir::GetFirstPattern und CDir::GetNextPattern
 * verwenden, oder falls mal Regular Expressions verwenden möchte, CDir::GetFirstRegExp und
 * CDir::GetNextRegExp.
 *
 * \since
 * Die Klasse gibt es schön länger, sie wurde jedoch in Version 6.3.0 komplett überarbeitet.
 */

/*!\enum CDir::Sort
 * \brief Sortiermöglichkeiten
 *
 * In dieser Enumeration sind die verschiedenen Sortiermöglichkeiten definiert:
 */

/*!\var CDir::Sort CDir::Sort_None
 * Keine Sortierung. Die Reihenfolge der Dateien hängt fom Betriebs- und Filesystem ab.
 */

/*!\var CDir::Sort CDir::Sort_Filename
 * Es wird eine Sortierung anhand der Dateinamen vorgenommen. Dabei wird Groß- und Kleinschreibung
 * beachtet. Dateien, die mit einem Großbuchstaben beginnen, werden zuerst aufgelistet, danach
 * Dateien mit Kleinbuchstaben.
 */

/*!\var CDir::Sort CDir::Sort_Filename_IgnoreCase
 * Es wird eine Sortierung anhand der Dateinamen vorgenommen. Dabei wird Groß- und Kleinschreibung
 * ignoriert. Dateien mit Großbuchstaben und Kleinbuchstaben werden vermischt ausgegeben, wobei
 * jedoch die Alphabetische Reihenfolge erhalten bleibt.
 */

/*!\var CDir::Sort CDir::Sort_Date
 * Es wird eine Sortierung nach dem Datum der letzten Modifizierung vorgenommen.
 */

CString CDir::CurrentPath()
/*!\ingroup PPLGroupFileIO
 * \brief Aktuelles Verzeichnis
 *
 * Diese Funktion liefert das aktuelle Verzeichnis zurück.
 *
 * \return String mit dem aktuellen Verzeichnis
 */
{
	CString ret;
	char *buf=(char*)malloc(2048);
#ifdef _WIN32
	if (_getcwd(buf,2048)) {
#else
	if (getcwd(buf,2048)) {
#endif
		ret=buf;
	} else {
		SetErrorFromErrno();
	}
	return ret;
}

/*!\ingroup PPLGroupFileIO
 * \brief Homeverzeichnis des aktuellen Users
 *
 * \desc
 * Diese statische Funktion liefert das Homeverzeichnis des aktuellen Benutzers
 * zurück.
 *
 * \return String mit dem Verzeichnis
 * \exception UnsupportedFeatureException Wird geworfen, wenn das Homeverzeichnis
 * nicht ermittelt werden kann.
 */
CString CDir::homePath()
{
	CString ret;
#ifdef _WIN32
	char *homeDir = getenv("HOMEPATH");
	char *homeDrive = getenv("HOMEDRIVE");
	ret.Setf("%s\\%s",homeDrive, homeDir);
	return ret;
#else
	char *homeDir = getenv("HOME");
	if (homeDir!=NULL && strlen(homeDir)>0) {
		ret.Set(homeDir);
		return ret;
	}
	throw UnsupportedFeatureException("CDir::homePath");
#endif
}

/*!\ingroup PPLGroupFileIO
 * \brief Verzeichnis für temporäre Dateien
 *
 * \desc
 * Diese statische Funktion liefert das Verzeichnis zurück, in dem
 * temporäre Dateien abgelegt werden können.
 *
 * \return String mit dem Verzeichnis
 */
CString CDir::tempPath()
{
#ifdef _WIN32
	TCHAR TempPath[MAX_PATH];
	GetTempPath(MAX_PATH, TempPath);
	return CString(TempPath);
#endif
	const char *dir = getenv("TMPDIR");
	if (dir!=NULL && strlen(dir)>0) return CString(dir);
#ifdef P_tmpdir
	dir=P_tmpdir;
#endif
	if (dir!=NULL && strlen(dir)>0) return CString(dir);
	return CString("/tmp");
}




CDir::CDir(const CString &path, Sort s)
/*!\brief Konstruktor der Klasse
 *
 * \desc
 * Der Konstruktor sorgt dafür, dass die internen Variablen und Datenstrukturen
 * initialisert werden. Optional kann auch schon mit dem Parameter \p path ein
 * zu öffnender Pfad angegeben werden und mit \p s eine Sortierreihenfolge.
 *
 * @param[in] path Zu öffnender Pfad (siehe auch CDir::Open)
 * @param[in] s gewünschte Sortierreihenfolge. Defaultmäßig wird keine Sortierung
 * verwendet.
 */
{
	sort=s;
	if (path.NotEmpty()) Open(path,s);
}


CDir::~CDir()
/*!\brief Destruktor der Klasse
 *
 * \desc
 * Der Destruktor sorgt dafür, dass der intern reservierte Speicher wieder
 * freigegeben wird.
 */
{
	Clear();
}

void CDir::Clear()
/*!\brief Verzeichnisliste löschen
 *
 * \desc
 * Wird diese Funktion nach CDir::Open aufgerufen, wird die interne Dateiliste
 * wieder gelöscht. Die Funktion wird automatisch vom Destruktor und zu Beginn
 * von CDir::Open, so dass sich ein manueller Aufruf der Funktion in der Regel
 * erübrigt.
 */
{
	Files.clear();
	SortedFiles.clear();
	Path.Clear();
	sort=Sort_None;

}


int CDir::Num() const
/*!\brief Anzahl Dateien
 *
 * \desc
 * Diese Funktion liefert die Anzahl Dateien im geöffneten Verzeichnis zurück. Sie
 * gibt daher erst nach Aufruf von CDir::Open einen korrekten Wert zurück.
 * @return
 */
{
	return Files.count();
}

/*!\brief Sortierung ändern
 *
 * \desc
 * Durch Aufruf dieser Funktion kann die Sortierreihenfolge für die get...-Befehle
 * geändert werden. Standardmäßig werden die Dateien unsortiert zurückgegeben.
 * Die Reihenfolge hängt somit im Wesentlichen davon ab, in welcher Reihenfolge
 * die Dateien erstellt wurden, aber auch von Betriebs- und Filesystemabhängigen
 * Vorgängen.
 * \par
 * Die Sortierreihenfolge läßt sich jederzeit durch Aufruf dieser Funktion ändern.
 *
 * \param[in] s Die gewünschte Sortierreihenfolge. Siehe dazu auch die Enumeration Dir::Sort
 * \exception IllegalArgumentException Wird geworfen, wenn eine ungültige Sortiermethode angegeben wird
 */
void CDir::Resort(Sort s)
{
	SortedFiles.clear();
	switch (s) {
		case Sort_None: resortNone(); break;
		case Sort_Filename: resortFilename(); break;
		case Sort_Filename_IgnoreCase: resortFilenameIgnoreCase(); break;
		case Sort_ATime: resortATime(); break;
		case Sort_CTime: resortCTime(); break;
		case Sort_MTime: resortMTime(); break;
		case Sort_Date: resortMTime(); break;
		case Sort_Size: resortSize(); break;
		default: throw IllegalArgumentException();
	}
	sort=s;
}

/*!\brief Dateien unsortiert belassen
 *
 * \desc
 * Diese interne Funktion kopiert lediglich das von Dir::open eingescannte Verzeichnis
 * unsortiert in die von den Iterationsfunktionen verwendete Liste. Die Funktion wird
 * von Dir::resort in Abhängigkeit des eingestellten Sortieralgorithmus aufgerufen.
 */
void CDir::resortNone()
{
	ppl6::List<CDirEntry>::Iterator it;
	Files.reset(it);
	while (Files.getNext(it)) {
		SortedFiles.add(&it.value());
	}
	return;
}

/*!\brief Dateien nach Dateiname sortieren
 *
 * \desc
 * Diese interne Funktion sortiert das durch Dir::open eingescannte Verzeichnis
 * nach Dateiname, unter Beachtung von Gross-/Kleinschreibung.
 * Die Funktion wird
 * von Dir::resort in Abhängigkeit des eingestellten Sortieralgorithmus aufgerufen.
 */
void CDir::resortFilename()
{
	ppl6::List<CDirEntry>::Iterator it;
	Files.reset(it);

	std::multimap<CString, const CDirEntry*> sorter;
	while (Files.getNext(it)) {
		const CDirEntry &de=it.value();
		sorter.insert(std::pair<CString,const CDirEntry*>(de.Filename,&de));
	}
	std::multimap<CString, const CDirEntry*>::const_iterator sortit;
	for (sortit=sorter.begin();sortit!=sorter.end();sortit++) {
		SortedFiles.add((*sortit).second);
	}
}

/*!\brief Dateien nach Dateiname sortieren, Gross-/Kleinschreibung wird ignoriert
 *
 * \desc
 * Diese interne Funktion sortiert das durch Dir::open eingescannte Verzeichnis
 * nach Dateiname, wobei Gross-/Kleinschreibung ignoriert wird.
 * Die Funktion wird
 * von Dir::resort in Abhängigkeit des eingestellten Sortieralgorithmus aufgerufen.
 */
void CDir::resortFilenameIgnoreCase()
{
	ppl6::List<CDirEntry>::Iterator it;
	Files.reset(it);

	std::multimap<CString, const CDirEntry*> sorter;
	CString filename;
	while (Files.getNext(it)) {
		const CDirEntry &de=it.value();
		filename.Set(de.Filename);
		filename.LCase();
		sorter.insert(std::pair<CString,const CDirEntry*>(filename,&de));
	}
	std::multimap<CString, const CDirEntry*>::const_iterator sortit;
	for (sortit=sorter.begin();sortit!=sorter.end();sortit++) {
		SortedFiles.add((*sortit).second);
	}
}

/*!\brief Dateien nach Modifizierungsdatum sortieren
 *
 * \desc
 * Diese interne Funktion sortiert das durch Dir::open eingescannte Verzeichnis
 * nach dem Modifikations-Zeitstempel der Dateien. Dieser Zeitstempel ändert sich
 * nur bei Neuanlage der Datei oder des Verzeichnisses, oder wenn ein Schreibzugriff
 * stattgefunden hat.
 * Falls mehrere Dateien den gleichen Zeitstempel haben, ist deren Reihenfolge unbestimmt.
 * \par
 * Die Funktion wird
 * von Dir::resort in Abhängigkeit des eingestellten Sortieralgorithmus aufgerufen.
 */
void CDir::resortMTime()
{
	ppl6::List<CDirEntry>::Iterator it;
	Files.reset(it);

	std::multimap<ppluint64, const CDirEntry*> sorter;
	while (Files.getNext(it)) {
		const CDirEntry &de=it.value();
		sorter.insert(std::pair<ppluint64,const CDirEntry*>(de.MTime,&de));
	}
	std::multimap<ppluint64, const CDirEntry*>::const_iterator sortit;
	for (sortit=sorter.begin();sortit!=sorter.end();sortit++) {
		SortedFiles.add((*sortit).second);
	}
}

/*!\brief Dateien nach Datum der letzten Statusänderung sortieren
 *
 * \desc
 * Diese interne Funktion sortiert das durch Dir::open eingescannte Verzeichnis
 * nach dem Zeitstempel der letzten Statusänderung der Dateien. Eine Statusänderung
 * besteht nicht nur bei Neuanlage und Schreibzugriff, sondern auch bei Änderung
 * der Zugriffsrechte oder Verlinkung.
 * Falls mehrere Dateien den gleichen Zeitstempel haben, ist deren Reihenfolge unbestimmt.
 * \par
 * Die Funktion wird
 * von Dir::resort in Abhängigkeit des eingestellten Sortieralgorithmus aufgerufen.
 */
void CDir::resortCTime()
{
	ppl6::List<CDirEntry>::Iterator it;
	Files.reset(it);

	std::multimap<ppluint64, const CDirEntry*> sorter;
	while (Files.getNext(it)) {
		const CDirEntry &de=it.value();
		sorter.insert(std::pair<ppluint64,const CDirEntry*>(de.CTime,&de));
	}
	std::multimap<ppluint64, const CDirEntry*>::const_iterator sortit;
	for (sortit=sorter.begin();sortit!=sorter.end();sortit++) {
		SortedFiles.add((*sortit).second);
	}
}

/*!\brief Dateien nach Datum des letzten Zugriffs sortieren
 *
 * \desc
 * Diese interne Funktion sortiert das durch Dir::open eingescannte Verzeichnis
 * nach dem Zeitstempel des letzten Zugriffs auf die Datei.
 * Falls mehrere Dateien den gleichen Zeitstempel haben, ist deren Reihenfolge unbestimmt.
 * \par
 * Die Funktion wird
 * von Dir::resort in Abhängigkeit des eingestellten Sortieralgorithmus aufgerufen.
 */
void CDir::resortATime()
{
	ppl6::List<CDirEntry>::Iterator it;
	Files.reset(it);

	std::multimap<ppluint64, const CDirEntry*> sorter;
	while (Files.getNext(it)) {
		const CDirEntry &de=it.value();
		sorter.insert(std::pair<ppluint64,const CDirEntry*>(de.ATime,&de));
	}
	std::multimap<ppluint64, const CDirEntry*>::const_iterator sortit;
	for (sortit=sorter.begin();sortit!=sorter.end();sortit++) {
		SortedFiles.add((*sortit).second);
	}
}

/*!\brief Dateien nach Dateigröße sortieren
 *
 * \desc
 * Diese interne Funktion sortiert das durch Dir::open eingescannte Verzeichnis
 * nach der Größe der Dateien. Falls mehrere Dateien mit gleicher Größe vorhanden
 * sind, ist deren Reihenfolge unbestimmt.
 * \par
 * Die Funktion wird
 * von Dir::resort in Abhängigkeit des eingestellten Sortieralgorithmus aufgerufen.
 */
void CDir::resortSize()
{
	ppl6::List<CDirEntry>::Iterator it;
	Files.reset(it);

	std::multimap<ppluint64, const CDirEntry*> sorter;
	while (Files.getNext(it)) {
		const CDirEntry &de=it.value();
		sorter.insert(std::pair<ppluint64,const CDirEntry*>(de.Size,&de));
	}
	std::multimap<ppluint64, const CDirEntry*>::const_iterator sortit;
	for (sortit=sorter.begin();sortit!=sorter.end();sortit++) {
		SortedFiles.add((*sortit).second);
	}

}



void CDir::Reset()
/*!\brief Zeiger auf den ersten Eintrag des Verzeichnisses
 *
 * \desc
 * Mit dieser Funktion wird der interne Zeiger auf den ersten Eintrag
 * im Verzeichnis gesetzt. Der nächste Aufruf von einer der "GetNext..."-Funktionen
 * würde somit den ersten Eintrag zurückliefern.
 */
{
	SortedFiles.reset(myit);
}

const CDirEntry *CDir::GetFirst()
/*!\brief Erster Verzeichniseintrag
 *
 * \desc
 * Durch Aufruf dieser Funktion wird der interne Zeiger auf die erste gefundene Datei gesetzt und ein Pointer
 * auf dessen CDirEntry-Struktur zurückgegeben. Alle weiteren Dateien können mit CDir::GetNext ausgelesen werden.
 * \par
 * Die Reihenfolge der durch CDir::GetFirst und CDir::GetNext zurückgelieferten Dateien hängt von
 * der eingestellten Sortierung ab. Siehe dazu CDir::Sort, CDir::Resort und CDir::Open
 *
 * @return Pointer auf die erste Datei des Verzeichnisses, oder NULL wenn das Verzeichnis leer ist oder kein
 * gültiges Verzeichnis ausgewählt wurde.
 */
{
	Reset();
	return GetNext();
}

const CDirEntry *CDir::GetNext()
/*!\brief Nächster Verzeichniseintrag
 *
 * \desc
 * Diese Funktion liefert die nächste Datei aus dem geöffneten Verzeichnis zurück.
 * \par
 * Die Reihenfolge der durch CDir::GetFirst und CDir::GetNext zurückgelieferten Dateien hängt von
 * der eingestellten Sortierung ab. Siehe dazu CDir::Sort, CDir::Resort und CDir::Open
 *
 * @return Pointer auf die nächste Datei des Verzeichnisses, oder NULL, wenn keine weiteren Dateien
 * vorhanden sind, das Verzeichnis leer ist oder kein gültiges Verzeichnis ausgewählt wurde.
 */
{
	if (SortedFiles.getNext(myit)) return myit.value();
	return NULL;
}

const CDirEntry *CDir::GetFirstPattern(const char *pattern, bool ignorecase)
/*!\brief Erster Verzeichniseintrag, der zu einem bestimmten Muster passt
 *
 * \desc
 * Durch Aufruf dieser Funktion wird die erste Datei aus dem Verzeichnis zurückgeliefert, die
 * zu dem angegebenen Muster \p pattern passt. Alle weiteren Dateien können mit CDir::GetNextPattern
 * ausgelesen werden.
 * \par
 * Die Reihenfolge der durch CDir::GetFirstPattern und CDir::GetNextPattern zurückgelieferten Dateien hängt von
 * der eingestellten Sortierung ab. Siehe dazu CDir::Sort, CDir::Resort und CDir::Open
 *
 * \param[in] pattern Ein beliebiges Suchpattern, wie es auch beim Unix-Befehl "ls" oder mit
 * "dir" unter Windows angegeben werden kann. Dabei sind die Wildcards "*" und "?" erlaubt.
 * Das Sternchen "*" steht dabei für beliebig viele Zeichen, das Fragezeichen "?" für ein einzelnes.
 * \param[in] ignorecase Wird diese Variable auf "true" gesetzt, wird Groß- und Kleinschreibung
 * ignoriert. Wird als Pattern beispielsweise "*.TXT" angegeben, würde auch "*.txt" passen.
 * Der Default ist "false"
 *
 * @return Pointer auf die erste Datei des Verzeichnisses, die zum angegebenen Muster passt, oder
 * NULL wenn keine Datei passt, das Verzeichnis leer ist oder kein
 * gültiges Verzeichnis ausgewählt wurde.
 */
{
	Reset();
	return GetNextPattern(pattern, ignorecase);
}

const CDirEntry *CDir::GetNextPattern(const char *pattern, bool ignorecase)
/*!\brief Nächster Verzeichniseintrag, der zu einem bestimmten Muster passt
 *
 * \desc
 * Diese Funktion liefert die nächste Datei aus dem geöffneten Verzeichnis zurück, die
  * zu dem angegebenen Muster \p pattern passt.
 * \par
 * Die Reihenfolge der durch CDir::GetFirstPattern und CDir::GetNextPattern zurückgelieferten Dateien hängt von
 * der eingestellten Sortierung ab. Siehe dazu CDir::Sort, CDir::Resort und CDir::Open
 *
 * \param[in] pattern Ein beliebiges Suchpattern, wie es auch beim Unix-Befehl "ls" oder mit
 * "dir" unter Windows angegeben werden kann. Dabei sind die Wildcards "*" und "?" erlaubt.
 * Das Sternchen "*" steht dabei für beliebig viele Zeichen, das Fragezeichen "?" für ein einzelnes.
 * \param[in] ignorecase Wird diese Variable auf "true" gesetzt, wird Groß- und Kleinschreibung
 * ignoriert. Wird als Pattern beispielsweise "*.TXT" angegeben, würde auch "*.txt" passen.
 * Der Default ist "false"
 *
 * @return Pointer auf die nächste Datei des Verzeichnisses, oder NULL, wenn keine weiteren Dateien
 * vorhanden sind, die auf das Muster passen, das Verzeichnis leer ist oder kein gültiges
 * Verzeichnis ausgewählt wurde.
 */
{
	const CDirEntry *de;
	CString Name, Pattern;
	Pattern=pattern;
	Pattern.PregEscape();
	Pattern.Replace("\\*",".*");
	Pattern.Replace("\\?",".");
	Pattern="/^"+Pattern;
	Pattern+="$/";
	if (ignorecase) Pattern+="i";
	//printf ("Pattern: %s\n",(const char*)Pattern);
	while (1) {
		if (!SortedFiles.getNext(myit)) return NULL;
		de=myit.value();
		// Patternmatch
		Name=de->Filename;
		if (Name.PregMatch(Pattern)) {
			//printf ("Match\n");
			return de;
		}
		//printf ("Kein Match: %s\n",(const char *)Name);
	}
	return NULL;
}

const CDirEntry *CDir::GetFirstRegExp(const char *regexp)
/*!\brief Erster Verzeichniseintrag, der zu der angegebenen Regular Expression passt
 *
 * \desc
 * Durch Aufruf dieser Funktion wird die erste Datei aus dem Verzeichnis zurückgeliefert, die
 * zu der angegebenen Regular Expression \p regexp passt. Alle weiteren Dateien können mit
 * CDir::GetNextRegExp ausgelesen werden.
 * \par
 * Die Reihenfolge der durch CDir::GetFirstRegExp und CDir::GetNextRegExp zurückgelieferten
 * Dateien hängt von der eingestellten Sortierung ab. Siehe dazu CDir::Sort, CDir::Resort
 * und CDir::Open
 *
 * \param[in] regexp Eine beliebige Perl kompatible Regular Expression. Beispiel:"/^*.txt$/i"
 *
 * @return Pointer auf die erste Datei des Verzeichnisses, oder NULL wenn das Verzeichnis leer ist,
 * kein Dateiname auf die angegebene Regular Expression passt oder kein
 * gültiges Verzeichnis ausgewählt wurde.
 */
{
	Reset();
	return GetNextRegExp(regexp);
}

const CDirEntry *CDir::GetNextRegExp(const char *regexp)
/*!\brief Nächster Verzeichniseintrag, der zu der angegebenen Regular Expression passt
 *
 * \desc
 * Diese Funktion liefert die nächste Datei aus dem geöffneten Verzeichnis zurück, die
 * zu der angegebenen Regular Expression \p regexp passt.
 *
 * \par
 * Die Reihenfolge der durch CDir::GetFirstRegExp und CDir::GetNextRegExp zurückgelieferten
 * Dateien hängt von der eingestellten Sortierung ab. Siehe dazu CDir::Sort, CDir::Resort
 * und CDir::Open
 *
 * \param[in] regexp Eine beliebige Perl kompatible Regular Expression. Beispiel:"/^*.txt$/i"
 *
 * @return Pointer auf die nächste Datei des Verzeichnisses, oder NULL wenn das Verzeichnis leer ist,
 * kein weiterer Dateiname auf die angegebene Regular Expression passt oder kein
 * gültiges Verzeichnis ausgewählt wurde.
 */
{
	const CDirEntry *de;
	CString Name;
	while (1) {
		if (!SortedFiles.getNext(myit)) return NULL;
		de=myit.value();
		// Patternmatch
		Name=de->Filename;
		if (Name.PregMatch(regexp)) return de;
	}
	return NULL;
}


void CDir::Print()
/*!\brief Verzeichnis auf STDOUT ausgeben
 *
 * Mit dieser Funktion wird das mit CDir::Open ausgewählte Verzeichnis
 * auf STDOUT ausgegeben.
 */
{
	Reset();
	const CDirEntry *de;
	printf ("Directory Listing: %s\n",(const char*)Path);
	printf ("Total Files: %u\n",Num());
	while ((de=GetNext())) {
		Print(de);
	}

}

void CDir::Print(const CDirEntry *de)
/*!\brief Verzeichnis-Eintrag auf STDOUT ausgeben
 *
 * Mit dieser Funktion kann ein Verzeichniseintrag auf STDOUT ausgegeben werden.
 * Die Ausgabe ist ähnlich der des "ls"-Befehls unter Unix, enthält jedoch
 * nicht die Benutzerrechte. Die Funktion wurde hauptsächlich zu Debuggingzwecken
 * eingebaut.
 *
 * \param[in] de Pointer auf einen Verzeichniseintrag
 */
{
	if (!de) return;
	CString s;
	ppl_printf ("%s %3i ",(char*)de->AttrStr,de->NumLinks);
	ppl_printf ("%10llu ",de->Size);
	MkDate(s,"%d %b %Y",de->MTime);
	ppl_printf ("%s %s\n",(const char*)s,(const char*)de->Filename);
}


/*!\fn CDir::Open(const CString &path, Sort s)
 * \brief Verzeichniss öffnen
 *
 * \desc
 * Mit diesem Befehl wird ein Verzeichnis geöffnet. Dabei werden alle im
 * angegebenen Pfad \p path befindlichen Dateinamen und deren Attribute eingelesen
 * und in einer internen Liste gespeichert. Diese kann mit den Get-Befehlen ausgelesen
 * werden. Sofern auch eine Sortierung gewünscht war (Aufruf von CDir::Resort oder
 * Parameter \p s dieser Funktion) werden die Dateien zusätzlich auch noch in einem
 * binären AVL-Baum abgelegt.
 *
 * @param[in] path Der gewünschte Pfad. Unter Windows können hier sowohl die
 * Windows-typischen Backslashes verwendet werden, als auch die Unix-typischen
 * Slashes.
 * @param[in] s Optional die gewünschte Sortierung. Diese kann auch nachträglich
 * noch mit CDir::Resort verändert werden. Für die möglichen Konstanten, siehe
 * CDir::Sort
 * @return Konnte das Verzeichnis erfolgreich eingelesen werden, liefert die
 * Funktion 1 zurück, im Fehlerfall 0
 *
 * \see
 * - CDir::GetFirst
 * - CDir::GetNext
 * - CDir::GetFirstPattern
 * - CDir::GetNextPattern
 * - CDir::GetFirstRegExp
 * - CDir::GetNextRegExp
 */

}

namespace ppl6 {


#if defined _WIN32
int CDir::Open(const CString &path, Sort s)
{
	Clear();
	sort=s;
	Path=path;
	Path.Trim();
	Path.RTrim("/");
	Path.RTrim("\\");
	//printf ("Path=%s \n",(char*)Path);
	CWString Pattern=Path;
	Pattern+="/*";
	struct _wfinddatai64_t data;
	memset(&data,0,sizeof(data));

	intptr_t handle=_wfindfirsti64 ((const wchar_t*)Pattern, &data);
	if (handle<0) {
		SetErrorFromErrno();
		return 0;
	}
	while (1==1) {
		CDirEntry de;
		strcpy (de.AttrStr,"----------");
		de.Attrib=0;
		de.ATime=de.CTime=de.MTime=0;

		de.Filename=data.name;
		de.Size=0;
		de.Path=Path;
		de.File=de.Path+CString("/")+de.Filename;
		de.Size=data.size;
		de.Uid=0;
		de.Gid=0;
		de.Blocks=0;
		de.BlockSize=0;
		de.NumLinks=1;

		if (data.attrib & _A_RDONLY) de.Attrib|=CPPLDIR_READONLY;
		if (data.attrib & _A_HIDDEN) de.Attrib|=CPPLDIR_HIDDEN;
		if (data.attrib & _A_SYSTEM) de.Attrib|=CPPLDIR_SYSTEM;
		if (data.attrib & _A_ARCH) de.Attrib|=CPPLDIR_ARCHIV;
		if (data.attrib & _A_SUBDIR) de.Attrib|=CPPLDIR_DIR;

		de.AttrStr[1]=de.AttrStr[4]=de.AttrStr[7]='r';
		if (de.Filename.Len()>4) {
			CString suf=de.Filename.Right(4);
			suf.LCase();
			if (suf.StrCmp(".exe")==0 ||
				suf.StrCmp(".com")==0 ||
				suf.StrCmp(".bat")==0 ) de.AttrStr[3]=de.AttrStr[6]=de.AttrStr[9]='x';
		}

		if (! (de.Attrib&CPPLDIR_READONLY)) de.AttrStr[2]=de.AttrStr[5]=de.AttrStr[8]='w';
		if ( ( de.Attrib & (CPPLDIR_DIR|CPPLDIR_LINK) )==0) de.Attrib|=CPPLDIR_FILE;
		de.ATime=data.time_access;
		de.CTime=data.time_create;
		de.MTime=data.time_write;

		if (de.Attrib&CPPLDIR_DIR) de.AttrStr[0]='d';
		if (de.Attrib&CPPLDIR_LINK) de.AttrStr[0]='l';
		Files.add(de);
		// Nächster Datensatz
		if (_wfindnexti64(handle,&data)<0) break;
	}
	_findclose(handle);
	Resort(sort);
	Reset();
	return 1;
}
#elif defined HAVE_OPENDIR
int CDir::Open(const CString &path, Sort s)
{
	Clear();
	sort=s;
	Path=path;
	Path.Trim();
	Path.RTrim("/");
	Path.RTrim("\\");
	DIR *dir=opendir((const char*)Path);
	if (!dir) {
		ppl6::SetErrorFromErrno("Verzeichnis: %s",(const char*)Path);
		return 0;
	}
	//struct dirent d;
	struct dirent *result;
	while (1==1) {
		result=readdir(dir);
		if (!result) {
			if (errno) {
				ppl6::SetErrorFromErrno("Verzeichnis: %s",(const char*)Path);
				closedir(dir);
				return 0;
			}
			break;
		}
		CDirEntry de;
		strcpy (de.AttrStr,"----------");
		de.Attrib=0;
		de.ATime=de.CTime=de.MTime=0;
		de.Filename=result->d_name;
		de.Size=0;
		de.File.Sprintf("%s/%s",(const char*)Path,result->d_name);		// Fileinfos holen
		de.Path=Path;
		struct stat st;
		if (stat ((const char*)de.File,&st)==0) {
			de.Size=st.st_size;
			de.Uid=st.st_uid;
			de.Gid=st.st_gid;
			de.Blocks=st.st_blocks;
			de.BlockSize=st.st_blksize;
			de.NumLinks=st.st_nlink;
			if (st.st_mode & S_IFDIR) de.Attrib|=CPPLDIR_DIR;
			if (st.st_mode & S_IFREG) de.Attrib|=CPPLDIR_FILE;
			#ifndef __DJGPP__
				if (st.st_mode & S_IFLNK) de.Attrib|=CPPLDIR_LINK;
			#endif

			if (st.st_mode & S_IRUSR) de.AttrStr[1]='r';
			if (st.st_mode & S_IWUSR) de.AttrStr[2]='w';
			if (st.st_mode & S_IXUSR) de.AttrStr[3]='x';
			if (st.st_mode & S_ISUID) de.AttrStr[3]='s';

			if (st.st_mode & S_IRGRP) de.AttrStr[4]='r';
			if (st.st_mode & S_IWGRP) de.AttrStr[5]='w';
			if (st.st_mode & S_IXGRP) de.AttrStr[6]='x';
			if (st.st_mode & S_ISGID) de.AttrStr[6]='s';

			if (st.st_mode & S_IROTH) de.AttrStr[7]='r';
			if (st.st_mode & S_IWOTH) de.AttrStr[8]='w';
			if (st.st_mode & S_IXOTH) de.AttrStr[9]='x';
			de.ATime=st.st_atime;
			de.CTime=st.st_ctime;
			de.MTime=st.st_mtime;
		}
		if (de.Attrib&CPPLDIR_DIR) de.AttrStr[0]='d';
		if (de.Attrib&CPPLDIR_LINK) de.AttrStr[0]='l';
		Files.add(de);
	}
	closedir(dir);
	Resort(sort);
	Reset();
	return 1;
}
#else
int CDir::Open(const char *path, Sort s)
{
	SetError(246);
	return 0;
}

#endif




} // end of namespace ppl6
