#include "addcoach.h"
#include "ui_addcoach.h"

AddCoach::AddCoach(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCoach)
{
    ui->setupUi(this);
    ui->dateOfBirth->setMaximumDate(QDate::currentDate());
    emit this->on_deleteButton_clicked();
    values = new QString[5];
}

AddCoach::AddCoach(QString *values, QByteArray byteArray, QWidget *parent):
    QDialog(parent),
    ui(new Ui::AddCoach)
{
    ui->setupUi(this);
    this->values = values;
    this->byteArray = byteArray;
    ui->name->setText(values[0]);
    ui->post->setText(values[1]);
    ui->dateOfBirth->setDate(QDate::fromString(values[2], "dd.MM.yyyy"));
    ui->telephone->setText(values[3]);
    ui->email->setText(values[4]);
    QPixmap pix = QPixmap();
    pix.loadFromData(byteArray);
    ui->photo->setPixmap(pix);
}

QString *AddCoach::getValues()
{
    return values;
}

QByteArray AddCoach::getByteArray()
{
    return byteArray;
}

AddCoach::~AddCoach()
{
    delete ui;
}

void AddCoach::on_deleteButton_clicked()
{
    photo.load(":/resourses/images/noImage.jpg");
    ui->photo->setPixmap(QPixmap::fromImage(photo));
    formatFile = "JPG";
}

void AddCoach::on_uploadButton_clicked()
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

void AddCoach::on_buttonBox_accepted()
{
    bool flag;
    ui->telephone->text().toLongLong(&flag);
    if (!flag){
        QMessageBox::critical(this, "Ошибка телефона", "Телефон может содержать только цифры от 0 до 9", QMessageBox::Ok);
        return;
    }
    values[0] = ui->name->text();
    values[1] = ui->post->text();
    values[2] = ui->dateOfBirth->text();
    values[3] = ui->telephone->text();
    if(values[3].length() > 11){
        QMessageBox::critical(this, "Ошибка телефона", "Должно быть не более 11 цифр", QMessageBox::Ok);
        return;
    }
    values[4] = ui->email->text();
    for (int i = 0; i < 4; ++i){
        if (values[i].isEmpty()){
            QMessageBox::critical(this, "Ошибка заполнения", "Можно не заполнять только Email", QMessageBox::Ok);
            return;
        }
    }
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    if (formatFile == "PNG") photo.save(&buffer, "PNG");
    else if (formatFile == "JPG") photo.save(&buffer, "JPG");
    else photo.save(&buffer, "JPEG");
    emit this->accept();
}
