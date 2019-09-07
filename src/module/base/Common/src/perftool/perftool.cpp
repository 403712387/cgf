#include <QtCore/QtGlobal>
#include <QtCore/QByteArray>
#include "Log.h"
#include "perftool.h"
#include "Common.h"
#include "gperftools/profiler.h"
#include "gperftools/heap-profiler.h"

std::string PerfTool::mClassName = "PerfTool";
bool PerfTool::mCpuProfiler = false;
bool PerfTool::mHeapProfiler = false;
std::string PerfTool::mCpuProfilerFile = "./html/perf/cpu.prof";
std::string PerfTool::mHeapProfilerFile = "./html/perf/heap.prof";

// 开始性能检测
std::string PerfTool::startCPUProfiler()
{
    // 判断是不是已经开启
    if (true == mCpuProfiler)
    {
        return mCpuProfilerFile;
    }

    mCpuProfiler = true;
    LOG_I(mClassName, "start cpu prifiler, file:" << mCpuProfilerFile);
    ProfilerStart(mCpuProfilerFile.c_str());
    return mCpuProfilerFile;
}

// 停止性能检测
std::string PerfTool::stopCPUProfiler()
{
    if (false == mCpuProfiler)
    {
        return "";
    }
    mCpuProfiler = false;
    LOG_I(mClassName, "stop cpu prifiler");
    ProfilerStop();
    return mCpuProfilerFile;
}

// 开始分析堆栈
std::string PerfTool::startHeapProfiler()
{
    // 判断是不是已经开启
    if (true == mHeapProfiler)
    {
        return mHeapProfilerFile;
    }

    mHeapProfiler = true;
    LOG_I(mClassName, "start heap prifiler, file:" << mHeapProfilerFile);
    HeapProfilerStart(mHeapProfilerFile.c_str());
    return mHeapProfilerFile;
}

// 停止堆栈分析
std::string PerfTool::stopHeapProfiler()
{
    if (false == mHeapProfiler)
    {
        return "";
    }
    mHeapProfiler = false;
    LOG_I(mClassName, "stop heap prifiler");
    HeapProfilerStop();
    return mHeapProfilerFile;
}
