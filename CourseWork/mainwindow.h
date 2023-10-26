#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QDate>
#include <QTreeWidget>
#include <QTime>
#include <QTimer>
#include <QByteArray>
#include <vector>

#include "addathlete.h"
#include "addathletecontact.h"
#include "addcompetition.h"
#include "addjudges.h"
#include "athletestable.h"
#include "addresult.h"
#include "addcoach.h"
#include "addschedule.h"
#include "addinventory.h"
#include "helpinformation.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_athletesTable_cellClicked(int row, int column);

    void on_refreshAthletesButton_clicked();

    void on_competitionsTable_cellClicked(int row, int column);

    void on_coachesTable_cellClicked(int row, int column);

    void on_inventotyTree_itemClicked(QTreeWidgetItem *item, int column);

    void on_participantsParticipantsTable_cellClicked(int row, int column);

    void on_refreshCometitionButton_clicked();

    void on_addAthleteButton_clicked();

    void on_editAthleteButton_clicked();

    void on_deleteAthleteButton_clicked();

    void on_addContactAthleteButton_clicked();

    void on_changeContactAthleteButton_clicked();

    void on_deleteContactAthleteButton_clicked();

    void on_addCometitionButton_clicked();

    void on_editCometitionButton_clicked();

    void on_deleteCometitionButton_2_clicked();

    void on_addJudgeButton_clicked();

    void on_changeJudgeButton_clicked();

    void on_deleteJudgeButton_clicked();

    void on_addParticipantButton_clicked();

    void on_removeParticipantButton_clicked();

    void on_addResultButton_clicked();

    void on_changeResultButton_clicked();

    void on_updateCategories_clicked();

    void on_deleteResultButton_clicked();

    void on_refreshResultButton_clicked();

    void on_refreshScheduleButton_clicked();

    void on_addCoacheButton_clicked();

    void on_refreshCoacheButton_clicked();

    void on_changeCoacheButton_clicked();

    void on_deleteCoacheButton_clicked();

    void on_addScheduleButton_clicked();

    void on_deleteScheduleButton_clicked();

    void on_changeScheduleButton_clicked();

    void on_refreshInventoryButton_clicked();

    void on_addInventoryButton_clicked();

    void on_changeInventoryButton_clicked();

    void on_deleteInventoryButton_clicked();

    void on_newDB_triggered();

    void on_exportDB_triggered();

    void on_importDB_triggered();

    void on_athletesTable_cellDoubleClicked(int row, int column);

    void on_contactsAthleteTable_cellDoubleClicked(int row, int column);

    void on_competitionsTable_cellDoubleClicked(int row, int column);

    void on_participantsResultsTable_cellDoubleClicked(int row, int column);

    void on_scheduleClassesTable_cellDoubleClicked(int row, int column);

    void on_inventoryTable_cellDoubleClicked(int row, int column);

    void on_coachesTable_cellDoubleClicked(int row, int column);

    void timerUpdate();

    void slotInfo();

private:
    Ui::MainWindow *ui;
    QSettings* settings;
    QSqlDatabase db;
    QSqlQuery query;
    QTimer *timer;
    std::vector<QString> categoryList;
    int lastAthleteSelected;
    int lastCompetitionSelected;
    int lastCoachSelected;
    void updateAthleteCategory();
    int addRes();
    int changeRes(QString);
    void saveSettings();
    void loadSettings();
    void tableSettings();
    void loadAthletesDataBase();
    void loadAthleteInfoDataBase(QString);
    void loadAthletePhotoDataBase(QString);
    void loadAthleteContactsDataBase(QString);
    void loadAthleteResultsDataBase(QString);
    void loadCompetitionsDataBase();
    void loadCompetitionInfoDataBase(QString);
    void loadCompetitionJudgesDataBase(QString);
    void loadCompetitionParticipantsDataBase(QString);
    void loadCompetitionResultsDataBase(QString, QString);
    void loadCoachesDataBase();
    void loadCoachPhotoDataBase(QString);
    void loadInventoryDataBase();
    void loadCategotiesDataBase();
    void loadScheduleDataBase();
    void loadResultsDataBase();

    AddAthlete *addAthlete;
    AddAthleteContact *addAthleteContact;
    AddCompetition *addCompetition;
    AddJudges *addJudges;
    AthletesTable *athletesTable;
    AddResult *addResult;
    AddCoach *addCoach;
    AddSchedule *addSchedule;
    AddInventory *addInventory;
};

#endif // MAINWINDOW_H
