#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new QSettings(this);
    db = QSqlDatabase::addDatabase("QSQLITE", "db");
    db.setDatabaseName("./../CourseWork/database.db");
    if(db.open()){
        ui->statusBar->showMessage("Database open", 3000);
    }
    else{
        ui->statusBar->showMessage("Database not open", 3000);
    }
    query = QSqlQuery(db);
    lastAthleteSelected = -1;
    categoryList = {"МСМК", "МС", "КМС", "1 разряд", "2 разряд", "3 разряд", "1 юношеский", "2 юношеский", "3 юношеский"};
    connect(ui->newDataBase, SIGNAL(triggered()), this, SLOT(on_newDB_triggered()));
    connect(ui->exportDataBase, SIGNAL(triggered()), this, SLOT(on_exportDB_triggered()));
    connect(ui->importDataBase, SIGNAL(triggered()), this, SLOT(on_importDB_triggered()));
    connect(ui->help, SIGNAL(triggered()), this, SLOT(slotInfo()));
    loadSettings();
    loadAthletesDataBase();
    loadCompetitionsDataBase();
    loadCoachesDataBase();
    loadInventoryDataBase();
    loadCategotiesDataBase();
    loadScheduleDataBase();
    loadResultsDataBase();
    tableSettings();
    ui->tabWidget->hide();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(60000);
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::saveSettings()
{
    settings->setValue("title", windowTitle());
    settings->setValue("geometry", geometry());
    settings->setValue("page", ui->scheduleTable->currentIndex());
}

void MainWindow::loadSettings()
{
    setWindowTitle(settings->value("title", "Спортивная школа").toString());
    setGeometry(settings->value("geometry", QRect(200,200,300,300)).toRect());
    ui->scheduleTable->setCurrentIndex(settings->value("page", 0).toInt());

}

void MainWindow::tableSettings()
{
    ui->athletesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->athletesTable->horizontalHeader()->setSectionResizeMode(13, QHeaderView::Stretch);
    ui->contactsAthleteTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->contactsAthleteTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->competitionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->competitionsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->judgesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->judgesTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->participantsParticipantsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->participantsParticipantsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->participantsResultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->participantsResultsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->resultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->resultsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->scheduleClassesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->categoriesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->categoriesTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->inventoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->inventoryTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->coachesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->coachesTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->resultsAthleteTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->resultsAthleteTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

void MainWindow::loadAthletesDataBase()
{
    ui->athletesTable->setRowCount(0);
    lastAthleteSelected = -1;
    ui->tabWidget_2->hide();
    ui->tabWidget_3->hide();
    if (!query.exec("SELECT a.ID, a.Surname, a.Name, a.Patronymic, a.Athlete, a.dateOfBirth, a.Gender, a.Weight, a.Height, a.Address, a.Telephone, a.DateOfEnrollment, b.Name, a.Category "
                    "FROM Athletes AS a, Coaches AS b "
                    "WHERE a.CoachID = b.ID")){
        ui->statusBar->showMessage("Database table 'Athletes' not open", 3000);
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

void MainWindow::loadAthleteInfoDataBase(QString athleteId)
{
    loadAthleteContactsDataBase(athleteId);
    loadAthletePhotoDataBase(athleteId);
    loadAthleteResultsDataBase(athleteId);
}

void MainWindow::loadAthletePhotoDataBase(QString athleteId)
{
    if (!query.exec("SELECT Photo FROM Athletes WHERE ID = " + athleteId)){
        ui->statusBar->showMessage("Database table 'Photo' 'Athlete' not open", 3000);
        return;
    }
    while(query.next()){
        QPixmap outPixmap = QPixmap();
        outPixmap.loadFromData(query.value(0).toByteArray());
        ui->athletPhoto->setPixmap(outPixmap);
    }
}

void MainWindow::loadAthleteContactsDataBase(QString athleteId)
{
    ui->contactsAthleteTable->setRowCount(0);
    if (!query.exec("SELECT * FROM AthletesContacts WHERE AthleteID = " + athleteId)){
        ui->statusBar->showMessage("Database table 'AthletesContacts' not open", 3000);
        return;
    }
    while (query.next()) {
        ui->contactsAthleteTable->insertRow(ui->contactsAthleteTable->rowCount());
        for (int i = 0; i < 7; ++i) ui->contactsAthleteTable->setItem(ui->contactsAthleteTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
    }
}

void MainWindow::loadAthleteResultsDataBase(QString athleteId)
{
    ui->resultsAthleteTable->setRowCount(0);
    if (!query.exec("SELECT r.ID, r.Date, c.Name, r.Distance, r.PoolSize, r.Swimming, r.Track, r.Result, r.Place, r.Category "
                    "FROM Results AS r, Competitions AS c "
                    "WHERE r.CompetitionID = c.ID AND r.AthleteID = " + athleteId)){
        ui->statusBar->showMessage("Database table 'AthletesContacts' not open", 3000);
        return;
    }
    while (query.next()) {
        ui->resultsAthleteTable->insertRow(ui->resultsAthleteTable->rowCount());
        for (int i = 0; i < 10; ++i) ui->resultsAthleteTable->setItem(ui->resultsAthleteTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
    }
}

void MainWindow::loadCompetitionsDataBase()
{
    ui->competitionsTable->setRowCount(0);
    lastCompetitionSelected = -1;
    ui->tabWidget_4->hide();
    if (!query.exec("SELECT * FROM Competitions")){
        ui->statusBar->showMessage("Database table 'Competitions' not open", 3000);
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

void MainWindow::loadCompetitionInfoDataBase(QString competitionId)
{
    loadCompetitionJudgesDataBase(competitionId);
    loadCompetitionParticipantsDataBase(competitionId);
}

void MainWindow::loadCompetitionJudgesDataBase(QString competitionId)
{
    ui->judgesTable->setRowCount(0);
    if (!query.exec("SELECT * FROM JudgesOfCompetitions WHERE CompetitionID = " + competitionId)){
        ui->statusBar->showMessage("Database table 'JudgesOfCompetitions' not open", 3000);
        return;
    }
    while (query.next()) {
        ui->judgesTable->insertRow(ui->judgesTable->rowCount());
        for (int i = 0; i < 3; ++i) ui->judgesTable->setItem(ui->judgesTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
    }

}

void MainWindow::loadCompetitionParticipantsDataBase(QString competitionId)
{
    ui->participantsParticipantsTable->setRowCount(0);
    if (!query.exec("SELECT p.ID, a.Athlete, c.Name "
                    "FROM ParticipantsOfCompetitions AS p, Athletes AS a, Coaches AS c "
                    "WHERE p.AthleteID = a.ID AND a.CoachID = c.ID AND p.CompetitionID = " + competitionId)){
        ui->statusBar->showMessage("Database table 'ParticipantsOfCompetitions' not open", 3000);
        return;
    }
    while (query.next()) {
        ui->participantsParticipantsTable->insertRow(ui->participantsParticipantsTable->rowCount());
        for (int i = 0; i < 3; ++i) ui->participantsParticipantsTable->setItem(ui->participantsParticipantsTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
    }
}

void MainWindow::loadCompetitionResultsDataBase(QString competitionID, QString athleteID)
{
    ui->participantsResultsTable->setRowCount(0);
    if (!query.exec("SELECT r.ID, r.Distance, r.PoolSize, Athletes.Athlete, r.Swimming, r.Track, r.Result, r.Place, r.Category "
                    "FROM Results AS r JOIN Athletes "
                    "ON r.AthleteID = Athletes.ID "
                    "JOIN Competitions ON r.CompetitionID = Competitions.ID "
                    "WHERE r.AthleteID = " + athleteID + " AND r.CompetitionID = " + competitionID)){
        ui->statusBar->showMessage("Database table 'ParticipantsOfCompetitions' not open", 3000);
        return;
    }
    while (query.next()) {
        ui->participantsResultsTable->insertRow(ui->participantsResultsTable->rowCount());
        for (int i = 0; i < 9; ++i) ui->participantsResultsTable->setItem(ui->participantsResultsTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
    }
}

void MainWindow::loadCoachesDataBase()
{
    ui->coachesTable->setRowCount(0);
    lastCoachSelected = -1;
    ui->tabWidget->hide();
    if (!query.exec("SELECT * FROM Coaches")){
        ui->statusBar->showMessage("Database table 'Coaches' not open", 3000);
        return;
    }
    while (query.next()) {
        ui->coachesTable->insertRow(ui->coachesTable->rowCount());
        for (int i = 0; i < 6; ++i) ui->coachesTable->setItem(ui->coachesTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
    }
}

void MainWindow::loadCoachPhotoDataBase(QString coachID)
{
    if (!query.exec("SELECT Photo FROM Coaches WHERE ID = " + coachID)){
        ui->statusBar->showMessage("Database table 'Photo' 'Coaches' not open", 3000);
        return;
    }
    while(query.next()){
        QPixmap outPixmap = QPixmap();
        outPixmap.loadFromData(query.value(0).toByteArray());
        ui->coachePhoto->setPixmap(outPixmap);
    }
}

void MainWindow::loadInventoryDataBase()
{
    if (!ui->inventotyTree->selectedItems().isEmpty()) ui->inventotyTree->selectedItems()[0]->setSelected(false);
    ui->inventotyTree->topLevelItem(0)->setSelected(true);
    emit this->on_inventotyTree_itemClicked(ui->inventotyTree->itemAt(0,0), 0);
    ui->inventoryTable->setRowCount(0);
    if (!query.exec("SELECT * FROM Inventory")){
        ui->statusBar->showMessage("Database table 'Inventory' not open", 3000);
        return;
    }
    while (query.next()) {
        ui->inventoryTable->insertRow(ui->inventoryTable->rowCount());
        for (int i = 0; i < 3; ++i) ui->inventoryTable->setItem(ui->inventoryTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
        ui->inventoryTable->setItem(ui->inventoryTable->rowCount()-1, 3, new QTableWidgetItem("Бассейн " + query.value(3).toString()));
    }
}

void MainWindow::loadCategotiesDataBase()
{
//    QString format = "mm:ss.zzz";
//    QTime time;
//    QString tim;
    ui->categoriesTable->setRowCount(0);
    if (!query.exec("SELECT * FROM Categories")){
        ui->statusBar->showMessage("Database table 'Categories' not open", 3000);
        return;
    }
    while (query.next()) {
        ui->categoriesTable->insertRow(ui->categoriesTable->rowCount());
        for (int i = 0; i < 13; ++i) ui->categoriesTable->setItem(ui->categoriesTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
    }
}

void MainWindow::loadScheduleDataBase()
{
    ui->scheduleClassesTable->setRowCount(0);
    if (!query.exec("SELECT s.ID, Coaches.Name, s.Monday, s.Tuesday, s.Wednesday, s.Thursday, s.Friday, s.Saturday, s.Sunday "
                    "FROM Schedule as s JOIN Coaches "
                    "ON s.CoachID = Coaches.ID")){
        ui->statusBar->showMessage("Database table 'Schedule' not open", 3000);
        return;
    }
    while (query.next()) {
        ui->scheduleClassesTable->insertRow(ui->scheduleClassesTable->rowCount());
        for (int i = 0; i < 9; ++i) ui->scheduleClassesTable->setItem(ui->scheduleClassesTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
    }
}

void MainWindow::loadResultsDataBase()
{
    ui->resultsTable->setRowCount(0);
    if (!query.exec("SELECT r.ID, r.Date, c.Name, r.Distance, r.PoolSize, a.Athlete, r.Swimming, r.Track, r.Result, r.Place, r.Category "
                    "FROM Results AS r, Competitions AS c, Athletes AS a "
                    "WHERE r.AthleteID = a.ID AND r.CompetitionID = c.ID")){
        ui->statusBar->showMessage("Database table 'Results' not open", 3000);
        return;
    }
    while (query.next()) {
        ui->resultsTable->insertRow(ui->resultsTable->rowCount());
        for (int i = 0; i < 11; ++i) ui->resultsTable->setItem(ui->resultsTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
    }

}

void MainWindow::on_athletesTable_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    ui->tabWidget_2->show();
    ui->tabWidget_3->show();
    int newAthlete =ui->athletesTable->item(row, 0)->text().toInt();
    if (lastAthleteSelected != newAthlete){
        loadAthleteInfoDataBase(ui->athletesTable->item(row, 0)->text());
        lastAthleteSelected = newAthlete;
    }
}

void MainWindow::on_refreshAthletesButton_clicked()
{
    loadAthletesDataBase();
}

void MainWindow::on_competitionsTable_cellClicked(int row, int column)
{
    ui->participantsResultsTable->hide();
    ui->groupBox->hide();
    Q_UNUSED(column);
    ui->participantsResultsTable->setRowCount(0);
    ui->tabWidget_4->show();
    int newCompetition =ui->competitionsTable->item(row, 0)->text().toInt();
    if (lastCompetitionSelected != newCompetition){
        loadCompetitionInfoDataBase(ui->competitionsTable->item(row, 0)->text());
        lastCompetitionSelected = newCompetition;
    }
}

void MainWindow::on_coachesTable_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    ui->tabWidget->show();
    int newCoach =ui->coachesTable->item(row, 0)->text().toInt();
    if (lastCoachSelected != newCoach){
        loadCoachPhotoDataBase(ui->coachesTable->item(row, 0)->text());
        lastCoachSelected = newCoach;
    }
}

void MainWindow::on_inventotyTree_itemClicked(QTreeWidgetItem *item, int column)
{
    QString str = item->text(column);
    ui->inventoryTable->setRowCount(0);
    if (str == "Все"){
        if (!query.exec("SELECT * FROM Inventory")){
            ui->statusBar->showMessage("Database table 'Inventory' not open", 3000);
            return;
        }
    }
    else{
        str = str.split(" ")[1];
        if (!query.exec("SELECT * FROM Inventory WHERE PlaceNumber = " + str)){
            ui->statusBar->showMessage("Database table 'Inventory' not open", 3000);
            return;
        }
    }
    while (query.next()) {
        ui->inventoryTable->insertRow(ui->inventoryTable->rowCount());
        for (int i = 0; i < 3; ++i) ui->inventoryTable->setItem(ui->inventoryTable->rowCount()-1, i, new QTableWidgetItem(query.value(i).toString()));
        ui->inventoryTable->setItem(ui->inventoryTable->rowCount()-1, 3, new QTableWidgetItem("Бассейн " + query.value(3).toString()));
    }
}

void MainWindow::on_participantsParticipantsTable_cellClicked(int row, int column)
{
    if (ui->competitionsTable->item(ui->competitionsTable->currentRow(), 8)->text() == "Запланировано"){
        return;
    }
    ui->groupBox->show();
    ui->participantsResultsTable->show();
    Q_UNUSED(column);
    QString competitionID = ui->competitionsTable->item(ui->competitionsTable->currentRow(), 0)->text();
    if (!query.exec("SELECT AthleteID "
                    "FROM ParticipantsOfCompetitions AS p "
                    "WHERE p.ID = " + ui->participantsParticipantsTable->item(row, 0)->text())){
        ui->statusBar->showMessage("Database table 'Inventory' not open", 3000);
        return;
    }
    while(query.next()) loadCompetitionResultsDataBase(competitionID, query.value(0).toString());
}

void MainWindow::on_refreshCometitionButton_clicked()
{
    loadCompetitionsDataBase();
}

void MainWindow::on_addAthleteButton_clicked()
{
    addAthlete = new AddAthlete(query, this);
    if(addAthlete->exec() == QDialog::Accepted){
        QString* values = addAthlete->getValues();
        QByteArray array = addAthlete->getByteArray();
        query.prepare( "INSERT INTO Athletes (Surname, Name, Patronymic, Athlete, dateOfBirth, Gender, Weight, Height, Address, Telephone, DateOfEnrollment, Photo, CoachID) "
                       "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)" );
        for(int i = 0; i < 6; ++i){
            query.addBindValue(values[i]);
        }
        for(int i = 6; i < 8; ++i){
            query.addBindValue(values[i].toInt());
        }
        query.addBindValue(values[8]);
        query.addBindValue(values[9].toLongLong());
        query.addBindValue(values[10]);
        query.addBindValue(array);
        query.addBindValue(values[11].toInt());
        if(!query.exec()){
            ui->statusBar->showMessage("Can't insert new athlete to database", 3000);
            return;
        }
        emit this->on_refreshAthletesButton_clicked();
    }
}

void MainWindow::on_editAthleteButton_clicked()
{
      if (ui->athletesTable->selectedItems().isEmpty()) return;
      QString ID = ui->athletesTable->selectedItems()[0]->text();
      if (!query.exec("SELECT * FROM Athletes WHERE ID = " +ID)){
          ui->statusBar->showMessage("Database table 'Athlete' not open", 3000);
          return;
      }
      QString* values = new QString[12];
      QByteArray byteArray;
      while(query.next()){
          for (int i = 0; i < 11; ++i) values[i] = query.value(i+1).toString();
          values[11] = query.value(13).toString();
          byteArray = query.value(12).toByteArray();
      }
      addAthlete = new AddAthlete(values, byteArray, query, this);
      if (addAthlete->exec() == QDialog::Accepted){
          values = addAthlete->getValues();
          byteArray = addAthlete->getByteArray();
          query.prepare( "UPDATE Athletes SET Surname=:surname, Name=:name, Patronymic=:pat, Athlete=:athlete, dateOfBirth=:dateOfBirth, Gender=:gender, Weight=:weight, Height=:height, Address=:address, Telephone=:tel, DateOfEnrollment=:dateEn, Photo=:photo, CoachID=:cID "
                         "WHERE ID=:id");
          query.bindValue(":surname", values[0]);
          query.bindValue(":name", values[1]);
          query.bindValue(":pat", values[2]);
          query.bindValue(":athlete", values[3]);
          query.bindValue(":dateOfBirth", values[4]);
          query.bindValue(":gender", values[5]);
          query.bindValue(":weight", values[6].toInt());
          query.bindValue(":height", values[7].toInt());
          query.bindValue(":address", values[8]);
          query.bindValue(":tel", values[9].toLongLong());
          query.bindValue(":dateEn", values[10]);
          query.bindValue(":cID", values[11].toInt());
          query.bindValue(":photo", byteArray);
          query.bindValue(":id", ID.toInt());
          if(!query.exec()){
              ui->statusBar->showMessage("Can't update athlete to database", 3000);
              return;
          }
          emit this->on_refreshAthletesButton_clicked();
      }
}

void MainWindow::on_deleteAthleteButton_clicked()
{
    if (ui->athletesTable->selectedItems().isEmpty()) return;
    QString ID = ui->athletesTable->selectedItems()[0]->text();
    if (!query.exec("DELETE FROM Athletes WHERE ID = " +ID)){
        ui->statusBar->showMessage("Not index", 3000);
        return;
    }
    emit this->on_refreshAthletesButton_clicked();
}

void MainWindow::on_addContactAthleteButton_clicked()
{
    addAthleteContact = new AddAthleteContact(this);
    QString *values;
    if (addAthleteContact->exec() == QDialog::Accepted){
        values = addAthleteContact->getValues();
        query.prepare( "INSERT INTO AthletesContacts (Contact, Who, Work, Post, Telephone, Email, AthleteID) "
                       "VALUES (?,?,?,?,?,?,?)" );
        for (int i = 0; i < 4; ++i){
            query.addBindValue(values[i]);
        }
        query.addBindValue(values[4].toLongLong());
        query.addBindValue(values[5]);
        query.addBindValue(ui->athletesTable->selectedItems()[0]->text().toInt());
        if(!query.exec()){
            ui->statusBar->showMessage("Can't insert athlete contact to database", 3000);
            return;
        }
        int temp = ui->athletesTable->selectionModel()->selectedRows()[0].row();
        emit this->on_refreshAthletesButton_clicked();
        ui->athletesTable->selectRow(temp);
        emit this->on_athletesTable_cellClicked(temp, 0);
    }
}

void MainWindow::on_changeContactAthleteButton_clicked()
{
    QList<QTableWidgetItem*> list = ui->contactsAthleteTable->selectedItems();
    QString *values = new QString[6];
    if (list.isEmpty()) return;
    for (int i = 0; i < 6; ++i){
        values[i] = list[i+1]->text();
    }
    addAthleteContact = new AddAthleteContact(values, this);
    if (addAthleteContact->exec() == QDialog::Accepted){
        values = addAthleteContact->getValues();
        query.prepare( "UPDATE AthletesContacts SET Contact=?, Who=?, Work=?, Post=?, Telephone=?, Email=?, AthleteID=? WHERE ID = " + list[0]->text());
        for (int i = 0; i < 4; ++i){
            query.addBindValue(values[i]);
        }
        query.addBindValue(values[4].toLongLong());
        query.addBindValue(values[5]);
        query.addBindValue(ui->athletesTable->selectedItems()[0]->text().toInt());
        if(!query.exec()){
            ui->statusBar->showMessage("Can't update athlete contact to database", 3000);
            return;
        }
        int temp = ui->athletesTable->selectionModel()->selectedRows()[0].row();
        emit this->on_refreshAthletesButton_clicked();
        ui->athletesTable->selectRow(temp);
        emit this->on_athletesTable_cellClicked(temp, 0);
    }
}

void MainWindow::on_deleteContactAthleteButton_clicked()
{
    if (ui->contactsAthleteTable->selectedItems().isEmpty()) return;
    QString ID = ui->contactsAthleteTable->selectedItems()[0]->text();
    if (!query.exec("DELETE FROM AthletesContacts WHERE ID = " +ID)){
        ui->statusBar->showMessage("Not index", 3000);
        return;
    }
    int temp = ui->athletesTable->selectionModel()->selectedRows()[0].row();
    emit this->on_refreshAthletesButton_clicked();
    ui->athletesTable->selectRow(temp);
    emit this->on_athletesTable_cellClicked(temp, 0);
}

void MainWindow::on_addCometitionButton_clicked()
{
    addCompetition = new AddCompetition(this);
    if (addCompetition->exec() == QDialog::Accepted){
        QString* values = addCompetition->getValues();
        query.prepare( "INSERT INTO Competitions (Name, Category, dateStart, dateEnd, Country, City, Place) "
                       "VALUES (?,?,?,?,?,?,?)" );
        for (int i = 0; i < 7; ++i){
            query.addBindValue(values[i]);
        }
        if(!query.exec()){
            ui->statusBar->showMessage("Can't insert competition to database", 3000);
            return;
        }
        emit this->on_refreshCometitionButton_clicked();
    }
}

void MainWindow::on_editCometitionButton_clicked()
{
    QList<QTableWidgetItem*> list = ui->competitionsTable->selectedItems();
    QString *values = new QString[7];
    if (list.isEmpty()) return;
    for (int i = 0; i < 7; ++i){
        values[i] = list[i+1]->text();
    }
    addCompetition = new AddCompetition(values, this);
    if (addCompetition->exec() == QDialog::Accepted){
        QString* values = addCompetition->getValues();
        query.prepare( "UPDATE Competitions SET Name=?, Category=?, dateStart=?, dateEnd=?, Country=?, City=?, Place=? "
                       "WHERE ID = " + list[0]->text());
        for (int i = 0; i < 7; ++i){
            query.addBindValue(values[i]);
        }
        if(!query.exec()){
            ui->statusBar->showMessage("Can't update competition in database", 3000);
            return;
        }
        emit this->on_refreshCometitionButton_clicked();
    }
}

void MainWindow::on_deleteCometitionButton_2_clicked()
{
    if (ui->competitionsTable->selectedItems().isEmpty()) return;
    QString ID = ui->competitionsTable->selectedItems()[0]->text();
    if (!query.exec("DELETE FROM Competitions WHERE ID = " +ID)){
        ui->statusBar->showMessage("Not index", 3000);
        return;
    }
    emit this->on_refreshCometitionButton_clicked();
}

void MainWindow::on_addJudgeButton_clicked()
{
    addJudges = new AddJudges(this);
    if (addJudges->exec() == QDialog::Accepted){
        QString* values = addJudges->getValues();
        query.prepare( "INSERT INTO JudgesOfCompetitions (Type, Name, CompetitionID) "
                       "VALUES (?,?,?)" );
        for (int i = 0; i < 2; ++i){
            query.addBindValue(values[i]);
        }
        query.addBindValue(ui->competitionsTable->selectedItems()[0]->text().toInt());
        if(!query.exec()){
            ui->statusBar->showMessage("Can't insert judge to database", 3000);
            return;
        }
        int temp = ui->competitionsTable->selectionModel()->selectedRows()[0].row();
        emit this->on_refreshCometitionButton_clicked();
        ui->competitionsTable->selectRow(temp);
        emit this->on_competitionsTable_cellClicked(temp, 0);
    }
}

void MainWindow::on_changeJudgeButton_clicked()
{
    QList<QTableWidgetItem*> list = ui->judgesTable->selectedItems();
    QString *values = new QString[2];
    if (list.isEmpty()) return;
    for (int i = 0; i < 2; ++i)
        values[i] = list[i+1]->text();
    addJudges = new AddJudges(values, this);
    if (addJudges->exec() == QDialog::Accepted){
        QString* values = addJudges->getValues();
        query.prepare( "UPDATE JudgesOfCompetitions SET Type=?, Name=?, CompetitionID=? "
                       "WHERE ID = " + list[0]->text());
        for (int i = 0; i < 2; ++i){
            query.addBindValue(values[i]);
        }
        query.addBindValue(ui->competitionsTable->selectedItems()[0]->text().toInt());
        if(!query.exec()){
            ui->statusBar->showMessage("Can't insert judge to database", 3000);
            return;
        }
        int temp = ui->competitionsTable->selectionModel()->selectedRows()[0].row();
        emit this->on_refreshCometitionButton_clicked();
        ui->competitionsTable->selectRow(temp);
        emit this->on_competitionsTable_cellClicked(temp, 0);
    }
}

void MainWindow::on_deleteJudgeButton_clicked()
{
    if (ui->judgesTable->selectedItems().isEmpty()) return;
    QString ID = ui->judgesTable->selectedItems()[0]->text();
    if (!query.exec("DELETE FROM JudgesOfCompetitions WHERE ID = " +ID)){
        ui->statusBar->showMessage("Not index", 3000);
        return;
    }
    int temp = ui->competitionsTable->selectionModel()->selectedRows()[0].row();
    emit this->on_refreshCometitionButton_clicked();
    ui->competitionsTable->selectRow(temp);
    emit this->on_competitionsTable_cellClicked(temp, 0);
}

void MainWindow::on_addParticipantButton_clicked()
{
    athletesTable = new AthletesTable(query, this);
    if(athletesTable->exec() == QDialog::Accepted){
        int count = 0;
        QString athleteID = athletesTable->getValue();
        QString competitionID = ui->competitionsTable->selectedItems()[0]->text();
        if(!query.exec("SELECT COUNT(*) FROM ParticipantsOfCompetitions WHERE CompetitionID = " + competitionID + " AND AthleteID = " + athleteID)){
            ui->statusBar->showMessage("Can't select ParticipantsOfCompetitions to database", 3000);
            return;
        }
        if(query.first())
               count = query.value(0).toInt();
        if (count > 0){
            ui->statusBar->showMessage("It is not possible to re-select the same participant", 10000);
            return;
        }
        query.prepare("INSERT INTO ParticipantsOfCompetitions (CompetitionID, AthleteID) "
                        "VALUES (?,?)");
        query.addBindValue(competitionID);
        query.addBindValue(athleteID);
        if (!query.exec()){
            ui->statusBar->showMessage("Can't insert ParticipantsOfCompetitions to database", 3000);
            return;
        }
        int temp = ui->competitionsTable->selectionModel()->selectedRows()[0].row();
        emit this->on_refreshCometitionButton_clicked();
        ui->competitionsTable->selectRow(temp);
        emit this->on_competitionsTable_cellClicked(temp, 0);
    }
}

void MainWindow::on_removeParticipantButton_clicked()
{
    if (ui->participantsParticipantsTable->selectedItems().isEmpty()) return;
    QString ID = ui->participantsParticipantsTable->selectedItems()[0]->text();
    if (!query.exec("DELETE FROM ParticipantsOfCompetitions WHERE ID = " +ID)){
        ui->statusBar->showMessage("Not index", 3000);
        return;
    }
    int temp = ui->competitionsTable->selectionModel()->selectedRows()[0].row();
    emit this->on_refreshCometitionButton_clicked();
    ui->competitionsTable->selectRow(temp);
    emit this->on_competitionsTable_cellClicked(temp, 0);
}

void MainWindow::updateAthleteCategory()
{
    std::vector<QString> athletes;
    if (!query.exec("SELECT ID, Category FROM Athletes")){
        ui->statusBar->showMessage("Can't open athletes database", 3000);
        return;
    }
    while(query.next()){
        athletes.push_back(query.value(0).toString());
    }
    for (auto& it : athletes){
        std::vector<QString> results;
        if (!query.exec("SELECT Category FROM Results WHERE AthleteID = " + it)){
            ui->statusBar->showMessage("Can't open result database", 3000);
            return;
        }
        while(query.next()){
            results.push_back(query.value(0).toString());
        }
        bool flag = false;
        QString newRes = "";
        for (auto& category : categoryList){
            for (auto& res : results){
                if (res == category){
                    flag = true;
                    newRes = res;
                    break;
                }
            }
            if (flag) break;
        }
        query.prepare("UPDATE Athletes SET Category=? WHERE ID = " + it);
        query.addBindValue(newRes);
        if (!query.exec()){
            ui->statusBar->showMessage("Can't update athlete category in database", 3000);
            return;
        }
    }
    emit this->on_refreshAthletesButton_clicked();
}

int MainWindow::addRes()
{
    if (ui->scheduleTable->currentIndex() == 1){
        if (!query.exec("SELECT AthleteID FROM ParticipantsOfCompetitions WHERE ID = " +ui->participantsParticipantsTable->selectedItems()[0]->text())){
            ui->statusBar->showMessage("Not athlete", 3000);
            return 1;
        }
        query.first();
        addResult = new AddResult(query.value(0).toString(), ui->competitionsTable->selectedItems()[0]->text(), query, this);
    }
    else{
        addResult = new AddResult(query, this);
    }
    if (addResult->exec() == AddResult::Accepted){
        QString *values = addResult->getValues();
        query.prepare("INSERT INTO Results (Date, Distance, PoolSize, Swimming, Track, Result, Place, Category, AthleteID, CompetitionID) "
                        "VALUES (?,?,?,?,?,?,?,?,?,?)");
        for (int i = 0; i < 2; ++i)query.addBindValue(values[i]);
        for (int i = 2; i < 5; ++i)query.addBindValue(values[i].toInt());
        query.addBindValue(values[5]);
        query.addBindValue(values[6].toInt());
        query.addBindValue(values[7]);
        for (int i = 8; i < 10; ++i)query.addBindValue(values[i].toInt());
        if (!query.exec()){
            ui->statusBar->showMessage("Can't insert result to database", 3000);
            return 1;
        }
   }
    return 0;
}

int MainWindow::changeRes(QString id)
{
    QString *values = new QString[10];
    if (!query.exec("SELECT * FROM Results WHERE ID = " +id)){
        ui->statusBar->showMessage("Can't open Results database", 3000);
        return 1;
    }
    query.first();
    for (int i = 0; i < 10; ++i){
        values[i] = query.value(i+1).toString();
    }
    addResult = new AddResult(values, query, this);
    if (addResult->exec() == AddResult::Accepted){
        QString *values = addResult->getValues();
        query.prepare("UPDATE Results SET Date=?, Distance=?, PoolSize=?, Swimming=?, Track=?, Result=?, Place=?, Category=?, AthleteID=?, CompetitionID=? "
                        "WHERE ID = " + id);
        for (int i = 0; i < 2; ++i)query.addBindValue(values[i]);
        for (int i = 2; i < 5; ++i)query.addBindValue(values[i].toInt());
        query.addBindValue(values[5]);
        query.addBindValue(values[6].toInt());
        query.addBindValue(values[7]);
        for (int i = 8; i < 10; ++i)query.addBindValue(values[i].toInt());
        if (!query.exec()){
            ui->statusBar->showMessage("Can't update result to database", 3000);
            return 1;
        }
    }
    return 0;
}

void MainWindow::on_addResultButton_clicked()
{
    if (ui->participantsParticipantsTable->selectedItems().isEmpty()) return;
    if (addRes()){
        return;
    }
    int temp = ui->competitionsTable->selectionModel()->selectedRows()[0].row();
    int temp2 = ui->participantsParticipantsTable->selectionModel()->selectedRows()[0].row();
    emit this->on_refreshCometitionButton_clicked();
    ui->competitionsTable->selectRow(temp);
    emit this->on_competitionsTable_cellClicked(temp, 0);
    ui->participantsParticipantsTable->selectRow(temp2);
    emit this->on_participantsParticipantsTable_cellClicked(temp2, 0);
}

void MainWindow::on_changeResultButton_clicked()
{
    if (ui->participantsResultsTable->selectedItems().isEmpty()) return;
    if (changeRes(ui->participantsResultsTable->selectedItems()[0]->text())){
        return;
    }
    int temp = ui->competitionsTable->selectionModel()->selectedRows()[0].row();
    int temp2 = ui->participantsParticipantsTable->selectionModel()->selectedRows()[0].row();
    emit this->on_refreshCometitionButton_clicked();
    ui->competitionsTable->selectRow(temp);
    emit this->on_competitionsTable_cellClicked(temp, 0);
    ui->participantsParticipantsTable->selectRow(temp2);
    emit this->on_participantsParticipantsTable_cellClicked(temp2, 0);
}

void MainWindow::on_updateCategories_clicked()
{
    updateAthleteCategory();
}

void MainWindow::on_deleteResultButton_clicked()
{
    if (ui->participantsResultsTable->selectedItems().isEmpty()) return;
    QString ID = ui->participantsResultsTable->selectedItems()[0]->text();
    if (!query.exec("DELETE FROM Results WHERE ID = " +ID)){
        ui->statusBar->showMessage("Not index", 3000);
        return;
    }
    int temp = ui->competitionsTable->selectionModel()->selectedRows()[0].row();
    int temp2 = ui->participantsParticipantsTable->selectionModel()->selectedRows()[0].row();
    emit this->on_refreshCometitionButton_clicked();
    ui->competitionsTable->selectRow(temp);
    emit this->on_competitionsTable_cellClicked(temp, 0);
    ui->participantsParticipantsTable->selectRow(temp2);
    emit this->on_participantsParticipantsTable_cellClicked(temp2, 0);
}

void MainWindow::on_refreshResultButton_clicked()
{
    loadResultsDataBase();
}

void MainWindow::on_refreshScheduleButton_clicked()
{
    loadScheduleDataBase();
}

void MainWindow::on_addCoacheButton_clicked()
{
    addCoach = new AddCoach(this);
    if(addCoach->exec() == QDialog::Accepted){
        QString* values = addCoach->getValues();
        QByteArray array = addCoach->getByteArray();
        query.prepare( "INSERT INTO Coaches (Name, Post, dateOfBirth, Telephone, Email, Photo) "
                       "VALUES (?,?,?,?,?,?)" );
        for(int i = 0; i < 3; ++i){
            query.addBindValue(values[i]);
        }
        query.addBindValue(values[3].toInt());
        query.addBindValue(values[4]);
        query.addBindValue(array);
        if(!query.exec()){
            ui->statusBar->showMessage("Can't insert new coach to database", 3000);
            return;
        }
        emit this->on_refreshCoacheButton_clicked();
    }
}

void MainWindow::on_refreshCoacheButton_clicked()
{
    loadCoachesDataBase();
}

void MainWindow::on_changeCoacheButton_clicked()
{
    if (ui->coachesTable->selectedItems().isEmpty()) return;
    QString ID = ui->coachesTable->selectedItems()[0]->text();
    if (!query.exec("SELECT * FROM Coaches WHERE ID = " + ID)){
        ui->statusBar->showMessage("Database table 'Coaches' not open", 3000);
        return;
    }
    QString* values = new QString[5];
    QByteArray byteArray;
    while(query.next()){
        for (int i = 0; i < 5; ++i) values[i] = query.value(i+1).toString();
        byteArray = query.value(6).toByteArray();
    }
    addCoach = new AddCoach(values, byteArray, this);
    if (addCoach->exec() == QDialog::Accepted){
        values = addCoach->getValues();
        byteArray = addCoach->getByteArray();
        query.prepare( "UPDATE Coaches SET Name=:name, Post=:post, dateOfBirth=:date, Telephone=:telephone, Email=:email, Photo=:photo "
                       "WHERE ID=:id");
        query.bindValue(":name", values[0]);
        query.bindValue(":post", values[1]);
        query.bindValue(":date", values[2]);
        query.bindValue(":telephone", values[3].toLongLong());
        query.bindValue(":email", values[4]);
        query.bindValue(":photo", byteArray);
        query.bindValue(":id", ID.toInt());
        if(!query.exec()){
            ui->statusBar->showMessage("Can't update coach to database", 3000);
            return;
        }
        emit this->on_refreshCoacheButton_clicked();
    }
}

void MainWindow::on_deleteCoacheButton_clicked()
{
    if (ui->coachesTable->selectedItems().isEmpty()) return;
    QString ID = ui->coachesTable->selectedItems()[0]->text();
    if (!query.exec("DELETE FROM Coaches WHERE ID = " +ID)){
        ui->statusBar->showMessage("Not index", 3000);
        return;
    }
    emit this->on_refreshCoacheButton_clicked();
}

void MainWindow::on_addScheduleButton_clicked()
{
    addSchedule = new AddSchedule(query, this);
    if (addSchedule->exec() == AddSchedule::Accepted){
        QString *values = addSchedule->getValues();
        query.prepare( "INSERT INTO Schedule (CoachID, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday) "
                       "VALUES (?,?,?,?,?,?,?,?)" );
        for(int i = 0; i < 8; ++i){
            query.addBindValue(values[i]);
        }
        if(!query.exec()){
            ui->statusBar->showMessage("Can't insert Schedule to database", 3000);
            return;
        }
        emit this->on_refreshScheduleButton_clicked();
    }
}

void MainWindow::on_deleteScheduleButton_clicked()
{
    if (ui->scheduleClassesTable->selectedItems().isEmpty()) return;
    QString ID = ui->scheduleClassesTable->selectedItems()[0]->text();
    if (!query.exec("DELETE FROM Schedule WHERE ID = " +ID)){
        ui->statusBar->showMessage("Not index", 3000);
        return;
    }
    emit this->on_refreshScheduleButton_clicked();
}

void MainWindow::on_changeScheduleButton_clicked()
{
    if (ui->scheduleClassesTable->selectedItems().isEmpty()) return;
    QString* values = new QString[8];
    QString ID = ui->scheduleClassesTable->selectedItems()[0]->text();
    if (!query.exec("SELECT * FROM Schedule WHERE ID = " + ID)){
        ui->statusBar->showMessage("Database table 'Coaches' not open", 3000);
        return;
    }
    while(query.next()){
        for (int i = 0; i < 8; ++i) values[i] = query.value(i+1).toString();
    }
    addSchedule = new AddSchedule(values, query, this);
    if (addSchedule->exec() == AddSchedule::Accepted){
        QString *values = addSchedule->getValues();
        query.prepare( "UPDATE Schedule SET CoachID=?, Monday=?, Tuesday=?, Wednesday=?, Thursday=?, Friday=?, Saturday=?, Sunday=? "
                       "WHERE ID = " + ID );
        for(int i = 0; i < 8; ++i){
            query.addBindValue(values[i]);
        }
        if(!query.exec()){
            ui->statusBar->showMessage("Can't insert Schedule to database", 3000);
            return;
        }
        emit this->on_refreshScheduleButton_clicked();
    }
}

void MainWindow::on_refreshInventoryButton_clicked()
{
    loadInventoryDataBase();
}

void MainWindow::on_addInventoryButton_clicked()
{
    addInventory = new AddInventory(this);
    if (addInventory->exec() == AddInventory::Accepted){
        QString *values = addInventory->getValues();
        query.prepare( "INSERT INTO Inventory (Inventory, Count, PlaceNumber) "
                       "VALUES (?,?,?)" );
        query.addBindValue(values[0]);
        query.addBindValue(values[1].toInt());
        query.addBindValue(values[2].toInt());
        if(!query.exec()){
            ui->statusBar->showMessage("Can't insert Inventory to database", 3000);
            return;
        }
        emit this->on_refreshInventoryButton_clicked();
    }
}

void MainWindow::on_changeInventoryButton_clicked()
{
    if (ui->inventoryTable->selectedItems().isEmpty()) return;
    QString* values = new QString[3];
    QString ID = ui->inventoryTable->selectedItems()[0]->text();
    if (!query.exec("SELECT * FROM Inventory WHERE ID = " + ID)){
        ui->statusBar->showMessage("Database table 'Inventory' not open", 3000);
        return;
    }
    while(query.next()){
        for (int i = 0; i < 2; ++i) values[i] = query.value(i+1).toString();
    }
    addInventory = new AddInventory(values, this);
    if (addInventory->exec() == AddSchedule::Accepted){
        QString *values = addInventory->getValues();
        query.prepare( "UPDATE Inventory SET Inventory=?, Count=?, PlaceNumber=? "
                       "WHERE ID = " + ID );
        query.addBindValue(values[0]);
        query.addBindValue(values[1].toInt());
        query.addBindValue(values[2].toInt());
        if(!query.exec()){
            ui->statusBar->showMessage("Can't update Inventory database", 3000);
            return;
        }
        emit this->on_refreshInventoryButton_clicked();
    }
}

void MainWindow::on_deleteInventoryButton_clicked()
{
    if (ui->inventoryTable->selectedItems().isEmpty()) return;
    QString ID = ui->inventoryTable->selectedItems()[0]->text();
    if (!query.exec("DELETE FROM Inventory WHERE ID = " + ID)){
        ui->statusBar->showMessage("Can't delete Inventory database", 3000);
        return;
    }
    emit this->on_refreshInventoryButton_clicked();
}

void MainWindow::on_newDB_triggered(){
    int ret = QMessageBox::question(this, "Создание новой базы данных", "Создание новой базы данных удалит текущую. Вы согласны?", QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok){
        if (QFile::exists("./../CourseWork/database.db")){
            db.close();
            QFile::remove("./../CourseWork/database.db");
        }
        if(!QFile::copy("./../CourseWork/newDatabase.db", "./../CourseWork/database.db"))
            QMessageBox::information(this,"Ошибка копирования","Не получилось импортировать файл");
        db.open();
    }
}

void MainWindow::on_exportDB_triggered(){
    QString filename = QFileDialog::getSaveFileName(this,"Export Database", QDir::currentPath() + "/database.db", "Databases (*.db)");
    if (filename.isEmpty()) return;
    if (QFile::exists(filename))
        QFile::remove(filename);
    if(!QFile::copy("./../CourseWork/database.db", filename))
        QMessageBox::information(this,"Ошибка копирования","Не получилось экспортировать файл");
}

void MainWindow::on_importDB_triggered(){
    QString filename = QFileDialog::getOpenFileName(this,"Import Database", QDir::currentPath() + "/database.db", "Databases (*.db)");
    if (filename.isEmpty()) return;
    if (QFile::exists("./../CourseWork/database.db")){
        db.close();
        QFile::remove("./../CourseWork/database.db");
    }
    if(!QFile::copy(filename, "./../CourseWork/database.db"))
        QMessageBox::information(this,"Ошибка копирования","Не получилось импортировать файл");
    db.open();
}

void MainWindow::on_athletesTable_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    emit this->on_editAthleteButton_clicked();
}

void MainWindow::on_contactsAthleteTable_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    emit this->on_changeContactAthleteButton_clicked();
}

void MainWindow::on_competitionsTable_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    emit this->on_editCometitionButton_clicked();
}

void MainWindow::on_participantsResultsTable_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    emit this->on_changeResultButton_clicked();
}

void MainWindow::on_scheduleClassesTable_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    emit this->on_changeScheduleButton_clicked();
}

void MainWindow::on_inventoryTable_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    emit this->on_changeInventoryButton_clicked();
}

void MainWindow::on_coachesTable_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    emit this->on_changeCoacheButton_clicked();
}

void MainWindow::timerUpdate()
{
    for (int i = 0; i < ui->competitionsTable->rowCount(); ++i){
        QDate startDate = QDate::fromString(ui->competitionsTable->item(i, 3)->text(), "dd.MM.yyyy");
        QDate endDate = QDate::fromString(ui->competitionsTable->item(i, 4)->text(), "dd.MM.yyyy");
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
        ui->competitionsTable->setItem(i, 8, new QTableWidgetItem(status));
    }
}

void MainWindow::slotInfo(){
    HelpInformation* form = new HelpInformation();
    form->setWindowModality(Qt::ApplicationModal);
    form->show();
}
