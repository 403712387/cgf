#ifndef BASE_PROCESS_H
#define BASE_PROCESS_H
#include <string>
#include <memory>
#include <mutex>
#include <list>
#include <thread>
#include <condition_variable>
#include "Log.h"
#include "BaseMessage.h"

/*
 * 消息处理的基类，其他的消息处理类都要从此类继承
 * 注意，此类现阶段只提供了订阅消息的接口，没有取消订阅的接口
*/
class MessageRoute;
class CGF_EXPORT BaseProcess : public std::enable_shared_from_this<BaseProcess>
{
    friend class MessageRoute;
public:
    BaseProcess(MessageRoute *messageRoute, std::string name);

protected:
    // 在这个函数中进行消息订阅
    virtual bool init() = 0;

    // 在这个线程中进行开启线程等操作
    virtual void beginWork();

    // 卸载模块
    virtual void uninit();

    // 发送消息
    std::shared_ptr<BaseResponse> sendMessage(std::shared_ptr<BaseMessage> message);

    // 发送消息的回应(大多数情况下，不用调用这个函数)
    void sendResponse(std::shared_ptr<BaseResponse> response);

    // 处理消息的函数
    virtual std::shared_ptr<BaseResponse> onProcessMessage(std::shared_ptr<BaseMessage> &message) = 0;

    // 处理消息的回应
    virtual void onProcessResponse(std::shared_ptr<BaseResponse> &response) = 0;

    // 预先处理消息（如果返回true,则表明此消息已经处理完毕，订阅模块就接收不到消息）
    virtual bool onForeseeMessage(std::shared_ptr<BaseMessage> &message);

    // 预先处理回应
    virtual bool onForeseeResponse(std::shared_ptr<BaseResponse> &response);

    // 订阅消息
    void subscribeMessage(MessageType messageType);

    // 预先处理消息（消息在订阅模块处理之前先处理，此函数是个hook函数,慎用！！！）
    void foreseeMessage(MessageType messageType);

    // 预先处理回应
    void foreseeResponse(MessageType messageType);

    // 获取模块名称
    std::string getModuleName();

    // 从队列中获取消息
    std::shared_ptr<BaseClass> getMessageFromBuffer();

private:
    // 缓存异步消息的线程
    void messageBufferLoop();

    // 消息队列是否为空
    bool messageBufferIsEmpty();

    // 将消息回应放入队列
    void putResponseToBuffer(std::shared_ptr<BaseResponse> &Response);

    // 异步消息放入缓存队列
    void putMessageToBuffer(std::shared_ptr<BaseMessage> &message);

    // 处理消息
    void processMessage(std::shared_ptr<BaseMessage> &message);

protected:
    bool           mExitModule = false;
    std::string     mClassName;

private:   
    MessageRoute    *mMessageRoute = NULL;    

    // 消息相关
    static const int       mMessagePriorityCount = 10; // 这里最多支持10个消息级别
    std::thread                               mMessageBufferThread;
    std::list<std::shared_ptr<BaseClass>>     mListMessage[mMessagePriorityCount];
    std::mutex                                mListMessageLocker;
    std::condition_variable                   mListMessageCondition;

    const int               mSleepTime = 50; // 当没有消息的时候，休眠的时间
};

#endif
