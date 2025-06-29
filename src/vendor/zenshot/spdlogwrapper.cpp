#include "spdlogwrapper.hpp"

LogLevel Logger::currentLogLevel = (LogLevel)0;
QFile Logger::logFile;
QTextStream Logger::logStream;
QMutex Logger::mutex;

void Logger::setLogLevel(LogLevel level) {
    currentLogLevel = level;
}

void Logger::setLogFile(const QString &filePath) {
    QMutexLocker locker(&mutex);
    logFile.setFileName(filePath);
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        logStream.setDevice(&logFile);
    }
}

void Logger::log(LogLevel level, const char *file, int line, const char *function, const char *format, ...) {
    if (static_cast<int>(level) < static_cast<int>(currentLogLevel))
        return;

    // 获取时间戳
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    // 格式化日志消息
    va_list args;
    va_start(args, format);
    QString message = QString::vasprintf(format, args);
    va_end(args);

    // 构建完整日志行
    QString logLine = QString("[%1] [%2] [%3:%4] %5: %6")
                          .arg(timestamp)
                          .arg(logLevelToString(level))
                          .arg(file)
                          .arg(line)
                          .arg(function)
                          .arg(message);

    // 输出到Qt调试
    switch (level) {
    case LogLevel::Trace:
    case LogLevel::Debug:
        qDebug().noquote() << logLine;
        break;
    case LogLevel::Info:
        qInfo().noquote() << logLine;
        break;
    case LogLevel::Warn:
        qWarning().noquote() << logLine;
        break;
    case LogLevel::Error:
        qCritical().noquote() << logLine;
        break;
    case LogLevel::Critical:
        qFatal("%s", qPrintable(logLine)); // 注意：qFatal会终止程序
        break;
    }

    // 输出到文件（如果已设置）
    QMutexLocker locker(&mutex);
    if (logFile.isOpen()) {
        logStream << logLine << "\n";
        logStream.flush();
    }
}

void Logger::logFunctionEntry(const char *file, int line, const char *function) {
    log(LogLevel::Debug, file, line, function, "ENTER");
}

QString Logger::logLevelToString(LogLevel level) {
    switch (level) {
    case LogLevel::Trace: return "TRACE";
    case LogLevel::Debug: return "DEBUG";
    case LogLevel::Info:  return "INFO ";
    case LogLevel::Warn:  return "WARN ";
    case LogLevel::Error: return "ERROR";
    case LogLevel::Critical: return "CRITICAL";
    default: return "UNKNOWN";
    }
}
