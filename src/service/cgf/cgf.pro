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
            ../../module/base/StorageManager/src \
            ../../module/base/NetworkManager/src \
            ../../module/base/DeviceManager/src \
            ../../module/base/ServiceStatusManager/src \
            ../../module/base/ConfigureManager/src \
            ../../module/base/RtmpServiceManager/src \
            ../../module/base/ExceptionManager/src \
            ../../module/base/FFmpegIngestManager/src \
            ../../module/base/IngestStatisticManager/src \
            ../../module/base/PublishStatisticManager/src \
            ../../module/business/LiveStreamManager/src \
            ../../module/business/RecordStreamManager/src \
            ../../module/protocol/log/src \
            ../../../thirdparty/include

win32 {
LIBS += -L../../bin/video/lib/module -lCommon -lCoreModule -lStorageManager -lNetworkManager \
        -lServiceStatusManager -lConfigureManager -lExceptionManager -lDeviceManager \
        -lLiveStreamManager -lRecordStreamManager -lFFmpegIngestManager -lPublishStatisticManager \
        -lIngestStatisticManager \
        -L../../bin/video/lib/module/protocol -llog \
        -L../../../thirdparty/lib/windows64/protobufModule -llibprotobuf \
        -L../../../thirdparty/lib/windows64/Qt -lQt5Core \
        -L../../../thirdparty/lib/windows64/jsoncpp  -ljsoncpp
}

unix {
LIBS += -L../../bin/video/lib/module -lCommon -lCoreModule -lStorageManager -lNetworkManager \
        -lServiceStatusManager -lConfigureManager -lExceptionManager -lDeviceManager \
        -lLiveStreamManager -lRecordStreamManager -lRtmpServiceManager -lFFmpegIngestManager -lPublishStatisticManager \
        -lIngestStatisticManager \
        -L../../bin/video/lib/module/protocol -llog -lHttpApiServer -lHttpServer -lRtmpServer -lCommonSocket \
        -L../../../thirdparty/lib/linux64/protobufModule -lprotobuf -lProtoBufModule \
        -L../../../thirdparty/lib/linux64/jsoncpp  -ljsoncpp \
        -L../../../thirdparty/lib/linux64/log4cplus -llog4cplus \
        -L../../../thirdparty/lib/linux64/curl -lcurl \
        -L../../../thirdparty/lib/linux64/Qt -lQt5Core \
        -L../../../thirdparty/lib/linux64/openssl -lcrypto -lssl \
        -L../../../thirdparty/lib/linux64/ffmpeg  -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale \
        -L../../../thirdparty/lib/linux64/st -lst \
        -luuid
}

SOURCES += \
    src/main.cpp 
