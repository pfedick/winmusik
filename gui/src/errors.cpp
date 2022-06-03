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


#include <QObject>
#include "winmusik3.h"

static const char* errors[] ={
	"No Error",															// 20000
	"Keine Konfigurationsdatei gefunden. Erster Start?",				// 20001
	"Keine WinMusik3-Datei",											// 20002		(20000)
	"Komprimierte Daten-Datei wird nicht unterstützt",					// 20003		(20001)
	"Keine Datei geöffnet",												// 20004		(20002)
	"Datei oder Datenchunk ist korrupt",								// 20005		(20003)
	"Daten-Chunk konnte nicht gelesen werden",							// 20006		(20004)
	"Keine weiteren Daten-Chunks vorhanden",							// 20007		(20005)
	"Unerlaubter ChunkName",											// 20008		(20006)
	"Datensatz wurde zwischenzeitlich verändert",						// 20009		(20007)
	"TitelId ist nicht vorhanden",										// 20010		(20008)
	"Ungültige Daten, Import als Titel nicht möglich",					// 20011		(20009)
	"Prüfsummenfehler, Import als Titel nicht möglich",					// 20012		(20010)
	"Kein gültiger Tabelleneintrag, Import nicht möglich",				// 20013		(20011)
	"Storage-Klasse ist nicht initialisiert",							// 20014		(20012)
	"ID ist nicht in Tabelle vorhanden",								// 20015		(20013)
	"Kein gültiger Shortcut, Import nicht möglich",						// 20016
	"Ungültiger Track",													// 20017
	"Track nicht vorhanden",											// 20018
	"Klasse wurde nicht durch CTrackStore erstellt, speichern nicht möglich",	// 20019
	"Ungültiger Tonträger, Import nicht möglich",						// 20020
	"Import: Tracktabelle ist ungültig",								// 20021
	"Datei nicht gefunden",												// 20022
	"Unbekannte Version des Datensatz-Formates",						// 20023
	"Ungültiger Datensatz, Import nicht möglich",						// 20024
	"Datensatz nicht vorhanden",										// 20025
	"Kein gültiger Track, Import nicht möglich",						// 20026
	"Druckvorgang kann nicht gestartet werden",							// 20027
	"Keine Suchbegriffe angegeben",										// 20028
	"Registration: Registration server encountered a database error. Please try later",	// 20029
	"Registration: Invalid Session",									// 20030
	"Registration: WinMusik has send an unknown command to the registration server. Please report as a bug.",	// 20031
	"Registration: A required variable was not send or empty",			// 20032
	"Registration: Registration server could not send registration email. Please try again",	// 20033
	"Registration: Invalid registration key",							// 20034
	"Registration: Unknown registration key",							// 20035
	"Registration: Can't identify your name or email with this registration key",	// 20036
	"Keine neue Version verfügbar",										// 20037
	"Shortcut darf nicht leer sein",									// 20038
	"Coverdruck steht für diese Tonträgerart nicht zur Verfügung",		// 20039
	"Kein Tonträger ausgewählt",										// 20040
	"Tonträger enthält keine Titel",									// 20041
	"Es wurde kein MP3-Archiv in den Einstellungen definiert",			// 20042
	"Invalid configuration file",										// 20043



	"unknown"
};

/*
static const pplchar* geterror(pplint32 code)
{
	pplint32 max=sizeof(errors) / sizeof(char*) - 2;
	if (code < 20000 || code >20999) return "unknown";
	code-=20000;
	if (code <= max) return ((pplchar*)errors[code]);
	return "unknown";
}

void CWmClient::InitErrors()
{
	ppl6::AttachErrorHandler(geterror, 20000, 20999);
}
*/
