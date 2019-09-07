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
    result["rd_ios"] = rd_ios;
    result["rd_merges"] = rd_merges;
    result["rd_sectors"] = rd_sectors;
    result["rd_ticks"] = rd_ticks;
    result["wr_ios"] = wr_ios;
    result["wr_merges"] = wr_merges;
    result["wr_sectors"] = wr_sectors;
    result["wr_ticks"] = wr_ticks;
    result["nb_current"] = nb_current;
    result["ticks"] = ticks;
    result["aveq"] = aveq;
    return result;
}
