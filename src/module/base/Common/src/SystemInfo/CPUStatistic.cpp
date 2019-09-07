#include "CPUStatistic.h"

Json::Value CPUStatistic::toJson()
{
    Json::Value result;
    if (!ok)
    {
        return result;
    }

    result["user"] = (Json::Int64)user;
    result["nice"] = (Json::Int64)nice;
    result["sys"] = (Json::Int64)sys;
    result["idle"] = (Json::Int64)idle;
    result["iowait"] = (Json::Int64)iowait;
    result["irq"] = (Json::Int64)irq;
    result["softirq"] = (Json::Int64)softirq;
    result["steal"] = (Json::Int64)steal;
    result["guest"] = (Json::Int64)guest;
    return result;
}
