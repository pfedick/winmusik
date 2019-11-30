#include "editstring.h"
#include "ui_editstring.h"

EditString::EditString(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditString)
{
    ui->setupUi(this);
}

EditString::~EditString()
{
    delete ui;
}

void EditString::setTitle(const QString &title)
{
    this->setWindowTitle(title);
}

void EditString::setString(const QString &str)
{
    ui->lineEdit->setText(str);
}

QString EditString::getString() const
{
    return ui->lineEdit->text().trimmed();
}

void EditString::on_okButton_clicked()
{
    done(1);
}

void EditString::on_cancelButton_clicked()
{
    done(0);
}
