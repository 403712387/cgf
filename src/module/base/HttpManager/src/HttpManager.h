#ifndef HTTP_MANAGER_H
#define HTTP_MANAGER_H
#include <string>
#include <memory>
#include "Common.h"
#include "BaseProcess.h"
#include "libmicrohttpd/microhttpd.h"

/*
 * http管理类
 */
class ConfigureInfo;
class CGF_EXPORT HttpManager : public BaseProcess
{
public:
    HttpManager(MessageRoute *messageRoute);

    virtual bool init();
    virtual void beginWork();

    // 卸载模块
    virtual void uninit();

    // 处理消息的函数
    virtual std::shared_ptr<BaseResponse> onProcessMessage(std::shared_ptr<BaseMessage> &message);

    // 处理消息的回应
    virtual void onProcessResponse(std::shared_ptr<BaseResponse> &response);

private:
    // 开始监听
    bool startListen(int port);

    // 停止监听
    bool stopListen();

    // 是否正在监听
    bool isListening();

private:
    // 处理配置消息
    std::shared_ptr<BaseResponse> onProcessConfigMessage(std::shared_ptr<BaseMessage> message);

private:
    // 处理http请求回调
    static int onProcessCallback(void *cls, MHD_Connection *connection, const char *url,const char *method, const char *version,
                  const char *upload_data, size_t *upload_data_size, void **con_cls);

    // 处理http请求
    int onProcess(void *cls, MHD_Connection *connection, const char *url,const char *method, const char *version,
                  const char *upload_data, size_t *upload_data_size, void **con_cls);

    // 获取配置信息
    std::string onGetConfigureInfo(std::string &body);

    // 设置配置信息
    std::string onSetConfigureInfo(std::string &body);

    // 获取服务状态
    std::string onGetServiceStatus(std::string &body);

    // 服务控制
    std::string onControlService(std::string &body);

    // index页面
    std::string onProcessIndex(std::string &body);

    // 开始cpu性能分析
    std::string onStartCpuProfile(std::string &body);

    // 停止cpu性能分析
    std::string onStopCpuProfile(std::string &body);

    // 开始内存性能分析
    std::string onStartHeapProfile(std::string &body);

    // 停止内存性能分析
    std::string onStopHeapProfile(std::string &body);

    // 下载文件
    std::string onDownloadFile(std::string url);

    // 获取CPU使用情况
    std::string onGetCPUStatisticInfo();

    // 获取进程统计信息
    std::string onGetProcessStatisticInfo();

    // 获取磁盘使用情况
    std::string onGetDiskStatisticInfo();

    // 获取内存使用情况
    std::string onGetMemoryStatisticInfo();

    // 获取平台信息
    std::string onGetPlatformInfo();

    // 获取http的body
    bool getHttpBody(void *cls, MHD_Connection *connection, const char *url,const char *method, const char *version,
                     const char *upload_data, size_t *upload_data_size, void **ptr, std::string &body);
private:
    // 发送回应
    bool sendResponse(MHD_Connection *connection, std::string info, std::string url, HttpBodyType bodyType);

    // 获取呼应的body
    std::string getResponseBody(int status, std::string reason);
private:
    int             mHttpPort = 0;   // http 监听端口
    MHD_Daemon      *mHttpHandle = NULL;
    static HttpManager *mHttpService;

    std::shared_ptr<ConfigureInfo>  mConfigureInfo;
};

#endif
