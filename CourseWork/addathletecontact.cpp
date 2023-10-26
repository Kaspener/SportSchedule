#include "addathletecontact.h"
#include "ui_addathletecontact.h"

AddAthleteContact::AddAthleteContact(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAthleteContact)
{
    ui->setupUi(this);
    values = new QString[6];
}

AddAthleteContact::AddAthleteContact(QString *values, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAthleteContact)
{
    ui->setupUi(this);
    this->values = values;
    ui->contact->setText(values[0]);
    ui->who->setText(values[1]);
    ui->work->setText(values[2]);
    ui->post->setText(values[3]);
    ui->telephone->setText(values[4]);
    ui->email->setText(values[5]);
}

QString *AddAthleteContact::getValues()
{
    return values;
}

AddAthleteContact::~AddAthleteContact()
{
    delete ui;
}

void AddAthleteContact::on_buttonBox_accepted()
{
    bool flag;
    ui->telephone->text().toLongLong(&flag);
    if (!flag){
        QMessageBox::critical(this, "Ошибка телефона", "Телефон может содержать только цифры от 0 до 9", QMessageBox::Ok);
        return;
    }
    values[0] = ui->contact->text();
    values[1] = ui->who->text();
    values[2] = ui->work->text();
    values[3] = ui->post->text();
    values[4] = ui->telephone->text();
    if (values[4].length() > 11) {QMessageBox::critical(this, "Ошибка телефона", "Телефон может содержать не более 11 символов", QMessageBox::Ok); return;}
    values[5] = ui->email->text();
    for (int i = 0; i < 5; ++i){
        if (values[i]==""){QMessageBox::critical(this, "Ошибка заполнения", "Не заполненым может быть только поле Email", QMessageBox::Ok); return;}
    }
    emit this->accept();
}
