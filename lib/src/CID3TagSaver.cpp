/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: CID3TagSaver.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
 *
 *
 * Copyright (c) 2010 Patrick Fedick
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "wm_id3tagsaver.h"
#include <ppl7-audio.h>


namespace de {
namespace pfp {
namespace winmusik {


CID3TagSaver::WorkItem::WorkItem()
{
	retry=0;
	cleartag=false;
	writev1=true;
	writev2=true;
}

CID3TagSaver::WorkItem::WorkItem(const ppl7::String &filename, const ppl7::AssocArray &Tags, bool cleartag, bool writev1, bool writev2)
{
	this->Filename=filename;
	this->Tags=Tags;
	retry=0;
	this->cleartag=cleartag;
	this->writev1=writev1;
	this->writev2=writev2;
}

/*!\class CID3TagSaver
 * \brief ID3Tags asynchron oder synchron speichern
 *
 * Mit dieser Klasse können ID3-Tags (Version 1 und Version 2.3) asynchron gespeichert werden.
 * Beim erstmaligen Aufruf der Add-Funktion wird dafür ein eigener Thread gestartet.
 */


CID3TagSaver::CID3TagSaver()
/*!\brief Konstruktor der Klasse
 *
 * Der Konstruktor der Klasse hat gegenwärtig keine Funktion, er setzt lediglich
 * die initialien Wert \c PaddingSize auf 1024
 */
{
	PaddingSize=1024;
	RetryIntervall=60;
}

CID3TagSaver::~CID3TagSaver()
/*!\brief Destruktor der Klasse
 *
 * Der Destruktor der Klasse sorgt dafür, dass ein eventuell laufender Update-Thread nach
 * Beendigung seiner aktuellen Aktion gestoppt wird. Falls zu diesem Zeitpunkt noch unerledigte
 * Aufträge in der Queue sind, werden diese nicht mehr abgearbeitet.
 */
{
	threadSignalStop();
	threadStop();
}

void CID3TagSaver::Add(const ppl7::String &filename, const ppl7::AssocArray &Tags, bool cleartag, bool writev1, bool writev2)
/*!\brief Auftrag in die Queue stellen
 *
 * Mit dieser Funktion wird ein neuer Update-Auftrag in die Queue gestellt.
 *
 * \param[in] filename Pointer auf den Dateinamen
 * \param[in] Tags Pointer auf ein Assoziatives Array, das die zu schreibenden Tags enthält. Folgende
 * Schlüssel sind möglich:
 * \arg \c artist Name des Interpreten
 * \arg \c title Der Titel des Songs
 * \arg \c version Die Version/Mix des Titels
 * \arg \c year Das Erscheinungsjahr
 * \arg \c genre Das Genre des Titels (String)
 * \arg \c comment Kommentar
 * \arg \c album Album
 * \param[in] cleartag Hat diese Variable den Wert \c true, wird ein vorhandener ID3v2-Tag zunächst
 * gelöscht und mit dem neuen überschrieben. Ist dies nicht der Fall, werden nur die gesetzten Werte
 * überschrieben.
 * \param[in] writev1 Wenn gesetzt, wird ein ID3v1-Tag geschrieben (=default)
 * \param[in] writev2 Wenn gesetzt, wird ein ID3v2-Tag geschrieben (=default)
 *
 */
{
	CID3TagSaver::WorkItem item(filename,Tags,cleartag,writev1,writev2);
	Mutex.lock();
	Todo.push(item);
	if (!this->threadIsRunning()) {
		this->threadStart();
	}
	Mutex.unlock();
}


void CID3TagSaver::UpdateNow(const CID3TagSaver::WorkItem &item)
/*!\brief Auftrag ausführen
 *
 * Mit dieser Funktion wird ein ID3-Tag sofort in die angegebene Datei geschrieben, ohne
 * ihn in die Queue zu stellen. Die Funktion kehrt erst wieder zurück, wenn der Auftrag
 * beendet ist.
 *
 * \returns Bei Erfolg liefert die Funktion 1 zurück, sonst 0.
 *
 */

{
	ppl7::ID3Tag Tag;
	Tag.setPaddingSize(PaddingSize);
	Tag.setMaxPaddingSpace(PaddingSize);
	uint32_t e;

#ifdef TODO
	if(Tags.)
	ppl7::String NewFilename=ppl7::to7(Tags->Get("renamefile");
	if ((NewFilename.NotEmpty()) && NewFilename!=filename) {
        QFile frename(filename);
        if (!frename.rename(NewFilename)) {
			return 0;
		}
		filename=NewFilename;
		Tags->Delete("renamefile");
	}

	if (!Tag.Load(filename)) {
		e=ppl6::GetErrorCode();
		if (e<402 || e> 405) {
			return 0;
		}
	}
	bool changes=false;
	if (cleartag) {
		// Cover retten, falls vorhanden
		ppl6::CBinary Cover;
		Tag.GetPicture(3,Cover);
		Tag.ClearTags();
		if (Cover.Size()>0) Tag.SetPicture(3,Cover,"image/jpeg");
		changes=true;
	}
	if (Tag.GetArtist()!=Tags->ToCString("artist")) {
		Tag.SetArtist(Tags->Get("artist"));
		changes=true;
	}
	if (Tag.GetTitle()!=Tags->ToCString("title")) {
		Tag.SetTitle(Tags->Get("title"));
		changes=true;
	}
	if (Tag.GetRemixer()!=Tags->ToCString("version")) {
		Tag.SetRemixer(Tags->Get("version"));
		changes=true;
	}
	if (Tag.GetYear()!=Tags->ToCString("year")) {
		Tag.SetYear(Tags->Get("year"));
		changes=true;
	}
	if (Tag.GetTrack()!=Tags->ToCString("track")) {
		Tag.SetTrack(Tags->Get("track"));
		changes=true;
	}
	if (Tag.GetGenre()!=Tags->ToCString("genre")) {
		Tag.SetGenre(Tags->Get("genre"));
		changes=true;
	}
	if (Tag.GetComment()!=Tags->ToCString("comment")) {
		Tag.SetComment(Tags->Get("comment"));
		changes=true;
	}
	if (Tag.GetAlbum()!=Tags->ToCString("album")) {
		Tag.SetAlbum(Tags->Get("album"));
		changes=true;
	}
	if (Tag.GetLabel()!=Tags->ToCString("publisher")) {
		Tag.SetLabel(Tags->Get("publisher"));
		changes=true;
	}
	if (Tag.GetBPM()!=Tags->ToCString("bpm")) {
		Tag.SetBPM(Tags->Get("bpm"));
		changes=true;
	}
	if (Tag.GetEnergyLevel()!=Tags->ToCString("EnergyLevel")) {
		Tag.SetEnergyLevel(Tags->Get("EnergyLevel"));
		changes=true;
	}
	if (Tag.GetKey()!=Tags->ToCString("key")) {
		Tag.SetKey(Tags->Get("key"));
		changes=true;
	}
	if (Tag.GetPopularimeter()!=Tags->ToInt("rating")) {
		Tag.RemovePopularimeter();
		Tag.SetPopularimeter("winmusik@pfp.de", Tags->ToInt("rating"));
		changes=true;
	}

	if (changes==false) {
		return 1;
	}

	if (!Tag.Save()) {
		ppl6::PushError();
		ppl6::PopError();
		return 0;
	}
#endif
}

void CID3TagSaver::SetPaddingSize(int bytes)
{
	Mutex.lock();
	PaddingSize=bytes;
	Mutex.unlock();
}

void CID3TagSaver::SetRetryIntervall(int seconds)
{
	Mutex.lock();
	if (seconds<2) seconds=2;
	RetryIntervall=seconds;
	Mutex.unlock();
}


void CID3TagSaver::run()
{
#ifdef TODO
	ppl6::CAssocArray *Job;
	ppl6::CAssocArray MyJob;
	ppl6::CString key;
	ppluint64 now, retry;
	int idlecount=0;
	while (1) {
		if (ThreadShouldStop()) break;
		Mutex.Lock();
		Todo.Reset();
		if (Todo.Count()==0) {	// Nix zu tun
			idlecount++;
			Mutex.Unlock();
			if (idlecount>10) break;	// Thread wird beendet, da nichts zu tun ist
			ppl6::MSleep(1000);			// Wir warten eine Sekunde
			continue;
		}
		idlecount=0;
		while ((Job=Todo.GetNextArray())) {
			now=ppl6::GetTime();
			if (!Job) break;
			// Gibt es ein Retry Flag?
			retry=ppl6::atoll(Job->Get("retry"));
			if (!retry) break;	// Nein
			if (retry<now) break;	// Retry-Zeitpunkt ist erreicht
		}
		if (!Job) {
			Mutex.Unlock();
			// Nichts zu tun, wir warten
			ppl6::MSleep(1000);
			continue;
		} else {
			MyJob.Copy(Job);
			Todo.GetCurrentKey(key);
			Todo.Delete(key);
			Mutex.Unlock();
			if (!UpdateNow(MyJob.Get("filename"),MyJob.GetArray("tags"),MyJob.IsTrue("cleartag"))) {
				// Update hat nicht funktioniert
				int e=ppl6::GetErrorCode();
				if (e!=364) {	// Datei existiert nicht
					Mutex.Lock();
					MyJob.Setf("retry","%llu",ppl6::GetTime()+10);
					Todo.Set(key,MyJob);
					Mutex.Unlock();
				}
			}
		}
	}
#endif
}


}	// EOF namespace winmusik
}	// EOF namespace pfp
}	// EOF namespace de
