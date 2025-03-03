#-------------------------------------------------
#
# Project created by QtCreator 2019-11-04T11:46:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client_Robot
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    robot/websocket.cpp \
    robot/robot.cpp \
    robotmanager.cpp \
    robot/codec.cpp

HEADERS += \
        mainwindow.h \
    robot/websocket.h \
    robot/robot.h \
    robotmanager.h \
    iocontextpool.h \
    robot/message.h \
    robot/codec.h

FORMS += \
        mainwindow.ui

CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -std=gnu++11 -g
} else {
    QMAKE_CXXFLAGS += -std=gnu++11 -O2
}

INCLUDEPATH += /home/rd2brian/lib/jsoncpp-1.8.4/include \
               /home/rd2brian/lib/boost_1_68_0/ \
               ../ProtocolBuilder/ \

LIBS += /home/rd2brian/lib/jsoncpp-1.8.4/src/lib_json/libjsoncpp.a \
        ../ProtocolBuilder/libProtocolBuilder.a \
        -L/home/rd2brian/lib/boost_1_68_0/stage/lib \
        -lboost_system \
        -lpthread \

LIBS += -lssl -lcrypto \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
