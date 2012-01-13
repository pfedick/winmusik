#ifndef REGISTRATION_H
#define REGISTRATION_H

#include "winmusik3.h"
#include <QtGui/QDialog>
#include "ui_registration.h"

class Registration : public QDialog
{
    Q_OBJECT

public:
    Registration(QWidget *parent = 0, CWmClient *wm = NULL);
    ~Registration();

private:
    Ui::RegistrationClass ui;
    CWmClient *wm;
    ppl6::CCurl Curl;
    ppl6::CString SerialMD5;

public slots:
	void on_newRegistrationButton_clicked() {
		ui.stackedWidget->setCurrentIndex(1);
	}

	void on_alreadyRegisteredButton_clicked() {
		ui.stackedWidget->setCurrentIndex(5);

	}

	void on_cancelButton1_clicked() {
		done(0);
	}
	void on_cancelButton2_clicked() {
		done(0);
	}
	void on_cancelButton3_clicked() {
		done(0);
	}
	void on_cancelButton4_clicked() {
		done(0);
	}
	void on_cancelButton5_clicked() {
		done(0);
	}

	void on_backButton1_clicked() {
		ui.stackedWidget->setCurrentIndex(1);
	}

	void on_continueButton1_clicked();
	void on_continueButton2_clicked();
	void on_continueButton3_clicked();
	void on_continueButton4_clicked();

	void on_block1_gotFocus() { ui.block1->selectAll(); }
	void on_block2_gotFocus() { ui.block2->selectAll(); }
	void on_block3_gotFocus() { ui.block3->selectAll(); }
	void on_block4_gotFocus() { ui.block4->selectAll(); }
	void on_block5_gotFocus() { ui.block5->selectAll(); }
	void on_block6_gotFocus() { ui.block6->selectAll(); }
	void CheckSerialInput();

	void on_ser1_gotFocus() { ui.ser1->selectAll(); }
	void on_ser2_gotFocus() { ui.ser2->selectAll(); }
	void on_ser3_gotFocus() { ui.ser3->selectAll(); }
	void on_ser4_gotFocus() { ui.ser4->selectAll(); }
	void on_ser5_gotFocus() { ui.ser5->selectAll(); }
	void on_ser6_gotFocus() { ui.ser6->selectAll(); }
	void CheckSerialInput2();

	void on_block1_textChanged(const QString &text)
	{
		if (text.size()==5) ui.block2->setFocus();
		ui.block1->setText(ui.block1->text().toUpper());
		CheckSerialInput();
	}
	void on_block2_textChanged(const QString &text)
	{
		if (text.size()==5) ui.block3->setFocus();
		ui.block2->setText(ui.block2->text().toUpper());
		CheckSerialInput();
	}
	void on_block3_textChanged(const QString &text)
	{
		if (text.size()==5) ui.block4->setFocus();
		ui.block3->setText(ui.block3->text().toUpper());
		CheckSerialInput();
	}
	void on_block4_textChanged(const QString &text)
	{
		if (text.size()==5) ui.block5->setFocus();
		ui.block4->setText(ui.block4->text().toUpper());
		CheckSerialInput();
	}
	void on_block5_textChanged(const QString &text)
	{
		if (text.size()==5) ui.block6->setFocus();
		ui.block5->setText(ui.block5->text().toUpper());
		CheckSerialInput();
	}
	void on_block6_textChanged(const QString &text)
	{
		ui.block6->setText(ui.block6->text().toUpper());
		CheckSerialInput();
		if (text.size()==5) ui.continueButton3->setFocus();
	}

	void on_ser1_textChanged(const QString &text)
	{
		if (text.size()==5) ui.ser2->setFocus();
		ui.ser1->setText(ui.ser1->text().toUpper());
		CheckSerialInput2();
	}
	void on_ser2_textChanged(const QString &text)
	{
		if (text.size()==5) ui.ser3->setFocus();
		ui.ser2->setText(ui.ser2->text().toUpper());
		CheckSerialInput2();
	}
	void on_ser3_textChanged(const QString &text)
	{
		if (text.size()==5) ui.ser4->setFocus();
		ui.ser3->setText(ui.ser3->text().toUpper());
		CheckSerialInput2();
	}
	void on_ser4_textChanged(const QString &text)
	{
		if (text.size()==5) ui.ser5->setFocus();
		ui.ser4->setText(ui.ser4->text().toUpper());
		CheckSerialInput2();
	}
	void on_ser5_textChanged(const QString &text)
	{
		if (text.size()==5) ui.ser6->setFocus();
		ui.ser5->setText(ui.ser5->text().toUpper());
		CheckSerialInput2();
	}
	void on_ser6_textChanged(const QString &text)
	{
		ui.ser6->setText(ui.ser6->text().toUpper());
		CheckSerialInput2();
		if (text.size()==5) ui.continueButton4->setFocus();
	}

};

#endif // REGISTRATION_H
