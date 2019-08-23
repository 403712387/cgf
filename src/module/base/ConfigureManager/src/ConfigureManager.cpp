#include <stdio.h>
#include <sstream>
#include "ConfigureInfo.h"
#include "ConfigureJson.h"
#include "ConfigureManager.h"
#include "ConfigureInfoMessage.h"
ConfigureManager::ConfigureManager(MessageRoute *messageRoute, std::string configFile)
    :BaseProcess(messageRoute, "ConfigureManager")
{
    mConfigFile = configFile;
    mConfigureInfo = std::make_shared<ConfigureInfo>();
}

bool ConfigureManager::init()
{
    LOG_I(mClassName, "begin init");
    mConfigureFile = std::make_shared<ConfigureJson>(mConfigFile);
    LOG_I(mClassName, "configure info:" << mConfigureFile->toString());

    //  设置配置信息
    setConfigureInfo();

    // 发送配置消息
    sendConfigureMessage();

    LOG_I(mClassName, "end module");
    return true;
}

void ConfigureManager::beginWork()
{
    LOG_I(mClassName, "begin work, module " << getModuleName());
}

// 卸载模块
void ConfigureManager::uninit()
{
    LOG_I(mClassName, "begin uninit module ");
    mConfigureFile.reset();
    BaseProcess::uninit();
    LOG_I(mClassName, "end uninit module ");
}

// 处理消息的函数
std::shared_ptr<BaseResponse> ConfigureManager::onProcessMessage(std::shared_ptr<BaseMessage> &message)
{
    std::shared_ptr<BaseResponse> result;
    switch (message->getMessageType())
    {
    default:
        break;
    }
    return result;
}

// 处理消息的回应
void ConfigureManager::onProcessResponse(std::shared_ptr<BaseResponse> &response)
{

}

// 设置配置信息
void ConfigureManager::setConfigureInfo()
{
    if (NULL == mConfigureFile.get() || NULL == mConfigureInfo.get())
    {
        return;
    }

    // 设置监听的端口
    mConfigureInfo->setHttpPort(mConfigureFile->getHttpPort(1224));

    // 设置日志级别
    mConfigureInfo->setLogLevel(common_log::getLogLevel());
}

// 发送配置消息
void ConfigureManager::sendConfigureMessage()
{
    if (NULL == mConfigureInfo.get())
    {
        return;
    }

    std::shared_ptr<ConfigureInfoMessage> message = std::make_shared<ConfigureInfoMessage>(mConfigureInfo);
    sendMessage(message);
}

