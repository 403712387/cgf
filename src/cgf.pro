TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS += \
        ./module/base/Common/Common.pro \
        ./module/base/CoreModule/CoreModule.pro \
        ./module/base/HttpManager/HttpManager.pro \
        ./module/base/ServiceStatusManager/ServiceStatusManager.pro \
        ./module/base/ConfigureManager/ConfigureManager.pro \
        ./service/cgf/cgf.pro
