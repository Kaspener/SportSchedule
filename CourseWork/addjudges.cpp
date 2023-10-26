#include "addjudges.h"
#include "ui_addjudges.h"

AddJudges::AddJudges(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddJudges)
{
    ui->setupUi(this);
    values = new QString[2];
}

AddJudges::AddJudges(QString *values, QWidget *parent):
    QDialog(parent),
    ui(new Ui::AddJudges)
{
    ui->setupUi(this);
    this->values = values;
    ui->type->setText(values[0]);
    ui->name->setText(values[1]);
}

QString *AddJudges::getValues()
{
    return values;
}

AddJudges::~AddJudges()
{
    delete ui;
}

void AddJudges::on_buttonBox_accepted()
{
    values[0] = ui->type->text();
    values[1] = ui->name->text();
    for (int i = 0; i < 2; ++i){
        if (values[i] == ""){
            QMessageBox::critical(this, "Ошибка заполнения", "Должны быть заполнены все поля", QMessageBox::Ok);
            return;
        }
    }
    emit this->accept();
}
