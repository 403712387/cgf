QT       -= gui
CONFIG += c++11

TARGET = CoreModule
TEMPLATE = lib
DESTDIR = ../../..//bin/cgf/lib/module
OBJECTS_DIR = ../../../output/CoreModule
MOC_DIR = ../../../output/CoreModule
DEFINES += QT_NO_VERSION_TAGGING
#QMAKE_LFLAGS += -fPIC

INCLUDEPATH += ../Common/src \
            ../Common/src/info \
            ../Common/src/message \
            ../../../../thirdparty/include/log4plus/windows \
            ../../../../thirdparty/include

win32 {
LIBS += -L../../../bin/cgf/lib/module -lCommon
}

unix {
LIBS += \
        #-L../../../../thirdparty/lib/linux64/gperftools/static -lprofiler -ltcmalloc
        -L../../../bin/cgf/lib/module -lCommon

}

HEADERS += \
    src/*.h

SOURCES += \
    src/*.cpp
