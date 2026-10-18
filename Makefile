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

# Qt6-qmake finden: Name ist distroabhaengig, Version ist eindeutig
ifeq ($(OS),Windows_NT)
QMAKE ?= qmake6
else
QMAKE ?= $(shell for c in qmake6 qmake-qt6 qmake; do \
                v=$$($$c -query QT_VERSION 2>/dev/null | cut -d. -f1); \
                test "$$v" = 6 && { echo $$c; break; }; done)
ifeq ($(QMAKE),)
QMAKE = qmake6
endif
endif

# Executable file extension based on OS
ifeq ($(OS),Windows_NT)
EXE = .exe
else
EXE =
endif

### PPLIB8 CMake Flags
PPLIB8_CMAKE_FLAGS = \
	-DPPLIB_ENABLE_CORE=ON \
	-DPPLIB_ENABLE_AUDIO=ON \
	-DPPLIB_ENABLE_INTERNET=ON \
	-DPPLIB_ENABLE_CRYPTO=OFF \
	-DPPLIB_ENABLE_GRAFIX=OFF \
	-DPPLIB_ENABLE_DATABASE=OFF \
	-DPPLIB_ENABLE_PICO=OFF \
	-DPPLIB_BUILD_TESTS=ON

###########################################################################
### Targets
###########################################################################


.PHONY: gui release debug all ppl7 wmlib update_version clean distclean history qmake-info gui_release gui_debug wmlib_release wmlib_debug ppl7_release ppl7_debug configure


release: ppl7_release pplib8_release wmlib_release gui_release

debug: ppl7_debug pplib8_debug wmlib_debug gui_debug

qmake-info:
	@echo "QMAKE = $(QMAKE)"
	@$(QMAKE) -query QT_VERSION
	@$(QMAKE) -query QT_INSTALL_BINS

configure: autoconf_configure pplib8_configure wmlib_configure

autoconf_configure:
	./configure --enable-gtest=`pwd`/wmlib/tests/googletest-release-1.10.0
	#cmake -S pplib -B pplib/build -DCMAKE_BUILD_TYPE=Release


all: release debug

ppl7: ppl7_release ppl7_debug


wmlib: wmlib_release wmlib_debug

gui: release

update_version:
	@mkdir -p tmp
	@echo "#ifndef VERSION_H_" > tmp/version.h
	@echo "#define VERSION_H_" >> tmp/version.h
	@echo "#define WM_VERSION \"$(VERSION)\"" >> tmp/version.h
	@echo "#define WM_REVISION \"$(BUILD_NUMBER)\"" >> tmp/version.h
	@echo "#define WM_RELEASEDATE $(BUILDDATE)" >> tmp/version.h
	@echo "#endif" >> tmp/version.h
	@diff tmp/version.h gui/include/version.h > /dev/null 2>&1; if [ $$? -ne 0 ] ; then cp tmp/version.h gui/include/version.h; fi

mingw: update_version
	mingw32-make -j8 ppl7_release
	mingw32-make -j8 wmlib_release
	cd gui; $(QMAKE) INCLUDEPATH+=../pplib/include INCLUDEPATH+=../lib/include WinMusik.pro
	mingw32-make -j8 gui_release
	cd gui && lupdate.exe WinMusik.pro
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

mingw_debug: update_version
	mingw32-make -j8 ppl7_debug
	mingw32-make -j8 wmlib_debug
	cd gui; $(QMAKE) INCLUDEPATH+=../pplib/include INCLUDEPATH+=../lib/include WinMusik.pro
	mingw32-make -j8 gui_debug
	
setup: mingw
	"C:\Program Files (x86)\Inno Setup 5\ISCC.exe" setup.iss


###########################################################################
### PPLIB8
###########################################################################
pplib8_configure:
	cmake -S pplib8 -B build/pplib8-debug   -DCMAKE_BUILD_TYPE=Debug   $(PPLIB8_CMAKE_FLAGS)
	cmake -S pplib8 -B build/pplib8-release -DCMAKE_BUILD_TYPE=Release $(PPLIB8_CMAKE_FLAGS)

pplib8: pplib8_debug pplib8_release

pplib8_debug:
	cmake -S pplib8 -B build/pplib8-debug   -DCMAKE_BUILD_TYPE=Debug   $(PPLIB8_CMAKE_FLAGS)
	cmake --build build/pplib8-debug -j

pplib8_release:
	cmake -S pplib8 -B build/pplib8-release -DCMAKE_BUILD_TYPE=Release $(PPLIB8_CMAKE_FLAGS)
	cmake --build build/pplib8-release -j

pplib8_test: pplib8_debug
	-build/pplib8-debug/tests/test_all$(EXE) --gtest_color=yes


###########################################################################
### wmlib
###########################################################################

wmlib_configure:
	cmake -S wmlib -B build/wmlib-debug -DCMAKE_BUILD_TYPE=Debug
	cmake -S wmlib -B build/wmlib-release -DCMAKE_BUILD_TYPE=Release

wmlib_release: pplib8_release
	cmake -S wmlib -B build/wmlib-release -DCMAKE_BUILD_TYPE=Release
	cmake --build build/wmlib-release -j

wmlib_debug: pplib8_debug
	cmake -S wmlib -B build/wmlib-debug -DCMAKE_BUILD_TYPE=Debug
	cmake --build build/wmlib-debug -j

wmlib_tests: pplib8_debug
	cmake -B build/wmlib-tests -DCMAKE_BUILD_TYPE=Debug \
		-DWMLIB_BUILD_TESTS=ON -DWMLIB_ENABLE_COVERAGE=OFF
	cmake --build build/wmlib-tests --target wmlib_test

	

wmlib_coverage:
	cmake -B build/wmlib-coverage -DCMAKE_BUILD_TYPE=Debug \
		-DWMLIB_BUILD_TESTS=ON -DWMLIB_ENABLE_COVERAGE=ON
	ln -sf build/wmlib-coverage/compile_commands.json compile_commands.json
	cmake --build build/wmlib-coverage --target wmlib_test
	cd wmlib/tests && ../../build/wmlib-coverage/tests/wmlib_test
	mkdir -p coverage_html
	gcovr --root . build/wmlib-coverage --filter wmlib/src --medium-threshold 70 \
		--exclude-throw-branches --html-details coverage_html/index.html --xml-pretty -o coverage.xml
	@echo "Report: coverage_html/index.html"


###########################################################################
### RELEASE
###########################################################################
ppl7_release:
	$(MAKE) --directory=pplib release/libppl7-core.a release/libppl7-crypto.a release/libppl7-inet.a release/libppl7-audio.a
	ar -r pplib/release/libpplib.a pplib/release/*.o


gui_release: gui/Makefile update_version
	mkdir -p gui/release
	rm -rf gui/release/WinMusik
	cd gui; $(QMAKE) INCLUDEPATH+=../pplib/include INCLUDEPATH+=../lib/include WinMusik.pro
	$(MAKE) --directory=gui release
	mkdir -p release
	cp gui/release/WinMusik release/WinMusik

###########################################################################
### DEBUG
###########################################################################
ppl7_debug:
	$(MAKE) --directory=pplib debug/libppl7-core.a debug/libppl7-crypto.a debug/libppl7-inet.a debug/libppl7-audio.a
	ar -r pplib/debug/libpplib.a pplib/debug/*.o

gui_debug: gui/Makefile update_version
	mkdir -p gui/debug
	rm -rf gui/debug/WinMusik
	cd gui; $(QMAKE) INCLUDEPATH+=../pplib/include INCLUDEPATH+=../lib/include WinMusik.pro
	$(MAKE) --directory=gui debug
	mkdir -p debug
	cp gui/debug/WinMusik debug/WinMusik

###########################################################################
### Tests
###########################################################################
test:
	echo "TODO"
	#cd wmlib/tests && ./testsuite$(EXE)


###########################################################################
### MISC
###########################################################################

clean:
	@rm -rf documentation release debug build
	@-find ./ -name 'stamp-h*' -exec rm -rf {} \;
	-@ $(MAKE) --directory=gui clean > /dev/null 2>&1
	@rm -rf gui/release gui/debug gui/Makefile* release debug pplib/debug lib/compile
	@rm -rf gui/ui_*.h
	-@ $(MAKE) --directory=pplib clean > /dev/null 2>&1

distclean: clean
	@rm -rf gui/Makefile* gui/WinMusik.pro .qmake.stash autom4te.cache
	@rm -rf config.log config.status
	@rm -rf Package/WinMusik.desktop
	-@ $(MAKE) --directory=pplib distclean > /dev/null 2>&1


gui/Makefile: gui/WinMusik.pro
	cd gui; $(QMAKE) INCLUDEPATH+=../pplib/include INCLUDEPATH+=../lib/include WinMusik.pro


history:
	@echo "Git history between Tag $(PREVIOUS_TAG) and head" >  history.tmp
	@echo "================================================================================" >>  history.tmp
	@git log --pretty=format:"%ad=!=%s" --date=iso8601-strict $(PREVIOUS_TAG)...master | sort | awk -F "=!=" '{ print $$2}' >> history.tmp
	cat history.tmp
