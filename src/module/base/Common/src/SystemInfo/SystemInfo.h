#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H
#include <memory>
#include <vector>
#include "Base.h"
/*
 * 获取系统的各种信息
*/
class PlatformInfo;
class CPUStatistic;
class DiskStatistic;
class MemoryStatistic;
class ProcessStatistic;
class CGF_EXPORT SystemInfo
{
public:
    // 统计cpu的使用信息
    static std::shared_ptr<CPUStatistic> getCpuInfo();

    // 统计进程信息
    static std::shared_ptr<ProcessStatistic> getProcessInfo();

    // 统计磁盘使用情况
    static std::vector<std::shared_ptr<DiskStatistic>> getDiskInfo();

    // 统计内存使用情况
    static std::shared_ptr<MemoryStatistic> getMemoryInfo();

    // 平台信息
    static std::shared_ptr<PlatformInfo> getPlatformInfo();
};

#endif
