# Refactoring WinMusik
WinMusik ist ein Programm zum Dokumentieren und Archivieren von Musiktiteln auf Audio-CDs, DVDs,
Audiokassetten, Schallplatten, Festplatten und anderen Audiogeräten. Es verfügt über einen
intelligenten Editor und eine einfach zu bedienende Suchoberfläche. Jeder Audio-Titel kann auch als Datei im Filesystem abgelegt werden, so dass er beispielsweise im Suchergebnis direkt geöffnet oder abgespielt werden kann.

- Zentrale Verwaltung der Audio-Dateien im Filesystem
- Durchdachtes Benutzerinterface zum Anlegen, Editieren und Verwalten von Musiktiteln
- Importiert MP3- und Aiff-Dateien von der Festplatte
- ID3v2-Tag-support, inklusive Cover-Bilder, Key und Energy
- Umfangreiche Suchfunktionen
- Drucken von CD-Covern
- Suchlisten
- Playlists
- "DJ-Modus" in Playlists mit Unterstützung für Native Instrument's Traktor 3 Cue-Points
- Läuft unter Windows 10/11, Linux, FreeBSD und anderen Unixen

# Ziele
- Upgrade von pplib (Version 7) auf pplib8 (Version 8)
- Verwendung von CMake zum Bauen von pplib8
- Eventuell kompletter Umstieg auf CMake?
- Anpassung des Codes an die neuen Strukturen und Funktionen von pplib8
- Modernisierung des Codes und Nutzung moderner C++-Features
- Refactoring der Datenbank
- Filehandling verbessern:
  - Zentrales Modul zum schreiben, lesen und auffinden von Audio-Dateien
- Neben mp3 und Aiff auch Unterstützung von Wave, Flac und Opus? Eventuel Ogg?
- Doku aktualisieren
- Nicht mehr benutzte Features entfernen



## Datenbank
Aktuell ein monolithisches File, welches via Nextcloud synchronisiert wird. Das File ist >200 MB groß, bei jeder kleinsten Änderung muss die gesamte Datei synchronisiert werden.

Ich überlege, ob ich dieses File in kleinere Dateien aufsplitte, so dass mein Editieren in der Regel nur das neuste oder betroffene Segment verändert und nicht die gesamte Datei synchronisiert werden muss.

Ein Augenmerk sollte auch auf die Struktur und Verwaltung der Datenobjekte gelegt werden. Hier sehe ich zum Teil Pointer auf Pointer, wo wahrscheinlich ein Vector oder ein anderes Container-Objekt sinnvoller wäre.

## pplib8
Dies ist eine überarbeitete Version der Bibliothek pplib, Version 7, die hier bisher verwendet wird. Sie bringt unter anderem eine modernisierte und besser getestete Codebasis mit sich, und eine modernisierte API. Einige der Änderungen brechen bewust die alte API, zum Beispiel pplib::Dir, die hier ebenfalls verwendet wird. Ein großteil der anderen Klassen und Funktionen ist unverändert oder erweitert worden. Ferner werden mehr Features von modernem C++20 genutzt.


# Ausgangsbasis
- pplib (Version 7) verwendet Autoconf. Winmusik hat sein eigenes Autoconf Configure-Skript, welches auch pplib7 konfiguriert.
- pplib8 verwendet CMake
- wmlib wird ebenfalls über das Configure-Skriot von Winmusik konfiguriert
- Winmusik selbst (GUI) wird über das Configure-Skript von Winmusik konfiguriert, verwendet aber qmake zum Bauen der GUI.


# Projektstruktur
- pplib: Alte Version der Bibliothek (pplib7)
- pplib8: Neue Version der Bibliothek (pplib8)
  - CMakeLists.txt: CMake-Konfigurationsdatei für pplib8
  - include: Header-Dateien der Bibliothek, inklusive API-Dokumentation
  - src: Quellcode der Bibliothek
- wmlib: Eigene Bibliothek von WinMusik
  - src: Quellcode der Bibliothek
  - include: Header-Dateien der Bibliothek
  - tests: Unit-Tests der Bibliothek (Google Test)
- gui: GUI und Hauptanwendung
  - inlcude: einige allgemeine Include-Dateien
  - src: Quellcode der GUI und Hauptanwendung. Hier liegen auch Header-Dateien der GUI-Formulare und die Qt-spezifischen .ui-Dateien
  - resources: Ressourcen-Dateien der GUI, wie Icons, Bilder und andere Mediendateien
  - widgets: Eigene GUI-Widgets der Anwendung
- docs: Dokumentation, insbesondere auch das Fileformat der Datenbank

# Technische Feinheiten
WinMusik verwendet set_locale, um die Locale-Einstellungen der Anwendung auf UTF-8 zu setzen, sowohl unter Windows als auch unter anderen Betriebssystemen. Wir können also davon ausgehen, dass Inhalte in der Klasse pplib::String immer im UTF-8 Encoding vorliegen.

Noch ungestestet sind die "ifdefs" in pplib/types/string.h für Qt6. Diese sorgend dafür, dass QString in pplib::String gewandelt wird und umgekehrt. Hier könnten noch Fehler vorhanden sein.


# Projektphasen
- wmlib auf pplib8 portieren. Ab hier ist die GUI wahrscheinlich kaputt. Vielleicht brauchen wir einen Konvertierungslayer, der zum Beispiel ppl7::String in pplib::String konvertiert.

## Migration auf pplib8
Ausführliche Landkarte (verwendete Symbole, API-Brüche, Datei- und Batch-Reihenfolge):
[docs/pplib8-migration.md](docs/pplib8-migration.md)

- Namespace ändert sich von ppl7 auf pplib
- Klassennamen sind weitestgehnd gleich
- Includes sind zwar weiterhin über zentrale Aggregate-Dateien verfügbar, ähnlich wie vorher über "ppl7.h", es wird aber empfohlen, die spezifischen Header-Dateien direkt einzubinden, um Abhängigkeiten klar zu machen.
- Größere, API-brechende Änderungen sind in pplib::File und pplib::Dir erfolgt.
- Unter Umständen sind in pplib mehr Funktionen verfügbar oder für spezielle Anwendungsfälle bessere Alternativen vorhanden (std-kompatible Itteratoren zum Beispiel)
- pplib8 hat eine hohe Testabdeckung und ein Großteil des Codes wurde mehreren Code-Reviews unterzogen. WinMusik ist aber der erste größere Verbraucher der Bibliothek, es könnten also noch unerwartete Probleme oder subtile Fehler auftreten.

Wichtige Klassen mit ihren Header-Dateien:
- pplib::String - <pplib/types/string.h>
- pplib::WideString - <pplib/types/widestring.h>
- pplib::AssocArray - <pplib/types/assocarray.h>
- pplib::ByteArray - <pplib/types/bytearray.h>
- pplib::ByteArrayPtr - <pplib/types/bytearrayptr.h>
- pplib::DateTime - <pplib/types/datetime.h>
- pplib::Variant - <pplib/types/variant.h>

- pplib::Mutex - <pplib/core/mutex.h>
- pplib::Logger - <pplib/core/logging.h>
- pplib::Thread - <pplib/core/thread.h>
- pplib::ppl_time_t - <pplib/core/time.h>

Verschiedene Funktionen:
- <pplib/core/functions.h>
- <pplib/core/stringfunctions.h>




# Erledigt
- Vorhandener Code muss wieder kompilieren
- Zentrales Makefile anpassen
- Einbindung von pplib8 in das bestehende WinMusik-Projekt
- wmlib auf CMake umstellen
