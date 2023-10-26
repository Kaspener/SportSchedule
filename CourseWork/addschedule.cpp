#include "addschedule.h"
#include "ui_addschedule.h"

QString id = "-1";

AddSchedule::AddSchedule(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSchedule)
{
    ui->setupUi(this);
}

AddSchedule::AddSchedule(QSqlQuery query, QWidget *parent):
    QDialog(parent),
    ui(new Ui::AddSchedule)
{
    ui->setupUi(this);
    this->query = query;
    this->values = new QString[8];
    ui->MondayStart->setEnabled(ui->isMonday->isChecked());
    ui->MondayEnd->setEnabled(ui->isMonday->isChecked());
    ui->TuesdayStart->setEnabled(ui->isTuesday->isChecked());
    ui->TuesdayEnd->setEnabled(ui->isTuesday->isChecked());
    ui->WednesdayStart->setEnabled(ui->isWednesday->isChecked());
    ui->WednesdayEnd->setEnabled(ui->isWednesday->isChecked());
    ui->ThursdayStart->setEnabled(ui->isThursday->isChecked());
    ui->ThursdayEnd->setEnabled(ui->isThursday->isChecked());
    ui->FridayStart->setEnabled(ui->isFriday->isChecked());
    ui->FridayEnd->setEnabled(ui->isFriday->isChecked());
    ui->SaturdayStart->setEnabled(ui->isSaturday->isChecked());
    ui->SaturdayEnd->setEnabled(ui->isSaturday->isChecked());
    ui->SundayStart->setEnabled(ui->isSunday->isChecked());
    ui->SundayEnd->setEnabled(ui->isSunday->isChecked());
}

AddSchedule::AddSchedule(QString *values, QSqlQuery query, QWidget *parent):
    QDialog(parent),
    ui(new Ui::AddSchedule)
{
    ui->setupUi(this);
    this->query = query;
    this->values = values;
    id = values[0];
    if (!query.exec("SELECT Name FROM Coaches WHERE ID = " + values[0])){
        return;
    }
    while(query.next())ui->coach->setText(query.value(0).toString());
    QStringList list;
    if (values[1]!=""){
        ui->isMonday->setChecked(true);
        list = values[1].split(" - ");
        ui->MondayStart->setTime(QTime::fromString(list[0], "H:mm"));
        ui->MondayEnd->setTime(QTime::fromString(list[1], "H:mm"));
    }
    if (values[2]!=""){
        ui->isTuesday->setChecked(true);
        list = values[2].split(" - ");
        ui->TuesdayStart->setTime(QTime::fromString(list[0], "H:mm"));
        ui->TuesdayEnd->setTime(QTime::fromString(list[1], "H:mm"));
    }
    if (values[3]!=""){
        ui->isWednesday->setChecked(true);
        list = values[3].split(" - ");
        ui->WednesdayStart->setTime(QTime::fromString(list[0], "H:mm"));
        ui->WednesdayEnd->setTime(QTime::fromString(list[1], "H:mm"));
    }
    if (values[4]!=""){
        ui->isThursday->setChecked(true);
        list = values[4].split(" - ");
        ui->ThursdayStart->setTime(QTime::fromString(list[0], "H:mm"));
        ui->ThursdayEnd->setTime(QTime::fromString(list[1], "H:mm"));
    }
    if (values[5]!=""){
        ui->isFriday->setChecked(true);
        list = values[5].split(" - ");
        ui->FridayStart->setTime(QTime::fromString(list[0], "H:mm"));
        ui->FridayEnd->setTime(QTime::fromString(list[1], "H:mm"));
    }
    if (values[6]!=""){
        ui->isSaturday->setChecked(true);
        list = values[6].split(" - ");
        ui->SaturdayStart->setTime(QTime::fromString(list[0], "H:mm"));
        ui->SaturdayEnd->setTime(QTime::fromString(list[1], "H:mm"));
    }
    if (values[7]!=""){
        ui->isSunday->setChecked(true);
        list = values[7].split(" - ");
        ui->SundayStart->setTime(QTime::fromString(list[0], "H:mm"));
        ui->SundayEnd->setTime(QTime::fromString(list[1], "H:mm"));
    }
    ui->MondayStart->setEnabled(ui->isMonday->isChecked());
    ui->MondayEnd->setEnabled(ui->isMonday->isChecked());
    ui->TuesdayStart->setEnabled(ui->isTuesday->isChecked());
    ui->TuesdayEnd->setEnabled(ui->isTuesday->isChecked());
    ui->WednesdayStart->setEnabled(ui->isWednesday->isChecked());
    ui->WednesdayEnd->setEnabled(ui->isWednesday->isChecked());
    ui->ThursdayStart->setEnabled(ui->isThursday->isChecked());
    ui->ThursdayEnd->setEnabled(ui->isThursday->isChecked());
    ui->FridayStart->setEnabled(ui->isFriday->isChecked());
    ui->FridayEnd->setEnabled(ui->isFriday->isChecked());
    ui->SaturdayStart->setEnabled(ui->isSaturday->isChecked());
    ui->SaturdayEnd->setEnabled(ui->isSaturday->isChecked());
    ui->SundayStart->setEnabled(ui->isSunday->isChecked());
    ui->SundayEnd->setEnabled(ui->isSunday->isChecked());
}

QString *AddSchedule::getValues()
{
    return values;
}

AddSchedule::~AddSchedule()
{
    delete ui;
}

void AddSchedule::on_toolButton_clicked()
{
    coaches = new CoahesTable("SELECT * FROM Coaches WHERE ID NOT IN (SELECT CoachID FROM Schedule) OR ID = " + id , query, this);
    if(coaches->exec() == QDialog::Accepted){
        ui->coach->setText(coaches->getValues()[1]);
        values[0] = coaches->getValues()[0];
    }
}

void AddSchedule::on_isMonday_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    ui->MondayStart->setEnabled(ui->isMonday->isChecked());
    ui->MondayEnd->setEnabled(ui->isMonday->isChecked());
}

void AddSchedule::on_isTuesday_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    ui->TuesdayStart->setEnabled(ui->isTuesday->isChecked());
    ui->TuesdayEnd->setEnabled(ui->isTuesday->isChecked());
}

void AddSchedule::on_isWednesday_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    ui->WednesdayStart->setEnabled(ui->isWednesday->isChecked());
    ui->WednesdayEnd->setEnabled(ui->isWednesday->isChecked());
}

void AddSchedule::on_isThursday_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    ui->ThursdayStart->setEnabled(ui->isThursday->isChecked());
    ui->ThursdayEnd->setEnabled(ui->isThursday->isChecked());
}

void AddSchedule::on_isFriday_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    ui->FridayStart->setEnabled(ui->isFriday->isChecked());
    ui->FridayEnd->setEnabled(ui->isFriday->isChecked());
}

void AddSchedule::on_isSaturday_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    ui->SaturdayStart->setEnabled(ui->isSaturday->isChecked());
    ui->SaturdayEnd->setEnabled(ui->isSaturday->isChecked());
}

void AddSchedule::on_isSunday_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    ui->SundayStart->setEnabled(ui->isSunday->isChecked());
    ui->SundayEnd->setEnabled(ui->isSunday->isChecked());
}

void AddSchedule::on_buttonBox_accepted()
{
    if (ui->isMonday->isChecked())
        values[1] = ui->MondayStart->text() + " - " + ui->MondayEnd->text();
    else values[1] = "";
    if (ui->isTuesday->isChecked())
        values[2] = ui->TuesdayStart->text() + " - " + ui->TuesdayEnd->text();
    else values[2] = "";
    if (ui->isWednesday->isChecked())
        values[3] = ui->WednesdayStart->text() + " - " + ui->WednesdayEnd->text();
    else values[3] = "";
    if (ui->isThursday->isChecked())
        values[4] = ui->ThursdayStart->text() + " - " + ui->ThursdayEnd->text();
    else values[4] = "";
    if (ui->isFriday->isChecked())
        values[5] = ui->FridayStart->text() + " - " + ui->FridayEnd->text();
    else values[5] = "";
    if (ui->isSaturday->isChecked())
        values[6] = ui->SaturdayStart->text() + " - " + ui->SaturdayEnd->text();
    else values[6] = "";
    if (ui->isSunday->isChecked())
        values[7] = ui->SundayStart->text() + " - " + ui->SundayEnd->text();
    else values[7] = "";
    for (int i = 1; i < 8; ++i){
        if (values[i]!=""){
            QStringList list = values[i].split(" - ");
            QTime start = QTime::fromString(list[0], "H:mm");
            QTime end = QTime::fromString(list[1], "H:mm");
            if (end < start){
                QMessageBox::critical(this, "Ошибка времени", "Время начала не может быть больше времени окончания", QMessageBox::Ok);
                return;
            }
        }
    }
    if (values[0].isEmpty()){
        QMessageBox::critical(this, "Ошибка заполнения", "Необходимо заполнить тренера", QMessageBox::Ok);
        return;
    }
    emit this->accept();
}
