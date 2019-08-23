#ifndef THREAD_H
#define THREAD_H
#include <thread>
#include <memory>
#include "Base.h"
#include "Error.h"

/*
 * 封装的公共线程类
*/
class ThreadHandler;
class CGF_EXPORT Thread
{
public:
    Thread(ThreadHandler *handler);

    // 启动线程
    std::shared_ptr<Error> start();

    // 停止线程
    void stop();

private:

    // 线程启动后运行的函数
    void work();
private:
    std::string         mClassName = "Thread";
    ThreadHandler       *mThreadHandler = NULL;
    std::thread         mThread;
};

#endif
