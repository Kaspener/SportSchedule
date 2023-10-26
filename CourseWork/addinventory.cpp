#include "addinventory.h"
#include "ui_addinventory.h"

AddInventory::AddInventory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddInventory)
{
    ui->setupUi(this);
    values = new QString[3];
}

AddInventory::AddInventory(QString *values, QWidget *parent):
    QDialog(parent),
    ui(new Ui::AddInventory)
{
    ui->setupUi(this);
    this->values = values;
    ui->inventory->setText(values[0]);
    ui->count->setValue(values[1].toInt());
    ui->poolNumber->setCurrentText(values[2]);
}

QString *AddInventory::getValues()
{
    return values;
}

AddInventory::~AddInventory()
{
    delete ui;
}

void AddInventory::on_buttonBox_accepted()
{
    values[0] = ui->inventory->text();
    values[1] = ui->count->text();
    values[2] = ui->poolNumber->currentText();
    for (int i = 0; i < 2; ++i){
        if (values[i].isEmpty()) {
            QMessageBox::critical(this, "Ошибка заполнения", "Все поля должны быть заполнены", QMessageBox::Ok);
            return;
        }
    }
    emit this->accept();
}
