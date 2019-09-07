#include "ProcessStatistic.h"
Json::Value ProcessStatistic::toJson()
{
    Json::Value result;
    if (!ok)
    {
        return result;
    }

    result["pid"] = Json::Int64(pid);
    result["comm"] = std::string(comm);
    result["state"] = state;
    result["ppid"] = Json::Int64(ppid);
    result["pgrp"] = Json::Int64(pgrp);
    result["session"] = Json::Int64(session);
    result["tty_nr"] = Json::Int64(tty_nr);
    result["tpgid"] = Json::Int64(tpgid);
    result["flags"] = Json::Int64(flags);
    result["minflt"] = Json::Int64(minflt);
    result["cminflt"] = Json::Int64(cminflt);
    result["majflt"] = Json::Int64(majflt);
    result["cmajflt"] = Json::Int64(cmajflt);
    result["utime"] = Json::Int64(utime);
    result["stime"] = Json::Int64(stime);
    result["cutime"] = Json::Int64(cutime);
    result["cstime"] = Json::Int64(cstime);
    result["priority"] = Json::Int64(priority);
    result["nice"] = Json::Int64(nice);
    result["num_threads"] = Json::Int64(num_threads);
    result["itrealvalue"] = Json::Int64(itrealvalue);
    result["starttime"] = Json::Int64(starttime);
    result["vsize"] = Json::Int64(vsize);
    result["rss"] = Json::Int64(rss);
    result["rsslim"] = Json::Int64(rsslim);
    result["startcode"] = Json::Int64(startcode);
    result["endcode"] = Json::Int64(endcode);
    result["startstack"] = Json::Int64(startstack);
    result["kstkesp"] = Json::Int64(kstkesp);
    result["kstkeip"] = Json::Int64(kstkeip);
    result["signal"] = Json::Int64(signal);
    result["blocked"] = Json::Int64(blocked);
    result["sigignore"] = Json::Int64(sigignore);
    result["sigcatch"] = Json::Int64(sigcatch);
    result["wchan"] = Json::Int64(wchan);
    result["nswap"] = Json::Int64(nswap);
    result["cnswap"] = Json::Int64(cnswap);
    result["processor"] = Json::Int64(processor);
    result["rt_priority"] = Json::Int64(rt_priority);
    result["policy"] = Json::Int64(policy);
    result["delayacct_blkio_ticks"] = Json::Int64(delayacct_blkio_ticks);
    result["guest_time"] = Json::Int64(guest_time);
    result["cguest_time"] = Json::Int64(cguest_time);
    return result;
}
