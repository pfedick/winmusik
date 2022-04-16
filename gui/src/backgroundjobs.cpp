/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author$
 * $Revision$
 * $Date$
 * $Id$
 *
 *
 * Copyright (c) 2013 Patrick Fedick
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


BackgroundJobs::BackgroundJobs()
{

}

BackgroundJobs::~BackgroundJobs()
{
	shutdown();
}

/*!\brief Hintergrund-Job hinzufügen
 *
 * \desc
 * Fügt einen neuen Job am Ende der Queue ein und startet den Worker-Thread, sofern er noch
 * nicht läuft. Falls doch, wird ihm ein Signal geschickt, damit er aufwacht.
 *
 * @param type String mit der Art des Jobs. Muss einen der folgenden Werte haben:
 * - updateMp3Tags
 * - renameFile
 * - writeTracklist
 * @param data Assoziatives Array mit den für den Job nötigen Parameter. Welche das sind,
 * wird bei den einzelnen Funktionen beschrieben.
 *
 * \see BackgroundJobs::updateMp3Tags, BackgroundJobs::renameFile, BackgroundJobs::writeTracklists
 */
void BackgroundJobs::add(JobType type, const ppl6::CAssocArray &data)
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,5,"BackgroundJobs","add",__FILE__,__LINE__,"Adding Job %s", (const char*)type);

	Mutex.Lock();
	Queue.push(Job(type,data));
	Mutex.Unlock();


	if (!ThreadIsRunning()) {
		if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"BackgroundJobs","add",__FILE__,__LINE__,"Starte Thread");
		if (ThreadStart()) {
			if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"BackgroundJobs","add",__FILE__,__LINE__,"Thread erfolgreich gestartet");
		} else {
			if (wmlog) {
				wmlog->LogError(ppl6::LOG::ERROR,1,"BackgroundJobs","add",__FILE__,__LINE__);
				wmlog->Printf(ppl6::LOG::ERROR,1,"BackgroundJobs","add",__FILE__,__LINE__,"Thread wurde nicht gestartet");
			}
		}
	}
	Mutex.Signal();
}

/*!\brief Anzahl Jobs in der Queue
 *
 * \desc
 * Liefert die Anzahl Jobs zurück, die aktuell in der Queue warten.
 *
 * @return Anzahl wartender Jobs
 */
size_t BackgroundJobs::size() const
{
	return Queue.size();
}

/*!\brief Ist die Queue leer?
 *
 * \desc
 * Prüft, ob die Queue leer ist.
 *
 * @return Liefert \i true zurück, wenn die Queue leer ist, sonst \i false.
 */
bool BackgroundJobs::empty() const
{
	return Queue.empty();
}

/*!\brief Thread beenden
 *
 * \desc
 * Signalisiert dem Thread, dass er sich beenden soll, und wartet darauf.
 * aufgerufen.
 */
void BackgroundJobs::shutdown()
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"BackgroundJobs","shutdown",__FILE__,__LINE__,"wait for worker thread to stop");
	this->ThreadSignalStop();
	Mutex.Signal();
	this->ThreadStop();
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"BackgroundJobs","shutdown",__FILE__,__LINE__,"worker thread stopped");
}

/*!\brief Thread signalisieren, dass er sich beenden soll
 *
 * \desc
 * Signalisiert dem Thread, dass er sich beenden soll, warten aber nicht darauf.
 * aufgerufen.
 */
void BackgroundJobs::triggerShutdown()
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"BackgroundJobs","triggerShutdown",__FILE__,__LINE__,"signal worker thread to shutdown");
	this->ThreadSignalStop();
	Mutex.Signal();
}


/*!\brief Worker-Thread
 *
 * \desc
 * Thread für Hintergrund-Jobs. Wird automatisch gestartet, wenn der erste Job in die Queue gelegt wird,
 * und erst bei Programmende beendet. Er arbeitet alle Jobs in der Queue ab und schläft dann entweder
 * solange, bis das Programm beendet wird, die Funktion BackgroundJobs::shutdown aufgerufen wird oder
 * mit BackgroundJobs::add ein neuer Job in die Queue gelegt wird.
 * \par
 * Je nach Art des Jobs wird eine der folgenden Unterfunktionen aufgerufen:
 * - BackgroundJobs::updateMp3Tags
 * - BackgroundJobs::renameFile
 * - BackgroundJobs::writeTracklists
 *
 * @param param Wird nicht verwendet
 */
void BackgroundJobs::ThreadMain(void *)
{
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"BackgroundJobs","ThreadMain",__FILE__,__LINE__,"Thread is running");
	while (1) {
		if (this->ThreadShouldStop()) break;			// Thread beenden?
		Mutex.Lock();
		if (!Queue.empty()) {
			Job j=Queue.front();
			Queue.pop();
			Mutex.Unlock();
			try {
				if (j.Type==TypeWriteTracklist) writeTracklists(j.Data);
				else if (j.Type==TypeRenameFile) renameFile(j.Data);
				else if (j.Type==TypeWriteTracklist) updateMp3Tags(j.Data);
				else {
					if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,5,"BackgroundJobs","ThreadMain",__FILE__,__LINE__,"Unknown Job is ignored an removed from queue: %s",(const char*)j.Type);
					continue;
				}
			} catch ( const ppl6::Exception &e) {
				j.ErrorCount++;
				j.LastError=e.toString();
				if (j.ErrorCount>10) {
					if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,5,"BackgroundJobs","ThreadMain",__FILE__,__LINE__,"Job fails after %u retries, Job is removed from queue: %s",j.ErrorCount, (const char*)j.Type);
					Mutex.Lock();
					Errors.push(j);		// In die Error-Queue
					Mutex.Unlock();
					continue;
				}
				ppl6::MSleep(60);
				Mutex.Lock();			// Zurück in die Queue
				Queue.push(j);
				Mutex.Unlock();
			}
		} else {
			if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,5,"BackgroundJobs","ThreadMain",__FILE__,__LINE__,"Nothing in queue, Thread sleeps");
			Mutex.Wait();
			if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,5,"BackgroundJobs","ThreadMain",__FILE__,__LINE__,"Thread awakes");
			Mutex.Unlock();
		}
	}
	if (wmlog) wmlog->Printf(ppl6::LOG::DEBUG,1,"BackgroundJobs","ThreadMain",__FILE__,__LINE__,"Thread is shutting down");
}

void BackgroundJobs::updateMp3Tags(const ppl6::CAssocArray &data)
{

}

void BackgroundJobs::renameFile(const ppl6::CAssocArray &data)
{

}

void BackgroundJobs::writeTracklists(const ppl6::CAssocArray &data)
{

}
