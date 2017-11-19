# This file is part of WinMusik 3 by Patrick Fedick
# $Author: pafe $
# $Revision: 1.10 $
# $Date: 2012/01/13 19:45:03 $
# $Id: WinMusik3.pro,v 1.10 2012/01/13 19:45:03 pafe Exp $
# Copyright (c) 2012 Patrick Fedick
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
TEMPLATE = app
TARGET = WinMusik
QT += core \
    gui \
    xml
    
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += debug_and_release
CONFIG(debug, debug|release) { 
    win32:TARGET = WinMusik
    unix:TARGET = debug/WinMusik
}
else { 
    win32:TARGET = WinMusik
    unix:TARGET = release/WinMusik
}
HEADERS += widgets/fkeys.h \
    widgets/edittrack.h \
    widgets/keywheel.h \
    src/searchlists/searchlists.h \
    src/searchlists/searchlistdialog.h \
    src/searchlists/searchlisttracks.h \
    src/searchlists/searchlisttrackdialog.h \
    src/properties/properties.h \
    src/properties/regexpedit.h \
    src/editor/shortcutdialog.h \
    src/editor/renumberdialog.h \
    src/editor/cddbimport.h \
    src/editor/cddbselect.h \
    src/editor/oimpinfo.h \
    src/editor/tablesearch.h \
    src/editor/edit.h \
    include/coverviewer.h \
    include/csearchlist.h \
    src/editor/massimport.h \
    include/edittrackdialog.h \
    include/coverprinter.h \
    include/updater.h \
    include/registration.h \
    include/printdevicedialog.h \
    include/wmstorage.h \
    include/tablecontrol.h \
    include/editdevice.h \
    include/splashscreen.h \
    include/wm20import.h \
    include/about.h \
    include/langselect.h \
    include/firststart.h \
    include/winmusik3.h \
    include/backgroundjobs.h \
    include/resultfilter.h \
    src/search/search.h \
    src/menue/menue.h \
    include/regexpcapture.h \
    src/playlist/playlistStatusBar.h \
    src/playlist/playlist.h \
    src/playlist/playlisttracks.h \
    src/playlist/playlistedit.h \
    src/playlist/setbpmplayed.h \
    include/devicelist.h \
    include/traktor.h \
    include/wmtoolbutton.h \
    include/asynchronousMessage.h \
    include/droparea.h \
    include/plugins.h
SOURCES += widgets/fkeys.cpp \
    widgets/edittrack.cpp \
    widgets/keywheel.cpp \
    src/searchlists/searchlists.cpp \
    src/searchlists/searchlistdialog.cpp \
    src/searchlists/searchlisttracks.cpp \
    src/searchlists/searchlisttrackdialog.cpp \
    src/searchlists/dupecheckthread.cpp \
    src/properties/properties.cpp \
    src/properties/regexpedit.cpp \
    src/editor/edit.cpp \
    src/editor/edit-functions.cpp \
    src/editor/shortcutdialog.cpp \
    src/editor/renumberdialog.cpp \
    src/editor/cddbimport.cpp \
    src/editor/cddbselect.cpp \
    src/editor/oimpinfo.cpp \
    src/editor/tablesearch.cpp \
    src/coverviewer.cpp \
    src/CSearchlist.cpp \
    src/edittrackdialog.cpp \
    src/CTrackList.cpp \
    src/editor/massimport.cpp \
    src/coverprinter.cpp \
    src/resultfilter.cpp \
    src/regexpcapture.cpp \
    src/updater.cpp \
    src/registration.cpp \
    src/CHashes.cpp \
    src/printdevicedialog.cpp \
    src/Print.cpp \
    src/StorageOriginalImportData.cpp \
    src/CID3TagSaver.cpp \
    src/CTableControl.cpp \
    src/functions.cpp \
    src/editdevice.cpp \
    src/splashscreen.cpp \
    src/StorageTrack.cpp \
    src/StorageDevice.cpp \
    src/StorageSimpleTable.cpp \
    src/StorageTitle.cpp \
    src/StorageShortcut.cpp \
    src/wm20import.cpp \
    src/CWmFile.cpp \
    src/CStorage.cpp \
    src/InitialDatabase.cpp \
    src/about.cpp \
    src/langselect.cpp \
    src/errors.cpp \
    src/CWmClient.cpp \
    src/backgroundjobs.cpp \
    src/firststart.cpp \
    src/config.cpp \
    src/search/search.cpp \
    src/menue/menue.cpp \
    src/main.cpp \
    src/playlist/playlist.cpp \
    src/playlist/playlistStatusBar.cpp \
    src/playlist/playlistedit.cpp \
    src/playlist/playlisttracks.cpp \
    src/playlist/setbpmplayed.cpp \
    src/devicelist.cpp \
    src/traktor.cpp \
    src/wmtoolbutton.cpp \
    src/asynchronousMessage.cpp \
    src/DropArea.cpp \
    src/plugins.cpp
FORMS += widgets/fkeys.ui \
    widgets/edittrack.ui \
    src/editor/oimpinfo.ui \
    src/editor/tablesearch.ui \
    src/editor/shortcutdialog.ui \
    src/editor/renumberdialog.ui \
    src/editor/cddbimport.ui \
    src/editor/cddbselect.ui \
    src/editor/edit.ui \
    src/searchlists/searchlists.ui \
    src/searchlists/searchlistdialog.ui \
    src/searchlists/searchlisttrackdialog.ui \
    src/properties/properties.ui \
    src/properties/regexpedit.ui \
    forms/coverviewer.ui \
    src/editor/massimport.ui \
    forms/edittrackdialog.ui \
    forms/coverprinter.ui \
    forms/updater.ui \
    forms/registration.ui \
    forms/printdevicedialog.ui \
    forms/editdevice.ui \
    forms/splashscreen.ui \
    forms/about.ui \
    forms/langselect.ui \
    forms/firststart.ui \
    src/search/search.ui \
    src/menue/menue.ui \
    src/playlist/playlist.ui \
    src/playlist/playlistedit.ui \
    src/playlist/setbpmplayed.ui \
    forms/devicelist.ui \
    forms/asynchronousMessage.ui
RESOURCES += resources.qrc
RC_FILE = resource.rc
INCLUDEPATH += include
INCLUDEPATH += widgets
unix:INCLUDEPATH += /usr/local/include
win32:INCLUDEPATH += C:/mingw/usr/local/include
win32:INCLUDEPATH += D:/jenkins/local/include
win32:QMAKE_LIBDIR += C:/mingw/usr/local/lib
win32:QMAKE_LFLAGS += -static-libgcc
#QMAKE_CXXFLAGS += `python-config --cflags`

CONFIG(debug, debug|release) {
    # Debug
    unix:LIBS += `ppl6-config \
        --libs \
        debug`
    win32:LIBS += `ppl6-config \
        --libs \
        debug`
}
else {
    # Release
    unix:LIBS += `ppl6-config \
        --libs \
        release`
    win32:LIBS += `ppl6-config \
        --libs \
        release`
}
#LIBS += `python-config --ldflags`
win32:LIBS += -lwinmm
CODECFORSRC = UTF-8
CODECFORTR = UTF-8
TRANSLATIONS = winmusik_de.ts \
    winmusik_en.ts

DISTFILES += \
    setup.iss \
    setup64.iss \
    HISTORY.TXT \
    build_mingw64.sh
