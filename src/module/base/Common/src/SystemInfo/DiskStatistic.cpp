#include "DiskStatistic.h"
Json::Value DiskStatistic::toJson()
{
    Json::Value result;
    if (!ok)
    {
        return result;
    }

    result["name"] = name;
    result["pgpgin"] = Json::Int64(pgpgin);
    result["pgpgout"] = Json::Int64(pgpgout);
    result["rd_ios"] = Json::Int64(rd_ios);
    result["rd_merges"] = Json::Int64(rd_merges);
    result["rd_sectors"] = Json::Int64(rd_sectors);
    result["rd_ticks"] = Json::Int64(rd_ticks);
    result["wr_ios"] = Json::Int64(wr_ios);
    result["wr_merges"] = Json::Int64(wr_merges);
    result["wr_sectors"] = Json::Int64(wr_sectors);
    result["wr_ticks"] = Json::Int64(wr_ticks);
    result["nb_current"] = Json::Int64(nb_current);
    result["ticks"] = Json::Int64(ticks);
    result["aveq"] = Json::Int64(aveq);
    return result;
}
