#include "competitionstable.h"
#include "ui_competitionstable.h"

CompetitionsTable::CompetitionsTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompetitionsTable)
{
    ui->setupUi(this);
}

CompetitionsTable::CompetitionsTable(QSqlQuery query, QWidget *parent):
    QDialog(parent),
    ui(new Ui::CompetitionsTable)
{
    ui->setupUi(this);
    ui->competitionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->competitionsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->competitionsTable->setRowCount(0);
    if (!query.exec("SELECT * FROM Competitions")){
        return;
    }
    while (query.next()) {
        ui->competitionsTable->insertRow(ui->competitionsTable->rowCount());
        for (int i = 0; i < 8; ++i) ui->competitionsTable->setItem(ui->competitionsTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
        QDate startDate = QDate::fromString(ui->competitionsTable->item(ui->competitionsTable->rowCount()-1, 3)->text(), "dd.MM.yyyy");
        QDate endDate = QDate::fromString(ui->competitionsTable->item(ui->competitionsTable->rowCount()-1, 4)->text(), "dd.MM.yyyy");
        QDate now = QDate::currentDate();
        QString status;
        if (startDate <= now && endDate >= now){
           status = "Идет";
        }
        else if(startDate > now){
            status = "Запланировано";
        }
        else{
            status = "Состоялось";
        }
        ui->competitionsTable->setItem(ui->competitionsTable->rowCount()-1, 8, new QTableWidgetItem(status));
    }
}

QString CompetitionsTable::getValue()
{
    return value;
}

CompetitionsTable::~CompetitionsTable()
{
    delete ui;
}

void CompetitionsTable::on_buttonBox_accepted()
{
    if (ui->competitionsTable->selectedItems().isEmpty()){
        QMessageBox::critical(this, "Ошибка", "Должен быть выбран элемент", QMessageBox::Ok);
        return;
    }
    value = ui->competitionsTable->selectedItems()[0]->text();
    emit this->accept();
}

void CompetitionsTable::on_competitionsTable_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    value = ui->competitionsTable->selectedItems()[0]->text();
    this->accept();
}
