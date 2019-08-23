#ifndef SPEED_STATISTIC_LIST_H
#define SPEED_STATISTIC_LIST_H
#include <vector>
#include <mutex>
#include <string>
#include <atomic>
#include <time.h>
#include "Common.h"
#include "jsoncpp/json.h"

// 速度统计列表（一般用来统计某个模块的处理速度）
class CGF_EXPORT SpeedStatisticList
{
public:
    SpeedStatisticList();

    // 添加处理的个数
    void add(int count = 1);

    // 获取统计的个数
    int getStatistic();
    std::vector<int> getStatistic(int count);

    std::string toString();
    Json::Value toJson();

private:
    std::string         mClassName = "SpeedStatisticList";
    const static int    mListSize = 61;     // 统计60s的速度

    int                 mSpeedList[mListSize];  // 速度列表
    time_t              mTimeList[mListSize];   // 事件列表
    std::mutex          mMutexStatistic;
};

#endif
