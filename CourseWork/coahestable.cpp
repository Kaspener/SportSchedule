#include "coahestable.h"
#include "ui_coahestable.h"

CoahesTable::CoahesTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CoahesTable)
{
    ui->setupUi(this);
}

CoahesTable::CoahesTable(QSqlQuery query, QWidget *parent):
    QDialog(parent),
    ui(new Ui::CoahesTable)
{
    ui->setupUi(this);
    values = new QString[2];
    ui->coachesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->coachesTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->coachesTable->setRowCount(0);
    if (!query.exec("SELECT * FROM Coaches")){
        return;
    }
    while (query.next()) {
        ui->coachesTable->insertRow(ui->coachesTable->rowCount());
        for (int i = 0; i < 6; ++i) ui->coachesTable->setItem(ui->coachesTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
    }
}

CoahesTable::CoahesTable(QString que, QSqlQuery query, QWidget *parent):
    QDialog(parent),
    ui(new Ui::CoahesTable)
{
    ui->setupUi(this);
    values = new QString[2];
    ui->coachesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->coachesTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->coachesTable->setRowCount(0);
    if (!query.exec(que)){
        return;
    }
    while (query.next()) {
        ui->coachesTable->insertRow(ui->coachesTable->rowCount());
        for (int i = 0; i < 6; ++i) ui->coachesTable->setItem(ui->coachesTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
    }
}

QString *CoahesTable::getValues()
{
    return values;
}

CoahesTable::~CoahesTable()
{
    delete ui;
}

void CoahesTable::on_buttonBox_accepted()
{
    if (ui->coachesTable->selectedItems().isEmpty()){
        QMessageBox::critical(this, "Ошибка", "Должен быть выбран элемент", QMessageBox::Ok);
        return;
    }
    for (int i = 0; i < 2; ++i) values[i] = ui->coachesTable->selectedItems()[i]->text();
    this->accept();
}

void CoahesTable::on_coachesTable_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    for (int i = 0; i < 2; ++i) values[i] = ui->coachesTable->selectedItems()[i]->text();
    this->accept();
}
