#ifndef ERROR_H
#define ERROR_H
#include <string>
#include "Base.h"

// 记录错误信息的类
class CGF_EXPORT Error
{
public:
    Error(std::string reason, long long code = 0);

    // 获取错误原因
    std::string getErrorReason();

    // 获取错误码
    long long getErrorCode();

    std::string toString();

private:
    std::string         mReason;
    long long           mCode = 0;

};
#endif
