# Landkarte: Migration wmlib/GUI von ppl7 auf pplib8

Stand: 2026-10-18. Erzeugt durch Auswertung aller `ppl7::`-Vorkommen in `wmlib/` und `gui/`
sowie Vergleich der Header von `pplib/include/ppl7*` mit `pplib8/include/pplib*`.

**Status:** wmlib ist vollständig auf pplib8 umgestellt — `make wmlib_tests` baut durch, alle
53 Tests grün. Die GUI ist jetzt in CMake eingebunden (siehe Abschnitt 8); ihre Migration ist
der nächste Schritt. Offene Punkte: CDDB/AudioCD in pplib8 (nur GUI betreffend, siehe
Abschnitt 7) und die GUI als Ganzes.

## 1. Ausgangslage

- `wmlib` ist auf CMake umgestellt und verlinkt bereits `pplib::core`, `pplib::audio`,
  `pplib::internet` (siehe `wmlib/CMakeLists.txt`). Der Root-`CMakeLists.txt` bindet `pplib8`
  als Submodul ein; kryptografische/grafische/DB-Module von pplib8 sind abgeschaltet.
- Der Build ist aktuell kaputt: alle wmlib-Quellen include-n noch `<ppl7.h>`
  (`fatal error: ppl7.h: Datei oder Verzeichnis nicht gefunden`).
- Bereits migriert: `wmlib/include/wm_id3tagsaver.h` + `wmlib/src/CID3TagSaver.cpp`
  (nutzen `pplib::` und spezifische pplib-Header).
- Die ID3Tag-API ist zwischen pplib7 und pplib8 **identisch** (Methodenliste verglichen) —
  der bereits migrierte Teil ist also repräsentativ für „nur Namespace + Includes".

## 2. Include-Mapping

| alt (pplib7) | neu (pplib8) |
|---|---|
| `ppl7.h` | `pplib.h` (Aggregat) oder gezielt `pplib/core.h` + `pplib/types.h` |
| `ppl7-types.h` | `pplib/types.h` |
| `ppl7-audio.h` | `pplib-audio.h` bzw. `pplib/audio/id3tag.h`, `audioinfo.h`, … |
| `ppl7-inet.h` | `pplib-inet.h` bzw. `pplib/inet/httpclient.h` |

`pplib.h` → `pplib/core.h` → zieht `pplib/types.h`, Exceptions, File/Dir, Logger, Regex, Time
automatisch nach. Ein 1:1-Tausch von `ppl7.h` gegen `pplib.h` funktioniert also; die
spezifischen Header sind danach als Feinschliff empfohlen.

## 3. Verwendete ppl7-Symbole → pplib8-Header

Häufigkeit über `wmlib/` + `gui/` zusammen. Alle Symbole existieren in pplib8, sofern nicht
vermerkt.

| Symbol | Anzahl | pplib8-Header |
|---|---:|---|
| `String` | 1191 | `pplib/types/string.h` |
| `RegEx` | 145 | `pplib/core/regex.h` |
| `File` | 143 | `pplib/core/file.h` |
| `Trim` | 124 | `pplib/core/stringfunctions.h` |
| `ToString` | 83 | `pplib/core/stringfunctions.h` |
| `Exception` | 77 | `pplib/core/baseexception.h` |
| `Logger` | 66 | `pplib/core/logging.h` |
| `PrintDebug` | 56 | `pplib/core/functions.h` |
| `Peek32` | 53 | `pplib/core/functions.h` |
| `EscapeHTMLTags` | 51 | `pplib/core/stringfunctions.h` |
| `WideString` | 50 | `pplib/types/widestring.h` |
| `UnescapeHTMLTags` | 47 | `pplib/core/stringfunctions.h` |
| `Array` | 44 | `pplib/types/array.h` |
| `Poke32` | 42 | `pplib/core/functions.h` |
| `ByteArray` | 42 | `pplib/types/bytearray.h` |
| `Dir` | 40 | `pplib/core/dir.h` |
| `ByteArrayPtr` | 37 | `pplib/types/bytearrayptr.h` |
| `DateTime` | 33 | `pplib/types/datetime.h` |
| `ID3Tag` | 32 | `pplib/audio/id3tag.h` |
| `Poke8` | 25 | `pplib/core/functions.h` |
| `CDDB` | 24 | deklariert in `pplib-audio.h`, **ohne Implementierung** (nur GUI) |
| `Peek8` | 23 | `pplib/core/functions.h` |
| `LowerCase` | 23 | `pplib/core/stringfunctions.h` |
| `DirEntry` | 21 | `pplib/core/dir.h` |
| `Peek16` / `Poke16` | 18 / 17 | `pplib/core/functions.h` |
| `AssocArray` | 14 | `pplib/types/assocarray.h` |
| `UrlEncode` | 9 | `pplib/core/stringfunctions.h` |
| `GetTime` | 6 | `pplib/core/time.h` |
| `AudioCD` | 6 | deklariert in `pplib-audio.h`, **ohne Implementierung** (nur GUI) |
| `Json` | 5 | `pplib/core/json.h` |
| `OutOfMemoryException` | 4 | `pplib/exceptions.h` |
| `HttpResponse` / `HttpClient` / `HttpRequest` | 4/4/2 | `pplib/inet/httpclient.h` |
| `Mutex` | 3 | `pplib/core/mutex.h` |
| `IsTrue` | 3 | `pplib/core/stringfunctions.h` |
| `HexDump` | 3 | `pplib/core/functions.h` |
| `UpperCase` | 2 | `pplib/core/stringfunctions.h` |
| `rand` | 2 | `pplib/core/random.h` |
| `ppl_time_t`, `MkTime`, `MkDate`, `USleep` | 2/2/2/1 | `pplib/core/time.h` |
| `ConfigParser` | 2 | `pplib/core/configparser.h` |
| `AudioInfo`, `IdentAudioFile` | 2/1 | `pplib/audio/audioinfo.h` |
| `GetID3GenreName` | 2 | `pplib/audio/id3tag.h` |
| `Thread` | 1 | `pplib/core/thread.h` |
| `PokeFloat`, `PeekFloat`, `PeekN32`, `Peek64` | je 1 | `pplib/core/functions.h` |
| `OverflowException`, `InvalidArgumentsException` | je 1 | `pplib/exceptions.h` |
| `ID3Frame` | 1 | `pplib/audio/id3tag.h` |
| `Iconv` | 1 | `pplib/core/iconv.h` |
| `HaveArgv`, `GetArgv` | je 2 | `pplib/core/args.h` |
| `InvalidGenreException` | 1 | **fehlt in pplib8** (nur GUI: `properties.cpp`) |

## 4. API-bruch-Check nach Klassen

### File / FileObject — kompatibel bis auf zwei Punkte
- `read(buf, bytes, fileposition)` / `write(buf, bytes, fileposition)` existieren weiterhin in
  `pplib::FileObject` → `wmlib/src/CWMFile.cpp` ist übertragbar.
- **`File::FileMode` ist jetzt `enum class`** (innerhalb von `File` verschachtelt):
  `ppl7::File::WRITE` → `pplib::File::FileMode::WRITE`
  (20 Fundstellen: 7 in wmlib, 13 in gui).
- **`map()`-Signatur geändert**: alt `const char* map(uint64_t, size_t)`,
  neu `char* map(uint64_t position, size_t size, MapProtection prot = MapProtection::READ)`.
  Aufrufe in `CWMFile.cpp` (6×) und `gui/src/wmfunctions.cpp` (1×) funktionieren unverändert,
  überflüssige `(const char*)`-Casts können entfallen.
- `File::exists/copy/remove/unlink/load/save` unverändert.

### Dir / DirEntry — nur GUI betroffen
- pplib8-`Dir` ist ein Container (`begin()/end()`, `filterPattern()`, `findPattern()`,
  `operator[]`). Das alte Iterations-API `reset(it)` / `getNext(entry, it)` existiert nicht mehr.
- Fundstellen ausschließlich in gui: `searchlists.cpp` (2), `search.cpp` (1),
  `renumberdialog.cpp` (2), `edit.cpp` (4), `CWmClient.cpp` (2).
- `Dir::mkDir(path, recursive)` statisch existiert unverändert (wird in wmlib-Tests genutzt).

### String — weitgehend identisch
- Entfernt: `getGlobalEncoding/setGlobalEncoding`, `stripSlashes`, `toUCS4/fromUCS4`,
  `toUpperCaseWords`, `toUtf8`. Genutzt in WinMusik nur:
  `.upperCaseWords()` 2× (gui/editor/edit.cpp) und `.toUtf8()` 1× live
  (gui/wmfunctions.cpp:531) + 1× im Makro gui/include/exceptions.h — beides GUI, später mit
  `WideString`-Äquivalent bzw. Qt-Functions lösen.
- Neu in pplib8: `contains()`, `isDigits()`, `string_view`-Konvertierung.

### AssocArray — Iterations-API ersetzt
- Entfernt: `getFirst/getLast/getNext/getPrevious`, `key()/value()/reset()`,
  `AssocArray::Iterator/ReverseIterator`, `fromConfig/fromTemplate/toTemplate`, `ArrayKey`, `compare`.
- pplib8 bietet `begin()/end()/rbegin()/rend()` (STL-Iteratoren).
- wmlib nutzt nur Zugriffsfunktionen (`getAssocArray`, `set`, `get`) → unkritisch.
  GUI nutzt keine AssocArray-Iteratoren → unkritisch.

### ByteArrayPtr
- `truncate()` entfernt. WinMusik nutzt es nicht.

### DateTime
- `print()` entfernt. WinMusik nutzt es nicht.

### Logger
- Signaturen identisch, inkl. `printf(PRIORITY, level, module, function, file, line, fmt, ...)`
  und `hexDump()` → 1:1 übertragbar.

### RegEx
- `match/capture/replace/compile/escape` unverändert. Die Timer-Helfer
  (`start/stop/duration/currentDuration`) sind intern entfallen, WinMusik nutzt sie nicht.

### Exceptions
- Makro heißt jetzt `PPLIBEXCEPTION(name, inherit)` statt `PPL7EXCEPTION`
  → `wmlib/include/wm_exceptions.h` (16 eigene Exception-Klassen) und
  `gui/include/exceptions.h` müssen das Makro umbenennen.
- `InvalidGenreException` (pplib7: deklariert in `ppl7-audio.h`) fehlt in pplib8 — nur
  `gui/src/properties/properties.cpp` nutzt sie; dort lokal definieren oder pplib8 nachrüsten.

### CDDB / AudioCD
- In pplib8 nur deklariert, **nicht implementiert** (kein `src/audio/cddb.cpp` o. ä.).
- Nutzer sind ausschließlich GUI-Dateien (`cddbimport.*`, `cddbselect.*`, `edit*.cpp`,
  `properties.cpp`). Für wmlib irrelevant; für die GUI später entscheiden: pplib8-Implementierung
  nachliefern oder Klasse in WinMusik ausgliedern.

## 5. Betroffene Dateien

### wmlib (25 Dateien + 14 Testdateien)

Header (10): `wm_coverdownload.h`, `wm_cwmfile.h`, `wm_exceptions.h`, `wm_functions.h`,
`wm_musicalkeys.h`, `wm_normalizer.h`, `wm_playlist.h`, `wm_regexpcapture.h`,
`wm_searchlist.h`, `wm_storage.h`, `wm_tracklist.h`, `wm_traktor.h`
(`wm_id3tagsaver.h` bereits fertig)

Quellen (15): `beatport.cpp`, `coverdownload.cpp`, `CStorage.cpp`, `CWMFile.cpp`,
`functions.cpp`, `MusicalKeys.cpp`, `normalizer.cpp`, `Playlist.cpp`, `regexpcapture.cpp`,
`StorageDevice.cpp`, `StorageShortcut.cpp`, `StorageSimpleTable.cpp`, `StorageTitle.cpp`,
`StorageTrack.cpp`, `traktor.cpp`

Tests (14): `main.cpp`, `cwmfile.cpp`, `id3tagsaver.cpp`, `playlist.cpp`, `regexpcapture.cpp`,
`storage*.cpp` (7), `test_coverdownload.cpp`, `traktor.cpp`

### gui (ca. 60 Dateien)
Für die erste Phase zurückgestellt. Besondere Brocken: `Dir`-Iteration (6 Dateien),
`File::WRITE` (13 Stellen), `CDDB`/`AudioCD` (7 Dateien), `InvalidGenreException`,
`String::upperCaseWords/toUtf8`.

## 6. Vorgehen

1. **Globaler Sweep** (ein Rutsch, keine Logikänderung):
   - `ppl7::` → `pplib::` in `wmlib/` und `gui/` (1294 Fundstellen in wmlib, Rest gui)
   - `PPL7EXCEPTION` → `PPLIBEXCEPTION`
   - `#include <ppl7.h>` → `<pplib.h>`, `<ppl7-audio.h>` → `<pplib-audio.h>`,
     `<ppl7-inet.h>` → `<pplib-inet.h>`, `"ppl7.h"` → `<pplib.h>`
2. **wmlib Batch für Batch übersetzen** (`make wmlib_tests`), Reihenfolge nach Kuppelung
   Header+Quelle, da Signaturen sonst gemischte Typen mischen:
   - Batch 1: `wm_exceptions.h`, `wm_functions.h`+`functions.cpp`, `wm_musicalkeys.h`+`MusicalKeys.cpp`
   - Batch 2: `wm_cwmfile.h`+`CWMFile.cpp` (FileMode/map-Bruch), `wm_tracklist.h`
   - Batch 3: `wm_storage.h` + `CStorage.cpp`, `StorageDevice.cpp`, `StorageShortcut.cpp`,
     `StorageSimpleTable.cpp`, `StorageTitle.cpp`, `StorageTrack.cpp`
   - Batch 4: `wm_normalizer.h`+`normalizer.cpp`, `wm_searchlist.h`, `wm_playlist.h`+`Playlist.cpp`
   - Batch 5: `wm_regexpcapture.h`+`regexpcapture.cpp`, `beatport.cpp`,
     `wm_traktor.h`+`traktor.cpp`, `wm_coverdownload.h`+`coverdownload.cpp`
   - Batch 6: Tests (`wmlib/tests/src/*.cpp`)
3. **Tests grün** (`make wmlib_tests`, ggf. `--gtest_filter`), dann Includes ausdünnen
   (Aggregate-Header durch spezifische ersetzen).
4. **GUI** in eigenen Schritten; vorher CDDB/AudioCD-Entscheidung.

## 7. Bekannte Fallstricke

- Nach Sweep, aber vor Include-Tausch, fehlt weiterhin `ppl7.h` → erst ab Batch 1 bauen.
- `pplib::String` hat kein `toUtf8()`; in wmlib nicht genutzt, in gui schon.
- `enum class FileMode` akzeptiert keine impliziten Vergleiche/Integer. Schreibweise ist
  `pplib::File::FileMode::WRITE` (FileMode ist in `File` verschachtelt, nicht auf Namespace-Ebene).
- `DirEntry`-Felder (`Filename`, `Path`, `File`, `Size`) sind unverändert öffentliche Member.
- pplib8-`String::toInt()`-Familie unverändert; `int64_t`/`uint64_t`-Overloads neu.
- pplib8-Header ziehen keine C-Standard-Header mehr (`ppl7.h` tat das). Quellen, die
  `memcpy`/`strlen`/`strcmp` nutzen, brauchen jetzt explizit `<string.h>`.
- `AssocArray::get(key)` liefert `const Variant&` (nicht `String&`) → `.toString()` nötig.
- pplib8-JSON-Parser legt Zahlen als `Int64`-Variant an, pplib7 speicherte sie als String.
  `getString()` auf ein Zahlenfeld wirft deshalb `TypeConversionException` → `getInt()`
  verwenden (betrifft `coverdownload.cpp`: `track_length_ms`, `length_ms`, `bpm`).
- `WideString(const String&)` ist `explicit` → `WideString s = term;` kompiliert nicht,
  `WideString s(term);` schon (`normalizer.cpp`).
- `<pplib-audio.h>` ist derzeit nicht einbindbar: die deklarierte `CDDB`-Klasse referenziert
  einen Typ `pplib::Curl`, den es in pplib8 nicht gibt. wmlib braucht nur `ID3Tag`, daher
  dort `<pplib/audio/id3tag.h>` statt des Aggregat-Headers. Betrifft die GUI erst, wenn
  CDDB/AudioCD umgesetzt wird.
- Der Sweep hat `#include <ppl7.h>` übersehen (nur `ppl7-*.h` wurden getauscht). Neun GUI-
  Dateien include-n weiterhin `ppl7.h`: `include/winmusik3.h`, `include/coverdownloadmanager.h`,
  `include/csearchlist.h`, `include/musickey.h`, `include/resultfilter.h`,
  `src/editor/edit-functions.cpp`, `src/playlist/playlistprint.cpp`, `src/regexpcapture.cpp`,
  `widgets/wmcoverwidget.cpp`. *(erledigt — `grep -rn ppl7 gui/` ist leer)*
- `Dir`-Iteration: die ppl7-Zugriffe `Iterator`, `reset()`, `getNext()`, `getNextPattern()`,
  `getNextRegExp()`, `getFirstRegExp()` und die `SORT_*`-Konstanten gibt es in pplib8 nicht.
  Muster: `for (const pplib::DirEntry& entry : Dir)` bzw. `Dir.filterPattern(Pattern, true)`,
  `Dir.filterRegExp(Pattern)`, `Dir.findRegExp(Pattern)` (`std::optional<DirEntry>`),
  `Dir.num()` → `Dir.size()` (bei `printf` Format `%zu`), Sortierung als
  `Dir::Sort::Filename` / `FilenameIgnoreCase` / `None`. Betrifft in der GUI: `CWmClient.cpp`,
  `editor/edit.cpp`, `editor/massimport.cpp`, `editor/renumberdialog.cpp`, `search/search.cpp`,
  `searchlists/searchlists.cpp`.
- `pplib::String::toWideString()` war in pplib8 deklariert, aber nicht implementiert →
  Linker-Fehler. Inzwischen in pplib8 implementiert (`src/types/String.cpp`); die GUI nutzt
  wieder `Tmp.toWideString()` (`src/CWmClient.cpp`).
- `pplib::InvalidGenreException` gibt es in pplib8 nicht; `pplib::GetID3GenreName(int)` liefert
  für eine ungültige ID einen leeren String. In `firststart/InitialDatabase.cpp` ist der
  try/catch-Block deshalb entfallen, die Abbruchbedingung `genre.isEmpty()` bleibt.
- Das `WMEXCEPTION`-Makro in `gui/include/exceptions.h` musste an die pplib8-Basis angepasst
  werden: `copyText(...)` → `initFromFormat(const char*, va_list)` (protected), und die
  Klassenname-Ausgabe läuft nicht mehr über ein `what()`-Override mit `STR_VALUE(name)`,
  sondern über `virtual const char* className() const noexcept`.
- `RegEx` akzeptiert in pplib8 weiterhin die ppl7-Syntax `/muster/flags` (die Implementierung
  schneidet die Flags hinter dem letzten Slash ab), alle bestehenden GUI-Pattern bleiben gültig.
  Achtung bei `Dir::filterRegExp()`: dort gilt dieselbe Slash-Syntax, ein Pattern wie
  `/^([0-9]{3}_.*$/` (in `edit.cpp` fehlerhaft) führt zu Fehlinterpretation der Flags.

## 8. GUI-Build mit CMake

`gui/WinMusik.pro` (qmake) ist durch `gui/CMakeLists.txt` ersetzt. Der qmake-Pfad ist
vollständig entfernt: `gui/WinMusik.pro.in` und `gui/WinMusik.pro.header` sind gelöscht,
`AC_CONFIG_FILES([gui/WinMusik.pro])` ist aus `configure.ac` herausgenommen (der eingecheckte
`configure` wurde mit autoconf 2.72 neu erzeugt), und die qmake-Suche samt Target `qmake-info`
ist aus dem Root-`Makefile` entfernt. `configure` prüft Qt weiterhin nur noch für die
Ausgabeumgebung (Report), der Bau läuft über CMake.

Build (Root-`Makefile`, alles über den Root-CMake-Superbuild `cmake -S . -B build/<baum>`):

Der Grund: `pplib8` exportiert sein CMake-Paket ausschließlich in den Installationsprefix
(`install(EXPORT pplib8Targets …)` in `pplib8/CMakeLists.txt`), es gibt keine Build-Baum-Config.
Ein Solo-`cmake -S wmlib` kann `find_package(pplib8)` daher nicht gegen einen Build-Baum
auflösen. Alle Bäume werden aus dem Root-`CMakeLists.txt` erzeugt, das `pplib8` und `wmlib`
per `add_subdirectory()` einbindet und die `PPLIB_ENABLE_*`-Flags setzt.

- `make configure` → konfiguriert `build/release` (`-DWINMUSIK_BUILD_GUI=ON`) und `build/debug`
- `make release` / `make debug` → baut pplib8 + wmlib + `WinMusik` in einem Durchlauf,
  Kopie nach `release/WinMusik` bzw. `debug/WinMusik`
- `make pplib8_release` / `pplib8_debug` → Teilbau nur der Bibliotheks-Libs
  (`pplib_core`, `pplib_audio`, `pplib_internet`); pplib8-eigene Tests laufen über die
  Toolchain des Submoduls (`make -C pplib8 test`)
- `make wmlib_release` / `wmlib_debug` → Teilbau nur `libwinmusik4.a`
- `make wmlib_tests` → eigener Baum `build/wmlib-tests`
  (`-DWINMUSIK_BUILD_GUI=OFF -DWINMUSIK_BUILD_TESTS=ON`), baut `wmlib_test` und führt `ctest`
  aus. `enable_testing()` dazu muss im Root-`CMakeLists.txt` stehen, sonst findet ctest im
  Superbuild keine Tests. Der Test läuft mit `WORKING_DIRECTORY wmlib/tests` (relative
  Testdatenpfade) — der Binary direkt aus dem Repo-Root zu starten gibt 20 Fehltests.
- `make wmlib_coverage` → analog mit `-DWMLIB_ENABLE_COVERAGE=ON`, danach gcovr. Die
  Instrumentierung der Bibliotheksquellen steckt jetzt in `wmlib/CMakeLists.txt` selbst
  (`--coverage` am Target `wmlib`); `wmlib/tests/CMakeLists.txt` instrumentiert nur die
  Test-Quellen. Ohne die Target-Flags meldet gcovr „All coverage data is filtered out",
  weil der Filter `wmlib/src` ins Leere läuft.
- `make translations` → `lupdate`/`lrelease` über die eingecheckten `.ts`-Dateien. Das
  Qt6LinguistTools-CMake-Paket fehlt auf manchen Distributionen (Fedora: `qt6-linguist`
  ohne `Qt6LinguistToolsConfig.cmake`; die CMake-Dateien stecken im `-devel`-Paket). In dem
  Fall sucht `gui/CMakeLists.txt` die Programme direkt über `find_program` (Namen
  `lupdate-qt6`/`lrelease-qt6`); erst wenn auch das fehlschlägt, entfällt das Target.
  Achtung: `lupdate -no-obsolete` überschreibt die eingecheckten `.ts`/`.qm` — nach dem
  CDDB-Rauswurf wurden dabei 771 veraltete Einträge entfernt.

Wichtige Punkte der neuen Konfiguration:

- Qt-Module: `Core Gui Widgets Xml Network PrintSupport` (`Xml` wegen `QDomDocument` in
  `editor/edit.cpp` und den Searchlists, `PrintSupport` wegen `coverprinter`/`playlistprint`).
- Quellen-Globs sind `GLOB_RECURSE` über `src/`, `widgets/` und `include/`. Mit
  `GLOB` allein (`src/*.cpp`) wären alle Unterverzeichnisse (`src/editor/`, `src/search/`, …)
  nicht Teil des Targets — das äußert sich ausschließlich als lange Liste von Linker-Fehlern.
- `AUTOUIC` ist abgeschaltet. CMake erzeugt `ui_*.h` nur, wenn die Quelldatei das Header-
  Include direkt enthält und die `.ui` in ihrem Verzeichnis (oder in `AUTOUIC_SEARCH_PATHS`)
  liegt; die GUI bindet die generierten Header aber aus anderen Verzeichnissen ein
  (`#include "ui_edit.h"` in `src/editor/edit.cpp`, `.ui` liegt in `include/`). Stattdessen
  läuft eine eigene Regel: alle 33 `.ui`-Dateien werden mit `Qt6::uic` in
  `${CMAKE_CURRENT_BINARY_DIR}/ui` erzeugt (Target `WinMusik_uic`, Verzeichnis vorab mit
  `cmake -E make_directory` angelegt, weil `uic` keine Verzeichnisse anlegt) und dieses
  Verzeichnis liegt im Include-Pfad des Targets.
- `AUTOMOC` kompiliert alle moctierten Header in `mocs_compilation.cpp` gemeinsam in einer
  TU. Duplizierte Include-Guards, die im normalen Build harmlos sind, werden dort tödlich.
  Gefunden: `src/editor/tablesearch.h` und `src/search/search.h` nutzten beide `SEARCH_H`;
  `tablesearch.h` verwendet jetzt `TABLESEARCH_H`.
- Die Makros `WITH_QT`, `PPL_WITH_QT6` und `PPL_QT_STRING_UTF8` kommen jetzt ausschließlich
  aus `target_compile_definitions`. Die handgeschriebenen `#define`s in den Quellen sind
  entfernt bzw. `#ifndef`-gekapselt (`winmusik3.h`, `coverdownloadmanager.h`); ohne das
  entstehen Warnungen wegen Makro-Redefinition.
- Include-Verzeichnisse: `gui/` (selbst, wegen `#include "src/menue/menue.h"`), `gui/include`,
  `gui/widgets`. Die alten Pfade `../pplib/include` (pplib7) und `../lib/include` sind entfallen.
- `target_compile_definitions(... PPL_WITH_QT6 PPL_QT_STRING_UTF8)`: erst diese Makros
  aktivieren in `pplib/types/string.h` und `widestring.h` die QString-/QVariant-Konstruktoren
  und -Operatoren. `PPL_WITH_QT6` definiert `WITH_QT` dabei selbst; `PPL_QT_STRING_UTF8`
  stellt die Konvertierung von `QString` ↔ `WideString` auf UTF-8 um (statt `toLocal8Bit()`),
  was zum UTF-8-Encoding aller WinMusik-Strings passt. Es sind ausschließlich Inline-Funktionen,
  die Datendarstellung von `String`/`WideString` ändert sich nicht — die Definition nur im
  GUI-Target ist damit ABI-verträglich. **Diese Ifdefs sind damit erstmals real übersetzt und
  müssen beim GUI-Test besonders beobachtet werden.**
- Externe Bibliotheken (`z`, `bz2`, `pcre2`, `ssl`, `crypto`, `curl`) werden über die
  pplib8-Targets propagiert und sind nicht mehr separat zu nennen. `-lcdio` ist entfallen
  (pplib8 kennt cdio nicht; nach CDDB/AudioCD-Rauswurf braucht die GUI es nicht mehr).
- `resource.rc` (Icon) und `winmm` sind nur unter `WIN32` aktiv, `add_executable(... WIN32)`
  erzeugt wie bisher eine Windows-Anwendung ohne Konsole.
- Die `.qm`-Dateien bleiben eingecheckt und werden über `resources.qrc` (Präfix `/translation`)
  eingebettet; CMake erzeugt sie nicht automatisch.
- Toter Ballast ist entfernt: `gui/src/player/player.{h,ui}` und
  `gui/src/exporttracks/exporttracks.ui` gehörten zu keiner übersetzten Quelle.
- CDDB/AudioCD ist aus der GUI ausgebaut (`editor/cddbimport.{h,cpp,ui}`,
  `editor/cddbselect.{h,cpp,ui}` gelöscht, F5-Handler und zugehörige Includes in
  `editor/edit.{h,cpp}` entfernt). In pplib8 entfällt die CDDB-Klasse ebenfalls.

## 9. Autoconf und pplib7: entfallen

`configure.ac`, `configure`, `autoconf/` und das Submodul `pplib` (pplib7) sind entfernt.
Gebaut wird ausschließlich über das Root-`CMakeLists.txt` (Superbuild aus `pplib8`,
`wmlib`, `gui`) mit dem Root-`Makefile` als Wrapper. Die Umgebungsprüfung aus dem
`configure`-Skript (Typgrößen von `int`/`long`/`wchar_t`, Qt/PCRE2/pthreads) existiert
nicht mehr; falls diese Informationen wieder geprüft werden sollen, müssten sie nach
CMake wandern.