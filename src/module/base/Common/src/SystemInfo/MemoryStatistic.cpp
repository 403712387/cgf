#include "MemoryStatistic.h"
Json::Value MemoryStatistic::toJson()
{
    Json::Value result;
    if (!ok)
    {
        return result;
    }
    result["MemActive"] = Json::Int64(MemActive);
    result["RealInUse"] = Json::Int64(RealInUse);
    result["NotInUse"] = Json::Int64(NotInUse);
    result["MemTotal"] = Json::Int64(MemTotal);
    result["MemFree"] = Json::Int64(MemFree);
    result["Buffers"] = Json::Int64(Buffers);
    result["Cached"] = Json::Int64(Cached);
    result["SwapTotal"] = Json::Int64(SwapTotal);
    result["SwapFree"] = Json::Int64(SwapFree);
    return result;
}
