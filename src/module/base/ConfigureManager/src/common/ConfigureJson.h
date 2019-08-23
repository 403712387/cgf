#ifndef CONFIGURE_JSON_H
#define CONFIGURE_JSON_H
#include <string>
#include <memory>
/*
 *  从json配置文件中加载配置
*/
class ConfigureJson
{
public:
    ConfigureJson(std::string file);

    // 获取http接口
    int getHttpPort(int defaultPort = 1224);

    // 设置http接口
    void setHttpPort(int port);

    std::string toString();

private:
    // 从配置文件中加载配置信息
    bool loadConfigure();

    // 保存到配置文件
    bool saveConfigure();

private:
    std::string     mClassName = "ConfigureJson";
    std::string     mConfigureFile = "config/config.json";

    int             mHttpPort = -1;   // http端口
};

#endif
