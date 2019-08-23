QT       += core
QT       -= gui
CONFIG += c++11 console
TARGET = cgf
TEMPLATE = app
DESTDIR = ../../bin/cgf
OBJECTS_DIR = ../../output/cgf
MOC_DIR = ../../output/cgf

INCLUDEPATH += \
            ../../module/base/Common/src \
            ../../module/base/Common/src/message \
            ../../module/base/Common/src/perftool \
            ../../module/base/Common/src/info \
            ../../module/base/Common/src/common \
            ../../module/base/CoreModule/src \
            ../../module/base/HttpManager/src \
            ../../module/base/ServiceStatusManager/src \
            ../../module/base/ConfigureManager/src \
            ../../../thirdparty/include

win32 {
LIBS += -L../../bin/cgf/lib/module -lCommon -lCoreModule -lHttpManager -lServiceStatusManager -lConfigureManager \
        -L../../../thirdparty/lib/windows64/Qt -lQt5Core \
        -L../../../thirdparty/lib/windows64/jsoncpp  -ljsoncpp
}

unix {
LIBS += -L../../bin/cgf/lib/module -lCommon -lCoreModule -lHttpManager -lServiceStatusManager -lConfigureManager \
        -L../../../thirdparty/lib/linux64/jsoncpp  -ljsoncpp \
        -L../../../thirdparty/lib/linux64/log4cplus -llog4cplus \
        -L../../../thirdparty/lib/linux64/curl -lcurl \
        -L../../../thirdparty/lib/linux64/Qt -lQt5Core \
        -luuid
}

SOURCES += \
    src/*.cpp
