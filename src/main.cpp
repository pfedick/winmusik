/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.4 $
 * $Date: 2011/05/15 10:55:57 $
 * $Id: main.cpp,v 1.4 2011/05/15 10:55:57 pafe Exp $
 *
 *
 * Copyright (c) 2010 Patrick Fedick
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
#include <ppl6-grafix.h>
#include "version.h"

#include <QtGui>
#include <QApplication>
#include <Python.h>

ppl6::CLog *wmlog=NULL;

void help()
{
	printf ("%s, Version %s\n",WM_APPNAME,WM_VERSION);
	printf ("\nCommandline options:\n"
			"  -h | --help   Show this help\n"
			"  -c FILE       Alternative configfile\n"
			);
}


int main(int argc, char *argv[])
{
	ppl6::PPLInit();
	ppl6::grafix::CGrafix gfx;
	if (ppl6::getargv(argc,argv,"-h")!=NULL || ppl6::getargv(argc,argv,"--help")!=NULL) {
		help();
		return 0;
	}

    QApplication a(argc, argv);

    // Deprecated in Qt5, Qt5 geht davon aus, dass der Sourcecode UTF-8 kodiert ist
#if QT_VERSION < 0x050000
    	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

    if (setlocale(LC_CTYPE,"")==NULL) {
		printf ("setlocale fehlgeschlagen\n");
		throw std::exception();
    }
    //printf ("Locale: %s\n",setlocale(LC_CTYPE,NULL));

    Py_InitializeEx(0);

    CWmClient Client;
    if (!Client.Init(argc,argv,&a)) {
    	Client.RaiseError();
    	Py_Finalize();
    	return 1;
    }
    if (!Client.Start()) {
    	if (ppl6::GetErrorCode()>0) {
    		Client.RaiseError();
    	}
    	Py_Finalize();
    	return 1;
    }
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    int ret=a.exec();
    Py_Finalize();
    return ret;
}
