#ifndef PRINTDEVICEDIALOG_H
#define PRINTDEVICEDIALOG_H

#include "winmusik3.h"
#include <QtGui/QDialog>
#include "ui_printdevicedialog.h"

class PrintDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    PrintDeviceDialog(QWidget *parent = 0);
    ~PrintDeviceDialog();

    void setDeviceRange(int min, int max) {
    	ui.fromDevice->setMinimum(min);
    	ui.toDevice->setMinimum(min);
    	ui.fromDevice->setMaximum(max);
    	ui.toDevice->setMaximum(max);
    }

    void setUseColor(bool flag) {
    	ui.printColors->setChecked(flag);
    }

    void setFont(ppl6::CString Font) {
    	int index=ui.font->findText(Font);
    	if (index>=0) ui.font->setCurrentIndex(index);
    }

    void setDeviceType(int Type);

    bool printCover() {
    	return ui.printCoverButton->isChecked();
    }

    void setFromDevice(int id) {
    	ui.fromDevice->setValue(id);
    }
    void setToDevice(int id) {
    	ui.toDevice->setValue(id);
    }

    int min() {
    	return ui.fromDevice->value();
    }

    int max() {
    	return ui.toDevice->value();
    }

    bool useColor() {
    	return ui.printColors->isChecked();
    }

    ppl6::CString font() {
    	return ui.font->currentText();
    }


private:
    Ui::PrintDeviceDialogClass ui;

public slots:
	void on_okButton_clicked() {
		done(1);
	}
	void on_cancelButton_clicked() {
		done(0);
	}

};

#endif // PRINTDEVICEDIALOG_H
