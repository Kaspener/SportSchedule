#ifndef COMPETITIONSTABLE_H
#define COMPETITIONSTABLE_H

#include <QDialog>
#include <QSqlQuery>
#include <QDate>
#include <QMessageBox>

namespace Ui {
class CompetitionsTable;
}

class CompetitionsTable : public QDialog
{
    Q_OBJECT

public:
    explicit CompetitionsTable(QWidget *parent = 0);
    CompetitionsTable(QSqlQuery query, QWidget *parent = 0);
    QString getValue();
    ~CompetitionsTable();

private slots:
    void on_buttonBox_accepted();

    void on_competitionsTable_cellDoubleClicked(int row, int column);

private:
    Ui::CompetitionsTable *ui;
    QString value;
};

#endif // COMPETITIONSTABLE_H
