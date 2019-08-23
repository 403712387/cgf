#include <sstream>
#include <algorithm>
#include "Log.h"
#include "BaseWaitMessage.h"

BaseWaitMessage::BaseWaitMessage(MessageType type, MessageTransType transType,
    PriorityType priority , MessageDistriubite distribute)
    :BaseMessage(type, transType, priority, distribute)
{

}

// 获取消息的信息
std::string BaseWaitMessage::toString()
{
    std::stringstream result;
    result << BaseMessage::toString();
    if (isWait())
    {
       result << ", wait message response";
    }

    return result.str();
}

// 等待回应(无限等待)
void BaseWaitMessage::wait()
{
    if (true == mIsWait)
    {
        LOG_W(mClassName, "message is already wait, type:" << getMessageType() << ", sequence:" << getMessageSequence());
    }
    mIsWait = true;

    LOG_V(mClassName, "begin wait message, message info:" << toString());

    std::unique_lock<std::mutex> autoLock(mMutex);
    mCondition.wait(autoLock);

    LOG_V(mClassName, "end wait message, message info:" << toString());
    return;
}

// 等待回应（有期限的等待）
bool BaseWaitMessage::waitExpiration(int millsecond)
{
    if (true == mIsWait)
    {
        LOG_W(mClassName, "message is already wait, type:" << getMessageType() << ", sequence:" << getMessageSequence());
    }
    mIsWait = true;

    LOG_I(mClassName, "begin wait expiration message, type:" << getMessageType() << ", sequence:" << getMessageSequence());

    std::unique_lock<std::mutex> autoLock(mMutex);
    millsecond= std::max(0, millsecond);
    std::chrono::milliseconds waitTime(millsecond);
    std::cv_status status = mCondition.wait_for(autoLock, waitTime);

    LOG_I(mClassName, "end wait expiration message, type:" << getMessageType() << ", sequence:" << getMessageSequence());
    return (status == std::cv_status::timeout);
}

// 是否在等待
bool BaseWaitMessage::isWait()
{
    return mIsWait;
}

// 响应通知
void BaseWaitMessage::notify(bool successful)
{
    mSuccessfulProcess = successful;
    LOG_V(mClassName, "notify for wait message, message info:" << toString());
    mCondition.notify_all();
}

// 是否成功处理
bool BaseWaitMessage::isSuccessfulProcess()
{
    return mSuccessfulProcess;
}
