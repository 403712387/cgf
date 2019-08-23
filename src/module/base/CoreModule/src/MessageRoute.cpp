#include <chrono>
#include <thread>
#include <time.h>
#include <malloc.h>
#include "MessageRoute.h"
#include "BaseProcess.h"
#include "ServiceControlMessage.h"

// 模块注册
void MessageRoute::registerProcess(BaseProcess *process)
{
    std::unique_lock<std::mutex> autolock(mAllProcessLocker);
    for (auto &info : mAllProcess)
    {
        if (process == info)
        {
            return;
        }
    }
    mAllProcess.push_back(process);
}

// 模块订阅消息
void MessageRoute::subscribeMessage(BaseProcess *process, MessageType messageType)
{
    std::unique_lock<std::mutex> autolock(mSubscribeMessageProcessLocker);
    auto iterAllProcess = mSubscribeMessageProcess.find(messageType);
    if (iterAllProcess == mSubscribeMessageProcess.end())
    {
        std::vector<BaseProcess*> vectProcess = { process };
        mSubscribeMessageProcess[messageType] = vectProcess;
    }
    else
    {
        for (auto &info : iterAllProcess->second)
        {
            if (info == process)
            {
                LOG_W(mClassName, process->getModuleName() << " already subscribe message " << messageType);
                return;
            }
        }
        iterAllProcess->second.push_back(process);
    }
}

// 预先处理消息（消息在订阅模块处理之前先处理，此函数是个hook函数,慎用！！！）
void MessageRoute::foreseeMessage(BaseProcess *process, MessageType messageType)
{
    std::unique_lock<std::mutex> autolock(mForeseeMessageProcessLocker);
    auto iterAllProcess = mForeseeMessageProcess.find(messageType);
    if (iterAllProcess == mForeseeMessageProcess.end())
    {
        std::vector<BaseProcess*> vectProcess = { process };
        mForeseeMessageProcess[messageType] = vectProcess;
    }
    else
    {
        for (auto &info : iterAllProcess->second)
        {
            if (info == process)
            {
                LOG_W(mClassName, process->getModuleName() << " already foresee message " << messageType);
                return;
            }
        }
        iterAllProcess->second.push_back(process);
    }
}

// 预先处理回应
void MessageRoute::foreseeResponse(BaseProcess *process, MessageType messageType)
{
    std::unique_lock<std::mutex> autolock(mForeseeResponseProcessLocker);
    auto iterAllProcess = mForeseeResponseProcess.find(messageType);
    if (iterAllProcess == mForeseeResponseProcess.end())
    {
        std::vector<BaseProcess*> vectProcess = { process };
        mForeseeResponseProcess[messageType] = vectProcess;
    }
    else
    {
        for (auto &info : iterAllProcess->second)
        {
            if (info == process)
            {
                LOG_W(mClassName, process->getModuleName() << " already foresee response " << messageType);
                return;
            }
        }
        iterAllProcess->second.push_back(process);
    }
}

// 模块取消消息订阅
void MessageRoute::unsubscribeMessage(BaseProcess *process, MessageType messageType)
{
    std::unique_lock<std::mutex> autolock(mSubscribeMessageProcessLocker);
    auto iterAllProcess = mSubscribeMessageProcess.find(messageType);
    if (iterAllProcess == mSubscribeMessageProcess.end())
    {
        return;
    }
   
    for (auto iterProcess = iterAllProcess->second.begin(); iterProcess != iterAllProcess->second.end(); ++iterProcess)
    {
        if (*iterProcess == process)
        {
            iterAllProcess->second.erase(iterProcess);
            return;
        }
    }

    LOG_E(mClassName, "unscribe message fail, not find subscribe info,module name:"
          << process->getModuleName() << ", message type:" << BaseMessage::getMessageTypeName(messageType));
}

// 取消预先处理消息
void MessageRoute::unforeseeMessage(BaseProcess *process, MessageType messageType)
{
    std::unique_lock<std::mutex> autolock(mForeseeMessageProcessLocker);
    auto iterAllProcess = mForeseeMessageProcess.find(messageType);
    if (iterAllProcess == mForeseeMessageProcess.end())
    {
        return;
    }

    for (auto iterProcess = iterAllProcess->second.begin(); iterProcess != iterAllProcess->second.end(); ++iterProcess)
    {
        if (*iterProcess == process)
        {
            iterAllProcess->second.erase(iterProcess);
            return;
        }
    }

    LOG_E(mClassName, "unforesee message fail, not find subscribe info,module name:"
          << process->getModuleName() << ", message type:" << BaseMessage::getMessageTypeName(messageType));
}

// 取消预先处理回应
void MessageRoute::unForeseeResponse(BaseProcess *process, MessageType messageType)
{
    std::unique_lock<std::mutex> autolock(mForeseeResponseProcessLocker);
    auto iterAllProcess = mForeseeResponseProcess.find(messageType);
    if (iterAllProcess == mForeseeResponseProcess.end())
    {
        return;
    }

    for (auto iterProcess = iterAllProcess->second.begin(); iterProcess != iterAllProcess->second.end(); ++iterProcess)
    {
        if (*iterProcess == process)
        {
            iterAllProcess->second.erase(iterProcess);
            return;
        }
    }

    LOG_E(mClassName, "unForesee response fail, not find subscribe info,module name:"
          << process->getModuleName() << ", message type:" << BaseMessage::getMessageTypeName(messageType));

}

// 发送消息
std::shared_ptr<BaseResponse> MessageRoute::sendMessage(std::shared_ptr<BaseMessage> &message)
{
    std::shared_ptr<BaseResponse> result;

    // 如果程序即将退出，则不处理任何消息
    if (mExit)
    {
        return result;
    }

    bool ret = processForeseeMessage(message);
    if (ret)
    {
        LOG_D(mClassName, "foresee message done, not send to subscribe module, message info:" << message->toString());
        return result;
    }

    result = processSubscribeMessage(message);
    return result;
}

// 处理消息
std::shared_ptr<BaseResponse> MessageRoute::processSubscribeMessage(std::shared_ptr<BaseMessage> &message)
{
    std::shared_ptr<BaseResponse> result;
    //std::unique_lock<std::mutex> autolock(mSubscribeMessageProcessLocker);
    if (message->getTransType() == Sync_Trans_Message)  // 同步消息
    {
        if (message->getMessageDistribute() == Message_Unicast)   // 单播消息,发送给订阅了消息的模块
        {
            result = sendSyncUnicastMessage(message);
        }
        else if (message->getMessageDistribute() == Message_Broadcast) // 广播消息，发送给所有模块
        {
            sendSyncBroadcastMessage(message);
        }
    }
    else // 异步消息
    {
        if (message->getMessageDistribute() == Message_Unicast)   // 单播消息,发送给订阅了消息的模块
        {
            sendAsyncUnicastMessage(message);
        }
        else if (message->getMessageDistribute() == Message_Broadcast) // 广播消息，发送给所有模块
        {
            sendASyncBroadcastMessage(message);
        }
    }

    // 消息路由模块自己处理的消息
    onProcessMessage(message);

    // 偷窥消息的回应
    if (NULL != result.get())
    {
        bool ret = processForeseeResponse(result);
        if (ret)
        {
            result.reset();
        }
    }

    return result;
}

// 预先处理消息
bool MessageRoute::processForeseeMessage(std::shared_ptr<BaseMessage> &message)
{
    bool result = false;
    std::unique_lock<std::mutex> autolock(mForeseeMessageProcessLocker);

    auto iterProcess = mForeseeMessageProcess.find(message->getMessageType());
    if (iterProcess == mForeseeMessageProcess.end())
    {
        return result;
    }

    for (auto &process : iterProcess->second)
    {
        bool ret = process->onForeseeMessage(message);
        if (ret)
        {
            result = ret;
        }
    }

    return result;
}

// 预先处理回应
bool MessageRoute::processForeseeResponse(std::shared_ptr<BaseResponse> &response)
{
    bool result = false;
    std::unique_lock<std::mutex> autolock(mForeseeResponseProcessLocker);

    auto iterProcess = mForeseeResponseProcess.find(response->getMessage()->getMessageType());
    if (iterProcess == mForeseeResponseProcess.end())
    {
        return result;
    }

    for (auto &process : iterProcess->second)
    {
        bool ret = process->onForeseeResponse(response);
        if (ret)
        {
            result = ret;
        }
    }

    return result;
}

// 发送同步的订阅消息
std::shared_ptr<BaseResponse> MessageRoute::sendSyncUnicastMessage(std::shared_ptr<BaseMessage> &message)
{
    std::shared_ptr<BaseResponse> result;
    auto iterProcess = mSubscribeMessageProcess.find(message->getMessageType());
    if (iterProcess == mSubscribeMessageProcess.end())
    {
        return result;
    }

    for (auto &process : iterProcess->second)
    {
        // 如果某个模块处理消息之后返回了回应消息，则消息不继续向下传递
        result = process->onProcessMessage(message);
        if (NULL != result.get())
        {
            break;
        }
    }

    // 广播消息的回应
    if(NULL != result.get())
    {
        for (BaseProcess *process : mAllProcess)
        {
            process->onProcessResponse(result);
        }
    }
    return result;
}

// 发送同步的广播消息
void MessageRoute::sendSyncBroadcastMessage(std::shared_ptr<BaseMessage> &message)
{
    std::vector<std::shared_ptr<BaseResponse>> vectResponse;
    for (auto &process : mAllProcess)
    {
        std::shared_ptr<BaseResponse> response = process->onProcessMessage(message);
        if (response.get() != NULL)
        {
            vectResponse.push_back(response);
        } 
    }

    // 广播消息的回应
    for (auto response : vectResponse)
    {
        sendResponse(response);
    }
}

// 发送异步的订阅消息
void MessageRoute::sendAsyncUnicastMessage(std::shared_ptr<BaseMessage> &message)
{
    auto iterProcess = mSubscribeMessageProcess.find(message->getMessageType());
    if (iterProcess == mSubscribeMessageProcess.end())
    {
        return;
    }

    for (auto &process : iterProcess->second)
    {
        process->putMessageToBuffer(message); 
    }
}

//  发送异步的广播消息
void MessageRoute::sendASyncBroadcastMessage(std::shared_ptr<BaseMessage> &message)
{
    for (auto &process : mAllProcess)
    {
        process->putMessageToBuffer(message);
    }
}

// 发送消息的回应(消息的回应是广播的)
void MessageRoute::sendResponse(std::shared_ptr<BaseResponse> response)
{
    // 如果程序即将退出，则不处理任何消息
    if (mExit)
    {
        return;
    }

    // 偷窥消息的回应
    if (NULL != response.get())
    {
        bool ret = processForeseeResponse(response);
        if (ret)
        {
            return;
        }
    }

    for (auto &process : mAllProcess)
    {
        process->putResponseToBuffer(response);
    }
}

// 开始启动
bool MessageRoute::beginWork()
{
    // 对所有的模块进行初始化
    for (auto &process : mAllProcess)
    {
        process->init();
    }

    for (auto &process : mAllProcess)
    {
        process->beginWork();
    }

    mLastFreeMemoryTime = time(NULL);
    while (mExit == false)
    {
        // 暂时先只休眠
        std::chrono::milliseconds sleepTime(mSleepTime);
        std::this_thread::sleep_for(sleepTime);

        long long current = time(NULL);
        if (current < mLastFreeMemoryTime)
        {
            mLastFreeMemoryTime = current;
        }
        else if ((current - mLastFreeMemoryTime) > 60 * 60) // 一个小时释放一次内存
        {
            mLastFreeMemoryTime = current;
#ifndef WIN32
            malloc_trim(0);
            LOG_I(mClassName, "free memory....");
#endif
        }

        if (mReload)
        {
            for (auto &process : mAllProcess)
            {
                process->uninit();
            }

            for (auto &process : mAllProcess)
            {
                process->init();
            }

            for (auto &process : mAllProcess)
            {
                process->beginWork();
            }
            mReload = false;
        }
    }
    exitService();
    LOG_I(mClassName, "exit main thread, service exit");
    return true;
}

// 消息路由模块自己也会处理一些消息
void MessageRoute::onProcessMessage(std::shared_ptr<BaseMessage> &message)
{
    switch(message->getMessageType())
    {
    case Service_Control_Message:   // 控制服务的消息
        onProcessControlServiceMessage(message);
        break;
    }
}

// 控制服务的消息
void MessageRoute::onProcessControlServiceMessage(std::shared_ptr<BaseMessage> &message)
{
    std::shared_ptr<ServiceControlMessage> controlMessage = std::dynamic_pointer_cast<ServiceControlMessage>(message);
    switch (controlMessage->getClassType())
    {
    case Operate_Exit:      // 退出服务
        mExit = true;
        break;
    case Operate_Reload:
        mReload = true;
        break;
    }
}

// 退出服务
void MessageRoute::exitService()
{
    // 清除个个模块
    {
        std::unique_lock<std::mutex> autoLocker(mSubscribeMessageProcessLocker);
        mSubscribeMessageProcess.clear();
    }

    {
        std::unique_lock<std::mutex> autoLocker(mForeseeMessageProcessLocker);
        mForeseeMessageProcess.clear();
    }

    {
        std::unique_lock<std::mutex> autoLocker(mForeseeResponseProcessLocker);
        mForeseeResponseProcess.clear();
    }

    {
        std::unique_lock<std::mutex> autoLocker(mAllProcessLocker);
        for (BaseProcess * process : mAllProcess)
        {
            process->uninit();
        }
        mAllProcess.clear();
    }
}
