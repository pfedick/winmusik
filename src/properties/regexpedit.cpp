/*
 * This file is part of WinMusik 3 by Patrick Fedick
 *
 * $Author: pafe $
 * $Revision: 1.5 $
 * $Date: 2010/10/10 10:31:01 $
 * $Id: properties.cpp,v 1.5 2010/10/10 10:31:01 pafe Exp $
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


#include "winmusik3.h"

#include "properties.h"
#include "regexpedit.h"
#include <QClipboard>

RegExpEdit::RegExpEdit(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
}

RegExpEdit::~RegExpEdit()
{

}

int RegExpEdit::checkValues()
{
	return 1;
}

void RegExpEdit::on_cancelButton_clicked()
{
	done(0);
}

void RegExpEdit::on_okButton_clicked()
{
	if (!checkValues()) return;
	done(1);
}

void RegExpEdit::setPattern(const RegExpPattern &pat)
{
	ui.capture_artist->setValue(pat.artist);
	ui.capture_title->setValue(pat.title);
	ui.capture_version->setValue(pat.version);
	ui.capture_genre->setValue(pat.genre);
	ui.capture_label->setValue(pat.label);
	ui.capture_bpm->setValue(pat.bpm);
	ui.capture_album->setValue(pat.album);
	ui.capture_hours->setValue(pat.hours);
	ui.capture_minutes->setValue(pat.minutes);
	ui.capture_seconds->setValue(pat.seconds);
	ui.capture_releasedate->setValue(pat.releasedate);
	ui.isHtmlCheckBox->setChecked(pat.isHTML);
	ui.name->setText(pat.Name);
	ui.pattern->setText(pat.Pattern);
}

RegExpPattern RegExpEdit::getPattern() const
{
	RegExpPattern pat;
	pat.Name=ui.name->text();
	pat.Pattern=ui.pattern->text();
	pat.artist=ui.capture_artist->value();
	pat.title=ui.capture_title->value();
	pat.version=ui.capture_version->value();
	pat.genre=ui.capture_genre->value();
	pat.label=ui.capture_label->value();
	pat.bpm=ui.capture_bpm->value();
	pat.album=ui.capture_album->value();
	pat.hours=ui.capture_hours->value();
	pat.minutes=ui.capture_minutes->value();
	pat.seconds=ui.capture_seconds->value();
	pat.releasedate=ui.capture_releasedate->value();
	pat.isHTML=ui.isHtmlCheckBox->isChecked();
	return pat;
}


void RegExpEdit::on_pattern_textChanged()
{
	on_teststring_textChanged();
}

void RegExpEdit::on_teststring_textChanged()
{
	RegExpMatch match;
	RegExpPattern pat=getPattern();
	ppl6::CString Text=ui.teststring->toPlainText();
	if (pat.Pattern.NotEmpty()==true && Text.NotEmpty()==true) {
		try {
			if (wm_main->RegExpCapture.testMatch(Text,match,pat)) {
				ui.match_indicator->setStyleSheet("background: green;");
				ui.test_artist->setText(match.Artist);
				ui.test_title->setText(match.Title);
				ui.test_version->setText(match.Version);
				ui.test_genre->setText(match.Genre);
				ui.test_label->setText(match.Label);
				ui.test_bpm->setText(match.Bpm);
				ui.test_album->setText(match.Album);
				ui.test_releasedate->setText(match.ReleaseDate);
				ppl6::CString Tmp;
				Tmp.Setf("%i:%02i",match.Length/60,match.Length%60);
				ui.test_length->setText(Tmp);
				return;
			}
		} catch (...) {

		}
	}
	ui.match_indicator->setStyleSheet("background: red;");
	ui.test_artist->setText("");
	ui.test_title->setText("");
	ui.test_version->setText("");
	ui.test_genre->setText("");
	ui.test_label->setText("");
	ui.test_bpm->setText("");
	ui.test_album->setText("");
	ui.test_length->setText("");
	ui.test_releasedate->setText("");
}

void RegExpEdit::on_copyPlaintext_clicked()
{
	QString Text=QApplication::clipboard()->text();
	ui.teststring->setPlainText(Text);
}

void RegExpEdit::on_copyHTML_clicked()
{
	QString format="html";
	QString Text=QApplication::clipboard()->text(format);
	ui.teststring->setPlainText(Text);
}
