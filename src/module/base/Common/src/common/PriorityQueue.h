#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#include <list>
#include <memory>
#include <mutex>
#include <chrono>
#include <QtGlobal>
#include <condition_variable>
#include "Base.h"

// 优先级数
#define PRIORITY_COUNT 10

//优先级队列
template <typename T>
class CGF_EXPORT PriorityQueue
{
public:

    // 队列是否为空
    bool empty()
    {
        std::unique_lock<std::mutex> autoLock(mDataMutex);
        for (int i = 0; i < PRIORITY_COUNT; ++i)
        {
            if (!mListData[i].empty())
            {
                return false;
            }
        }
        return true;
    }

    // 获取队列中元素的个数
    int size()
    {
        std::unique_lock<std::mutex> autoLock(mDataMutex);
        int count = 0;
        for (int i = 0; i < PRIORITY_COUNT; ++i)
        {
            count += (int)mListData[i].size();
        }
        return count;
    }

    // 清空队列
    void clear()
    {
        std::unique_lock<std::mutex> autoLock(mDataMutex);
        for (int i = 0; i < PRIORITY_COUNT; ++i)
        {
            mListData[i].clear();
        }
        mIsClear = true;
        mDataCondition.notify_all();
    }

    // 获取优先级的个数
    int priorityCount()
    {
        return PRIORITY_COUNT;
    }

    // 一个元素入队列,入队列成功返回true,失败返回false
    bool push(std::shared_ptr<T> data, int priority = 5)
    {
        mIsClear = false;
        priority = qBound(0, priority, PRIORITY_COUNT);

        std::unique_lock<std::mutex> autoLock(mDataMutex);
        mListData[priority].push_back(data);
        mDataCondition.notify_one();
        return true;
    }

    // 从队列中取出一个元素
    std::shared_ptr<T> pop()
    {
        std::shared_ptr<T> result;
        while (true)
        {
            // 如果整个队列被清空，则返回（可能是要析构了，不返回会造成死等）
            if (mIsClear)
            {
                break;
            }

            {
                std::unique_lock<std::mutex> autoLock(mDataMutex);
                for (int i = 0; i < PRIORITY_COUNT; ++i)
                {
                    if (mListData[i].empty())
                    {
                        continue;
                    }

                    result = mListData[i].front();
                    mListData[i].pop_front();
                    break;
                }

                // 判断是否取出数据
                if (NULL == result.get())
                {
                    mDataCondition.wait(autoLock);
                }
                else
                {
                    break;
                }
            }
        }

        return result;
    }

    // 从队列中取出一个元素，如果超过一定时间，则返回空
    std::shared_ptr<T> popExpiration(int millsecond)
    {
        std::shared_ptr<T> result;
        while (true)
        {
            // 如果整个队列被清空，则返回（可能是要析构了，不返回会造成死等）
            if (mIsClear)
            {
                break;
            }

            {
                std::unique_lock<std::mutex> autoLock(mDataMutex);
                for (int i = 0; i < PRIORITY_COUNT; ++i)
                {
                    if (mListData[i].empty())
                    {
                        continue;
                    }

                    result = mListData[i].front();
                    mListData[i].pop_front();
                    break;
                }

                // 判断是否取出数据
                if (NULL == result.get())
                {
                    std::chrono::milliseconds sleepTime(millsecond);
                    mDataCondition.wait_for(autoLock, sleepTime);
                }
                break;
            }
        }

        return result;
    }

private:
    bool                mIsClear = false;
    //static const int    mPriorityCount = 10;
    std::list<std::shared_ptr<T>>   mListData[PRIORITY_COUNT];  // 存放数据的队列
    std::mutex          mDataMutex;     // 队列的锁
    std::condition_variable mDataCondition;

};

#endif
