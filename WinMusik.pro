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
    gui
CONFIG += debug_and_release
CONFIG(debug, debug|release) { 
    win32:TARGET = WinMusik
    unix:TARGET = debug/WinMusik
}
else { 
    win32:TARGET = WinMusik
    unix:TARGET = release/WinMusik
}
HEADERS += include/coverviewer.h \
    widgets/fkeys.h \
    widgets/edittrack.h \
    include/csearchlist.h \
    include/massimport.h \
    include/edittrackdialog.h \
    include/coverprinter.h \
    include/searchlists.h \
    include/searchlistdialog.h \
    include/searchlisttrackdialog.h \
    include/shortcutdialog.h \
    include/updater.h \
    include/registration.h \
    include/printdevicedialog.h \
    include/oimpinfo.h \
    include/wmstorage.h \
    include/tablecontrol.h \
    include/tablesearch.h \
    include/editdevice.h \
    include/splashscreen.h \
    include/wm20import.h \
    include/about.h \
    include/langselect.h \
    include/firststart.h \
    include/winmusik3.h \
    src/properties/properties.h \
    include/search.h \
    include/edit.h \
    include/menue.h \
    include/playlists.h \
    include/devicelist.h \
    include/wmtoolbutton.h \
    include/asynchronousMessage.h \
    include/droparea.h
SOURCES += src/coverviewer.cpp \
    widgets/fkeys.cpp \
    widgets/edittrack.cpp \
    src/CSearchlist.cpp \
    src/edittrackdialog.cpp \
    src/CTrackList.cpp \
    src/massimport.cpp \
    src/coverprinter.cpp \
    src/searchlists.cpp \
    src/searchlistdialog.cpp \
    src/searchlisttrackdialog.cpp \
    src/shortcutdialog.cpp \
    src/updater.cpp \
    src/registration.cpp \
    src/CHashes.cpp \
    src/printdevicedialog.cpp \
    src/Print.cpp \
    src/oimpinfo.cpp \
    src/StorageOriginalImportData.cpp \
    src/CID3TagSaver.cpp \
    src/CTableControl.cpp \
    src/tablesearch.cpp \
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
    src/firststart.cpp \
    src/config.cpp \
    src/properties/properties.cpp \
    src/search.cpp \
    src/menue.cpp \
    src/main.cpp \
    src/edit.cpp \
    src/edit-functions.cpp \
    src/playlists.cpp \
    src/devicelist.cpp \
    src/wmtoolbutton.cpp \
    src/asynchronousMessage.cpp \
    src/DropArea.cpp
FORMS += forms/coverviewer.ui \
    widgets/fkeys.ui \
    widgets/edittrack.ui \
    forms/edittrackdialog.ui \
    forms/searchlists.ui \
    forms/massimport.ui \
    forms/searchlistdialog.ui \
    forms/searchlisttrackdialog.ui \
    forms/coverprinter.ui \
    forms/shortcutdialog.ui \
    forms/updater.ui \
    forms/registration.ui \
    forms/printdevicedialog.ui \
    forms/oimpinfo.ui \
    forms/tablesearch.ui \
    forms/editdevice.ui \
    forms/splashscreen.ui \
    forms/about.ui \
    forms/langselect.ui \
    forms/firststart.ui \
    src/properties/properties.ui \
    forms/search.ui \
    forms/edit.ui \
    forms/menue.ui \
    forms/playlists.ui \
    forms/devicelist.ui \
    forms/asynchronousMessage.ui
RESOURCES += resources.qrc
RC_FILE = resource.rc
INCLUDEPATH += include
INCLUDEPATH += widgets
unix:INCLUDEPATH += /usr/local/include
win32:INCLUDEPATH += C:/mingw/usr/local/include
win32:QMAKE_LIBDIR += C:/mingw/usr/local/lib
win32:QMAKE_LFLAGS += -static-libgcc
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
CODECFORSRC = UTF-8
CODECFORTR = UTF-8
TRANSLATIONS = winmusik_de.ts \
    winmusik_en.ts
