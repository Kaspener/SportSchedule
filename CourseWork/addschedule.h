#ifndef ADDSCHEDULE_H
#define ADDSCHEDULE_H

#include <QDialog>
#include <QSqlQuery>

#include "coahestable.h"

namespace Ui {
class AddSchedule;
}

class AddSchedule : public QDialog
{
    Q_OBJECT

public:
    explicit AddSchedule(QWidget *parent = 0);
    AddSchedule(QSqlQuery query, QWidget *parent = 0);
    AddSchedule(QString *values, QSqlQuery query, QWidget *parent = 0);
    QString *getValues();
    ~AddSchedule();

private slots:
    void on_toolButton_clicked();

    void on_isMonday_stateChanged(int arg1);

    void on_isTuesday_stateChanged(int arg1);

    void on_isWednesday_stateChanged(int arg1);

    void on_isThursday_stateChanged(int arg1);

    void on_isFriday_stateChanged(int arg1);

    void on_isSaturday_stateChanged(int arg1);

    void on_isSunday_stateChanged(int arg1);

    void on_buttonBox_accepted();

private:
    Ui::AddSchedule *ui;
    QSqlQuery query;
    CoahesTable* coaches;
    QString *values;
};

#endif // ADDSCHEDULE_H
