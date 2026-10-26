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


###############################################################################
### Global Options
###############################################################################
VERSION = 4.0.0
PREVIOUS_TAG = REL_3_7_2
BUILD_NUMBER := $(shell git rev-list --all --count)
BUILDDATE := $(shell date '+%Y%m%d')

# Executable file extension based on OS
ifeq ($(OS),Windows_NT)
EXE = .exe
else
EXE =
endif

### CMake Superbuild (Root-CMakeLists: pplib8 + wmlib + GUI)
CMAKE ?= cmake
CTEST ?= ctest

BUILD_RELEASE = build/release
BUILD_DEBUG   = build/debug
BUILD_TESTS   = build/wmlib-tests
BUILD_COV     = build/wmlib-coverage

CMAKE_REL = -DCMAKE_BUILD_TYPE=Release -DWINMUSIK_BUILD_GUI=ON
CMAKE_DBG = -DCMAKE_BUILD_TYPE=Debug   -DWINMUSIK_BUILD_GUI=ON
CMAKE_TST = -DCMAKE_BUILD_TYPE=Debug   -DWINMUSIK_BUILD_GUI=OFF -DWINMUSIK_BUILD_TESTS=ON
CMAKE_COV = -DCMAKE_BUILD_TYPE=Debug   -DWINMUSIK_BUILD_GUI=OFF -DWINMUSIK_BUILD_TESTS=ON -DWMLIB_ENABLE_COVERAGE=ON

###########################################################################
### Targets
###########################################################################


.PHONY: gui release debug all wmlib update_version clean distclean history \
	configure pplib8_configure wmlib_configure pplib8 pplib8_debug pplib8_release \
	wmlib_release wmlib_debug wmlib_tests wmlib_coverage \
	gui_release gui_debug test translations


release: pplib8_release wmlib_release gui_release
	$(CMAKE) -S . -B $(BUILD_RELEASE) $(CMAKE_REL)
	ln -sf build/release/compile_commands.json compile_commands.json
	$(CMAKE) --build $(BUILD_RELEASE) --target translations WinMusik -j
	mkdir -p release
	cp $(BUILD_RELEASE)/WinMusik$(EXE) release/WinMusik$(EXE)

debug: update_version
	$(CMAKE) -S . -B $(BUILD_DEBUG) $(CMAKE_DBG)
	ln -sf build/debug/compile_commands.json compile_commands.json
	$(CMAKE) --build $(BUILD_DEBUG) --target translations WinMusik -j
	mkdir -p debug
	cp $(BUILD_DEBUG)/WinMusik$(EXE) debug/WinMusik$(EXE)


all: release debug

wmlib: wmlib_release wmlib_debug

update_version:
	@mkdir -p tmp
	@echo "#ifndef VERSION_H_" > tmp/version.h
	@echo "#define VERSION_H_" >> tmp/version.h
	@echo "#define WM_VERSION \"$(VERSION)\"" >> tmp/version.h
	@echo "#define WM_REVISION \"$(BUILD_NUMBER)\"" >> tmp/version.h
	@echo "#define WM_RELEASEDATE $(BUILDDATE)" >> tmp/version.h
	@echo "#endif" >> tmp/version.h
	@diff tmp/version.h gui/include/version.h > /dev/null 2>&1; if [ $$? -ne 0 ] ; then cp tmp/version.h gui/include/version.h; fi

mingw: release
	rm -rf release/deploy
	windeployqt.exe --dir release/deploy --translations de,en release/WinMusik.exe
	ldd release/WinMusik.exe  | grep -v WINDOWS | grep ucrt64 | awk '{print $$3}' | while read line  ; do cp $${line} release/deploy; done
	ldd release/WinMusik.exe  | grep -v WINDOWS | grep mingw64 | awk '{print $$3}' | while read line  ; do cp $${line} release/deploy; done
	cp release/WinMusik.exe release/deploy
	strip release/deploy/WinMusik.exe

	cat Package/setup64.iss | sed "s/^AppVersion=.*/AppVersion=${VERSION}.${BUILD_NUMBER}/"  \
		| sed "s/^AppVerName=.*/AppVerName=WinMusik ${VERSION}/"  \
		| sed "s/^VersionInfoVersion=.*/VersionInfoVersion=${VERSION}/"  \
		| sed "s/^OutputBaseFilename=.*/OutputBaseFilename=WinMusik-${VERSION}-Setup-64bit/"  \
		> setup.iss
	
setup: mingw
	"C:\Program Files (x86)\Inno Setup 5\ISCC.exe" setup.iss



###########################################################################
### wmlib Tests & Coverage
###########################################################################
FILTER ?=
GTEST_ARGS = --gtest_color=yes $(if $(FILTER),--gtest_filter="$(FILTER)") $(ARGS)

wmlib_tests:
	$(CMAKE) -S . -B $(BUILD_TESTS) $(CMAKE_TST)
	$(CMAKE) --build $(BUILD_TESTS) --target wmlib_test -j
	cd wmlib/tests && ../../$(BUILD_TESTS)/wmlib/tests/wmlib_test$(EXE) $(GTEST_ARGS)

tests: wmlib_tests

wmlib_coverage:
	$(CMAKE) -S . -B $(BUILD_COV) $(CMAKE_COV)
	ln -sf $(BUILD_COV)/compile_commands.json compile_commands.json
	$(CMAKE) --build $(BUILD_COV) --target wmlib_test -j
	cd wmlib/tests && ../../$(BUILD_COV)/wmlib/tests/wmlib_test$(EXE) $(GTEST_ARGS)
	mkdir -p coverage_html
	gcovr --root . $(BUILD_COV) --filter wmlib/src --medium-threshold 70 \
		--exclude-throw-branches --html-details coverage_html/index.html --xml-pretty -o coverage.xml
	@echo "Report: coverage_html/index.html"


###########################################################################
### GUI
###########################################################################
gui_release: update_version
	$(CMAKE) -S . -B $(BUILD_RELEASE) $(CMAKE_REL)
	$(CMAKE) --build $(BUILD_RELEASE) --target WinMusik -j
	mkdir -p release
	cp $(BUILD_RELEASE)/WinMusik$(EXE) release/WinMusik$(EXE)

gui_debug: update_version
	$(CMAKE) -S . -B $(BUILD_DEBUG) $(CMAKE_DBG)
	$(CMAKE) --build $(BUILD_DEBUG) --target WinMusik -j
	mkdir -p debug
	cp $(BUILD_DEBUG)/WinMusik$(EXE) debug/WinMusik$(EXE)


###########################################################################
### Tests
###########################################################################
test: wmlib_tests


###########################################################################
### Uebersetzungen
###########################################################################
translations:
	$(CMAKE) -S . -B $(BUILD_RELEASE) $(CMAKE_REL)
	$(CMAKE) --build $(BUILD_RELEASE) --target translations


###########################################################################
### MISC
###########################################################################

clean:
	@rm -rf documentation release debug build coverage_html coverage.xml
	@rm -rf gui/release gui/debug release debug

distclean: clean
	@rm -rf .qmake.stash autom4te.cache
	@rm -rf config.log config.status
	@rm -rf Package/WinMusik.desktop

history:
	@echo "Git history between Tag $(PREVIOUS_TAG) and head" >  history.tmp
	@echo "================================================================================" >>  history.tmp
	@git log --pretty=format:"%ad=!=%s" --date=iso8601-strict $(PREVIOUS_TAG)...master | sort | awk -F "=!=" '{ print $$2}' >> history.tmp
	cat history.tmp
