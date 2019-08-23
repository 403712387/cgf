#include <cstdlib>
#include <stdio.h>

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>

#define _access access
#endif

#include "LogManager.h"

LogManager::LogManager()
{
}

LogManager *LogManager::getInstance()
{
	static LogManager *manager = NULL;
	if (NULL == manager)
	{
		manager = new LogManager();
        manager->init();
	}
	return manager;
}

void LogManager::init()
{
	Logger root = Logger::getRoot();
    mLogger = Logger::getInstance(LOG4CPLUS_TEXT("log"));
}

// 初始化日志配置
bool LogManager::initConfig()
{
	static bool isInit = false;
	if (isInit == true)
	{
		return isInit;
	}

	// 如果没有默认配置，则写入默认配置
	if (mConfigFile.empty() || _access(mConfigFile.c_str(), 0) != 0)
	{
		mConfigFile = mDefaultConfig;

		if (_access(mConfigFile.c_str(), 0) != 0)
		{
			FILE *fp = fopen(mConfigFile.c_str(), "w+");
			if (fp == NULL)
			{
				printf("create log config %s fail....\n", mConfigFile.c_str());
				return false;
			}
			fwrite(mConfieFileText.c_str(), 1, mConfieFileText.size(), fp);
			fclose(fp);
			fp = NULL;
		}
	}

    PropertyConfigurator::doConfigure(LOG4CPLUS_STRING_TO_TSTRING(mConfigFile));

	isInit = true;
	return isInit;
}

// 设置日志的配置文件，如果不设置，则使用默认配置
int LogManager::setLogConfigFile(const std::string configFile)
{
	if (!mConfigFile.empty() || configFile.empty())
	{
		return -1;
	}
	mConfigFile = configFile;

    initConfig();
	return 0;
}

// 设置日志级别
void LogManager::setLogLevel(common_log::Log_Level logLevel)
{
	mLogLevel = logLevel;
}

// 获取日志级别
common_log::Log_Level LogManager::getLogLevel()
{
    return (common_log::Log_Level)mLogLevel;
}

// 设置过滤关键字
void LogManager::setFilterWord(const std::string keyWord)
{
	mKeyWord = keyWord;
}

// 写日志
void LogManager::writeLog(const std::string &info, common_log::Log_Level logLevel, bool forceWrite)
{
	if (mKeyWord.empty())		// 没有设置关键字
	{
		// 级别较低的日志，不写入
        if (logLevel < mLogLevel && !forceWrite)
		{
			return;
		}	
	}
	else	// 设置了关键字，根据关键字过滤
	{
        if (std::string::npos == info.find(mKeyWord) && logLevel < mLogLevel && !forceWrite)
		{
			return;
		}
	}

	// 配置日志
    if (!initConfig())
	{
		printf("write log fail, init log conf fail, log info:%s\n", info.c_str());
		return;
	}

	// 写入日志
	switch (logLevel)
	{
    case common_log::Log_Level_Verbose:
        LOG4CPLUS_TRACE(mLogger, LOG4CPLUS_STRING_TO_TSTRING(info));
		break;
    case common_log::Log_Level_Debug:
        LOG4CPLUS_DEBUG(mLogger, LOG4CPLUS_STRING_TO_TSTRING(info));
		break;
    case common_log::Log_Level_Info:
        LOG4CPLUS_INFO(mLogger, LOG4CPLUS_STRING_TO_TSTRING(info));
		break;
    case common_log::Log_Level_Warn:
        LOG4CPLUS_WARN(mLogger, LOG4CPLUS_STRING_TO_TSTRING(info));
        break;
    case common_log::Log_Level_Error:
        LOG4CPLUS_ERROR(mLogger, LOG4CPLUS_STRING_TO_TSTRING(info));
		break;
    case common_log::LOG_Level_Fatal:
        LOG4CPLUS_FATAL(mLogger, LOG4CPLUS_STRING_TO_TSTRING(info));
        break;
	default:
        LOG4CPLUS_INFO(mLogger, LOG4CPLUS_STRING_TO_TSTRING(info));
		break;
	}
}
