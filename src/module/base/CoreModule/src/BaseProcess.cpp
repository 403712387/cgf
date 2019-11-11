#include "Log.h"
#include "Common.h"
#include "BaseProcess.h"
#include "MessageRoute.h"
BaseProcess::BaseProcess(MessageRoute *messageRoute, std::string name)
{
    mMessageRoute = messageRoute;
    mClassName = name;
    mMessageBufferThread = std::thread(std::bind(&BaseProcess::messageBufferLoop, this));
    if (mMessageRoute != NULL)
    {
        mMessageRoute->registerProcess(this);
    }
}

// 在这个线程中进行开启线程等操作
void BaseProcess::beginWork()
{

}

// 卸载模块
void BaseProcess::uninit()
{
    if (mExitModule == false)
    {
        mExitModule = true;
        mListMessageCondition.notify_all();
        if (mMessageBufferThread.joinable())
        {
            mMessageBufferThread.join();
        }
    }
}

// 获取模块名称
std::string  BaseProcess::getModuleName()
{
    return mClassName;
}

// 订阅消息
void BaseProcess::subscribeMessage(MessageType messageType)
{
    if (mMessageRoute != NULL)
    {
        mMessageRoute->subscribeMessage(this, messageType);
        LOG_V(mClassName, "subscribe message successful, module name:" << getModuleName()
            << ", message type:" << BaseMessage::getMessageTypeName(messageType));
    }
    else
    {
        LOG_E(mClassName, "subscribe message error, message route is NULL, module name:" << getModuleName()
            << ", message type:" << BaseMessage::getMessageTypeName(messageType));
    }
}

// 预先处理消息（消息在订阅模块处理之前先处理）
void BaseProcess::foreseeMessage(MessageType messageType)
{
    if (mMessageRoute != NULL)
    {
        mMessageRoute->foreseeMessage(this, messageType);
        LOG_V(mClassName, "forcesee message successful, module name:" << getModuleName()
            << ", message type:" << BaseMessage::getMessageTypeName(messageType));
    }
    else
    {
        LOG_E(mClassName, "forcesee message error, message route is NULL, module name:" << getModuleName()
            << ", message type:" << BaseMessage::getMessageTypeName(messageType));
    }
}

// 预先处理回应
void BaseProcess::foreseeResponse(MessageType messageType)
{
    if (mMessageRoute != NULL)
    {
        mMessageRoute->foreseeResponse(this, messageType);
        LOG_V(mClassName, "forcesee response successful, module name:" << getModuleName()
            << ", message type:" << BaseMessage::getMessageTypeName(messageType));
    }
    else
    {
        LOG_E(mClassName, "forcesee response error, message route is NULL, module name:" << getModuleName()
            << ", message type:" << BaseMessage::getMessageTypeName(messageType));
    }
}

// 发送消息
std::shared_ptr<BaseResponse> BaseProcess::sendMessage(std::shared_ptr<BaseMessage> message)
{
    std::shared_ptr<BaseResponse> result;

    if (message.get() == NULL)
    {
        LOG_E(mClassName, "send message fail, message is NULL, module name:" << getModuleName());
        return result;
    }

    if (mMessageRoute == NULL)
    {
        LOG_E(mClassName, "send message fail, message route is NULL, module name:" << getModuleName()
              << ", message :" << message->toString());
        return result;
    }
    result = mMessageRoute->sendMessage(message);
    return result;
}

// 发送消息的回应(大多数情况下，不用调用这个函数)
void BaseProcess::sendResponse(std::shared_ptr<BaseResponse> response)
{
    if (NULL == response)
    {
        LOG_E(mClassName, "send message response fail, message response is NULL");
        return;
    }

    mMessageRoute->sendResponse(response);
}

void BaseProcess::putMessageToBuffer(std::shared_ptr<BaseMessage> &message)
{
    if (message.get() == NULL)
    {
        LOG_E(mClassName, "put message to buffer fail, message is NULL");
        return;
    }

    int messagePriority = (int)message->getMessagePriority();
    if (messagePriority < 0 || messagePriority >= mMessagePriorityCount)
    {
        LOG_E(mClassName, "put message to buffer fail, invite message priority:" << messagePriority
            << ", message :" << message->toString());
        return;
    }

    std::unique_lock<std::mutex> autolock(mListMessageLocker);
    mListMessage[messagePriority].push_back(message);
    mListMessageCondition.notify_one();
}

// 将消息回应放入队列
void BaseProcess::putResponseToBuffer(std::shared_ptr<BaseResponse> &response)
{
    if (response.get() == NULL || response->getMessage().get() == NULL)
    {
        LOG_E(mClassName, "put message response to buffer fail, message is NULL");
        return;
    }

    int messagePriority = (int)response->getMessage()->getMessagePriority();
    if (messagePriority < 0 || messagePriority >= mMessagePriorityCount)
    {
        LOG_E(mClassName, "put message response to buffer fail, invite message priority:" << messagePriority
            << ", message :" << response->getMessage()->toString());
        return;
    }

    std::unique_lock<std::mutex> autolock(mListMessageLocker);
    mListMessage[messagePriority].push_back(response);
    mListMessageCondition.notify_one();
}

// 从队列中获取消息
std::shared_ptr<BaseClass> BaseProcess::getMessageFromBuffer()
{
    std::shared_ptr<BaseClass> result;
    std::unique_lock<std::mutex> autolock(mListMessageLocker);
    if (messageBufferIsEmpty())  // 队列为空，等待50ms
    {
        mListMessageCondition.wait_for(autolock, std::chrono::milliseconds(mSleepTime));
    }
   
    if (!messageBufferIsEmpty())
    {
        for (int i = 0; i < mMessagePriorityCount; ++i)
        {
            if (mListMessage[i].empty())
            {
                continue;
            }
            result = mListMessage[i].front();
            mListMessage[i].erase(mListMessage[i].begin());
            break;
        }
    }
   
    return result;
}

// 缓存异步消息的线程
void BaseProcess::messageBufferLoop()
{
    LOG_I(mClassName, "begin process async message");
    Common::setThreadName(mClassName);
    while (mExitModule == false)
    {
        std::shared_ptr<BaseClass> baseClass = getMessageFromBuffer();
        if (baseClass.get() == NULL)
        {
            onIdle();
            continue;
        }

        // 如果是消息
        if (Type_Message == baseClass->getClassType())
        {
            std::shared_ptr<BaseMessage> message = std::dynamic_pointer_cast<BaseMessage>(baseClass);
            if (NULL == message.get())
            {
                LOG_E(mClassName, "process message fail, message is NULL, message info:" << baseClass->toString());
                continue;
            }
            processMessage(message);
        }
        else  //如果是消息的回应
        {
            std::shared_ptr<BaseResponse> response = std::dynamic_pointer_cast<BaseResponse>(baseClass);
            if (NULL == response.get())
            {
                LOG_E(mClassName, "process message response fail, response is NULL, response info:" << baseClass->toString());
                continue;
            }

            onProcessResponse(response);
        }
    }
    LOG_I(mClassName, "end process async message");
}

// 消息队列是否为空
bool BaseProcess::messageBufferIsEmpty()
{
    for (int i = 0; i < mMessagePriorityCount; ++i)
    {
        if (mListMessage[i].empty() == false)
        {
            return false;
        }
    }
    return true;
}

// 处理消息
void BaseProcess::processMessage(std::shared_ptr<BaseMessage> &message)
{
    std::shared_ptr<BaseResponse> response = onProcessMessage(message);

    // 消息的回应
    if (NULL != response.get() &&  NULL != mMessageRoute)
    {
        // 异步消息的回应
        if (message->getTransType() == Async_Trans_Message)
        {
            mMessageRoute->sendResponse(response);
        }
    }
}

// 预先处理消息（如果返回true,则表明此消息已经处理完毕，订阅模块就接收不到消息）
bool BaseProcess::onForeseeMessage(std::shared_ptr<BaseMessage> &message)
{
    return false;
}

// 预先处理回应
bool BaseProcess::onForeseeResponse(std::shared_ptr<BaseResponse> &response)
{
    return false;
}

// 消息队列中没有消息的时候调用
void BaseProcess::onIdle()
{

}
