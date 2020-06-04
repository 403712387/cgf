#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "Log.h"

#include "log4cplus/logger.h"
#include "log4cplus/configurator.h"
#include "log4cplus/helpers/stringhelper.h"
#include "log4cplus/loggingmacros.h"
#include "log4cplus/fileappender.h"
using namespace log4cplus;

class LogManager
{
public:
    static LogManager *getInstance();

	// 设置日志的配置文件，如果不设置，则使用默认配置
    int setLogConfigFile(const std::string configFile);

	// 设置日志级别
    void setLogLevel(common_log::Log_Level logLevel);

    // 获取日志级别
    common_log::Log_Level getLogLevel();

	// 设置过滤关键字
    void setFilterWord(const std::string keyWord);

	// 写日志
    void writeLog(const std::string &info, common_log::Log_Level logLevel = common_log::Log_Level_Info, bool forceWrite = false);

private:
	LogManager();

	// 初始化
    void init();

	// 初始化日志配置
    bool initConfig();

private:
    const std::string			mDefaultConfig = "config/log.conf";
    int							mLogLevel = common_log::Log_Level_Info;			// 日志级别
	Logger						mLogger;							// 日志的模块类
	std::string					mConfigFile;						// 配置文件名称
	std::mutex					mHandleLock;						// 句柄的锁
	std::string					mKeyWord;

	std::string					mConfieFileText = \
		"/*\n"
		"*/\n"
        "#Default level is INFO, but you can use others in your favor\n"
        "log4cplus.rootLogger=TRACE,DEBUG,ALL_MSGS,ERROR_MSGS,FATAL_MSGS,D\n\n\n"
		"log4cplus.appender.D=log4cplus::ConsoleAppender\n"
		"log4cplus.appender.D.layout=log4cplus::PatternLayout\n"
		"log4cplus.appender.D.layout.ConversionPattern=[%D{%Y-%m-%d %H:%M:%S,%Q}] [%t] %-5p %m%n\n"		
		"log4cplus.appender.ALL_MSGS=log4cplus::RollingFileAppender\n"
		"log4cplus.appender.ALL_MSGS.MaxFileSize=10MB\n"
		"log4cplus.appender.ALL_MSGS.CreateDirs=true\n"
		"log4cplus.appender.ALL_MSGS.MaxBackupIndex=10\n"
        "log4cplus.appender.ALL_MSGS.File=logs/cgf.log\n"
		"log4cplus.appender.ALL_MSGS.layout=log4cplus::PatternLayout\n"
		"log4cplus.appender.ALL_MSGS.layout.ConversionPattern=[%D{%Y-%m-%d %H:%M:%S,%Q}] [%t] %-5p %m%n\n\n"
		"#Range\n"		
		"log4cplus.appender.ALL_MSGS.filters.1=log4cplus::spi::LogLevelRangeFilter\n"
		"log4cplus.appender.ALL_MSGS.filters.1.LogLevelMin=TRACE\n"
		"log4cplus.appender.ALL_MSGS.filters.1.LogLevelMax=FATAL\n"
		"log4cplus.appender.ALL_MSGS.filters.1.AcceptOnMatch=true\n"
		"log4cplus.appender.ALL_MSGS.filters.2=log4cplus::spi::DenyAllFilter\n\n"
        "log4cplus.appender.WARN_MSGS=log4cplus::RollingFileAppender\n"
        "log4cplus.appender.WARN_MSGS.MaxFileSize=10MB\n"
        "log4cplus.appender.WARN_MSGS.CreateDirs=true\n"
        "log4cplus.appender.WARN_MSGS.MaxBackupIndex=5\n"
        "log4cplus.appender.WARN_MSGS.File=logs/cgf.warn\n"
        "log4cplus.appender.WARN_MSGS.layout=log4cplus::PatternLayout\n"
        "log4cplus.appender.WARN_MSGS.layout.ConversionPattern=[%D{%Y-%m-%d %H:%M:%S,%Q}] [%t] %-5p %m%n\n"
        "log4cplus.appender.WARN_MSGS.filters.1=log4cplus::spi::LogLevelMatchFilter\n"
        "log4cplus.appender.WARN_MSGS.filters.1.LogLevelToMatch=WARN\n"
        "log4cplus.appender.WARN_MSGS.filters.1.AcceptOnMatch=true\n"
        "log4cplus.appender.WARN_MSGS.filters.2=log4cplus::spi::DenyAllFilter\n"
        "log4cplus.appender.ERROR_MSGS=log4cplus::RollingFileAppender\n"
		"log4cplus.appender.ERROR_MSGS.MaxFileSize=10MB\n"
		"log4cplus.appender.ERROR_MSGS.CreateDirs=true\n"
		"log4cplus.appender.ERROR_MSGS.MaxBackupIndex=5\n"
        "log4cplus.appender.ERROR_MSGS.File=logs/cgf.error\n"
		"log4cplus.appender.ERROR_MSGS.layout=log4cplus::PatternLayout\n"
		"log4cplus.appender.ERROR_MSGS.layout.ConversionPattern=[%D{%Y-%m-%d %H:%M:%S,%Q}] [%t] %-5p %m%n\n"
		"log4cplus.appender.ERROR_MSGS.filters.1=log4cplus::spi::LogLevelMatchFilter\n"
		"log4cplus.appender.ERROR_MSGS.filters.1.LogLevelToMatch=ERROR\n"
		"log4cplus.appender.ERROR_MSGS.filters.1.AcceptOnMatch=true\n"
		"log4cplus.appender.ERROR_MSGS.filters.2=log4cplus::spi::DenyAllFilter\n"
        "log4cplus.appender.FATAL_MSGS=log4cplus::RollingFileAppender\n"
		"log4cplus.appender.FATAL_MSGS.MaxFileSize=10MB\n"
		"log4cplus.appender.FATAL_MSGS.CreateDirs=true\n"
		"log4cplus.appender.FATAL_MSGS.MaxBackupIndex=5\n"
        "log4cplus.appender.FATAL_MSGS.File=logs/cgf.fatal\n"
		"log4cplus.appender.FATAL_MSGS.layout=log4cplus::PatternLayout\n"
		"log4cplus.appender.FATAL_MSGS.layout.ConversionPattern=[%D{%Y-%m-%d %H:%M:%S,%Q}] [%t] %-5p %m%n\n"
		"log4cplus.appender.FATAL_MSGS.filters.1=log4cplus::spi::LogLevelMatchFilter\n"
		"log4cplus.appender.FATAL_MSGS.filters.1.LogLevelToMatch=FATAL\n"
		"log4cplus.appender.FATAL_MSGS.filters.1.AcceptOnMatch=true\n"
		"log4cplus.appender.FATAL_MSGS.filters.2=log4cplus::spi::DenyAllFilter\n";
};

#endif
