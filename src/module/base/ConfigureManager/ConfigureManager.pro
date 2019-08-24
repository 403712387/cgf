QT       += core
QT       -= gui
CONFIG += c++11
TARGET = ConfigureManager
TEMPLATE = lib
DESTDIR = ../../../bin/cgf/lib/module
OBJECTS_DIR = ../../../output/ConfigureManager
MOC_DIR = ../../../output/ConfigureManager

INCLUDEPATH += ../Common/src \
            ../Common/src/info \
            ./src/common \
            ../../protocol/log/src \
            ../../../../thirdparty/include \
            ../../../../thirdparty/include/Qt/QtCore \
            ../Common/src/message \
            ../CoreModule/src
win32 {
LIBS += -L../../../bin/cgf/lib/module -lCommon -lCoreModule \
        -L../../../../thirdparty/lib/windows64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/windows64/jsoncpp -ljsoncpp
}

unix {
LIBS += -L../../../bin/cgf/lib/module -lCommon -lCoreModule \
        -L../../../../thirdparty/lib/linux64/Qt -lQt5Core \
        -L../../../bin/video/lib/module/protocol -llog \
        -L../../../../thirdparty/lib/linux64/jsoncpp -ljsoncpp
}

HEADERS += \
    src/*.h \
    src/common/*.h

SOURCES += \
    src/*.cpp \
    src/common/*.cpp