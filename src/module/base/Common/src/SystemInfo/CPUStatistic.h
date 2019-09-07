#ifndef CPU_STATSTIC_H
#define CPU_STATSTIC_H
#include "Base.h"
#include "jsoncpp/json.h"

class SystemInfo;
class CGF_EXPORT CPUStatistic
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
    // The percent is in [0, 1], where 1 is 100%.
    // for multiple core cpu, max also is 100%.
    float percent = 0;
    // The total cpu time units
    // @remark, zero for the previous total() is zero.
    //          the usaged_cpu_delta = total_delta * percent
    //          previous cpu total = this->total() - total_delta
    long long total_delta = 0;

    // data of /proc/stat
public:
    // The amount of time, measured in units  of  USER_HZ
    // (1/100ths  of  a  second  on  most  architectures,  use
    // sysconf(_SC_CLK_TCK)  to  obtain  the  right value)
    //
    // The system spent in user mode,
    unsigned long long user = 0;
    // user mode with low priority (nice),
    unsigned long long nice = 0;
    // system mode,
    unsigned long long sys = 0;
    // and the idle task, respectively.
    unsigned long long idle = 0;

    // In  Linux 2.6 this line includes three additional columns:
    //
    // iowait - time waiting for I/O to complete (since 2.5.41);
    unsigned long long iowait = 0;
    // irq - time servicing interrupts (since 2.6.0-test4);
    unsigned long long irq = 0;
    // softirq  -  time  servicing  softirqs  (since 2.6.0-test4).
    unsigned long long softirq = 0;

    // Since  Linux 2.6.11, there is an eighth column,
    // steal - stolen time, which is the time spent in other oper-
    // ating systems when running in a virtualized environment
    unsigned long long steal = 0;

    // Since Linux 2.6.24, there is a ninth column,
    // guest, which is the time spent running a virtual CPU for guest
    // operating systems under the control of the Linux kernel.
    unsigned long long guest = 0;
};

#endif
