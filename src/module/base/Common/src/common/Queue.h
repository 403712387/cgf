#ifndef QUEUE_H
#define QUEUE_H
#include <list>
#include <memory>
#include <mutex>
#include <chrono>
#include <QtGlobal>
#include <condition_variable>
#include "Base.h"

//普通队列
template <typename T>
class CGF_EXPORT Queue
{
public:

    // 队列是否为空
    bool empty()
    {
        std::unique_lock<std::mutex> autoLock(mDataMutex);
        if (!mListData.empty())
        {
            return false;
        }

        return true;
    }

    // 获取队列中元素的个数
    int size()
    {
        std::unique_lock<std::mutex> autoLock(mDataMutex);
        int count = (int)mListData.size();

        return count;
    }

    // 清空队列
    void clear()
    {
        std::unique_lock<std::mutex> autoLock(mDataMutex);
        mListData.clear();

        mIsClear = true;
        mDataCondition.notify_all();
    }

    // 一个元素入队列,入队列成功返回true,失败返回false
    bool push(std::shared_ptr<T> data)
    {
        mIsClear = false;

        std::unique_lock<std::mutex> autoLock(mDataMutex);
        mListData.push_back(data);
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

            std::unique_lock<std::mutex> autoLock(mDataMutex);
            {
                if (mListData.empty())
                {
                    mDataCondition.wait(autoLock);
                }

                if (!mListData.empty())
                {
                    result = mListData.front();
                    mListData.pop_front();
                }
                break;
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

                if (mListData.empty())
                {
                    std::chrono::milliseconds sleepTime(millsecond);
                    std::cv_status status = mDataCondition.wait_for(autoLock, sleepTime);
                }

                if (!mListData.empty())
                {
                    result = mListData.front();
                    mListData.pop_front();
                }
                break;
            }
        }

        return result;
    }

private:
    bool                mIsClear = false;
    std::list<std::shared_ptr<T>>   mListData;  // 存放数据的队列
    std::mutex          mDataMutex;     // 队列的锁
    std::condition_variable mDataCondition;

};

#endif
