/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.2 $
 * $Date: 2010/05/16 12:40:40 $
 * $Id: InitialDatabase.cpp,v 1.2 2010/05/16 12:40:40 pafe Exp $
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

static const char * versions[] = {
		"Single",
		"Maxi",
		"Album",
		"Original Version",
		"Original Mix",
		"Club Mix",
		"Remix",
		"Radio Edit",
		"Radio Mix",

		NULL
};

typedef struct {
	const char *shortcut;
	const char *artist;
} ARTISTSHORTCUTS;

static ARTISTSHORTCUTS shortcuts[] = {
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
	if (!Storage.DeleteDatabase()) return 0;

	// First, we create some common used versions
	DataVersion v;
	for (int i=0;;i++) {
		if (versions[i]==NULL) break;
		v.Clear();
		if (!v.SetValue(versions[i])) return 0;
		if (!VersionStore.Put(&v)) return 0;
	}

	// Now the default genres defined by http://www.id3.org/
	const char *genre;
	DataGenre g;
	for (int i=0;;i++) {
		genre=ppl6::GetID3GenreName(i);
		if(!genre) break;
		g.Clear();
		if (!g.SetValue(genre)) return 0;
		if (!GenreStore.Put(&g)) return 0;
	}

	// A few artist shortcuts
	DataShortcut s;
	for (int i=0;;i++) {
		if (!shortcuts[i].shortcut) break;
		s.Clear();
		if (!s.SetValue(shortcuts[i].shortcut,shortcuts[i].artist)) return 0;
		if (!ShortcutStore.Put(&s)) return 0;
	}



	return 1;
}
