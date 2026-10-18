###############################################################################
# This file is part of WinMusik 4 by Patrick Fedick                           #
# Web: https://www.pfp.de/winmusik/                                           #
# Sourcecode: https://github.com/pfedick/winmusik
###############################################################################
#
# Copyright (c) 2026, Patrick Fedick <patrick@pfp.de>
# All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
###############################################################################

cmake_minimum_required(VERSION 3.20)
project(WinMusikGUI VERSION 4.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)              # gnu++20, wie im Root-CMakeLists

# Qt-Codegenerierung: moc / rcc automatisch, uic explizit (siehe unten)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Xml Network PrintSupport)
# LinguistTools ist optional: wird nur fuer das "translations"-Target benoetigt.
# Falls das CMake-Paket fehlt, werden lupdate/lrelease spaeter direkt gesucht.
find_package(Qt6 COMPONENTS LinguistTools QUIET)

# wmlib ist im Monorepo bereits vorhanden, wenn das Projekt ueber das
# Root-CMakeLists (WINMUSIK_BUILD_GUI=ON) konfiguriert wird. Beim direkten
# Konfigurieren von gui/ einkaufen.
if(NOT TARGET wmlib)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../wmlib ${CMAKE_BINARY_DIR}/wmlib)
endif()

file(GLOB_RECURSE GUI_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "widgets/*.cpp")
file(GLOB_RECURSE GUI_HEADERS CONFIGURE_DEPENDS "src/*.h" "widgets/*.h" "include/*.h")
set(GUI_RESOURCES "resources.qrc")

# Die ui_-Header werden nicht von AUTOUIC erzeugt: AUTOUIC wird pro
# Uebersetzungseinheit aktiv und findet eine .ui-Datei nur, wenn sie im
# Verzeichnis der Einheit liegt. WinMusik-Header binden ihren ui_-Header aber
# aus anderen Verzeichnissen ein (z.B. CWmClient.cpp -> src/menue/menue.h,
# oder mocs_compilation.cpp). Deshalb erzeugt uic alle Header zentral in ein
# Verzeichnis, das fuer alle Uebersetzungseinheiten im Include-Pfad liegt.
file(GLOB_RECURSE GUI_UI_FILES CONFIGURE_DEPENDS "src/*.ui" "widgets/*.ui" "include/*.ui")
set(GUI_UI_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/ui")
file(MAKE_DIRECTORY "${GUI_UI_GENERATED_DIR}")
set(GUI_UI_GENERATED_HEADERS "")
foreach(ui_file IN LISTS GUI_UI_FILES)
    get_filename_component(ui_name "${ui_file}" NAME_WE)
    set(ui_header "${GUI_UI_GENERATED_DIR}/ui_${ui_name}.h")
    add_custom_command(
        OUTPUT "${ui_header}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${GUI_UI_GENERATED_DIR}"
        COMMAND Qt6::uic "${ui_file}" -o "${ui_header}"
        MAIN_DEPENDENCY "${ui_file}"
        COMMENT "uic ${ui_name}.ui")
    list(APPEND GUI_UI_GENERATED_HEADERS "${ui_header}")
endforeach()
add_custom_target(WinMusik_uic DEPENDS ${GUI_UI_GENERATED_HEADERS})

if(WIN32)
    add_executable(WinMusik WIN32 ${GUI_SOURCES} ${GUI_HEADERS} ${GUI_RESOURCES} resource.rc)
else()
    add_executable(WinMusik ${GUI_SOURCES} ${GUI_HEADERS} ${GUI_RESOURCES})
endif()
add_dependencies(WinMusik WinMusik_uic)

# "src/menue/menue.h" wird aus anderen Unterverzeichnissen mit diesem Pfad
# eingebunden, daher muss gui/ selbst Teil des Include-Pfades sein.
target_include_directories(WinMusik PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
    ${GUI_UI_GENERATED_DIR})

# pplib8 schaltet die QString-/QVariant-Konstruktoren und Konvertierungs-
# operatoren von pplib::String und pplib::WideString erst ueber diese Makros
# frei. PPL_WITH_QT6 definiert WITH_QT in den Headern selbst.
# PPL_QT_STRING_UTF8: QString <-> WideString ueber UTF-8 statt Local8Bit,
# passend zum UTF-8-Encoding aller WinMusik-Strings.
target_compile_definitions(WinMusik PRIVATE PPL_WITH_QT6 PPL_QT_STRING_UTF8)

# Die externen Abhaengigkeiten von pplib8 (z, bz2, pcre2, ssl, curl, ...)
# werden ueber die pplib-Targets propagiert und sind hier nicht zu nennen.
target_link_libraries(WinMusik PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Xml
    Qt6::Network
    Qt6::PrintSupport
    wmlib)

if(WIN32)
    target_link_libraries(WinMusik PRIVATE winmm)
endif()

set_target_properties(WinMusik PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# Uebersetzungen: .qm Dateien sind eingecheckt und werden ueber
# resources.qrc (Praefix /translation) eingebettet. Dieses Target erzeugt
# sie bei Bedarf neu (entspricht dem bisherigen lupdate/lrelease-Ablauf).
# Die Qt6LinguistTools-CMake-Pakete fehlen auf manchen Distributionen (z.B.
# Fedora: qt6-linguist ohne Qt6LinguistToolsConfig.cmake). Dann die Programme
# direkt suchen; die Pakete heissen dort oft lupdate-qt6 / lrelease-qt6.
if(TARGET Qt6::lupdate AND TARGET Qt6::lrelease)
    set(LUPDATE_EXECUTABLE $<TARGET_FILE:Qt6::lupdate>)
    set(LRELEASE_EXECUTABLE $<TARGET_FILE:Qt6::lrelease>)
else()
    find_program(LUPDATE_EXECUTABLE NAMES lupdate-qt6 lupdate)
    find_program(LRELEASE_EXECUTABLE NAMES lrelease-qt6 lrelease)
endif()

if(LUPDATE_EXECUTABLE AND LRELEASE_EXECUTABLE)
    add_custom_target(translations
        COMMAND ${LUPDATE_EXECUTABLE} -no-obsolete
                ${GUI_SOURCES} ${GUI_HEADERS}
                -ts ${CMAKE_CURRENT_SOURCE_DIR}/winmusik_de.ts ${CMAKE_CURRENT_SOURCE_DIR}/winmusik_en.ts
        COMMAND ${LRELEASE_EXECUTABLE}
                ${CMAKE_CURRENT_SOURCE_DIR}/winmusik_de.ts ${CMAKE_CURRENT_SOURCE_DIR}/winmusik_en.ts
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Uebersetzungen (.ts/.qm) aktualisieren"
        VERBATIM)
else()
    message(STATUS "lupdate/lrelease nicht gefunden - Target 'translations' nicht verfuegbar")
endif()
