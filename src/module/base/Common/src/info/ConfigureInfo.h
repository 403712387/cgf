#ifndef CONFIGURE_INFO_H
#define CONFIGURE_INFO_H
#include <QtCore/QMap>
#include <mutex>
#include "Common.h"
#include "jsoncpp/json.h"
/*
 * 配置信息
*/
class CGF_EXPORT ConfigureInfo
{
public:

    // 日志级别
    void setLogLevel(common_log::Log_Level level);
    std::string getLogLevel();

    // 日志的关键字
    void setLogKeyword(std::string keyword);
    std::string getLogKeyword();

    // 设置本机监听的端口
    void setHttpPort(int port);
    int getHttpPort();

    std::string toString();
    Json::Value toJson();

private:
    std::string                 mClassName = "ConfigureInfo";
    common_log::Log_Level       mLogLevel = common_log::Log_Level_Info; // 默认日志的级别时info级别
    std::string                 mLogKeyword;

    int                         mHttpPort = 1224;       // 本机监听的端口
};

#endif
