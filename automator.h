#ifndef AUTOMATOR_H
#define AUTOMATOR_H

#include <QObject>
#include <QThread>
#include <atomic>
#include "wechatcontroller.h"
#include "imagerecognizer.h"  // 现在使用我们自己的ImageRecognizer
#include "inputsimulator.h"
#include "questionmanager.h"
#include "configmanager.h"
#include "recognitionoverlay.h"

class Automator : public QObject
{
    Q_OBJECT
public:
    // 自动化状态
    enum State {
        Idle,        // 空闲
        Starting,    // 启动中
        Running,     // 运行中
        Completed,   // 完成
        Error        // 错误
    };
    Q_ENUM(State)

    // 问题模式（与QuestionManager一致）
    using QuestionMode = QuestionManager::QuestionMode;

    explicit Automator(QObject *parent = nullptr);
    ~Automator() override;

    // 开始自动化（参数：循环次数）
    bool start(int totalCount);

    // 停止自动化
    void stop();

    // 设置问题模式
    void setQuestionMode(QuestionMode mode) {
        if (m_questionManager) m_questionManager->setQuestionMode(mode);
    }

    // 获取当前状态
    State getCurrentState() const { return m_state; }
    


signals:
    // 日志信号（传递给主窗口）
    void logMessage(const QString &message);

    // 进度更新信号
    void progressUpdated(int currentCount, int totalCount);

    // 状态变化信号
    void stateChanged(State state);

    // 自动化完成信号
    void automationCompleted();
    
    // 错误提示信号，用于显示弹窗
    void errorMessage(const QString &message);

private slots:
    // 自动化核心逻辑（在子线程中运行）
    void runAutomation();

    // 自动化完成处理
    void onFinished();

private:
    // 准备企业微信（启动、激活、置顶）
    bool prepareWeChat();

    // 进入企业微信工作台
    bool enterWeChatWorkbench();

    // 打开"MindSpark"应用
    bool openMindSpark();

    // 进入历史对话界面
    bool enterHistoryDialog();

    // 执行单次问答流程
    bool performQuestionAnswer(const QString& question);

    // 等待回答完成
    bool waitForAnswerCompletion(HWND hwnd);

    // 更新状态（线程安全）
    void setState(State state);
    
    // 非阻塞性延时函数
    void nonBlockingDelay(int delayMs);

private:
    // 子线程（避免阻塞UI）
    QThread m_workerThread;

    // 核心模块实例
    WeChatController *m_weChatController = nullptr;
    ImageRecognizer *m_imageRecognizer = nullptr;  // 使用新的图像识别器
    InputSimulator *m_inputSimulator = nullptr;
    QuestionManager *m_questionManager = nullptr;
    ConfigManager *m_configManager = nullptr;

    // 状态变量（原子类型，线程安全）
    std::atomic<State> m_state = Idle;
    std::atomic<bool> m_stopRequested = false;

    // 计数变量
    int m_currentCount = 0;  // 当前完成次数
    int m_totalCount = 0;    // 总次数
    
    // 日志去重相关
    QString m_lastLogMessage;  // 上次日志内容
    QMap<QString, qint64> m_logMessageHistory;  // 日志历史记录，用于去重
    qint64 m_lastLogTime = 0;  // 上次日志时间
    int m_duplicateLogCount = 0;  // 重复日志计数
    
    // 上次识别到的发送按钮位置
    QPoint m_lastSendButtonPos; // 上次发送按钮的客户区坐标
    bool m_hasLastSendButtonPos = false; // 是否有有效的上次发送按钮位置
    
    // 记录日志（带去重）
    void recordLog(const QString& message);
};

#endif // AUTOMATOR_H
