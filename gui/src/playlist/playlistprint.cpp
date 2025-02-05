/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * Copyright (c) 2024, Patrick Fedick
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

#define WITH_QT
#define PPL_QT_STRING_UTF8
#include "ppl7.h"
#include "winmusik3.h"
#include "playlisttracks.h"
#include "playlist.h"
#include "musickey.h"
#include <QDir>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QRect>
#include <QPoint>

#define COL_TRACK		0
#define COL_TIMECODE	1
#define COL_TITLE		2
#define COL_COMMENT		3
#define COL_BPM			4
#define COL_KEY			5
#define COL_START		6
#define COL_END			7
#define COL_CUTS		8
#define COL_LENGTH		9

class ColumnDimension
{
public:
    int x;
    int width;
    ColumnDimension();
    void set(int x, int width);
};


class PlayListPrintDJ
{
private:
    QPainter painter;
    QFont Font;
    QPen Pen;  // creates a default pen
    QBrush Brush;
    float w, h;
    int x, y;
    int trackno, page;
    int lastbpm=0;
    float totalLength;
    bool headerPrinted;
    ColumnDimension dimension[10];
    void printHeader(const PlaylistTracks* tracks);
    void printFooter(QPrinter& Printer);
    void printLine(PlaylistItem* item);
    void printRect(int column, const ppl7::String& text);
    void highlightRect(int column);
public:
    PlayListPrintDJ();
    void print(const PlaylistTracks* tracks, QPrinter& Printer);
};

ColumnDimension::ColumnDimension()
{
    x=width=0;
}

void ColumnDimension::set(int x, int width)
{
    this->x=x;
    this->width=width;
}

void Playlist::on_printPlaylist_triggered()
{
    QPrinter Printer(QPrinter::HighResolution);
    if (wm->conf.bPrintColors) Printer.setColorMode(QPrinter::Color);
    else Printer.setColorMode(QPrinter::GrayScale);
    wm->conf.DefaultPrintPath.trim();
    if (wm->conf.DefaultPrintPath.isEmpty()) wm->conf.DefaultPrintPath=QDir::homePath();
    ppl7::String Filename=wm->conf.DefaultPrintPath;
    Filename+="/";
    Filename+=tr("WinMusik Playlist", "Default Filename in Printdialog");
    Filename+=" - playlist";
#ifndef _WIN32
    Filename+=".pdf";
#endif
    Printer.setOutputFileName(Filename);
    QPrintDialog printDialog(&Printer, NULL);
    if (printDialog.exec() != QDialog::Accepted) return;
    wm->conf.DefaultPrintPath=ppl7::File::getPath(Printer.outputFileName());

    if (Printer.colorMode() == QPrinter::Color) wm->conf.bPrintColors=true;
    else wm->conf.bPrintColors=false;
    wm->conf.trySave();

    PlayListPrintDJ print;
    print.print(ui.tracks, Printer);
}

PlayListPrintDJ::PlayListPrintDJ()
{
    headerPrinted=false;
    w=h=0.0f;
    x=y=0;
    trackno=page=0;
    totalLength=0.0f;

    Pen.setStyle(Qt::SolidLine);
    Pen.setWidth(2);
    Pen.setBrush(Qt::black);
    Brush.setColor(Qt::black);
    Brush.setStyle(Qt::NoBrush);

    dimension[COL_TRACK].set(0, 7);
    dimension[COL_TIMECODE].set(7, 15);
    dimension[COL_TITLE].set(22, 85);
    dimension[COL_COMMENT].set(107, 32);
    dimension[COL_BPM].set(139, 10);
    dimension[COL_KEY].set(149, 13);
    dimension[COL_START].set(162, 10);
    dimension[COL_END].set(172, 10);
    dimension[COL_CUTS].set(182, 10);
    dimension[COL_LENGTH].set(192, 10);


}

void PlayListPrintDJ::printRect(int column, const ppl7::String& text)
{
    painter.save();
    const ColumnDimension& d=dimension[column];
    painter.drawRect((x + d.x) * w, y * h, d.width * w, 5 * h);
    painter.setClipRect((x + d.x) * w, y * h, (d.width - 1) * w, 5 * h);
    painter.drawText((x + d.x + 1) * w, (y + 4) * h, text);
    painter.restore();
}

void PlayListPrintDJ::highlightRect(int column)
{
    const ColumnDimension& d=dimension[column];
    painter.save();
    painter.fillRect((x + d.x) * w, y * h, d.width * w, 5 * h, QColor(240, 240, 240));
    painter.restore();
}

void PlayListPrintDJ::printHeader(const PlaylistTracks* tracks)
{
    headerPrinted=true;
    Font.setPointSize(30);
    Font.setBold(true);
    painter.save();
    painter.setPen(Pen);
    painter.setBrush(Brush);
    painter.setFont(Font);
    ppl7::String Tmp=tracks->getName();
    painter.drawText(x * w, (y)*h, Tmp);
    Font.setPointSize(12);
    painter.setFont(Font);
    Tmp.setf("Ausgabe %d vom %s", tracks->getIssueNumber(),
        (const char*)tracks->getIssueDate().getDate("%d.%m.%Y"));
    if (tracks->getSubName().notEmpty())
        Tmp+=", " + tracks->getSubName();
    painter.drawText(x * w, (y + 8) * h, Tmp);
    y+=10;
    Font.setPointSize(10);
    painter.setFont(Font);

    printRect(COL_TRACK, "#");
    printRect(COL_TIMECODE, "TC");
    printRect(COL_TITLE, "Artist - Title (Version)");
    printRect(COL_COMMENT, "Comment");
    printRect(COL_BPM, "BPM");
    printRect(COL_KEY, "Key");
    printRect(COL_START, "Start");
    printRect(COL_END, "End");
    printRect(COL_CUTS, "Cuts");
    printRect(COL_LENGTH, "Length");


    painter.restore();
    y+=5;
}

void PlayListPrintDJ::printFooter(QPrinter& Printer)
{
    painter.save();
    Font.setPointSize(10);
    Font.setBold(true);
    painter.setFont(Font);
    painter.drawText(90 * w, Printer.height() - 1 * h, ppl7::ToString("Page %d", page));
    painter.restore();
}

static ppl7::String formatTime(float t)
{
    ppl7::String Tmp;
    Tmp.setf("%02i:%02i", (int)(t / 60.0f), (int)t % 60);
    return Tmp;
}

void PlayListPrintDJ::printLine(PlaylistItem* item)
{
    ppl7::String Tmp;
    Font.setPointSize(7);
    Font.setBold(false);
    painter.save();
    painter.setFont(Font);
    painter.setPen(Pen);
    painter.setBrush(Brush);
    printRect(COL_TRACK, ppl7::ToString("%d", trackno));
    printRect(COL_TIMECODE, getReadableTimeFromSeconds(totalLength));

    Tmp=item->Artist + " - " + item->Title;
    Tmp+=" (";
    Tmp+=item->Version;
    Tmp+=")";
    printRect(COL_TITLE, Tmp);
    if (item->Remarks.notEmpty())
        printRect(COL_COMMENT, item->Remarks);
    else
        printRect(COL_COMMENT, "");
    int bpm=(item->bpmPlayed > 0 ? item->bpmPlayed : item->bpm);
    if (trackno > 1 && lastbpm != bpm) {
        Font.setBold(true);
        painter.setFont(Font);
        Tmp.setf("%d %s", bpm, (bpm > lastbpm ? "▲" : "▼"));
        highlightRect(COL_BPM);
        printRect(COL_BPM, Tmp);
        Font.setBold(false);
        painter.setFont(Font);
    } else {
        Tmp.setf("%d", bpm);
        printRect(COL_BPM, Tmp);
    }
    lastbpm=bpm;
    MusicKey key((int)item->musicKey);
    std::vector<ppl7::String> res;
    if (item->keyModification > 0) {
        Font.setBold(true);
        painter.setFont(Font);
        highlightRect(COL_KEY);
        key=key.addSemitone(item->keyModification);
        printRect(COL_KEY, ppl7::ToString("%s ▲+%d", (const char*)key.name(),
            item->keyModification));
        Font.setBold(false);
        painter.setFont(Font);
    } else if (item->keyModification < 0) {
        Font.setBold(true);
        painter.setFont(Font);
        highlightRect(COL_KEY);
        key=key.addSemitone(item->keyModification);
        printRect(COL_KEY, ppl7::ToString("%s ▼%d", (const char*)key.name(),
            item->keyModification));
        Font.setBold(false);
        painter.setFont(Font);
    } else if (ppl7::RegEx::capture("key\\s*[+]([1234])", item->Remarks, res)) {
        Font.setBold(true);
        painter.setFont(Font);
        highlightRect(COL_KEY);
        key=key.addSemitone(atoi(res[1]));
        printRect(COL_KEY, ppl7::ToString("%s ▲+%s", (const char*)key.name(), res[1]));
        Font.setBold(false);
        painter.setFont(Font);
    } else if (ppl7::RegEx::capture("key\\s*[-]([1234])", item->Remarks, res)) {
        Font.setBold(true);
        painter.setFont(Font);
        highlightRect(COL_KEY);
        key=key.addSemitone(-atoi(res[1]));
        printRect(COL_KEY, ppl7::ToString("%s ▼-%s", (const char*)key.name(), res[1]));
        Font.setBold(false);
        painter.setFont(Font);
    } else {
        printRect(COL_KEY, ppl7::ToString("%s", (const char*)key.name()));
    }
    printRect(COL_START, formatTime(item->startPositionSec));
    printRect(COL_END, formatTime(item->endPositionSec));
    float cutLength=0.0f;
    for (int i=0;i < 5;i++) {
        cutLength+=(item->cutEndPosition[i] - item->cutStartPosition[i]);
    }
    printRect(COL_CUTS, formatTime(cutLength));
    printRect(COL_LENGTH, formatTime(item->mixLength));
    totalLength+=item->mixLength;


    painter.restore();
    trackno++;

    y+=5;
}

void PlayListPrintDJ::print(const PlaylistTracks* tracks, QPrinter& Printer)
{
    if (!painter.begin(&Printer)) {
        ShowError(QObject::tr("could not start printing"));
        return;
    }
    headerPrinted=false;
    trackno=page=1;
    totalLength=0.0f;
    lastbpm=0;
    w=(float)Printer.width() / (float)Printer.widthMM();
    h=(float)Printer.height() / (float)Printer.heightMM();
    x=1;
    y=10;
    printHeader(tracks);
    for (int i=0;i < tracks->topLevelItemCount();i++) {
        if (!headerPrinted) printHeader(tracks);
        PlaylistItem* item=static_cast<PlaylistItem*>(tracks->topLevelItem(i));
        printLine(item);
        if (y * h > Printer.height() - (15 * h)) {
            headerPrinted=false;
            printFooter(Printer);
            Printer.newPage();
            x=1;
            y=10;
            page++;
        }
    }
    printFooter(Printer);
    painter.end();
}
