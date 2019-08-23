#ifndef PANDA_MESSAGE_H
#define PANDA_MESSAGE_H
#include <string>
#include <memory>
#include <QDateTime>
#include "Error.h"
#include "Base.h"

//类类型，消息还是消息的回应
enum ClassType
{
    Type_Message = 0,           // 消息类型
    Type_Message_Response = 1,  // 消息的回应
};

// 消息如何分发，是按照订阅/分发方式，还是按照广播方式
enum MessageDistriubite
{
    Message_Unicast = 0,  // 订阅-发布模式
    Message_Broadcast = 1,  // 广播模式，所有模块都能接受
};

// 同步消息还是异步消息
enum MessageTransType
{
    Sync_Trans_Message = 0,     // 同步消息
    Async_Trans_Message = 1,    // 异步消息
};

// 消息类型
enum MessageType
{
    Base_Mesage = 0,
    Service_Control_Message ,   // 控制服务的消息
    Config_Message ,         // 配置消息
    Service_Status_Message , // 运行状态信息
};

// 优先级的类型
enum PriorityType
{
    Priority_First = 0,     // 第一优先级(最高优先级)
    Priority_Second = 1,    // 第二优先级
    Priority_Third = 2,    // 第三优先级
    Priority_Fourth = 3,    // 第四优先级
    Priority_Fifth = 4,     // 第五优先级(最低优先级)
};

// 消息和消息回应的类都从此类中继承
class CGF_EXPORT BaseClass
{
public:
    BaseClass(ClassType type);

    ClassType getClassType();

    virtual std::string toString();

    // 获取消息类型的字符串
    static std::string getMessageTypeName(MessageType type);

    // 根据字符串，获取消息类型
    static MessageType getMessageTypeByName(std::string name);

protected:

    // 获取消息发送类型的字符串
    std::string getMessageDistriubiteName(MessageDistriubite type);

    // 获取消息传输类型的字符串
    std::string getMessageTransTypeName(MessageTransType type);
private:
    ClassType       mClassType = Type_Message;
};

//  基础消息，不能被实例化
class CGF_EXPORT BaseMessage : public BaseClass
{
public:
    BaseMessage(MessageType type, MessageTransType transType = Async_Trans_Message, 
        PriorityType priority = Priority_Third, MessageDistriubite distribute = Message_Unicast);
    BaseMessage(const BaseMessage&) = delete;  //禁止拷贝
    BaseMessage & operator = (const BaseMessage&) = delete;  // 禁止赋值

    // 获取消息的类型
    MessageType getMessageType();

    // 获取消息的传输类型
    MessageTransType getTransType();

    // 获取消息的优先级
    PriorityType getMessagePriority();

    // 获取消息的范围
    MessageDistriubite getMessageDistribute();

    // 生成消息的时间
    QDateTime getMessageBirthday();

    // 获取消息的信息
    virtual std::string toString();

    // 获取消息的序列号
    long long getMessageSequence();
protected:
    // 获取消息优先级的字符串
    std::string getPriorityOfString();

    // 获取消息分发范围的字符串
    std::string getDistributeOfString();

private:
    // 生成一个新的序列号
    static long long getNewSequence();

protected:
    long long              messageSequence = 0;
    MessageType            mMessageType = Base_Mesage;     // 消息类型
    MessageTransType       mTransType = Async_Trans_Message;    // 消息传送方式，默认为异步发送
    PriorityType           mMessagePriority = Priority_Third;   // 消息的级别
    MessageDistriubite     mMessageDistribute = Message_Unicast;  // 默认为多播消息
    QDateTime              mBirthday;       // 生成消息的时间
};

// 基础消息的回应
class CGF_EXPORT BaseResponse : public BaseClass
{
public:
    BaseResponse(std::shared_ptr<BaseMessage> message, std::shared_ptr<Error> error);

    std::shared_ptr<BaseMessage> getMessage();

    // 消息是否成功处理
    bool isSuccessfulProcess();

    // 获取出错的原因
    std::string getErrorReason();

    // 获取消息回应的生成时间
    QDateTime getResponseBirthday();

    // 获取错误信息
    std::shared_ptr<Error> getError();

    // 获取回应的信息
    virtual std::string toString();
protected:
    std::shared_ptr<BaseMessage> mMessage;
    std::shared_ptr<Error>       mError;
    QDateTime                    mBirthday;
};


#endif
