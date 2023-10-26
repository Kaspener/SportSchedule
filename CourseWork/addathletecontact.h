#ifndef ADDATHLETECONTACT_H
#define ADDATHLETECONTACT_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class AddAthleteContact;
}

class AddAthleteContact : public QDialog
{
    Q_OBJECT

public:
    explicit AddAthleteContact(QWidget *parent = 0);
    AddAthleteContact(QString *values, QWidget *parent = 0);
    QString *getValues();
    ~AddAthleteContact();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddAthleteContact *ui;
    QString* values;
};

#endif // ADDATHLETECONTACT_H
