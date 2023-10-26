#include "addresult.h"
#include "ui_addresult.h"

QString categoryList[] = {"МСМК", "МС", "КМС", "1 разряд", "2 разряд", "3 разряд", "1 юношеский", "2 юношеский", "3 юношеский"};

AddResult::AddResult(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddResult)
{
    ui->setupUi(this);
}

AddResult::AddResult(QSqlQuery query, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddResult)
{
    ui->setupUi(this);
    values = new QString[10];
    this->query = query;
    ui->date->setDate(QDate::currentDate());
    ui->date->setMaximumDate(QDate::currentDate());
}

AddResult::AddResult(QString *values, QSqlQuery query, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddResult)
{
    ui->setupUi(this);
    this->query = query;
    ui->date->setDate(QDate::currentDate());
    ui->date->setMaximumDate(QDate::currentDate());
    this->values = values;
    query.exec("SELECT Athlete FROM Athletes WHERE ID = " + values[8]);
    if (query.first()){
        ui->athlete->setText(query.value(0).toString());
    }
    query.exec("SELECT Name, dateStart, dateEnd FROM Competitions WHERE ID = " + values[9]);
    if (query.first()){
        ui->competition->setText(query.value(0).toString());
    }
    ui->date->setDate(QDate::fromString(values[0], "dd.MM.yyyy"));
    ui->distance->setCurrentText(values[1]);
    ui->pool->setCurrentText(values[2]);
    ui->swimming->setValue(values[3].toInt());
    ui->track->setValue(values[4].toInt());
    ui->result->setTime(QTime::fromString(values[5], "mm:ss.zzz"));
    ui->place->setValue(values[6].toInt());
}

AddResult::AddResult(QString athleteID, QString competitionID, QSqlQuery query, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddResult)
{
    ui->setupUi(this);
    values = new QString[10];
    this->query = query;
    values[8] = athleteID;
    values[9] = competitionID;
    query.exec("SELECT Athlete FROM Athletes WHERE ID = " + athleteID);
    if (query.first()){
        ui->athlete->setText(query.value(0).toString());
    }
    query.exec("SELECT Name, dateStart, dateEnd FROM Competitions WHERE ID = " + competitionID);
    if (query.first()){
        ui->competition->setText(query.value(0).toString());
        ui->date->setMinimumDate(QDate::fromString(query.value(1).toString(), "dd.MM.yyyy"));
        ui->date->setMaximumDate(QDate::fromString(query.value(2).toString(), "dd.MM.yyyy"));
        ui->date->setDate(QDate::fromString(query.value(1).toString(), "dd.MM.yyyy"));
    }
}

QString *AddResult::getValues()
{
    return values;
}

AddResult::~AddResult()
{
    delete ui;
}

void AddResult::on_buttonBox_accepted()
{
    if (ui->distance->currentText() == "100м (комплексное плавание)" && ui->pool->currentText() == "50"){
        QMessageBox::critical(this, "Ошибка заполнения", "У длины бассейна 50 нет такой дистанции", QMessageBox::Ok);
        return;
    }
    QDate date = ui->date->date();
    if (date > QDate::currentDate()){
        QMessageBox::critical(this, "Ошибка даты", "Дата не может быть больше текущей", QMessageBox::Ok);
        return;
    }
    values[0] = ui->date->text();
    values[1] = ui->distance->currentText();
    values[2] = ui->pool->currentText();
    values[3] = ui->swimming->text();
    values[4] = ui->track->text();
    values[5] = ui->result->text();
    values[6] = ui->place->text();
    if (!query.exec("SELECT dateStart, dateEnd FROM Competitions WHERE ID = " + values[9])){
        QMessageBox::critical(this, "Ошибка", "Нет соревнования", QMessageBox::Ok);
        return;
    }
    QDate start;
    QDate end;
    while(query.next()){
        start = QDate::fromString(query.value(0).toString(), "dd.MM.yyyy");
        end = QDate::fromString(query.value(1).toString(), "dd.MM.yyyy");
    }
    if (!(date >= start && date <= end)){
        QMessageBox::critical(this, "Ошибка даты", "Неверная дата", QMessageBox::Ok);
        return;
    }
    for (int i = 0; i < 10; ++i){
        if (i!=7)
            if (values[i].isEmpty()){
                qDebug() << i;
                QMessageBox::critical(this, "Ошибка заполнения", "Все поля должны быть заполнены", QMessageBox::Ok);
                return;
            }
    }
    if (!query.exec("SELECT Gender FROM Athletes WHERE ID = " + values[8])){
        QMessageBox::critical(this, "Ошибка", "Нет спортсмена", QMessageBox::Ok);
        return;
    }
    QString gender;
    if (query.first()){
        gender = query.value(0).toString();
    }
    if (!query.exec("SELECT MSMK, MS, KMS, category1, category2, category3, youth1, youth2, youth3 FROM Categories WHERE Style = '" + values[1] + "' AND Pool = '" + values[2] + "' AND Gender = '" + gender + "'")){
        QMessageBox::critical(this, "Ошибка", "Нет категории", QMessageBox::Ok);
        return;
    }
    while(query.next()){
        for (int i = 0; i < 9; ++i){
            if (values[5] <= query.value(i).toString()){
                values[7] = categoryList[i];
                break;
            }
        }
    }
    emit this->accept();
}

void AddResult::on_toolButton_clicked()
{
    athlete = new AthletesTable(query, this);
    if(athlete->exec() == QDialog::Accepted){
        values[8] = athlete->getValue();
        if (!query.exec("SELECT Athlete FROM Athletes WHERE ID = " + values[8])){
            return;
        }
        if (query.first()){
            ui->athlete->setText(query.value(0).toString());
        }
    }
}

void AddResult::on_toolButton_2_clicked()
{
    competition = new CompetitionsTable(query, this);
    if(competition->exec() == QDialog::Accepted){
        values[9] = competition->getValue();
        if (!query.exec("SELECT Name, dateStart, dateEnd FROM Competitions WHERE ID = " + values[9])){
            return;
        }
        if (query.first()){
            ui->competition->setText(query.value(0).toString());
            ui->date->setMinimumDate(QDate::fromString(query.value(1).toString(), "dd.MM.yyyy"));
            ui->date->setMaximumDate(QDate::fromString(query.value(2).toString(), "dd.MM.yyyy"));
            ui->date->setDate(QDate::fromString(query.value(1).toString(), "dd.MM.yyyy"));
        }
    }
}
