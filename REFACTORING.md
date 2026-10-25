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

## Ziele

**Erledigt:**
- Upgrade von pplib (Version 7) auf pplib8 (Version 8) => DONE
- Verwendung von CMake zum Bauen von pplib8 => DONE
- Eventuell kompletter Umstieg auf CMake? => DONE
- Anpassung des Codes an die neuen Strukturen und Funktionen von pplib8 => DONE

**Offen:**
- Modernisierung des Codes und Nutzung moderner C++-Features
- Refactoring der Datenbank
- Filehandling verbessern:
  - Zentrales Modul zum schreiben, lesen und auffinden von Audio-Dateien
- Neben mp3 und Aiff auch Unterstützung von Wave, Flac und Opus? Eventuel Ogg?
- Doku aktualisieren
- Nicht mehr benutzte Features entfernen

## Datenbank
Aktuell ein monolithisches File, welches via Nextcloud synchronisiert wird. Das File ist >200 MB groß, bei jeder kleinsten Änderung muss die gesamte Datei synchronisiert werden. Das File selber besteht aus einem PFPFile-Header und den eigentlichen Datenblöcken als Chunks mit Header und Payload. Formatbeschreibung: docs/fileformat31.dox und docs/chunks.dox

Ich überlege, ob ich dieses File in kleinere Chunks aufsplitte, so dass beim Editieren in der Regel nur das neuste oder betroffene Segment verändert und nicht mehr die gesamte Datenbank synchronisiert werden muss. Dazu muss intern im CStorageItem neben der Position im File auch vermerkt werden, aus welcher Datei der Datensatz stammt. Dateien sollten vielleicht einfach durchnummeriert werden, dann kann man mit Integern arbeiten.

Beim Start der Anwendung müssen die alte Datenbank automatisch migriert werden.

Ein Augenmerk sollte auch auf die Struktur und Verwaltung der Datenobjekte gelegt werden. Hier sehe ich zum Teil Pointer auf Pointer, wo wahrscheinlich ein Vector oder ein anderes Container-Objekt sinnvoller wäre.

### Zukünftige Architektur
Es wird ein kleines File geben mit Metadaten. Hier wird insbesondere die Transaxtion-ID (künftig 8 Byte) gespeichert, die für die Konsistenz und Nachverfolgbarkeit der Datenbankänderungen wichtig ist.

Daten werden auf mehrere Dateien aufgesplittet, wobei jede Datei eine fortlaufende Nummer erhält. Die Datensätze werden in der Reihenfolge der Dateien und Chunks eingelesen.

Bei Änderungen werden die vorhandenen Daten nicht angefasst, sondern in eine Logfile (WAL, Write-Ahead-Log) geschrieben. Beim Einlesen wird zunächst die Hauptdatenbank eingelesen und anschließend die Änderungen aus dem Logfile angewendet, um den aktuellen Zustand zu rekonstruieren.

Die WAL-Datei sollte hin und wieder in die Hauptdatenbank integriert werden, um die Performance beim Einlesen zu verbessern. Dazu könnte man beim Einlesen ermitteln, wieviele Datensätze oder wieviel Speicherplatz durch Änderungen überschrieben wurden und entsprechend einen Compact-Vorgang anbieten, bei dem die Hauptdatenbank inklusive aller Änderungen neu geschrieben wird. Auch bei überschreiten einer bestimmten Größe der WAL-Datei könnte man das anbieten.

## pplib8
Dies ist eine überarbeitete Version der Bibliothek pplib, Version 7, die hier bisher verwendet wird. Sie bringt unter anderem eine modernisierte und besser getestete Codebasis mit sich, und eine modernisierte API. Einige der Änderungen brechen bewust die alte API, zum Beispiel pplib::Dir, die hier ebenfalls verwendet wird. Ein großteil der anderen Klassen und Funktionen ist unverändert oder erweitert worden. Ferner werden mehr Features von modernem C++20 genutzt.




## Projektstruktur
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

## Technische Feinheiten
**Encoding: UTF-8**
WinMusik verwendet set_locale, um die Locale-Einstellungen der Anwendung auf UTF-8 zu setzen, sowohl unter Windows als auch unter anderen Betriebssystemen. Wir können also davon ausgehen, dass Inhalte in der Klasse pplib::String immer im UTF-8 Encoding vorliegen.

**Qt6 String Konvertierung**
Noch ungestestet sind die "ifdefs" in pplib/types/string.h für Qt6. Diese sorgend dafür, dass QString in pplib::String gewandelt wird und umgekehrt. Hier könnten noch Fehler vorhanden sein.

## Entscheidungen
- AudioCD / CDDB: Dieses Feature wurde in der GUI lange nicht mehr benutzt. Die CDDB und freedb.org existieren nicht mehr. Das Feature wird daher hier ausgebaut und in pplib8 nicht mehr implementiert. Falls wir es in Zukunft wieder benötigen, können wir eine passende Implementierung in der Gui oder in wmlib nachreichen.


