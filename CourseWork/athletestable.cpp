#include "athletestable.h"
#include "ui_athletestable.h"

AthletesTable::AthletesTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AthletesTable)
{
    ui->setupUi(this);
}

AthletesTable::AthletesTable(QSqlQuery query, QWidget *parent):
QDialog(parent),
ui(new Ui::AthletesTable)
{
    ui->setupUi(this);
    ui->athletesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->athletesTable->horizontalHeader()->setSectionResizeMode(13, QHeaderView::Stretch);
    ui->athletesTable->setRowCount(0);
    if (!query.exec("SELECT a.ID, a.Surname, a.Name, a.Patronymic, a.Athlete, a.dateOfBirth, a.Gender, a.Weight, a.Height, a.Address, a.Telephone, a.DateOfEnrollment, b.Name, a.Category "
                    "FROM Athletes AS a, Coaches AS b "
                    "WHERE a.CoachID = b.ID")){
        return;
    }
    while (query.next()) {
        ui->athletesTable->insertRow(ui->athletesTable->rowCount());
        for (int i = 0; i < 5; ++i) ui->athletesTable->setItem(ui->athletesTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
        QString dateOfBirth = query.value(5).toString();
        ui->athletesTable->setItem(ui->athletesTable->rowCount()-1, 5, new QTableWidgetItem(dateOfBirth));
        QDate bD = QDate::fromString(dateOfBirth,"dd.MM.yyyy");
        short age=QDate::currentDate().year()-bD.year();
        if(QDate(0,bD.month(),bD.day())>QDate(0,QDate::currentDate().month (),QDate::currentDate().day())) --age;
        ui->athletesTable->setItem(ui->athletesTable->rowCount()-1, 6, new QTableWidgetItem(QString::number(age)));
        for (int i = 6; i < 14; ++i)ui->athletesTable->setItem(ui->athletesTable->rowCount()-1, i+1, new QTableWidgetItem(query.value(i).toString()));
    }
}

QString AthletesTable::getValue()
{
    return value;
}

AthletesTable::~AthletesTable()
{
    delete ui;
}

void AthletesTable::on_buttonBox_accepted()
{
    if (ui->athletesTable->selectedItems().isEmpty()){
        QMessageBox::critical(this, "Ошибка", "Должен быть выбран элемент", QMessageBox::Ok);
        return;
    }
    value = ui->athletesTable->selectedItems()[0]->text();
    this->accept();
}

void AthletesTable::on_athletesTable_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    value = ui->athletesTable->selectedItems()[0]->text();
    this->accept();
}
