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

#include "wm_cwmfile.h"
#include "wm_storage.h"
#include "wm_exceptions.h"

namespace de {
namespace pfp {
namespace winmusik {

/*!\class CStorageItem
 * \ingroup WM3DataTypes
 * \brief Basisklasse für alle Datenelemente, die in der WinMusik-Datenbank gespeichert werden müssen
 *
 * Dies ist die Basisklasse für alle Datenelemente, die in der WinMusik-Datenbank gespeichert werden müssen.
 * Sie verwaltet die ID der Datenbank-Datei, die Position des Datensatzes in der Datei, Timestamp der letzten
 * Änderung, sowie die Version.
 */

/*!\var CStorageBase::fileid
 * \brief ID der Datenbank-Datei
 */

/*!\var CStorageItem::filepos
 * \brief Position des Datensatzes innerhalb der Datenbank-Datei
 */

/*!\var CStorageItem::lastchange
 * \brief Timestamp der letzten Änderung
 */

/*!\var CStorageItem::version
 * \brief Versionsnummer des Datensatzen
 *
 * Diese Variable enthält die Versionsnummer des Datensatzen. Bei jedem Speichern wird sie um 1 hochgezählt, um bei
 * Nutzung einer Netzwerk-Datenbank feststellen zu können, ob der Datensatz zwischenzeitlich parallel durch einen
 * anderen Client verändert wurde.
 */

/*!\var CStorageItem::formatversion
 * \brief Version des Datensatz-Formates
 *
 * Diese Variable enthält die Version des Datensatz-Formates. Die Version ändert sich nur, wenn
 * am Format des Datensatzes etwas verändert wird. Sie wird dann jeweils um 1 hochgezählt.
 * Die Export-Funktionen der Datentypen speichern jeweils immer im Format der aktuellsten
 * Version, während die Import-Funktionen in der Lage sind auch alle älteren Formate richtig
 * einzulesen. Dies hat den Vorteil, dass bei einer Änderung am Format eines Datentyps die
 * Datenbank nicht aktualisiert werden muss.
 */

CStorageItem::CStorageItem()
/*!\brief Konstruktor der Klasse
 *
 * Der Konstruktor der Klasse initialisiert alle Variablen mit 0.
 */
{
	filepos=0;
	lastchange=0;
	version=0;
	formatversion=0;
}

CStorageItem::CStorageItem(const CStorageItem& other)
/*!\brief Copy-Konstruktor der Klasse
 *
 * Copy-Konstruktor der Klasse
 */
{
	filepos=other.filepos;
	lastchange=other.lastchange;
	version=other.version;
	formatversion=other.formatversion;
}

CStorageItem::~CStorageItem()
/*!\brief Destruktor der Klasse
 *
 * Destruktor der Klasse
 */
{
}

void CStorageItem::Clear()
/*!\brief Setzt alle Variablen der Klasse auf 0
 *
 * Setzt alle Variablen der Klasse auf 0. Die Clear-Funktion einer abgeleiteten Klasse muss
 * sicherstellen, dass am Ende diese Funktion ebenfalls aufgerufen wird.
 */
{
	ClearStorageData();
}

void CStorageItem::ClearStorageData()
/*!\brief Setzt alle Variablen der Klasse auf 0
 *
 * Setzt alle Variablen der Klasse auf 0.
 */
{
	filepos=0;
	lastchange=0;
	version=0;
	formatversion=0;
}

void CStorageItem::CopyStorageFrom(const CStorageItem& other)
/*!\brief Storage Daten kopieren
 *
 * Diese Funktion übernimmt die Storage Daten des angegebenen Datensatzes.
 *
 * \param[in] item Pointer auf einen anderen Datensatz
 */
{
	filepos=other.filepos;
	lastchange=other.lastchange;
	version=other.version;
	formatversion=other.formatversion;
}

void CStorageItem::CopyStorageFrom(const CWMFileChunk& chunk)
/*!\brief Storage Daten kopieren
 *
 * Diese Funktion übernimmt die Storage Daten des angegebenen Chunks.
 *
 * \param[in] chunk Pointer auf einen WinMusik Chunk vom Typ CWMFileChunk
 */
{
	filepos=chunk.GetFilepos();
	lastchange=chunk.GetTimestamp();
	version=chunk.GetVersion();
	formatversion=chunk.GetFormatVersion();
}

uint32_t CStorageItem::GetVersion()
/*!\brief Version auslesen
 *
 * Mit dieser Funktion kann die Version des Datensatzes ausgelesen werden
 *
 * \returns Version des Datensatzes
 */
{
	return version;
}

uint32_t CStorageItem::GetLastChange()
/*!\brief Timestamp der letzten Änderung auslesen
 *
 * Mit dieser Funktion kann der Timestamp der letzten Änderung des Datensatzes ausgelesen werden
 *
 * \returns Timestamp
 */
{
	return lastchange;
}

uint8_t CStorageItem::GetFormatVersion()
/*!\brief Version des Datensatz-Formates auslesen
 *
 * Mit dieser Funktion kann die Version des Datensatz-Formates ausgelesen werden
 *
 * \returns Version des Formates
 */
{
	return formatversion;
}

void CStorageItem::PrintStorageData()
{
	printf("filepos: %u, lastchange: %u, version: %u, format: %u\n",
		filepos, lastchange, version, formatversion);
}



}
}
}	// EOF Namespace de.pfp.winmusik
