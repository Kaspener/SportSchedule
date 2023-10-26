#ifndef ADDINVENTORY_H
#define ADDINVENTORY_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class AddInventory;
}

class AddInventory : public QDialog
{
    Q_OBJECT

public:
    explicit AddInventory(QWidget *parent = 0);
    AddInventory(QString *values, QWidget *parent = 0);
    QString *getValues();
    ~AddInventory();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddInventory *ui;
    QString* values;
};

#endif // ADDINVENTORY_H
