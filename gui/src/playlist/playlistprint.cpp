#define WITH_QT
#define PPL_QT_STRING_UTF8
#include <ppl6.h>
#include <ppl7.h>
#include <ppl7-ppl6compat.h>
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
#define COL_TITLE		1
#define COL_COMMENT		2
#define COL_BPM			3
#define COL_KEY			4
#define COL_START		5
#define COL_END			6
#define COL_CUTS		7
#define COL_LENGTH		8
#define COL_TOTAL		9


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
    int x,y;
    int trackno, page;
    int lastbpm=0;
    float totalLength;
    bool headerPrinted;
    ColumnDimension dimension[10];
    void printHeader(const PlaylistTracks *tracks);
    void printFooter(QPrinter &Printer);
    void printLine(PlaylistItem *item);
    void printRect(int column, const ppl7::String &text);
    void highlightRect(int column);
public:
    PlayListPrintDJ ();
    void print(const PlaylistTracks *tracks, QPrinter &Printer);
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
    ppl6::CString Tmp;
    QPrinter Printer(QPrinter::HighResolution);
    if (wm->conf.bPrintColors) Printer.setColorMode(QPrinter::Color);
    else Printer.setColorMode(QPrinter::GrayScale);
    wm->conf.DefaultPrintPath.Trim();
    if (wm->conf.DefaultPrintPath.IsEmpty()) wm->conf.DefaultPrintPath=QDir::homePath();
    ppl6::CString Filename=wm->conf.DefaultPrintPath;
    Filename.Concatf("/");
    Filename+=tr("WinMusik Playlist","Default Filename in Printdialog");
    Filename.Concat(" - playlist");
#ifndef _WIN32
    Filename.Concat(".pdf");
#endif
    Printer.setOutputFileName (Filename);
    QPrintDialog printDialog(&Printer, NULL);
    if (printDialog.exec() != QDialog::Accepted) return;
    wm->conf.DefaultPrintPath=ppl6::GetPath(Printer.outputFileName());

    if (Printer.colorMode()==QPrinter::Color) wm->conf.bPrintColors=true;
    else wm->conf.bPrintColors=false;
    wm->conf.Save();

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

	dimension[COL_TRACK].set(0,7);
	dimension[COL_TITLE].set(7,88);
	dimension[COL_COMMENT].set(95,32);
	dimension[COL_BPM].set(127,10);
	dimension[COL_KEY].set(137,13);
	dimension[COL_START].set(150,10);
	dimension[COL_END].set(160,10);
	dimension[COL_CUTS].set(170,10);
	dimension[COL_LENGTH].set(180,10);
	dimension[COL_TOTAL].set(190,12);

}

void PlayListPrintDJ::printRect(int column, const ppl7::String &text)
{
	painter.save();
	const ColumnDimension &d=dimension[column];
	painter.drawRect((x+d.x)*w, y*h, d.width*w, 5*h);
	painter.setClipRect((x+d.x)*w, y*h, (d.width-1)*w, 5*h);
	painter.drawText((x+d.x+1)*w,(y+4)*h,text);
	painter.restore();
}

void PlayListPrintDJ::highlightRect(int column)
{
	const ColumnDimension &d=dimension[column];
	painter.save();
	painter.fillRect((x+d.x)*w, y*h, d.width*w, 5*h, QColor(240,240,240));
	painter.restore();
}

void PlayListPrintDJ::printHeader(const PlaylistTracks *tracks)
{
    headerPrinted=true;
    Font.setPointSize(30);
    Font.setBold(true);
    painter.save();
    painter.setPen(Pen);
    painter.setBrush(Brush);
    painter.setFont(Font);
    ppl6::CString Tmp=tracks->getName();
    painter.drawText(x*w,(y)*h,Tmp);
    Font.setPointSize(12);
    painter.setFont(Font);
    Tmp.Setf("Ausgabe %d vom %s", tracks->getIssueNumber(),
    		(const char*)tracks->getIssueDate().getDate("%d.%m.%Y"));
    if (tracks->getSubName().NotEmpty())
    	Tmp+=", "+tracks->getSubName();
    painter.drawText(x*w,(y+8)*h,Tmp);
    y+=10;
    Font.setPointSize(10);
    painter.setFont(Font);

    printRect(COL_TRACK,"#");
    printRect(COL_TITLE,"Artist - Title (Version)");
    printRect(COL_COMMENT,"Comment");
    printRect(COL_BPM,"BPM");
    printRect(COL_KEY,"Key");
    printRect(COL_START,"Start");
    printRect(COL_END,"End");
    printRect(COL_CUTS,"Cuts");
    printRect(COL_LENGTH,"Length");
    printRect(COL_TOTAL,"Total");

    painter.restore();
    y+=5;
}

void PlayListPrintDJ::printFooter(QPrinter &Printer)
{
	painter.save();
	Font.setPointSize(10);
	Font.setBold(true);
	painter.setFont(Font);
	painter.drawText(90*w,Printer.height()-1*h,ppl7::ToString("Page %d",page));
	painter.restore();
}

static ppl7::String formatTime(float t)
{
	ppl7::String Tmp;
	Tmp.setf("%02i:%02i",(int)(t/60.0f),(int)t%60);
	return Tmp;
}

void PlayListPrintDJ::printLine(PlaylistItem *item)
{
	ppl7::String Tmp;
	Font.setPointSize(7);
	Font.setBold(false);
	painter.save();
	painter.setFont(Font);
	painter.setPen(Pen);
	painter.setBrush(Brush);
	printRect(COL_TRACK,ppl7::ToString("%d",trackno));
	Tmp=item->Artist + " - " + item->Title;
	Tmp+=" (";
	Tmp+=item->Version;
	Tmp+=")";
	printRect(COL_TITLE,Tmp);
	if (item->Remarks.NotEmpty())
		printRect(COL_COMMENT,ppl7::ToString("%s",(const char*)item->Remarks));
	else
		printRect(COL_COMMENT,"");
	int bpm=(item->bpmPlayed>0?item->bpmPlayed:item->bpm);
	if (trackno>1 && lastbpm!=bpm) {
		Font.setBold(true);
		painter.setFont(Font);
		Tmp.setf("%d %s",bpm,(bpm>lastbpm?"⇧":"⇩"));
		highlightRect(COL_BPM);
		printRect(COL_BPM,Tmp);
		Font.setBold(false);
		painter.setFont(Font);
	} else {
		Tmp.setf("%d",bpm);
		printRect(COL_BPM,Tmp);
	}
	lastbpm=bpm;
	MusicKey key((int)item->musicKey);
	ppl6::CArray res;
	if (item->Remarks.PregMatch("key\\s*[+]([1234])",res)) {
		Font.setBold(true);
		painter.setFont(Font);
		highlightRect(COL_KEY);
		key=key.addSemitone(atoi(res[1]));
		printRect(COL_KEY,ppl7::ToString("%s ⇧+%s",(const char*)key.name(),
				res[1]));
		Font.setBold(false);
		painter.setFont(Font);
	} else if (item->Remarks.PregMatch("key\\s*[-]([1234])",res)) {
		Font.setBold(true);
		painter.setFont(Font);
		highlightRect(COL_KEY);
		key=key.addSemitone(-atoi(res[1]));
		printRect(COL_KEY,ppl7::ToString("%s ⇩-%s",(const char*)key.name(),
						res[1]));
		Font.setBold(false);
		painter.setFont(Font);
	} else {
		printRect(COL_KEY,ppl7::ToString("%s",(const char*)key.name()));
	}
	printRect(COL_START,formatTime(item->startPositionSec));
	printRect(COL_END,formatTime(item->endPositionSec));
	float cutLength=0.0f;
	for (int i=0;i<5;i++) {
		cutLength+=(item->cutEndPosition[i]-item->cutStartPosition[i]);
	}
	printRect(COL_CUTS,formatTime(cutLength));
	printRect(COL_LENGTH,formatTime(item->mixLength));
	totalLength+=item->mixLength;
	printRect(COL_TOTAL,formatTime(totalLength));

	painter.restore();
	trackno++;

	y+=5;
}

void PlayListPrintDJ::print(const PlaylistTracks *tracks, QPrinter &Printer)
{
    if (!painter.begin(&Printer)) {
        ppl6::SetError(20027);
        wm_main->RaiseError();
        return;
    }
    headerPrinted=false;
    trackno=page=1;
    totalLength=0.0f;
    lastbpm=0;
    w=(float)Printer.width()/(float)Printer.widthMM();
    h=(float)Printer.height()/(float)Printer.heightMM();
    x=1;
    y=10;
    printHeader(tracks);
    for (int i=0;i<tracks->topLevelItemCount();i++) {
        if (!headerPrinted) printHeader(tracks);
        PlaylistItem *item=static_cast<PlaylistItem*>(tracks->topLevelItem(i));
        printLine(item);
        if (y*h>Printer.height()-(15*h)) {
            headerPrinted=false;
            printFooter(Printer);
            Printer.newPage();
            x=10;
            y=10;
            page++;
        }
    }
    printFooter(Printer);
    painter.end();
}
