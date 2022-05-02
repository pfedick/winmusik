/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2022 Patrick Fedick
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

#include "ppl7-audio.h"

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
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CID3TagSaver", "~CID3TagSaver", __FILE__, __LINE__, "Stoppe Thread");
	threadSignalStop();
	threadStop();
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CID3TagSaver", "~CID3TagSaver", __FILE__, __LINE__, "done");
}

void CID3TagSaver::Add(const ppl7::String& filename, const ppl7::AssocArray& Tags, bool cleartag)
/*!\brief Auftrag in die Queue stellen
 *
 * Mit dieser Funktion wird ein neuer Update-Auftrag in die Queue gestellt.
 *
 * \param[in] filename Dateinamen
 * \param[in] Tags Assoziatives Array, das die zu schreibenden Tags enthält. Folgende
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
 */
{
	CID3TagSaver::QueueItem item;
	item.filename=filename;
	item.Tags=Tags;
	item.cleartag=cleartag;
	Mutex.lock();
	Queue.push(item);
	if (!threadIsRunning()) {
		threadStart();
	}
	Mutex.unlock();
}


int CID3TagSaver::UpdateNow(const ppl7::String& filename, const ppl7::AssocArray& Tags, bool cleartag)
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
 *
 * \returns Bei Erfolg liefert die Funktion 1 zurück, sonst 0.
 *
 */

{
	ppl7::ID3Tag Tag;

	Tag.setPaddingSize(PaddingSize);
	Tag.setMaxPaddingSpace(PaddingSize);
	ppl7::String NewFilename=filename;
	ppluint32 e;
	if (Tags.exists("renamefile")) {
		ppl7::String NewFilename=Tags.getString("renamefile");
		if ((NewFilename.notEmpty()) && NewFilename != filename) {
			QFile frename(filename);
			if (!frename.rename(NewFilename)) {
				return 0;
			}
			if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 7, "CID3TagSaver", "UpdateNow", __FILE__, __LINE__, "File successfully renamed");
			filename=NewFilename;
			Tags->Delete("renamefile");
		}
	}

	ppl6::CString NewFilename=Tags->Get("renamefile");
	if ((NewFilename.NotEmpty()) && NewFilename != filename) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 7, "CID3TagSaver", "UpdateNow", __FILE__, __LINE__, "File should be renamed from >>%s<< to >>%s<<", filename, (const char*)NewFilename);
		QFile frename(filename);
		if (!frename.rename(NewFilename)) {
			if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CID3TagSaver", "UpdateNow", __FILE__, __LINE__, "ERROR, rename failed");
			return 0;
		}
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 7, "CID3TagSaver", "UpdateNow", __FILE__, __LINE__, "File successfully renamed");
		filename=NewFilename;
		Tags->Delete("renamefile");
	}

	if (!Tag.Load(filename)) {
		e=ppl6::GetErrorCode();
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CID3TagSaver", "UpdateNow", __FILE__, __LINE__, "UpdateNow Tag.Load failed: %u", e);
		if (e < 402 || e> 405) {
			if (wmlog) wmlog->LogError();
			return 0;
		}
	}
	bool changes=false;
	if (cleartag) {
		// Cover retten, falls vorhanden
		ppl6::CBinary Cover;
		Tag.GetPicture(3, Cover);
		Tag.ClearTags();
		if (Cover.Size() > 0) Tag.SetPicture(3, Cover, "image/jpeg");
		changes=true;
	}
	if (Tag.GetArtist() != Tags->ToCString("artist")) {
		Tag.SetArtist(Tags->Get("artist"));
		changes=true;
	}
	if (Tag.GetTitle() != Tags->ToCString("title")) {
		Tag.SetTitle(Tags->Get("title"));
		changes=true;
	}
	if (Tag.GetRemixer() != Tags->ToCString("version")) {
		Tag.SetRemixer(Tags->Get("version"));
		changes=true;
	}
	if (Tag.GetYear() != Tags->ToCString("year")) {
		Tag.SetYear(Tags->Get("year"));
		changes=true;
	}
	if (Tag.GetTrack() != Tags->ToCString("track")) {
		Tag.SetTrack(Tags->Get("track"));
		changes=true;
	}
	if (Tag.GetGenre() != Tags->ToCString("genre")) {
		Tag.SetGenre(Tags->Get("genre"));
		changes=true;
	}
	if (Tag.GetComment() != Tags->ToCString("comment")) {
		Tag.SetComment(Tags->Get("comment"));
		changes=true;
	}
	if (Tag.GetAlbum() != Tags->ToCString("album")) {
		Tag.SetAlbum(Tags->Get("album"));
		changes=true;
	}
	if (Tag.GetLabel() != Tags->ToCString("publisher")) {
		Tag.SetLabel(Tags->Get("publisher"));
		changes=true;
	}
	if (Tag.GetBPM() != Tags->ToCString("bpm")) {
		Tag.SetBPM(Tags->Get("bpm"));
		changes=true;
	}
	if (Tag.GetEnergyLevel() != Tags->ToCString("EnergyLevel")) {
		Tag.SetEnergyLevel(Tags->Get("EnergyLevel"));
		changes=true;
	}
	if (Tag.GetKey() != Tags->ToCString("key")) {
		Tag.SetKey(Tags->Get("key"));
		changes=true;
	}
	if (Tag.GetPopularimeter() != Tags->ToInt("rating")) {
		Tag.RemovePopularimeter();
		Tag.SetPopularimeter("winmusik@pfp.de", Tags->ToInt("rating"));
		changes=true;
	}

	if (changes == false) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CID3TagSaver", "UpdateNow", __FILE__, __LINE__, "Tags did not change, skipping update: %s", filename);
		return 1;
	}

	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CID3TagSaver", "UpdateNow", __FILE__, __LINE__, "Saving: %s", filename);
	if (!Tag.Save()) {
		ppl6::PushError();
		if (wmlog) {
			wmlog->LogError();
			wmlog->Printf(ppl6::LOG::DEBUG, 1, "CID3TagSaver", "UpdateNow", __FILE__, __LINE__, "Failed: %s", filename);
		}
		ppl6::PopError();
		return 0;
	}
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CID3TagSaver", "UpdateNow", __FILE__, __LINE__, "Done: %s", filename);
	return 1;
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
	if (seconds < 2) seconds=2;
	RetryIntervall=seconds;
	Mutex.unlock();
}


void CID3TagSaver::run()
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CID3TagSaver", "ThreadMain", __FILE__, __LINE__, "ThreadMain");
	ppl6::CAssocArray* Job;
	ppl6::CAssocArray MyJob;
	ppl6::CString key;
	ppluint64 now, retry;
	int idlecount=0;
	while (1) {
		if (threadShouldStop()) break;
		Mutex.lock();
		Todo.Reset();
		if (Todo.Count() == 0) {	// Nix zu tun
			idlecount++;
			Mutex.unlock();
			if (idlecount > 10) break;	// Thread wird beendet, da nichts zu tun ist
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
			if (retry < now) break;	// Retry-Zeitpunkt ist erreicht
		}
		if (!Job) {
			Mutex.unlock();
			// Nichts zu tun, wir warten
			ppl6::MSleep(1000);
			continue;
		} else {
			MyJob.Copy(Job);
			Todo.GetCurrentKey(key);
			Todo.Delete(key);
			Mutex.unlock();
			if (wmlog) {
				wmlog->Printf(ppl6::LOG::DEBUG, 1, "CID3TagSaver", "ThreadMain", __FILE__, __LINE__, "Key: >>%s<<", (const char*)key);
				wmlog->PrintArray(ppl6::LOG::DEBUG, 5, "CID3TagSaver", "ThreadMain", __FILE__, __LINE__, &MyJob, "MyJob");
			}
			if (!UpdateNow(MyJob.Get("filename"), MyJob.GetArray("tags"), MyJob.IsTrue("cleartag"))) {
				// Update hat nicht funktioniert
				int e=ppl6::GetErrorCode();
				if (e != 364) {	// Datei existiert nicht
					Mutex.lock();
					MyJob.Setf("retry", "%llu", ppl6::GetTime() + 10);
					Todo.Set(key, MyJob);
					if (wmlog) {
						wmlog->Printf(ppl6::LOG::DEBUG, 1, "CID3TagSaver", "ThreadMain", __FILE__, __LINE__, "Retry");
					}
					Mutex.unlock();
				} else {
					if (wmlog) {
						wmlog->Printf(ppl6::LOG::DEBUG, 1, "CID3TagSaver", "ThreadMain", __FILE__, __LINE__, "Auftrag wird aus TODO-Liste gelöscht");
					}
				}
			}
		}
	}
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG, 1, "CID3TagSaver", "ThreadMain", __FILE__, __LINE__, "Thread wurde beendet");
}
