#include <QtGlobal>
#include <QByteArray>
#include "Log.h"
#include "perftool.h"
#include "Common.h"
#include "gperftools/profiler.h"
#include "gperftools/heap-profiler.h"

std::string PerfTool::mClassName = "PerfTool";
bool PerfTool::mCpuProfiler = false;
bool PerfTool::mHeapProfiler = false;
std::string PerfTool::mCpuProfilerFile = "./perf/cpu.prof";
std::string PerfTool::mHeapProfilerFile = "./perf/heap.prof";

// 开始性能检测
void PerfTool::startCPUProfiler()
{
    // 判断是不是已经开启
    if (true == mCpuProfiler)
    {
        return;
    }

    mCpuProfiler = true;
    LOG_I(mClassName, "start cpu prifiler, file:" << mCpuProfilerFile);
#if 0
    ProfilerStart(mCpuProfilerFile.c_str());
#endif
}

// 停止性能检测
void PerfTool::stopCPUProfiler()
{
    if (false == mCpuProfiler)
    {
        return;
    }
    mCpuProfiler = false;
    LOG_I(mClassName, "stop cpu prifiler");
#if 0
    ProfilerStop();
#endif
}

// 开始分析堆栈
void PerfTool::startHeapProfiler()
{
    // 判断是不是已经开启
    if (true == mHeapProfiler)
    {
        return;
    }

    mHeapProfiler = true;
    LOG_I(mClassName, "start heap prifiler, file:" << mHeapProfilerFile);
#if 0
    HeapProfilerStart(mHeapProfilerFile.c_str());
#endif
}

// 停止堆栈分析
void PerfTool::stopHeapProfiler()
{
    if (false == mHeapProfiler)
    {
        return;
    }
    mHeapProfiler = false;
    LOG_I(mClassName, "stop heap prifiler");
#if 0
    HeapProfilerStop();
#endif
}
