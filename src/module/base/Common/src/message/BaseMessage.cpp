#include <sstream>
#include <atomic>
#include "BaseMessage.h"

BaseClass::BaseClass(ClassType type)
{
    mClassType = type;
}

ClassType BaseClass::getClassType()
{
    return mClassType;
}

// 获取消息类型的字符串
std::string BaseClass::getMessageTypeName(MessageType type)
{
    std::string result = "base message";
    switch(type)
    {
    case Service_Control_Message:  // 控制服务的消息
        result = "service control";
        break;
    case Config_Message:    // 配置信息
        result = "config message";
        break;
    case Storage_Message:   // 存储消息
        result = "storage";
        break;
    case Service_Status_Message: // 运行时信息
        result = "service status";
        break;
    case Get_Storage_Config_Message:    // 查询存储配置
        result = "get storage configure";
        break;
    case Set_Storage_Config_Message:    // 设置存储配置
        result = "set storage configure";
        break;
    case Login_Service_Message:     // 登陆服务组件
        result = "login service";
        break;
    case Send_Network_Message:      // 发送网络消息
        result = "send network";
        break;
    case Publish_Device_Message:    // 查询设备信息
        result = "publish device";
        break;
    case Remove_Device_Message:     // 删除设备信息
        result = "remove device";
        break;
    case Query_Device_Message:      // 查询设备信息
        result = "query device";
        break;
    case Play_Live_Message:         // 播放实时流
        result = "play live";
        break;
    case Play_Record_Message:       // 播放历史流
        result = "play record";
        break;
    case Stop_Play_Message:      // 停止播放流
        result = "stop play";
        break;
    case FFmpeg_Ingest_Message:     // ffmpeg拉取流
        result = "ffmpeg ingest";
        break;
    case Stop_Ingest_Message:       // 停止拉流
        result = "stop ingest";
        break;
    case Ingest_Exception_Message:  // 拉流异常
        result = "ingest exception";
        break;
    case Video_Frame_Message:   // 视频帧
        result = "video frame";
        break;
    case Ingest_Statistic_Message:  // 拉流统计
        result = "ingest statistic";
        break;
    case Publish_Statistic_Message: // 发布流统计
        result = "publish statistic";
        break;
    }
    return result;
}

// 根据字符串，获取消息类型
MessageType BaseClass::getMessageTypeByName(std::string name)
{
    MessageType result = Base_Mesage;
    if ("service control" == name)
    {
        result = Service_Control_Message;
    }
    else if ("config message"  == name)
    {
        result = Config_Message;
    }
    else if ("storage"  == name)
    {
        result = Storage_Message;
    }
    else if ("service status"  == name)
    {
        result = Service_Status_Message;
    }
    else if ("get storage configure" == name)
    {
        result = Get_Storage_Config_Message;
    }
    else if ("set storage configure" == name)
    {
        result = Set_Storage_Config_Message;
    }
    else if ("login service" == name)
    {
        result = Login_Service_Message;
    }
    else if ("send network" == name)
    {
        result = Send_Network_Message;
    } 
    else if ("publish device" == name)
    {
        result = Publish_Device_Message;
    }
    else if ("remove device" == name)
    {
        result = Remove_Device_Message;
    }
    else if ("query device" == name)
    {
        result = Query_Device_Message;
    }
    else if ("play live" == name)
    {
        result = Play_Live_Message;
    }
    else if ("play record" == name)
    {
        result = Play_Record_Message;
    }
    else if ("stop play" == name)
    {
        result = Stop_Play_Message;
    }
    else if ("ffmpeg ingest" == name)
    {
        result = FFmpeg_Ingest_Message;
    }
    else if ("stop ingest" == name)
    {
        result = Stop_Ingest_Message;
    }
    else if ("ingest exception" == name)
    {
        result = Ingest_Exception_Message;
    }
    else if ("video frame" == name)
    {
        result = Video_Frame_Message;
    }
    else if ("ingest statistic" == name)
    {
        result = Ingest_Statistic_Message;
    }
    else if ("publish statistic" == name)
    {
        result = Publish_Statistic_Message;
    }
    return result;
}

// 获取消息发送类型的字符串
std::string BaseClass::getMessageDistriubiteName(MessageDistriubite type)
{
   std::string result = "unicast";
   switch(type)
   {
   case Message_Broadcast:  // 广播
       result = "broadcase";
       break;
   }
   return result;
}

// 获取消息传输类型的字符串
std::string BaseClass::getMessageTransTypeName(MessageTransType type)
{
    std::string result = "async";
    switch(type)
    {
    case Sync_Trans_Message:    // 同步
        result = "sync";
        break;
    }
    return result;
}

std::string BaseClass::toString()
{
    return "i want to fly....";
}

//-----------------------基类消息---------------------------//
BaseMessage::BaseMessage(MessageType type, MessageTransType transType ,
    PriorityType priority, MessageDistriubite distribute )
    :BaseClass(Type_Message)
{
    mMessageType = type;
    mMessagePriority = priority;
    mMessageDistribute = distribute;
    mTransType = transType;
    messageSequence = getNewSequence();
    mBirthday = QDateTime::currentDateTime();
}

// 获取消息的类型
MessageType BaseMessage::getMessageType()
{
    return mMessageType;
}

// 获取消息的传输类型
MessageTransType BaseMessage::getTransType()
{
    return mTransType;
}

// 获取消息的优先级
PriorityType BaseMessage::getMessagePriority()
{
    return mMessagePriority;
}

// 获取消息的范围
MessageDistriubite BaseMessage::getMessageDistribute()
{
    return mMessageDistribute;
}

// 获取消息的信息
std::string BaseMessage::toString()
{
    std::stringstream result;
    result << "message type:" << getMessageTypeName(mMessageType) << ", trans type:" << getMessageTransTypeName(mTransType) << ", priority:" << mMessagePriority
        << ", distribute:" << getMessageDistriubiteName(mMessageDistribute) << ", sequence:" << messageSequence << ", birthday:" << mBirthday.toString("yyyy-MM-dd HH:mm:ss.zzz").toStdString();
    return result.str();
}

// 获取消息的序列号
long long BaseMessage::getMessageSequence()
{
    return messageSequence;
}

// 生成一个新的序列号
long long BaseMessage::getNewSequence()
{
    static std::atomic_llong sequence = {1};
    return sequence++;
}

// 生成消息的时间
QDateTime BaseMessage::getMessageBirthday()
{
    return mBirthday;
}

// 获取消息优先级的字符串
std::string BaseMessage::getPriorityOfString()
{
    std::string result = "unknow priority";
    switch (mMessagePriority)
    {
    case Priority_First:
        result = "priority first";
        break;
    case Priority_Second:
        result = "priority second";
        break;
    case Priority_Third:
        result = "priority third";
        break;
    case Priority_Fourth:
        result = "priority fourth";
        break;
    case Priority_Fifth:
        result = "priority fifth";
        break;
    }
    return result;
}

// 获取消息分发范围的字符串
std::string BaseMessage::getDistributeOfString()
{
    std::string result = "unknow distribute";
    switch (mMessageDistribute)
    {
    case Message_Unicast:
        result = "muliticast";
        break;
    case Message_Broadcast:
        result = "broadcast";
        break;
    }
    return result;
}

// base message消息的回应
BaseResponse::BaseResponse(std::shared_ptr<BaseMessage> message, std::shared_ptr<Error> error)
    :BaseClass(Type_Message_Response)
{
    mMessage = message;
    mError = error;
    mBirthday = QDateTime::currentDateTime();
}

std::shared_ptr<BaseMessage> BaseResponse::getMessage()
{
    return mMessage;
}

// 消息是否成功处理
bool BaseResponse::isSuccessfulProcess()
{
    return NULL != mError.get();
}

// 获取出错的原因
std::string BaseResponse::getErrorReason()
{
    if (NULL == mError.get())
    {
        return "";
    }

    return mError->getErrorReason();
}

// 获取消息回应的生成时间
QDateTime BaseResponse::getResponseBirthday()
{
    return mBirthday;
}

// 获取错误信息
std::shared_ptr<Error> BaseResponse::getError()
{
    return mError;
}

// 获取消息的信息
std::string BaseResponse::toString()
{
    std::stringstream buf;
    buf << "response birthday:" << mBirthday.toString("yyyy-MM-dd HH:mm:ss.zzz").toStdString() << ", " <<mMessage->toString();
    if (NULL != mError.get())
    {
        buf << ", error " << mError->toString();
    }
    return buf.str();
}
