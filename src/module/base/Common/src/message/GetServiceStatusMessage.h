#ifndef GET_SERVICE_STATUS_MESSAGE_H
#define GET_SERVICE_STATUS_MESSAGE_H
#include "BaseMessage.h"

/*
 * 服务的状态消息
*/
class ServiceStatusInfo;
class CGF_EXPORT  GetServiceStatusMessage : public BaseMessage
{
public:
    GetServiceStatusMessage(MessageTransType transType = Async_Trans_Message,
                         PriorityType priority = Priority_Third, MessageDistriubite distribute = Message_Unicast);

    virtual std::string toString();
};

/*
 * 服务状态的回应
 */
class CGF_EXPORT GetServiceStatusResponse : public BaseResponse
{
public:
    GetServiceStatusResponse(std::shared_ptr<ServiceStatusInfo> info, std::shared_ptr<BaseMessage> message, std::shared_ptr<Error> error);

    // 获取服务状态信息
    std::shared_ptr<ServiceStatusInfo> getServiceStatusInfo();

    virtual std::string toString();

private:
    std::shared_ptr<ServiceStatusInfo>      mServiceStatusInfo;
};

#endif
