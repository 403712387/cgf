TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS += \
        ./service/cgf/cgf.pro \
        ./module/base/Common/Common.pro \
        ./module/base/CoreModule/CoreModule.pro \
        ./module/base/HttpManager/HttpManager.pro \
        ./module/base/ServiceStatusManager/ServiceStatusManager.pro \
        ./module/base/ConfigureManager/ConfigureManager.pro 
