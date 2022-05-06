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


#include "winmusik3.h"
#include "coverprinter.h"

CoverPrinter::CoverPrinter(QWidget* parent, CWmClient* wm)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->wm=wm;
	Printer=new QPrinter(QPrinter::HighResolution);
	//QSettings settings("Patrick F.-Productions","CoverPrinter");
	//CoverPath=settings.value("coverpath","").toString();
}

CoverPrinter::~CoverPrinter()
{
	delete Printer;
	if (wm) wm->CoverPrinterClosed(this);
}


void CoverPrinter::ReloadTranslation()
{
	ui.retranslateUi(this);
}


/*
void CoverPrinter::on_front_cover_dragEnterEvent(QDragEnterEvent *event)
 {
	 if (event->mimeData()->hasImage())
		 event->acceptProposedAction();
 }

void CoverPrinter::on_front_cover_dropEvent(QDropEvent *event)
{
	ppl6::CString s;
	if (event->mimeData()->hasImage()) ui.front_cover->setText(s);

}

void CoverPrinter::on_cover_dragEnterEvent(QDragEnterEvent *event)
 {
	 //if (event->mimeData()->hasImage())
		 event->acceptProposedAction();
 }

void CoverPrinter::on_cover_dropEvent(QDropEvent *event)
{


}
*/

static const char* chan[]=
{
		"",
		"Mono",
		"Stereo",
		"Dolby Surround",
		"Dolby Digital 1.0",
		"Dolby Digital 2.0",
		"Dolby Digital 5.1",
		"Dolby Digital 5.1ex",
		"DTS 5.1",
		""
};

void CoverPrinter::on_type_currentIndexChanged(int)
{
	if (ui.type->currentIndex() < 6) {
		ui.InfoBox->setEnabled(true);
		ui.TonBox->setEnabled(true);
	} else {
		ui.InfoBox->setEnabled(false);
		ui.TonBox->setEnabled(false);
	}
}

void CoverPrinter::on_loadcover_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Select cover"), wm->conf.CoverPath, tr("Grafik-Dateien (*.png *.jpg *.bmp *.gif *.tif)"));
	ui.cover_file->setText(fileName);
	if (fileName.isEmpty()) {
		pix=QPixmap();
		ui.cover->setPixmap(pix);
		return;
	}
	ppl7::String s=ppl7::File::getPath(fileName);
	if (s != wm->conf.CoverPath) {
		wm->conf.CoverPath=s;
		wm->conf.trySave();
	}
	pix.load(fileName);
	ui.cover->setPixmap(pix);

}

void CoverPrinter::on_unbekannt1_stateChanged(int state)
{
	if (state == Qt::Unchecked) ui.erscheinungsjahr->setEnabled(true);
	else ui.erscheinungsjahr->setEnabled(false);
}

void CoverPrinter::on_unbekannt2_stateChanged(int state)
{
	if (state == Qt::Unchecked) ui.aufnahmedatum->setEnabled(true);
	else ui.aufnahmedatum->setEnabled(false);
}

void CoverPrinter::on_printOptions_clicked()
{
	QPrintDialog printDialog(Printer, this);
	//Printer.setOutputFileName("cover.pdf");
	if (printDialog.exec() == QDialog::Accepted) {
		QPainter painter;
		QBrush Brush;
		QRect Rect;
		QRect bbox;
		bool haveFrontTitle=false;

		Brush.setColor(Qt::black);
		Brush.setStyle(Qt::NoBrush);

		QPen Pen;  // creates a default pen
		Pen.setStyle(Qt::SolidLine);
		Pen.setWidth(1);
		Pen.setBrush(Qt::black);
		//pen.setCapStyle(Qt::RoundCap);

		painter.begin(Printer);
		float w=(float)Printer->width() / (float)Printer->widthMM();
		float h=(float)Printer->height() / (float)Printer->heightMM();

		//s.Setf("Width: %i x %i Pixel = %i x %i mm",Printer->width(),Printer->height(),Printer->widthMM(),Printer->heightMM());
		//QMessageBox::information(this, tr("CoverPrinter"),s,QMessageBox::Ok);


		QFont Font;
		Font.setFamily(wm->conf.PrinterFont);

		//Font.setKerning(true);
		//Font.setWeight(QFont::Bold);
		//Font.setBold(true);
		//Font.setFixedPitch(false);

		// Front Titel
		int ps=30;
		QString qs=ui.front_title->text();
		if (!qs.isEmpty()) {
			Rect.setRect((int)(60 * w), (int)(4 * h), (int)(119 * w), (int)(80 * h));
			while (ps > 12) {
				Font.setPointSize(ps);
				Font.setBold(true);
				painter.setFont(Font);
				bbox=painter.boundingRect(Rect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, qs);
				if (bbox.width() < 119 * w) break;
				//painter.drawRect(bbox);
				ps--;
			}
			painter.drawText((int)(60 * w + (Rect.width() - bbox.width()) / 2), (int)(4 * h + bbox.height()), qs);
			haveFrontTitle=true;
		}

		ps=30;
		qs=ui.back_title->text();
		Rect.setRect((int)(10 * w), (int)(120 * h), (int)(130 * w), (int)(25 * h));
		while (ps > 12) {
			Font.setPointSize(ps);
			Font.setBold(true);
			painter.setFont(Font);
			bbox=painter.boundingRect(Rect, Qt::AlignLeft | Qt::AlignTop | Qt::TextSingleLine, qs);
			if (bbox.width() < 119 * w) break;
			//painter.drawRect(bbox);
			ps--;
		}
		painter.drawText((int)(10 * w), (int)(120 * h + bbox.height()), qs);

		// Text

		Font.setPointSize(ui.back_fontsize->text().toInt());
		Font.setBold(false);
		painter.setFont(Font);
		qs=ui.back_text->toPlainText();
		Rect.setRect((int)(10 * w), (int)(140 * h), (int)(130 * w), (int)(70 * h));
		painter.drawText(Rect, Qt::AlignJustify | Qt::AlignTop | Qt::TextWordWrap, qs);


		// Hilfslinien
		painter.setPen(Pen);
		painter.setBrush(Brush);
		painter.drawRect(0, 0, (int)(181 * w), (int)(120 * h));
		painter.drawLine((int)(60 * w), 0, (int)(60 * w), (int)(120 * h));

		painter.drawRect(0, (int)(120 * h), (int)(151 * w), (int)(118 * h));
		painter.drawLine((int)(6 * w), (int)(120 * h), (int)(6 * w), (int)(238 * h));
		painter.drawLine((int)(145 * w), (int)(120 * h), (int)(145 * w), (int)(238 * h));

		// Cover anzeigen
		if (pix.width() > 0) {
			painter.save();
			if (haveFrontTitle) Rect.setRect((int)(62 * w), (int)(20 * h), (int)(117 * w), (int)(98 * h));
			else Rect.setRect((int)(62 * w), (int)(5 * h), (int)(117 * w), (int)(110 * h));
			QSize size=pix.size();
			size.scale(Rect.size(), Qt::KeepAspectRatio);
			painter.setViewport(Rect.x() + (Rect.width() - size.width()) / 2, Rect.y(), size.width(), size.height());
			//painter.setViewport(60*w,20*h,size.width(),size.height());
			painter.setWindow(pix.rect());
			//painter.drawPixmap((Rect.width()-size.width())/2,0,pix);
			painter.drawPixmap(0, 0, pix);
			painter.restore();
		}

		// Seite beschriften
		painter.save();
		Font.setPointSize(14);
		Font.setBold(true);
		painter.setFont(Font);
		qs=ui.side_title->text();
		//painter.setViewport(145*w,120*h,6*w,115*h);
		painter.rotate(90.0);
		painter.drawText((int)(123 * h), (int)(-146 * w), qs);

		QPixmap Type;
		switch (ui.type->currentIndex()) {
		case 0:
			Type.load(":/cover/resources/coverdruck/print-vcd.png");
			break;
		case 1:
			Type.load(":/cover/resources/coverdruck/print-dvd.png");
			break;
		case 2:
			Type.load(":/cover/resources/coverdruck/print-dvd-5.1.png");
			break;
		case 3:
			Type.load(":/cover/resources/coverdruck/print-blueray.png");
			break;
		case 4:
			Type.load(":/cover/resources/coverdruck/print-divx.png");
			break;
		case 5:
			Type.load(":/cover/resources/coverdruck/print-xvid.png");
			break;
		case 6:
			Type.load(":/cover/resources/coverdruck/print-dvd2.png");
			break;
		case 7:
			Type.load(":/cover/resources/coverdruck/print-cd.png");
			break;
		}
		painter.restore();
		if (Type.width() > 0) {
			painter.save();
			Rect.setRect((int)(150 * w), (int)(223 * h), (int)(10 * h), (int)(5 * w));
			QSize size=Type.size();
			size.scale(Rect.size(), Qt::KeepAspectRatio);
			painter.setViewport(Rect.x(), Rect.y(), size.width(), size.height());
			painter.setWindow(Type.rect());
			painter.rotate(90.0);
			painter.drawPixmap(0, 0, Type);
			painter.restore();

			//painter.drawPixmap(224*h,-150*w,Type);
		}

		if (ui.type->currentIndex() < 6) {
			// Technische Daten
			Font.setPointSize(8);
			Font.setBold(true);
			painter.setFont(Font);
			Brush.setStyle(Qt::SolidPattern);
			Brush.setColor(QColor(240, 240, 240, 255));
			painter.setBrush(Brush);
			painter.drawRect((int)(10 * w), (int)(210 * h), (int)(110 * w), (int)(24 * h));
			Brush.setColor(Qt::lightGray);
			painter.setBrush(Brush);
			painter.drawRect((int)(10 * w), (int)(210 * h), (int)(110 * w), (int)(4 * h));
			painter.drawText((int)(12 * w), (int)(213 * h), "Technische Daten:");
			Font.setPointSize(7);
			Font.setBold(true);
			painter.setFont(Font);
			painter.drawText((int)(12 * w), (int)(218 * h), "Sprache:");
			painter.drawText((int)(12 * w), (int)(222 * h), "Bildformat:");
			painter.drawText((int)(12 * w), (int)(226 * h), "Länge:");
			painter.drawText((int)(12 * w), (int)(230 * h), "Erscheinungsjahr:");
			Font.setBold(false);
			painter.setFont(Font);
			painter.drawText((int)(40 * w), (int)(222 * h), ui.bildformat->currentText());
			qs=ui.laenge->text();
			if (qs.length() > 0) {
				qs+=" Minuten";
				painter.drawText((int)(40 * w), (int)(226 * h), qs);
			}
			if (!ui.unbekannt1->isChecked()) painter.drawText((int)(40 * w), (int)(230 * h), ui.erscheinungsjahr->date().toString("yyyy"));

			// Quelle
			qs=ui.quelle->text();
			if (qs.length() > 0) {
				Font.setBold(true);
				painter.setFont(Font);
				painter.drawText((int)(60 * w), (int)(226 * h), "Quelle:");
				Font.setBold(false);
				painter.setFont(Font);
				painter.drawText((int)(70 * w), (int)(226 * h), qs);
			}
			// Sprachen
			QString s;
			qs=ui.tonspur1->currentText();
			if (qs.length() > 0) {
				s+=qs;
				s+=" (";
				s+=chan[ui.ton1->currentIndex()];
				s+=")";
			}
			qs=ui.tonspur2->currentText();
			if (qs.length() > 0) {
				if (s.length() > 0) s+=", ";
				s+=qs;
				s+=" (";
				s+=chan[ui.ton2->currentIndex()];
				s+=")";
			}
			qs=ui.tonspur3->currentText();
			if (qs.length() > 0) {
				if (s.length() > 0) s+=", ";
				s+=qs;
				s+=" (";
				s+=chan[ui.ton3->currentIndex()];
				s+=")";
			}
			painter.drawText((int)(40 * w), (int)(218 * h), s);
		}
		painter.end();



	}
}
