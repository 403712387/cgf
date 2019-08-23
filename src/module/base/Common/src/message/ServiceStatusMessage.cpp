#include <sstream>
#include "ServiceStatusInfo.h"
#include "ServiceStatusMessage.h"

ServiceStatusMessage::ServiceStatusMessage(std::shared_ptr<ServiceStatusInfo> info, MessageTransType transType, PriorityType priority , MessageDistriubite distribute)
    :BaseMessage(Service_Status_Message, transType, priority, distribute)
{
    mServiceStatusInfo = info;
}

// 获取服务状态信息
std::shared_ptr<ServiceStatusInfo> ServiceStatusMessage::getServiceStatusInfo()
{
    return mServiceStatusInfo;
}

std::string ServiceStatusMessage::toString()
{
    std::stringstream buf;
    buf << "service status: " << mServiceStatusInfo->toString() << ", " << BaseMessage::toString();
    return buf.str();
}
