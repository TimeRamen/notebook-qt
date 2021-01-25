#include "dialog.h"
#include "ui_dialog.h"

#include <QListWidget>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>
#include <QFileDialog>
#include <QDateEdit>


Dialog::Dialog(Note note, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
        ui->setupUi(this);
        ui->lineEdit->setText(note.getName());
        ui->plainTextEdit->setPlainText(note.getContent());
        ui->listWidget->addItems(note.getCategories());
        ui->checkBox->setChecked(note.getMarkdown());
        //set regular expression for the line edits to only accept proper characters
        //because we don't want to accept special character strings and end up having some problems while saving
        //also makes the categories look nice
        ui->lineEdit->setValidator(new QRegExpValidator( QRegExp("[A-Za-z0-9_\\s]+"), this ));
        ui->lineEdit_2->setValidator(new QRegExpValidator( QRegExp("[A-Za-z\\s]+"), this ));
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::on_pushButton_clicked()
{
    //this is when toggle buton is pressed
    auto * lw = ui->listWidget;
    QString catItem = ui->lineEdit_2->text();
    //check if catItem is in the catlist
    //if it isnt in the list, add it.
    //if it is already in the list, remove it.
    int idToRemove = 0;
    bool found = false;
    for (int i = 0; i < lw->count(); ++i) {
        if (lw->item(i)->text() == catItem) {
            found = true;
            idToRemove = i;
            break;
        }
    }

    if (found) {
        lw->takeItem(idToRemove);
    }else{
        lw->insertItem(0,catItem);
    }
}

void Dialog::on_buttonBox_accepted()
{
    auto *w =  ui->listWidget;
    QString title = ui->lineEdit->text();
    bool markdown = ui->checkBox->isChecked();
    QString content = ui->plainTextEdit->toPlainText();

    QString catList;
    catList = "[";
    if(w->count()==0){
        catList += "Uncategorised";
    }else{
        for(int i=0;i<w->count();i++){
            catList += w->item(i)->text();
            if(i!=w->count()-1){
                catList += ",";
            }
        }
    }
    catList += "]";

    QString path = "../untitled/notes/";
    QString FileName = path+ title;
    if(markdown){
        FileName += ".md";
    }else{
        FileName += ".txt";
    }
        if (FileName != "")
        {
            QFile file(QFileInfo(FileName).absoluteFilePath());

            if (file.exists())
            {
                QMessageBox::StandardButton chosenButton
                    = QMessageBox::warning(this, tr("File exists"), tr("The file already exists. Do you want to overwrite it?"),
                        QMessageBox::Ok | QMessageBox::Cancel,
                        QMessageBox::Cancel);
                if (chosenButton != QMessageBox::Ok)
                {
                    return; //Save was cancelled
                }
            }
            if (!file.open(QIODevice::WriteOnly))
            {
                QMessageBox::critical(this, tr("Error"), tr("Failed to save file"));
                return; //Aborted
            }
            //All ok - save data
            QDateTime modified = QFileInfo(FileName).lastModified();
            QString date = modified.toString("dd/MM/yyyy");
            QTextStream out(&file);
            out << "Title: " << title << endl <<"Date: " << date << endl << "Categories: " << catList <<endl<<endl<<content;
            file.close();
        }
}
