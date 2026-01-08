#include "automator.h"
#include <QTimer>
#include <QThread>
#include <QMessageBox>
#include <QEventLoop>
#include <QMetaType>
#include <QDateTime>
#include <QCoreApplication>
#include <QRandomGenerator>
#include <windows.h>

Automator::Automator(QObject *parent)
    : QObject(parent)
{
    // 初始化核心模块实例 - 不设置父对象，因为我们需要移动到工作线程
    m_weChatController = new WeChatController();
    m_imageRecognizer = new ImageRecognizer();
    m_inputSimulator = new InputSimulator();
    m_questionManager = new QuestionManager();
    m_configManager = nullptr;  // 延迟初始化，避免在构造函数中访问未初始化的ConfigManager

    // 连接日志信号，使用recordLog函数进行去重
    connect(m_weChatController, &WeChatController::logMessage,
            this, &Automator::recordLog);
    connect(m_imageRecognizer, &ImageRecognizer::logMessage,
            this, &Automator::recordLog);
    connect(m_inputSimulator, &InputSimulator::logMessage,
            this, &Automator::recordLog);
    connect(m_questionManager, &QuestionManager::logMessage,
            this, &Automator::recordLog);
    
    // 删除识别区域信号连接，识别标识框功能已删除
    
    // 连接模板未找到信号，不清除识别区域，让用户可以看到最后一次识别结果
    connect(m_imageRecognizer, &ImageRecognizer::templateNotFound,
            this, [this](const QString &/*templateName*/) {
                // 不清除识别区域，让用户可以看到最后一次识别结果
                recordLog("[INFO] 模板未找到，保持显示最后一次识别区域");
            });

    // 不在构造函数中移动对象到工作线程，避免线程安全问题
    // 将在start()函数中移动对象并启动工作线程
}

Automator::~Automator()
{
    // 直接清理资源，避免调用可能引发竞态条件的函数
    m_stopRequested = true;
    
    // 安全检查：确保对象存在才调用其方法
    if (m_imageRecognizer) {
        m_imageRecognizer->setStopRequested(true);
    }
    
    if (m_inputSimulator) {
        m_inputSimulator->setStopRequested(true);
    }
    
    m_workerThread.quit();
    m_workerThread.wait();

    // 按顺序释放资源，确保依赖关系正确
    delete m_weChatController;
    delete m_imageRecognizer;
    delete m_inputSimulator;
    delete m_questionManager;
    // 不要删除ConfigManager，它是单例，由全局管理
}

bool Automator::start(int totalCount)
{
    // 允许从Idle或Error状态启动，进入Error状态后可以重新启动
    if (m_state != Idle && m_state != Error) {
        recordLog("自动化已在运行中");
        return false;
    }
    
    // 如果是从Error状态启动，先重置状态
    if (m_state == Error) {
        recordLog("[DEBUG] 从错误状态重置，准备重新启动");
        // 重置所有必要的状态
        m_stopRequested = false;
        m_imageRecognizer->resetState();
    }

    m_totalCount = totalCount;
    m_currentCount = 0;
    m_stopRequested = false;
    // 重置ImageRecognizer的停止请求标志
    m_imageRecognizer->setStopRequested(false);

    setState(Starting);
    
    // 初始化进度条，显示正确的初始状态
    emit progressUpdated(0, m_totalCount);

    // 确保ConfigManager已初始化
    if (!m_configManager) {
        m_configManager = ConfigManager::getInstance();
    }
    
    // 重新获取最新配置并应用到ImageRecognizer
    m_imageRecognizer->setRecognitionThreshold(m_configManager->getImageRecognitionThreshold());
    
    // 初始化问题
    m_questionManager->setPresetQuestions(m_configManager->getQuestionList());
    m_questionManager->setKeywords(m_configManager->getKeywordList());
    
    // 移动所有子对象到工作线程（仅在第一次启动时执行）
    static bool objectsMoved = false;
    if (!objectsMoved) {
        m_weChatController->moveToThread(&m_workerThread);
        m_imageRecognizer->moveToThread(&m_workerThread);
        m_inputSimulator->moveToThread(&m_workerThread);
        m_questionManager->moveToThread(&m_workerThread);
        objectsMoved = true;
    }
    
    // 连接线程结束信号（仅在第一次启动时执行）
    static bool threadConnected = false;
    if (!threadConnected) {
        connect(&m_workerThread, &QThread::finished, this, &Automator::deleteLater);
        threadConnected = true;
    }

    // 启动工作线程
    if (!m_workerThread.isRunning()) {
        m_workerThread.start();
    }

    // 开始自动化流程 - use QMetaObject::invokeMethod for thread-safe call
    QMetaObject::invokeMethod(this, "runAutomation", Qt::QueuedConnection);
    return true;
}

void Automator::stop() {
    // 不管当前状态如何，都允许停止操作
    recordLog("正在停止自动化...");
    
    try {
        // 立即设置停止标志
        m_stopRequested = true;
        
        // 安全检查：确保ImageRecognizer和InputSimulator对象存在
        if (m_imageRecognizer) {
            // 通知ImageRecognizer停止
            m_imageRecognizer->setStopRequested(true);
            
            // 重置图像识别器状态
            m_imageRecognizer->resetState();
        }
        
        if (m_inputSimulator) {
            // 通知InputSimulator停止
            m_inputSimulator->setStopRequested(true);
        }
        
        // 设置为Idle状态，不调用onFinished()，让runAutomation()自己处理完成逻辑
        setState(Idle);
        
        recordLog("自动化已成功停止");
        
    } catch (const std::exception& e) {
        recordLog(QString("停止自动化时发生异常: %1").arg(e.what()));
        // 确保状态正确设置
        setState(Idle);
    } catch (...) {
        recordLog("停止自动化时发生未知异常");
        // 确保状态正确设置
        setState(Idle);
    }
}

void Automator::runAutomation()
{
    recordLog("[DEBUG] 开始执行自动化流程");
    
    try {
        // 确保ConfigManager已初始化
        if (!m_configManager) {
            m_configManager = ConfigManager::getInstance();
        }
        
        // 设置图像识别阈值
        m_imageRecognizer->setRecognitionThreshold(
            m_configManager->getImageRecognitionThreshold()
            );
        
        // 确保状态重置正确
        setState(Running);
        recordLog("[DEBUG] 状态已设置为Running");

        // 检查是否请求停止
        if (m_stopRequested) {
            recordLog("[INFO] 收到停止请求，退出runAutomation");
            onFinished();
            return;
        }

    // 加载图像模板 - 移到工作线程中执行
    recordLog("[DEBUG] 开始加载图像模板");
    bool templatesLoaded = true;
    
    // 检查是否请求停止
    if (m_stopRequested) {
        recordLog("[INFO] 收到停止请求，退出runAutomation");
        onFinished();
        return;
    }
    
    recordLog("[DEBUG] 开始加载workbench模板");
    templatesLoaded &= m_imageRecognizer->loadTemplate("workbench", m_configManager->getIconPath("workbench"));
    recordLog(QString("[DEBUG] workbench模板加载完成，结果: %1").arg(templatesLoaded ? "成功" : "失败"));
    
    // 检查是否请求停止
    if (m_stopRequested) {
        recordLog("[INFO] 收到停止请求，退出runAutomation");
        onFinished();
        return;
    }
    
    recordLog("[DEBUG] 开始加载mindspark模板");
    templatesLoaded &= m_imageRecognizer->loadTemplate("mindspark", m_configManager->getIconPath("mindspark"));
    recordLog(QString("[DEBUG] mindspark模板加载完成，结果: %1").arg(templatesLoaded ? "成功" : "失败"));
    
    // 检查是否请求停止
    if (m_stopRequested) {
        recordLog("[INFO] 收到停止请求，退出runAutomation");
        onFinished();
        return;
    }
    
    recordLog("[DEBUG] 开始加载input_box模板");
    templatesLoaded &= m_imageRecognizer->loadTemplate("input_box", m_configManager->getIconPath("input_box"));
    recordLog(QString("[DEBUG] input_box模板加载完成，结果: %1").arg(templatesLoaded ? "成功" : "失败"));
    
    // 检查是否请求停止
    if (m_stopRequested) {
        recordLog("[INFO] 收到停止请求，退出runAutomation");
        onFinished();
        return;
    }
    
    recordLog("[DEBUG] 开始加载send_button模板");
    templatesLoaded &= m_imageRecognizer->loadTemplate("send_button", m_configManager->getIconPath("send_button"));
    recordLog(QString("[DEBUG] send_button模板加载完成，结果: %1").arg(templatesLoaded ? "成功" : "失败"));
    
    // 检查是否请求停止
    if (m_stopRequested) {
        recordLog("[INFO] 收到停止请求，退出runAutomation");
        onFinished();
        return;
    }
    
    recordLog("[DEBUG] 开始加载历史对话模板");
    // 历史对话模板是可选的，如果加载失败不影响整体流程
    bool historyLoaded = m_imageRecognizer->loadTemplate("history_dialog", m_configManager->getIconPath("history_dialog"));
    // 尝试加载其他尺寸的历史对话模板
    m_imageRecognizer->loadTemplate("history_dialog_small", m_configManager->getIconPath("history_dialog_small"));
    m_imageRecognizer->loadTemplate("history_dialog_large", m_configManager->getIconPath("history_dialog_large"));
    recordLog(QString("[DEBUG] 历史对话模板加载完成，主要模板结果: %1").arg(historyLoaded ? "成功" : "失败"));

    // 检查是否请求停止
    if (m_stopRequested) {
        recordLog("[INFO] 收到停止请求，退出runAutomation");
        onFinished();
        return;
    }

    if (!templatesLoaded) {
        QString errorMsg = "图像模板加载失败，无法启动自动化";
        recordLog("[ERROR] " + errorMsg);
        emit errorMessage(errorMsg);
        setState(Error);
        recordLog("[DEBUG] 状态已设置为Error");
        emit automationCompleted();
        recordLog("[DEBUG] 已发送automationCompleted信号");
        return;
    }
    recordLog("[DEBUG] 所有图像模板加载完成");

    // 检查是否请求停止
    if (m_stopRequested) {
        recordLog("[INFO] 收到停止请求，退出runAutomation");
        onFinished();
        return;
    }
    
    // 准备企业微信
    recordLog("[DEBUG] 开始准备企业微信");
    if (!prepareWeChat()) {
        // 检查是否是因为停止请求导致的失败
        if (m_stopRequested) {
            recordLog("[INFO] 准备企业微信时收到停止请求");
            onFinished();
            return;
        }
        
        QString errorMsg = "准备企业微信失败";
        recordLog("[ERROR] " + errorMsg);
        emit errorMessage(errorMsg);
        setState(Error);
        recordLog("[DEBUG] 状态已设置为Error");
        emit automationCompleted();
        recordLog("[DEBUG] 已发送automationCompleted信号");
        return;
    }
    recordLog("[DEBUG] 企业微信准备完成");

    // 检查是否请求停止
    if (m_stopRequested) {
        recordLog("[INFO] 收到停止请求，退出runAutomation");
        onFinished();
        return;
    }
    
    // 进入工作台
    recordLog("[DEBUG] 开始进入企业微信工作台");
    if (!enterWeChatWorkbench()) {
        // 检查是否是因为停止请求导致的失败
        if (m_stopRequested) {
            recordLog("[INFO] 进入工作台时收到停止请求");
            onFinished();
            return;
        }
        
        QString errorMsg = "进入工作台失败";
        recordLog("[ERROR] " + errorMsg);
        if (!m_configManager->getContinueOnError()) {
            emit errorMessage(errorMsg);
            setState(Error);
            recordLog("[DEBUG] 状态已设置为Error");
            emit automationCompleted();
            recordLog("[DEBUG] 已发送automationCompleted信号");
            // 不调用onFinished()，保留Error状态，防止自动重启
            return;
        } else {
            recordLog("[WARNING] 进入工作台失败，继续执行后续步骤");
        }
    } else {
        recordLog("[DEBUG] 已成功进入企业微信工作台");
    }

    // 检查是否请求停止
    if (m_stopRequested) {
        recordLog("[INFO] 收到停止请求，退出runAutomation");
        onFinished();
        return;
    }
    
    // 打开MindSpark
    recordLog("[DEBUG] 开始打开MindSpark应用");
    if (!openMindSpark()) {
        // 检查是否是因为停止请求导致的失败
        if (m_stopRequested) {
            recordLog("[INFO] 打开MindSpark时收到停止请求");
            onFinished();
            return;
        }
        
        QString errorMsg = "打开MindSpark失败";
        recordLog("[ERROR] " + errorMsg);
        if (!m_configManager->getContinueOnError()) {
            emit errorMessage(errorMsg);
            setState(Error);
            recordLog("[DEBUG] 状态已设置为Error");
            emit automationCompleted();
            recordLog("[DEBUG] 已发送automationCompleted信号");
            // 不调用onFinished()，保留Error状态，防止自动重启
            return;
        } else {
            recordLog("[WARNING] 打开MindSpark失败，继续执行后续步骤");
        }
    } else {
        recordLog("[DEBUG] 已成功打开MindSpark应用");
    }

    // 进入历史对话界面
    recordLog("[DEBUG] 开始进入历史对话界面");
    if (!enterHistoryDialog()) {
        // 检查是否是因为停止请求导致的失败
        if (m_stopRequested) {
            recordLog("[INFO] 进入历史对话时收到停止请求");
            onFinished();
            return;
        }
        
        QString errorMsg = "进入历史对话失败";
        recordLog("[ERROR] " + errorMsg);
        if (!m_configManager->getContinueOnError()) {
            emit errorMessage(errorMsg);
            setState(Error);
            recordLog("[DEBUG] 状态已设置为Error");
            emit automationCompleted();
            recordLog("[DEBUG] 已发送automationCompleted信号");
            // 不调用onFinished()，保留Error状态，防止自动重启
            return;
        } else {
            recordLog("[WARNING] 进入历史对话失败，继续执行后续步骤");
        }
    } else {
        recordLog("[DEBUG] 已成功进入历史对话界面");
    }

    // 获取所有预设问题
    recordLog("[DEBUG] 开始获取所有预设问题");
    QVector<QString> presetQuestions = m_questionManager->getPresetQuestions();
    QStringList questions = presetQuestions.toList();
    recordLog(QString("[DEBUG] 共获取到 %1 个预设问题").arg(questions.size()));
    
    // 检查问题列表是否为空
    if (questions.isEmpty()) {
        recordLog("[WARNING] 预设问题列表为空，无法执行批量发送");
        onFinished();
        return;
    }
    
    // 执行批量发送循环
    recordLog("[DEBUG] 开始执行批量发送，共 " + QString::number(m_totalCount) + " 个问题");
    for (m_currentCount = 0; m_currentCount < m_totalCount; ++m_currentCount) {
        recordLog("[DEBUG] 开始发送第 " + QString::number(m_currentCount + 1) + "/" + QString::number(m_totalCount) + " 个问题");
        
        if (m_stopRequested) {
            recordLog("[INFO] 用户请求停止自动化");
            break;
        }

        recordLog(QString("开始发送第 %1/%2 个问题").arg(m_currentCount + 1).arg(m_totalCount));
        
        // 在开始处理当前问题前更新进度条，显示当前正在处理的问题数
        emit progressUpdated(m_currentCount + 1, m_totalCount);
        recordLog(QString("[DEBUG] 已发送progressUpdated信号，当前进度: %1/%2").arg(m_currentCount + 1).arg(m_totalCount));
        
        // 获取当前要发送的问题（考虑问题模式）
        QString question;
        int questionIndex;
        
        if (questions.isEmpty()) {
            recordLog("[WARNING] 问题列表为空，跳过当前迭代");
            // 只在跳过当前迭代时手动增加计数，否则由for循环自动增加
            continue;
        }
        
        // 根据问题模式获取问题
        switch (m_questionManager->getQuestionMode()) {
        case QuestionManager::RandomMode:
            // 随机模式：每次随机选择一个问题
            questionIndex = QRandomGenerator::global()->bounded(questions.size());
            question = questions.at(questionIndex);
            break;
        case QuestionManager::CycleMode:
        default:
            // 循环模式：循环使用问题列表
            questionIndex = m_currentCount % questions.size();
            question = questions.at(questionIndex);
            break;
        }
        
        recordLog("[DEBUG] 准备发送问题: " + question);
        
        bool sendResult = performQuestionAnswer(question);
        recordLog(QString("[DEBUG] 第 %1 个问题发送完成，结果: %2").arg(m_currentCount + 1).arg(sendResult ? "成功" : "失败"));

        if (!sendResult) {
            recordLog(QString("[ERROR] 第 %1 个问题发送失败").arg(m_currentCount + 1));

            if (!m_configManager->getContinueOnError()) {
                recordLog("[DEBUG] 配置为不继续错误，停止自动化");
                setState(Error);
                recordLog("[DEBUG] 状态已设置为Error");
                emit automationCompleted();
                recordLog("[DEBUG] 已发送automationCompleted信号");
                return;
            }
        }

        // 检查是否请求停止
        if (m_stopRequested) {
            recordLog("[INFO] 用户请求停止自动化");
            break;
        }

        // 清理识别区域，确保下次识别不受影响
        // 删除识别标识框功能
        
        // 重置图像识别器状态
        m_imageRecognizer->setStopRequested(false);
        
        // 如果不是最后一个问题，等待并处理事件
        if (m_currentCount < m_totalCount - 1) {
            recordLog("[DEBUG] 开始等待，确保系统有足够时间处理当前请求");
            
            // 等待3秒，确保系统有足够时间响应
            if (!waitWithESCDetection(3000)) {
                recordLog("[INFO] 用户请求停止自动化");
                break;
            }
            recordLog("[DEBUG] 等待完成，准备发送下一个问题");
        }
    }

    recordLog("[DEBUG] 问答循环执行完成，准备调用onFinished");
    onFinished();
    recordLog("[DEBUG] 自动化流程执行完成");
    } catch (const std::exception& e) {
        recordLog(QString("[ERROR] 自动化流程执行时发生异常: %1").arg(e.what()));
        emit errorMessage(QString("自动化流程执行时发生异常: %1").arg(e.what()));
        setState(Error);
        onFinished();
    } catch (...) {
        recordLog("[ERROR] 自动化流程执行时发生未知异常");
        emit errorMessage("自动化流程执行时发生未知异常");
        setState(Error);
        onFinished();
    }
}

void Automator::onFinished()
{
    // 重置ImageRecognizer的停止请求标志
    m_imageRecognizer->setStopRequested(false);
    
    // 删除识别标识框功能，不再需要清除识别框
    
    if (m_stopRequested) {
        recordLog("[INFO] 自动化被用户停止");
    } else {
        recordLog("[INFO] 自动化执行完成");
    }
    
    // 无论成功还是失败，都设置为Idle状态
    setState(Idle);

    // 重置停止请求标志，确保下次启动能正常运行
    m_stopRequested = false;
    
    // 发送最终进度更新，确保进度条显示正确的最终状态
    emit progressUpdated(m_currentCount, m_totalCount);
    emit automationCompleted();
    
    // 重置InputSimulator的停止标志，确保下次启动能正常运行
    m_inputSimulator->setStopRequested(false);
}

bool Automator::prepareWeChat()
{
    recordLog("[DEBUG] 开始执行prepareWeChat函数");
    // 获取企业微信路径
    QString weChatPath = m_configManager->getWeChatPath();
    if (weChatPath.isEmpty()) {
        recordLog("[INFO] 配置未指定企业微信路径，尝试从注册表获取...");
        weChatPath = m_weChatController->getWeChatPathFromRegistry();
    }

    // 尝试启动企业微信，即使已经在运行也不会有问题
    if (!weChatPath.isEmpty()) {
        m_weChatController->startWeChat(weChatPath);
    } else {
        recordLog("[WARNING] 未找到企业微信路径，跳过启动步骤");
    }

    // 等待企业微信启动，使用阻塞方式，避免窗口焦点问题
    const int maxWaitTime = 15;  // 最多等待15秒
    const int checkInterval = 500;  // 每500毫秒检查一次
    const int maxChecks = maxWaitTime * 1000 / checkInterval;
    
    int attempt = 0;
    
    while (attempt < maxChecks && !m_stopRequested) {
        attempt++;
        
        // 每2秒输出一次等待信息
        if (attempt % 4 == 0) {
            recordLog(QString("[INFO] 等待企业微信启动，已等待 %1 秒...").arg((attempt - 1) * checkInterval / 1000));
        }
        
        // 尝试激活窗口，如果成功则退出等待
        if (m_weChatController->activateWeChatWindow()) {
            recordLog("[INFO] 企业微信窗口已激活，跳过剩余等待时间");
            break;
        }
        
        // 等待检查间隔
        QThread::msleep(checkInterval);
    }
    
    // 激活企业微信窗口
    if (!m_weChatController->activateWeChatWindow()) {
        QString errorMsg = "企业微信未准备好";
        recordLog("[ERROR] " + errorMsg);
        emit errorMessage(errorMsg);
        return false;
    }

    // 激活窗口并置顶
    if (m_configManager->getWindowTopMost()) {
        m_weChatController->setWindowTopMost(true);
    }

    return true;
}

bool Automator::enterWeChatWorkbench()
{
    recordLog("[DEBUG] 开始执行enterWeChatWorkbench函数");

    try {
        // 获取企业微信窗口句柄
        recordLog("[DEBUG] 获取企业微信窗口句柄");
        HWND hwnd = m_weChatController->getWeChatWindowHandle();
        if (!hwnd) {
            recordLog("[ERROR] 无法获取企业微信窗口句柄");
            QString errorMsg = "无法获取企业微信窗口句柄，无法继续操作";
            emit errorMessage(errorMsg);
            return false;
        }
    recordLog(QString("[DEBUG] 获取到企业微信窗口句柄: %1").arg((quintptr)hwnd, 0, 16));

    QPoint workbenchPos;
    bool found = false;
    
    // 多次尝试查找工作台位置（仅使用图像识别）
    const int maxAttempts = 5;
    recordLog("[DEBUG] 开始查找工作台图标，最大尝试次数: " + QString::number(maxAttempts));
    
    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        recordLog(QString("[DEBUG] 尝试 %1/%2 查找工作台图标").arg(attempt + 1).arg(maxAttempts));
        
        // 检查是否请求停止
        if (m_stopRequested) {
            recordLog("[INFO] 收到停止请求，退出enterWeChatWorkbench");
            return false;
        }
        
        // 1. 捕获企业微信首页截图
        QImage windowImage = m_imageRecognizer->captureWindow(hwnd);
        if (windowImage.isNull()) {
            recordLog("[ERROR] 无法捕获企业微信窗口图像");
            continue;
        }
        
        // 2. 在截图左侧区域进行工作台识别
        recordLog("[DEBUG] 在截图左侧区域进行工作台识别");
        if (m_imageRecognizer->findTemplateInWindow(hwnd, "workbench", workbenchPos)) {
            found = true;
            recordLog(QString("[INFO] 图像识别找到工作台图标位置: (%1, %2)").arg(workbenchPos.x()).arg(workbenchPos.y()));
            
            // 获取模板尺寸，计算点击位置为识别区域的中心点
            ConfigManager* config = ConfigManager::getInstance();
            QSize templateSize = config->getTemplateSize("workbench");
            if (templateSize.width() == 100 && templateSize.height() == 100) {
                // 如果配置中没有保存尺寸，使用默认尺寸
                templateSize = QSize(64, 64);
            }
            
            // 计算中心点坐标
            workbenchPos.setX(workbenchPos.x() + templateSize.width() / 2);
            workbenchPos.setY(workbenchPos.y() + templateSize.height() / 2);
            recordLog(QString("[DEBUG] 计算得到工作台图标中心点位置: (%1, %2)").arg(workbenchPos.x()).arg(workbenchPos.y()));
            break;
        }
        
        recordLog(QString("[DEBUG] 未找到工作台图标，尝试 %1/%2").arg(attempt + 1).arg(maxAttempts));
        
        // 如果是第一次尝试失败，尝试重新最大化窗口
        if (attempt == 0) {
            recordLog("[DEBUG] 第一次尝试失败，尝试重新最大化窗口");
            // 重新最大化窗口，确保完全显示
            ShowWindow(hwnd, SW_MAXIMIZE);
            recordLog("[DEBUG] 已重新最大化窗口，重新尝试查找工作台图标");
        }
        
        // 等待一段时间后重试，使用waitWithESCDetection
        recordLog("[DEBUG] 等待1秒后重试");
        if (!waitWithESCDetection(1000)) {
            recordLog("[INFO] 收到停止请求，退出enterWeChatWorkbench");
            return false;
        }
    }

    if (!found) {
        recordLog("[ERROR] 多次尝试后仍未找到工作台图标，无法继续操作");
        QString errorMsg = "无法识别工作台图标，请检查企业微信是否正常运行或调整识别参数";
        emit errorMessage(errorMsg);
        this->stop();
        return false;
    }

    // 转换为屏幕坐标
            POINT clientPos = {workbenchPos.x(), workbenchPos.y()};
            POINT screenPos;
            ClientToScreen(hwnd, &clientPos);
            screenPos = clientPos;
            recordLog(QString("[DEBUG] 转换为屏幕坐标: (%1, %2)").arg(screenPos.x).arg(screenPos.y));

            // 等待识别框显示，确保用户能看到识别结果
            recordLog("[DEBUG] 等待200毫秒，确保识别框显示");
            QThread::msleep(200);

            // 点击工作台图标
            recordLog("[DEBUG] 准备点击工作台图标");
            m_inputSimulator->clickAt(screenPos.x, screenPos.y);
            recordLog("[DEBUG] 点击工作台图标完成");
    
    // 等待工作台加载，根据系统性能调整等待时间
    int waitTime = m_configManager->getPageLoadTimeout();
    recordLog(QString("[DEBUG] 开始等待工作台加载，等待时间: %1 毫秒").arg(waitTime));
    
    // 使用QThread::msleep代替事件循环，避免窗口焦点问题
    const int checkInterval = 500; // 每500毫秒检查一次
    int elapsed = 0;
    while (elapsed < waitTime && !m_stopRequested) {
        QThread::msleep(checkInterval);
        elapsed += checkInterval;
        recordLog("[DEBUG] 等待中，已等待 " + QString::number(elapsed) + " 毫秒");
    }
    
    // 检查是否请求停止
    if (m_stopRequested) {
        return false;
    }
    
    recordLog("[DEBUG] 工作台加载等待完成，已进入企业微信工作台");
    return true;
    } catch (const std::exception& e) {
        recordLog(QString("[ERROR] 进入工作台时发生异常: %1").arg(e.what()));
        emit errorMessage(QString("进入工作台时发生异常: %1").arg(e.what()));
        return false;
    } catch (...) {
        recordLog("[ERROR] 进入工作台时发生未知异常");
        emit errorMessage("进入工作台时发生未知异常");
        return false;
    }
}

bool Automator::openMindSpark()
{
    recordLog("[DEBUG] 开始执行openMindSpark函数");

    try {
        // 获取企业微信窗口句柄
        recordLog("[DEBUG] 获取企业微信窗口句柄");
        HWND hwnd = m_weChatController->getWeChatWindowHandle();
        if (!hwnd) {
            recordLog("[ERROR] 无法获取企业微信窗口句柄");
            return false;
        }
    recordLog(QString("[DEBUG] 获取到企业微信窗口句柄: %1").arg((quintptr)hwnd, 0, 16));

    QPoint mindsparkPos;
    bool found = false;
    
    // 多次尝试查找MindSpark图标位置（仅使用图像识别）
    const int maxAttempts = 5;
    recordLog("[DEBUG] 开始查找MindSpark图标，最大尝试次数: " + QString::number(maxAttempts));
    
    // 定义模板优先级：先尝试小图标，再尝试大图标
    QStringList smallIconTemplates = {"mindspark_small"};
    QStringList largeIconTemplates = {"mindspark"};
    
    // 1. 先尝试识别小图标
    recordLog("[DEBUG] 开始尝试识别MindSpark小图标");
    for (int attempt = 0; attempt < maxAttempts && !found && !m_stopRequested; ++attempt) {
        recordLog(QString("[DEBUG] 尝试 %1/%2 查找MindSpark小图标").arg(attempt + 1).arg(maxAttempts));
        
        // 尝试所有小图标模板
        for (const QString& templateName : smallIconTemplates) {
            recordLog(QString("[DEBUG] 使用模板 '%1' 查找MindSpark小图标").arg(templateName));
            if (m_imageRecognizer->findTemplateInWindow(hwnd, templateName, mindsparkPos)) {
                found = true;
                recordLog(QString("[INFO] 使用模板 '%1' 找到MindSpark小图标，位置: (%2, %3)").arg(templateName).arg(mindsparkPos.x()).arg(mindsparkPos.y()));
                
                // 获取模板尺寸，计算点击位置为识别区域的中心点
                ConfigManager* config = ConfigManager::getInstance();
                QSize templateSize = config->getTemplateSize(templateName);
                if (templateSize.width() == 100 && templateSize.height() == 100) {
                    // 如果配置中没有保存尺寸，使用默认尺寸
                    templateSize = QSize(48, 48);
                }
                
                // 计算中心点坐标
                mindsparkPos.setX(mindsparkPos.x() + templateSize.width() / 2);
                mindsparkPos.setY(mindsparkPos.y() + templateSize.height() / 2);
                recordLog(QString("[DEBUG] 计算得到MindSpark小图标中心点位置: (%1, %2)").arg(mindsparkPos.x()).arg(mindsparkPos.y()));
                break;
            }
            recordLog(QString("[DEBUG] 使用模板 '%1' 未找到MindSpark小图标").arg(templateName));
        }
        
        if (found) break;
        
        // 如果是第一次尝试失败，尝试滚动工作台页面
        if (attempt == 0) {
            recordLog("[DEBUG] 第一次尝试失败，尝试滚动工作台页面");
            // 获取窗口客户区大小
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int clientWidth = clientRect.right - clientRect.left;
            int clientHeight = clientRect.bottom - clientRect.top;
            recordLog(QString("[DEBUG] 客户区大小: %1x%2").arg(clientWidth).arg(clientHeight));
            
            // 计算滚动起点和终点（从中间向上滚动）
            POINT startPos = {clientWidth / 2, clientHeight / 2};
            POINT endPos = {clientWidth / 2, clientHeight / 4};
            recordLog(QString("[DEBUG] 滚动起点: (%1, %2), 终点: (%3, %4)").arg(startPos.x).arg(startPos.y).arg(endPos.x).arg(endPos.y));
            
            // 转换为屏幕坐标
            ClientToScreen(hwnd, &startPos);
            ClientToScreen(hwnd, &endPos);
            recordLog(QString("[DEBUG] 转换为屏幕坐标 - 起点: (%1, %2), 终点: (%3, %4)").arg(startPos.x).arg(startPos.y).arg(endPos.x).arg(endPos.y));
            
            // 执行滚动操作
            recordLog("[DEBUG] 执行滚动操作");
            m_inputSimulator->dragMouse(startPos.x, startPos.y, endPos.x, endPos.y);
            recordLog("[DEBUG] 已滚动工作台页面，重新查找MindSpark小图标");
        }
        
        // 等待一段时间后重试
        recordLog("[DEBUG] 等待1秒后重试");
        if (!waitWithESCDetection(1000)) {
            recordLog("[INFO] 收到停止请求，退出openMindSpark");
            return false;
        }
    }
    
    // 2. 如果未识别到小图标，尝试识别大图标
    if (!found && !m_stopRequested) {
        recordLog("[DEBUG] 未找到MindSpark小图标，开始尝试识别大图标");
        
        for (int attempt = 0; attempt < maxAttempts && !found && !m_stopRequested; ++attempt) {
            recordLog(QString("[DEBUG] 尝试 %1/%2 查找MindSpark大图标").arg(attempt + 1).arg(maxAttempts));
            
            // 尝试所有大图标模板
            for (const QString& templateName : largeIconTemplates) {
                recordLog(QString("[DEBUG] 使用模板 '%1' 查找MindSpark大图标").arg(templateName));
                if (m_imageRecognizer->findTemplateInWindow(hwnd, templateName, mindsparkPos)) {
                    found = true;
                    recordLog(QString("[INFO] 使用模板 '%1' 找到MindSpark大图标，位置: (%2, %3)").arg(templateName).arg(mindsparkPos.x()).arg(mindsparkPos.y()));
                    
                    // 获取模板尺寸，计算点击位置为识别区域的中心点
                    ConfigManager* config = ConfigManager::getInstance();
                    QSize templateSize = config->getTemplateSize(templateName);
                    if (templateSize.width() == 100 && templateSize.height() == 100) {
                        // 如果配置中没有保存尺寸，使用默认尺寸
                        templateSize = QSize(64, 64);
                    }
                    
                    // 计算中心点坐标
                    mindsparkPos.setX(mindsparkPos.x() + templateSize.width() / 2);
                    mindsparkPos.setY(mindsparkPos.y() + templateSize.height() / 2);
                    recordLog(QString("[DEBUG] 计算得到MindSpark大图标中心点位置: (%1, %2)").arg(mindsparkPos.x()).arg(mindsparkPos.y()));
                    break;
                }
                recordLog(QString("[DEBUG] 使用模板 '%1' 未找到MindSpark大图标").arg(templateName));
            }
            
            if (found) break;
            
            // 如果是第一次尝试失败，尝试滚动工作台页面
            if (attempt == 0) {
                recordLog("[DEBUG] 第一次尝试失败，尝试滚动工作台页面");
                // 获取窗口客户区大小
                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                int clientWidth = clientRect.right - clientRect.left;
                int clientHeight = clientRect.bottom - clientRect.top;
                recordLog(QString("[DEBUG] 客户区大小: %1x%2").arg(clientWidth).arg(clientHeight));
                
                // 计算滚动起点和终点（从中间向上滚动）
                POINT startPos = {clientWidth / 2, clientHeight / 2};
                POINT endPos = {clientWidth / 2, clientHeight / 4};
                recordLog(QString("[DEBUG] 滚动起点: (%1, %2), 终点: (%3, %4)").arg(startPos.x).arg(startPos.y).arg(endPos.x).arg(endPos.y));
                
                // 转换为屏幕坐标
                ClientToScreen(hwnd, &startPos);
                ClientToScreen(hwnd, &endPos);
                recordLog(QString("[DEBUG] 转换为屏幕坐标 - 起点: (%1, %2), 终点: (%3, %4)").arg(startPos.x).arg(startPos.y).arg(endPos.x).arg(endPos.y));
                
                // 执行滚动操作
                recordLog("[DEBUG] 执行滚动操作");
                m_inputSimulator->dragMouse(startPos.x, startPos.y, endPos.x, endPos.y);
                recordLog("[DEBUG] 已滚动工作台页面，重新查找MindSpark大图标");
            }
            
            // 等待一段时间后重试
            recordLog("[DEBUG] 等待1秒后重试");
            if (!waitWithESCDetection(1000)) {
                recordLog("[INFO] 收到停止请求，退出openMindSpark");
                return false;
            }
        }
    }
    
    // 3. 如果仍然没有找到，显示错误信息
    if (!found) {
        recordLog("[ERROR] 多次尝试后仍未找到MindSpark图标，无法继续操作");
        QString errorMsg = "无法识别MindSpark图标，请检查企业微信是否正常运行或调整识别参数";
        emit errorMessage(errorMsg);
        // 设置停止标志，让调用者处理后续逻辑，避免重复调用stop()导致崩溃
        m_stopRequested = true;
        return false;
    }

    // 转换为屏幕坐标
    POINT clientPos = {mindsparkPos.x(), mindsparkPos.y()};
    POINT screenPos;
    ClientToScreen(hwnd, &clientPos);
    screenPos = clientPos;
    recordLog(QString("[DEBUG] 转换为屏幕坐标: (%1, %2)").arg(screenPos.x).arg(screenPos.y));

    // 点击MindSpark
    recordLog("[DEBUG] 准备点击MindSpark");
    m_inputSimulator->clickAt(screenPos.x, screenPos.y);
    recordLog("[DEBUG] 点击完成");
    
    // 等待MindSpark加载，根据系统性能调整等待时间
    int waitTime = m_configManager->getPageLoadTimeout();
    recordLog(QString("[DEBUG] 开始等待MindSpark加载，等待时间: %1 毫秒").arg(waitTime));
    
    // 使用QThread::msleep代替事件循环，避免窗口焦点问题
    const int checkInterval = 500; // 每500毫秒检查一次
    int elapsed = 0;
    while (elapsed < waitTime && !m_stopRequested) {
        QThread::msleep(checkInterval);
        elapsed += checkInterval;
        recordLog("[DEBUG] 等待中，已等待 " + QString::number(elapsed) + " 毫秒");
    }
    
    // 检查是否请求停止
    if (m_stopRequested) {
        recordLog("[INFO] 收到停止请求，退出openMindSpark");
        return false;
    }
    
    // 5. 无论图标类型，首先尝试识别输入框
    recordLog("[DEBUG] 无论图标类型，首先尝试识别输入框");
    
    // 直接使用已声明的hwnd变量
    if (hwnd) {
        // 尝试识别输入框
        QPoint inputBoxPos;
        QStringList inputBoxTemplates = {"input_box", "input_box_small", "input_box_large"};
        bool foundInputBox = false;
        
        for (const QString& templateName : inputBoxTemplates) {
            if (m_imageRecognizer->findTemplateInWindow(hwnd, templateName, inputBoxPos)) {
                foundInputBox = true;
                recordLog(QString("[INFO] 已识别到输入框，位置: (%1, %2)").arg(inputBoxPos.x()).arg(inputBoxPos.y()));
                break;
            }
        }
        
        if (foundInputBox) {
            // 已识别到输入框，直接返回成功
            recordLog("[DEBUG] 已成功识别到输入框，跳过历史记录步骤");
            recordLog("[DEBUG] MindSpark加载等待完成，已打开MindSpark应用");
            return true;
        } else {
            // 未识别到输入框，才尝试识别历史对话图标
            recordLog("[DEBUG] 未识别到输入框，尝试识别历史对话图标");
            
            // 调用enterHistoryDialog方法进入历史记录
            if (!enterHistoryDialog()) {
                recordLog("[WARNING] 无法进入历史记录，但已打开MindSpark，尝试继续执行");
            }
        }
    }
    
    recordLog("[DEBUG] MindSpark加载等待完成，已打开MindSpark应用");
    return true;
    } catch (const std::exception& e) {
        recordLog(QString("[ERROR] 打开MindSpark时发生异常: %1").arg(e.what()));
        emit errorMessage(QString("打开MindSpark时发生异常: %1").arg(e.what()));
        return false;
    } catch (...) {
        recordLog("[ERROR] 打开MindSpark时发生未知异常");
        emit errorMessage("打开MindSpark时发生未知异常");
        return false;
    }
}

bool Automator::enterHistoryDialog()
{
    recordLog("[DEBUG] 开始执行enterHistoryDialog函数");

    try {
        // 获取企业微信窗口句柄
        recordLog("[DEBUG] 获取企业微信窗口句柄");
        HWND hwnd = m_weChatController->getWeChatWindowHandle();
        if (!hwnd) {
            recordLog("[ERROR] 无法获取企业微信窗口句柄");
            return false;
        }
    recordLog(QString("[DEBUG] 获取到企业微信窗口句柄: %1").arg((quintptr)hwnd, 0, 16));

    QPoint historyDialogPos;
    bool found = false;
    
    // 多次尝试查找历史对话图标位置（仅使用图像识别）
    const int maxAttempts = 5;
    recordLog("[DEBUG] 开始查找历史对话图标，最大尝试次数: " + QString::number(maxAttempts));
    
    // 只使用默认的历史对话模板
    QStringList templateNames = {"history_dialog"};
    recordLog("[DEBUG] 准备尝试的模板: " + templateNames.join(", "));
    
    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        recordLog(QString("[DEBUG] 尝试 %1/%2 查找历史对话图标").arg(attempt + 1).arg(maxAttempts));
        
        // 检查是否请求停止
        if (m_stopRequested) {
            recordLog("[INFO] 收到停止请求，退出enterHistoryDialog");
            return false;
        }
        
        // 1. 捕获MindSpark界面截图
        QImage mindsparkImage = m_imageRecognizer->captureWindow(hwnd);
    
        // 2. 尝试图像识别查找历史对话图标
        recordLog("[DEBUG] 尝试图像识别查找历史对话图标");
        // 尝试所有可能的模板
        bool templateFound = false;
        for (const QString& templateName : templateNames) {
            recordLog(QString("[DEBUG] 使用模板 '%1' 查找历史对话图标").arg(templateName));
            if (m_imageRecognizer->findTemplateInWindow(hwnd, templateName, historyDialogPos)) {
                found = true;
                templateFound = true;
                recordLog(QString("[INFO] 使用模板 '%1' 找到历史对话图标，位置: (%2, %3)").arg(templateName).arg(historyDialogPos.x()).arg(historyDialogPos.y()));
                
                // 获取模板尺寸，计算点击位置为识别区域的中心点
                ConfigManager* config = ConfigManager::getInstance();
                QSize templateSize = config->getTemplateSize(templateName);
                if (templateSize.width() == 100 && templateSize.height() == 100) {
                    // 如果配置中没有保存尺寸，使用默认尺寸
                    templateSize = QSize(64, 64);
                }
                
                // 计算中心点坐标
                historyDialogPos.setX(historyDialogPos.x() + templateSize.width() / 2);
                historyDialogPos.setY(historyDialogPos.y() + templateSize.height() / 2);
                recordLog(QString("[DEBUG] 计算得到历史对话图标中心点位置: (%1, %2)").arg(historyDialogPos.x()).arg(historyDialogPos.y()));
                break;
            }
            recordLog(QString("[DEBUG] 使用模板 '%1' 未找到历史对话图标").arg(templateName));
        }
        
        if (templateFound) break;
        
        recordLog(QString("[DEBUG] 未找到历史对话图标，尝试 %1/%2").arg(attempt + 1).arg(maxAttempts));
        
        // 如果是第一次尝试失败，尝试滚动页面
        if (attempt == 0 || attempt == 2) {
            recordLog("[DEBUG] 尝试滚动页面");
            // 获取窗口客户区大小
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int clientWidth = clientRect.right - clientRect.left;
            int clientHeight = clientRect.bottom - clientRect.top;
            recordLog(QString("[DEBUG] 客户区大小: %1x%2").arg(clientWidth).arg(clientHeight));
            
            // 计算滚动起点和终点（从中间向上滚动）
            POINT startPos = {clientWidth / 2, clientHeight / 2};
            POINT endPos = {clientWidth / 2, clientHeight / 4};
            recordLog(QString("[DEBUG] 滚动起点: (%1, %2), 终点: (%3, %4)").arg(startPos.x).arg(startPos.y).arg(endPos.x).arg(endPos.y));
            
            // 转换为屏幕坐标
            ClientToScreen(hwnd, &startPos);
            ClientToScreen(hwnd, &endPos);
            recordLog(QString("[DEBUG] 转换为屏幕坐标 - 起点: (%1, %2), 终点: (%3, %4)").arg(startPos.x).arg(startPos.y).arg(endPos.x).arg(endPos.y));
            
            // 执行滚动操作
            recordLog("[DEBUG] 执行滚动操作");
            m_inputSimulator->dragMouse(startPos.x, startPos.y, endPos.x, endPos.y);
            recordLog("[DEBUG] 已滚动页面，重新查找历史对话图标");
        }
        
        // 等待一段时间后重试，使用QThread::msleep
        recordLog("[DEBUG] 等待1秒后重试");
        QThread::msleep(1000);
        
        // 检查是否请求停止
        if (m_stopRequested) {
            recordLog("[INFO] 收到停止请求，退出enterHistoryDialog");
            return false;
        }
    }

    if (!found) {
        recordLog("[ERROR] 多次尝试后仍未找到历史对话图标，无法继续操作");
        QString errorMsg = "无法识别历史对话图标，请检查企业微信是否正常运行或调整识别参数";
        emit errorMessage(errorMsg);
        this->stop();
        return false;
    }

    // 转换为屏幕坐标
    POINT clientPos = {historyDialogPos.x(), historyDialogPos.y()};
    POINT screenPos;
    ClientToScreen(hwnd, &clientPos);
    screenPos = clientPos;
    recordLog(QString("[DEBUG] 转换为屏幕坐标: (%1, %2)").arg(screenPos.x).arg(screenPos.y));

    // 点击历史对话图标
    recordLog("[DEBUG] 准备点击历史对话图标");
    m_inputSimulator->clickAt(screenPos.x, screenPos.y);
    recordLog("[DEBUG] 点击完成");
    
    // 等待历史对话界面加载，根据系统性能调整等待时间
    int waitTime = m_configManager->getPageLoadTimeout();
    recordLog(QString("[DEBUG] 开始等待历史对话界面加载，等待时间: %1 毫秒").arg(waitTime));
    
    // 使用waitWithESCDetection代替QThread::msleep，支持ESC按键检测
    if (!waitWithESCDetection(waitTime, hwnd)) {
        return false;
    }

    recordLog("[DEBUG] 历史对话界面加载等待完成，已进入历史对话界面");
    return true;
    } catch (const std::exception& e) {
        recordLog(QString("[ERROR] 进入历史对话时发生异常: %1").arg(e.what()));
        emit errorMessage(QString("进入历史对话时发生异常: %1").arg(e.what()));
        return false;
    } catch (...) {
        recordLog("[ERROR] 进入历史对话时发生未知异常");
        emit errorMessage("进入历史对话时发生未知异常");
        return false;
    }
}


    bool Automator::performQuestionAnswer(const QString& question)
{    
    try {
        // 获取企业微信窗口句柄
        HWND hwnd = m_weChatController->getWeChatWindowHandle();
        if (!hwnd) {
            recordLog("[ERROR] 无法获取企业微信窗口句柄");
            return false;
        }
    recordLog(QString("[DEBUG] 获取到企业微信窗口句柄: %1").arg((quintptr)hwnd, 0, 16));
    
    // 1. 捕获MindSpark页面截图（仅用于识别，不保存）
    QImage mindsparkImage = m_imageRecognizer->captureWindow(hwnd);
    
    // 2. 查找输入框，尝试多个模板变体
    QPoint inputBoxPos;
    bool foundInputBox = false;
    QStringList inputBoxTemplates = {"input_box", "input_box_small", "input_box_large"};
    recordLog("[DEBUG] 开始查找输入框，尝试的模板: " + inputBoxTemplates.join(", "));
    
    // 2.1 尝试图像识别查找输入框
    recordLog("[DEBUG] 尝试图像识别查找输入框");
    
    // 优化：增加模板识别的重试机制
    int maxRetries = 3;
    for (int retry = 0; retry < maxRetries && !foundInputBox; ++retry) {
        for (const QString& templateName : inputBoxTemplates) {
            recordLog(QString("[DEBUG] 使用模板 '%1' 查找输入框，重试次数: %2").arg(templateName).arg(retry + 1));
            if (m_imageRecognizer->findTemplateInWindow(hwnd, templateName, inputBoxPos)) {
                foundInputBox = true;
                recordLog(QString("[INFO] 使用模板 '%1' 找到输入框，位置: (%2, %3)").arg(templateName).arg(inputBoxPos.x()).arg(inputBoxPos.y()));
                
                // 获取模板尺寸，计算点击位置为识别区域的中心点
                ConfigManager* config = ConfigManager::getInstance();
                QSize templateSize = config->getTemplateSize(templateName);
                if (templateSize.width() == 100 && templateSize.height() == 100) {
                    // 如果配置中没有保存尺寸，使用模板文件的实际尺寸
                    QImage templateImage(config->getIconPath(templateName));
                    if (!templateImage.isNull()) {
                        templateSize = templateImage.size();
                        recordLog(QString("[DEBUG] 使用模板文件实际尺寸: %1x%2").arg(templateSize.width()).arg(templateSize.height()));
                    } else {
                        // 如果无法获取模板文件尺寸，使用默认尺寸
                        templateSize = QSize(200, 40);
                        recordLog(QString("[DEBUG] 使用默认尺寸: %1x%2").arg(templateSize.width()).arg(templateSize.height()));
                    }
                }
                
                // 计算中心点坐标，优化：略微调整点击位置到输入框上部，提高点击成功率
                inputBoxPos.setX(inputBoxPos.x() + templateSize.width() / 2);
                inputBoxPos.setY(inputBoxPos.y() + templateSize.height() / 3); // 点击输入框上部，提高成功率
                recordLog(QString("[DEBUG] 计算得到输入框点击位置: (%1, %2)").arg(inputBoxPos.x()).arg(inputBoxPos.y()));
                break;
            }
            recordLog(QString("[DEBUG] 使用模板 '%1' 未找到输入框，重试次数: %2").arg(templateName).arg(retry + 1));
        }
        
        // 如果没找到，短暂延时后重试
        if (!foundInputBox && retry < maxRetries - 1) {
            QThread::msleep(500);
            recordLog(QString("[DEBUG] 输入框识别失败，%1毫秒后重试").arg(500));
        }
    }
    
    if (!foundInputBox) {
        recordLog("[WARNING] 未找到输入框，尝试使用固定位置");
        
        // 获取窗口客户区大小
        recordLog("[DEBUG] 获取窗口客户区大小");
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        int clientWidth = clientRect.right - clientRect.left;
        int clientHeight = clientRect.bottom - clientRect.top;
        recordLog(QString("[DEBUG] 客户区大小: %1x%2").arg(clientWidth).arg(clientHeight));
        
        // 优化：使用更精确的固定位置计算
        // 假设输入框在窗口底部中间位置，距离底部约70像素
        inputBoxPos.setX(clientWidth / 2);
        inputBoxPos.setY(clientHeight - 70);
        recordLog(QString("[DEBUG] 使用优化的固定位置作为输入框位置: (%1, %2)").arg(inputBoxPos.x()).arg(inputBoxPos.y()));
    }

    // 转换为屏幕坐标
    POINT clientPos = {inputBoxPos.x(), inputBoxPos.y()};
    POINT inputScreenPos;
    ClientToScreen(hwnd, &clientPos);
    inputScreenPos = clientPos;
    recordLog(QString("[DEBUG] 输入框屏幕坐标: (%1, %2)").arg(inputScreenPos.x).arg(inputScreenPos.y));

    // 使用传入的问题
     recordLog("[DEBUG] 准备发送问题: " + question);

     // 添加回答限制提示
     QString answerLimitPrompt = m_configManager->getAnswerLimitPrompt();
     recordLog("[DEBUG] 获取到回答限制提示: " + answerLimitPrompt);
     QString finalQuestion = question + " " + answerLimitPrompt;
     recordLog(QString("[DEBUG] 最终发送问题: %1").arg(finalQuestion));

     // 确保企业微信窗口在前台
     SetForegroundWindow(hwnd);
     QThread::msleep(500);
     
     // 确保输入框获得焦点 - 更可靠的点击方式
     recordLog("[DEBUG] 第一次点击输入框，确保获得焦点");
     m_inputSimulator->clickAt(inputScreenPos.x, inputScreenPos.y);
     
     // 短暂延时后再次点击，确保焦点获取
     QThread::msleep(300);
     recordLog("[DEBUG] 第二次点击输入框，确保获得焦点");
     m_inputSimulator->clickAt(inputScreenPos.x, inputScreenPos.y);
     
     // 增加更长的延时，确保输入框完全获得焦点
     QThread::msleep(1500); 
     
     // 简化焦点验证，直接检查前台窗口
     HWND focusedHwnd = GetForegroundWindow();
     if (focusedHwnd != hwnd) {
         recordLog(QString("[WARNING] 前台窗口不是企业微信窗口，尝试再次激活"));
         SetForegroundWindow(hwnd);
         QThread::msleep(500);
         
         // 第三次点击输入框
         recordLog("[DEBUG] 第三次点击输入框，确保获得焦点");
         m_inputSimulator->clickAt(inputScreenPos.x, inputScreenPos.y);
         QThread::msleep(1000);
     }
     
     recordLog("[DEBUG] 输入框焦点处理完成，准备输入文字");
     
     // 输入问题
     recordLog("[DEBUG] 准备输入问题: " + question);
     recordLog(QString("[DEBUG] 问题长度: %1 字符").arg(question.length()));
     recordLog(QString("[DEBUG] 问题编码: %1").arg(question.toUtf8().toHex()));
     
     // 根据配置选择输入方式
     int inputMethod = m_configManager->getInputMethod();
     recordLog(QString("[DEBUG] 使用输入方式: %1 (0=键盘, 1=粘贴)").arg(inputMethod));
     
     // 根据输入方式选择输入方法
     if (inputMethod == 0) {
         // 使用键盘模拟输入文本
         recordLog("[DEBUG] 使用键盘模拟方式输入文本");
         m_inputSimulator->typeText(finalQuestion);
     } else {
         // 使用复制粘贴方式输入文本
         recordLog("[DEBUG] 使用复制粘贴方式输入文本");
         m_inputSimulator->pasteText(finalQuestion);
     }
     recordLog("[DEBUG] 问题输入完成");
     
     // 等待500ms，确保输入完成
     QThread::msleep(500);

    // 3. 点击发送按钮，尝试多个模板变体
    QPoint sendBtnPos;
    bool foundSendButton = false;
    QStringList sendButtonTemplates = {"send_button", "send_button_small", "send_button_large"};
    recordLog("[DEBUG] 开始查找发送按钮，尝试的模板: " + sendButtonTemplates.join(", "));
    
    // 3.2 尝试图像识别查找发送按钮
    recordLog("[DEBUG] 尝试图像识别查找发送按钮");
    for (const QString& templateName : sendButtonTemplates) {
        recordLog(QString("[DEBUG] 使用模板 '%1' 查找发送按钮").arg(templateName));
        if (m_imageRecognizer->findTemplateInWindow(hwnd, templateName, sendBtnPos)) {
            foundSendButton = true;
            recordLog(QString("[INFO] 使用模板 '%1' 找到发送按钮，位置: (%2, %3)").arg(templateName).arg(sendBtnPos.x()).arg(sendBtnPos.y()));
            
            // 获取模板尺寸，计算点击位置为识别区域的中心点
            ConfigManager* config = ConfigManager::getInstance();
            QSize templateSize = config->getTemplateSize(templateName);
            if (templateSize.width() == 100 && templateSize.height() == 100) {
                // 如果配置中没有保存尺寸，使用默认尺寸
                templateSize = QSize(60, 30);
            }
            
            // 计算中心点坐标
            sendBtnPos.setX(sendBtnPos.x() + templateSize.width() / 2);
            sendBtnPos.setY(sendBtnPos.y() + templateSize.height() / 2);
            recordLog(QString("[DEBUG] 计算得到发送按钮中心点位置: (%1, %2)").arg(sendBtnPos.x()).arg(sendBtnPos.y()));
            
            // 保存上次发送按钮位置
            m_lastSendButtonPos = sendBtnPos;
            m_hasLastSendButtonPos = true;
            recordLog(QString("[DEBUG] 保存发送按钮位置: (%1, %2)").arg(m_lastSendButtonPos.x()).arg(m_lastSendButtonPos.y()));
            break;
        }
        recordLog(QString("[DEBUG] 使用模板 '%1' 未找到发送按钮").arg(templateName));
    }
    
    if (foundSendButton) {
         recordLog("[DEBUG] 准备点击发送按钮");
         POINT clientPos = {sendBtnPos.x(), sendBtnPos.y()};
         POINT sendScreenPos;
         ClientToScreen(hwnd, &clientPos);
         sendScreenPos = clientPos;
         recordLog(QString("[DEBUG] 发送按钮屏幕坐标: (%1, %2)").arg(sendScreenPos.x).arg(sendScreenPos.y));
         
         // 单次点击发送按钮
         m_inputSimulator->clickAt(sendScreenPos.x, sendScreenPos.y);
         recordLog("[DEBUG] 发送按钮点击完成");
         
         // 等待500毫秒，确保发送操作完成
         QThread::msleep(500); 
     } else if (m_hasLastSendButtonPos) {
         // 如果找不到发送按钮，使用上次的位置点击两次
         recordLog(QString("[WARNING] 未找到发送按钮，使用上次位置 (%1, %2) 点击两次发送").arg(m_lastSendButtonPos.x()).arg(m_lastSendButtonPos.y()));
         
         // 转换为屏幕坐标
         POINT clientPos = {m_lastSendButtonPos.x(), m_lastSendButtonPos.y()};
         POINT sendScreenPos;
         ClientToScreen(hwnd, &clientPos);
         sendScreenPos = clientPos;
         recordLog(QString("[DEBUG] 上次发送按钮屏幕坐标: (%1, %2)").arg(sendScreenPos.x).arg(sendScreenPos.y));
         
         // 第一次点击
         m_inputSimulator->clickAt(sendScreenPos.x, sendScreenPos.y);
         recordLog("[DEBUG] 第一次点击发送按钮完成");
         QThread::msleep(200);
         
         // 第二次点击
         m_inputSimulator->clickAt(sendScreenPos.x, sendScreenPos.y);
         recordLog("[DEBUG] 第二次点击发送按钮完成");
         QThread::msleep(500);
     } else {
         recordLog("[WARNING] 未找到发送按钮，尝试用Enter发送");
         
         // 单次Enter键发送
         m_inputSimulator->pressKey(VK_RETURN);
         recordLog("[DEBUG] Enter键发送完成");
         
         // 等待500毫秒，确保发送操作完成
         QThread::msleep(500); 
     }

    // 等待回答完成
    recordLog("[DEBUG] 开始等待回答完成");
    if (!waitForAnswerCompletion(hwnd)) {
        recordLog("[WARNING] 等待回答超时");
        if (!m_configManager->getContinueOnTimeout()) {
            recordLog("[DEBUG] 配置为不继续超时，返回失败");
            return false;
        }
        recordLog("[DEBUG] 配置为继续超时，返回成功");
    } else {
        recordLog("[DEBUG] 回答已完成");
    }

    recordLog("[DEBUG] 问答流程完成，performQuestionAnswer函数执行结束");
    return true;
    } catch (const std::exception& e) {
        recordLog(QString("[ERROR] 执行问答流程时发生异常: %1").arg(e.what()));
        emit errorMessage(QString("执行问答流程时发生异常: %1").arg(e.what()));
        return false;
    } catch (...) {
        recordLog("[ERROR] 执行问答流程时发生未知异常");
        emit errorMessage("执行问答流程时发生未知异常");
        return false;
    }
}

bool Automator::waitForAnswerCompletion(HWND hwnd)
{
    recordLog("[DEBUG] 开始执行waitForAnswerCompletion函数");
    int answerTimeout = m_configManager->getAnswerTimeout();
    recordLog(QString("[DEBUG] 等待回答完成（固定 %1 秒）").arg(answerTimeout));

    // 使用传入的窗口句柄，不重新获取
    recordLog(QString("[DEBUG] 使用传入的企业微信窗口句柄: %1").arg((quintptr)hwnd, 0, 16));

    // 使用新的等待函数，支持ESC按键检测
    bool result = waitWithESCDetection(answerTimeout * 1000, hwnd);
    
    if (result) {
        recordLog("[DEBUG] 回答已完成");
    }
    
    recordLog("[DEBUG] waitForAnswerCompletion函数执行完成");
    return result;
}

bool Automator::waitWithESCDetection(int delayMs, HWND weChatHwnd)
{
    recordLog(QString("[DEBUG] 开始执行waitWithESCDetection函数，等待时间: %1 毫秒").arg(delayMs));
    
    // 获取WeBot窗口句柄（用于接收ESC按键）
    HWND weBotHwnd = FindWindowW(L"WeBotWindowClass", nullptr);
    if (!weBotHwnd) {
        // 如果找不到WeBot窗口，尝试通过窗口标题查找
        weBotHwnd = FindWindowW(nullptr, L"WeBot");
    }
    recordLog(QString("[DEBUG] WeBot窗口句柄: %1").arg((quintptr)weBotHwnd, 0, 16));
    
    // 分小段等待，以便及时响应停止请求
    const int checkInterval = 100; // 每100毫秒检查一次
    int elapsedTime = 0;
    
    while (elapsedTime < delayMs && !m_stopRequested) {
        // 在等待期间，将焦点放在WeBot窗口上，但不置顶显示
        if (weBotHwnd) {
            // 使用AttachThreadInput和SetFocus，避免置顶
            DWORD foregroundThreadId = GetWindowThreadProcessId(GetForegroundWindow(), nullptr);
            DWORD currentThreadId = GetCurrentThreadId();
            
            if (foregroundThreadId != currentThreadId) {
                AttachThreadInput(currentThreadId, foregroundThreadId, TRUE);
                SetFocus(weBotHwnd);
                AttachThreadInput(currentThreadId, foregroundThreadId, FALSE);
            } else {
                SetFocus(weBotHwnd);
            }
        }
        
        // 等待检查间隔，期间处理事件
        QThread::msleep(checkInterval);
        QCoreApplication::processEvents();
        elapsedTime += checkInterval;
    }
    
    // 等待时间到了，将焦点切换回企业微信窗口（如果提供了窗口句柄）
    if (weChatHwnd) {
        SetForegroundWindow(weChatHwnd);
        recordLog("[DEBUG] 将焦点切换回企业微信窗口");
        QThread::msleep(100); // 短暂延时，确保焦点切换完成
    } else if (weBotHwnd) {
        // 如果没有提供企业微信窗口句柄，则将焦点切换回WeBot窗口
        SetForegroundWindow(weBotHwnd);
        recordLog("[DEBUG] 将焦点切换回WeBot窗口");
        QThread::msleep(100); // 短暂延时，确保焦点切换完成
    }
    
    // 检查是否请求停止
    if (m_stopRequested) {
        recordLog("[INFO] 等待过程中收到停止请求");
        return false;
    }
    
    recordLog("[DEBUG] waitWithESCDetection函数执行完成，返回true");
    return true;
}

void Automator::recordLog(const QString& message)
{
    // 日志去重逻辑实现
    // 1. 检查是否是与上次相同的日志
    if (message == m_lastLogMessage) {
        // 完全相同的日志，不输出
        return;
    }
    
    // 2. 检查是否是最近1秒内的相同日志
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (m_logMessageHistory.contains(message)) {
        qint64 lastLogTime = m_logMessageHistory.value(message);
        if (currentTime - lastLogTime < 1000) {
            // 最近1秒内已记录过相同日志，不输出
            return;
        }
    }
    
    // 3. 更新日志历史
    m_logMessageHistory[message] = currentTime;
    m_lastLogMessage = message;
    m_lastLogTime = currentTime;
    
    // 4. 清理超过1秒的旧日志
    QStringList keysToRemove;
    for (auto it = m_logMessageHistory.constBegin(); it != m_logMessageHistory.constEnd(); ++it) {
        if (currentTime - it.value() > 1000) {
            keysToRemove.append(it.key());
        }
    }
    
    for (const QString& key : keysToRemove) {
        m_logMessageHistory.remove(key);
    }
    
    // 4. 输出日志
    emit logMessage(message);
}

void Automator::setState(State state)
{
    m_state = state;
    emit stateChanged(state);
}

void Automator::nonBlockingDelay(int delayMs)
{
    if (delayMs <= 0) {
        return;
    }
    
    // 创建事件循环和定时器，实现非阻塞延时
    QEventLoop loop;
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(delayMs);
    loop.exec();
}

