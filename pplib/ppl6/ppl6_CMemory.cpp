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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "ppl6.h"


namespace ppl6 {


/*!\class CMemory
 * \ingroup PPLGroupMemory
 * \brief Verwaltung eines einzelnen Speicherblocks
 *
 * \desc
 * Mit dieser Klasse kann ein einzelner Speicherblock verwaltet werden.
 * Dieser besteht immer aus einer Speicheraddresse und der Größe des
 * Speicherbereichs in Bytes.
 * \par
 * Der Speicherbereich kann mit CMemory::set gesetzt werden und mit CMemory::adr und CMemory::size
 * ausgelesen werden. Außerdem kann mit dem Operator [] ein bestimmtes Byte aus dem
 * Speicherbereich ausgelesen werden.
 * \par
 * Im Gegensatz zu CMemoryReference verwaltet die Klasse den Speicherbereich selbst. Das heisst
 * der verwaltete Speicher wird bei Veränderungen oder Beendigung der Klasse automatisch freigegeben.
 * \par
 * Mit CMemory::malloc oder CMemory::calloc wird Speicher allokiert, mit CMemory::free kann er
 * vorzeitig wieder freigegeben werden. Mit CMemory::copy wird der Speicher eines anderen CMemory-
 * oder CMemoryReference-Objekts oder aus einer angegebenen Speicheradresse dupliziert.
 * \par
 * Mit CMemory::set oder durch den Konstruktor angegebener Speicherbereich wird nicht dupliziert,
 * wird aber fortan von der Klasse verwaltet, das heisst die Anwendung darf diesen nicht selbst
 * freigeben.
 *
 * \see
 * - CMemoryReference
 *
 * \since
 * Diese Klasse wurde mit Version 6.3.3 eingeführt
 */

/*!\var CMemory::manage
 * \brief Flag, das die Verwaltung des Speichers regelt.
 *
 * \desc
 * Enthält dieses Flag \c true, wird der Speicher von der CMemory-Klasse verwaltet. Das heisst
 * beim Aufruf des Konstruktors oder Aufruf einer Funktion, die eine Änderung des Speicherblocks
 * bewirkt, wird der zuvor referenzierte Speicher freigegeben.
 */

/*!\brief Konstruktor ohne Argumente
 *
 * \desc
 * Dieser Konstruktor erstellt eine Inszanz der Klasse ohne einen zugeweisenen Speicherbereich
 *
 */
CMemory::CMemory()
{
	ptr=NULL;
	s=0;
}

/*!\brief Destruktor der Klasse
 *
 * \desc
 * Der Destruktor sort dafür, dass der allokierte Speicher mittes "free" wieder freigegeben wird.
 *
 */
CMemory::~CMemory()
{
	::free(ptr);
}

/*!\brief Copy-Konstruktor
 *
 * \desc
 * Mit diesem Konstruktor wird der Speicherbereich eines anderen CMemory- oder CMemoryReference-
 * Objekts kopiert.
 *
 * @param[in] other Referenz auf eine andere CMemory- oder CMemoryReference-Klasse
 *
 * \exception OutOfMemoryException Diese Exception wird geworfen, wenn der Speicher nicht allokiert werden konnte
 */
CMemory::CMemory(const CMemoryReference &other)
{
	if (other.ptr) {
		ptr=::malloc(other.s);
		if (!ptr) throw OutOfMemoryException();
		memcpy(ptr,other.ptr,other.s);
		s=other.s;
	} else {
		ptr=NULL;
		s=0;
	}
}

/*!\brief Konstruktor mit Kopieren aus einem CBinary-Objekt
 *
 * \desc
 * Mit diesem Konstruktor wird der Speicher eines CBinary-Objekts
 * kopiert.
 *
 * @param[in] bin Referenz auf ein CBinary-Objekt
 *
 */
CMemory::CMemory(const CBinary &bin)
{
	ptr=NULL;
	s=0;
	copy(bin.GetPtr(),bin.GetSize());
}

/*!\brief Konstruktor mit Angabe einer Speicheradresse und Größe
 *
 * \desc
 * Mit diesem Konstruktor wird eine neue Instanz der Klasse erzeugt, die die Verwaltung des
 * mit \p adr und \p size angegebenen Speicherbereichs übernimmt. Die Anwendung darf diesen daher
 * nicht selbst feriegeben. Der Speicher muss mit "malloc" anegelegt worden sein.
 *
 * @param[in] adr Pointer auf den Beginn des Speicherbereichs
 * @param[in] size Größe des Speicherbereichs in Bytes
 */
CMemory::CMemory(void *adr, size_t size)
{
	ptr=adr;
	s=size;
}

/*!\brief Konstruktor mit Speicher-Allokierung
 *
 * \desc
 * Mit diesem Konstruktor wird eine Instanz der Klasse erzeugt, die einen Speicherblock mit
 * der Größe \p size allokiert und diesen selbst verwaltet.
 *
 * @param[in] size Gewünschte Größe des Speicherblocks in Bytes
 * \exception OutOfMemoryException Diese Exception wird geworfen, wenn der Speicher nicht allokiert werden konnte
 */
CMemory::CMemory(size_t size)
{
	s=0;
	ptr=::malloc(size);
	if (!ptr) throw OutOfMemoryException();
	s=size;
}

/*!\brief Speicherverwaltung übernehmen
 *
 * \desc
 * Mit dieser Funktion wird der Klasse die Verwaltung des Speicherbereich mit der Adresse \p adr und der
 * Größe \p size übergeben. Der Speicher muss zuvor mit "malloc" bzw. "calloc" allokiert worden sein
 * und darf von der Anwendung selbst nicht freigegeben werden.
 *
 * @param[in] adr Startadresse des Speicherbereichs
 * @param[in] size Größe des Speicherbereichs in Bytes
 *
 * \note
 * Falls die Klasse vor Aufruf bereits Speicher verwaltet hat, wird dieser zuerst freigegeben.
 */
void CMemory::set(void *adr, size_t size)
{
	::free(ptr);
	ptr=adr;
	s=size;
}

/*!\brief Speicherbereich von einer anderen Referenz CMemoryReference- oder CMemory-Instanz kopieren.
 *
 * \desc
 * Mit dieser Funktion wird der Speicher einer anderen CMemoryReference- oder CMemory-Instanz
 * kopiert. Sie ist identisch zu CMemory::copy.
 *
 * @param[in] other Referenz auf ein anderes CMemoryReference- oder CMemory-Objekt.
 *
 * \note
 * Falls die Klasse vor Aufruf bereits Speicher verwaltet hat, wird dieser zuerst freigegeben.
 */
void CMemory::set(const CMemoryReference &other)
{
	copy(other);
}

/*!\brief Speicherbereich von einem CBinary-Objekt kopieren.
 *
 * \desc
 * Mit dieser Funktion wird der Speicher eines CBinary-Objekts
 * kopiert. Sie ist identisch zu CMemory::copy.
 *
 * @param[in] bin Referenz auf ein CBinary-Objekt
 *
 * \note
 * Falls die Klasse vor Aufruf bereits Speicher verwaltet hat, wird dieser zuerst freigegeben.
 */
void CMemory::set(const CBinary &bin)
{
	copy(bin.GetPtr(),bin.GetSize());
}

/*!\brief Speicherbereich kopieren
 *
 * \desc
 * Mit dieser Funktion wird der durch \p adr und \p size angegebene Speicherbereich in
 * die CMemory-Klasse kopiert. Die Klasse allokiert dafür zunächst den erforderlichen
 * Speicher und übernimmt dessen Verwaltung.
 *
 * @param[in] adr
 * @param[in] size
 * \return Bei erfolgreichem Kopieren liefert die Funktion einen Pointer auf den
 * neu allokierten Speicherbereich zurück. Im Fehlerfall wird NULL zurückgegeben und
 * ein entsprechender Fehlercode gesetzt.
 *
 * \note
 * Falls die Klasse vor Aufruf bereits Speicher verwaltet hat, wird dieser zuerst freigegeben.
 */
void *CMemory::copy(void *adr, size_t size)
{
	::free(ptr);
	s=0;
	ptr=NULL;
	if (adr==NULL || size==0) {
		SetError(194);
		return NULL;
	}
	ptr=::malloc(size);
	if (!ptr) {
		SetError(2);
		return NULL;
	}
	if (memcpy(ptr,adr,size)!=ptr) {
		::free(ptr);
		ptr=NULL;
		s=0;
		SetError(357);
		return NULL;
	}
	s=size;
	return ptr;
}

/*!\brief Speicherbereich aus einem anderen CMemory-Objekt kopieren
 *
 * \desc
 * Mit dieser Funktion wird der durch \p other referenzierte Speicherbereich
 * in diese Instanz der CMemory-Klasse kopiert. Die Klasse allokiert dafür zunächst
 * den dafür erforderlichen Speicher und übernimmt dessen Verwaltung.
 *
 * @param[in] other Referenz auf das CMemory- oder CMemoryReference-Objekt, von dem der
 * Speicher kopiert werden soll.
 * \return Bei erfolgreichem Kopieren liefert die Funktion einen Pointer auf den
 * neu allokierten Speicherbereich zurück. Im Fehlerfall wird NULL zurückgegeben und
 * ein entsprechender Fehlercode gesetzt.
 *
 */
void *CMemory::copy(const CMemoryReference &other)
{
	return copy(other.ptr, other.s);
}

/*!\brief Speicherbereich von einem CBinary-Objekt kopieren.
 *
 * \desc
 * Mit dieser Funktion wird der Speicher eines CBinary-Objekts
 * kopiert. Sie ist identisch zu CMemory::copy.
 *
 * @param[in] bin Referenz auf ein CBinary-Objekt
 *
 * \note
 * Falls die Klasse vor Aufruf bereits Speicher verwaltet hat, wird dieser zuerst freigegeben.
 */
void *CMemory::copy(const CBinary &bin)
{
	return copy(bin.GetPtr(),bin.GetSize());
}

/*!\brief Speicherreferenz von anderem CMemory-Objekt kopieren
 *
 * \desc
 * Mit diesem Operator wird der Speicherbereich eines anderen CMemory- oder CMemoryReference-Objekts
 * kopiert. Er ist identisch zur Funktion CMemory::copy.
 *
 * @param[in] other Referenz auf ein anderes CMemory- oder CMemoryReference-Objekt.
 * @return Referenz auf das Objekt
 */
CMemory &CMemory::operator=(const CMemoryReference &other)
{
	copy(other.ptr, other.s);
	return *this;
}

/*!\brief Speicherbereich von einem CBinary-Objekt kopieren.
 *
 * \desc
 * Mit dieser Funktion wird der Speicher eines CBinary-Objekts
 * kopiert. Sie ist identisch zu CMemory::copy.
 *
 * @param[in] bin Referenz auf ein CBinary-Objekt
 *
 * \note
 * Falls die Klasse vor Aufruf bereits Speicher verwaltet hat, wird dieser zuerst freigegeben.
 */
CMemory &CMemory::operator=(const CBinary &bin)
{
	copy(bin.GetPtr(),bin.GetSize());
	return *this;
}

/*!\brief Adresse des Speicherblocks auslesen
 *
 * \desc
 * Mit diesem Operator wird die Adresse des Speicherblocks ausgelesen
 *
 * @return Adresse des Speicherblocks
 */
CMemory::operator void*() const
{
	return ptr;
}

/*!\brief Einzelnes Byte aus dem Speicherbereich kopieren
 *
 * \desc
 * Mit dem Operator [] kann ein bestimmtes Byte \p pos aus dem Speicherbereich
 * ausgelesen werden. Ist kein Speicher referenziert oder ist \p pos größer als
 * der Speicherblock, wird eine Exception ausgelöst.
 *
 * @param [in] pos Auszulesendes Byte, beginnend mit 0.
 * @return Wert der Speicherstelle
 * \exception OutOfBoundsEception Diese Exception wird geworfen, wenn die mit
 * \p pos angegebene Speicherstelle ausseralb des referenzierten Speichers liegt oder
 * kein Speicher referenziert ist.
 */
unsigned char CMemory::operator[](size_t pos) const
{
	if (ptr!=NULL && pos<s) return ((unsigned char*)ptr)[pos];
	throw OutOfBoundsEception();
}

/*!\brief Speicher allokieren
 *
 * \desc
 * Mit dieser Funktion wird ein neuer Speicherblock mit der Größe von \p size Bytes allokiert
 * und dessen Adresse als Ergebnis zurückgeliefert. Das CMemory-Objekt übernimmt die Verwaltung
 * des Speicherblocks.
 *
 * @param[in] size Gewünschte Größe des Speicherblocks.
 * \return
 * Pointer auf den Beginn des allokierten Speichers, oder NULL, wenn ein Fehler beim
 * Allokieren aufgetreten ist. Ein entsprechender Fehlercode wird gesetzt.
 */
void *CMemory::malloc(size_t size)
{
	::free(ptr);
	ptr=::malloc(size);
	if (ptr) {
		s=size;
	} else {
		SetError(2);
		s=0;
	}
	return ptr;
}

/*!\brief Speicher allokieren und mit 0 initialisieren
 *
 * \desc
 * Mit dieser Funktion wird ein neuer Speicherblock mit der Größe von \p size Bytes allokiert,
 * der Inhalt mit 0 initialisiert
 * und dessen Adresse als Ergebnis zurückgeliefert. Das CMemory-Objekt übernimmt die Verwaltung
 * des Speicherblocks.
 *
 * @param[in] size Gewünschte Größe des Speicherblocks.
 * \return
 * Pointer auf den Beginn des allokierten Speichers, oder NULL, wenn ein Fehler beim
 * Allokieren aufgetreten ist. Ein entsprechender Fehlercode wird gesetzt.
 */
void *CMemory::calloc(size_t size)
{
	::free(ptr);
	ptr=::calloc(size,1);
	if (ptr) {
		s=size;
	} else {
		SetError(2);
		s=0;
	}
	return ptr;
}

/*!\brief Speicher freigeben
 *
 * \desc
 * Wird der Speicher vom CMemory-Objekt verwaltet, wird dieser durch Aufruf dieser Funktion
 * wieder freigegeben. Bei nichtverwaltetem Speicher wird lediglich die interne Referenz auf
 * NULL gesetzt, aber der Speicher nicht freigegeben.
 */
void CMemory::free()
{
	::free(ptr);
	ptr=NULL;
	s=0;
}

void *CMemory::fromHex(const CString &hex)
{
	::free(ptr);
	ptr=NULL;
	s=0;
	if (hex.IsEmpty()) {
		SetError(562,"String ist leer");
		return NULL;
	}
	const unsigned char *p=(const unsigned char *)hex.GetPtr();
	unsigned char *t;
	size_t bytes=hex.Len();
	if ((bytes&1)==1) {
		SetError(562,"ungerade Anzahl Zeichen");
		return NULL;
	}
	ptr=::malloc(bytes>>1);
	if (!ptr) {
		SetError(2);
		return NULL;
	}
	t=(unsigned char*)ptr;
	s=bytes>>1;
	unsigned char value;
	for (size_t source=0, target=0;source<bytes;source+=2,target++) {
		unsigned char first=p[source];
		unsigned char second=p[source+1];
		if (first>='0' && first<='9') value=(first-'0');
		else if (first>='a' && first<='f') value=(first-'a'+10);
		else if (first>='A' && first<='F') value=(first-'A'+10);
		else {
			::free(ptr);
			ptr=NULL;s=0;
			SetError(562,"ungültiges Zeichen im String");
			return 0;
		}
		value=value<<4;
		if (second>='0' && second<='9') value|=(second-'0');
		else if (second>='a' && second<='f') value|=(second-'a'+10);
		else if (second>='A' && second<='F') value|=(second-'A'+10);
		else {
			::free(ptr);
			ptr=NULL;s=0;
			SetError(562,"ungültiges Zeichen im String");
			return 0;
		}
		t[target]=value;
	}
	return ptr;
}

}	// EOF namespace ppl6
