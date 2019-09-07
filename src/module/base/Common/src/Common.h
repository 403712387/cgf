#ifndef COMMON_COMMON_H
#define COMMON_COMMON_H
#include <vector>
#include <string>
#include <QtCore/QRect>
#include <QtCore/QDateTime>
#include "Log.h"
#include "message/BaseMessage.h"

// 本机监听的端口类型
enum PortType
{
    Port_Http = 1,          // http端口
};

// 服务控制
enum ServiceOperateType
{
    Operate_Exit = 0,       // 退出服务
    Operate_Reload,         // 重新加载
};

// http的响应类型
enum HttpBodyType
{
    Http_Body_Json = 0,         // json格式
    Http_Body_Image = 1,        // 图片格式
    Http_Body_Html = 2,         // html格式
    Http_Body_File,             // 文件格式
};

namespace Common
{
    // 根据字符串获取日志级别
    CGF_EXPORT common_log::Log_Level getLogLevelFromString(std::string level);

    // 获取日志级别的字符串
    CGF_EXPORT std::string getLogLevelName(common_log::Log_Level level);

    // 获取服务控制类型的字符串
    CGF_EXPORT std::string getServiceOperateTypeName(ServiceOperateType type);

    // 设置线程名字
    CGF_EXPORT void setThreadName(std::string name);

    // 生成一个唯一的ID
    CGF_EXPORT long long getUniqueId();

    // 生成一个sequence ID(sequence ID是从0开始自增的，Unique ID不是)
    CGF_EXPORT long long getSequenceId();

    // 获取端口的名字
    CGF_EXPORT std::string getPortTypeName(PortType type);

    // 获取目录下的所有文件
    CGF_EXPORT std::vector<std::string> getFilesInDirectory(std::string directory);

    // 获取目录下的一个文件
    CGF_EXPORT std::string getFileInDirectory(std::string directory);

    // 数据写入文件
    CGF_EXPORT bool writeFile(std::string fileName, std::string &fileData);

    // 读文件
    CGF_EXPORT std::string readFile(std::string fileName);

    // 获取易读的容量(比如，1.2G，100.7P这种)
    CGF_EXPORT std::string getReadableCapacitySize(long long bytes);

    // 获取http body tpye的字符串
    CGF_EXPORT std::string getHttpTypeName(HttpBodyType type);

    // 创建目录
    CGF_EXPORT bool createPath(std::string path);

    // 获取Error信息
    CGF_EXPORT std::shared_ptr<Error> getError(std::string reason, long long code = 0);

    // 没有错误
    CGF_EXPORT std::shared_ptr<Error> noError();

    // 判断是否为有效的url
    CGF_EXPORT bool isValidUrl(std::string url);
}

#endif

