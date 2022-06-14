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


#include "winmusik3.h"
#include "version.h"
#include <QMessageBox>
#include <QPrinter>
#include <QDir>
#include <QPrintDialog>
#include <QPainter>
#include <QPen>
#include <QBrush>

#include "src/printdevicedialog/printdevicedialog.h"

int CWmClient::PrintCoverDialog(QWidget* parent, int DeviceType, uint32_t DeviceId)
{
	PrintDeviceDialog Dialog(parent);
	Dialog.setDeviceType(DeviceType);
	Dialog.setDeviceRange(1, DeviceStore.GetHighestDevice(DeviceType));
	Dialog.setFromDevice(DeviceId);
	Dialog.setToDevice(DeviceId);
	Dialog.setFont(conf.PrinterFont);
	Dialog.setUseColor(conf.bPrintColors);
	if (Dialog.exec()) {
		conf.PrinterFont=Dialog.font();
		conf.bPrintColors=Dialog.useColor();
		if (Dialog.printCover()) {
			if (DeviceType == 2) return PrintMP3Cover(parent, DeviceType, Dialog.min(), Dialog.max());
			if (DeviceType == 7) return PrintMP3Cover(parent, DeviceType, Dialog.min(), Dialog.max());
			RaiseError(parent, tr("Cannot print cover"), tr("Coverdruck steht für diese Tonträgerart nicht zur Verfügung"));
		} else {
			PrintTracklist(parent, DeviceType, Dialog.min(), Dialog.max());
		}
	}

	return 1;
}

int CWmClient::PrintMP3Cover(QWidget* parent, int DeviceType, uint32_t start, uint32_t end)
{
	ppl7::String Tmp;
	const char* tmp;
	QPrinter Printer(QPrinter::HighResolution);
	if (conf.bPrintColors) Printer.setColorMode(QPrinter::Color);
	else Printer.setColorMode(QPrinter::GrayScale);
	//Printer.setPrinterName(conf.PrinterName);
	conf.DefaultPrintPath.trim();
	if (conf.DefaultPrintPath.isEmpty()) conf.DefaultPrintPath=QDir::homePath();
	ppl7::String Filename=conf.DefaultPrintPath;
	Filename.appendf("/");
	Filename+=tr("WinMusik Cover", "Default Filename in Printdialog");
	if (start != end) Filename.appendf(" - %s %i-%i.", (const char*)GetDeviceNameShort(DeviceType), start, end);
	else Filename.appendf(" - %s %i.pdf", (const char*)GetDeviceNameShort(DeviceType), start);

	Printer.setOutputFileName(Filename);

	QPrintDialog printDialog(&Printer, parent);

	if (printDialog.exec() != QDialog::Accepted) return 0;

	// Save Options
	conf.DefaultPrintPath=ppl7::File::getPath(Printer.outputFileName());
	conf.PrinterName=Printer.printerName();
	if (Printer.colorMode() == QPrinter::Color) conf.bPrintColors=true;
	else conf.bPrintColors=false;
	conf.trySave();

	// Es kann losgehen, wir treffen einige Vorbereitungen und setzen Defaults
	QPainter painter;
	QBrush Brush;
	QBrush Red;
	QRect Rect;
	QRect bbox;
	int x, y;

	Brush.setColor(Qt::black);
	Brush.setStyle(Qt::NoBrush);

	Red.setColor(Qt::red);
	Red.setStyle(Qt::NoBrush);

	QPen Pen;  // creates a default pen
	Pen.setStyle(Qt::SolidLine);
	Pen.setWidth(1);
	Pen.setBrush(Qt::black);

	QPen RedPen;  // creates a default pen
	RedPen.setStyle(Qt::SolidLine);
	RedPen.setWidth(1);
	RedPen.setBrush(Qt::red);

	if (!painter.begin(&Printer)) {
		RaiseError(parent, tr("Druckvorgang kann nicht gestartet werden"));
		return 0;
	}
	float w=(float)Printer.width() / (float)Printer.widthMM();
	float h=(float)Printer.height() / (float)Printer.heightMM();
	QFont Font;
	Font.setFamily(conf.PrinterFont);

	// Nun können wir der Reihe nach alle Cover drucken
	for (uint32_t DeviceId=start;DeviceId <= end;DeviceId++) {
		if (DeviceId > start) Printer.newPage();
		// Tonträger laden
		CTrackList tracklist=GetTracklist(DeviceType, DeviceId, 1);
		const DataDevice* device=DeviceStore.GetPtr(DeviceType, DeviceId);
		if (!device) continue;

		// Hilfslinien zeichnen
		painter.setPen(Pen);
		painter.setBrush(Brush);
		painter.drawRect(0, 0, (int)(181 * w), (int)(120 * h));
		painter.drawLine((int)(60 * w), 0, (int)(60 * w), (int)(120 * h));

		painter.drawRect(0, (int)(120 * h), (int)(151 * w), (int)(118 * h));
		painter.drawLine((int)(6 * w), (int)(120 * h), (int)(6 * w), (int)(238 * h));
		painter.drawLine((int)(145 * w), (int)(120 * h), (int)(145 * w), (int)(238 * h));

		// Frontside
		Font.setPointSize(20);
		Font.setBold(true);
		painter.setFont(Font);
		Tmp.setf("%s %u", (const char*)GetDeviceNameShort(DeviceType), DeviceId);
		//Rect.setRect ((int)(60*w), (int)(4*h), (int)(119*w),(int)(80*h));
		//bbox=painter.boundingRect ( Rect,Qt::AlignLeft|Qt::AlignTop|Qt::TextSingleLine,Tmp);
		painter.drawText((int)(65 * w), (int)(8 * h), Tmp);
		Font.setPointSize(10);
		Font.setBold(false);
		painter.setFont(Font);
		painter.drawText((int)(65 * w), (int)(25 * h), tr("Title:"));
		painter.drawText((int)(65 * w), (int)(40 * h), tr("Tracks:"));
		painter.drawText((int)(65 * w), (int)(45 * h), tr("Length:"));
		Font.setPointSize(10);
		Font.setBold(true);
		painter.setFont(Font);
		y=25;
		if (device->Title) {
			painter.drawText((int)(82 * w), (int)(y * h), device->Title);
			y+=5;
		}
		if (device->SubTitle) {
			painter.drawText((int)(82 * w), (int)(y * h), device->SubTitle);
			y+=5;
		}
		Tmp.setf("%u", device->NumTracks);
		painter.drawText((int)(82 * w), (int)(40 * h), Tmp);
		Tmp.setf("%i:%02i ", (int)(device->Recorded / 60), device->Recorded % 60);
		Tmp+=tr("min.", "Shortcut für Minutes on Cover");
		painter.drawText((int)(82 * w), (int)(45 * h), Tmp);

		// Side
		painter.save();
		Font.setPointSize(14);
		Font.setBold(true);
		painter.setFont(Font);
		painter.rotate(90.0);
		Tmp.setf("%s %u", (const char*)GetDeviceNameShort(DeviceType), DeviceId);
		painter.drawText((int)(123 * h), (int)(-146 * w), Tmp);
		Font.setPointSize(10);
		Font.setBold(false);
		painter.setFont(Font);
		y=-120;
		if (device->Title) {
			painter.drawText((int)(160 * h), (int)(-147 * w), device->Title);
		}
		painter.restore();
		QPixmap Icon=GetDevicePixmap(DeviceType);
		if (Icon.width() > 0) {
			painter.save();
			Rect.setRect((int)(151 * w), (int)(230 * h), (int)(5 * h), (int)(5 * w));
			QSize size=Icon.size();
			size.scale(Rect.size(), Qt::KeepAspectRatio);
			painter.setViewport(Rect.x(), Rect.y(), size.width(), size.height());
			painter.setWindow(Icon.rect());
			painter.rotate(90.0);
			painter.drawPixmap(0, 0, Icon);
			painter.restore();
		}

		// Backside
		Font.setPointSize(20);
		Font.setBold(true);
		painter.setFont(Font);
		Tmp.setf("%s %u", (const char*)GetDeviceNameShort(DeviceType), DeviceId);
		painter.drawText((int)(10 * w), (int)(128 * h), Tmp);
		Font.setPointSize(10);
		Font.setBold(true);
		painter.setFont(Font);
		Tmp.clear();
		if (device->Title) Tmp=device->Title;
		if (device->SubTitle) {
			if (Tmp.notEmpty()) Tmp+=", ";
			Tmp+=device->SubTitle;
		}
		painter.drawText((int)(10 * w), (int)(135 * h), Tmp);

		// Tracklisting
		int num=tracklist.Num();
		x=(int)(10 * w);
		y=(int)(140 * h);
		// Wir müssen ein paar entscheidungen anhand der Anzahl Tracks treffen
		int columns=1;
		int maxrows=0;
		if (num < 36) {
			Font.setPointSize(6);
			maxrows=35;
			painter.setClipRect((int)(x - w), (int)(y - h * 2), (int)(130 * w), (int)(92 * h), Qt::ReplaceClip);

		} else if (num <= 80) {
			Font.setPointSize(5);
			columns=2;
			maxrows=40;
			painter.setClipRect((int)(x - w), (int)(y - h * 2), (int)(65 * w), (int)(92 * h), Qt::ReplaceClip);
		} else {
			Font.setPointSize(4);
			columns=2;
			maxrows=50;
			painter.setClipRect((int)(x - w), (int)(y - h * 2), (int)(65 * w), (int)(92 * h), Qt::ReplaceClip);
		}

		Font.setBold(false);
		painter.setFont(Font);
		// Zeilenabstand berechnen
		bbox=painter.boundingRect(Rect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, "WÄgelchen");
		int rowheight=bbox.height();
		int row=0;
		for (int Track=tracklist.GetMin();Track <= tracklist.GetMax();Track++) {
			const DataTrack* t=tracklist.GetPtr(Track);
			if (t) {
				const DataTitle* ti=GetTitle(t->TitleId);
				if (ti) {
					Tmp.setf("%u.", Track);
					painter.drawText(x, y, Tmp);
					Tmp.setf("%s - %s ",
						(ti->Artist.notEmpty() ? (const char*)ti->Artist : "?"),
						(ti->Title.notEmpty() ? (const char*)ti->Title : "?"));
					bbox=painter.boundingRect(Rect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, Tmp);
					painter.drawText((int)(x + 4 * w), y, Tmp);
					Tmp.setf("(%i.%02i ", (int)(ti->Length / 60), ti->Length % 60);
					Tmp+=tr("min.", "Shortcut für Minutes on Cover");
					tmp=GetVersionText(ti->VersionId);
					if (tmp) Tmp.appendf(", %s", tmp);
					Tmp+=")";
					painter.setPen(RedPen);
					painter.drawText((int)(x + 4 * w + bbox.width()), y, Tmp);
					painter.setPen(Pen);
					y+=rowheight;
					row++;
					if (row >= maxrows && columns > 1) {
						row=0;
						y=(int)(140 * h);
						x=(int)(80 * w);
						painter.setClipRect((int)(x - w), (int)(y - h * 2), (int)(65 * w), (int)(92 * h), Qt::ReplaceClip);
					} else if (row >= maxrows) {
						break;
					}
				}
			}
		}
		painter.setClipRect((int)(x - w), (int)(y - h), (int)(65 * w), (int)(90 * h), Qt::NoClip);



		// Disclaimer
		Font.setPointSize(6);
		Font.setBold(false);
		painter.setFont(Font);
		Tmp=tr("Printed by WinMusik");
		Tmp.appendf(" %s, %s", WM_VERSION, WM_COPYRIGHT);
		painter.drawText((int)(10 * w), (int)(236 * h), Tmp);



	}
	painter.end();



	return 1;
}

int CWmClient::PrintTracklistTableHeader(QFont& Font, QPainter& painter, int rx, int ry)
/*!\brief Tracklist Tabellenkopf drucken
 *
 * \return Liefert die Höhe der Zeile im Device-spezifischen Format zurück. Der Wert muss
 * zur y-Koordinate dazuaddiert werden, um zur Position der nächsten Zeile zu gelangen.
 */
{
	QRect bbox, Rect;
	const QPaintDevice* device=painter.device();

	float w=(float)device->width() / (float)device->widthMM();
	float h=(float)device->height() / (float)device->heightMM();
	int rowheight;
	// Tabellenkopf
	Font.setPointSize(8);
	Font.setBold(true);
	painter.setFont(Font);
	bbox=painter.boundingRect(Rect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, "WÄgelchen");
	rowheight=bbox.height();
	QBrush TH(QColor(230, 230, 230), Qt::SolidPattern);
	QBrush NormalBrush(Qt::black, Qt::NoBrush);


	painter.setBrush(TH);
	painter.drawRect((int)(rx - (1 * w)), (int)(ry - (1 * h)), (int)(182 * w), (int)(rowheight + 2 * h));
	painter.setBrush(NormalBrush);
	//Rect.setCoords ((x)*w,y*h,(x+10)*w,y*h+rowheight);
	painter.drawText(QRect(rx, ry, (int)(10 * w), rowheight), Qt::AlignLeft, tr("Track"));
	painter.drawText(QRect((int)(rx + (10 * w)), ry, (int)(80 * w), rowheight), Qt::AlignLeft, tr("Artist - Title"));
	painter.drawText(QRect((int)(rx + (90 * w)), ry, (int)(50 * w), rowheight), Qt::AlignLeft, tr("Version"));
	painter.drawText(QRect((int)(rx + (140 * w)), ry, (int)(20 * w), rowheight), Qt::AlignLeft, tr("Genre"));
	painter.drawText(QRect((int)(rx + (160 * w)), ry, (int)(20 * w), rowheight), Qt::AlignLeft, tr("Length"));
	return (int)(rowheight + 2 * h);
}

int CWmClient::PrintTracklistDisclaimer(QFont& Font, QPainter& painter)
{
	ppl7::String Tmp;
	const QPaintDevice* device=painter.device();

	float w=(float)device->width() / (float)device->widthMM();
	float h=(float)device->height() / (float)device->heightMM();

	// Disclaimer
	Font.setPointSize(7);
	Font.setBold(false);
	painter.setFont(Font);
	Tmp=tr("Printed by WinMusik");
	Tmp.appendf(" %s, %s", WM_VERSION, WM_COPYRIGHT);
	painter.drawText((int)(15 * w), (int)(device->height() - 5 * h), Tmp);
	return 1;
}

int CWmClient::PrintTracklistDeviceHeader(QFont& Font, QPainter& painter, int x, int y, const DataDevice* device)
{
	ppl7::String Tmp;
	QRect Rect;
	const QPaintDevice* paintdevice=painter.device();
	QBrush NormalBrush(Qt::black, Qt::NoBrush);
	QPen Pen;  // creates a default pen
	Pen.setStyle(Qt::SolidLine);
	Pen.setWidth(1);
	Pen.setBrush(Qt::black);

	painter.setPen(Pen);
	painter.setBrush(NormalBrush);


	float w=(float)paintdevice->width() / (float)paintdevice->widthMM();
	float h=(float)paintdevice->height() / (float)paintdevice->heightMM();

	Font.setPointSize(20);
	Font.setBold(true);
	painter.setFont(Font);
	Tmp.setf("%s %u", (const char*)GetDeviceName(device->DeviceType), device->DeviceId);
	//Rect.setRect ((int)(60*w), (int)(4*h), (int)(119*w),(int)(80*h));
	//bbox=painter.boundingRect ( Rect,Qt::AlignLeft|Qt::AlignTop|Qt::TextSingleLine,Tmp);
	painter.drawText((int)(x * w), (int)(y * h), Tmp);
	Font.setPointSize(10);
	Font.setBold(false);
	painter.setFont(Font);
	painter.drawText((int)(x * w), (int)((y + 10) * h), tr("Title:"));
	painter.drawText((int)(x * w), (int)((y + 20) * h), tr("Tracks:"));
	if (device->Pages > 1) painter.drawText((int)((x + 50) * w), (int)((y + 20) * h), tr("Pages:"));
	painter.drawText((int)(x * w), (int)((y + 25) * h), tr("Length:"));
	painter.drawText((int)((x + 50) * w), (int)((y + 25) * h), tr("Recording timescale:"));
	Font.setPointSize(10);
	Font.setBold(true);
	painter.setFont(Font);
	int y1=y + 10;
	if (device->Title) {
		painter.drawText((int)((x + 20) * w), (int)(y1 * h), device->Title);
		y1+=5;
	}
	if (device->SubTitle) {
		painter.drawText((int)((x + 20) * w), (int)(y1 * h), device->SubTitle);
	}
	Tmp.setf("%u", device->NumTracks);
	painter.drawText((int)((x + 20) * w), (int)((y + 20) * h), Tmp);
	if (device->Pages > 1) {
		Tmp.setf("%u", device->Pages);
		painter.drawText((int)((x + 70) * w), (int)((y + 20) * h), Tmp);
	}
	ppl7::String FirstDate="?", LastDate="?", DateFormat=tr("%d.%m.%Y", "Date format");
	if (device->FirstDate) FirstDate=Long2Date(DateFormat, device->FirstDate);
	if (device->LastDate) LastDate=Long2Date(DateFormat, device->LastDate);
	Tmp.setf("%s - %s", (const char*)FirstDate, (const char*)LastDate);
	painter.drawText((int)((x + 90) * w), (int)((y + 25) * h), Tmp);


	Tmp.setf("%i:%02i ", (int)(device->Recorded / 60), device->Recorded % 60);
	Tmp+=tr("min.", "Shortcut für Minutes on Cover");
	painter.drawText((int)((x + 20) * w), (int)((y + 25) * h), Tmp);
	QPixmap Icon=GetDevicePixmap(device->DeviceType);
	if (Icon.width() > 0) {
		painter.save();
		Rect.setRect((int)((x + 150) * w), (int)((y + 0) * h), (int)(20 * h), (int)(20 * w));
		QSize size=Icon.size();
		size.scale(Rect.size(), Qt::KeepAspectRatio);
		painter.setViewport(Rect.x(), Rect.y(), size.width(), size.height());
		painter.setWindow(Icon.rect());
		painter.drawPixmap(0, 0, Icon);
		painter.restore();
	}
	return 35;
}

int CWmClient::PrintTracklist(QWidget* parent, int DeviceType, uint32_t start, uint32_t end)
{
	ppl7::String Tmp;
	QPrinter Printer(QPrinter::HighResolution);
	if (conf.bPrintColors) Printer.setColorMode(QPrinter::Color);
	else Printer.setColorMode(QPrinter::GrayScale);
	//Printer.setPrinterName(conf.PrinterName);
	conf.DefaultPrintPath.trim();
	if (conf.DefaultPrintPath.isEmpty()) conf.DefaultPrintPath=QDir::homePath();
	ppl7::String Filename=conf.DefaultPrintPath;
	Filename.appendf("/");
	Filename+=tr("WinMusik Tracklist", "Default Filename in Printdialog");
	if (start != end) Filename.appendf(" - %s %i-%i-Tracklist.pdf", (const char*)GetDeviceNameShort(DeviceType), start, end);
	else Filename.appendf(" - %s %i-Tracklist.pdf", (const char*)GetDeviceNameShort(DeviceType), start);

	Printer.setOutputFileName(Filename);

	QPrintDialog printDialog(&Printer, parent);

	if (printDialog.exec() != QDialog::Accepted) return 0;

	// Save Options
	conf.DefaultPrintPath=ppl7::File::getPath(Printer.outputFileName());
	conf.PrinterName=Printer.printerName();
	if (Printer.colorMode() == QPrinter::Color) conf.bPrintColors=true;
	else conf.bPrintColors=false;
	conf.trySave();

	// Es kann losgehen, wir treffen einige Vorbereitungen und setzen Defaults
	QPainter painter;
	QBrush Brush;
	QBrush Red;
	QRect Rect;
	QRect bbox;
	int x, y;

	Brush.setColor(Qt::black);
	Brush.setStyle(Qt::NoBrush);



	Red.setColor(Qt::red);
	Red.setStyle(Qt::NoBrush);

	QPen Pen;  // creates a default pen
	Pen.setStyle(Qt::SolidLine);
	Pen.setWidth(1);
	Pen.setBrush(Qt::black);

	QPen RedPen;  // creates a default pen
	RedPen.setStyle(Qt::SolidLine);
	RedPen.setWidth(1);
	RedPen.setBrush(Qt::red);

	if (!painter.begin(&Printer)) {
		RaiseError(NULL, tr("Druckvorgang kann nicht gestartet werden"));
		return 0;
	}
	float w=(float)Printer.width() / (float)Printer.widthMM();
	float h=(float)Printer.height() / (float)Printer.heightMM();
	QFont Font;
	Font.setFamily(conf.PrinterFont);

	x=15;
	y=10;
	// Nun können wir der Reihe nach alle Cover drucken
	for (uint32_t DeviceId=start;DeviceId <= end;DeviceId++) {
		if (DeviceId > start) {
			Printer.newPage();
			x=15;
			y=10;

		}
		painter.setPen(Pen);
		painter.setBrush(Brush);

		// Tonträger laden
		const DataDevice* device=DeviceStore.GetPtr(DeviceType, DeviceId);
		if (!device) continue;

		y+=PrintTracklistDeviceHeader(Font, painter, x, y, device);
		int rx=(int)(x * w);
		int ry=(int)(y * h);
		int rowheight;
		for (int page=1;page <= device->Pages;page++) {
			CTrackList tracklist=GetTracklist(DeviceType, DeviceId, page);
			if (device->Pages > 0) {
				Font.setPointSize(12);
				Font.setBold(true);
				painter.setFont(Font);
				Tmp=tr("Page");
				Tmp.appendf(" %i:", page);
				painter.drawText(QRect(rx, ry, (int)(50 * w), (int)(10 * h)), Qt::AlignLeft, Tmp);

				ry+=(int)(10 * h);
			}
			if (tracklist.Num() > 0) {
				// Tabellenkopf
				ry+=PrintTracklistTableHeader(Font, painter, rx, ry);



				Font.setPointSize(8);
				Font.setBold(false);
				painter.setFont(Font);
				bbox=painter.boundingRect(Rect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, "WÄge|ßchen§");
				rowheight=bbox.height();
				CTrackList::const_iterator it;
				for (it=tracklist.begin();it != tracklist.end();++it) {
					const DataTrack& t=it->second;
					const DataTitle* ti=GetTitle(t.TitleId);
					if (ti) {
						Tmp.setf("%u.", t.Track);
						painter.drawText(QRect(rx, ry, (int)(10 * w), rowheight), Qt::AlignLeft, Tmp);
						Tmp.setf("%s - %s ",
							(ti->Artist.notEmpty() ? (const char*)ti->Artist : "?"),
							(ti->Title.notEmpty() ? (const char*)ti->Title : "?"));
						painter.drawText(QRect(rx + (int)(10 * w), ry, (int)(80 * w), rowheight), Qt::AlignLeft, Tmp);
						painter.setPen(RedPen);
						Tmp=GetVersionText(ti->VersionId);
						painter.drawText(QRect(rx + (int)(90 * w), ry, (int)(50 * w), rowheight), Qt::AlignLeft, Tmp);
						painter.setPen(Pen);
						Tmp=GetGenreText(ti->GenreId);
						painter.drawText(QRect(rx + (int)(140 * w), ry, (int)(20 * w), rowheight), Qt::AlignLeft, Tmp);
						Tmp.setf("%i.%02i ", (int)(ti->Length / 60), ti->Length % 60);
						Tmp+=tr("min.", "Shortcut für Minutes on Cover");
						painter.drawText(QRect(rx + (int)(160 * w), ry, (int)(20 * w), rowheight), Qt::AlignRight, Tmp);
						ry+=rowheight;
						if (ry > Printer.height() - (15 * h)) {
							// Neue Seite
							PrintTracklistDisclaimer(Font, painter);
							Printer.newPage();
							x=15;
							y=10;
							y+=PrintTracklistDeviceHeader(Font, painter, x, y, device);
							rx=(int)(x * w);
							ry=(int)(y * h);
							if (device->Pages > 0) {
								Font.setPointSize(12);
								Font.setBold(true);
								painter.setFont(Font);
								Tmp=tr("Page");
								Tmp.appendf(" %i:", page);
								painter.drawText(QRect(rx, ry, (int)(50 * w), (int)(10 * h)), Qt::AlignLeft, Tmp);

								ry+=(int)(10 * h);
							}
							ry+=PrintTracklistTableHeader(Font, painter, rx, ry);

							Font.setPointSize(8);
							Font.setBold(false);
							painter.setFont(Font);
						}
					}
				}
			}
			ry+=(int)(5 * h);
		}
	}
	PrintTracklistDisclaimer(Font, painter);
	painter.end();


	return 1;
}
