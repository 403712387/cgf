QT       += core
QT       -= gui
CONFIG += c++11

TARGET = Common
TEMPLATE = lib
DESTDIR = ../../../bin/cgf/lib/module
OBJECTS_DIR = ../../../output/Common
MOC_DIR = ../../../output/Common
QMAKE_LFLAGS += -fPIC

INCLUDEPATH += ../../../../thirdparty/include \
            ../../../../thirdparty/include/Qt \
            ../../protocol/log/src \
            src \
            src/common \
            src/info \
            src/log \
            src/perftool \
            src/thread \
            src/config/StorageConfig \
            src/message

win32 {
LIBS += -L../../../../thirdparty/lib/windows64/curl -llibcurl \
        -L../../../../thirdparty/lib/windows64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/windows64/log4cplus -llog4cplusU \
        -L../../../../thirdparty/lib/windows64/jsoncpp -ljsoncpp
}

unix {
LIBS += -L../../../../thirdparty/lib/linux64/curl -lcurl \
        -L../../../../thirdparty/lib/linux64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/linux64/log4cplus -llog4cplus \
        #-L../../../../thirdparty/lib/linux64/gperftools/static -lprofiler -ltcmalloc \
        -L../../../../thirdparty/lib/linux64/jsoncpp -ljsoncpp
}

HEADERS += \
    src/*.h \
    src/common/*.h \
    src/message/*.h \
    src/info/*.h \
    src/log/*.h \
    src/thread/*.h \
    src/perftool/*.h

SOURCES += \
    src/*.cpp \
    src/common/*.cpp \
    src/message/*.cpp \
    src/info/*.cpp \
    src/log/*.cpp \
    src/thread/*.cpp \
    src/perftool/*.cpp

