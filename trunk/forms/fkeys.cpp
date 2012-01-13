#include "fkeys.h"

Fkeys::Fkeys(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	button[0]=ui.esc;
	button[1]=ui.f1;
	button[2]=ui.f2;
	button[3]=ui.f3;
	button[4]=ui.f4;
	button[5]=ui.f5;
	button[6]=ui.f6;
	button[7]=ui.f7;
	button[8]=ui.f8;
	button[9]=ui.f9;
	button[10]=ui.f10;
	button[11]=ui.f11;
	button[12]=ui.f12;
}

Fkeys::~Fkeys()
{

}


void Fkeys::setFkey(int num, const QString &Icon, const QString &Text, bool enabled)
{
	if (num<0 || num>12) return;
	button[num]->setIcon(QIcon(Icon));
	if (Text.size()) button[num]->setText(Text);
	button[num]->setEnabled(enabled);
}

void Fkeys::setFkey(int num, const QIcon &Icon, const QString &Text, bool enabled)
{
	if (num<0 || num>12) return;
	button[num]->setIcon(Icon);
	if (Text.size()) button[num]->setText(Text);
	button[num]->setEnabled(enabled);
}

void Fkeys::setEnabled(int num, bool flag)
{
	if (num<0 || num>12) return;
	button[num]->setEnabled(flag);
}

bool Fkeys::enabled(int num) const
{
	if (num<0 || num>12) return (false);
	return (button[num]->isEnabled());
}

void Fkeys::on_esc_clicked()
{
	emit clicked(0);
}

void Fkeys::on_f1_clicked()
{
	emit clicked(1);
}

void Fkeys::on_f2_clicked()
{
	emit clicked(2);
}

void Fkeys::on_f3_clicked()
{
	emit clicked(3);
}

void Fkeys::on_f4_clicked()
{
	emit clicked(4);
}

void Fkeys::on_f5_clicked()
{
	emit clicked(5);
}

void Fkeys::on_f6_clicked()
{
	emit clicked(6);
}

void Fkeys::on_f7_clicked()
{
	emit clicked(7);
}

void Fkeys::on_f8_clicked()
{
	emit clicked(8);
}

void Fkeys::on_f9_clicked()
{
	emit clicked(9);
}

void Fkeys::on_f10_clicked()
{
	emit clicked(10);
}

void Fkeys::on_f11_clicked()
{
	emit clicked(11);
}

void Fkeys::on_f12_clicked()
{
	emit clicked(12);
}
