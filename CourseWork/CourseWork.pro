#-------------------------------------------------
#
# Project created by QtCreator 2023-10-14T11:59:29
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CourseWork
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    addathlete.cpp \
    athletestable.cpp \
    coahestable.cpp \
    addathletecontact.cpp \
    addcompetition.cpp \
    addjudges.cpp \
    addresult.cpp \
    competitionstable.cpp \
    addschedule.cpp \
    addcoach.cpp \
    addinventory.cpp \
    helpinformation.cpp

HEADERS += \
        mainwindow.h \
    addathlete.h \
    athletestable.h \
    coahestable.h \
    addathletecontact.h \
    addcompetition.h \
    addjudges.h \
    addresult.h \
    competitionstable.h \
    addschedule.h \
    addcoach.h \
    addinventory.h \
    helpinformation.h

FORMS += \
        mainwindow.ui \
    addathlete.ui \
    athletestable.ui \
    coahestable.ui \
    addathletecontact.ui \
    addcompetition.ui \
    addjudges.ui \
    addresult.ui \
    competitionstable.ui \
    addschedule.ui \
    addcoach.ui \
    addinventory.ui \
    helpinformation.ui

RESOURCES += \
    resourses.qrc
