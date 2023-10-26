#ifndef COAHESTABLE_H
#define COAHESTABLE_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class CoahesTable;
}

class CoahesTable : public QDialog
{
    Q_OBJECT

public:
    explicit CoahesTable(QWidget *parent = 0);
    CoahesTable(QSqlQuery query, QWidget *parent = 0);
    CoahesTable(QString que, QSqlQuery query, QWidget *parent = 0);
    QString *getValues();
    ~CoahesTable();

private slots:
    void on_buttonBox_accepted();

    void on_coachesTable_cellDoubleClicked(int row, int column);

private:
    Ui::CoahesTable *ui;
    QString *values;
};

#endif // COAHESTABLE_H
