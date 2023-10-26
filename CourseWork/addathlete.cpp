#include "addathlete.h"
#include "ui_addathlete.h"

AddAthlete::AddAthlete(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAthlete)
{
    ui->setupUi(this);
    ui->dateOfBirth->setMaximumDate(QDate::currentDate());
    ui->dateOfEnrollment->setMaximumDate(QDate::currentDate());
    ui->dateOfEnrollment->setDate(QDate::currentDate());
    emit ui->deleteButton->click();
}

AddAthlete::AddAthlete(QSqlQuery query, QWidget *parent):
    QDialog(parent),
    ui(new Ui::AddAthlete)
{
    values = new QString[12];
    ui->setupUi(this);
    ui->dateOfBirth->setMaximumDate(QDate::currentDate());
    ui->dateOfEnrollment->setMaximumDate(QDate::currentDate());
    ui->dateOfEnrollment->setDate(QDate::currentDate());
    emit ui->deleteButton->click();
    this->query = query;
}

AddAthlete::AddAthlete(QString *values, QByteArray byteArray, QSqlQuery query, QWidget *parent):
    QDialog(parent),
    ui(new Ui::AddAthlete)
{
    ui->setupUi(this);
    ui->dateOfBirth->setMaximumDate(QDate::currentDate());
    ui->dateOfEnrollment->setMaximumDate(QDate::currentDate());
    this->query = query;
    this->values = values;
    this->byteArray = byteArray;
    ui->Surname->setText(values[0]);
    ui->Name->setText(values[1]);
    ui->Patronymic->setText(values[2]);
    ui->dateOfBirth->setDate(QDate::fromString(values[4], "dd.MM.yyyy"));
    ui->Gender->setCurrentText(values[5]);
    ui->Weight->setText(values[6]);
    ui->Height->setText(values[7]);
    ui->Address->setText(values[8]);
    ui->Telephone->setText(values[9]);
    ui->dateOfEnrollment->setDate(QDate::fromString(values[10], "dd.MM.yyyy"));
    if (!query.exec("SELECT Name FROM Coaches WHERE ID = " + values[11])){
        return;
    }
    while(query.next())ui->Coach->setText(query.value(0).toString());
    QPixmap pix = QPixmap();
    pix.loadFromData(byteArray);
    ui->photo->setPixmap(pix);
}

QString* AddAthlete::getValues()
{
    return values;
}

QByteArray AddAthlete::getByteArray()
{
    return byteArray;
}

AddAthlete::~AddAthlete()
{
    delete ui;
}

void AddAthlete::on_uploadButton_clicked()
{
    QString photoname = QFileDialog::getOpenFileName(this, "Open file", QDir::currentPath(), "Images (*.png *.jpeg *.jpg)");
    if (!photoname.isEmpty()){
        QStringList lst = photoname.split('.');
        formatFile = lst[lst.size()-1];
        formatFile = formatFile.toUpper();
        photo.load(photoname);
        ui->photo->setPixmap(QPixmap::fromImage(photo));
    }
}

void AddAthlete::on_deleteButton_clicked()
{
    photo.load(":/resourses/images/noImage.jpg");
    ui->photo->setPixmap(QPixmap::fromImage(photo));
    formatFile = "JPG";
}

void AddAthlete::on_buttonBox_accepted()
{
    bool flag;
    ui->Telephone->text().toLongLong(&flag);
    if (!flag){
        QMessageBox::critical(this, "Ошибка телефона", "Телефон может содержать только цифры от 0 до 9", QMessageBox::Ok);
        return;
    }
    ui->Weight->text().toLongLong(&flag);
    if (!flag){
        QMessageBox::critical(this, "Ошибка веса", "Вес может содержать только цифры от 0 до 9", QMessageBox::Ok);
        return;
    }
    ui->Height->text().toLongLong(&flag);
    if (!flag){
        QMessageBox::critical(this, "Ошибка роста", "Рост может содержать только цифры от 0 до 9", QMessageBox::Ok);
        return;
    }
    if (ui->Telephone->text().length() > 11){
        QMessageBox::critical(this, "Ошибка телефона", "Должно быть не более 11 цифр", QMessageBox::Ok);
        return;
    }
    QDate temp = ui->dateOfBirth->date();
    QDate temp2 = ui->dateOfEnrollment->date();
    if (temp2 < temp) {QMessageBox::critical(this, "Ошибка даты", "Дата поступления не может меньше больше даты рождения", QMessageBox::Ok); return;}
    values[0] = ui->Surname->text();
    values[1] = ui->Name->text();
    values[2] = ui->Patronymic->text();
    for (int i = 0; i < 2; ++i){
        values[i] = values[i].toLower();
        values[i][0] = values[i][0].toUpper();
    }
    QString name = values[0] + " " + values[1][0] + '.';
    if (values[2]!="") {
        values[2] = values[2].toLower();
        values[2][0] = values[2][0].toUpper();
        name += " " + values[2][0] + '.';
    }
    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < values[i].length(); ++j){
            if (!values[i][j].isLetter()){
                QMessageBox::critical(this, "Ошибка строк", "В имени, фамилии или же отчестве могут быть только буквы", QMessageBox::Ok);
                return;
            }
        }
    }
    values[3] = name;
    values[4] = ui->dateOfBirth->text();
    values[5] = ui->Gender->currentText();
    values[6] = ui->Weight->text();
    values[7] = ui->Height->text();
    values[8] = ui->Address->text();
    values[9] = ui->Telephone->text();
    values[10] = ui->dateOfEnrollment->text();
    for (int i = 0; i < 11; ++i){
        if (i!=2){
            if (values[i] == ""){
                QMessageBox::critical(this, "Ошибка заполнения", "Можно не заполнять только поле 'Отчество'", QMessageBox::Ok);
                return;
            }
        }
    }
    if (ui->Coach->text().isEmpty()) {
        QMessageBox::critical(this, "Ошибка заполнения", "Можно не заполнять только поле 'Отчество'", QMessageBox::Ok);
        return;
    }
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    if (formatFile == "PNG") photo.save(&buffer, "PNG");
    else if (formatFile == "JPG") photo.save(&buffer, "JPG");
    else photo.save(&buffer, "JPEG");
    emit this->accept();
}

void AddAthlete::on_CoachSelect_clicked()
{
    coaches = new CoahesTable(query, this);
    if(coaches->exec() == QDialog::Accepted){
        ui->Coach->setText(coaches->getValues()[1]);
        values[11] = coaches->getValues()[0];
    }
}
