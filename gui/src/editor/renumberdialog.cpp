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

#include "winmusik3.h"
#include <QMenu>
#include <QMessageBox>
#include "renumberdialog.h"

RenumberDialog::RenumberDialog(QWidget* parent, CWmClient* wm)
    : QDialog(parent)
{
    ui.setupUi(this);
    this->wm = wm;
    ui.newNumber->setFocus();
}

RenumberDialog::~RenumberDialog()
{
}

void RenumberDialog::setOldNumber(int number)
{
    ui.oldNumber->setText(pplib::ToString("%i", number));
}

void RenumberDialog::setNewNumber(int number)
{
    ui.newNumber->setText(pplib::ToString("%i", number));
}

int RenumberDialog::getOldNumber() const
{
    return ui.oldNumber->text().toInt();
}

int RenumberDialog::getNewNumber() const
{
    return ui.newNumber->text().toInt();
}

void RenumberDialog::on_okButton_clicked()
{
    done(1);
}

void RenumberDialog::on_cancelButton_clicked()
{
    done(0);
}

bool RenumberDialog::isTargetPathUsable(uint8_t DeviceType, uint32_t newDeviceId) const
{
    // Falls Filesystem verwendet wird, darf die neue Nummer nicht existieren oder
    // das Verzeichnis muss leer sein

    pplib::String Tmp;
    pplib::String Path = wm->GetAudioPath(DeviceType, newDeviceId, 1);
    Path.trimRight("/");
    pplib::DirEntry de;
    if (pplib::File::tryStatFile(Path, de)) {
        if (!de.isDir()) {
            Tmp = tr("A file with the name of the new device already exists and is not a directory.\n\n");
            Tmp += Path;
            QMessageBox::critical(NULL, tr("Error"), Tmp, QMessageBox::Ok, QMessageBox::Ok);
            return false;
        }
        pplib::Dir dir;
        if (!dir.tryOpen(Path)) {
            // Das dürfte nicht passieren
            Tmp = tr("Directory with the name of the new device already exists, but I can't open it. Wrong permissions?\n\n");
            Tmp += Path;
            QMessageBox::critical(NULL, tr("Error"), Tmp, QMessageBox::Ok, QMessageBox::Ok);
            return false;
        }
        int count = 0;
        for (const pplib::DirEntry& e : dir) {
            if (e.Filename != "." && e.Filename != "..") {
                count++;
                break;
            }
        }
        if (count > 0) {
            Tmp = tr("Directory with the name of the new device already exists and is not empty.\n\n");
            Tmp += Path;
            QMessageBox::critical(NULL, tr("Error"), Tmp, QMessageBox::Ok, QMessageBox::Ok);
            return false;
        }
    }
    return true;
}

bool RenumberDialog::isTargetDeviceFree(uint8_t DeviceType, uint32_t newDeviceId) const
{
    pplib::String Tmp;
    DataDevice targetDevice;
    if (wm->LoadDevice(DeviceType, newDeviceId, &targetDevice)) {
        Tmp = tr("Device with the new number already exists and cannot be overwritten.\n\n");
        Tmp += tr("DeviceId: ");
        Tmp.appendf("%u", newDeviceId);
        QMessageBox::critical(NULL, tr("Error"), Tmp, QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    return true;
}
