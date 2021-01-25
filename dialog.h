#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "note.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(Note note = Note("",  QDate::currentDate() ,{},"",false), QWidget *parent = 0);

    ~Dialog();

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
    QString curItem = "";
};

#endif // DIALOG_H
