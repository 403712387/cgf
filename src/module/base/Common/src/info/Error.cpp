#include <sstream>
#include "Error.h"

Error::Error(std::string reason, long long code)
{
    mReason = reason;
    mCode = code;
}

// 获取错误原因
std::string Error::getErrorReason()
{
    return mReason;
}

// 获取错误码
long long Error::getErrorCode()
{
    return mCode;
}

std::string Error::toString()
{
    std::stringstream buf;
    buf << "reason:" << mReason << ", code:" << mCode;
    return buf.str();
}
