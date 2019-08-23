QT       += core
QT       -= gui
CONFIG += c++11
TARGET = NetworkManager
TEMPLATE = lib
DESTDIR = ../../../bin/cgf/lib/module
DEFINES += PROTOBUF_USE_DLLS
OBJECTS_DIR = ../../../output/NetworkManager
MOC_DIR = ../../../output/NetworkManager

INCLUDEPATH += ../Common/src \
            ../../../../thirdparty/include \
            ../../../../thirdparty/include/Qt/QtCore \
            ../Common/src/message \
            ../Common/src/info \
            ../Common/src/common \
            ./src \
            ./src/helper \
            ../CoreModule/src

win32 {
LIBS += -L../../../bin/video/lib/module -lCommon -lCoreModule -lCommon \
        -L../../../../thirdparty/lib/windows64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/windows64/libmicrohttpd -llibmicrohttpd-dll \
        -L../../../../thirdparty/lib/windows64/jsoncpp -ljsoncpp
}

unix {
LIBS += -L../../../bin/video/lib/module -lCommon -lCoreModule -lCommon \
        -L../../../../thirdparty/lib/linux64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/linux64/libmicrohttpd  -lmicrohttpd \
        -L../../../../thirdparty/lib/linux64/jsoncpp  -ljsoncpp
}

HEADERS += \
    src/*.h \
    src/helper/*.h

SOURCES += \
    src/*.cpp \
    src/helper/*.cpp
