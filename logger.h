#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QMutex>
#include <QTimer>
#include <QQueue>
#include <QThread>
#include <QDebug>

class Logger : public QObject
{
    Q_OBJECT
public:
    enum LogLevel {
        Debug = 0,
        Info = 1,
        Warning = 2,
        Error = 3,
        Critical = 4
    };

    // 日志条目结构
    struct LogEntry {
        QDateTime timestamp;
        LogLevel level;
        QString message;
        QString threadId;
        QString function;
        int line;
        
        LogEntry() : level(Info), line(0) {}
        LogEntry(LogLevel l, const QString &msg, const QString &func = "", int ln = 0) 
            : timestamp(QDateTime::currentDateTime()), level(l), message(msg), 
              threadId(QString::number(reinterpret_cast<quintptr>(QThread::currentThread()) & 0xFFFF)),
              function(func), line(ln) {}
    };

    static Logger* getInstance();
    static void log(LogLevel level, const QString &message, const QString &function = "", int line = 0);
    static void close();
    static QString getLogContent();
    static bool exportLog(const QString &filePath);
    static void setLogLevel(LogLevel level);
    static LogLevel getLogLevel();

    void setLogPath(const QString &path);
    QString getLogPath() const;
    
    // 日志统计
    int getLogCount(LogLevel level) const;
    int getTotalLogCount() const;
    QDateTime getFirstLogTime() const;
    QDateTime getLastLogTime() const;
    
    // 日志过滤
    void setLogFilter(LogLevel minLevel);
    QStringList getFilteredLogs(LogLevel minLevel = Debug) const;
    
    // 日志清理
    void clearOldLogs(int daysToKeep = 7);
    void setMaxLogSize(qint64 maxSize);
    void setMaxLogFiles(int maxFiles);

signals:
    void logMessage(const QString &message);
    void logLevelChanged(LogLevel level);
    void logStatsUpdated();

private slots:
    void processLogQueue();
    void rotateLogFile();

private:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();
    
    static Logger* instance;
    static QMutex mutex;
    
    QString logPath;
    QFile* logFile;
    QTextStream* logStream;
    QQueue<LogEntry> logQueue;
    QTimer* processTimer;
    QTimer* rotateTimer;
    
    LogLevel currentLogLevel;
    LogLevel filterLevel;
    qint64 maxLogSize;
    int maxLogFiles;
    QDateTime firstLogTime;
    QDateTime lastLogTime;
    
    // 统计信息
    int logCounts[5]; // 对应5个日志级别
    
    // 去重机制 - 存储最近的日志消息
    QString lastLogMessage;
    LogLevel lastLogLevel;
    
    void writeLog(const LogEntry &entry);
    QString formatLogMessage(const LogEntry &entry);
    QString getLogLevelString(LogLevel level);
    QString getLogLevelColor(LogLevel level);
    void updateStats(const LogEntry &entry);
    bool shouldLog(LogLevel level) const;
    bool isDuplicateLog(const LogEntry &entry) const;
    void checkLogRotation();
    QString generateLogFileName() const;
};

// 便捷宏定义
#define LOG_DEBUG(msg) Logger::log(Logger::Debug, msg, __FUNCTION__, __LINE__)
#define LOG_INFO(msg) Logger::log(Logger::Info, msg, __FUNCTION__, __LINE__)
#define LOG_WARNING(msg) Logger::log(Logger::Warning, msg, __FUNCTION__, __LINE__)
#define LOG_ERROR(msg) Logger::log(Logger::Error, msg, __FUNCTION__, __LINE__)
#define LOG_CRITICAL(msg) Logger::log(Logger::Critical, msg, __FUNCTION__, __LINE__)

#endif // LOGGER_H