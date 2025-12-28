#include "logger.h"
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QStandardPaths>
#include <QApplication>
#include <QMutexLocker>

// 静态成员初始化
Logger* Logger::instance = nullptr;
QMutex Logger::mutex;

Logger::Logger(QObject *parent) 
    : QObject(parent)
    , logFile(nullptr)
    , logStream(nullptr)
    , processTimer(nullptr)
    , rotateTimer(nullptr)
    , currentLogLevel(Debug) // 设置默认日志级别为Debug，方便调试
    , filterLevel(Debug)
    , maxLogSize(10 * 1024 * 1024) // 10MB
    , maxLogFiles(5)
{
    // 初始化统计信息
    for (int i = 0; i < 5; ++i) {
        logCounts[i] = 0;
    }
    
    // 设置日志路径为当前程序目录下的logs文件夹，避免中文和空格问题
    logPath = QCoreApplication::applicationDirPath() + "/logs/";
    
    // 确保日志路径存在
    QDir logDir(logPath);
    if (!logDir.exists()) {
        bool created = logDir.mkpath(".");
        qDebug() << "创建日志目录:" << logPath << "结果:" << created;
    }
    
    // 立即创建日志文件，确保ConfigManager的日志消息能被记录
    QString logFileName = generateLogFileName();
    qDebug() << "初始化日志文件:" << logFileName;
    
    logFile = new QFile(logFileName, this);
    if (logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qDebug() << "日志文件打开成功:" << logFileName;
        logStream = new QTextStream(logFile);
        logStream->setEncoding(QStringConverter::Utf8); // 设置UTF-8编码
        
        // 立即写入一条启动日志
        LogEntry entry(Info, "Logger initialized", __FUNCTION__, __LINE__);
        writeLog(entry);
        qDebug() << "Logger初始化完成";
    } else {
        qDebug() << "日志文件打开失败:" << logFileName << "错误:" << logFile->errorString();
    }
    
    // 创建定时器处理日志队列
    processTimer = new QTimer(this);
    processTimer->setInterval(100); // 100ms处理一次
    connect(processTimer, &QTimer::timeout, this, &Logger::processLogQueue);
    processTimer->start();
    
    // 创建定时器检查日志轮转
    rotateTimer = new QTimer(this);
    rotateTimer->setInterval(60000); // 1分钟检查一次
    connect(rotateTimer, &QTimer::timeout, this, &Logger::rotateLogFile);
    rotateTimer->start();
}

Logger::~Logger()
{
    if (processTimer) {
        processTimer->stop();
    }
    if (rotateTimer) {
        rotateTimer->stop();
    }
    
    // 处理剩余的日志条目
    processLogQueue();
    
    if (logStream) {
        delete logStream;
    }
    if (logFile) {
        logFile->close();
        delete logFile;
    }
}

Logger* Logger::getInstance()
{
    if (!instance) {
        QMutexLocker locker(&mutex);
        if (!instance) {
            instance = new Logger();
        }
    }
    return instance;
}

void Logger::log(LogLevel level, const QString &message, const QString &function, int line)
{
    if (!instance) {
        getInstance();
    }
    
    LogEntry entry(level, message, function, line);
    
    QMutexLocker locker(&mutex);
    instance->logQueue.enqueue(entry);
}

void Logger::close()
{
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

QString Logger::getLogContent()
{
    if (!instance) return QString();
    
    QMutexLocker locker(&mutex);
    if (!instance->logFile || !instance->logFile->isOpen()) {
        return QString();
    }
    
    // 刷新流
    if (instance->logStream) {
        instance->logStream->flush();
    }
    
    // 读取文件内容
    QString fileName = instance->logFile->fileName();
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString content = in.readAll();
        file.close();
        return content;
    }
    
    return QString();
}

bool Logger::exportLog(const QString &filePath)
{
    if (!instance) return false;
    
    QMutexLocker locker(&mutex);
    if (!instance->logFile || !instance->logFile->isOpen()) {
        return false;
    }
    
    // 刷新流
    if (instance->logStream) {
        instance->logStream->flush();
    }
    
    // 复制文件
    QString sourceFile = instance->logFile->fileName();
    return QFile::copy(sourceFile, filePath);
}

void Logger::setLogLevel(LogLevel level)
{
    if (!instance) return;
    
    QMutexLocker locker(&mutex);
    instance->currentLogLevel = level;
    emit instance->logLevelChanged(level);
}

Logger::LogLevel Logger::getLogLevel()
{
    if (!instance) return Info;
    return instance->currentLogLevel;
}

void Logger::setLogPath(const QString &path)
{
    if (!instance) return;
    
    QMutexLocker locker(&mutex);
    
    // 如果路径相同且日志文件已经打开，不需要重新打开
    if (instance->logPath == path && instance->logFile && instance->logFile->isOpen()) {
        return;
    }
    
    // 保存旧日志文件路径和内容
    QString oldLogFileName;
    QString oldLogContent;
    if (instance->logFile && instance->logFile->isOpen()) {
        oldLogFileName = instance->logFile->fileName();
        
        // 读取旧日志文件内容
        if (instance->logStream) {
            instance->logStream->flush();
        }
        QFile oldFile(oldLogFileName);
        if (oldFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&oldFile);
            oldLogContent = in.readAll();
            oldFile.close();
        }
    }
    
    instance->logPath = path;
    QDir().mkpath(path);
    
    // 重新打开日志文件
    if (instance->logFile) {
        instance->logFile->close();
        delete instance->logFile;
    }
    if (instance->logStream) {
        delete instance->logStream;
    }
    
    QString logFileName = instance->generateLogFileName();
    instance->logFile = new QFile(logFileName, instance);
    if (instance->logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        instance->logStream = new QTextStream(instance->logFile);
        instance->logStream->setEncoding(QStringConverter::Utf8); // 设置UTF-8编码
        
        // 将旧日志内容写入新文件（如果有）
        if (!oldLogContent.isEmpty()) {
            *instance->logStream << oldLogContent;
            instance->logStream->flush();
        } else {
            // 如果是第一次创建日志文件，写入初始化日志
            LogEntry entry(Info, "Logger initialized", __FUNCTION__, __LINE__);
            writeLog(entry);
            qDebug() << "Logger初始化完成，日志文件:" << logFileName;
        }
    } else {
        qDebug() << "日志文件打开失败:" << logFileName << "错误:" << instance->logFile->errorString();
    }
}

QString Logger::getLogPath() const
{
    return logPath;
}

int Logger::getLogCount(LogLevel level) const
{
    if (level >= 0 && level < 5) {
        return logCounts[level];
    }
    return 0;
}

int Logger::getTotalLogCount() const
{
    int total = 0;
    for (int i = 0; i < 5; ++i) {
        total += logCounts[i];
    }
    return total;
}

QDateTime Logger::getFirstLogTime() const
{
    return firstLogTime;
}

QDateTime Logger::getLastLogTime() const
{
    return lastLogTime;
}

void Logger::setLogFilter(LogLevel minLevel)
{
    filterLevel = minLevel;
}

QStringList Logger::getFilteredLogs(LogLevel minLevel) const
{
    QStringList filteredLogs;
    QString content = getLogContent();
    QStringList lines = content.split('\n', Qt::SkipEmptyParts);
    
    for (const QString &line : lines) {
        // 简单的日志级别过滤
        if (line.contains("DEBUG") && minLevel <= Debug) {
            filteredLogs << line;
        } else if (line.contains("INFO") && minLevel <= Info) {
            filteredLogs << line;
        } else if (line.contains("WARNING") && minLevel <= Warning) {
            filteredLogs << line;
        } else if (line.contains("ERROR") && minLevel <= Error) {
            filteredLogs << line;
        } else if (line.contains("CRITICAL") && minLevel <= Critical) {
            filteredLogs << line;
        }
    }
    
    return filteredLogs;
}

void Logger::clearOldLogs(int daysToKeep)
{
    QDir logDir(logPath);
    if (!logDir.exists()) return;
    
    QDateTime cutoffDate = QDateTime::currentDateTime().addDays(-daysToKeep);
    
    QStringList filters;
    filters << "webot_*.log";
    QFileInfoList files = logDir.entryInfoList(filters, QDir::Files);
    
    for (const QFileInfo &fileInfo : files) {
        if (fileInfo.lastModified() < cutoffDate) {
            QFile::remove(fileInfo.absoluteFilePath());
        }
    }
}

void Logger::setMaxLogSize(qint64 maxSize)
{
    maxLogSize = maxSize;
}

void Logger::setMaxLogFiles(int maxFiles)
{
    maxLogFiles = maxFiles;
}

bool Logger::isDuplicateLog(const LogEntry &entry) const
{
    // 检查是否与上一条日志重复
    // 只对DEBUG级别的日志进行去重，其他级别日志保留
    if (entry.level == Debug) {
        return (entry.message == lastLogMessage && entry.level == lastLogLevel);
    }
    return false;
}

void Logger::processLogQueue()
{
    QMutexLocker locker(&mutex);
    
    while (!logQueue.isEmpty()) {
        LogEntry entry = logQueue.dequeue();
        
        if (shouldLog(entry.level) && !isDuplicateLog(entry)) {
            writeLog(entry);
            updateStats(entry);
            
            // 更新最后一条日志信息
            lastLogMessage = entry.message;
            lastLogLevel = entry.level;
        }
    }
}

void Logger::rotateLogFile()
{
    QMutexLocker locker(&mutex);
    checkLogRotation();
}

void Logger::writeLog(const LogEntry &entry)
{
    if (!logFile || !logFile->isOpen() || !logStream) {
        // 如果日志文件无法打开，至少输出到控制台
        QString formattedMessage = formatLogMessage(entry);
        qDebug() << formattedMessage;
        return;
    }
    
    QString formattedMessage = formatLogMessage(entry);
    *logStream << formattedMessage << Qt::endl;
    logStream->flush();
    
    // 同时输出到控制台
    qDebug() << formattedMessage;
    
    // 检查是否需要轮转
    checkLogRotation();
}

QString Logger::formatLogMessage(const LogEntry &entry)
{
    QString timestamp = entry.timestamp.toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString levelStr = getLogLevelString(entry.level);
    QString threadStr = QString("[T%1]").arg(entry.threadId);
    QString locationStr = entry.function.isEmpty() ? "" : QString("[%1:%2]").arg(entry.function).arg(entry.line);
    
    return QString("[%1] %2 %3 %4 %5")
            .arg(timestamp)
            .arg(levelStr)
            .arg(threadStr)
            .arg(locationStr)
            .arg(entry.message);
}

QString Logger::getLogLevelString(LogLevel level)
{
    switch (level) {
    case Debug: return "DEBUG";
    case Info: return "INFO";
    case Warning: return "WARNING";
    case Error: return "ERROR";
    case Critical: return "CRITICAL";
    default: return "UNKNOWN";
    }
}

QString Logger::getLogLevelColor(LogLevel level)
{
    switch (level) {
    case Debug: return "#808080";      // 灰色
    case Info: return "#000000";      // 黑色
    case Warning: return "#FFA500";   // 橙色
    case Error: return "#FF0000";     // 红色
    case Critical: return "#8B0000";  // 深红色
    default: return "#000000";
    }
}

void Logger::updateStats(const LogEntry &entry)
{
    if (entry.level >= 0 && entry.level < 5) {
        logCounts[entry.level]++;
    }
    
    if (firstLogTime.isNull() || entry.timestamp < firstLogTime) {
        firstLogTime = entry.timestamp;
    }
    if (lastLogTime.isNull() || entry.timestamp > lastLogTime) {
        lastLogTime = entry.timestamp;
    }
    
    emit logStatsUpdated();
}

bool Logger::shouldLog(LogLevel level) const
{
    return level >= currentLogLevel && level >= filterLevel;
}

void Logger::checkLogRotation()
{
    if (!logFile || !logFile->isOpen()) return;
    
    // 检查文件大小
    if (logFile->size() > maxLogSize) {
        // 关闭当前文件
        logFile->close();
        delete logFile;
        logFile = nullptr;
        
        if (logStream) {
            delete logStream;
            logStream = nullptr;
        }
        
        // 创建新的日志文件（覆盖旧文件）
        QString newFileName = generateLogFileName();
        logFile = new QFile(newFileName, this);
        if (logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            logStream = new QTextStream(logFile);
            logStream->setEncoding(QStringConverter::Utf8); // 设置UTF-8编码
        }
    }
}

QString Logger::generateLogFileName() const
{
    // 生成只包含日期的日志文件名，确保程序每次启动只创建一个日志文件
    QString date = QDateTime::currentDateTime().toString("yyyyMMdd");
    QString fileName = QString("webot_%1.log").arg(date);
    return QDir(logPath).absoluteFilePath(fileName);
}