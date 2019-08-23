#include <sstream>
#include "ConfigureInfo.h"
#include "ConfigureInfoMessage.h"

ConfigureInfoMessage::ConfigureInfoMessage(std::shared_ptr<ConfigureInfo> info, MessageTransType transType,PriorityType priority, MessageDistriubite distribute)
    :BaseMessage(Config_Message, transType, priority, distribute)
{
    mConfigureInfo = info;
}

// 获取配置信息
std::shared_ptr<ConfigureInfo> ConfigureInfoMessage::getConfigureInfo()
{
    return mConfigureInfo;
}

std::string ConfigureInfoMessage::toString()
{
    std::stringstream buf;
    buf << "configure: " << mConfigureInfo->toString() << ", " << BaseMessage::toString();
    return buf.str();
}
