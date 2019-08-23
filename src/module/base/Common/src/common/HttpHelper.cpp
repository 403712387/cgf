#include "Log.h"
#include "HttpHelper.h"
#include "curl/curl.h"

std::string HttpHelper::mClassName  = "HttpHelper";

// 下载图片
std::shared_ptr<std::string> HttpHelper::downloadFile(std::string url, int timeout)
{
    std::shared_ptr<std::string> result;
    std::string downloadData;
    CURL * handle = curl_easy_init();
    if (handle == NULL)
    {
        LOG_E(mClassName, "download file fail, failed to init curl handle, url:" << url);
        return result;
    }

    curl_slist *httpHeader = NULL;
    std::string httpConnect = "Connection: keep-alive";
    httpHeader = curl_slist_append(httpHeader, httpConnect.c_str());
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &downloadData);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, HttpHelper::receiveDownloadData);
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, timeout);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, timeout);
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, httpHeader);

    // 下载三次
    CURLcode ret = CURLE_GOT_NOTHING;
    long responseStatusCode = 0;
    for (int i = 0; i < 3; ++i)
    {
        if (0 != i)
        {
            curl_easy_setopt(handle, CURLOPT_NOBODY, 1L);
            curl_easy_setopt(handle, CURLOPT_HEADER, 1L);

            ret = curl_easy_perform(handle);

            if (ret != CURLE_OK)
            {
                LOG_W(mClassName, "download  faild, url: " << url << ", reason:" << curl_easy_strerror(ret));
                continue;
            }

            curl_easy_setopt(handle, CURLOPT_NOBODY, 0L);
            curl_easy_setopt(handle, CURLOPT_HEADER, 0L);
            curl_easy_setopt(handle, CURLOPT_APPEND, 1L);
        }
        else
        {
            curl_easy_setopt(handle, CURLOPT_APPEND, 0L);
        }

        ret = curl_easy_perform(handle);
        if (ret == CURLE_OK)
        {
            curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &responseStatusCode);
            break;
        }
    }
    curl_easy_cleanup(handle);

    if (!downloadData.empty() && 200 == responseStatusCode)
    {
        result = std::make_shared<std::string>(downloadData);
    }

    return result;
}

// 接收到下载的内容
int HttpHelper::receiveDownloadData(char *data, size_t size, size_t nmemb, std::string * writerData)
{
    size_t sizes = size * nmemb;
    if (writerData == NULL)
    {
        return 0;
    }

    writerData->append(data, sizes);
    return (int)sizes;
}

