#ifndef NOTE_H
#define NOTE_H

#include <QDateTimeEdit>
#include <QString>
#include <QDebug>
#include <QDataStream>

class Note{
public:
    Note(QString N ="", QDate D=QDate::currentDate() ,QStringList cat={}, QString noteContent="", bool mark=false);
    friend QDebug operator<<(QDebug stream, const Note &note);
    QString getName();
    QDate getDate();
    QStringList getCategories();
    QString getContent();
    bool getMarkdown();
    bool isEmpty();
private:
    QString Name;
    QDate Date;
    QStringList Categories;
    QString Content;
    bool Markdown;
};

#endif // NOTE_H
