#include "configmanager.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

// 初始化静态成员
ConfigManager *ConfigManager::instance = nullptr;
QMutex ConfigManager::mutex;

ConfigManager::ConfigManager(QObject *parent) : QObject(parent)
{
    // 构造函数中只做最基本的初始化，不触发任何信号
}

void ConfigManager::initialize()
{
    // 延迟初始化，在信号连接后执行
    initDefaultConfig();
    loadConfig();
    loadQuestionLibrary();
    loadKeywordLibrary();
}

ConfigManager::~ConfigManager()
{
    // 保存配置
    saveConfig();
}

ConfigManager *ConfigManager::getInstance()
{
    // 双重检查锁定模式，确保线程安全
    if (!instance) {
        QMutexLocker locker(&mutex);
        if (!instance) {
            // 在堆上创建实例，避免栈上对象的生命周期问题
            instance = new ConfigManager(nullptr);
            // 设置为应用程序级别的对象，不随线程销毁
            instance->setParent(nullptr);
        }
    }
    return instance;
}

void ConfigManager::initDefaultConfig()
{
    // 企业微信配置
    wechatPath = "";
    windowTopMost = true;

    // 问答设置
    answerTimeout = 30; // 30秒
    delayBetweenRounds = 5; // 5秒
    continueOnError = true;
    continueOnTimeout = true;
    answerLimitPrompt = "回答请勿超过十个字";
    questionMode = 0; // 默认循环使用预设问题
    loopCount = 10; // 默认10次

    // 图像识别配置
    imageRecognitionThreshold = 0.8;
    maxRecognitionAttempts = 3;
    pageLoadTimeout = 2000; // 默认2000毫秒
    recognitionTimeout = 3000; // 默认3000毫秒
    recognitionTechnique = "NCC"; // 默认使用NCC算法

    // 多显示器适配配置
    m_multiMonitorSupport = false;
    m_primaryMonitorIndex = 0;

    // 路径配置，日志路径使用用户指定的D盘路径
    logPath = "D:/OneDrive - linl/文档/WeBot/Logs/";
    questionLibraryPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/WeBot/Questions.txt";
    keywordLibraryPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/WeBot/Keywords.txt";
    
    // 确保日志目录存在
    QDir logDir(logPath);
    if (!logDir.exists()) {
        logDir.mkpath(".");
    }

    // 初始化默认图标路径
    QString templatePath = QCoreApplication::applicationDirPath() + "/templates/";
    iconPaths["input_box"] = templatePath + "输入框.png";
    iconPaths["workbench"] = templatePath + "工作台.png";
    iconPaths["mindspark"] = templatePath + "MindSpark_logo.png";
    iconPaths["history_dialog"] = templatePath + "history_dialog.png";
    
    // 图像识别开关默认值
    m_useImageRecognition = true;
    
    // 鼠标移动检测设置默认值
    m_mouseMovementStopAutomation = true;
    m_mouseInactivityTimeout = 30; // 30秒

    // 新配置项默认值
    mouseClickDelay = 100; // 100毫秒
    keyboardInputDelay = 50; // 50毫秒
    logLevel = 2; // 日志级别：2表示Info
    autoCheckUpdates = true; // 默认启用自动更新检查
    debugMode = false; // 默认关闭调试模式

    // 确保目录存在
    QDir dir;
    dir.mkpath(logPath);
    dir.mkpath(QFileInfo(questionLibraryPath).path());
    dir.mkpath(QFileInfo(keywordLibraryPath).path());
}

bool ConfigManager::loadConfig()
{
    try {
        QString configPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/WeBot/config.ini";
        
        if (!QFile::exists(configPath)) {
            // 配置文件不存在，保存默认配置
            qDebug() << "配置文件不存在，创建默认配置: " << configPath;
            emit logMessage("配置文件不存在，创建默认配置");
            return saveConfig();
        }
        
        // 创建QSettings对象
        QSettings settings(configPath, QSettings::IniFormat);
        if (settings.status() != QSettings::NoError) {
            qDebug() << "无法打开配置文件: " << configPath;
            emit logMessage("无法打开配置文件: " + configPath);
            return false;
        }

        // 读取企业微信配置
        wechatPath = settings.value("WeChat/Path", wechatPath).toString();
        windowTopMost = settings.value("WeChat/WindowTopMost", windowTopMost).toBool();

        // 读取问答设置
        answerTimeout = settings.value("QA/AnswerTimeout", answerTimeout).toInt();
        delayBetweenRounds = settings.value("QA/DelayBetweenRounds", delayBetweenRounds).toInt();
        loopCount = settings.value("QA/LoopCount", loopCount).toInt();
        continueOnError = settings.value("QA/ContinueOnError", continueOnError).toBool();
        continueOnTimeout = settings.value("QA/ContinueOnTimeout", continueOnTimeout).toBool();
        answerLimitPrompt = settings.value("QA/AnswerLimitPrompt", answerLimitPrompt).toString();
        questionMode = settings.value("QA/QuestionMode", questionMode).toInt();

        // 读取图像识别配置
        imageRecognitionThreshold = settings.value("ImageRecognition/Threshold", imageRecognitionThreshold).toDouble();
        maxRecognitionAttempts = settings.value("ImageRecognition/MaxAttempts", maxRecognitionAttempts).toInt();
        pageLoadTimeout = settings.value("ImageRecognition/PageLoadTimeout", pageLoadTimeout).toInt();
        recognitionTimeout = settings.value("ImageRecognition/RecognitionTimeout", recognitionTimeout).toInt();
        recognitionTechnique = settings.value("ImageRecognition/RecognitionTechnique", recognitionTechnique).toString();

        // 读取路径配置
        logPath = settings.value("Paths/LogPath", logPath).toString();
        questionLibraryPath = settings.value("Paths/QuestionLibraryPath", questionLibraryPath).toString();
        keywordLibraryPath = settings.value("Paths/KeywordLibraryPath", keywordLibraryPath).toString();
        
        // 读取图标路径配置
        settings.beginGroup("IconPaths");
        QStringList iconKeys = settings.childKeys();
        foreach (const QString &key, iconKeys) {
            iconPaths[key] = settings.value(key, iconPaths.value(key)).toString();
        }
        settings.endGroup();
        
        // 读取模板尺寸配置
        settings.beginGroup("TemplateSizes");
        QStringList sizeKeys = settings.childKeys();
        foreach (const QString &key, sizeKeys) {
            QString sizeStr = settings.value(key).toString();
            QStringList sizeParts = sizeStr.split(",");
            if (sizeParts.size() == 2) {
                int width = sizeParts[0].toInt();
                int height = sizeParts[1].toInt();
                templateSizes[key] = QSize(width, height);
            }
        }
        settings.endGroup();
        
        // 读取图像识别开关配置
        m_useImageRecognition = settings.value("ImageRecognition/UseImageRecognition", m_useImageRecognition).toBool();
        
        // 读取新配置项
        mouseClickDelay = settings.value("Advanced/MouseClickDelay", mouseClickDelay).toInt();
        keyboardInputDelay = settings.value("Advanced/KeyboardInputDelay", keyboardInputDelay).toInt();
        logLevel = settings.value("Advanced/LogLevel", logLevel).toInt();
        autoCheckUpdates = settings.value("Advanced/AutoCheckUpdates", autoCheckUpdates).toBool();
        debugMode = settings.value("Advanced/DebugMode", debugMode).toBool();
        
        // 读取鼠标移动检测设置
        m_mouseMovementStopAutomation = settings.value("Advanced/MouseMovementStopAutomation", m_mouseMovementStopAutomation).toBool();
        m_mouseInactivityTimeout = settings.value("Advanced/MouseInactivityTimeout", m_mouseInactivityTimeout).toInt();
        
        if (settings.status() != QSettings::NoError) {
            qDebug() << "加载配置时发生错误";
            emit logMessage("加载配置时发生错误");
            return false;
        }
        
        emit logMessage("配置已成功加载");
        return true;
    } catch (const std::exception& e) {
        qDebug() << "加载配置时发生异常: " << e.what();
        emit logMessage(QString("加载配置时发生异常: %1").arg(e.what()));
        return false;
    } catch (...) {
        qDebug() << "加载配置时发生未知异常";
        emit logMessage("加载配置时发生未知异常");
        return false;
    }
}

bool ConfigManager::saveConfig()
{
    try {
        QString configPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/WeBot/config.ini";
        
        // 确保目录存在
        QDir dir;
        if (!dir.exists(QFileInfo(configPath).path())) {
            if (!dir.mkpath(QFileInfo(configPath).path())) {
                qDebug() << "无法创建配置目录: " << QFileInfo(configPath).path();
                emit logMessage("无法创建配置目录: " + QFileInfo(configPath).path());
                return false;
            }
        }
        
        // 创建QSettings对象
        QSettings settings(configPath, QSettings::IniFormat);
        if (settings.status() != QSettings::NoError) {
            qDebug() << "无法创建配置文件: " << configPath;
            emit logMessage("无法创建配置文件: " + configPath);
            return false;
        }
        
        // 写入企业微信配置
        settings.setValue("WeChat/Path", wechatPath);
        settings.setValue("WeChat/WindowTopMost", windowTopMost);

        // 写入问答设置
        settings.setValue("QA/AnswerTimeout", answerTimeout);
        settings.setValue("QA/DelayBetweenRounds", delayBetweenRounds);
        settings.setValue("QA/LoopCount", loopCount);
        settings.setValue("QA/ContinueOnError", continueOnError);
        settings.setValue("QA/ContinueOnTimeout", continueOnTimeout);
        settings.setValue("QA/AnswerLimitPrompt", answerLimitPrompt);
        settings.setValue("QA/QuestionMode", questionMode);

        // 保存图像识别配置
        settings.setValue("ImageRecognition/UseImageRecognition", m_useImageRecognition);
        settings.setValue("ImageRecognition/Threshold", imageRecognitionThreshold);
        settings.setValue("ImageRecognition/MaxAttempts", maxRecognitionAttempts);
        settings.setValue("ImageRecognition/PageLoadTimeout", pageLoadTimeout);
        settings.setValue("ImageRecognition/RecognitionTimeout", recognitionTimeout);
        settings.setValue("ImageRecognition/RecognitionTechnique", recognitionTechnique);

        // 写入路径配置
        settings.setValue("Paths/LogPath", logPath);
        settings.setValue("Paths/QuestionLibraryPath", questionLibraryPath);
        settings.setValue("Paths/KeywordLibraryPath", keywordLibraryPath);
        
        // 写入图标路径配置
        settings.beginGroup("IconPaths");
        foreach (const QString &key, iconPaths.keys()) {
            settings.setValue(key, iconPaths.value(key));
        }
        settings.endGroup();
        
        // 写入模板尺寸配置
        settings.beginGroup("TemplateSizes");
        foreach (const QString &key, templateSizes.keys()) {
            QSize size = templateSizes.value(key);
            QString sizeStr = QString("%1,%2").arg(size.width()).arg(size.height());
            settings.setValue(key, sizeStr);
        }
        settings.endGroup();
        
        // 写入新配置项
        settings.setValue("Advanced/MouseClickDelay", mouseClickDelay);
        settings.setValue("Advanced/KeyboardInputDelay", keyboardInputDelay);
        settings.setValue("Advanced/LogLevel", logLevel);
        settings.setValue("Advanced/AutoCheckUpdates", autoCheckUpdates);
        settings.setValue("Advanced/DebugMode", debugMode);
        
        // 写入鼠标移动检测设置
        settings.setValue("Advanced/MouseMovementStopAutomation", m_mouseMovementStopAutomation);
        settings.setValue("Advanced/MouseInactivityTimeout", m_mouseInactivityTimeout);
        
        // 同步设置到文件
        settings.sync();
        
        if (settings.status() != QSettings::NoError) {
            qDebug() << "保存配置时发生错误";
            emit logMessage("保存配置时发生错误");
            return false;
        }
        
        emit logMessage("配置已成功保存");
        return true;
    } catch (const std::exception& e) {
        qDebug() << "保存配置时发生异常: " << e.what();
        emit logMessage(QString("保存配置时发生异常: %1").arg(e.what()));
        return false;
    } catch (...) {
        qDebug() << "保存配置时发生未知异常";
        emit logMessage("保存配置时发生未知异常");
        return false;
    }
}

bool ConfigManager::loadQuestionLibrary()
{
    questionList.clear();

    // 如果文件不存在，创建默认问题库
    if (!QFile::exists(questionLibraryPath)) {
        createDefaultQuestionLibrary();
    }

    QFile file(questionLibraryPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit logMessage("无法打开问题库文件: " + questionLibraryPath);
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            questionList.append(line);
        }
    }

    file.close();
    emit logMessage(QString("已加载 %1 个预设问题").arg(questionList.size()));
    return true;
}

bool ConfigManager::saveQuestionLibrary()
{
    QFile file(questionLibraryPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit logMessage("无法写入问题库文件: " + questionLibraryPath);
        return false;
    }

    QTextStream out(&file);
    for (const QString& question : questionList) {
        out << question << "\n";
    }

    file.close();
    return true;
}

bool ConfigManager::loadKeywordLibrary()
{
    keywordList.clear();

    // 如果文件不存在，创建默认关键词库
    if (!QFile::exists(keywordLibraryPath)) {
        createDefaultKeywordLibrary();
    }

    QFile file(keywordLibraryPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit logMessage("无法打开关键词库文件: " + keywordLibraryPath);
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            keywordList.append(line);
        }
    }

    file.close();
    emit logMessage(QString("已加载 %1 个关键词").arg(keywordList.size()));
    return true;
}

bool ConfigManager::saveKeywordLibrary()
{
    QFile file(keywordLibraryPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit logMessage("无法写入关键词库文件: " + keywordLibraryPath);
        return false;
    }

    QTextStream out(&file);
    for (const QString& keyword : keywordList) {
        out << keyword << "\n";
    }

    file.close();
    return true;
}

void ConfigManager::createDefaultQuestionLibrary()
{
    // 创建默认问题库
    QFile file(questionLibraryPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "什么是嵌入式系统？\n";
        out << "嵌入式系统有哪些应用领域？\n";
        out << "MCU和CPU有什么区别？\n";
        out << "嵌入式开发常用的语言有哪些？\n";
        out << "什么是RTOS？\n";
        out << "嵌入式系统有哪些主要的特点？\n";
        out << "什么是中断服务程序？\n";
        out << "嵌入式系统的开发流程是什么？\n";
        out << "什么是GPIO？\n";
        out << "I2C和SPI有什么区别？\n";
        file.close();
    }
}

void ConfigManager::createDefaultKeywordLibrary()
{
    // 创建默认关键词库
    QFile file(keywordLibraryPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "嵌入式系统\n";
        out << "MCU\n";
        out << "CPU\n";
        out << "ARM\n";
        out << "RTOS\n";
        out << "Linux\n";
        out << "微控制器\n";
        out << "微处理器\n";
        out << "GPIO\n";
        out << "UART\n";
        out << "I2C\n";
        out << "SPI\n";
        out << "ADC\n";
        out << "PWM\n";
        out << "中断\n";
        out << "定时器\n";
        out << "存储器\n";
        out << "调试\n";
        out << "Bootloader\n";
        out << "固件\n";
        file.close();
    }
}

bool ConfigManager::importQuestionsFromCSV(const QString &csvPath) {
    try {
        QFile file(csvPath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            emit logMessage("无法打开CSV文件: " + csvPath);
            return false;
        }

        QTextStream in(&file);
        QStringList newQuestions;

        // 读取CSV文件
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) {
                continue;
            }

            // 简单的CSV解析，支持逗号分隔
            QStringList fields = line.split(",");
            if (!fields.isEmpty()) {
                // 取第一个字段作为问题
                QString question = fields.first().trimmed();
                if (!question.isEmpty()) {
                    newQuestions.append(question);
                }
            }
        }

        file.close();

        if (newQuestions.isEmpty()) {
            emit logMessage("CSV文件中没有找到有效的问题");
            return false;
        }

        // 将新问题添加到问题列表
        questionList.append(newQuestions);
        emit logMessage(QString("从CSV文件导入了 %1 个问题").arg(newQuestions.size()));

        // 保存更新后的问题库
        if (!saveQuestionLibrary()) {
            emit logMessage("保存问题库失败");
            return false;
        }

        return true;
    } catch (const std::exception& e) {
        emit logMessage(QString("导入CSV文件时发生异常: %1").arg(e.what()));
        return false;
    } catch (...) {
        emit logMessage("导入CSV文件时发生未知异常");
        return false;
    }
}

QStringList ConfigManager::previewCSVFile(const QString &csvPath, int maxLines) {
    QStringList previewLines;
    QString encoding = "";
    int totalLines = 0;

    try {
        QFile file(csvPath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            previewLines.append("无法打开CSV文件: " + csvPath);
            return previewLines;
        }

        // 先统计总行数
        QTextStream in_total(&file);
        while (!in_total.atEnd()) {
            in_total.readLine();
            totalLines++;
        }

        // 尝试使用UTF-8读取文件
        file.reset(); // 重置文件指针到开头
        QTextStream in(&file);
        in.setEncoding(QStringConverter::Utf8);
        
        int lineCount = 0;
        previewLines.clear();
        
        // 读取CSV文件前maxLines行
        while (!in.atEnd() && lineCount < maxLines) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                // 检查是否有乱码字符
                if (!line.contains("\uFFFD")) {
                    previewLines.append(line);
                    lineCount++;
                } else {
                    // 如果有乱码，尝试使用GBK编码重新读取
                    previewLines.clear();
                    break;
                }
            }
        }
        
        if (!previewLines.isEmpty()) {
            encoding = "UTF-8";
        } else {
            // 如果UTF-8读取失败，尝试使用GBK编码
            file.reset(); // 重置文件指针到开头
            
            // 使用二进制方式读取文件内容
            QByteArray fileContent = file.readAll();
            
            // 尝试使用GBK编码
            QString decodedContent = QString::fromLocal8Bit(fileContent);
            
            // 按行分割内容
            QStringList lines = decodedContent.split("\n");
            
            lineCount = 0;
            previewLines.clear();
            
            // 只取前maxLines行
            for (const QString& line : lines) {
                if (lineCount >= maxLines) {
                    break;
                }
                QString trimmedLine = line.trimmed();
                if (!trimmedLine.isEmpty()) {
                    // 检查是否有乱码字符
                    if (!trimmedLine.contains("\uFFFD")) {
                        previewLines.append(trimmedLine);
                        lineCount++;
                    }
                }
            }
            
            if (!previewLines.isEmpty()) {
                encoding = "GBK";
            }
        }
        
        // 插入文件信息行
        if (!previewLines.isEmpty()) {
            QString infoLine = QString("[文件信息] 编码: %1, 总行数: %2, 预览行数: %3").arg(encoding).arg(totalLines).arg(previewLines.size());
            previewLines.insert(0, infoLine);
            previewLines.insert(1, "[分隔符] 使用逗号(,)分隔字段，第一行为标题行");
            previewLines.insert(2, "-------------------------------");
        }
        
        // 如果还是读取失败，显示错误信息
        if (previewLines.isEmpty()) {
            previewLines.append("无法正确读取CSV文件，可能是编码问题");
            previewLines.append("请确保文件为UTF-8或GBK编码的CSV格式");
        }

        file.close();

        if (previewLines.isEmpty()) {
            previewLines.append("CSV文件为空");
        }
    } catch (const std::exception& e) {
        previewLines.append(QString("预览CSV文件时发生异常: %1").arg(e.what()));
    } catch (...) {
        previewLines.append("预览CSV文件时发生未知异常");
    }

    return previewLines;
}

// 各种getter和setter方法
QString ConfigManager::getWeChatPath() const { return wechatPath; }
void ConfigManager::setWeChatPath(const QString &path) { 
    wechatPath = path; 
    emit configChanged(); 
}

bool ConfigManager::getWindowTopMost() const { return windowTopMost; }
void ConfigManager::setWindowTopMost(bool topMost) { 
    windowTopMost = topMost; 
    emit configChanged(); 
}

int ConfigManager::getAnswerTimeout() const { return answerTimeout; }
void ConfigManager::setAnswerTimeout(int timeout) { 
    answerTimeout = timeout; 
    emit configChanged(); 
}

int ConfigManager::getDelayBetweenRounds() const { return delayBetweenRounds; }
void ConfigManager::setDelayBetweenRounds(int delay) { 
    delayBetweenRounds = delay; 
    emit configChanged(); 
}

bool ConfigManager::getContinueOnError() const { return continueOnError; }
void ConfigManager::setContinueOnError(bool continueFlag) { 
    continueOnError = continueFlag; 
    emit configChanged(); 
}

bool ConfigManager::getContinueOnTimeout() const { return continueOnTimeout; }
void ConfigManager::setContinueOnTimeout(bool continueFlag) { 
    continueOnTimeout = continueFlag; 
    emit configChanged(); 
}

QString ConfigManager::getAnswerLimitPrompt() const { return answerLimitPrompt; }
void ConfigManager::setAnswerLimitPrompt(const QString &prompt) { 
    answerLimitPrompt = prompt; 
    emit configChanged(); 
}

double ConfigManager::getImageRecognitionThreshold() const { return imageRecognitionThreshold; }
void ConfigManager::setImageRecognitionThreshold(double threshold) { 
    imageRecognitionThreshold = threshold; 
    emit configChanged(); 
}

int ConfigManager::getMaxRecognitionAttempts() const { return maxRecognitionAttempts; }
void ConfigManager::setMaxRecognitionAttempts(int attempts) { 
    maxRecognitionAttempts = attempts; 
    emit configChanged(); 
}

int ConfigManager::getPageLoadTimeout() const
{
    return pageLoadTimeout;
}

void ConfigManager::setPageLoadTimeout(int timeout)
{
    pageLoadTimeout = timeout;
    emit configChanged();
}

// 识别超时时间的getter和setter方法
int ConfigManager::getRecognitionTimeout() const
{
    return recognitionTimeout;
}

void ConfigManager::setRecognitionTimeout(int timeout)
{
    recognitionTimeout = timeout;
    emit configChanged();
}

// 识别技术的getter和setter方法
QString ConfigManager::getRecognitionTechnique() const
{
    return recognitionTechnique;
}

void ConfigManager::setRecognitionTechnique(const QString &technique)
{
    recognitionTechnique = technique;
    emit configChanged();
}

QString ConfigManager::getLogPath() const { return logPath; }
void ConfigManager::setLogPath(const QString &path) { 
    logPath = path; 
    emit configChanged(); 
}

QString ConfigManager::getQuestionLibraryPath() const { return questionLibraryPath; }
void ConfigManager::setQuestionLibraryPath(const QString &path) { 
    questionLibraryPath = path; 
    emit configChanged(); 
}

QString ConfigManager::getKeywordLibraryPath() const { return keywordLibraryPath; }
void ConfigManager::setKeywordLibraryPath(const QString &path) { 
    keywordLibraryPath = path; 
    emit configChanged(); 
}

QStringList ConfigManager::getQuestionList() const { return questionList; }
void ConfigManager::setQuestionList(const QStringList &list) { 
    questionList = list; 
    emit configChanged(); 
}

QStringList ConfigManager::getKeywordList() const { return keywordList; }
void ConfigManager::setKeywordList(const QStringList &list) { 
    keywordList = list; 
    emit configChanged(); 
}

bool ConfigManager::getMultiMonitorSupport() const { return m_multiMonitorSupport; }
void ConfigManager::setMultiMonitorSupport(bool support) { 
    m_multiMonitorSupport = support; 
    emit configChanged();
}

int ConfigManager::getPrimaryMonitorIndex() const { return m_primaryMonitorIndex; }
void ConfigManager::setPrimaryMonitorIndex(int index) { 
    m_primaryMonitorIndex = index; 
    emit configChanged();
}

// 鼠标移动检测设置
bool ConfigManager::getMouseMovementStopAutomation() const { return m_mouseMovementStopAutomation; }
void ConfigManager::setMouseMovementStopAutomation(bool enabled) {
    m_mouseMovementStopAutomation = enabled;
    emit configChanged();
}

int ConfigManager::getMouseInactivityTimeout() const { return m_mouseInactivityTimeout; }
void ConfigManager::setMouseInactivityTimeout(int timeout) {
    m_mouseInactivityTimeout = timeout;
    emit configChanged();
}

// 图标路径管理方法实现
QString ConfigManager::getIconPath(const QString &iconName) const {
    if (iconPaths.contains(iconName)) {
        return iconPaths.value(iconName);
    }
    // 返回默认图标路径
    return QString(":/templates/%1.svg").arg(iconName);
}

void ConfigManager::setIconPath(const QString &iconName, const QString &path) {
    iconPaths[iconName] = path;
    emit configChanged();
}

QMap<QString, QString> ConfigManager::getAllIconPaths() const {
    return iconPaths;
}

void ConfigManager::setAllIconPaths(const QMap<QString, QString> &paths) {
    iconPaths = paths;
}

// 模板尺寸管理方法实现
QSize ConfigManager::getTemplateSize(const QString &templateName) const {
    if (templateSizes.contains(templateName)) {
        return templateSizes.value(templateName);
    }
    // 返回默认尺寸
    return QSize(100, 100);
}

void ConfigManager::setTemplateSize(const QString &templateName, const QSize &size) {
    templateSizes[templateName] = size;
    emit configChanged();
}

QMap<QString, QSize> ConfigManager::getAllTemplateSizes() const {
    return templateSizes;
}

void ConfigManager::setAllTemplateSizes(const QMap<QString, QSize> &sizes) {
    templateSizes = sizes;
}

// 图像识别开关 getter 和 setter 方法
bool ConfigManager::getUseImageRecognition() const {
    return m_useImageRecognition;
}

void ConfigManager::setUseImageRecognition(bool useImage) {
    m_useImageRecognition = useImage;
    emit configChanged();
}



// 问题模式的getter和setter方法
int ConfigManager::getQuestionMode() const {
    return questionMode;
}

void ConfigManager::setQuestionMode(int mode) {
    questionMode = mode;
    emit configChanged();
}

// 循环次数的getter和setter方法
int ConfigManager::getLoopCount() const {
    return loopCount;
}

void ConfigManager::setLoopCount(int count) {
    loopCount = count;
    emit configChanged();
}

// 鼠标点击延迟的getter和setter方法
int ConfigManager::getMouseClickDelay() const {
    return mouseClickDelay;
}

void ConfigManager::setMouseClickDelay(int delay) {
    mouseClickDelay = delay;
    emit configChanged();
}

// 键盘输入延迟的getter和setter方法
int ConfigManager::getKeyboardInputDelay() const {
    return keyboardInputDelay;
}

void ConfigManager::setKeyboardInputDelay(int delay) {
    keyboardInputDelay = delay;
    emit configChanged();
}

// 日志级别的getter和setter方法
int ConfigManager::getLogLevel() const {
    return logLevel;
}

void ConfigManager::setLogLevel(int level) {
    logLevel = level;
    emit configChanged();
}

// 自动更新检查的getter和setter方法
bool ConfigManager::getAutoCheckUpdates() const {
    return autoCheckUpdates;
}

void ConfigManager::setAutoCheckUpdates(bool autoCheck) {
    autoCheckUpdates = autoCheck;
    emit configChanged();
}

// 调试模式的getter和setter方法
bool ConfigManager::getDebugMode() const {
    return debugMode;
}

void ConfigManager::setDebugMode(bool debugMode) {
    this->debugMode = debugMode;
    emit configChanged();
}