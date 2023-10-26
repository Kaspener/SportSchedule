#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Kaspenium");
    QApplication::setApplicationName("Спортивная школа");
    a.setWindowIcon(QIcon(":/resourses/images/icon.png"));
    QFile file(":/resourses/style/style.qss");
    file.open(QFile::ReadOnly);
    a.setStyleSheet(file.readAll());
    MainWindow w;
    w.show();

    return a.exec();
}
