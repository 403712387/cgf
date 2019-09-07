#include <stdio.h>
#include "SystemInfo.h"
#include "PlatformInfo.h"
#include "CPUStatistic.h"
#include "DiskStatistic.h"
#include "MemoryStatistic.h"
#include "ProcessStatistic.h"

// 统计cpu的使用信息
std::shared_ptr<CPUStatistic> SystemInfo::getCpuInfo()
{
    std::shared_ptr<CPUStatistic> result;
#ifdef WIN32
    return result;
#endif
    FILE* f = fopen("/proc/stat", "r");
    if (f == NULL)
    {
        return result;
    }

    static char buf[1024];
    CPUStatistic r;
    while (fgets(buf, sizeof(buf), f))
    {
        if (strncmp(buf, "cpu ", 4) != 0)
        {
            continue;
        }

        // @see: read_stat_cpu() from https://github.com/sysstat/sysstat/blob/master/rd_stats.c#L88
        // @remark, ignore the filed 10 cpu_guest_nice
        sscanf(buf + 5, "%llu %llu %llu %llu %llu %llu %llu %llu %llu\n",
               &r.user,
               &r.nice,
               &r.sys,
               &r.idle,
               &r.iowait,
               &r.irq,
               &r.softirq,
               &r.steal,
               &r.guest);

        break;
    }

    fclose(f);
    r.ok = true;
    result = std::make_shared<CPUStatistic>(r);
    return result;
}

// 统计进程信息
std::shared_ptr<ProcessStatistic> SystemInfo::getProcessInfo()
{
    std::shared_ptr<ProcessStatistic> result;
#ifdef WIN32
    return result;
#endif
    FILE* f = fopen("/proc/self/stat", "r");
    if (f == NULL)
    {
        return result;
    }

    ProcessStatistic r;
    char comm[32] = {0};
    fscanf(f, "%d %32s %c %d %d %d %d "
           "%d %u %lu %lu %lu %lu "
           "%lu %lu %ld %ld %ld %ld "
           "%ld %ld %llu %lu %ld "
           "%lu %lu %lu %lu %lu "
           "%lu %lu %lu %lu %lu "
           "%lu %lu %lu %d %d "
           "%u %u %llu "
           "%lu %ld",
           &r.pid, comm, &r.state, &r.ppid, &r.pgrp, &r.session, &r.tty_nr,
           &r.tpgid, &r.flags, &r.minflt, &r.cminflt, &r.majflt, &r.cmajflt,
           &r.utime, &r.stime, &r.cutime, &r.cstime, &r.priority, &r.nice,
           &r.num_threads, &r.itrealvalue, &r.starttime, &r.vsize, &r.rss,
           &r.rsslim, &r.startcode, &r.endcode, &r.startstack, &r.kstkesp,
           &r.kstkeip, &r.signal, &r.blocked, &r.sigignore, &r.sigcatch,
           &r.wchan, &r.nswap, &r.cnswap, &r.exit_signal, &r.processor,
           &r.rt_priority, &r.policy, &r.delayacct_blkio_ticks,
           &r.guest_time, &r.cguest_time);

    r.comm = comm;
    fclose(f);
    r.ok = true;

    result = std::make_shared<ProcessStatistic>(r);
    return result;
}

// 统计磁盘使用情况
std::vector<std::shared_ptr<DiskStatistic>> SystemInfo::getDiskInfo()
{
    std::vector<std::shared_ptr<DiskStatistic>> result;
#ifdef WIN32
    return result;
#endif
    FILE* f = fopen("/proc/diskstats", "r");
    if (f == NULL)
    {
        return result;
    }

    static char buf[1024] = {0};
    while (fgets(buf, sizeof(buf), f))
    {
        unsigned int major = 0;
        unsigned int minor = 0;
        static char name[32];
        unsigned int rd_ios = 0;
        unsigned int rd_merges = 0;
        unsigned long long rd_sectors = 0;
        unsigned int rd_ticks = 0;
        unsigned int wr_ios = 0;
        unsigned int wr_merges = 0;
        unsigned long long wr_sectors = 0;
        unsigned int wr_ticks = 0;
        unsigned int nb_current = 0;
        unsigned int ticks = 0;
        unsigned int aveq = 0;
        memset(name, 0, sizeof(name));

        sscanf(buf, "%4d %4d %31s %u %u %llu %u %u %u %llu %u %u %u %u",
               &major,
               &minor,
               name,
               &rd_ios,
               &rd_merges,
               &rd_sectors,
               &rd_ticks,
               &wr_ios,
               &wr_merges,
               &wr_sectors,
               &wr_ticks,
               &nb_current,
               &ticks,
               &aveq);


        DiskStatistic r;
        r.name = name;
        r.rd_ios += rd_ios;
        r.rd_merges += rd_merges;
        r.rd_sectors += rd_sectors;
        r.rd_ticks += rd_ticks;
        r.wr_ios += wr_ios;
        r.wr_merges += wr_merges;
        r.wr_sectors += wr_sectors;
        r.wr_ticks += wr_ticks;
        r.nb_current += nb_current;
        r.ticks += ticks;
        r.aveq += aveq;
        r.ok = true;
        result.push_back(std::make_shared<DiskStatistic>(r));
    }
    fclose(f);
    return result;
}

// 统计内存使用情况
std::shared_ptr<MemoryStatistic> SystemInfo::getMemoryInfo()
{
    std::shared_ptr<MemoryStatistic> result;
#ifdef WIN32
    return result;
#endif
    FILE* f = fopen("/proc/meminfo", "r");
    if (f == NULL)
    {
        return result;
    }

    MemoryStatistic r;
    static char buf[1024];
    while (fgets(buf, sizeof(buf), f))
    {
        // @see: read_meminfo() from https://github.com/sysstat/sysstat/blob/master/rd_stats.c#L227
        if (strncmp(buf, "MemTotal:", 9) == 0)
        {
            sscanf(buf + 9, "%lu", &r.MemTotal);
        }
        else if (strncmp(buf, "MemFree:", 8) == 0)
        {
            sscanf(buf + 8, "%lu", &r.MemFree);
        }
        else if (strncmp(buf, "Buffers:", 8) == 0)
        {
            sscanf(buf + 8, "%lu", &r.Buffers);
        }
        else if (strncmp(buf, "Cached:", 7) == 0)
        {
            sscanf(buf + 7, "%lu", &r.Cached);
        }
        else if (strncmp(buf, "SwapTotal:", 10) == 0)
        {
            sscanf(buf + 10, "%lu", &r.SwapTotal);
        }
        else if (strncmp(buf, "SwapFree:", 9) == 0)
        {
            sscanf(buf + 9, "%lu", &r.SwapFree);
        }
    }
    fclose(f);

    r.MemActive = r.MemTotal - r.MemFree;
    r.RealInUse = r.MemActive - r.Buffers - r.Cached;
    r.NotInUse = r.MemTotal - r.RealInUse;

    if (r.MemTotal > 0)
    {
        r.percent_ram = (float)(r.RealInUse / (double)r.MemTotal);
    }
    if (r.SwapTotal > 0)
    {
        r.percent_swap = (float)((r.SwapTotal - r.SwapFree) / (double)r.SwapTotal);
    }

    r.ok = true;
    result = std::make_shared<MemoryStatistic>(r);
    return result;
}

// 平台信息
std::shared_ptr<PlatformInfo> SystemInfo::getPlatformInfo()
{
    std::shared_ptr<PlatformInfo> result;
#ifdef WIN32
    return result;
#endif
    PlatformInfo r;
    {
        FILE* f = fopen("/proc/uptime", "r");
        if (f == NULL)
        {
            return result;
        }

        fscanf(f, "%lf %lf\n", &r.os_uptime, &r.os_ilde_time);
        fclose(f);
    }
    {
        FILE* f = fopen("/proc/loadavg", "r");
        if (f == NULL)
        {
            return result;
        }

        // @see: read_loadavg() from https://github.com/sysstat/sysstat/blob/master/rd_stats.c#L402
        // @remark, we use our algorithm, not sysstat.
        fscanf(f, "%lf %lf %lf\n",
               &r.load_one_minutes,
               &r.load_five_minutes,
               &r.load_fifteen_minutes);

        fclose(f);
    }
    r.ok = true;
    result = std::make_shared<PlatformInfo>(r);
    return result;
}
