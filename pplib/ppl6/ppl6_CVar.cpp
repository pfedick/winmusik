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

#include "ppl6.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "ppl6.h"

namespace ppl6 {

/*!\class ppl6::CVar
 * \ingroup PPLGroupDataTypes
 * \brief Basisklasse für alle weiteren Datentypen
 *
 * \desc
 * Diese Klasse wird als Basisklasse für alle weiteren Datentypen verwendet. Als einzige
 * Information speichert sie den Typ der Variablen. Dieser wird von allen
 * abgeleiteten Klassen mit einem Wert belegt, der aussagt, um welchen Datentyp es sich handelt.
 * Dazu wurde die Klasse ebenfalls um die Enumeration CVar::PPLDataType erweitert.
 *
 * \see
 * - CInt
 * - CBool
 * - CString
 * - CWString
 * - CArray
 * - CAssocArray
 * - CBinary
 *
 * \example
 *
 * \code
void Machwas(const CVar &object)
{
	int t=object.DataType();
	if (t==CVar::CBINARY) {
			const CBinary &bin= static_cast<const CBinary&>(object);  // Objekt zu CBinary umwandeln
			printf ("Es ist ein CBinary mit %i Bytes an Daten\n",bin.Size());
			return;
	} else if (t==CVar::CSTRING) {
			const CString &str= static_cast<const CString&>(object);  // Objekt zu CString umwandeln
			printf ("Es ist ein CString mit folgendem Inhalt: %s\n",(char*)str);
			return;
	} else if (t==CVar::CWSTRING) {
			const CWString &wstr= static_cast<const CWString&>(object);  // Objekt zu CWString umwandeln
			printf ("Es ist ein CWString mit folgendem Inhalt: %s\n",(char*)wstr);
			return;
	}
	printf ("Datentyp wird nicht unterstützt\n");
}
\endcode
 *
 */

/*!\enum CVar::PPLDataType
 * \brief Enumeration der verschiedenen Datenobjekte, die in der Library verwendet werden
 *
 * Enumeration der verschiedenen Datenobjekte, die in der Library verwendet werden
 *
 * \since
 * Wurde mit Version 6.3.0 eingeführt.
 */

/*!\var CVar::PPLDataType CVar::UNKNOWN
 * \brief Unbekannter Datentyp
 */

/*!\var CVar::PPLDataType CVar::CINT
 * \brief Klasse vom Typ CInt
 */

/*!\var CVar::PPLDataType CVar::CSTRING
 * \brief Klasse vom Typ CString
 */

/*!\var CVar::PPLDataType CVar::CWSTRING
 * \brief Klasse vom Typ CWString
 */

/*!\var CVar::PPLDataType CVar::CBINARY
 * \brief Klasse vom Typ CBinary
 */

/*!\var CVar::PPLDataType CVar::CASSOCARRAY
 * \brief Klasse vom Typ CAssocArray
 */

/*!\var CVar::PPLDataType CVar::CARRAY
 * \brief Klasse vom Typ CArray
 */

/*!\var CVar::PPLDataType CVar::CBOOL
 * \brief Klasse vom Typ CBool
 */

/*!\var CVar::PPLDataType CVar::CDATETIME
 * \brief Klasse vom Typ CDateTime
 */


CVar::CVar()
/*!\brief Konstruktor der Klasse
 *
 * Der Konstruktor initialisiert den Typ der Klasse mit CVar::UNKNOWN
 */
{
	type=CVar::UNKNOWN;
}

CVar::CVar(const CVar &copy)
/*!\brief Copy-Konstruktor der Klasse
 *
 * Der Konstruktor kopiert den Datentyp der anderen Klasse.
 */
{
	type=copy.type;
}



CVar& CVar::operator=(const CVar& var)
{
	type=var.type;
	return *this;
}

int CVar::DataType() const
/*!\brief Liefert den Datentyp des Objekts zurück
 *
 * Diese Funktion liefert den Datentyp des Objekts zurück.
 * @return ID aus der Enumeration CVar::PPLDataType
 *
 * \since
 * Wurde mit Version 6.3.0 eingeführt.
 *
 */
{
	return (int)type;
}

int CVar::IsType(int type) const
{
	if(type==this->type) return 1;
	return 0;
}


} // EOF namespace ppl6

