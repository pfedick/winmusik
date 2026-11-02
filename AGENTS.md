# Projekt: WinMusik
WinMusik ist ein GUI-Anwendung zum Dokumentieren und Archivieren von Musiktiteln auf Audio-CDs, DVDs, Kassetten, Schallplatten und Audio-Dateien auf Festplatten. Es ist in C++20 geschrieben, verwendet Qt6 als GUI-Framework, nutzt eine eigene Bibliothek `wmlib` für die Datenverwaltung, und setzt auf pplib8 für grundlegende Datenstrukturen und Filehandling auf. Es wird aktiv auf Windows MinGW/MSys, Linux und FreeBSD getestet.

Im Wesentlichen besteht es aus einem Editor zum erfassen und visualisieren von Musiktiteln und einer einfach zu bedienenden Suchoberfläche. Jeder Audio-Titel auf CD oder anderen Medien kann auch als Datei im Filesystem abgelegt werden, so dass er beispielsweise im Suchergebnis direkt geöffnet oder abgespielt werden kann. Ferner verfügt es über Suchlisten und Playlisten, kann ID3-Tags auslesen und bearbeiten und seine Playlisten können Cue-Points von Native Instrument's Traktor 4 Pro lesen. Es unterstützt zudem das Drucken von CD-Covern und bietet einen "DJ-Modus" für die Wiedergabe von Playlists, mit exakten Timing-Parametern für Cue-In und Cue-Out, unter Berücksichtigung der BPM, so dass bei Erstellung der Playlist die Gesamtlaufzeit berechnet werden kann.

# Aktueller Stand
Siehe `REFACTORING.md` für eine detaillierte Übersicht der aktuellen Aufgaben und des Refactoring-Plans.


## Projektstruktur
- `pplib8/` – Submodul `pfedick/pplib` (Branch `pplib8`), neue Bibliothek, CMake + eigenes `Makefile`
- `wmlib/` – WinMusik-Bibliothek: `src/`, `include/`, `tests/src/`, `tests/testdata/`, Google Test vendored unter `tests/googletest-release-1.10.0`
- `gui/` – Qt6-Hauptanwendung: `src/`, `widgets/`, `resources/`, `WinMusik.pro` (qmake6), generierte `ui_*.h`
- `docs/` – Doku, insbesondere das Datenbank-Dateiformat (`fileformat.dox`, `fileformat31.dox`)
- `configure.ac`, `Makefile` (Root), `REFACTORING.md` – Build und Refactoring-Plan
- `codereview/todo/`, `codereview/done/`, `codereview/obsolete/` – in WinMusik noch nicht vorhanden; Vorlage liegt in `pplib8/codereview/`
- `Makefile`: Zentrales Makefile zum Bauen und Testen
- `coverage.xml`: Coverage-Report im XML-Format
- `coverage_html/`: Coverage-Reports im HTML-Format
  
## Testdaten
- Verwende vorhandene Testdaten unter `wmlib/tests/testdata/` (WinMusik) bzw. `pplib8/tests/testdata/` (Bibliothek)
- Reichen die Testdaten für Corner-Cases nicht aus, sind sie im Unit-Test durch Inline-Strings oder zusätzliche Testdateien zu ergänzen.


## Build und Tests

Nicht die CMakeTools-Erweiterung benutzen. WinMusik verwenden zwar CMake, wird aber über handgeschriebenes Root-`Makefile` gebaut.

WinMusik-Root (`Makefile`, vom `configure`-Skript erzeugt):

- `./configure` – konfiguriert WinMusik, wmlib und das Submodul `pplib` (pplib7).
- `make debug` / `make release` – pplib7 + wmlib + GUI in einem Durchlauf.
- `make wmlib`, `make gui` – nur Teilbereiche; `make mingw`, `make mingw_debug` – Windows/MinGW.
- `make clean`, `make distclean`, `make install`. Kein `test`- oder `coverage`-Target im Root.

Tests der eigenen Bibliothek (`wmlib/tests/Makefile`):

- `make -C wmlib/tests test` – baut und startet `wmlib/tests/testsuite`.
- `make -C wmlib/tests coverage` – baut `testsuite_coverage` und fährt ihn an.
- `make -C wmlib/tests xml` – Google-Test-XML; `make -C wmlib/tests memcheck` – Valgrind.

Einzelne Tests: erst `make -C wmlib/tests`, dann
`cd wmlib/tests && ./testsuite --gtest_filter=StorageTest*`

pplib8 nur bei Bibliotheksarbeit am Submodul, dann dessen eigene Toolchain:
`make -C pplib8 build_tests`, `make -C pplib8 test`, `make -C pplib8 coverage`.

Achtung: GUI und wmlib-Tests linken aktuell `pplib/debug/libpplib.a` (pplib7), nicht pplib8. Jede Umstellung auf pplib8 ist ein eigener, expliziter Schritt (`gui/WinMusik.pro`, `wmlib/tests/Makefile`, `configure.ac`).

## Coding Standards
- **Pragmatisches C++20:** Es wird geradliniger, wartbarer Code gewünscht, der zum vorhandenen Stil der Bibliothek passt.
- Keine verschachtelten Ranges-Pipelines oder Template-Metaprogrammierung, wenn einfache Schleifen und Funktionen genügen. 
- Keine komplexen Lambda-Ausdrücke, wenn einfache Funktionen ausreichen.
- Bibliothekscode hat keine statischen oder globalen mutablen Zustände. Bei Bedarf Mutexes, Locks oder atomare Operationen. Kein geteilter Zustand ohne Kontextobjekt des Aufrufers.
- Header schlank halten; keine unnötigen Includes.
- Interner Code in `pplib8`/`wmlib` nutzt `pplib::String` und `std::string_view` (kein `std::string`, vermeide Heap-Allokationen durch Substrings).
- `gui/` ist Qt6-Code: dort sind `QString`, `QList`, `.ui`-Dateien und Qt-Signale normal und werden nicht gegen die Bibliotheksregeln ausgetauscht. Qt-Typen an der Grenze zu `wmlib` in `std::string_view`/`pplib::String` übersetzen.

