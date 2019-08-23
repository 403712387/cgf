#ifndef CONFIGURE_INFO_MESSAGE_H
#define CONFIGURE_INFO_MESSAGE_H
#include "BaseMessage.h"

/*
 * 配置信息的消息
*/
class ConfigureInfo;
class CGF_EXPORT  ConfigureInfoMessage : public BaseMessage
{
public:
    ConfigureInfoMessage(std::shared_ptr<ConfigureInfo> info, MessageTransType transType = Async_Trans_Message,
                         PriorityType priority = Priority_Third, MessageDistriubite distribute = Message_Unicast);

    // 获取配置信息
    std::shared_ptr<ConfigureInfo> getConfigureInfo();

    virtual std::string toString();

private:
    std::shared_ptr<ConfigureInfo>      mConfigureInfo;
};

#endif
