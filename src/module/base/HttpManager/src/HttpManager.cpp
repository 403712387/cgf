#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <sstream>
#include "Log.h"
#include "HttpHelper.h"
#include "JsonHelper.h"
#include "HttpManager.h"
#include "ConfigureInfo.h"
#include "ServiceStatusInfo.h"
#include "ServiceControlMessage.h"
#include "GetServiceStatusMessage.h"
#include "ConfigureInfoMessage.h"
#include "jsoncpp/json.h"

struct TPostStatus
{
    bool status = false;
    std::string buffer;
};

HttpManager *HttpManager::mHttpService = NULL;
HttpManager::HttpManager(MessageRoute *messageRoute)
    :BaseProcess(messageRoute, "HttpManager")
{
    mHttpService = this;

    // 订阅消息
    subscribeMessage(Config_Message);
}

bool HttpManager::init()
{
    LOG_I(mClassName, "begin init");
    LOG_I(mClassName, "end init");
    return true;
}

void HttpManager::beginWork()
{
    LOG_I(mClassName, "begin work");
}

// 反初始化
void HttpManager::uninit()
{
    stopListen();
    LOG_I(mClassName, "uninit http manager, port:" << mHttpPort);
}

// 处理消息的函数
std::shared_ptr<BaseResponse> HttpManager::onProcessMessage(std::shared_ptr<BaseMessage> &message)
{
    std::shared_ptr<BaseResponse> response;
    switch(message->getMessageType())
    {
    case Config_Message:        // 配置信息
        response = onProcessConfigMessage(message);
    }

    return response;
}

// 处理消息的回应
void HttpManager::onProcessResponse(std::shared_ptr<BaseResponse> &response)
{

}

// 初始化服务
bool HttpManager::startListen(int port)
{
    if (isListening())
    {
        LOG_W(mClassName, "http server already run, listen port:" << mHttpPort << ", want listen port:" << port);
        return false;
    }

    // 监听端口
    unsigned int flags = MHD_USE_EPOLL_INTERNAL_THREAD | MHD_USE_ERROR_LOG;

    // windows下只支持select
#ifdef WIN32
    flags = MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG;
#endif

    mHttpHandle = MHD_start_daemon(flags, port, NULL, NULL, HttpManager::onProcessCallback, NULL, MHD_OPTION_END);
    if (NULL == mHttpHandle)
    {
        LOG_F(mClassName, "start http service fail, port:" << port);
        return false;
    }

    mHttpPort = port;
    LOG_I(mClassName, "start http service, port:" << port);
    return true;
}

// 停止监听
bool HttpManager::stopListen()
{
    if (!isListening())
    {
        LOG_W(mClassName, "http not listen");
        return false;
    }

    MHD_stop_daemon(mHttpHandle);
    LOG_I(mClassName, "stop http listen");
    return true;
}

// 是否正在监听
bool HttpManager::isListening()
{
    return (NULL != mHttpHandle);
}

// 处理配置消息
std::shared_ptr<BaseResponse> HttpManager::onProcessConfigMessage(std::shared_ptr<BaseMessage> message)
{
    std::shared_ptr<BaseResponse> response;
    std::shared_ptr<ConfigureInfoMessage> configureMessage = std::dynamic_pointer_cast<ConfigureInfoMessage>(message);
    mConfigureInfo = configureMessage->getConfigureInfo();

    // 如果http端口发生改变，则要重新监听
    if (mHttpPort != mConfigureInfo->getHttpPort())
    {
        if (isListening())
        {
            stopListen();
        }
        mHttpPort = mConfigureInfo->getHttpPort();
        startListen(mHttpPort);
    }

    return response;
}

// 处理http请求回调
int HttpManager::onProcessCallback(void *cls, MHD_Connection *connection, const char *url,const char *method, const char *version,
              const char *upload_data, size_t *upload_data_size, void **con_cls)
{
    return mHttpService->onProcess(cls, connection, url, method, version, upload_data, upload_data_size, con_cls);
}

// 处理http请求
int HttpManager::onProcess(void *cls, MHD_Connection *connection, const char *url,const char *method, const char *version,
              const char *upload_data, size_t *upload_data_size, void **con_cls)
{
    QString requestUrl(url);
    std::string body;
    HttpBodyType httpBodyType = Http_Body_Json;
    bool ret = getHttpBody(cls, connection, url, method, version, upload_data, upload_data_size, con_cls, body);
    if (false == ret)
    {
        return MHD_YES;
    }

    std::string response;
    if (requestUrl.compare("/index", Qt::CaseInsensitive) == 0) // index页面
    {
        response = onProcessIndex(body);
        httpBodyType = Http_Body_Html;
    }
    else if (requestUrl.compare("/get/configure", Qt::CaseInsensitive) == 0) // 获取配置信息
    {
        response = onGetConfigureInfo(body);
    }
    else if (requestUrl.compare("/set/configure", Qt::CaseInsensitive) == 0) // 获取配置信息
    {
        response = onSetConfigureInfo(body);
    }
    else if (requestUrl.compare("/get/service/status", Qt::CaseInsensitive) == 0) // 获取服务状态
    {
        response = onGetServiceStatus(body);
    }
    else if (requestUrl.compare("/control/service", Qt::CaseInsensitive) == 0)  // 服务控制
    {
        response =onControlService(body);
    }
    else
    {
        LOG_E(mClassName, "not find http request process, url " << url);
        response = getResponseBody(404, "don not touch me");
    }

    ret = sendResponse(connection, response, url, httpBodyType);
    return ret ? MHD_YES : MHD_NO;
}

// 获取配置信息
std::string HttpManager::onGetConfigureInfo(std::string &body)
{
    std::string result;
    if (NULL == mConfigureInfo.get())
    {
        result = getResponseBody(500, "get configure fail");
    }
    else
    {
        result = mConfigureInfo->toJson().toStyledString();
    }
    return result;
}

// 设置配置信息
std::string HttpManager::onSetConfigureInfo(std::string &body)
{
    bool ret = JsonHelper::parseConfigure(body, mConfigureInfo);
    if (!ret)
    {
        return getResponseBody(400, "parse json fail");
    }

    // 发送更新配置消息
    std::shared_ptr<ConfigureInfoMessage> message = std::make_shared<ConfigureInfoMessage>(mConfigureInfo);
    sendMessage(message);

    return getResponseBody(200, "OK");
}

// 获取服务状态
std::string HttpManager::onGetServiceStatus(std::string &body)
{
    std::string result;

    // 获取服务器状态
    std::shared_ptr<GetServiceStatusMessage> message = std::make_shared<GetServiceStatusMessage>(Sync_Trans_Message);
    std::shared_ptr<BaseResponse> response = sendMessage(message);
    std::shared_ptr<GetServiceStatusResponse> serviceResponse = std::dynamic_pointer_cast<GetServiceStatusResponse>(response);
    if (NULL == serviceResponse.get())
    {
        LOG_E(mClassName, "get service status fail, response is NULL");
        result = getResponseBody(500, "get service status fail");
        return result;
    }

    // 获取服务状态信息
    Json::Value serviceStatus = serviceResponse->getServiceStatusInfo()->toJson();
    result = serviceStatus.toStyledString();

    return result;
}

// 服务控制
std::string HttpManager::onControlService(std::string &body)
{
    std::string result;
    ServiceOperateType type = JsonHelper::parseServiceControl(body);
    std::shared_ptr<ServiceControlMessage> message = std::make_shared<ServiceControlMessage>(type);
    sendMessage(message);
    result = getResponseBody(200, "OK");
    return result;
}

// 发送回应
bool HttpManager::sendResponse(MHD_Connection *connection, std::string info, std::string url, HttpBodyType bodyType)
{
    // 返回结果
    std::string responseJson = info;
    struct MHD_Response *response = MHD_create_response_from_buffer(responseJson.size(),(void*)responseJson.c_str(), MHD_RESPMEM_MUST_COPY);

    std::string httpBodyType = Common::getHttpTypeName(bodyType);
    if (MHD_add_response_header(response, "Content-Type", httpBodyType.c_str()) == MHD_NO)
    {
        LOG_E(mClassName, "create response head fail when process request, url:" << url);
        return false;
    }
    if (MHD_queue_response(connection, MHD_HTTP_OK, response) == MHD_NO)
    {
        LOG_E(mClassName, "put response fail when process request, url:" << url);
        return false;
    }

    if (Http_Body_Image != bodyType)
    {
        LOG_I(mClassName, "send response:" << info);
    }

    MHD_destroy_response(response);
    return true;
}

// 获取http的body
bool HttpManager::getHttpBody(void *cls, MHD_Connection *connection, const char *url,const char *method, const char *version,
                 const char *upload_data, size_t *upload_data_size, void **ptr, std::string &body)
{
    TPostStatus *post = NULL;
    post = (struct TPostStatus*)*ptr;

    if (NULL == post)
    {
        post = new TPostStatus();
        post->status = false;
        *ptr = post;
    }

    if (!post->status)
    {
        post->status = true;
        return false;
    }
    else
    {
        if (*upload_data_size != 0)
        {
            std::string data = std::string(upload_data, *upload_data_size);
            post->buffer += data;
            *upload_data_size = 0;
            return false;
        }
        else
        {
            body = post->buffer;
        }
    }

    if (post != NULL)
    {
        delete post;
    }
    return true;
}

// 获取呼应的body
std::string HttpManager::getResponseBody(int status, std::string reason)
{
    std::string responseJson = "{\"status\":" + std::to_string(status) + ", \"reason\":\"" + reason + "\"}";
    return responseJson;
}

// index页面
std::string HttpManager::onProcessIndex(std::string &body)
{
    std::string result = "Welcome to cfg(c++ general framework)";
    return result;
}
