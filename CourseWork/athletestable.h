#ifndef ATHLETESTABLE_H
#define ATHLETESTABLE_H

#include <QDialog>
#include <QSqlQuery>
#include <QDate>
#include <QMessageBox>

namespace Ui {
class AthletesTable;
}

class AthletesTable : public QDialog
{
    Q_OBJECT

public:
    explicit AthletesTable(QWidget *parent = 0);
    AthletesTable(QSqlQuery query, QWidget *parent = 0);
    QString getValue();
    ~AthletesTable();

private slots:
    void on_buttonBox_accepted();

    void on_athletesTable_cellDoubleClicked(int row, int column);

private:
    Ui::AthletesTable *ui;
    QString value;
};

#endif // ATHLETESTABLE_H
