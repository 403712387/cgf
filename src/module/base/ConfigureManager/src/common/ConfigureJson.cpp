#include <QFile>
#include <QString>
#include <sstream>
#include "ConfigureInfo.h"
#include "ConfigureJson.h"
#include "jsoncpp/json.h"

ConfigureJson::ConfigureJson(std::string file)
{
    mConfigureFile = file;
    loadConfigure();
}

// 从配置文件中加载配置信息
bool ConfigureJson::loadConfigure()
{
    std::string data = Common::readFile(mConfigureFile);
    if (data.empty())
    {
        return false;
    }

    Json::Reader reader;
    Json::Value readerRoot;
    bool ret = reader.parse(data, readerRoot);
    if (false == ret)
    {
        LOG_E(mClassName, "prase json info fail, info:" << data);
        return false;
    }

    std::string videoService = "service";
    if (readerRoot.isMember(videoService))
    {
        // http port
        if (readerRoot[videoService].isMember("http_port"))
        {
            mHttpPort = readerRoot[videoService]["http_port"].asInt();
        }
    }

    return true;
}

// 获取http接口
int ConfigureJson::getHttpPort(int defaultPort)
{
    return (mHttpPort > 0) ? mHttpPort : defaultPort;
}

std::string ConfigureJson::toString()
{
    std::stringstream buf;
    buf << "http port:" << mHttpPort;
    return buf.str();
}
