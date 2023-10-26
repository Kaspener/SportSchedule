#ifndef ADDJUDGES_H
#define ADDJUDGES_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class AddJudges;
}

class AddJudges : public QDialog
{
    Q_OBJECT

public:
    explicit AddJudges(QWidget *parent = 0);
    AddJudges(QString *values, QWidget *parent = 0);
    QString *getValues();
    ~AddJudges();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddJudges *ui;
    QString *values;
};

#endif // ADDJUDGES_H
