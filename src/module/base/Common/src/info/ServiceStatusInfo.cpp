#include <sstream>
#include <thread>
#include "BaseMessage.h"
#include "SpeedStatisticList.h"
#include "ServiceStatusInfo.h"

// 启动时间
void ServiceStatusInfo::setStartupTime(QDateTime startup)
{
    mStartupTime = startup;
}

std::string ServiceStatusInfo::getStartupTime()
{
    return mStartupTime.toString("yyyy-MM-dd HH:mm:ss").toStdString();
}

// git分支信息
void ServiceStatusInfo::setGitBranch(std::string branch)
{
    mGitBranch = branch;
}

std::string ServiceStatusInfo::getGitBranch()
{
    return mGitBranch;
}

// git commit信息
void ServiceStatusInfo::setGitCommitId(std::string commitId)
{
    mGitCommitId = commitId;
}

std::string ServiceStatusInfo::getGitCommitId()
{
    return mGitCommitId;
}

// 各个库的版本信息
void ServiceStatusInfo::setLibraryVersion(std::string libraryName, std::string version)
{
    std::unique_lock<std::mutex> autoLock(mMutexLibraryVersion);
    mLibraryVersion.insert(libraryName, version);
}

std::string ServiceStatusInfo::getLibraryVersion(std::string libraryName)
{
    std::string result;
    std::unique_lock<std::mutex> autoLock(mMutexLibraryVersion);
    return mLibraryVersion.value(libraryName, result);
}

QMap<std::string, std::string> ServiceStatusInfo::getLibraryVersion()
{
    std::unique_lock<std::mutex> autoLock(mMutexLibraryVersion);
    return mLibraryVersion;
}

// git信息
std::string ServiceStatusInfo::gitInfoToString()
{
    std::stringstream buf;
    buf << "git branch:" << mGitBranch << ", git commit id:" << mGitCommitId;
    return buf.str();
}

Json::Value ServiceStatusInfo::gitInfoToJson()
{
    Json::Value result;
    result["branch"] = mGitBranch;
    result["commit"] = mGitCommitId;
    return result;
}

// 第三方库信息
std::string ServiceStatusInfo::libraryInfoToString()
{
    std::stringstream buf;
    QMap<std::string, std::string> librarys = getLibraryVersion();
    QMapIterator<std::string, std::string> iterator(librarys);
    while(iterator.hasNext())
    {
        iterator.next();
        buf << "library:" << iterator.key() << ", version:" << iterator.value() << "; ";
    }
    return buf.str();
}

Json::Value ServiceStatusInfo::libraryInfoToJson()
{
    Json::Value result;
    QMap<std::string, std::string> librarys = getLibraryVersion();
    QMapIterator<std::string, std::string> iterator(librarys);
    int index = 0;
    while(iterator.hasNext())
    {
        iterator.next();
        result[index]["name"] = iterator.key();
        result[index]["version"] =iterator.value();
        ++index;
    }
    return result;
}

std::string ServiceStatusInfo::toString()
{
    std::stringstream buf;
    buf << "startup time:" << getStartupTime() << ", " << gitInfoToString();
    buf << "library version info:\n" << libraryInfoToString();
    return buf.str();
}

// 时间信息
Json::Value ServiceStatusInfo::timeToJson()
{
    Json::Value result;
    result["startup"] = getStartupTime();
    result["current"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString();
    return result;
}

Json::Value ServiceStatusInfo::toJson()
{
    Json::Value result;
    result["time"] = timeToJson();
    result["git"] = gitInfoToJson();
    result["library"] = libraryInfoToJson();
    result["cpu_count"] = std::thread::hardware_concurrency();
    return result;
}
