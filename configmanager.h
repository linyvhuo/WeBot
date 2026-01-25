#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QMutex>
#include <QPoint>
#include <QSize>
#include <QMap>

class ConfigManager : public QObject {
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = nullptr);
    ~ConfigManager();

    static ConfigManager *getInstance();

    // 延迟初始化方法，用于在信号连接后执行初始化
    void initialize();

    // 加载配置
    bool loadConfig();

    // 保存配置
    bool saveConfig();

    // 加载问题库
    bool loadQuestionLibrary();

    // 保存问题库
    bool saveQuestionLibrary();

    // 加载关键词库
    bool loadKeywordLibrary();

    // 保存关键词库
    bool saveKeywordLibrary();

    // 创建默认问题库
    void createDefaultQuestionLibrary();

    // 创建默认关键词库
    void createDefaultKeywordLibrary();
    
    // 从CSV文件导入问题
    bool importQuestionsFromCSV(const QString &csvPath);
    
    // 预览CSV文件内容
    QStringList previewCSVFile(const QString &csvPath, int maxLines = 20);

    // 获取企业微信路径
    QString getWeChatPath() const;

    // 设置企业微信路径
    void setWeChatPath(const QString &path);

    // 获取窗口置顶状态
    bool getWindowTopMost() const;

    // 设置窗口置顶状态
    void setWindowTopMost(bool topMost);

    // 获取问答超时时间
    int getAnswerTimeout() const;

    // 设置问答超时时间
    void setAnswerTimeout(int timeout);

    // 获取轮次间隔时间
    int getDelayBetweenRounds() const;

    // 设置轮次间隔时间
    void setDelayBetweenRounds(int delay);

    // 获取出错时继续状态
    bool getContinueOnError() const;

    // 设置出错时继续状态
    void setContinueOnError(bool continueFlag);

    // 获取超时时继续状态
    bool getContinueOnTimeout() const;

    // 设置超时时继续状态
    void setContinueOnTimeout(bool continueFlag);

    // 获取回答限制提示
    QString getAnswerLimitPrompt() const;
    
    // 设置回答限制提示
    void setAnswerLimitPrompt(const QString &prompt);
    
    // 获取问题模式
    int getQuestionMode() const;
    
    // 设置问题模式
    void setQuestionMode(int mode);
    
    // 获取循环次数
    int getLoopCount() const;
    
    // 设置循环次数
    void setLoopCount(int count);
    
    // 获取图像识别阈值
    double getImageRecognitionThreshold() const;

    // 设置图像识别阈值
    void setImageRecognitionThreshold(double threshold);

    // 获取最大识别尝试次数
    int getMaxRecognitionAttempts() const;

    // 设置最大识别尝试次数
    void setMaxRecognitionAttempts(int attempts);

    // 获取页面加载超时时间
    int getPageLoadTimeout() const;

    // 设置页面加载超时时间
    void setPageLoadTimeout(int timeout);

    // 获取识别超时时间
    int getRecognitionTimeout() const;

    // 设置识别超时时间
    void setRecognitionTimeout(int timeout);

    // 获取识别技术
    QString getRecognitionTechnique() const;

    // 设置识别技术
    void setRecognitionTechnique(const QString &technique);

    // 获取日志路径
    QString getLogPath() const;

    // 设置日志路径
    void setLogPath(const QString &path);
    
    // 获取鼠标点击延迟（毫秒）
    int getMouseClickDelay() const;
    
    // 设置鼠标点击延迟（毫秒）
    void setMouseClickDelay(int delay);
    
    // 获取键盘输入延迟（毫秒）
    int getKeyboardInputDelay() const;
    
    // 设置键盘输入延迟（毫秒）
    void setKeyboardInputDelay(int delay);

    // 获取截图快捷键
    QString getScreenshotShortcut() const;

    // 设置截图快捷键
    void setScreenshotShortcut(const QString &shortcut);
    
    // 获取日志级别
    int getLogLevel() const;
    
    // 设置日志级别
    void setLogLevel(int level);
    
    // 获取自动更新检查状态
    bool getAutoCheckUpdates() const;
    
    // 设置自动更新检查状态
    void setAutoCheckUpdates(bool autoCheck);
    
    // 获取调试模式状态
    bool getDebugMode() const;
    
    // 设置调试模式状态
    void setDebugMode(bool debugMode);

    // 获取问题库路径
    QString getQuestionLibraryPath() const;

    // 设置问题库路径
    void setQuestionLibraryPath(const QString &path);

    // 获取关键词库路径
    QString getKeywordLibraryPath() const;

    // 设置关键词库路径
    void setKeywordLibraryPath(const QString &path);

    // 获取问题列表
    QStringList getQuestionList() const;

    // 设置问题列表
    void setQuestionList(const QStringList &list);

    // 获取关键词列表
    QStringList getKeywordList() const;

    // 设置关键词列表
    void setKeywordList(const QStringList &list);

    // 获取多显示器支持状态
    bool getMultiMonitorSupport() const;

    // 设置多显示器支持状态
    void setMultiMonitorSupport(bool support);

    // 获取主显示器索引
    int getPrimaryMonitorIndex() const;

    // 设置主显示器索引
    void setPrimaryMonitorIndex(int index);
    
    // 图标路径管理
    QString getIconPath(const QString &iconName) const;
    void setIconPath(const QString &iconName, const QString &path);
    QMap<QString, QString> getAllIconPaths() const;
    void setAllIconPaths(const QMap<QString, QString> &paths);
    
    // 文件路径管理
    QString getConfigFilePath() const;
    void setConfigFilePath(const QString &path);
    QString getQuestionFilePath() const;
    void setQuestionFilePath(const QString &path);
    QString getKeywordFilePath() const;
    void setKeywordFilePath(const QString &path);
    QString getLogFilePath() const;
    void setLogFilePath(const QString &path);
    QString getTemplateFilePath() const;
    void setTemplateFilePath(const QString &path);
    
    // 模板尺寸管理
    QSize getTemplateSize(const QString &templateName) const;
    void setTemplateSize(const QString &templateName, const QSize &size);
    QMap<QString, QSize> getAllTemplateSizes() const;
    void setAllTemplateSizes(const QMap<QString, QSize> &sizes);
    
    // 图像识别开关
    bool getUseImageRecognition() const;
    void setUseImageRecognition(bool useImage);
    
    // 鼠标移动检测设置
    bool getMouseMovementStopAutomation() const;
    void setMouseMovementStopAutomation(bool enabled);
    int getMouseInactivityTimeout() const;
    void setMouseInactivityTimeout(int timeout);
    
    // 输入方式设置
    int getInputMethod() const;
    void setInputMethod(int method);

    // 主题设置
    int getTheme() const;
    void setTheme(int theme);

    // 悬浮窗设置
    int getFloatingWindowX() const;
    void setFloatingWindowX(int x);
    int getFloatingWindowY() const;
    void setFloatingWindowY(int y);
    int getFloatingWindowWidth() const;
    void setFloatingWindowWidth(int width);
    int getFloatingWindowHeight() const;
    void setFloatingWindowHeight(int height);
    bool getFloatingWindowVisible() const;
    void setFloatingWindowVisible(bool visible);

    // 关闭行为设置
    enum CloseBehavior {
        MinimizeToTray = 0,  // 最小化到系统托盘
        CloseDirectly = 1     // 直接关闭
    };
    int getCloseBehavior() const;
    void setCloseBehavior(int behavior);

signals:
    void logMessage(const QString &message);
    void configChanged();

private:
    void initDefaultConfig();

    // 企业微信路径
    QString wechatPath;

    // 窗口置顶
    bool windowTopMost;

    // 问答超时时间（秒）
    int answerTimeout;

    // 轮次间隔时间（秒）
    int delayBetweenRounds;

    // 出错时继续
    bool continueOnError;

    // 超时时继续
    bool continueOnTimeout;
    
    // 回答限制提示
    QString answerLimitPrompt;
    
    // 问题模式
    int questionMode;
    
    // 循环次数
    int loopCount;

    // 图像识别阈值
    double imageRecognitionThreshold;

    // 最大识别尝试次数
    int maxRecognitionAttempts;

    // 页面加载超时时间（毫秒）
    int pageLoadTimeout;

    // 识别超时时间（毫秒）
    int recognitionTimeout;

    // 识别技术
    QString recognitionTechnique;

    // 日志路径
    QString logPath;
    
    // 鼠标点击延迟（毫秒）
    int mouseClickDelay;
    
    // 键盘输入延迟（毫秒）
    int keyboardInputDelay;
    
    // 日志级别
    int logLevel;
    
    // 自动更新检查
    bool autoCheckUpdates;
    
    // 调试模式
    bool debugMode;
    
    // 问题库路径
    QString questionLibraryPath;
    
    // 关键词库路径
    QString keywordLibraryPath;
    
    // 文件路径管理
    QString configFilePath;
    QString questionFilePath;
    QString keywordFilePath;
    QString logFilePath;
    QString templateFilePath;

    // 问题列表
    QStringList questionList;

    // 关键词列表
    QStringList keywordList;

    // 添加缺失的成员变量声明
    bool m_multiMonitorSupport;
    int m_primaryMonitorIndex;

    // 图标路径配置
    QMap<QString, QString> iconPaths;
    
    // 模板尺寸配置
    QMap<QString, QSize> templateSizes;
    
    // 图像识别开关
    bool m_useImageRecognition;
    
    // 鼠标移动检测设置
    bool m_mouseMovementStopAutomation;
    int m_mouseInactivityTimeout;
    
    // 输入方式设置
    int inputMethod;

    // 主题设置
    int theme;

    // 悬浮窗设置
    int floatingWindowX;
    int floatingWindowY;
    int floatingWindowWidth;
    int floatingWindowHeight;
    bool floatingWindowVisible;

    // 关闭行为设置
    int closeBehavior;

    // 截图快捷键设置
    QString m_screenshotShortcut;

    QSettings *settings;

    // 默认配置
    void setDefaultValues();

private:
    // 单例实例
    static ConfigManager *instance;
    // 线程安全锁
    static QMutex mutex;
};

