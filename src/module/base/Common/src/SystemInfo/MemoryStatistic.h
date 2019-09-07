#ifndef MEMORY_STATISTIC_H
#define MEMORY_STATISTIC_H
#include "Base.h"
#include "jsoncpp/json.h"
/*
 * 统计内存使用情况
*/
class SystemInfo;
class CGF_EXPORT MemoryStatistic
{
    friend class SystemInfo;
public:
    Json::Value toJson();
private:
    // Whether the data is ok.
    bool ok = false;
    // The time in ms when sample.
    long long sample_time = 0;
    // The percent of usage. 0.153 is 15.3%.
    float percent_ram = 0;
    float percent_swap = 0;

    // data of /proc/meminfo
public:
    // MemActive = MemTotal - MemFree
    uint64_t MemActive = 0;
    // RealInUse = MemActive - Buffers - Cached
    uint64_t RealInUse = 0;
    // NotInUse = MemTotal - RealInUse
    //          = MemTotal - MemActive + Buffers + Cached
    //          = MemTotal - MemTotal + MemFree + Buffers + Cached
    //          = MemFree + Buffers + Cached
    uint64_t NotInUse = 0;

    unsigned long MemTotal = 0;
    unsigned long MemFree = 0;
    unsigned long Buffers = 0;
    unsigned long Cached = 0;
    unsigned long SwapTotal = 0;
    unsigned long SwapFree = 0;
};

#endif
