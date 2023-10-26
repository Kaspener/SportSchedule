#ifndef ADDCOMPETITION_H
#define ADDCOMPETITION_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class AddCompetition;
}

class AddCompetition : public QDialog
{
    Q_OBJECT

public:
    explicit AddCompetition(QWidget *parent = 0);
    AddCompetition(QString *values, QWidget *parent = 0);
    QString *getValues();
    ~AddCompetition();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddCompetition *ui;
    QString *values;
};

#endif // ADDCOMPETITION_H
