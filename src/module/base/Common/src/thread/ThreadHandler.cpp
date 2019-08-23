#include "Common.h"
#include "ThreadHandler.h"

ThreadHandler::ThreadHandler(std::string threadName)
{
    mThreadName = threadName;
}

// 开始
std::shared_ptr<Error> ThreadHandler::start()
{
    LOG_I(mClassName, "thread handler start");
    return Common::noError();
}

// 结束
void ThreadHandler::stop()
{
    mIsStop = true;
    LOG_I(mClassName, "thread handler stop");
}

bool ThreadHandler::isStop()
{
    return mIsStop;
}

// 获取线程名称
std::string ThreadHandler::getThreadName()
{
    return mThreadName;
}
