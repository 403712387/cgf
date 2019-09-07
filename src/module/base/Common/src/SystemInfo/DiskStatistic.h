#ifndef DISK_STATISTIC_H
#define DISK_STATISTIC_H
#include "Base.h"
#include "jsoncpp/json.h"
/*
 * 统计磁盘的使用情况
*/
class SystemInfo;
class CGF_EXPORT DiskStatistic
{
    friend class SystemInfo;
public:
    Json::Value toJson();
private:
    // Whether the data is ok.
    bool ok = false;
    // The time in ms when sample.
    long long sample_time = 0;

    // input(read) KBytes per seconds
    int in_KBps = 0;
    // output(write) KBytes per seconds
    int out_KBps = 0;

    // @see: print_partition_stats() of iostat.c
    // but its value is [0, +], for instance, 0.1532 means 15.32%.
    float busy = 0;

private:
    std::string name;
    // @see: cat /proc/vmstat
    // The in(read) page count, pgpgin*1024 is the read bytes.
    // Total number of kilobytes the system paged in from disk per second.
    unsigned long pgpgin = 0;
    // The out(write) page count, pgpgout*1024 is the write bytes.
    // Total number of kilobytes the system paged out to disk per second.
    unsigned long pgpgout = 0;

    // @see: https://www.kernel.org/doc/Documentation/iostats.txt
    // @see: http://tester-higkoo.googlecode.com/svn-history/r14/trunk/Tools/iostat/iostat.c
    // @see: cat /proc/diskstats
    //
    // Number of issued reads.
    // This is the total number of reads completed successfully.
    // Read I/O operations
    unsigned int rd_ios = 0;
    // Number of reads merged
    // Reads merged
    unsigned int rd_merges = 0;
    // Number of sectors read.
    // This is the total number of sectors read successfully.
    // Sectors read
    unsigned long long rd_sectors = 0;
    // Number of milliseconds spent reading.
    // This is the total number of milliseconds spent by all reads
    // (as measured from make_request() to end_that_request_last()).
    // Time in queue + service for read
    unsigned int rd_ticks = 0;
    //
    // Number of writes completed.
    // This is the total number of writes completed successfully
    // Write I/O operations
    unsigned int wr_ios = 0;
    // Number of writes merged Reads and writes which are adjacent
    // To each other may be merged for efficiency. Thus two 4K
    // reads may become one 8K read before it is ultimately
    // handed to the disk, and so it will be counted (and queued)
    // as only one I/O. This field lets you know how often this was done.
    // Writes merged
    unsigned int wr_merges = 0;
    // Number of sectors written.
    // This is the total number of sectors written successfully.
    // Sectors written
    unsigned long long wr_sectors = 0;
    // Number of milliseconds spent writing .
    // This is the total number of milliseconds spent by all writes
    // (as measured from make_request() to end_that_request_last()).
    // Time in queue + service for write
    unsigned int wr_ticks = 0;
    //
    // Number of I/Os currently in progress.
    // The only field that should go to zero.
    // Incremented as requests are given to appropriate request_queue_t
    // and decremented as they finish.
    unsigned int nb_current = 0;
    // Number of milliseconds spent doing I/Os.
    // This field is increased so long as field 9 is nonzero.
    // Time of requests in queue
    unsigned int ticks = 0;
    // Number of milliseconds spent doing I/Os.
    // This field is incremented at each I/O start, I/O completion,
    // I/O merge, or read of these stats by the number of I/Os in
    // progress (field 9) times the number of milliseconds spent
    // doing I/O since the last update of this field. This can
    // provide an easy measure of both I/O completion time and
    // The backlog that may be accumulating.
    // Average queue length
    unsigned int aveq = 0;
};

#endif
