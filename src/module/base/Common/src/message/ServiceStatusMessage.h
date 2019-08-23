#ifndef SERVICE_STATUS_MESSAGE_H
#define SERVICE_STATUS_MESSAGE_H
#include "BaseMessage.h"

/*
 * 服务的状态消息
*/
class ServiceStatusInfo;
class CGF_EXPORT  ServiceStatusMessage : public BaseMessage
{
public:
    ServiceStatusMessage(std::shared_ptr<ServiceStatusInfo> info, MessageTransType transType = Async_Trans_Message,
                         PriorityType priority = Priority_Third, MessageDistriubite distribute = Message_Unicast);

    // 获取服务状态信息
    std::shared_ptr<ServiceStatusInfo> getServiceStatusInfo();

    virtual std::string toString();

private:
    std::shared_ptr<ServiceStatusInfo>      mServiceStatusInfo;
};

#endif
