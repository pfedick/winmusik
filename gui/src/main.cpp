/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2022 Patrick Fedick
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include "src/menue/menue.h"
#include "winmusik3.h"
#include "version.h"

#include <QtGui>
#include <QApplication>
#include <QSettings>
#include <QStyleFactory>
#ifdef HAVE_PYTHON
#include <Python.h>
#endif

ppl7::Logger* wmlog=NULL;

void help()
{
    printf("%s, Version %s\n", WM_APPNAME, WM_VERSION);
    printf("\nCommandline options:\n"
        "  -h | --help   Show this help\n"
        "  -c FILE       Alternative configfile\n"
    );
}


int main(int argc, char* argv[])
{
    //printf ("DEBUG\n");
    ppl6::PPLInit();
    //ppl6::grafix::CGrafix gfx;
    if (ppl6::getargv(argc, argv, "-h") != NULL || ppl6::getargv(argc, argv, "--help") != NULL) {
        help();
        return 0;
    }

    QApplication a(argc, argv);

#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", QSettings::NativeFormat);
    if (settings.value("AppsUseLightTheme") == 0) {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        QPalette darkPalette;
        QColor darkColor = QColor(45, 45, 45);
        QColor disabledColor = QColor(127, 127, 127);
        darkPalette.setColor(QPalette::Window, darkColor);
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(18, 18, 18));
        darkPalette.setColor(QPalette::AlternateBase, darkColor);
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
        darkPalette.setColor(QPalette::Button, darkColor);
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
        darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledColor);

        qApp->setPalette(darkPalette);

        qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
    }
#endif

    // Deprecated in Qt5, Qt5 geht davon aus, dass der Sourcecode UTF-8 kodiert ist
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

    if (setlocale(LC_CTYPE, "") == NULL) {
        printf("setlocale fehlgeschlagen\n");
        throw std::exception();
    }
    if (setlocale(LC_NUMERIC, "C") == NULL) {
        printf("setlocale fuer LC_NUMERIC fehlgeschlagen\n");
        throw std::exception();
    }
    //printf ("Locale: %s\n",setlocale(LC_CTYPE,NULL));
#ifdef HAVE_PYTHON
    Py_InitializeEx(0);
#endif


    CWmClient Client;
    try {
        Client.Init(argc, argv, &a);
    } catch (const ppl7::Exception& exp) {
        ShowException(exp, QApplication::tr("Could not initialize WinMusik"));
        return 1;
    }
    if (!Client.Start()) {
        return 1;
    }
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    int ret=a.exec();
    return ret;
}
