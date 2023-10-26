#ifndef ADDATHLETE_H
#define ADDATHLETE_H

#include <QDialog>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QBuffer>
#include <QByteArray>
#include "coahestable.h"

namespace Ui {
class AddAthlete;
}

class AddAthlete : public QDialog
{
    Q_OBJECT

public:
    explicit AddAthlete(QWidget *parent = 0);
    AddAthlete(QSqlQuery query, QWidget *parent = 0);
    AddAthlete(QString* values, QByteArray byteArray, QSqlQuery query, QWidget *parent = 0);
    QString* getValues();
    QByteArray getByteArray();
    ~AddAthlete();

private slots:
    void on_uploadButton_clicked();

    void on_deleteButton_clicked();

    void on_buttonBox_accepted();

    void on_CoachSelect_clicked();

private:
    Ui::AddAthlete *ui;
    CoahesTable* coaches;
    QString* values;
    QImage photo;
    QByteArray byteArray;
    QString formatFile;
    QSqlQuery query;
};

#endif // ADDATHLETE_H
