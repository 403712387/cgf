#include <sstream>
#include <algorithm>
#include "SpeedStatisticList.h"

SpeedStatisticList::SpeedStatisticList()
{
    memset(&mSpeedList, 0, sizeof(mSpeedList));
    memset(&mTimeList, 0, sizeof(mTimeList));
}

// 添加处理的个数
void SpeedStatisticList::add(int count)
{
    if (count <= 0)
    {
        return;
    }

    // 获取当前时间
    time_t current = time(NULL);
    int index = current % mListSize;

    {
        std::unique_lock<std::mutex> autoLock(mMutexStatistic);
        if (mTimeList[index] == current)    // 统计同一秒的速度
        {
            mSpeedList[index] += count;
        }
        else        // 新的一秒
        {
            mTimeList[index] = current;
            mSpeedList[index] = count;
        }
    }
}

// 获取统计的个数
int SpeedStatisticList::getStatistic()
{
    std::vector<int> result = getStatistic(1);
    return result.empty() ? 0 : result.at(0);
}

std::vector<int> SpeedStatisticList::getStatistic(int count)
{
    std::vector<int> result;
    count = std::min<int>(count, mListSize - 1);

    // 获取当前时间
    time_t current = time(NULL);

    {
        int beginIndex = current - count;
        int endIndex = current;
        for(int i = beginIndex; i < endIndex; ++i)
        {
            int index = i % mListSize;
            if (mTimeList[index] == i)
            {
                result.push_back(mSpeedList[index]);
            }
            else
            {
                result.push_back(0);
            }
        }
    }

    return result;
}

std::string SpeedStatisticList::toString()
{
    std::stringstream buf;
    std::vector<int> speeds = getStatistic(mListSize - 1);
    for (int speed : speeds)
    {
        buf << speed << " ";
    }
    return buf.str();
}

Json::Value SpeedStatisticList::toJson()
{
    Json::Value result;
    int speed = getStatistic();
    result["speed"] = speed;
    return result;
}
