/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2024, Patrick Fedick
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

static const char* versions[] ={
		"Single",
		"Maxi",
		"Album",
		"Original Version",
		"Original Mix",
		"Club Mix",
		"Extended Mix",
		"Remix",
		"Radio Edit",
		"Radio Mix",

		NULL
};

typedef struct {
	const char* shortcut;
	const char* artist;
} ARTISTSHORTCUTS;

static ARTISTSHORTCUTS shortcuts[] ={
		{ "psb","Pet Shop Boys" },
		{ "tafkap","The Artist formerly known as Prince" },
		{ NULL,NULL }
};

int CWmClient::CreateInitialDatabase()
/*!\brief Initiale Datenbank anlegen
 *
 * Diese Funktion wird während des Setups aufgerufen, um eine initiale Datenbank anzulegen,
 * sofern keine Daten einer früheren WinMusik Version importiert werden sollen.
 *
 * Die Datenbank enthält einige Versionen, alle durch ID3v1 definierten Genres (inklusive
 * Winamp-Erweiterungen) sowie ein paar Abkürzungen.
 *
 * \returns Bei Erfolg liefert die Funktion 1 zurück, sonst 0.
 *
 */
{
	Storage.DeleteDatabase();
	// First, we create some common used versions
	for (int i=0;;i++) {
		if (versions[i] == NULL) break;
		DataVersion v;
		v.SetValue(versions[i]);
		VersionStore.Put(v);
	}

	// Now the default genres defined by http://www.id3.org/
	try {
		for (int i=0;;i++) {
			ppl7::String genre=ppl7::GetID3GenreName(i);
			if (genre.isEmpty()) break;
			DataGenre g;
			g.SetValue(genre);
			GenreStore.Put(g);
		}
	} catch (const ppl7::InvalidGenreException&) {}

	// A few artist shortcuts
	for (int i=0;;i++) {
		if (!shortcuts[i].shortcut) break;
		DataShortcut s;
		s.SetValue(shortcuts[i].shortcut, shortcuts[i].artist);
		ShortcutStore.Put(s);
	}
	return 1;
}
