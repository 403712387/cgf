#include "PriorityQueue.h"
/*
// 队列是否为空
template <typename T> bool PriorityQueue<T>::empty()
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
template <typename T> int PriorityQueue<T>::size()
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
template <typename T> void PriorityQueue<T>::clear()
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
template <typename T> int PriorityQueue<T>::priorityCount()
{
    return PRIORITY_COUNT;
}

// 一个元素入队列,入队列成功返回true,失败返回false
template <typename T> bool PriorityQueue<T>::push(std::shared_ptr<T> data, int priority)
{
    mIsClear = false;
    priority = qBound(0, priority, PRIORITY_COUNT);

    std::unique_lock<std::mutex> autoLock(mDataMutex);
    mListData[priority - 1].push_back(data);
    mDataCondition.notify_one();
    return true;
}

// 从队列中取出一个元素
template <typename T> std::shared_ptr<T> PriorityQueue<T>::pop()
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
template <typename T> std::shared_ptr<T> PriorityQueue<T>::popExpiration(int millsecond)
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
            else
            {
                break;
            }
        }
    }

    return result;
}
*/
