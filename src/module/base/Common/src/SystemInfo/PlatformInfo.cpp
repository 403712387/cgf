#include "PlatformInfo.h"
Json::Value PlatformInfo::toJson()
{
    Json::Value result;
    if (!ok)
    {
        return result;
    }

    result["srs_startup_time"] = srs_startup_time;
    result["os_uptime"] = os_uptime;
    result["os_ilde_time"] = os_ilde_time;
    result["load_one_minutes"] = load_one_minutes;
    result["load_five_minutes"] = load_five_minutes;
    result["load_fifteen_minutes"] = load_fifteen_minutes;
    return result;
}
