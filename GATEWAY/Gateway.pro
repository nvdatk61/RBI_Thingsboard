# -------------------------------------------------
# Project created by QtCreator 2016-03-31T16:51:26
# -------------------------------------------------
QT += core\
    gui
QT += printsupport
QT += webkitwidgets
QT += webkit
QT += widgets \
    network
QT += sql
greaterThan(QT_MAJOR_VERSION, 4):QT += widgets printsupport
TARGET = Terminal1
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    hled.cpp \
    console.cpp \
    filedata.cpp \
    data.cpp \
    qcustomplot.cpp \
    readfile.cpp \
    confmqtt.cpp \
    define.cpp \
    tranceiverlora.cpp \
    dbmanager.cpp
HEADERS += mainwindow.h \
    hled.h \
    console.h \
    define.h \
    filedata.h \
    data.h \
    qcustomplot.h \
    qtmosq.h \
    readfile.h \
    confmqtt.h \
    tranceiverlora.h \
    dbmanager.h
FORMS += mainwindow.ui \
    
include(qextserialport/qextserialport.pri)
LIBS += -lmosquittopp
LIBS += -lmosquitto
#LIBS += -lwiringPi
#RESOURCES += resources.qrc
#OTHER_FILES += html_js/tools.js \
#   html_js/init.js \
#    html_js/index.html
