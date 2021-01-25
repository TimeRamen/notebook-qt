#include "note.h"
#include <QTextStream>


Note::Note(QString N, QDate D,QStringList cat, QString noteContent, bool mark){
    Name = N;
    Date = D;
    Categories = cat;
    Content = noteContent;
    Markdown = mark;
}
bool Note::isEmpty()
{
    if(Name == ""){
        if(Categories.isEmpty()){
            return true;
        }
    }
        return true;
}
QString Note::getName()
{
    return Name;
}
QDate Note::getDate()
{
    return Date;
}
QStringList Note::getCategories()
{
    return Categories;
}
QString Note::getContent()
{
    return Content;
}
bool Note::getMarkdown()
{
    return Markdown;
}
QDebug operator<<(QDebug stream, const Note &note) {
    stream << "Name = "
           << note.Name
           << endl
           << "Date = "
           << note.Date
           << endl
           << "Categories = "
           << note.Categories
           << endl
           << "Content = "
           << note.Content;
    return stream;
}


