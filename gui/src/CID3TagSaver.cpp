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


#include "winmusik3.h"

#include "ppl6-sound.h"

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
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","~CID3TagSaver",__FILE__,__LINE__,"Stoppe Thread");
	ThreadSignalStop();
	ThreadStop();
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","~CID3TagSaver",__FILE__,__LINE__,"done");
}

void CID3TagSaver::Add(const char *filename, ppl6::CAssocArray *Tags, bool cleartag, bool writev1, bool writev2)
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
	ppl6::CAssocArray a;
	a.Set("tags",Tags);
	a.Set("filename",filename);
	if (cleartag) a.Set("cleartag","true");
	else a.Set("cleartag","false");
	if (writev1) a.Set("writev1","true");
	else a.Set("writev1","false");
	if (writev2) a.Set("writev2","true");
	else a.Set("writev2","false");

	Mutex.Lock();
	Todo.Set("[]",&a);
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,2,"CID3TagSaver","Add",__FILE__,__LINE__,"Adding File %s", filename);
	if (!ThreadIsRunning()) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","Add",__FILE__,__LINE__,"Starte Thread");
		if (ThreadStart()) {
			if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","Add",__FILE__,__LINE__,"Thread erfolgreich gestartet");
		} else {
			if (wmlog) {
				wmlog->LogError(ppl6::LOG::DEBUG,1,"CID3TagSaver","Add",__FILE__,__LINE__);
				wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","Add",__FILE__,__LINE__,"Thread wurde nicht gestartet");
			}
		}
	}
	Mutex.Unlock();
}


int CID3TagSaver::UpdateNow(const char *filename, ppl6::CAssocArray *Tags, bool cleartag)
/*!\brief Auftrag ausführen
 *
 * Mit dieser Funktion wird ein ID3-Tag sofort in die angegebene Datei geschrieben, ohne
 * ihn in die Queue zu stellen. Die Funktion kehrt erst wieder zurück, wenn der Auftrag
 * beendet ist.
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
 *
 * \returns Bei Erfolg liefert die Funktion 1 zurück, sonst 0.
 *
 */

{
	if (wmlog) {
		wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","UpdateNow",__FILE__,__LINE__,"UpdateNow: %s",filename);
		wmlog->PrintArray(ppl6::LOG::DEBUG,5,"CID3TagSaver","UpdateNow",__FILE__,__LINE__,Tags,"tags:");
	}

	ppl6::CID3Tag Tag;
	if (wmlog) Tag.SetLogfile(wmlog);
	Tag.SetPaddingSize(PaddingSize);
	Tag.SetMaxPaddingSpace(PaddingSize);

	ppluint32 e;

	ppl6::CString NewFilename=Tags->Get("renamefile");
	if ((NewFilename.NotEmpty()) && NewFilename!=filename) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,7,"CID3TagSaver","UpdateNow",__FILE__,__LINE__,"File should be renamed from >>%s<< to >>%s<<",filename,(const char*)NewFilename);
        QFile frename(filename);
        if (!frename.rename(NewFilename)) {
			if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","UpdateNow",__FILE__,__LINE__,"ERROR, rename failed");
			return 0;
		}
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,7,"CID3TagSaver","UpdateNow",__FILE__,__LINE__,"File successfully renamed");
		filename=NewFilename;
		Tags->Delete("renamefile");
	}

	if (!Tag.Load(filename)) {
		e=ppl6::GetErrorCode();
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","UpdateNow",__FILE__,__LINE__,"UpdateNow Tag.Load failed: %u",e);
		if (e<402 || e> 405) {
			if (wmlog) wmlog->LogError();
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
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","UpdateNow",__FILE__,__LINE__,"Tags did not change, skipping update: %s",filename);
		return 1;
	}

	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","UpdateNow",__FILE__,__LINE__,"Saving: %s",filename);
	if (!Tag.Save()) {
		ppl6::PushError();
		if (wmlog) {
			wmlog->LogError();
			wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","UpdateNow",__FILE__,__LINE__,"Failed: %s",filename);
		}
		ppl6::PopError();
		return 0;
	}
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","UpdateNow",__FILE__,__LINE__,"Done: %s",filename);
	return 1;
}

void CID3TagSaver::SetPaddingSize(int bytes)
{
	Mutex.Lock();
	PaddingSize=bytes;
	Mutex.Unlock();
}

void CID3TagSaver::SetRetryIntervall(int seconds)
{
	Mutex.Lock();
	if (seconds<2) seconds=2;
	RetryIntervall=seconds;
	Mutex.Unlock();
}


void CID3TagSaver::ThreadMain(void *)
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","ThreadMain",__FILE__,__LINE__,"ThreadMain");
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
			if (wmlog) {
				wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","ThreadMain",__FILE__,__LINE__,"Key: >>%s<<",(const char*)key);
				wmlog->PrintArray(ppl6::LOG::DEBUG,5,"CID3TagSaver","ThreadMain",__FILE__,__LINE__,&MyJob,"MyJob");
			}
			if (!UpdateNow(MyJob.Get("filename"),MyJob.GetArray("tags"),MyJob.IsTrue("cleartag"))) {
				// Update hat nicht funktioniert
				int e=ppl6::GetErrorCode();
				if (e!=364) {	// Datei existiert nicht
					Mutex.Lock();
					MyJob.Setf("retry","%llu",ppl6::GetTime()+10);
					Todo.Set(key,MyJob);
					if (wmlog) {
						wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","ThreadMain",__FILE__,__LINE__,"Retry");
					}
					Mutex.Unlock();
				} else {
					if (wmlog) {
						wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","ThreadMain",__FILE__,__LINE__,"Auftrag wird aus TODO-Liste gelöscht");
					}
				}
			}
		}
	}
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"CID3TagSaver","ThreadMain",__FILE__,__LINE__,"Thread wurde beendet");
}
