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
- Signierung der ausführbaren Dateien und Installationspakete
- Tonart-Erkennung
- BPM-Erkennung
- Traktor-Style Waveforms
- Integrierter Audio-Player

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

## Signierung der ausführbaren Dateien und Installationspakete
Über die gemeinnützige **SignPath Foundation** (https://about.signpath.io/open-source/free-code-signing) können Open-Source-Projekte kostenlos offizielle Code-Signing-Zertifikate nutzen, um Warnungen von Windows Defender SmartScreen und UAC zu vermeiden.

### Voraussetzungen:
- Öffentliches GitHub-Repository mit anerkannter Open-Source-Lizenz (GPL v3 erfüllt).
- Builds müssen reproduzierbar über CI (GitHub Actions) laufen, damit sichergestellt ist, dass das Binärpaket aus dem Quellcode gebaut wurde.

### Ablauf & Schritte zur Integration:
1. **Bewerbung:** Antrag auf kostenloses Signieren bei der SignPath Foundation einreichen (Projektlink + Open-Source-Nachweis).
2. **Organisation & Projekt in SignPath anlegen:** Zertifikatsverwaltung und Vertrauensstufe konfigurieren.
3. **GitHub Actions Workflow einrichten:**
   - Windows-Runner (`windows-latest`) baut `WinMusik.exe`.
   - SignPath GitHub Action signiert `WinMusik.exe`.
   - Inno Setup baut das Installationsprogramm (`setup64.iss`).
   - SignPath signiert das fertige Installer-Binary (`WinMusik-...-Setup.exe`).
   - Veröffentlichung des signierten Installers in den GitHub Releases.

## Tonart-Erkennung
- **Ziel:** Automatische Erkennung von Tonart (Key), BPM und Energy direkt in WinMusik, um externe Tools wie *Mixed in Key* zu ersetzen.
- **Prinzip:**
  1. Audio decodieren (z. B. über `pplib::AudioDecoder`).
  2. FFT pro Zeitfenster zur Frequenzanalyse.
  3. Frequenzen auf die 12 Halbtöne der Tonleiter falten $\rightarrow$ Chromagramm (Pitch Class Profile, PCP).
  4. Mit den 24 Krumhansl-Schmuckler-Tonartprofilen (12 Dur, 12 Moll) korrelieren; das Profil mit der höchsten Korrelation bestimmt die Tonart.
  5. Mapping über die bestehende Klasse `MusicalKeys` (Camelot z. B. 8A/8B, OpenKey oder Standard-Notation).
- **Referenz / Vorlage:** [`libKeyFinder`](https://github.com/ibsh/libKeyFinder) (schlanke, freie C++-Bibliothek, wird u. a. von *Mixxx* genutzt).

## 3-Band-Farb-Waveform (Traktor-Style) & Preview
- **Ziel:** Visuelle Vorschau des Tracks, an der man musikalische Struktur, Drops, Vocals und Rhythmus auf einen Blick erkennt.
- **Prinzip (Frequenzweiche per IIR-Filter):**
  Statt langsamer FFT werden 3 performante digitale Biquad-Filter (Linkwitz-Riley Crossover) verwendet:
  - **Tiefpass (< 200 Hz):** Bassdrum, Bassline $\rightarrow$ **Rot**
  - **Bandpass (200 Hz – 2.5 kHz):** Melodie, Vocals, Snare $\rightarrow$ **Grün**
  - **Hochpass (> 2.5 kHz):** Hi-Hats, Cymbals, Air $\rightarrow$ **Blau**
- **Speicherung als Preview:**
  - Aggregation z. B. alle 1.000 Samples (~44 Punkte/Sekunde): Speichern von Amplitude (`uint8_t`) und Farbanteilen (R, G, B je `uint8_t`).
  - Ein 5-Minuten-Track belegt nur ca. 50 KB Daten.
  - Kann als eigener kleiner Datenbank-Chunk (z. B. `WAVF`) gespeichert werden und muss nur einmalig berechnet werden.
- **Darstellung:** Eigenes `QWidget` mit `QPainter`, das vertikale farbige Linien rendert.

## Integrierter Audio-Player
- **Ziel:** Schnelles Vorhören von Tracks direkt in der Oberfläche, nahtlos mit der Waveform gekoppelt.
- **Umsetzung mit Qt 6:**
  - `QMediaPlayer` und `QAudioOutput` nutzen (unterstützt MP3, FLAC, WAV, AAC nativ über die Multimedia-Backends des Betriebssystems).
- **Interaktion mit der Waveform:**
  - Klick in die Waveform ruft `player->setPosition(ms)` auf zum sofortigen Spulen.
  - Das Signal `player->positionChanged(qint64)` steuert eine vertikale Playhead-Linie über der Waveform.


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


