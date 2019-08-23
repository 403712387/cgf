#ifndef SERVICE_STATUS_INFO_H
#define SERVICE_STATUS_INFO_H
#include <QMap>
#include <mutex>
#include <QDateTime>
#include "Common.h"
#include "jsoncpp/json.h"

/*
 * 服务的状态信息
*/
class SpeedStatisticList;
class CGF_EXPORT ServiceStatusInfo
{
public:
    // 启动时间
    void setStartupTime(QDateTime startup = QDateTime::currentDateTime());
    std::string getStartupTime();

    // git分支信息
    void setGitBranch(std::string branch);
    std::string getGitBranch();

    // git commit信息
    void setGitCommitId(std::string commitId);
    std::string getGitCommitId();

    // 各个库的版本信息
    void setLibraryVersion(std::string libraryName, std::string version);
    std::string getLibraryVersion(std::string libraryName);
    QMap<std::string, std::string> getLibraryVersion();

    // git信息
    std::string gitInfoToString();
    Json::Value gitInfoToJson();

    // 积压信息
    std::string overstockToString();
    Json::Value overstockToJson();

    // 第三方库信息
    std::string libraryInfoToString();
    Json::Value libraryInfoToJson();

    // 时间信息
    Json::Value timeToJson();

    std::string toString();
    Json::Value toJson();

private:
    std::string     mClassName = "ServiceStatusInfo";
    QDateTime       mStartupTime = QDateTime::currentDateTime();    // 程序启动时间
    std::string     mGitBranch = "unknown";
    std::string     mGitCommitId = "unknown";

    QMap<std::string, std::string>      mLibraryVersion;     // 第三方库的版本信息
    std::mutex                          mMutexLibraryVersion;
};

#endif
