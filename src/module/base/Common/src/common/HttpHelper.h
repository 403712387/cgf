#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H
#include <string>
#include <mutex>
#include <memory>
#include <condition_variable>
#include "Base.h"

class CGF_EXPORT HttpHelper
{
public:

    // 下载文件,10s没有下载完认为是超时
    static std::shared_ptr<std::string> downloadFile(std::string url, int timeout = 10);

private:
    // 接收到下载的内容
    static int receiveDownloadData(char *data, size_t size, size_t nmemb, std::string * writerData);

private:
    static std::string         mClassName;
};

#endif
