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

#define tr(string) translate("libppl6",__FILE__,string)
#define tr_str_array

#include "prolog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#ifdef HAVE_ERRNO_H
	#include <errno.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <winerror.h>
#endif


#include "ppl6.h"

const char *translate(const char *module, const char *file, const char *string)
{
	return string;
}

static tr_str_array const char * errors[] = {
        "Kein Fehler",										// 0
        "MEMMAN konnte nicht initialisiert werden",			// 1
        "Nicht genug Speicher (ENOMEM)",					// 2
        "Unbekannter Fehlercode",							// 3
        "Unbekannte Funktion",								// 4
        "Funktion noch nicht verfuegbar",					// 5
        "Parameter unvollstaendig",							// 6
        "Falscher DMA-Kanal",								// 7
        "Zieladresse ist identisch mit Quelle",				// 8
        "Datei nicht gefunden",								// 9
        "Datei kann nicht geschlossen werden",				// 10
        "Dateiposition nicht moeglich",						// 11
        "Unbekanntes Dateihandle",							// 12
        "Unbekannter Fehler bei Dateizugriff",				// 13
        "Ungueltiges Handle",								// 14
        "Lineare Adresse zu gross zum Umrechnen",			// 15
        "Kein PFS/PFP-File",								// 16
        "Dateiversion wird nicht unterstuetzt",				// 17
        "Dateiname bereits vorhanden (EEXIST)",				// 18
        "Zugriff verweigert",								// 19
        "Unbekanntes Format",								// 20
        "Zielpuffer zu klein",								// 21
        "Notify-Handle zu gross",							// 22
        "Notify-Handle nicht vorhanden",					// 23
        "Keine Notify-Handles mehr frei",					// 24
        "Kein gueltiges PAR-Archiv",						// 25
        "Font nicht geladen",								// 26
        "String ist leer",									// 27
        "Zu wenig String-Speicher",							// 28
        "Kein VESA-Treiber installiert",					// 29
        "VESA-Treiber nicht initialisiert",					// 30
        "VESA-Modus nicht gefunden",						// 31
        "VESA-Modus konnte nicht aktiviert werden",			// 32
        "Farbtiefe wird nicht unterstuetzt",				// 33
        "Nicht genug Konventioneller Speicher",				// 34
        "Kein Timer mehr frei",								// 35
        "Ungueltiger Timer",								// 36
        "Adresse darf nicht Null sein",						// 37
        "Object nicht vorhanden",							// 38
        "Verzeichnis konnte nicht angelegt werden",			// 39
        "Verzeichnis konnte nicht geloescht werden",		// 40
        "Datei konnte nicht geloescht werden",				// 41
        "Datei konnte nicht erstellt werden",				// 42
        "Keine weiteren Dateien",							// 43
        "MSCDEX nicht installiert oder keine CD-Laufwerke vorhanden",				// 44
        "DPMI: Konventionaler Speicher konnte nicht reserviert werden",				// 45
        "DPMI: Real Mode Speicher konnte nicht als linear markiert werden",			// 46
        "MSCDEX: Fehler bei Zugriff auf CDROM",										// 47
        "DPMI: Selector konnte nicht angefordert werden",							// 48
        "DPMI: Physik. Adresse konnte nicht auf lineare Adresse gemapped werden",	// 49
        "DPMI: Basisadresse konnte nicht gesetzt werden",							// 50
        "DPMI: Segment Limit konnte nicht gesetzt werden",							// 51
        "DPMI: Segment Zugriffsrechte konnten nicht gesetzt werden",				// 52
        "DPMI: Selector konnte nicht freigegeben werden",							// 53
        "DPMI: Real Mode Callback Adresse konnte nicht reserviert werden",			// 54
        "Maximum ueberschritten",													// 55
        "Minimum unterschritten",										// 56
        "MEMMAN ist bereits initialisiert",								// 57
        "MEMMAN ist nicht initialisiert",								// 58
        "Handle zeigt auf NULL",										// 59
        "Freigabe wurde verweigert",									// 60
        "Ungueltiger Dateiname",										// 61
        "Interrupt konnte nicht gesetzt werden",						// 62
        "Konventioneller Speicher konnte nicht reserviert werden",		// 63
        "Nicht erlaubter Zugriffsmodus",								// 64
        "Kein freies Handle mehr",										// 65
        "Pfad nicht gefunden",											// 66
        "Unbekannte Kompressionsmethode",								// 67
        "Resource nicht verfuegbar",									// 68
        "Clickhandle ausserhalb des definierten Bereichs",				// 69
        "Kein Maustreiber installiert",									// 70
        "MausForm nicht vorhanden",										// 71
        "Unerlaubtes Handle oder Datei nicht geoeffnet. (EBADF)",		// 72
        "Unbekanntes Geraet",											// 73
        "Topic nicht gefunden",											// 74
        "Wert nicht gefunden",											// 75
        "Keine Config-Datei geladen",									// 76
        "Connect zur Datenbank fehlgeschlagen",							// 77
        "Tabelle nicht vorhanden oder Zugriff verweigert",				// 78
        "Soundkarte konnte nicht initialisiert werden",					// 79
        "Soundkarte ist nicht initialisiert",							// 80
        "Zu viele Sound-Kanaele",										// 81
        "Soundkarte wird nicht unterstuetzt",							// 82
        "Soundsystem ist nicht aktiviert",								// 83
        "Kein Kanal mehr frei",											// 84
        "Ungueltiger Kanal",											// 85
        "Kanal nicht benutzt",											// 86
        "Unbekannter Frametyp",											// 87
        "Format wird nicht unterstuetzt",								// 88
        "Zugriffsmodus nicht gesetzt",									// 89
        "Grafisches Objekt nicht vorhanden",							// 90
		"DirectSound konnte nicht initialisiert werden",				// 91
		"DirectSound CooperativLevel konnte nicht gesetzt werden",		// 92
		"DirectSound Buffer konnte nicht reserviert werden",			// 93
		"DirectSound Kein Buffer reserviert",							// 94
		"Datei nicht angegeben",										// 95
		"Fenster konnte nicht erstellt werden",							// 96
		"DirectDraw konnte nicht initialisiert werden",					// 97
		"DirectDraw Exclusive mode konnte nicht aktiviert werden",		// 98
		"DirectDraw Aufloesung konnte nicht gesetzt werden",			// 99
		"DirectDraw Primary Screen konnte nicht erstellt werden",		// 100
		"DirectDraw Secondary Screen konnte nicht erstellt werden",		// 101
		"DirectDraw Backbuffer konnte nicht gesperrt werden",			// 102
		"DirectDraw Backbuffer konnte nicht entsperrt werden",			// 103
		"DirectDraw Surface lost",										// 104
		"DirectDraw konnte Buffer nicht tauschen (flip)",				// 105
		"Surface konnte nicht erstellt werden",							// 106
		"Koordinaten ausserhalb der Oberflaeche",						// 107
		"Unbekanntes PFP-Format",										// 108
		"Frame-Pointer ist NULL",										// 109
		"DirectDraw2 Primary Screen konnte nicht erstellt werden",		// 110
		"Kein Object geladen",											// 111
		"DirectDraw Surface nicht geladen",								// 112
		"DirectDraw SurfaceDescription konnte nicht geholt werden",		// 113
		"DirectDraw gewuenschte Frequenz wird nicht unterstuetzt, benutze Default",	// 114
		"DirectDraw ist nicht initialisiert",							// 115
		"DirectDraw Display Modes konnten nicht ermittelt werden",		// 116
		"DirectDraw IDirectDrawSurface3 konnte nicht erstellt werden",	// 117
		"DirectDrawClipper konnte nicht erstellt werden",				// 118
		"DirectInput: Initialisierung fehlgeschlagen",					// 119
		"DirectInput: Mouse konnte nicht initialisiert werden",			// 120
		"DirectInput: nicht initialisiert",								// 121
		"DirectInput: CooperativeLevel konnte nicht gesetzt werden",	// 122
		"DirectInput: Aufruf fehlgeschlagen",							// 123
		"DirectDraw: Aufruf fehlgeschlagen (siehe suberr)",				// 124
		"Class Cursor: Cursordatei enthaelt keine Cursor",				// 125
		"Class Cursor: Cursor nicht vorhanden",							// 126
		"Class Cursor: Cursor ist nicht initialisiert",					// 127
		"Class Cursor: CursorBitmap nicht geladen",						// 128
		"Class Click: Nicht moeglich, ist bereits initialisiert",		// 129
		"Class Click: Parameter ausserhalb des gueltigen Bereichs",		// 130
		"Class Click: Nicht initialisiert",								// 131
		"Class Click: ungueltiges Handle",								// 132
		"Sprachdatei konnte nicht geladen werden",						// 133
		"Sprachdatei nicht geladen",									// 134
		"Element nicht in Sprachdatei",									// 135
		"Ungueltige Parameter",											// 136
		"Datenbank gesperrt, Zugriff nicht moeglich",					// 137
		"Datenbank-Query fehlgeschlagen",								// 138
		"Kein DirectX installiert",										// 139
		"Buffer ist nicht gross genug",									// 140
		"DirectSound Primary Buffer konnte nicht eingerichtet werden",	// 141
		"DirectSound Format des Primary Buffer konnte nicht gesetzt werden",	// 142
		"Komprimierung fehlgeschlagen",									// 143
		"Dekomprimierung fehlgeschlagen",								// 144
		"Surface konnte nicht gelocked werden",							// 145
		"DirectDraw Blt fehlgeschlagen",								// 146
		"Surface darf nicht NULL sein",									// 147
		"Format der Grafik stimmt nicht mit dem angegebenen Surface ueberein",	// 148
		"Ungueltige IP-Adresse",										// 149
		"Netzmaske passt nicht zur IP-Nummer, falsche Boundary",		// 150
		"DirectDraw CooperativLevel nicht gesetzt",						// 151
		"Ungueltiger Index",											// 152
		"Kein Archiv geoeffnet",										// 153
		"Dateiende erreicht",											// 154
		"Kein SoundBuffer mehr frei",									// 155
		"Fehler in WAVE-Datei, kein data-chunk",						// 156
		"Stream ist gelocked",											// 157
		"Ende des Streams erreicht",									// 158
		"Datei konnte nicht geoeffnet werden",							// 159
		"DirectSound notification-Event konnte nicht gesetzt werden",	// 160
		"DirectSound Primaerer Buffer konnte nicht gestartet werden",	// 161
		"Parameter werden von dieser Klasse nicht unterstuetzt",		// 162
		"MemFile: Anzahl Bytes ueberschreiten Speichergroesse, schreiben unmoeglich",	// 163
		"Quellfile nicht angegeben",									// 164
		"DirectDraw4 Objekt konnte nicht erstellt werden",				// 165
		"DirectDraw Objekt ungueltig",									// 166
		"DirectDraw Bildschirmaufloesung konnte nicht wiederhergestellt werden",	// 167
		"Falsche Parameter",											// 168
		"DirectDraw Surface konnte nicht geloescht werden",				// 169
		"Exception!",													// 170
		"DirectDraw Colorkey konnte nicht gesetzt werden",				// 171
		"DirectDraw keine Surfaces vorhanden",							// 172
		"Passende Winsock-Version nicht vorhanden",						// 173
		"Timeout aufgetreten (ETIMEDOUT)",								// 174
		"DNS-Abfrage gescheitert",										// 175
		"Font konnte nicht erstellt werden",							// 176
		"Funktion wird nicht mehr unterstuetzt!",						// 177
		"ungueltiges Image!",											// 178
		"ungueltiger Funktionsaufruf",									// 179
		"Funktion der Basisklasse kann nicht direkt aufgerufen werden",	// 180
		"kein Connect zur Datenbank vorhanden",							// 181
		"Kein Query-Result vorhanden",									// 182
		"Feld in Query-Result nicht gefunden",							// 183
		"Grafik konnte nicht gespeichert werden",						// 184
		"PNG-Write fehlgeschlagen",										// 185
		"3D-Interface konnte nicht erstellt werden",					// 186
		"3D-Device konnte nicht erstellt werden",						// 187
		"3D-Viewport konnte nicht gesetzt werden",						// 188
		"Es ist kein Primary Surface vorhanden",						// 189
		"FreeType Library konnte nicht initialisiert werden",			// 190
		"Font konnte nicht geladen werden",								// 191
		"Klasse ist bereits initialisiert",								// 192
		"Klasse ist nicht initialisiert",								// 193
		"Parameter darf nicht NULL sein",								// 194
		"Grafikengine nicht initialisiert",								// 195
		"Grafikengine ist bereits initialisiert",						// 196
		"Grafikengine wird nicht unterstuetzt",							// 197
		"Unbekannte Grafikengine",										// 198
		"Unbekanntes Farbformat",										// 199
		"Initialisierung fehlgeschlagen",								// 200
		"Primary Surface ist bereits vorhanden",						// 201
		"Backbuffer des Screens konnte nicht geholt werden",			// 202
		"Backbuffer konnte nicht gelocked werden",						// 203
		"Backbuffer war nicht gelocked",								// 204
		"Buffer konnten nicht rotiert werden (Flip)",					// 205
		"Unbekannte Komprimierung",										// 206
		"Sound-Engine nicht verfuegbar",								// 207
		"Sound-Format wird nicht unterstuetzt",							// 208
		"Daten konnten nicht komplett geladen werden",					// 209
		"Keine Sounddatei geladen",										// 210
		"unbekannte Surfaceart",										// 211
		"Surface konnte nicht entsperrt werden",						// 212
		"Textur zu gross, wird von Hardware nicht unterstuetzt",		// 213
		"Datei kann nicht mit dem angegebenen Filter geladen werden",	// 214
		"Kein Filter fuer die Datei gefunden",							// 215
		"Dekomprimierung fehlgeschlagen",								// 216
		"Encoder-Fehler",												// 217
		"Encoder noch nicht gestartet",									// 218
		"Funktion wird nicht unterstuetzt",								// 219
		"Farbreduzierung auf gewuenschte Anzahl nicht moeglich",		// 220
		"Section nicht vorhanden",										// 221
		"Zeile (Row) in Query Result nicht vorhanden",					// 222
		"Dekomprimierung fehlgeschlagen",								// 223
		"Dekomprimierte Daten kleiner als erwartet",					// 224
		"Komprimierung fehlgeschlagen",									// 225
		"Keine Reset-Funktion fuer diese GrafikEngine definiert",		// 226
		"Device kann zur Zeit nicht Resettet werden",					// 227
		"Interner Fehler bei Reset aufgetreten (DirectX)",				// 228
		"Nicht genug Videospeicher (DirectX)",							// 229
		"Nicht genug Hauptspeicher (DirectX)",							// 230
		"Device Lost (DirectX)",										// 231
		"Nicht alle Surfaces konnten wiederhergestellt werden",			// 232
		"Resize fehlgeschlagen",										// 233
		"Es ist bereits eine Instanz der SoundEngine geladen",			// 234
		"Buffer fuer Komprimierung zu klein",							// 235
		"Buffer fuer Dekomprimierung zu klein",							// 236
		"Komprimiermethode nicht verfuegbar/einkompiliert",				// 237
		"Datenbank-Fehler",												// 238
		"Datei nicht geoeffnet",										// 239
		"Kein gueltiges PARC-Archiv",									// 240
		"Unbekannte Version",											// 241
		"Unbekannter CHUNK",											// 242
		"Dateiname nicht angegeben",									// 243
		"Datei bereits im Archiv",										// 244
		"Die Datei ist zu gross (EFBIG)",								// 245
		"Funktion auf dieser Plattform nicht verfeugbar",				// 246
		"Es darf nur eine Instanz von CpplGrafix geben",				// 247
		"Surface hat kein Backup",										// 248
		"Funktion wird von dieser Grafik-Engine nicht unterstuetzt",	// 249
		"Surface ist gelocked",											// 250
		"CApplication-Klasse wurde nicht definiert",					// 251
		"Zu viele Parameter",											// 252
		"Dekodierung laeuft bereits",									// 253
		"Heap ist bereits initialisiert",								// 254
		"Heap ist nicht initialisiert",									// 255
		"Ungueltiger Heap-Pointer",										// 256
		"Dekoder ist noch nicht gestartet",								// 257
		"Nichts zu Dekoden",											// 258
		"Fehler im Format",												// 259
		"Ungueltige Resource",											// 260
		"Keine Resource geladen",										// 261
		"Resource nicht gefunden",										// 262
		"Resource kann nicht dekomprimiert werden",						// 263
		"Fontname ist nicht angegeben",									// 264
		"Socket konnte nicht geoeffnet werden",							// 265  =>247
		"Socket konnte nicht an gewuenschten Port gebunden werden",		// 266  =>248
		"Socket nicht verbunden",										// 267  =>249
		"Socket listen fehlgeschlagen",									// 268  =>250
		"Thread konnte nicht gestartet werden",							// 269  =>251
		"Hostname ist NULL",											// 270  =>252
		"Port darf nicht 0 sein",										// 271  =>253
		"Socket konnte nicht erstellt werden",							// 272  =>254
		"Hostname kann nicht aufgeloest werden",						// 273  =>255
		"Connect fehlgeschlagen",										// 274  =>256
		"Nicht connected",												// 275  =>257
		"PCRE-Unterstuetzung nicht einkompiliert",						// 276
		"File ist gelocked oder Die Resource ist zurzeit nicht verfügbar (EAGAIN/EWOULDBLOCK)",	// 277
		"Operation (z.B. File-Locking) wird fuer diese Datei nicht unterstuetzt (ENOTSUP)",		// 278
		"Datei oder Argument ist ungültig (EINVAL)",					// 279
		"File-Locking fehlgeschlagen",									// 280
		"File konnte nicht entsperrt werden",							// 281
		"Truncate fehlgeschlagen",										// 282
		"Unbekannter Font",												// 283
		"Font-Filter hat keine Render-Routinen!",						// 284
		"Font-Renderer konnte nicht hinzugefuegt werden",				// 285
		"LibIconv ist nicht einkompiliert",								// 286
		"Charset wird nicht unterstuetzt",								// 287
		"Charset ist nicht initialisiert",								// 288
		"Konvertierung fehlgeschlagen",									// 289
		"Widget bereits Attached",										// 290
		"Host-Angabe nicht im Format Host:Port",						// 291
		"OpenSSL-Unterstuetzung nicht einkompiliert",					// 292
		"Keine Section ausgewaehlt",									// 293
		"Key nicht vorhanden",											// 294
		"Datenbank nicht vorhanden oder keine Zugriffsrechte",			// 295
		"Datenbank-Library konnte nicht initialisiert werden",			// 296
		"Feld nicht vorhanden",											// 297
		"NewRow nicht aufgerufen oder fehlgeschlagen",					// 298
		"Verbindung wurde getrennt",									// 299
		"Keine weiteren Rows im Result",								// 300
		"Key konnte nicht erstellt werden",								// 301
		"Key ist ungueltig",											// 302
		"Key ist kein String",											// 303
		"Key ist kein Array",											// 304
		"Das numerische Ergebnis ist außerhalb des gültigen Bereiches (ERANGE)",							// 305
		"OBSOLETE: Das Argument ist ungültig (EINVAL) => Siehe Fehler 279",											// 306
		"Pfad zeigt auf einen Speicherbereich außerhalb des erlaubten Adreßraums des Prozesses. (EFAULT)",	// 307
		"Datenübergabe unterbrochen (broken pipe) (EPIPE)",													// 308
		"OBSOLETE: File ist gelocked oder Die Resource ist zurzeit nicht verfügbar (EAGAIN) (siehe Fehler 277)",							// 309
		"Unterbrechung während des Betriebssystemaufrufs (EINTR)",											// 310
		"Auf dem Gerät ist kein Speicherplatz mehr verfügbar (ENOSPC)",										// 311
		"Unbekannter Socket Fehler",									// 312
		"Ist ein Verzeichnis (EISDIR)",									// 313
		"Ungueltige Stacksize",											// 314
		"Stacksize wird nicht unterstuetzt",							// 315
		"SSL-Library konnte nicht initialisiert werden",				// 316
		"SSL-Library ist nicht initialisiert",							// 317
		"SSL Trusted CA-Zertifikat konnte nicht geladen werden",		// 318
		"SSL Instanz konnte nicht initialisiert werden",				// 319
		"Ungueltige SSL_METHOD",										// 320
		"SSL Instanz ist nicht initialisiert",							// 321
		"SSL konnte nicht gestartet werden",							// 322
		"Ungültiges Zertifikat",										// 323
		"The certificate's owner does not match hostname",				// 324
		"Datei ist zu gross",											// 325
		"Datum ist nicht im ISO 8601-Format",							// 326
		"SSL-Zertifikat konnte nicht geladen werden",					// 327
		"SSL-Zertifikat: Kein Zugriff auf Private Key",					// 328
		"SSL-Klasse ist noch referenziert",								// 329
		"Interner SSL-Fehler: Inkonsistenz in Referenz-Kette",			// 330
		"SSL ist nicht gestartet",										// 331
		"SSL_Accept fehlgeschlagen",									// 332
		"Ungueltige SSL-Cipher-List",									// 333
		"Socket konnte nicht mit SO_REUSEADDR konfiguriert werden",		// 334
		"PPL6Core konnte nicht initialisiert werden",					// 335
		"Thread Stop-Flag gesetzt",										// 336
		"Ungueltiger Datentyp",											// 337
		"Keine Daten vorhanden",										// 338
		"Ungueltige Daten",												// 339
		"Key ist kein Pointer",											// 340
		"Unvollstaendige Datenbank-Parameter",							// 341
		"Unbekannter Datenbank-Typ",									// 342
		"Array ist leer",												// 343
		"Tabelle ist nicht angegeben",									// 344
		"Unbekannte Methode, muss insert, replace oder update sein",	// 345
		"Protokoll-Fehler",												// 346
		"Tree ist leer",												// 347
		"MkDate fehlgeschlagen, vielleicht Format-Fehler?",				// 348
		"Syntax Error",													// 349
		"Timeout konnte nicht gesetzt werden",							// 350
		"Kein unterstuetzter Window-Manager gefunden",					// 351
		"Falsche PPL-Version",											// 352
		"Initialisierungsfehler in LibCURL",							// 353
		"LibCURL Error",												// 354
		"String konnte nicht Escaped werden",							// 355
		"CURL ist nicht eingebunden",									// 356
		"Kopieren der Daten fehlgeschlagen (memcpy)",					// 357
		"Key ist kein Binary",											// 358
		"MD5-Summe stimmt nicht überein",								// 359
		"Die Operation ist nicht erlaubt (EPERM)",						// 360
		"Die angegebene Datei befindet sich auf einem Dateisystem, das nur lesbar ist. (EROFS)",					// 361
		"Nicht erlaubter Seek (ESPIPE)",										// 362
		"Pfad ist zu lang. (ENAMETOOLONG)",										// 363
		"Die angegebene Datei oder der Pfadname existiert nicht. (ENOENT)",		// 364
		"Kein passender Prozeß gefunden (ESRCH)",								// 365
		"Eine Komponente von Pfad ist kein Verzeichnis. (ENOTDIR)",				// 366
		"Kein Zugriff auf Datei oder Pfad (EACCES)",							// 367
		"Pfad enthält zu viele symbolische Verweise. (ELOOP)",					// 368
		"Ein Ein-/Ausgabefehler ist aufgetreten. (EIO)",						// 369
		"Die Argumentliste ist zu lang (E2BIG)",								// 370
		"Ungültige Nachricht (EBADMSG)",										// 371
		"Das Gerät oder die Resource ist belegt (EBUSY)",						// 372
		"Operation abgebrochen (ECANCELED)",									// 373
		"Keine Kind-Prozesse (ECHILD)",											// 374
		"Verklemmung beim Zugriff auf eine Resource vermieden (EDEADLK)",		// 375
		"Das numerische Argument ist außerhalb des Definitionsbereiches (EDOM)",// 376
		"Die Operation ist jetzt in Bearbeitung (EINPROGRESS)",					// 377
		"Die Operation wird von diesem Gerät nicht unterstützt (ENODEV)",		// 378
		"Zu viele offene Dateien (EMFILE)",										// 379
		"Zu viele Links (EMLINK)",												// 380
		"Die Nachricht ist zu lang (EMSGSIZE)",									// 381
		"Zu viele offene Dateien im System (ENFILE)",							// 382
		"Fehler im Format der Programmdatei (ENOEXEC)",							// 383
		"Keine Sperren verfügbar (ENOLCK)",										// 384
		"Die angeforderte Funktion ist nicht implementiert (ENOSYS)",			// 385
		"Das Verzeichnis ist nicht leer (ENOTEMPTY)",							// 386
		"Das Gerät ist nicht konfiguriert (ENXIO)",								// 387
		"Unpassender IOCTL (I/O-Control) für das Gerät (ENOTTY)",				// 388
		"Ungültiger Link über Gerätegrenzen hinweg (EXDEV)",					// 389
		"Kommunikation mit Mailserver fehlgeschlagen",					// 390
		"Konfigurationsdatei nicht gefunden",							// 391
		"Konfigurationsdatei unvollständig",							// 392
		"Socket für Sourceport konnte nicht geöffnet werden",			// 393
		"Socket konnte nicht an Sourceport gebunden werden",			// 394
		"Unbekanntes Internet-Protokoll (getprotobyname)",				// 395
		"Application scheint nicht unter Windows zu laufen",			// 396
		"Fenster wird bereits angezeigt",								// 397
		"xxx",															// 398
		"Primitive wird von Farbformat nicht unterstützt",				// 399
		"Array unterstuetzt keine Sortierung",							// 400
		"MP3-File konnte nicht eingelesen werden",						// 401
		"File hat kein ID3v2-Tag",										// 402
		"Version des ID3v2-Tags wird nicht unterstützt",				// 403
		"Gesetzten Unsynchronisation-Flag im ID3v2-Tag wird nicht unterstützt",	// 404
		"Extended Header im ID3v2-Tag wird nicht unterstützt",			// 405
		"Cannot assign requested address (EADDRNOTAVAIL)",				// 406
		"Connection refused. Service is probably not running (ECONNREFUSED)",		// 407
		"Socket is not connected (ENOTCONN)",										// 408
		"Connection reset by peer (ECONNRESET)",									// 409
		"Zugriff von dieser IP-Adresse nicht erlaubt",								// 410
		"CRC-Fehler im Datenpaket",													// 411
		"Ungültige Protokollversion",												// 412
		"Element ist bereits Teil einer Liste",										// 413
		"Element ist nicht Teil dieser Liste",										// 414
		"Keine weiteren Elemente in dieser Liste",									// 415
		"Liste ist leer",															// 416
		"Liste ist inkonsistent",													// 417
		"Element ist bereits vorhanden im Baum",									// 418
		"Element ist nicht Teil dieses Baums",										// 419
		"Element ist bereits Teil eines Baums",										// 420
		"Element nicht gefunden",													// 421
		"Keine weiteren Elemente im Baum",											// 422
		"Chunkname für PFP-File muss exakt 4 Byte lang sein",						// 423
		"Chunk enthält keine Daten",												// 424
		"PFP-File ist leer",														// 425
		"Chunk hat keinen Namen",													// 426
		"Chunk konnte nicht hinzugefügt werden",									// 427
		"Chunk ist nicht in der Datei enthalten",									// 428
		"ID des PFP-Files muss exakt 4 Byte lang sein",								// 429
		"Kein Suchstring angegeben",												// 430
		"Ungültige Version für PFP-File. Version muss zwischen 0 und 255 liegen",	// 431
		"Kein gültiges PFP-File Version 3",											// 432
		"PFP-File ist korrupt",														// 433
		"PFP-File enthält ungültige Chunks",										// 434
		"PFP-File hat unerwartete ID oder Version",									// 435
		"String ist kürzer als die gewünschte Position",							// 436
		"Unicode-Wert kann nicht in den String eingefügt werden",					// 437
		"Ungültige String-Position",												// 438
		"Datenbank-Verbindung ist teil eines anderen Pools",						// 439
		"ID oder Name ist nicht im Datenbank-Pool definiert",						// 440
		"Es ist bereits eine Datenbank mit gleichem Namen aber anderer Id definiert",	// 441
		"Anzahl Maximaler Datenbank-Verbindungen ist erreicht",						// 442
		"Datenbank-Pool wurde noch nicht initialisiert",							// 443
		"Keine Email geladen",														// 444
		"Mail enthält keinen Body",													// 445
		"Mailheader ist leer",														// 446
		"Unbekannte Mailkodierung",													// 447
		"Dekodierung der Mail fehlgeschlagen",										// 448
		"Element entspricht nicht dem gewünschten Datentyp",						// 449
		"Element enthält keine Daten",												// 450
		"Socket ist nicht zum Schreiben bereit",									// 451
		"Socket zeigt Fehler an",													// 452
		"Schreib- oder Lesevorgang mit 0 Bytes",									// 453
		"Vorgang ist bereits im gange (EALREADY)",									// 454
		"Socket operation on non-socket (ENOTSOCK)",								// 455
		"Socket: Destination address required (EDESTADDRREQ)",						// 456
		"Socket: Protocol wrong type for socket (EPROTOTYPE)",						// 457
		"Socket: Bad protocol option (ENOPROTOOPT)",								// 458
		"Socket: Protocol not supported (EPROTONOSUPPORT)",							// 459
		"Socket: Socket type not supported (ESOCKTNOSUPPORT)",						// 460
		"Socket: Protocol family not supported (EPFNOSUPPORT)",						// 461
		"Socket: Address family not supported by protocol family (EAFNOSUPPORT)",	// 462
		"Socket: Address already in use (EADDRINUSE)",								// 463
		"Socket: Network is down (ENETDOWN)",										// 464
		"Socket: Network is unreachable (ENETUNREACH)",								// 465
		"Socket: Network dropped connection on reset (ENETRESET)",					// 466
		"Socket: Software caused connection abort (ECONNABORTED)",					// 467
		"Socket: No buffer space available (ENOBUFS)",								// 468
		"Socket: Socket is already connected (EISCONN)",							// 469
		"Socket: Cannot send after socket shutdown (ESHUTDOWN)",					// 470
		"Socket: Too many references (ETOOMANYREFS)",								// 471
		"Socket: Host is down (EHOSTDOWN)",											// 472
		"Socket: No route to host (EHOSTUNREACH)",									// 473
		"Socket: Too many processes (EPROCLIM)",									// 474
		"Socket: User quota exceeded (EUSERS)",										// 475
		"Socket: Disk quota exceeded (EDQUOT)",										// 476
		"Socket: The file handle reference is no longer available (ESTALE)",		// 477
		"Socket: Item is remote (EREMOTE)",											// 478
		"Socket: Resource temporarily unavailable (EAI_AGAIN)",						// 479
		"Socket: Invalid value for ai_flags (EAI_BADFLAGS)",						// 480
		"Socket: Invalid value for hints (EAI_BADHINTS)",							// 481
		"Socket: Non-recoverable failure in name resolution (EAI_FAIL)",			// 482
		"Socket: ai_family not supported (EAI_FAMILY)",								// 483
		"Socket: memory allocation failure (EAI_MEMORY)",							// 484
		"Socket: hostname not provided or not known (EAI_NONAME)",					// 485
		"Socket: argument buffer overflow (EAI_OVERFLOW)",							// 486
		"Socket: resolved protocol is unknown (EAI_PROTOCOL)",						// 487
		"Socket: servname not supported for ai_socktype (EAI_SERVICE)",				// 488
		"Socket: ai_socktype not supported (EAI_SOCKTYPE)",							// 489
		"Socket: Network subsystem is unavailable (WSASYSNOTREADY)",				// 490
		"Socket: The Windows Sockets version requested is not supported. (WSAVERNOTSUPPORTED)",	// 491
		"Socket: Successful WSAStartup not yet performed (WSANOTINITIALISED)",		// 492
		"Socket: Graceful shutdown in progress (WSAEDISCON)",						// 493
		"Socket: No more results (WSAENOMORE)",										// 494
		"Socket: Call has been canceled (WSAECANCELLED)",							// 495
		"Socket: Procedure call table is invalid (WSAEINVALIDPROCTABLE)",			// 496
		"Socket: Service provider is invalid (WSAEINVALIDPROVIDER)",				// 497
		"Socket: Service provider failed to initialize (WSAEPROVIDERFAILEDINIT)",	// 498
		"Socket: System call failure (WSASYSCALLFAILURE)",							// 499
		"Socket: Service not found (WSASERVICE_NOT_FOUND)",							// 500
		"Socket: Class type not found (WSATYPE_NOT_FOUND)",							// 501
		"Socket: No more results (WSA_E_NO_MORE)",									// 502
		"Socket: Call was canceled (WSA_E_CANCELLED)",								// 503
		"Socket: Database query was refused (WSAEREFUSED)",							// 504
		"Socket: Host not found (WSAHOST_NOT_FOUND)",								// 505
		"Socket: Nonauthoritative host not found (WSATRY_AGAIN)",					// 506
		"Socket: This is a nonrecoverable error (WSANO_RECOVERY)",					// 507
		"Socket: Invalid name, no data record of requested type (WSANO_DATA)",		// 508
		"Socket: Host exists, but does not have any network addresses defined (EAI_NODATA)",	// 509
		"Database: Version wird nicht unterstützt",									// 510
		"Database: Datenbank wird nicht unterstützt",								// 511
		"Lokaler Hostname ist nicht bekannt, Aktion kann nicht durchgeführt werden",	// 512
		"Sync wird nicht unterstützt",												// 513
		"Queue ist voll",															// 514
		"Item ist bereits in einer Queue enthalten",								// 515
		"Queue ist leer",															// 516
		"Gewünschte mapping-Größe überschreitet Dateigröße",						// 517
		"MCrypt ist nicht einkompiliert",											// 518
		"Kombination von Verschlüsselungs-Algorithmus und Modus wird nicht unterstützt",	// 519
		"MCrypt ist nicht initialisiert",											// 520
		"MCrypt: IV wird benötigt, wurde aber nicht angegeben",						// 521
		"MCrypt: Es wurde kein Schlüssel angegeben",								// 522
		"MCrypt: Initialisierung der Verschlüsselung fehlgeschlagen",				// 523
		"MCrypt: Verschlüsselung fehlgeschlagen",									// 524
		"Unterstützung für Wide-Character (Unicode) fehlt",							// 525
		"CMHash: Kombination von Algorithmus und Bittiefe wird nicht unterstützt",	// 526
		"MHash ist nicht einkompiliert",											// 527
		"MHash: Algorithmus wird nicht unterstützt",								// 528
		"MHash ist nicht initialisiert",											// 529
		"MHash: Hashing der Daten fehlgeschlagen",									// 530
		"MHash: Es wurden keine Daten gehashed",									// 531
		"MHash: CInt ist nur bei Hashes mit 32-Bit-Ergebnis möglich",				// 532
		"CCompression: Ungültiger Kompressions-Prefix",								// 533
		"CCompression: Unvollständiger oder defekter Datenstrom",					// 534
		"CCompression: Keine V2 Kompressionsprefix",								// 535
		"CTreeController ist nicht implementiert",									// 536
		"Keine URL angegeben",														// 537
		"Zugriff auf Datenbank-Server verweigert",									// 538
		"Zugriff auf Datenbank verweigert",											// 539
		"Datenbank konnte nicht angelegt werden",									// 540
		"Unbekannter Parameter",													// 541
		"Login fehlgeschlagen",														// 542
		"Keine Datei geladen",														// 543
		"Speicherbereich ist leer",													// 544
		"Operation fehlgeschlagen",													// 545
		"Resolver: Query fehlgeschlagen",											// 546
		"Resolver: Antwort konnte nicht geparsed werden",							// 547
		"Resolver: Initialisierung fehlgeschlagen",									// 548
		"Resolver: Ungültiges Label",												// 549
		"Resolver: ldns-Library ist nicht einkompiliert",							// 550
		"Resolver: DNSSEC-Schlüssel konnte nicht eingelesen werden",				// 551
		"mpg123 Library wird nicht unterstützt",									// 552
		"mpg123 Decoder konnte nicht gestartet werden",								// 553
		"Kein MP3 Dekoder installiert",												// 554
		"Gewünschter MP3-Dekoder ist nicht installiert",							// 555
		"Unbekannter MP3-Dekoder",													// 556
		"MP3-Dekoder konnte nicht initialisiert werden",							// 557
		"Datumsformat wurde nicht erkannt",											// 558
		"Ungültige Adresse",														// 559
		"Parameter ausserhalb des gültigen Bereichs",								// 560
		"Gesuchte Grafik ist nicht in der MP3-Datei vorhanden",						// 561
		"Hexadezimal-String kann nicht zum Binary konvertiert werden",				// 562
		"Icecast Error",															// 563
		"Feature ist nicht einkompiliert",											// 564
		"Funktion wird nicht unterstützt",											// 565
		"SSL: Konnte DHParams nicht laden",											// 566
		"unknown"																	//
};



namespace ppl6 {

typedef struct tagErrorHandlers {
    const pplchar *(*geterror)(pplint32 code);
	pplint32 first_errno;
	pplint32 last_errno;
	struct tagErrorHandlers *nexthandler;
} ERRORHANDLERS;

static ERRORHANDLERS *errhandlers=NULL;

static CMutex Mutex;

void PrintPFPErrors()
/*!\brief Alle PPL-Fehlermeldungen auf stdout ausgeben
 * \ingroup Errors
 */
{
	pplint32 max=sizeof(errors) / sizeof(char *)-2;
	pplint32 i;
	for (i=0;i<=max;i++)
		printf ("%3u: %s\n",i,errors[i]);
}

static void ClearErrorHandlers()
/*!\ingroup Errors
 */
{
	Mutex.Lock();
	while (errhandlers) {
		ERRORHANDLERS *h=errhandlers;
		errhandlers=errhandlers->nexthandler;
		free(h);
	}
	Mutex.Unlock();
}



void SetError(ppluint32 errorcode)
/*! \brief Setzt einen Fehler
 * \ingroup Errors
 *
 *
 * Setzt einen Fehler. Diese Funktion ist mehrfach definiert und unterscheidet sich nur durch
 * ihre Parameter:
 * \code
 * void	SetError(ppldd errorcode);
 * void	SetError(ppldd errorcode, char *msg, ...);
 * void	SetError(ppldd errorcode, CString *msg);
 * void	SetError(ppldd errorcode, ppldd suberrorcode, char *msg=NULL, ...);
 * \endcode
 * \param errorcode Der Fehlercode, der gesetzt werden soll
 *
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	PPL_ERROR *t=GetErrorThreadPtr();
	if (!t) return;
	t->err=errorcode;
	t->suberr=0;
	t->ErrorText.Clear();
}

void SetError(ppluint32 errorcode, const char *msg, ...)
/*!\brief Setzt einen Fehler
 * \ingroup Errors
 *
 *
 * Setzt einen Fehler. Diese Funktion ist mehrfach definiert und unterscheidet sich nur durch
 * ihre Parameter:
 * \code
 * void	SetError(ppldd errorcode);
 * void	SetError(ppldd errorcode, char *msg, ...);
 * * void	SetError(ppldd errorcode, CString *msg);
 * void	SetError(ppldd errorcode, ppldd suberrorcode, char *msg=NULL, ...);
 * \endcode
 * \param errorcode Der Fehlercode, der gesetzt werden soll
 * \param msg Ein optionaler Formatstring
 * \param ... Weitere Parameter für den Formatstring
 *
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	PPL_ERROR *t=GetErrorThreadPtr();
	if (!t) return;
	t->err=errorcode;
	t->suberr=0;
	if (msg) {
		va_list args;
		va_start(args, msg);
		t->ErrorText.Vasprintf(msg,args);
		va_end(args);
	} else {
		t->ErrorText.Clear();
	}
}


void SetError(ppluint32 errorcode, CString &msg)
/*!\brief Setzt einen Fehler
 * \ingroup Errors
 *
 *
 * Setzt einen Fehler. Diese Funktion ist mehrfach definiert und unterscheidet sich nur durch
 * ihre Parameter:
 * \code
 * void	SetError(ppldd errorcode);
 * void	SetError(ppldd errorcode, char *msg, ...);
 * void	SetError(ppldd errorcode, CString &msg);
 * void	SetError(ppldd errorcode, ppldd suberrorcode, char *msg=NULL, ...);
 * \endcode
 * \param errorcode Der Fehlercode, der gesetzt werden soll
 * \param msg Ein String mit optionalem Fehlertext
 *
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	PPL_ERROR *t=GetErrorThreadPtr();
	if (!t) return;
	t->err=errorcode;
	t->suberr=0;
	t->ErrorText=msg;
}

void ExtendError(ppluint32 errorcode)
/*!\brief Fehler wird erweitert
 * \ingroup Errors
 *
 *
 * Der bisherige Fehlercode und Text wird in einen String gewandelt und als Zusatztext für den
 * neuen Fehlercode verwendet.
 *
 * \param errorcode Neuer Fehlertext
 */
{
	CString e;
	Error2String(e);
	SetError(errorcode,e);
}

void SetErrorFromErrno(const char *msg, ...)
/*!\brief Setzt einen Fehler abhängig vom Fehlercode der errno-Variablen
 * \ingroup Errors
 *
 *
 * Diese Funktion setzt einen Fehler, dessen Code vom Inhalt der errno-Variablen
 * abhängt.
 *
 * \param msg Ein optionaler Formatstring
 * \param ... Weitere Parameter für den Formatstring
 *
 * \see \link Errors Fehlerfunktionen \endlink
 * \see ppl6::TranslateErrno(int e)
 */
{
	CString s;
	int e=errno;
	PPL_ERROR *t=GetErrorThreadPtr();
	if (!t) return;
	t->err=TranslateErrno(e);
	t->suberr=e;
	if (msg) {
		va_list args;
		va_start(args, msg);
		s.Vasprintf(msg,args);
		if (t->err==3) {
			t->ErrorText.Setf("%s => %s",strerror(e),(const char*)s);
		} else {
			t->ErrorText=s;
		}
		va_end(args);
	} else {
		if (t->err==3) {
			t->ErrorText.Setf("%s",strerror(e));
		} else {
			t->ErrorText.Clear();
		}
	}
}

void SetError(ppluint32 errorcode, ppluint32 suberrorcode, const char *msg, ...)
/*!\brief Setzt einen Fehler
 * \ingroup Errors
 *
 *
 * Setzt einen Fehler. Diese Funktion ist mehrfach definiert und unterscheidet sich nur durch
 * ihre Parameter:
 * \code
 * void	SetError(ppldd errorcode);
 * void	SetError(ppldd errorcode, char *msg, ...);
 * void	SetError(ppldd errorcode, CString *msg);
 * void	SetError(ppldd errorcode, ppldd suberrorcode, char *msg=NULL, ...);
 * \endcode
 * \param errorcode Der Fehlercode, der gesetzt werden soll
 * \param suberrorcode Ein zusätzlicher optionaler Fehlercode
 * \param msg Ein optionaler Formatstring
 * \param ... Weitere Parameter für den Formatstring
 *
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	PPL_ERROR *t=GetErrorThreadPtr();
	if (!t) return;
	t->err=errorcode;
	t->suberr=suberrorcode;
	if (msg) {
		va_list args;
		va_start(args, msg);
		t->ErrorText.Vasprintf(msg,args);
		va_end(args);
	} else {
		t->ErrorText.Clear();
	}
}


pplint32 GetErrorCode()
/*!\brief Liefert den Fehlercode des zuletzt aufgetretenen Fehlers zurück
 * \ingroup Errors
 *
 *
 * Liefert den Fehlercode des zulTranslateErrno(errno);etzt aufgetretenen Fehlers zurück
 * \return Liefert den Fehlercode des zuletzt aufgetretenen Fehlers zurück
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	PPL_ERROR *t=GetErrorThreadPtr();
	if (!t) return 0;
	return t->err;
}

pplint32 GetSubError()
/*!\brief Liefert den zusätzlichen Fehlercode des zuletzt aufgetretenen Fehlers zurück
 * \ingroup Errors
 *
 *
 * Liefert den zusätzlichen Fehlercode des zuletzt aufgetretenen Fehlers zurück
 * \return Liefert den zusätzlichen Fehlercode des zuletzt aufgetretenen Fehlers zurück
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	PPL_ERROR *t=GetErrorThreadPtr();
	if (!t) return 0;
	return t->suberr;
}

const char *GetError()
/*!\brief Liefert den Fehlertext zum letzten aufgetretenen Fehlercode zurück
 * \ingroup Errors
 *
 *
 * Liefert den Fehlertext zum letzten aufgetretenen Fehlercode zurück.
 *
 * \return Der Text zu dem Fehlercode. War der Fehlercode unbekannt, wird "Unbekannter Fehlercode"
 * zurückgegeben.
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	PPL_ERROR *t=GetErrorThreadPtr();
	if (!t) return errors[1];
	return GetError(t->err);
}

const char *GetError(pplint32 code)
/*!\brief Liefert den Fehlertext zu einem bestimmten Code zurück
 * \ingroup Errors
 *
 *
 * Liefert den Fehlertext zu einem bestimmten Code zurück.
 * \param code Der gewünschte Fehlercode.
 * \return Der Text zu dem Fehlercode. War der Fehlercode unbekannt, wird "Unbekannter Fehlercode"
 * zurückgegeben.
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	pplint32 max=sizeof(errors) / sizeof(char *)-2;
	if (code<=max) return (tr(errors[code]));
	// Vielleicht gibt es einen Errorhandler fuer diesen Code?
	Mutex.Lock();
	ERRORHANDLERS *eh=errhandlers;
	while (eh) {
		if (eh->first_errno<=code && eh->last_errno>=code) {
			const char *ret=eh->geterror(code);
			Mutex.Unlock();
			if (ret) return ret;
			return (errors[3]);
		}
		eh=eh->nexthandler;
	}
	Mutex.Unlock();
	return (errors[3]);
}

const char *GetExtendedError()
/*!\brief Liefert zusätzlichen Fehlertext zurück
 * \ingroup Errors
 *
 *
 * Wurde mit SetError außer dem Fehlercode auch ein optionaler Text gesetzt,
 * kann er mit dieser Funktion ausgelesen werden.
 * \return Liefert einen char-Pointer auf den Text. War kein zusätzlicher Text gesetzt,
 * wird ein leerer String zurückgegeben.
 * \see SetError
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	PPL_ERROR *t=GetErrorThreadPtr();
	if (!t) return "";
	return t->ErrorText.GetPtr();
}

PPL_ERROR *StoreError()
/*!\brief Fehler speichern
 * \ingroup Errors
 *
 *
 * Falls PushError und PopError nicht verwendet werden können, kann mit diesem Befehl
 * der Fehler in einer Variablen gespeichert werden. Mit RestoreError kann der gespeicherte
 * Fehler wieder eingelesen werden, mit ClearError wird er gelöscht.
 *
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	PPL_ERROR *t=GetErrorThreadPtr();
	if (!t) return NULL;
	PPL_ERROR *e=new PPL_ERROR;
	if (!e) return NULL;
	e->err=t->err;
	e->suberr=t->suberr;
	e->ErrorText=t->ErrorText;
	e->flags=1;
	return e;
}

int StoreError(PPL_ERROR *e)
/*!\brief Fehler speichern
 * \ingroup Errors
 *
 *
 * Falls PushError und PopError nicht verwendet werden können, kann mit diesem Befehl
 * der Fehler in einer Struktur gespeichert werden. Mit RestoreError kann der gespeicherte
 * Fehler wieder eingelesen werden, mit ClearError wird er gelöscht.
 *
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	if (!e) return 0;
	PPL_ERROR *t=GetErrorThreadPtr();
	if (!t) return 0;
	e->err=t->err;
	e->suberr=t->suberr;
	e->ErrorText=t->ErrorText;
	e->flags=0;
	return 1;
}

void RestoreError(PPL_ERROR *e, bool clear)
/*!\brief gespeicherten Fehler wiederherstellen
 * \ingroup Errors
 *
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	PPL_ERROR *t=GetErrorThreadPtr();
	if (!t) return;
	if (e) {
		t->err=e->err;
		t->suberr=e->suberr;
		t->ErrorText=e->ErrorText;
		if (clear) {
			if (e->flags&1) delete e;
		}
	}
}

void ClearError(PPL_ERROR *e)
/*!\brief gespeicherten Fehler löschen
 * \ingroup Errors
 *
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	if (e) {
		delete e;
	}
}


pplint32 AttachErrorHandler(const pplchar *geterrorfunction(pplint32 code),pplint32 first, pplint32 last)
/*!\brief Routine zur Ausgabe von Fehlertexten hinzufügen
 * \ingroup Errors
 *
 *
 * Damit die Library selbst definierte Fehlercodes ausgeben kann, muss eine entsprechende
 * Routine in das System eingeklinkt werden. Dies geschieht mit dieser Funktion.
 * \param geterrorfunction
 * Pointer auf eine Funktion, die zu einem Fehlercode den jeweiligen Text zurückgibt
 * \param first Nummer des ersten Fehlercodes, der von dieser Funktion bearbeitet wird
 * \param last Nummer des letzten Fehlercodes
 * \return Bei Erfolg gibt die Funktion 1 zurück, bei einem Fehler 0. Der Fehler kann
 * über die Fehlerfunktionen abgefragt werden.
 * \note Eine so hinzugefügte Fehlerroutine kann nicht wieder gelöscht werden
 * \par Example:
 * \code
static char * errors[] = {
	"Mein erster Fehlercode",							// 70000
	"Noch ein Fehler",									// 70001
	NULL
};

static const char *myerrors(int code)
{
	int max=sizeof(errors) / sizeof(char *)-2;
	if (code<70000 || code>70000+max) return "Unknown Error";
	code-=70000;
	return (errors[code]);
}

int main(int argc, char **argv)
{
	ppl6::AttachErrorHandler(myerrors,70000, 71000);
	ppl6::SetError(70001,"Nur ein Test");
	ppl6::PrintError();
}
\endcode
 *
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	Mutex.Lock();
	if (!errhandlers) {
		errhandlers=(ERRORHANDLERS*)malloc(sizeof(ERRORHANDLERS));
		if (!errhandlers) {
			Mutex.Unlock();
			SetError(2);
			return 0;
		}
		atexit(ClearErrorHandlers);
		errhandlers->nexthandler=NULL;
		errhandlers->geterror=geterrorfunction;
		errhandlers->first_errno=first;
		errhandlers->last_errno=last;
		Mutex.Unlock();
		return 1;
	}
	ERRORHANDLERS *eh=errhandlers;
	while (eh) {
		if (eh->geterror==geterrorfunction && eh->first_errno==first && eh->last_errno==last) {	// Handler gibts schon
			Mutex.Unlock();
			return 1;
		}
		if (!eh->nexthandler) {
			eh->nexthandler=(ERRORHANDLERS*)malloc(sizeof(ERRORHANDLERS));
			eh=eh->nexthandler;
			if (!eh) {
				SetError(2);
				Mutex.Unlock();
				return 0;
			}
			eh->geterror=geterrorfunction;
			eh->first_errno=first;
			eh->last_errno=last;
			eh->nexthandler=NULL;
			Mutex.Unlock();
			return 1;
		}
		eh=eh->nexthandler;
	}
	Mutex.Unlock();
	return 1;
}

void PrintError()
/*!\brief Gibt den letzten aufgetretenen Fehler auf STDOUT aus
 * \ingroup Errors
 *
 *
 * Gibt den letzten aufgetretenen Fehler auf STDOUT aus
 *
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	PrintDebug(tr("Error %u: %s"),GetErrorCode(),
		GetError());
	const char *tmp=GetExtendedError();
	if ((tmp) && strlen(tmp)>0) {
		PrintDebug (" [%s]",tmp);
	}
	PrintDebug ("\n");
}

void Error2String(CString &str)
/*!\brief Speichert den letzten aufgetretenen Fehler in einem String
 * \ingroup Errors
 *
 *
 * Speichert den letzten aufgetretenen Fehler in einem String
 * \param[out] str Ein Pointer auf eine CString-Klasse, in der der Fehlertext
 * gespeichert wird.
 *
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	str.Sprintf(tr("Error %u: %s"),GetErrorCode(),
		GetError());
	const char *tmp=GetExtendedError();
	if ((tmp) && strlen(tmp)>0) {
		str.Concatf (" [%s]",tmp);
	}
}

CString Error2String()
/*!\brief Speichert den letzten aufgetretenen Fehler in einem String
 * \ingroup Errors
 *
 *
 * Speichert den letzten aufgetretenen Fehler in einem String
 * \returns CString, der die Fehlerbeschreibung enthält
 *
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	CString str;
	str.Sprintf(tr("Error %u: %s"),GetErrorCode(),
		GetError());
	const char *tmp=GetExtendedError();
	if ((tmp) && strlen(tmp)>0) {
		str.Concatf (" [%s]",tmp);
	}
	return str;
}

CString Error()
/*!\brief Speichert den letzten aufgetretenen Fehler in einem String
 * \ingroup Errors
 *
 *
 * Speichert den letzten aufgetretenen Fehler in einem String
 * \returns CString, der die Fehlerbeschreibung enthält
 *
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
	CString str;
	str.Sprintf(tr("Error %u: %s"),GetErrorCode(),
		GetError());
	const char *tmp=GetExtendedError();
	if ((tmp) && strlen(tmp)>0) {
		str.Concatf (" [%s]",tmp);
	}
	return str;
}


int DisplayError()
/*!\brief Zeigt den zuletzt aufgetretenen Fehler in einem Fenster an
 * \ingroup Errors
 *
 *
 * Zeigt den zuletzt aufgetretenen Fehler in einem Fenster an
 * \note Diese Funktion funktioniert zur Zeit nur unter Windows
 *
 * \see \link Errors Fehlerfunktionen \endlink
 */
{
#ifdef _WIN32
	PushError();
	CString Text, Caption;
	Caption.Setf(tr("PPL - Fehler: %u"),GetErrorCode());
	Text.Setf(tr("In der Anwendung ist ein Fehler aufgetreten.\n"));
	Text.Concatf(tr("Fehlercode: %u\n"),GetErrorCode());
	Text.Concatf(tr("Fehler: %s\n"),GetError());
	const char *tmp=GetExtendedError();
	if ((tmp) && strlen(tmp)>0) {
		Text.Concatf(tr("Erweiterte Beschreibung: %s\n"),tmp);
	}
	Text.Transcode("UTF-8","ISO-8859-1");
	int ret=::MessageBox(NULL,(const char*)Text,(const char*)Caption,MB_ABORTRETRYIGNORE|MB_ICONERROR);
	PopError();
	return ret;
#else
	PrintError();
	return 0;
#endif
}

int TranslateErrno(int e)
/*!\brief Übersetzt ERRNO in einen PPL-Fehlercode
 * \ingroup Errors
 *
 *
 * Diese Funktion übersetzt die errno-Variable des Betriebssystems
 * in einen PPL-Fehlercode
 *
 * \param e Der Wert aus der errno-Variablen
 * \return Der Dazugehörige PPL-Fehlercode oder "3", wenn der Code
 * der errno-Variablen nicht bekannt ist
 * \see ppl6::SetErrorFromErrno(char *msg, ...)
 * */
{
#ifndef CPPCHECK
	switch (e) {
		case 0:
			return 0;
#ifdef EAGAIN
		case EAGAIN: return 277;
#endif
#ifdef EBADF
		case EBADF:	return 72;
#endif
#ifdef EINVAL
		case EINVAL:return 279;
#endif
#ifdef EOPNOTSUPP
		case EOPNOTSUPP: return 278;
#endif
#ifdef EPERM
		case EPERM:	return 360;
#endif
#ifdef EROFS
		case EROFS: return 361;
#endif
#ifdef EFAULT
		case EFAULT: return 307;
#endif
#ifdef ENAMETOOLONG
		case ENAMETOOLONG: return 363;
#endif
#ifdef ENOENT
		case ENOENT: return 364;
#endif
#ifdef ENOENT
		case ENOMEM: return 2;
#endif
#ifdef ENOTDIR
		case ENOTDIR: return 366;
#endif
#ifdef EACCES
		case EACCES: return 367;
#endif
#ifdef ELOOP
		case ELOOP: return 368;
#endif
#ifdef EIO
		case EIO: return 369;
#endif
#ifdef E2BIG
		case E2BIG: return 370;
#endif
#ifdef EBADMSG
		case EBADMSG: return 371;
#endif
#ifdef EBUSY
		case EBUSY: return 372;
#endif
#ifdef ECANCELED
		case ECANCELED:return 373;
#endif
#ifdef ECHILD
		case ECHILD: return 374;
#endif
#ifdef EDEADLK
		case EDEADLK: return 375;
#endif
#ifdef EDOM
		case EDOM: return 376;
#endif
#ifdef EEXIST
		case EEXIST: return 18;
#endif
#ifdef EFBIG
		case EFBIG: return 245;
#endif
#ifdef EINPROGRESS
		case EINPROGRESS: return 377;
#endif
#ifdef EINTR
		case EINTR: return 310;
#endif
#ifdef EISDIR
		case EISDIR: return 313;
#endif
#ifdef EMFILE
		case EMFILE: return 379;
#endif
#ifdef EMLINK
		case EMLINK: return 380;
#endif
#ifdef EMSGSIZE
		case EMSGSIZE: return 381;
#endif
#ifdef ENFILE
		case ENFILE: return 382;
#endif
#ifdef ENODEV
		case ENODEV: return 378;
#endif
#ifdef ENOEXEC
		case ENOEXEC: return 383;
#endif
#ifdef ENOLCK
		case ENOLCK: return 384;
#endif
#ifdef ENOSPC
		case ENOSPC: return 311;
#endif
#ifdef ENOSYS
		case ENOSYS: return 385;
#endif
#ifdef ENOTEMPTY
		case ENOTEMPTY: return 386;
#endif
#ifdef ENOTTY
		case ENOTTY: return 388;
#endif
#ifdef ENXIO
		case ENXIO: return 387;
#endif
#ifdef EPIPE
		case EPIPE: return 308;
#endif
#ifdef ERANGE
		case ERANGE: return 305;
#endif
#ifdef ESPIPE
		case ESPIPE: return 362;
#endif
#ifdef ESRCH
		case ESRCH: return 365;
#endif
#ifdef ETIMEDOUT
		case ETIMEDOUT: return 174;
#endif
#ifdef EXDEV
		case EXDEV: return 389;
#endif
#ifdef EADDRNOTAVAIL
		case EADDRNOTAVAIL: return 406;
#endif
#ifdef ECONNREFUSED
		case ECONNREFUSED: return 407;
#endif
#ifdef ENOTCONN
		case ENOTCONN: return 408;
#endif
#ifdef ECONNRESET
		case ECONNRESET: return 409;
#endif
#ifdef EALREADY
		case EALREADY: return 454;
#endif
#ifdef ENOTSOCK
		case ENOTSOCK: return 455;
#endif
#ifdef EDESTADDRREQ
		case EDESTADDRREQ: return 456;
#endif
#ifdef EPROTOTYPE
		case EPROTOTYPE: return 457;
#endif
#ifdef ENOPROTOOPT
		case ENOPROTOOPT: return 458;
#endif
#ifdef EPROTONOSUPPORT
		case EPROTONOSUPPORT: return 459;
#endif
#ifdef ESOCKTNOSUPPORT
		case ESOCKTNOSUPPORT: return 460;
#endif
#ifdef EPFNOSUPPORT
		case EPFNOSUPPORT: return 461;
#endif
#ifdef EAFNOSUPPORT
		case EAFNOSUPPORT: return 462;
#endif
#ifdef EADDRINUSE
		case EADDRINUSE: return 463;
#endif
#ifdef ENETDOWN
		case ENETDOWN: return 464;
#endif
#ifdef ENETUNREACH
		case ENETUNREACH: return 465;
#endif
#ifdef ENETRESET
		case ENETRESET: return 466;
#endif
#ifdef ECONNABORTED
		case ECONNABORTED: return 467;
#endif
#ifdef ENOBUFS
		case ENOBUFS: return 468;
#endif
#ifdef EISCONN
		case EISCONN: return 469;
#endif
#ifdef ESHUTDOWN
		case ESHUTDOWN: return 470;
#endif
#ifdef ETOOMANYREFS
		case ETOOMANYREFS: return 471;
#endif
#ifdef EHOSTDOWN
		case EHOSTDOWN: return 472;
#endif
#ifdef EHOSTUNREACH
		case EHOSTUNREACH: return 473;
#endif
#ifdef EPROCLIM
		case EPROCLIM: return 474;
#endif
#ifdef EUSERS
		case EUSERS: return 475;
#endif
#ifdef EDQUOT
		case EDQUOT: return 476;
#endif
#ifdef ESTALE
		case ESTALE: return 477;
#endif
#ifdef EREMOTE
		case EREMOTE: return 478;
#endif

	};
#endif	// CPPCHECK
	//printf("errno: %i, %x\n",errno,errno);
	return 3;
}


} // end of namespace ppl


