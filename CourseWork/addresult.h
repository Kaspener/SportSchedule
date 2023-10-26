#ifndef ADDRESULT_H
#define ADDRESULT_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

#include "athletestable.h"
#include "competitionstable.h"

namespace Ui {
class AddResult;
}

class AddResult : public QDialog
{
    Q_OBJECT

public:
    explicit AddResult(QWidget *parent = 0);
    AddResult(QSqlQuery query, QWidget *parent = 0);
    AddResult(QString *values, QSqlQuery query, QWidget *parent = 0);
    AddResult(QString athleteID, QString competitionID, QSqlQuery query, QWidget *parent = 0);
    QString *getValues();
    ~AddResult();

private slots:
    void on_buttonBox_accepted();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

private:
    Ui::AddResult *ui;
    QSqlQuery query;
    QString *values;
    AthletesTable *athlete;
    CompetitionsTable *competition;

};

#endif // ADDRESULT_H
