QT       += core
QT       -= gui
CONFIG += c++11
TARGET = HttpManager
TEMPLATE = lib
DESTDIR = ../../../bin/cgf/lib/module
OBJECTS_DIR = ../../../output/HttpManager
MOC_DIR = ../../../output/HttpManager
QMAKE_LFLAGS += -fPIC

INCLUDEPATH += ../Common/src \
            ../../../../thirdparty/include \
            ../../../../thirdparty/include/Qt \
            ../Common/src/message \
            ../Common/src/info \
            ../Common/src/common \
            ./src \
            ./src/helper \
            ../CoreModule/src

win32 {
LIBS += -L../../../bin/cgf/lib/module -lCommon -lCoreModule -lCommon \
        -L../../../../thirdparty/lib/windows64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/windows64/libmicrohttpd -llibmicrohttpd-dll \
        -L../../../../thirdparty/lib/windows64/jsoncpp -ljsoncpp
}

unix {
LIBS += -L../../../bin/cgf/lib/module -lCommon -lCoreModule -lCommon \
        -L../../../../thirdparty/lib/linux64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/linux64/gperftools/static -lprofiler -ltcmalloc \
        -L../../../../thirdparty/lib/linux64/libmicrohttpd  -lmicrohttpd \
        -L../../../../thirdparty/lib/linux64/jsoncpp  -ljsoncpp
}

HEADERS += \
    src/*.h \
    src/helper/*.h

SOURCES += \
    src/*.cpp \
    src/helper/*.cpp
