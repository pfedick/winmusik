#include "../include/printdevicedialog.h"

PrintDeviceDialog::PrintDeviceDialog(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
}

PrintDeviceDialog::~PrintDeviceDialog()
{

}

void PrintDeviceDialog::setDeviceType(int Type)
{
	if (Type==2 || Type==7) {
		ui.printCoverButton->setEnabled(true);
		ui.printCoverButton->setChecked(true);
	} else {
		ui.printCoverButton->setEnabled(false);
		ui.printTracklistButton->setChecked(true);

	}
}
