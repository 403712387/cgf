#ifndef JSON_HELPER_H
#define JSON_HELPER_H
#include <map>
#include <vector>
#include "Common.h"

class Error;
class ConfigureInfo;
class JsonHelper
{
public:

    // 解析服务控制
    static ServiceOperateType parseServiceControl(std::string &info);

private:
    static std::string        mClassName;
};

#endif
