#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include "note.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Note getNoteFromPath(QString path, bool mark);
    QStringList getListFromTextStream(QTextStream stream);
    Note makeNote(QStringList NoteStream, bool mark);
    QStringList getCatListFromNoteStream(QStringList NoteStream);
    QStringList renderComboBox();
public slots:
    void openDialog();
    void openDialog2();
    //void debugDisplay();
    void removeFile();
    void on_tr_clicked(const QModelIndex &index);
private:
    QFileSystemModel * model;
    //QModelIndex parentIndex;
    QString curFile = "";
signals:
    void sendNote(const Note sent);
};

#endif // MAINWINDOW_H
