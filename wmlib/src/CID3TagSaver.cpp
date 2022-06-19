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

#include "wm_id3tagsaver.h"

#include "ppl7-audio.h"


namespace de {
namespace pfp {
namespace winmusik {


CID3TagSaver::WorkItem::WorkItem()
{
	retry=0;
	retry_counter=0;
	cleartag=false;
}

CID3TagSaver::WorkItem::WorkItem(const ppl7::String& filename, const ppl7::AssocArray& Tags, bool cleartag)
{
	this->Filename=filename;
	this->Tags=Tags;
	retry=0;
	this->cleartag=cleartag;
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
	logger=NULL;
}

CID3TagSaver::~CID3TagSaver()
/*!\brief Destruktor der Klasse
 *
 * Der Destruktor der Klasse sorgt dafür, dass ein eventuell laufender Update-Thread nach
 * Beendigung seiner aktuellen Aktion gestoppt wird. Falls zu diesem Zeitpunkt noch unerledigte
 * Aufträge in der Queue sind, werden diese nicht mehr abgearbeitet.
 */
{
	if (logger) logger->printf(ppl7::Logger::DEBUG,1,"CID3TagSaver","~CID3TagSaver",__FILE__,__LINE__,"Stoppe Thread");
	threadSignalStop();
	threadStop();
	if (logger) logger->printf(ppl7::Logger::DEBUG,1,"CID3TagSaver","~CID3TagSaver",__FILE__,__LINE__,"done");
}

void CID3TagSaver::SetLogger(ppl7::Logger *logger)
{
	this->logger=logger;
}

void CID3TagSaver::Add(const ppl7::String& filename, const ppl7::AssocArray& Tags, bool cleartag)
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
 * \arg \c renamefile Datei umbenennen in neuen Namen
 * \param[in] cleartag Hat diese Variable den Wert \c true, wird ein vorhandener ID3v2-Tag zunächst
 * gelöscht und mit dem neuen überschrieben. Ist dies nicht der Fall, werden nur die gesetzten Werte
 * überschrieben.
 * \param[in] writev1 Wenn gesetzt, wird ein ID3v1-Tag geschrieben (=default)
 * \param[in] writev2 Wenn gesetzt, wird ein ID3v2-Tag geschrieben (=default)
 *
 */
{
	CID3TagSaver::WorkItem item(filename, Tags, cleartag);
	Mutex.lock();
	Queue.push_back(item);
	Mutex.unlock();
	if (logger) logger->printf(ppl7::Logger::DEBUG,2,"CID3TagSaver","Add",__FILE__,__LINE__,"Adding File %s", (const char*)item.Filename);
	if (!this->threadIsRunning()) {
		this->threadStart();
	}
}


void CID3TagSaver::UpdateNow(CID3TagSaver::WorkItem& item)
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
	if (item.Tags.exists("renamefile") == true && item.Tags.getString("renamefile") != item.Filename) {
		if (logger) logger->printf(ppl7::Logger::DEBUG,9,"CID3TagSaver","run",__FILE__,__LINE__,"UpdateNow, rename: %s => %s",
			(const char*)item.Filename, (const char*)item.Tags.getString("renamefile"));
		ppl7::File::rename(item.Filename, item.Tags.getString("renamefile"));
		item.Filename=item.Tags.getString("renamefile");
		item.Tags.remove("renamefile");
	}
	if (logger) logger->printf(ppl7::Logger::DEBUG,9,"CID3TagSaver","run",__FILE__,__LINE__,"UpdateNow, tagging: %s", (const char*)item.Filename);

	try {
		//could fail, if file does not exist or contains usupported ID3Tags
		Tag.load(item.Filename);
	} catch (...) {}
	bool changes=false;
	ppl7::Array changelist;
	if (item.cleartag) {
		// Cover retten, falls vorhanden
		ppl7::ByteArray Cover;
		Tag.getPicture(3, Cover);
		Tag.clearTags();
		if (Cover.size() > 0) Tag.setPicture(3, Cover, "image/jpeg");
		changelist.add("clear tags");
		changes=true;
	}
	ppl7::String empty;
	if (item.Tags.getString("artist", empty) != Tag.getArtist()) {
		Tag.setArtist(item.Tags.getString("artist", empty));
		changes=true;
		changelist.add("artist");
	}
	if (Tag.getTitle() != item.Tags.getString("title", empty)) {
		Tag.setTitle(item.Tags.getString("title", empty));
		changes=true;
		changelist.add("title");
	}
	if (Tag.getRemixer() != item.Tags.getString("version", empty)) {
		Tag.setRemixer(item.Tags.getString("version", empty));
		changes=true;
		changelist.add("version");
	}
	if (Tag.getYear() != item.Tags.getString("year", empty)) {
		Tag.setYear(item.Tags.getString("year", empty));
		changes=true;
		changelist.add("year");
	}
	if (Tag.getTrack() != item.Tags.getString("track", empty)) {
		Tag.setTrack(item.Tags.getString("track", empty));
		changes=true;
		changelist.add("track");
	}
	if (Tag.getGenre() != item.Tags.getString("genre", empty)) {
		Tag.setGenre(item.Tags.getString("genre", empty));
		changes=true;
		changelist.add("genre");
	}
	if (Tag.getComment() != item.Tags.getString("comment", empty)) {
		Tag.setComment(item.Tags.getString("comment", empty));
		changes=true;
		changelist.add("comment");
	}
	if (Tag.getAlbum() != item.Tags.getString("album", empty)) {
		Tag.setAlbum(item.Tags.getString("album", empty));
		changes=true;
		changelist.add("album");
	}
	if (Tag.getLabel() != item.Tags.getString("publisher", empty)) {
		Tag.setLabel(item.Tags.getString("publisher", empty));
		changes=true;
		changelist.add("publisher");
	}
	if (Tag.getBPM() != item.Tags.getString("bpm", empty)) {
		Tag.setBPM(item.Tags.getString("bpm", empty));
		changes=true;
		changelist.add("bpm");
	}
	if (Tag.getEnergyLevel() != item.Tags.getString("EnergyLevel", empty)) {
		Tag.setEnergyLevel(item.Tags.getString("EnergyLevel", empty));
		changes=true;
		changelist.add("energylevel");
	}
	ppl7::String key=item.Tags.getString("key", empty);
	key.replace("♯", "#");
	key.replace("♭", "b");
	if (Tag.getKey() != key) {
		changelist.addf("key %s=>%s", (const char*)Tag.getKey(), (const char*)key );
		Tag.setKey(key);
		changes=true;
		
	}
	empty.set("0");
	if (Tag.getPopularimeter() != item.Tags.getString("rating", empty).toInt()) {
		Tag.removePopularimeter();
		Tag.setPopularimeter("winmusik@pfp.de", item.Tags.getString("rating", empty).toInt());
		changes=true;
		if (logger) logger->print(ppl7::Logger::DEBUG,10,"CID3TagSaver","run",__FILE__,__LINE__,"changes: popularimeter");
	}

	if (changes == false) {
		if (logger) logger->printf(ppl7::Logger::DEBUG,10,"CID3TagSaver","run",__FILE__,__LINE__,"UpdateNow, no changes: %s", (const char*)item.Filename);
		return;
	}
	if (logger) {
		ppl7::String Tmp=changelist.implode(", ");
		logger->printf(ppl7::Logger::DEBUG,10,"CID3TagSaver","run",__FILE__,__LINE__,"UpdateNow, write changes: %s [%s]",
			(const char*)item.Filename, (const char*)Tmp);
	}
	Tag.save();
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

void CID3TagSaver::IterateQueue()
{
	std::list<WorkItem>::iterator it;
	ppl7::ppl_time_t now=ppl7::GetTime();
	for (it=Queue.begin();it != Queue.end();++it) {
		if ((*it).retry == 0 || (*it).retry < now) {
			WorkItem item=(*it);
			Queue.erase(it);
			Mutex.unlock();
			try {
				UpdateNow(item);
				return;
			} catch (...) {
				if (item.retry_counter < 5) {
					item.retry=ppl7::GetTime() + 5;
					item.retry_counter++;
					Mutex.lock();
					Queue.push_back(item);
					Mutex.unlock();
					return;
				}
			}
		}
	}
	Mutex.unlock();
	ppl7::MSleep(200);	// We couldn't do anything in this run, let's wait a bit
}


void CID3TagSaver::run()
{
	if (logger) logger->printf(ppl7::Logger::DEBUG,1,"CID3TagSaver","run",__FILE__,__LINE__,"Thread started");
	int idlecount=0;
	while (!this->threadShouldStop()) {
		Mutex.lock();
		if (Queue.empty()) {
			if (logger!=NULL && idlecount==0) logger->printf(ppl7::Logger::DEBUG,5,"CID3TagSaver","run",__FILE__,__LINE__,"Queue is empty");
			Mutex.unlock();
			idlecount++;
			if (idlecount > 20) break;	// terminate thread, because we have nothing to do
			ppl7::MSleep(200); // Queue is empty, let's wait a bit
		} else {
			if (logger) logger->printf(ppl7::Logger::DEBUG,5,"CID3TagSaver","run",__FILE__,__LINE__,"Jobs in queue: %zd",Queue.size());
			idlecount=0;
			IterateQueue();
		}
	}
	if (logger) logger->printf(ppl7::Logger::DEBUG,1,"CID3TagSaver","run",__FILE__,__LINE__,"Thread ended");
}


}	// EOF namespace winmusik
}	// EOF namespace pfp
}	// EOF namespace de
