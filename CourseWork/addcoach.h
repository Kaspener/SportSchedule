#ifndef ADDCOACH_H
#define ADDCOACH_H

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QBuffer>

namespace Ui {
class AddCoach;
}

class AddCoach : public QDialog
{
    Q_OBJECT

public:
    explicit AddCoach(QWidget *parent = 0);
    AddCoach(QString *values, QByteArray byteArray, QWidget *parent = 0);
    QString *getValues();
    QByteArray getByteArray();
    ~AddCoach();

private slots:
    void on_deleteButton_clicked();

    void on_uploadButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::AddCoach *ui;
    QString* values;
    QImage photo;
    QByteArray byteArray;
    QString formatFile;
};

#endif // ADDCOACH_H
