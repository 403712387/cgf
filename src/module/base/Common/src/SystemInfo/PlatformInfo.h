#ifndef PLATFORM_INFO_H
#define PLATFORM_INFO_H
#include "Base.h"
#include "jsoncpp/json.h"
/*
 * 平台信息
*/
class SystemInfo;
class PlatformInfo
{
    friend class SystemInfo;
public:
    Json::Value toJson();

private:
    // Whether the data is ok.
    bool ok = false;

    // srs startup time, in ms.
    int64_t srs_startup_time = 0;

private:
    // @see: cat /proc/uptime
    // system startup time in seconds.
    double os_uptime = 0;
    // system all cpu idle time in seconds.
    // @remark to cal the cpu ustime percent:
    //      os_ilde_time % (os_uptime * SrsCpuInfo.nb_processors_online)
    double os_ilde_time = 0;

    // @see: cat /proc/loadavg
    double load_one_minutes = 0;
    double load_five_minutes = 0;
    double load_fifteen_minutes = 0;
};

#endif
