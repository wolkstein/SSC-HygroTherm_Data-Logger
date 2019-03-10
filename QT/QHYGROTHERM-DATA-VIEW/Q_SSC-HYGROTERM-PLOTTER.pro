#-------------------------------------------------
#
# Project created by QtCreator 2013-07-25T20:43:22
#
#-------------------------------------------------

QT     += core gui
QT     += serialport
CONFIG += static

# uncomment to disable debug output
#CONFIG += QT_NO_DEBUG_OUTPUT

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SSC_HG_LOGGER_VIEW
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        #qcpdocumentobject.cpp \
        qcustomplot.cpp \
    serialDialog.cpp \
    masterthread.cpp

HEADERS  += mainwindow.h \
         #qcpdocumentobject.h \
         qcustomplot.h \
    serialDialog.h \
    masterthread.h

FORMS    += mainwindow.ui

DISTFILES += \
    img/log.png \
    img/log2.png \
    img/log3.png

RESOURCES += \
    q_ssc_resourcen.qrc


