#include "mainwindow.h"
#include "dialog.h"
#include "note.h"

#include <QGroupBox>
#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>
#include <QtDebug>
#include <QDateEdit>
#include <QComboBox>
#include <QLabel>
#include <QSortFilterProxyModel>
#include <QModelIndex>
#include <QItemSelectionModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *widget = new QWidget;

    model = new QFileSystemModel(this);
    model->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    model->setRootPath("../untitled/notes/");
    auto *l = new QVBoxLayout(widget);

    auto *gb = new QGroupBox;
    //check the earliest date from list of notes. then create the date. else, set it as current date.

    auto date1=QDate::currentDate();
    auto *de1 = new QDateEdit(date1);
    de1->setDisplayFormat("dd.MM.yyyy");
    de1->setCalendarPopup(true);

    auto date2=de1->date();
    auto *de2 = new QDateEdit(date2.addDays(7));
    de2->setDisplayFormat("dd.MM.yyyy");
    de2->setCalendarPopup(true);

    //de1->setMinimumDate(date1.addYears(-1));
    //de1->setMaximumDate(date1.addYears(10));
    //de2->setMinimumDate(date2);
    //de2->setMaximumDate(date2.addDays(365));


    //categories combo box..
    auto *catBox = new QComboBox();
    //renderComboBox();
    //i want this to be updated each time something gets deleted or added..
    for ( const auto& i : renderComboBox()  )
    {
        catBox->addItem(i);
    }

    //filter and clear buttons...
    QPushButton *fil = new QPushButton(tr("Filter"));
    QPushButton *clear = new QPushButton(tr("Clear"));
    //
    QLabel *de1Label = new QLabel(tr("To:"));
    de1Label->setBuddy(de1);
    QLabel *de2Label = new QLabel(tr("From:"));
    de2Label->setBuddy(de2);
    QLabel *catLabel = new QLabel(tr("Category:"));
    catLabel->setBuddy(catBox);

    auto *hgbox1=new QGroupBox;
    auto *hgbox2=new QGroupBox;
    auto *hbox1=new QHBoxLayout;
    auto *hbox2=new QHBoxLayout;
    auto *gbox = new QVBoxLayout;
    hbox1->addWidget(de1Label);
    hbox1->addWidget(de1);
    hbox1->addWidget(de2Label);
    hbox1->addWidget(de2);
    hgbox1->setLayout(hbox1);
    gbox->addWidget(catLabel);
    gbox->addWidget(catBox);
    gb->setLayout(gbox);
    hbox2->addWidget(fil);
    hbox2->addWidget(clear);
    hgbox2->setLayout(hbox2);
    //tree view
    auto *tr = new QTreeView;
    tr->setModel(model);
    tr->setRootIndex(model->index("../untitled/notes/"));
    tr->hideColumn(1);

    //QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    //proxyModel->setSourceModel(model);
    //tr->setModel(proxyModel);

    //top group box
    l->addWidget(hgbox1);
    l->addWidget(gb);
    l->addWidget(hgbox2);

    connect(tr,SIGNAL(clicked(const QModelIndex &)),this,SLOT(on_tr_clicked(const QModelIndex &)));
    //tree view
    l->addWidget(tr);
    //https://doc.qt.io/archives/qt-4.8/qsortfilterproxymodel.html <- check this out for the filter

    //even if edit does the same thing as new, we still need a new button in case we click on treeview
    //but still want to make a new note
    QPushButton *pb = new QPushButton("New");
    connect(pb,SIGNAL(clicked()),this,SLOT(openDialog()));

    //this defaults into the new button if nothing in the tree view is selected.
    QPushButton *pb2 = new QPushButton("Edit");
    connect(pb2,SIGNAL(clicked()),this,SLOT(openDialog2()));

    //this does nothing if nothing in the tree view is selected.
    QPushButton *pb3 = new QPushButton("Delete");
    connect(pb3,SIGNAL(clicked()),this,SLOT(removeFile()));

    auto *hbox3=new QHBoxLayout;
    auto *hgbox3=new QGroupBox;
    hbox3->addWidget(pb);
    hbox3->addWidget(pb2);
    hbox3->addWidget(pb3);
    hgbox3->setLayout(hbox3);

    l->addWidget(hgbox3);

/*
 *   --BUTTON FOR DEBUGGING PURPOSES--
 *
    QPushButton *pb4 = new QPushButton("Debug");
    connect(pb4,SIGNAL(clicked()),this,SLOT(debugDisplay()));
    l->addWidget(pb4);
*/
    setCentralWidget(widget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::openDialog()
{
    Dialog dialog;
    if(dialog.exec()){
        qDebug() << "Hey, you clicked OK!";
    }
}
void MainWindow::openDialog2()
{
    if(curFile==""){
        Dialog dialog;
        dialog.exec();
    }else{
        bool mark = false;
        if(curFile.split('.')[1]=="md"){
            mark = true;
        }
        Dialog dialog(getNoteFromPath(curFile,mark));
        dialog.exec();
    }
}
void MainWindow::removeFile()
{
    if(curFile!=""){
       QFile::remove(curFile);
       qDebug() << curFile << " removed";
    }
    else{
        qDebug() << "You have not selected a file.";
    }
}
void MainWindow::on_tr_clicked(const QModelIndex &index)
{
    QString path = model->fileInfo(index).absoluteFilePath();
    qDebug() <<"This file has been selected: " <<path << endl;
    curFile = path;
}

Note MainWindow::getNoteFromPath(QString path, bool mark)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QString note = file.readAll();
    file.close();
    QStringList list = note.split('\n');
    return makeNote(list,mark);
}

Note MainWindow::makeNote(QStringList NoteStream, bool mark)
{
   QString firstLine = NoteStream.at(0).split(':').at(1).mid(1).trimmed();//name
   QString secondLine = NoteStream.at(1).split(':').at(1).mid(1).trimmed();//date
   QStringList catList = getCatListFromNoteStream(NoteStream);
   QString cont ="";
   for(int i=4;i<NoteStream.count() ; i++)
   {
       cont += NoteStream.at(i);
   }
   return Note(firstLine, QDate::fromString(secondLine,"dd/MM/yyyy"), catList, cont, mark);
}

QStringList MainWindow::getCatListFromNoteStream(QStringList NoteStream)
{
    QString thirdLine = NoteStream.at(2).split(':').at(1).mid(1).trimmed();//categories
    int startPos = thirdLine.indexOf('[')+1;
    int endPos = thirdLine.indexOf(']');
    int length = endPos - startPos;
    QStringList catList = thirdLine.mid(startPos,length).split(',');
    return catList;
}

QStringList MainWindow::renderComboBox()
{
    QDir directory("../untitled/notes/");
    QStringList noteList = directory.entryList(QStringList() << "*.txt" <<"*.md" <<"*.TXT" << "*.MD",QDir::Files);
    QStringList masterList;
    for(int i = 0; i < noteList.count();i++)
    {
        QString path= directory.absolutePath() +"/"+ noteList.at(i);
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QString note = file.readAll();
        file.close();
        QStringList list = note.split('\n');
        QStringList curList = getCatListFromNoteStream(list);
        for ( const auto& i : curList  )
        {
            if(!masterList.contains(i))
            {
                masterList.append(i);
            }
        }
    }
    return masterList;
}
/*
void MainWindow::debugDisplay()
{
    int rowCount = model->rowCount(parentIndex);
    for(int i = 0; i < rowCount;i++)
    {
        QModelIndex index = model->index(i,0,parentIndex);
        QString data = model->data(index,Qt::DisplayRole).toString();
        //qDebug() << "Data " << QString::number(i) << ": " << data;
        QString path= model->rootPath() +"/"+ data;
        //qDebug() << path;
    }
    //qDebug() << QDir::current();
}
*/
