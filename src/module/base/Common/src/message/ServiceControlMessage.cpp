#include <sstream>
#include "ServiceControlMessage.h"

ServiceControlMessage::ServiceControlMessage(ServiceOperateType controlType, MessageTransType transType,
                     PriorityType priority, MessageDistriubite distribute)
    :BaseMessage(Service_Control_Message, transType, priority, distribute)
{
    mServiceControl = controlType;
}

// 获取控制服务的类型
ServiceOperateType ServiceControlMessage::getServiceOperateType()
{
    return mServiceControl;
}

std::string ServiceControlMessage::toString()
{
    std::stringstream buf;
    buf  << "service control type:" << Common::getServiceOperateTypeName(mServiceControl) << ", " << BaseMessage::toString();
    return buf.str();
}
