#ifndef PANDA_LOG_H
#define PANDA_LOG_H
#include <string>
#include <sstream>
#include "Base.h"

// 打印日志
#define LOG_I(moduleName, info)  {std::stringstream tmpInfo; tmpInfo << "["<< moduleName << "] - " << info;common_log::writeLog(tmpInfo.str(), common_log::Log_Level_Info);}
#define LOG_W(moduleName, info)  {std::stringstream tmpInfo; tmpInfo << "["<< moduleName << "] - " << info;common_log::writeLog(tmpInfo.str(), common_log::Log_Level_Warn);}
#define LOG_E(moduleName, info)  {std::stringstream tmpInfo; tmpInfo << "["<< moduleName << "] - " << info;common_log::writeLog(tmpInfo.str(), common_log::Log_Level_Error);}
#define LOG_D(moduleName, info)  {std::stringstream tmpInfo; tmpInfo << "["<< moduleName << "] - " << info;common_log::writeLog(tmpInfo.str(), common_log::Log_Level_Debug);}
#define LOG_V(moduleName, info)  {std::stringstream tmpInfo; tmpInfo << "["<< moduleName << "] - " << info;common_log::writeLog(tmpInfo.str(), common_log::Log_Level_Verbose);}
#define LOG_F(moduleName, info)  {std::stringstream tmpInfo; tmpInfo << "["<< moduleName << "] - " << info;common_log::writeLog(tmpInfo.str(), common_log::LOG_Level_Fatal);}

// 打印参数(第三个参数代表是否强制写入日志)
#define LOG_I_F(moduleName, info, forceWrite)  {std::stringstream tmpInfo; tmpInfo << "["<< moduleName << "] - " << info;common_log::writeLog(tmpInfo.str(), common_log::Log_Level_Info, forceWrite);}
#define LOG_W_F(moduleName, info, forceWrite)  {std::stringstream tmpInfo; tmpInfo << "["<< moduleName << "] - " << info;common_log::writeLog(tmpInfo.str(), common_log::Log_Level_Warn, forceWrite);}
#define LOG_E_F(moduleName, info, forceWrite)  {std::stringstream tmpInfo; tmpInfo << "["<< moduleName << "] - " << info;common_log::writeLog(tmpInfo.str(), common_log::Log_Level_Error, forceWrite);}
#define LOG_D_F(moduleName, info, forceWrite)  {std::stringstream tmpInfo; tmpInfo << "["<< moduleName << "] - " << info;common_log::writeLog(tmpInfo.str(), common_log::Log_Level_Debug, forceWrite);}
#define LOG_V_F(moduleName, info, forceWrite)  {std::stringstream tmpInfo; tmpInfo << "["<< moduleName << "] - " << info;common_log::writeLog(tmpInfo.str(), common_log::Log_Level_Verbose, forceWrite);}
#define LOG_F_F(moduleName, info, forceWrite)  {std::stringstream tmpInfo; tmpInfo << "["<< moduleName << "] - " << info;common_log::writeLog(tmpInfo.str(), common_log::LOG_Level_Fatal, forceWrite);}

namespace common_log
{
	enum Log_Level
	{
		Log_Level_Verbose = 0,		// 详细的日志级
		Log_Level_Debug = 10,		// debug级别的日
		Log_Level_Info = 20,		// info级别的打
        Log_Level_Warn = 30,		// warn级别的打
        Log_Level_Error = 40,		// error 级别的打
        LOG_Level_Fatal = 50,       // 致命的错误
	};

	// 设置日志的配置文件，如果不设置，则使用默认配
    CGF_EXPORT void setLogConfigFile(const std::string fileConfig);

	// 设置日志级别
    CGF_EXPORT void setLogLevel(Log_Level logLevel);

    // 获取日志级别
    CGF_EXPORT Log_Level getLogLevel();

	// 设置过滤关键
    CGF_EXPORT void setFilterWord(const std::string keyWord);

    // 写日志(forceWrite如果为true,无论日志级别，强制写入)
    CGF_EXPORT void writeLog(const std::string &info, Log_Level logLevel = Log_Level_Info, bool forceWrite = false);
}

#endif
