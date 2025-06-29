#ifndef SPDLOGWRAPPER_HPP
#define SPDLOGWRAPPER_HPP

#include <QDebug>
#include <QDateTime>
#include <QString>
#include <QMessageLogger>
#include <QFile>
#include <QTextStream>
#include <QMutex>

// 日志级别枚举
enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Critical
};

// 日志管理器类
class Logger {
public:
    // 设置当前日志级别
    static void setLogLevel(LogLevel level);

    // 设置日志输出文件
    static void setLogFile(const QString& filePath);

    // 日志记录函数
    static void log(LogLevel level, const char* file, int line, const char* function, const char* format, ...);

    // 记录函数进入
    static void logFunctionEntry(const char* file, int line, const char* function);

private:
    static LogLevel currentLogLevel;
    static QFile logFile;
    static QTextStream logStream;
    static QMutex mutex;

    // 将日志级别转换为字符串
    static QString logLevelToString(LogLevel level);
};

// 简化的日志调用宏
#define L_TRACE(fmt, ...) Logger::log(LogLevel::Trace, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define L_DEBUG(fmt, ...) Logger::log(LogLevel::Debug, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define L_INFO(fmt, ...)  Logger::log(LogLevel::Info, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define L_WARN(fmt, ...)  Logger::log(LogLevel::Warn, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define L_ERROR(fmt, ...) Logger::log(LogLevel::Error, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define L_CRITICAL(fmt, ...) Logger::log(LogLevel::Critical, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define L_FUNCTION() Logger::logFunctionEntry(__FILE__, __LINE__, __FUNCTION__)


#endif // SPDLOGWRAPPER_HPP
