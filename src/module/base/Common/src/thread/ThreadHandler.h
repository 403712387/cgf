#ifndef THREAD_HANDLER_H
#define THREAD_HANDLER_H
#include <memory>
#include "Base.h"
#include "Error.h"

/*
 * 线程的worker
*/
class Thread;
class CGF_EXPORT ThreadHandler
{
    friend class Thread;
public:
    ThreadHandler(std::string threadName);

protected:
    // 开始
    virtual std::shared_ptr<Error> start();

    // 工作
    virtual std::shared_ptr<Error> work() = 0;

    // 结束
    virtual void stop();

    // 获取线程名称
    std::string getThreadName();

    bool isStop();

private:
    std::string mClassName = "ThreadHandler";
    std::string mThreadName;
    bool        mIsStop = false;
};

#endif
