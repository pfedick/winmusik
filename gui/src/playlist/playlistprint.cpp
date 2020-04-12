#include "winmusik3.h"
#include "playlisttracks.h"
#include "playlist.h"
#include <QDir>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPen>
#include <QBrush>

class PlayListPrintDJ
{
private:
    QPainter painter;
    float w, h;
    int x,y;
    bool headerPrinted;
    void printHeader(const PlaylistTracks *tracks, QPrinter &Printer);
public:
    PlayListPrintDJ ();
    void print(const PlaylistTracks *tracks, QPrinter &Printer);
};

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
}

void PlayListPrintDJ::printHeader(const PlaylistTracks *tracks, QPrinter &Printer)
{
    headerPrinted=true;
}

void PlayListPrintDJ::print(const PlaylistTracks *tracks, QPrinter &Printer)
{
    if (!painter.begin(&Printer)) {
        ppl6::SetError(20027);
        wm_main->RaiseError();
        return;
    }

    w=(float)Printer.width()/(float)Printer.widthMM();
    h=(float)Printer.height()/(float)Printer.heightMM();
    x=15;
    y=10;

    for (int i=0;i<tracks->topLevelItemCount();i++) {
        if (!headerPrinted) printHeader(tracks, Printer);
        PlaylistItem *item=static_cast<PlaylistItem*>(tracks->topLevelItem(i));
        /*
        Tmp=item->Artist + " - " + item->Title;
        Tmp+=" (";
        Tmp+=item->Version;
        Tmp+=")";
        ppl6::CString TmpTxt=Tmp;
        TmpTxt.Chop(1);
        txt.Putsf("%3u. %s, %0i:%02i %s)\r\n",i+1,static_cast<const char*>(TmpTxt),
                  static_cast<int>(item->trackLength/60),item->trackLength%60,
                  static_cast<const char*>(Minuten));
        */

        if (y*h>Printer.height()-(15*h)) {
            headerPrinted=false;
            Printer.newPage();
            x=15;
            y=10;
        }

    }

    painter.end();

}
