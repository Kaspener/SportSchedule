#include "addcompetition.h"
#include "ui_addcompetition.h"

AddCompetition::AddCompetition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCompetition)
{
    ui->setupUi(this);
    values = new QString[7];
    ui->dateStart->setMaximumDate(QDate::currentDate());
    ui->dateEnd->setMaximumDate(QDate::currentDate());
    ui->dateEnd->setDate(QDate::currentDate());
}

AddCompetition::AddCompetition(QString *values, QWidget *parent):
    QDialog(parent),
    ui(new Ui::AddCompetition)
{
    ui->setupUi(this);
    this->values = values;
    ui->dateStart->setMaximumDate(QDate::currentDate());
    ui->dateEnd->setMaximumDate(QDate::currentDate());
    ui->name->setText(values[0]);
    ui->category->setText(values[1]);
    ui->dateStart->setDate(QDate::fromString(values[2], "dd.MM.yyyy"));
    ui->dateEnd->setDate(QDate::fromString(values[3], "dd.MM.yyyy"));
    ui->contry->setText(values[4]);
    ui->city->setText(values[5]);
    ui->place->setText(values[6]);
}

QString *AddCompetition::getValues()
{
    return values;
}

AddCompetition::~AddCompetition()
{
    delete ui;
}

void AddCompetition::on_buttonBox_accepted()
{
    if (ui->dateStart->date() > ui->dateEnd->date()){
        QMessageBox::critical(this, "Ошибка даты", "Дата окончания не может быть меньше даты начала", QMessageBox::Ok);
        return;
    }
    values[0] = ui->name->text();
    values[1] = ui->category->text();
    values[2] = ui->dateStart->text();
    values[3] = ui->dateEnd->text();
    values[4] = ui->contry->text();
    values[5] = ui->city->text();
    values[6] = ui->place->text();
    for (int i = 0; i < 7; ++i){
        if (values[i] == ""){
            QMessageBox::critical(this, "Ошибка заполнения", "Должны быть заполнены все поля", QMessageBox::Ok);
            return;
        }
    }
    emit this->accept();
}
