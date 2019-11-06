#ifndef EDITSTRING_H
#define EDITSTRING_H

#include <QDialog>
#include <QString>

namespace Ui {
class EditString;
}

class EditString : public QDialog
{
    Q_OBJECT

public:
    explicit EditString(QWidget *parent = nullptr);
    ~EditString();
    void setTitle(const QString &title);
    void setString(const QString &str);
    QString getString() const;

private:
    Ui::EditString *ui;

public slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

};

#endif // EDITSTRING_H
