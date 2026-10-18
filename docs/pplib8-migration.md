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
  `widgets/wmcoverwidget.cpp`.

## 8. GUI-Build mit CMake

`gui/WinMusik.pro` (qmake) ist durch `gui/CMakeLists.txt` ersetzt; die Qt-Codegenerierung
übernimmt CMake (`AUTOMOC`, `AUTOUIC`, `AUTORCC`). `gui/WinMusik.pro` wird von `configure`
aus `gui/WinMusik.pro.in` erzeugt — mit dem qmake-Pfad entfallen `AC_CONFIG_FILES([gui/WinMusik.pro])`
in `configure.ac` sowie `WinMusik.pro.in`/`WinMusik.pro.header`.

Build (Root-`Makefile`):

- `make gui_debug` → konfiguriert `build/gui-debug` mit `-DWINMUSIK_BUILD_GUI=ON`, baut
  `pplib8` + `wmlib` + `WinMusik` und kopiert nach `debug/WinMusik`
- `make gui_release` → analog in `build/gui-release`, Kopie nach `release/WinMusik`
- `make translations` → `lupdate`/`lrelease` über die eingecheckten `.ts`-Dateien (setzt
  Qt6 LinguistTools voraus; ohne dieses Paket meldet CMake nur eine Statusmeldung)

Wichtige Punkte der neuen Konfiguration:

- Qt-Module: `Core Gui Widgets Xml Network PrintSupport` (`Xml` wegen `QDomDocument` in
  `editor/edit.cpp` und den Searchlists, `PrintSupport` wegen `coverprinter`/`playlistprint`).
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
- Toter Ballast, der beim ersten sauberen Build auffällt: `gui/src/player/player.{h,ui}` und
  `gui/src/exporttracks/exporttracks.ui` gehören zu keiner übersetzten Quelle.
