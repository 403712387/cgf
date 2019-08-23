#include "Thread.h"
#include "Common.h"
#include "ThreadHandler.h"

Thread::Thread(ThreadHandler *handler)
{
    mThreadHandler = handler;
}

// 启动线程
std::shared_ptr<Error> Thread::start()
{
    if (mThread.joinable())
    {
        LOG_W(mClassName, "thread is running");
        return Common::noError();
    }

    mThread = std::thread(&Thread::work, this);
    return Common::noError();
}

// 停止线程
void Thread::stop()
{
    if (!mThread.joinable())
    {
        LOG_I(mClassName, "thread is alread stop");
        return;
    }
    mThreadHandler->stop();
    mThread.join();
}

// 线程启动后运行的函数
void Thread::work()
{
    // 设置线程名称
    Common::setThreadName(mThreadHandler->getThreadName());

    mThreadHandler->start();
    mThreadHandler->work();
}
