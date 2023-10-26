#ifndef ATHLETE_H
#define ATHLETE_H

#include <QString>
#include <QDate>

class Athlete
{
public:
    Athlete();
    QString surname;
    QString name;
    QString patronymic;
    QString athlete;
    QDate dateOfBirth;

};

#endif // ATHLETE_H
