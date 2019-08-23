#ifndef BASE_WAIT_MESSAGE_H
#define BASE_WAIT_MESSAGE_H
#include <mutex>
#include <condition_variable>
#include "BaseMessage.h"

// 可以等待的消息，主要用在算法模块（把数据丢进去之后等待回应）
class CGF_EXPORT BaseWaitMessage : public BaseMessage
{
public:
    BaseWaitMessage(MessageType type, MessageTransType transType = Async_Trans_Message,
        PriorityType priority = Priority_Third, MessageDistriubite distribute = Message_Unicast);

    // 获取消息的信息
    virtual std::string toString();

    // 等待回应(无限等待)
    void wait();

    // 等待回应（有期限的等待）
    bool waitExpiration(int millsecond);

    // 是否在等待
    bool isWait();

    // 响应通知
    void notify(bool successful);

    // 是否成功处理
    bool isSuccessfulProcess();

protected:
    std::string         mClassName = "BaseWaitMessage";
    bool                mIsWait = false;
    std::mutex          mMutex;
    std::condition_variable mCondition;

    bool                mSuccessfulProcess = false;
};

#endif
