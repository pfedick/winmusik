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

# Qt-Codegenerierung: moc / uic / rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Xml Network PrintSupport)
# LinguistTools ist optional: wird nur fuer das "translations"-Target benoetigt
# und fehlt auf manchen Systemen (z.B. ohne Qt6-Entwicklungspakete).
find_package(Qt6 COMPONENTS LinguistTools QUIET)
if(NOT TARGET Qt6::lupdate)
    message(STATUS "Qt6 LinguistTools nicht gefunden - Target 'translations' nicht verfuegbar")
endif()

# wmlib ist im Monorepo bereits vorhanden, wenn das Projekt ueber das
# Root-CMakeLists (WINMUSIK_BUILD_GUI=ON) konfiguriert wird. Beim direkten
# Konfigurieren von gui/ einkaufen.
if(NOT TARGET wmlib)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../wmlib ${CMAKE_BINARY_DIR}/wmlib)
endif()

file(GLOB GUI_SOURCES   CONFIGURE_DEPENDS "src/*.cpp" "widgets/*.cpp")
file(GLOB GUI_HEADERS   CONFIGURE_DEPENDS "src/*.h" "widgets/*.h" "include/*.h")
set(GUI_RESOURCES "resources.qrc")

if(WIN32)
    add_executable(WinMusik WIN32 ${GUI_SOURCES} ${GUI_HEADERS} ${GUI_RESOURCES} resource.rc)
else()
    add_executable(WinMusik ${GUI_SOURCES} ${GUI_HEADERS} ${GUI_RESOURCES})
endif()

# "src/menue/menue.h" wird aus anderen Unterverzeichnissen mit diesem Pfad
# eingebunden, daher muss gui/ selbst Teil des Include-Pfades sein.
target_include_directories(WinMusik PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets)

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
if(TARGET Qt6::lupdate AND TARGET Qt6::lrelease)
    add_custom_target(translations
        COMMAND $<TARGET_FILE:Qt6::lupdate> -no-obsolete
                ${GUI_SOURCES} ${GUI_HEADERS}
                -ts ${CMAKE_CURRENT_SOURCE_DIR}/winmusik_de.ts ${CMAKE_CURRENT_SOURCE_DIR}/winmusik_en.ts
        COMMAND $<TARGET_FILE:Qt6::lrelease>
                ${CMAKE_CURRENT_SOURCE_DIR}/winmusik_de.ts ${CMAKE_CURRENT_SOURCE_DIR}/winmusik_en.ts
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Uebersetzungen (.ts/.qm) aktualisieren"
        VERBATIM)
endif()
