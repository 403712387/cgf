#include <sstream>
#include "ServiceStatusInfo.h"
#include "GetServiceStatusMessage.h"

GetServiceStatusMessage::GetServiceStatusMessage(MessageTransType transType, PriorityType priority , MessageDistriubite distribute)
    :BaseMessage(Service_Status_Message, transType, priority, distribute)
{
}

std::string GetServiceStatusMessage::toString()
{
    return BaseMessage::toString();
}

/*
 * 获取服务状态的回应
*/
GetServiceStatusResponse::GetServiceStatusResponse(std::shared_ptr<ServiceStatusInfo> info, std::shared_ptr<BaseMessage> message, std::shared_ptr<Error> error)
    :BaseResponse(message, error)
{
    mServiceStatusInfo = info;
}

// 获取服务状态信息
std::shared_ptr<ServiceStatusInfo> GetServiceStatusResponse::getServiceStatusInfo()
{
    return mServiceStatusInfo;
}

std::string GetServiceStatusResponse::toString()
{
    std::stringstream buf;
    buf << "service status:" << mServiceStatusInfo->toString() << ", " << BaseResponse::toString();
    return buf.str();
}
