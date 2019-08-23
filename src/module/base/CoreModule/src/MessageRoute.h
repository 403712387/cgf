#ifndef MESSAGE_ROUTE_H
#define MESSAGE_ROUTE_H
#include <map>
#include <vector>
#include <mutex>
#include <memory>
#include "BaseMessage.h"

class BaseProcess;

// 消息路由类，用来转发消息
class CGF_EXPORT MessageRoute
{
public:
    // 模块注册
    void registerProcess(BaseProcess *process);

    // 模块订阅消息
    void subscribeMessage(BaseProcess *process, MessageType messageType);

    // 预先处理消息（消息在订阅模块处理之前先处理，此函数是个hook函数,慎用！！！）
    void foreseeMessage(BaseProcess *process, MessageType messageType);

    // 预先处理回应
    void foreseeResponse(BaseProcess *process, MessageType messageType);

    // 模块取消消息订阅
    void unsubscribeMessage(BaseProcess *process, MessageType messageType);

    // 取消预先处理消息
    void unforeseeMessage(BaseProcess *process, MessageType messageType);

    // 取消预先处理回应
    void unForeseeResponse(BaseProcess *process, MessageType messageType);

    // 发送消息
    std::shared_ptr<BaseResponse> sendMessage(std::shared_ptr<BaseMessage> &message);

    // 发送消息的回应(消息的回应是广播的)
    void sendResponse(std::shared_ptr<BaseResponse> response);

    // 开始启动
    bool beginWork();

private:
    // 发送同步的订阅消息
    std::shared_ptr<BaseResponse> sendSyncUnicastMessage(std::shared_ptr<BaseMessage> &message);

    // 发送同步的广播消息
    void sendSyncBroadcastMessage(std::shared_ptr<BaseMessage> &message);

    // 发送异步的订阅消息
    void sendAsyncUnicastMessage(std::shared_ptr<BaseMessage> &message);

    //  发送异步的广播消息
    void sendASyncBroadcastMessage(std::shared_ptr<BaseMessage> &message);

    // 处理消息
    std::shared_ptr<BaseResponse> processSubscribeMessage(std::shared_ptr<BaseMessage> &message);

    // 预先处理消息
    bool processForeseeMessage(std::shared_ptr<BaseMessage> &message);

    // 预先处理回应
    bool processForeseeResponse(std::shared_ptr<BaseResponse> &response);
private:
    // 消息路由模块自己也会处理一些消息
    void onProcessMessage(std::shared_ptr<BaseMessage> &message);

    // 控制服务的消息
    void onProcessControlServiceMessage(std::shared_ptr<BaseMessage> &message);

    // 退出服务
    void exitService();

private:
    std::string                                 mClassName = "MessageRoute";
    std::vector<BaseProcess *>                  mAllProcess;       // 处理广播消息的模块
    std::mutex                                  mAllProcessLocker;

    std::map<MessageType, std::vector<BaseProcess*>>    mSubscribeMessageProcess;  // 处理多播消息的模块
    std::mutex                                   mSubscribeMessageProcessLocker;

    std::map<MessageType, std::vector<BaseProcess*>>    mForeseeMessageProcess;  // 事先处理消息的模块
    std::mutex                                   mForeseeMessageProcessLocker;

    std::map<MessageType, std::vector<BaseProcess*>>    mForeseeResponseProcess;  // 事后处理消息的模块
    std::mutex                                   mForeseeResponseProcessLocker;

    bool                                         mExit = false;
    bool                                         mReload = false;
    int                                          mSleepTime = 200;      //无事可做的时候，休眠200ms

    long long                                    mLastFreeMemoryTime = 0;   // 上次释放内存的时间
};

#endif
