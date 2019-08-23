#include  "Log.h"
#include "log/LogManager.h"

// 设置日志的配置文件，如果不设置，则使用默认配置
void common_log::setLogConfigFile(const std::string fileConfig)
{
    LogManager::getInstance()->setLogConfigFile(fileConfig);
}

// 设置日志级别
void common_log::setLogLevel(Log_Level logLevel)
{
    LogManager::getInstance()->setLogLevel(logLevel);
}

// 获取日志级别
common_log::Log_Level common_log::getLogLevel()
{
    return LogManager::getInstance()->getLogLevel();
}

// 设置过滤关键字
void common_log::setFilterWord(const std::string keyWord)
{
    LogManager::getInstance()->setFilterWord(keyWord);
}

// 写日志
void common_log::writeLog(const std::string &info, Log_Level logLevel, bool forceWrite)
{
    LogManager::getInstance()->writeLog(info, logLevel, forceWrite);
}
