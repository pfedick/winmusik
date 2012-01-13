TEMPLATE = app
CONFIG += debug
TARGET = wm2search
QT += core \
    gui
HEADERS += wm2search.h
SOURCES += main.cpp \
    wm2search.cpp
FORMS += wm2search.ui
RESOURCES += resources.qrc
win32:INCLUDEPATH += /usr/local/include
unix:INCLUDEPATH += /usr/local/include
unix:LIBS += -lwm40 `ppl6-config \
    --libs \
    debug`
win32:LIBS += -lwm40 `ppl6-config \
    --libs \
    debug`

