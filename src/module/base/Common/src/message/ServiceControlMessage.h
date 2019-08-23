#ifndef SERVICE_CONTROL_MESSAGE_H
#define SERVICE_CONTROL_MESSAGE_H
#include "Common.h"
#include "BaseMessage.h"

/*
 * 控制服务的消息
*/
class CGF_EXPORT  ServiceControlMessage : public BaseMessage
{
public:
    ServiceControlMessage(ServiceOperateType controlType = Operate_Exit, MessageTransType transType = Async_Trans_Message,
                         PriorityType priority = Priority_Third, MessageDistriubite distribute = Message_Unicast);

    // 获取控制服务的类型
    ServiceOperateType getServiceOperateType();

    virtual std::string toString();

private:
    ServiceOperateType      mServiceControl = Operate_Exit;
};

#endif
