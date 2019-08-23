#include <stdio.h>
#include <sstream>
#include "ServiceStatusInfo.h"
#include "ServiceStatusMessage.h"
#include "ServiceStatusManager.h"
#include "curl/curl.h"
#include "jsoncpp/json.h"
#include "libmicrohttpd/microhttpd.h"
#include "log4cplus/version.h"
#include "cryptopp/cryptlib.h"

#define _STR(x) _VAL(x)
#define _VAL(x) #x

ServiceStatusManager::ServiceStatusManager(MessageRoute *messageRoute)
    :BaseProcess(messageRoute, "ServiceStatusManager")
{
    // 初始化状态信息
    initServiceStatusInfo();

    // 获取第三方库信息
    initLibraryVersionInfo();
}

// 初始化服务的状态信息
void ServiceStatusManager::initServiceStatusInfo()
{
    // 服务的状态信息
    mServiceStatusInfo = std::make_shared<ServiceStatusInfo>();
    mServiceStatusInfo->setStartupTime(QDateTime::currentDateTime());
#ifdef GIT_BRANCH
    mServiceStatusInfo->setGitBranch(_STR(GIT_BRANCH));
#endif
#ifdef GIT_COMMIT_ID
    mServiceStatusInfo->setGitCommitId(_STR(GIT_COMMIT_ID));
#endif
}

// 初始化模块的版本信息
void ServiceStatusManager::initLibraryVersionInfo()
{
    std::string curlVersion = LIBCURL_VERSION;
    std::string jsoncppVersion = JSONCPP_VERSION_STRING;
    std::string libmicrohttpdVersion = MHD_get_version();
    std::string log4cplusVersion = LOG4CPLUS_VERSION_STR;
    std::string qtVersion = qVersion();

    mServiceStatusInfo->setLibraryVersion("curl", curlVersion);
    mServiceStatusInfo->setLibraryVersion("jsoncpp", jsoncppVersion);
    mServiceStatusInfo->setLibraryVersion("libmicrohttpd", libmicrohttpdVersion);
    mServiceStatusInfo->setLibraryVersion("log4cplus", log4cplusVersion);
    mServiceStatusInfo->setLibraryVersion("Qt", qtVersion);
}

bool ServiceStatusManager::init()
{
    LOG_I(mClassName, "init module " << getModuleName());

    // 打印git信息
    LOG_I(mClassName, "git info:" << mServiceStatusInfo->gitInfoToString());

    // 打印库信息
    LOG_I(mClassName, "library version info:" << mServiceStatusInfo->libraryInfoToString());

    return true;
}

void ServiceStatusManager::beginWork()
{
    LOG_I(mClassName, "begin work, module " << getModuleName());
}

// 卸载模块
void ServiceStatusManager::uninit()
{
    LOG_I(mClassName, "begin uninit ");
    BaseProcess::uninit();
    LOG_I(mClassName, "end uninit module ");
}

// 处理消息的函数
std::shared_ptr<BaseResponse> ServiceStatusManager::onProcessMessage(std::shared_ptr<BaseMessage> &message)
{
    std::shared_ptr<BaseResponse> response;
    return response;
}

// 偷窥消息的处理函数
bool ServiceStatusManager::onForeseeMessage(std::shared_ptr<BaseMessage> &message)
{
    return false;
}

// 处理消息的回应
void ServiceStatusManager::onProcessMessageResponse(std::shared_ptr<BaseResponse> &response)
{
}
