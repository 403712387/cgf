#include <malloc.h>
#include <QDateTime>
#include "Log.h"
#include "JsonHelper.h"
#include "BaseMessage.h"
#include "ConfigureInfo.h"
#include "jsoncpp/json.h"

std::string JsonHelper::mClassName = "JsonHelper";

// 解析服务控制
ServiceOperateType JsonHelper::parseServiceControl(std::string &info)
{
    ServiceOperateType result = Operate_Exit;
    if (info.empty())
    {
        return result;
    }

    Json::Reader reader;
    Json::Value readerRoot;
    bool ret = reader.parse(info, readerRoot);
    if (false == ret)
    {
        LOG_E(mClassName, "prase service control fail");
        return result;
    }

    try
    {
        // 解析type
        if (!readerRoot.isMember("type"))
        {
            return result;
        }

        std::string value = readerRoot["type"].asString();
        if ("exit" == value)
        {
            result = Operate_Exit;
        }
    }
    catch (...)
    {
        LOG_E(mClassName, "prase service control exception");
    }

    return result;
}
