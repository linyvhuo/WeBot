#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logger.h"
#include "configmanager.h"
#include "automator.h"
#include "clickcapturewidget.h"

#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QPropertyAnimation>
#include <QTabWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QStyleFactory>
#include <QStyle>
#include <QStandardPaths>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , automator(new Automator(this))
    , recognitionOverlay(nullptr)
{
    // 安装事件过滤器，拦截样式表解析错误
    qApp->installEventFilter(this);
    
    ui->setupUi(this);
    
    // 设置窗口为可接收键盘事件
    this->setFocusPolicy(Qt::StrongFocus);
    this->activateWindow();
    this->setFocus();

    // 直接处理每个按钮，避免使用QList和findChildren，解决Qt版本兼容性问题
    ui->browseWechatButton->setStyleSheet("");
    ui->browseQuestionsButton->setStyleSheet("");
    ui->startButton->setStyleSheet("");
    ui->stopButton->setStyleSheet("");
    ui->saveConfigButton->setStyleSheet("");
    ui->loadConfigButton->setStyleSheet("");
    ui->exportLogButton->setStyleSheet("");
    ui->clearLogButton->setStyleSheet("");
    ui->browseIconButton->setStyleSheet("");
    ui->saveIconButton->setStyleSheet("");
    ui->resetIconButton->setStyleSheet("");
    
    // 重新设置按钮的文本，确保按钮显示正确
    ui->browseWechatButton->setText("浏览...");
    ui->browseQuestionsButton->setText("浏览...");
    ui->startButton->setText("🚀 开始自动问答");
    ui->stopButton->setText("⏹️ 停止执行");
    ui->saveConfigButton->setText("💾 保存配置");
    ui->loadConfigButton->setText("📂 加载配置");
    ui->exportLogButton->setText("📤 导出日志");
    ui->clearLogButton->setText("🗑️ 清空日志");
    ui->browseIconButton->setText("浏览...");
    ui->saveIconButton->setText("💾 保存图标配置");
    ui->resetIconButton->setText("🔄 重置为默认");
    
    // 重新设置按钮的大小策略，确保按钮大小合适
    ui->browseWechatButton->setMinimumSize(80, 25);
    ui->browseQuestionsButton->setMinimumSize(80, 25);
    ui->startButton->setMinimumSize(150, 40);
    ui->stopButton->setMinimumSize(120, 40);
    ui->saveConfigButton->setMinimumSize(100, 35);
    ui->loadConfigButton->setMinimumSize(100, 35);
    ui->exportLogButton->setMinimumSize(120, 35);
    ui->clearLogButton->setMinimumSize(120, 35);
    ui->browseIconButton->setMinimumSize(80, 25);
    ui->saveIconButton->setMinimumSize(120, 35);
    ui->resetIconButton->setMinimumSize(120, 35);

    try {
        // 先初始化日志系统
        Logger::getInstance();
        
        // 初始化配置管理器
        ConfigManager* config = ConfigManager::getInstance();

        // 连接ConfigManager信号槽（必须在初始化之前连接）
        connect(config, &ConfigManager::logMessage, this, [this](const QString& logEntry) {
            this->addLogEntry(logEntry);
        });
        
        // 延迟初始化ConfigManager，确保信号已经连接
        config->initialize();
        
        // 设置日志路径
        QString logFolderPath = config->getLogPath();
        Logger::getInstance()->setLogPath(logFolderPath);
        LOG_INFO(QString("日志文件路径: %1").arg(logFolderPath));
        
        // 在UI上显示日志保存位置
        ui->logSavePathLabel->setText(QString("日志保存位置: %1").arg(logFolderPath));

        // 删除识别区域覆盖层功能
    

    
    // 连接信号槽
    connect(automator, &Automator::logMessage, this, [this](const QString& logEntry) {
        this->addLogEntry(logEntry);
    });
        connect(automator, &Automator::progressUpdated, this, &MainWindow::updateProgress);
        connect(automator, &Automator::stateChanged, this, &MainWindow::onAutomationStateChanged);
        connect(automator, &Automator::automationCompleted, this, &MainWindow::onAutomationCompleted);
        connect(automator, &Automator::errorMessage, this, &MainWindow::showErrorMessage);
        
        // 连接标签页切换信号
        connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);

        // 加载配置到UI
        loadConfigToUI();

        // 初始化UI状态
        updateUIState(false);

        // 设置科技风样式表
    QString techStyle = R"(
        /* 主窗口背景 */
        QMainWindow {
            background-color: #1a1a2e;
            color: #ffffff;
            border: 1px solid #00ffff;
        }
        
        /* 自定义标题栏样式 */
        QMainWindow::title {
            background-color: #0f3460;
            color: #00ffff;
        }
        
        /* 窗口按钮样式 */
        QPushButton[window="true"] {
            background-color: #0f3460;
            color: #ffffff;
            border: 1px solid #00ffff;
            border-radius: 3px;
        }
        
        QPushButton[window="true"]:hover {
            background-color: #00ffff;
            color: #1a1a2e;
        }
        
        /* 中央部件 */
        QWidget {
            background-color: #1a1a2e;
            color: #ffffff;
        }
        
        /* 标签页 */
        QTabWidget {
            background-color: #16213e;
            border: none;
        }
        
        QTabBar {
            background-color: #0f3460;
            color: #ffffff;
            border-bottom: 2px solid #16213e;
        }
        
        QTabBar::tab {
            background-color: #0f3460;
            color: #ffffff;
            padding: 8px 20px;
            border: none;
            border-radius: 4px 4px 0 0;
            margin-right: 2px;
        }
        
        QTabBar::tab:selected {
            background-color: #16213e;
            color: #00ffff;
            border-bottom: 2px solid #00ffff;
        }
        
        /* 按钮样式 */
        QPushButton {
            background-color: #0f3460;
            color: #ffffff;
            border: 1px solid #00ffff;
            border-radius: 4px;
            padding: 6px 12px;
            font-size: 10pt;
            font-weight: bold;
        }
        
        QPushButton:hover {
            background-color: #16213e;
            color: #00ffff;
            border: 1px solid #00ffff;
        }
        
        QPushButton:pressed {
            background-color: #00ffff;
            color: #1a1a2e;
            transform: translateY(1px);
        }
        
        /* 输入框样式 */
        QLineEdit {
            background-color: #0f3460;
            color: #ffffff;
            border: 1px solid #00ffff;
            border-radius: 4px;
            padding: 6px;
        }
        
        QLineEdit:focus {
            border: 2px solid #00ffff;
            background-color: #16213e;
        }
        
        /* 文本编辑框 */
        QTextEdit {
            background-color: #0f3460;
            color: #ffffff;
            border: 1px solid #00ffff;
            border-radius: 4px;
            padding: 6px;
        }
        
        /* 标签样式 */
        QLabel {
            color: #ffffff;
            font-size: 9pt;
        }
        
        /* 进度条样式 */
        QProgressBar {
            background-color: #0f3460;
            color: #ffffff;
            border: 1px solid #00ffff;
            border-radius: 4px;
            text-align: center;
        }
        
        QProgressBar::chunk {
            background-color: #00ffff;
            border-radius: 4px;
        }
        
        /* 滑动条样式 */
        QSlider {
            background-color: #0f3460;
        }
        
        QSlider::groove:horizontal {
            background-color: #0f3460;
            border-radius: 4px;
            height: 6px;
        }
        
        QSlider::handle:horizontal {
            background-color: #00ffff;
            border: 1px solid #0f3460;
            border-radius: 8px;
            width: 16px;
            height: 16px;
            margin: -5px 0;
        }
        
        /* 旋转框样式 */
        QSpinBox, QDoubleSpinBox {
            background-color: #0f3460;
            color: #ffffff;
            border: 1px solid #00ffff;
            border-radius: 4px;
            padding: 4px;
        }
        
        /* 复选框样式 */
        QCheckBox {
            color: #ffffff;
        }
        
        QCheckBox::indicator {
            width: 16px;
            height: 16px;
            border: 1px solid #00ffff;
            border-radius: 4px;
            background-color: #0f3460;
        }
        
        QCheckBox::indicator:checked {
            background-color: #00ffff;
        }
        
        /* 组合框样式 */
        QComboBox {
            background-color: #0f3460;
            color: #ffffff;
            border: 1px solid #00ffff;
            border-radius: 4px;
            padding: 4px;
        }
        
        QComboBox::drop-down {
            border: none;
            background-color: #00ffff;
            color: #1a1a2e;
            border-radius: 0 4px 4px 0;
        }
        
        /* 分组框样式 */
        QGroupBox {
            background-color: #16213e;
            color: #00ffff;
            border: 1px solid #00ffff;
            border-radius: 4px;
            margin-top: 10px;
            padding-top: 10px;
            padding-left: 10px;
            padding-right: 10px;
            padding-bottom: 10px;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            left: -10px;
            margin-top: 0px;
            padding-left: 10px;
            padding-right: 10px;
            padding-top: 0px;
            padding-bottom: 0px;
            background-color: #16213e;
            border-radius: 4px;
            font-size: 10pt;
            font-weight: bold;
            height: 10px;
            color: #00ffff;
        }
        
        /* 进度条样式 */
            padding: 0 5px;
            background-color: transparent;
            color: #00ffff;
            font-weight: bold;
        }
        
        /* 状态栏样式 */
        QStatusBar {
            background-color: #0f3460;
            color: #ffffff;
            border-top: 1px solid #00ffff;
        }
    )";
    
    // 设置科技风样式表
    setStyleSheet(techStyle);
    
} catch (const std::exception& e) {
    QString errorMsg = QString("初始化时发生异常: %1").arg(e.what());
    LOG_CRITICAL(errorMsg);
    QMessageBox::critical(this, "错误", errorMsg);
} catch (...) {
    LOG_CRITICAL("初始化时发生未知异常");
    QMessageBox::critical(this, "错误", "初始化时发生未知异常");
}
}

MainWindow::~MainWindow() {
    // 停止自动化
    if (automator) {
        automator->stop();
    }
    

    
    // 关闭日志系统
    Logger::close();
    
    // 删除识别区域覆盖层
    delete recognitionOverlay;
    
    // 删除automator对象
    delete automator;
    
    // 删除UI对象
    delete ui;
}

bool MainWindow::loadConfigToUI() {
    try {
        ConfigManager* config = ConfigManager::getInstance();

        // 加载企业微信路径
    ui->wechatPathEdit->setText(config->getWeChatPath());
    
    // 加载问题库路径
    ui->questionPathEdit->setText(config->getQuestionLibraryPath());

        // 加载问答设置
        ui->loopCountSpin->setValue(config->getLoopCount()); // 从配置加载循环次数
        ui->timeoutSpin->setValue(config->getAnswerTimeout());
        ui->delaySpin->setValue(config->getDelayBetweenRounds());
        ui->promptEdit->setText(config->getAnswerLimitPrompt());

        // 加载高级设置
        ui->thresholdSpin->setValue(config->getImageRecognitionThreshold());
        ui->attemptsSpin->setValue(config->getMaxRecognitionAttempts());
        ui->pageTimeoutSpin->setValue(config->getPageLoadTimeout());
        // 加载识别超时设置
        ui->recognitionTimeoutSpin->setValue(config->getRecognitionTimeout());
        // 识别技术已固定为NCC，无需设置

        // 加载多显示器设置
        ui->multiMonitorCheck->setChecked(config->getMultiMonitorSupport());
        ui->primaryMonitorSpin->setValue(config->getPrimaryMonitorIndex());
        
        // 加载图像识别开关设置
        // 删除了识别方式设置，默认使用图像识别

        // 加载选项设置
        ui->topMostCheck->setChecked(config->getWindowTopMost());
        ui->continueOnErrorCheck->setChecked(config->getContinueOnError());
        ui->continueOnTimeoutCheck->setChecked(config->getContinueOnTimeout());


        // 加载问题模式
    ui->questionModeCombo->clear(); // 先清除现有选项，避免重复
    ui->questionModeCombo->addItem("循环使用预设问题", 0); // Automator::CycleMode
    ui->questionModeCombo->addItem("随机使用预设问题", 1); // Automator::RandomMode
    ui->questionModeCombo->addItem("自动生成问题", 2);   // Automator::GenerateMode
    ui->questionModeCombo->setCurrentIndex(config->getQuestionMode());
    
    // 加载输入方式设置
    int inputMethod = config->getInputMethod();
    if (inputMethod == 0) {
        ui->keyboardInputRadio->setChecked(true);
    } else {
        ui->pasteInputRadio->setChecked(true);
    }

    // 初始化图标管理界面
    // 确保图标名称下拉框有正确的选项
    QStringList iconOptions = {
        "workbench - 工作台图标",
        "mindspark_small - MindSpark标题栏图标",
        "mindspark - MindSpark图标",
        "input_box - 输入框图标",
        "send_button - 发送按钮图标",
        "history_dialog - 历史对话图标"
    };
    
    // 清除现有选项并添加新选项
    ui->iconNameCombo->clear();
    ui->iconNameCombo->addItems(iconOptions);
    
    // 初始化图标管理界面
    on_iconNameCombo_currentIndexChanged(ui->iconNameCombo->currentIndex());

    // 更新进度条，使用配置文件中的循环次数作为总数
    updateProgress(0, ui->loopCountSpin->value());
    
    addLogEntry("配置已加载到界面");
    return true;
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "错误", QString("加载配置到界面时发生异常: %1").arg(e.what()));
        addLogEntry(QString("加载配置到界面时发生异常: %1").arg(e.what()));
        return false;
    } catch (...) {
        QMessageBox::critical(this, "错误", "加载配置到界面时发生未知异常");
        addLogEntry("加载配置到界面时发生未知异常");
        return false;
    }
}

bool MainWindow::saveConfigFromUI() {
    try {
        ConfigManager* config = ConfigManager::getInstance();

        // 保存企业微信路径
    config->setWeChatPath(ui->wechatPathEdit->text());
    
    // 保存问题库路径
    config->setQuestionLibraryPath(ui->questionPathEdit->text());

        // 保存问答设置
        config->setAnswerTimeout(ui->timeoutSpin->value());
        config->setDelayBetweenRounds(ui->delaySpin->value());
        config->setLoopCount(ui->loopCountSpin->value());
        config->setAnswerLimitPrompt(ui->promptEdit->text());

        // 保存选项设置
        config->setWindowTopMost(ui->topMostCheck->isChecked());
        config->setContinueOnError(ui->continueOnErrorCheck->isChecked());
        config->setContinueOnTimeout(ui->continueOnTimeoutCheck->isChecked());
        
        // 保存问题模式设置
        config->setQuestionMode(ui->questionModeCombo->currentIndex());
        
        // 保存输入方式设置
        int inputMethod = ui->keyboardInputRadio->isChecked() ? 0 : 1;
        config->setInputMethod(inputMethod);

        // 保存高级设置
        config->setImageRecognitionThreshold(ui->thresholdSpin->value());
        config->setMaxRecognitionAttempts(ui->attemptsSpin->value());
        config->setPageLoadTimeout(ui->pageTimeoutSpin->value());
        // 保存识别超时设置
        config->setRecognitionTimeout(ui->recognitionTimeoutSpin->value());
        // 保存识别技术设置（固定为NCC）
        config->setRecognitionTechnique("NCC");

        // 保存多显示器设置
        config->setMultiMonitorSupport(ui->multiMonitorCheck->isChecked());
        
        // 计算并保存所有识别图标的尺寸到配置文件
        addLogEntry("开始计算并保存识别图标尺寸");
        QMap<QString, QString> iconPaths = config->getAllIconPaths();
        for (auto it = iconPaths.constBegin(); it != iconPaths.constEnd(); ++it) {
            QString iconName = it.key();
            QString iconPath = it.value();
            
            // 加载图标文件，计算尺寸
            QImage iconImage(iconPath);
            if (!iconImage.isNull()) {
                QSize iconSize = iconImage.size();
                config->setTemplateSize(iconName, iconSize);
                addLogEntry(QString("已保存图标 %1 的尺寸: %2x%3").arg(iconName).arg(iconSize.width()).arg(iconSize.height()));
            } else {
                addLogEntry(QString("警告: 无法加载图标 %1，路径: %2").arg(iconName).arg(iconPath));
            }
        }
        addLogEntry("识别图标尺寸计算并保存完成");
        config->setPrimaryMonitorIndex(ui->primaryMonitorSpin->value());
        
        // 删除了识别方式设置，默认使用图像识别
        config->setUseImageRecognition(true);

        // 保存配置
        if (!config->saveConfig()) {
            addLogEntry("保存配置到文件失败");
            return false;
        }
        
        // 显示配置文件路径
        QString configPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/WeBot/config.ini";
        addLogEntry(QString("配置已保存到: %1").arg(configPath));
        
        // 重新加载配置到UI，确保显示的是最新值
        loadConfigToUI();
        
        // 更新进度条，使用配置文件中的循环次数作为总数
        updateProgress(0, ui->loopCountSpin->value());
        
        return true;
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "错误", QString("保存配置时发生异常: %1").arg(e.what()));
        addLogEntry(QString("保存配置时发生异常: %1").arg(e.what()));
        return false;
    } catch (...) {
        QMessageBox::critical(this, "错误", "保存配置时发生未知异常");
        addLogEntry("保存配置时发生未知异常");
        return false;
    }
}

void MainWindow::updateUIState(bool isRunning) {
    // 根据运行状态更新UI控件的可用性
    ui->startButton->setEnabled(!isRunning);
    ui->stopButton->setEnabled(true); // 停止按钮始终可用
    ui->wechatPathEdit->setEnabled(!isRunning);
    ui->browseWechatButton->setEnabled(!isRunning);
    ui->loopCountSpin->setEnabled(!isRunning);
    ui->timeoutSpin->setEnabled(!isRunning);
    ui->delaySpin->setEnabled(!isRunning);
    ui->promptEdit->setEnabled(!isRunning);
    ui->topMostCheck->setEnabled(!isRunning);
    ui->continueOnErrorCheck->setEnabled(!isRunning);
    ui->continueOnTimeoutCheck->setEnabled(!isRunning);
    ui->questionModeCombo->setEnabled(!isRunning);
    ui->saveConfigButton->setEnabled(!isRunning);
    ui->exportLogButton->setEnabled(!isRunning);
}

void MainWindow::addLogEntry(const QString &logEntry, bool logToFile) {
    // 添加日志到界面
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");
    ui->logTextEdit->append(QString("[%1] %2").arg(timestamp, logEntry));

    // 同时记录到日志文件（默认记录）
    if (logToFile) {
        Logger::log(Logger::Info, logEntry);
    }
}

// 重载版本，默认记录到日志文件
void MainWindow::addLogEntry(const QString &logEntry) {
    addLogEntry(logEntry, true);
}

void MainWindow::updateProgress(int current, int total) {
    // 更新进度条
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(current);

    // 更新进度文本
    ui->progressLabel->setText(QString("进度: %1/%2").arg(current).arg(total));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    // 处理ESC按键，停止自动化
    if (event->key() == Qt::Key_Escape) {
        if (automator->getCurrentState() == Automator::Running) {
            addLogEntry("检测到ESC按键，停止自动化");
            automator->stop();           
        }
    }
    
    // 调用父类的按键事件处理函数
    QMainWindow::keyPressEvent(event);
}

void MainWindow::on_startButton_clicked() {
    try {
        // 保存当前配置
        if (!saveConfigFromUI()) {
            QMessageBox::warning(this, "配置错误", "保存配置失败，请检查配置");
            return;
        }

        // 设置问题模式
        int mode = ui->questionModeCombo->currentData().toInt();
        automator->setQuestionMode((Automator::QuestionMode)mode);

        // 获取循环次数
        int count = ui->loopCountSpin->value();
        if (count <= 0) {
            QMessageBox::warning(this, "参数错误", "循环次数必须大于0");
            return;
        }

        // 检查企业微信路径
        QString wechatPath = ui->wechatPathEdit->text();
        if (wechatPath.isEmpty()) {
            QMessageBox::warning(this, "参数错误", "请选择企业微信路径");
            return;
        }

        // 开始自动化
        LOG_INFO(QString("开始自动问答，共 %1 次").arg(count));
        addLogEntry(QString("开始自动问答，共 %1 次").arg(count));
        
        if (!automator->start(count)) {
            LOG_ERROR("自动问答启动失败");
            // 移除不必要的警告提示
            updateUIState(false);
            return;
        }
        
        updateUIState(true);
        LOG_INFO("自动问答启动成功");
        addLogEntry("提示：按下ESC键可停止自动化");
    } catch (const std::exception& e) {
        QString errorMsg = QString("启动时发生异常: %1").arg(e.what());
        LOG_CRITICAL(errorMsg);
        QMessageBox::critical(this, "错误", errorMsg);
        addLogEntry(errorMsg);
        updateUIState(false);
    } catch (...) {
        LOG_CRITICAL("启动时发生未知异常");
        QMessageBox::critical(this, "错误", "启动时发生未知异常");
        addLogEntry("启动时发生未知异常");
        updateUIState(false);
    }
}

void MainWindow::on_stopButton_clicked() {
    try {
        // 停止自动化
        addLogEntry("正在停止自动问答...");      
        // 调用automator->stop()停止自动化
        automator->stop();
        
        // 不要直接调用updateUIState(false)，而是等待automationCompleted信号
        // 这样可以确保自动化过程真正停止后再更新UI状态
    } catch (const std::exception& e) {
        QString errorMsg = QString("停止自动化时发生异常: %1").arg(e.what());
        addLogEntry(errorMsg);
        LOG_CRITICAL(errorMsg);
        // 确保UI状态正确
        updateUIState(false);
    } catch (...) {
        addLogEntry("停止自动化时发生未知异常");
        LOG_CRITICAL("停止自动化时发生未知异常");
        // 确保UI状态正确
        updateUIState(false);
    }
}



void MainWindow::on_browseWechatButton_clicked() {
    // 浏览企业微信路径
    QString path = QFileDialog::getOpenFileName(this, "选择微信路径");
    if (!path.isEmpty()) {
        ui->wechatPathEdit->setText(path);
    }
}

void MainWindow::on_saveConfigButton_clicked() {
    try {
        // 保存配置
        QApplication::setOverrideCursor(Qt::WaitCursor);
        if (saveConfigFromUI()) {
            QApplication::restoreOverrideCursor();
            // 移除成功提示
        } else {
            QApplication::restoreOverrideCursor();
            // 移除失败警告
        }
    } catch (const std::exception& e) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "错误", QString("保存配置时发生异常: %1").arg(e.what()));
        addLogEntry(QString("保存配置时发生异常: %1").arg(e.what()));
    } catch (...) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "错误", "保存配置时发生未知异常");
        addLogEntry("保存配置时发生未知异常");
    }
}

void MainWindow::on_exportLogButton_clicked() {
    try {
        // 导出日志
        QString path = QFileDialog::getSaveFileName(this, "导出日志", "", "日志文件 (*.log);;所有文件 (*.*)");
        if (path.isEmpty()) {
            return; // 用户取消了选择
        }
        
        QApplication::setOverrideCursor(Qt::WaitCursor);
        if (Logger::exportLog(path)) {
            QApplication::restoreOverrideCursor();
            addLogEntry("日志已导出到: " + path);
        } else {
            QApplication::restoreOverrideCursor();
            addLogEntry("日志导出失败: " + path);
        }
    } catch (const std::exception& e) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "错误", QString("导出日志时发生异常: %1").arg(e.what()));
        addLogEntry(QString("导出日志时发生异常: %1").arg(e.what()));
    } catch (...) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "错误", "导出日志时发生未知异常");
        addLogEntry("导出日志时发生未知异常");
    }
}

QString MainWindow::getLogContent() {
    return Logger::getLogContent();
}

void MainWindow::onAutomationStateChanged(Automator::State state) {
    // 处理自动化状态变化
    QString stateStr;
    switch (state) {
    case Automator::Idle: stateStr = "空闲"; break;
    case Automator::Starting: stateStr = "启动中"; break;
    case Automator::Running: stateStr = "运行中"; break;
    case Automator::Completed: stateStr = "已完成"; break;
    case Automator::Error: stateStr = "错误"; break;
    default: stateStr = "未知状态"; break;
    }

    ui->statusLabel->setText("状态: " + stateStr);
    addLogEntry("状态变更为: " + stateStr);
}

void MainWindow::onAutomationCompleted() {
    // 自动化完成
    updateUIState(false);
    
    addLogEntry("自动问答已完成");
    // 移除模态对话框，避免导致崩溃
    QMessageBox::information(this, "完成", "自动问答已完成");
}

void MainWindow::on_browseQuestionsButton_clicked() {
    // 浏览问题库路径，支持CSV和TXT文件
    QString path = QFileDialog::getOpenFileName(this, "选择问题库路径", "", "问题库文件 (*.txt *.csv);;所有文件 (*.*)");
    if (!path.isEmpty()) {
        ui->questionPathEdit->setText(path);
        
        // 显示预览窗口，无论文件类型
        addLogEntry(QString("正在预览问题库文件: %1").arg(path));
        
        QStringList previewLines;
        bool isCSV = path.endsWith(".csv", Qt::CaseInsensitive);
        
        if (isCSV) {
            // 调用ConfigManager的previewCSVFile方法获取预览内容
            ConfigManager* config = ConfigManager::getInstance();
            previewLines = config->previewCSVFile(path);
        } else {
            // 处理TXT文件
            QFile file(path);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                int maxLines = 200; // 最多显示50行
                int lineCount = 0;
                
                while (!in.atEnd() && lineCount < maxLines) {
                    QString line = in.readLine().trimmed();
                    if (!line.isEmpty()) {
                        previewLines.append(line);
                        lineCount++;
                    }
                }
                file.close();
            } else {
                previewLines.append("无法打开TXT文件: " + path);
            }
        }
        
        // 创建预览窗口
        QDialog previewDialog(this);
        previewDialog.setWindowTitle(QString("%1文件预览: %2").arg(isCSV ? "CSV" : "TXT").arg(QFileInfo(path).fileName()));
        previewDialog.resize(600, 400);
        
        // 设置布局
        QVBoxLayout* layout = new QVBoxLayout(&previewDialog);
        
        // 创建文本编辑框显示预览内容
        QTextEdit* previewTextEdit = new QTextEdit(&previewDialog);
        previewTextEdit->setReadOnly(true);
        previewTextEdit->setFont(QFont("Consolas", 9));
        
        // 设置预览内容，对所有文件类型的问题进行编号
        QString previewContent;
        if (isCSV) {
            if (!previewLines.isEmpty()) {
                // 显示CSV标题行
                previewContent += previewLines[0] + "\n";
                previewContent += "-------------------------------\n";
                
                // 对CSV数据行进行编号
                for (int i = 1; i < previewLines.size(); i++) {
                    previewContent += QString("%1. %2\n").arg(i).arg(previewLines[i]);
                }
            }
        } else {
            // 对TXT文件的问题进行编号
            for (int i = 0; i < previewLines.size(); i++) {
                previewContent += QString("%1. %2\n").arg(i + 1).arg(previewLines[i]);
            }
        }
        previewTextEdit->setPlainText(previewContent);
        
        // 创建按钮布局
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        QPushButton* okButton = new QPushButton("确定", &previewDialog);
        QPushButton* cancelButton = new QPushButton("取消", &previewDialog);
        
        buttonLayout->addStretch();
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        
        // 添加控件到布局
        layout->addWidget(previewTextEdit);
        layout->addLayout(buttonLayout);
        
        // 连接信号槽
        connect(okButton, &QPushButton::clicked, &previewDialog, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, &previewDialog, &QDialog::reject);
        
        // 显示预览窗口
        if (previewDialog.exec() != QDialog::Accepted) {
            // 如果用户取消，清空路径
            ui->questionPathEdit->clear();
            return;
        }
        
        // 立即保存路径到ConfigManager，确保下次启动时能正确加载
        ConfigManager* config = ConfigManager::getInstance();
        config->setQuestionLibraryPath(path);
        config->saveConfig();
        
        addLogEntry(QString("%1文件预览完成，共显示 %2 个问题").arg(isCSV ? "CSV" : "TXT").arg(previewLines.size()));
        addLogEntry("问题库路径已保存");
    }
}

void MainWindow::on_loadConfigButton_clicked() {
    try {
        // 加载配置
        QApplication::setOverrideCursor(Qt::WaitCursor);
        if (loadConfigToUI()) {
            QApplication::restoreOverrideCursor();
        } else {
            QApplication::restoreOverrideCursor();
        }
    } catch (const std::exception& e) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "错误", QString("加载配置时发生异常: %1").arg(e.what()));
        addLogEntry(QString("加载配置时发生异常: %1").arg(e.what()));
    } catch (...) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "错误", "加载配置时发生未知异常");
        addLogEntry("加载配置时发生未知异常");
    }
}

void MainWindow::on_clearLogButton_clicked() {
    // 清空日志显示
    ui->logTextEdit->clear();
    addLogEntry("日志显示已清空");
}

void MainWindow::on_thresholdSpin_valueChanged(double value) {
    // 图像识别阈值变化
    addLogEntry(QString("图像识别阈值已更改为: %1").arg(value));
}

void MainWindow::on_attemptsSpin_valueChanged(int value) {
    // 最大识别尝试次数变化
    addLogEntry(QString("最大识别尝试次数已更改为: %1").arg(value));
}

void MainWindow::on_pageTimeoutSpin_valueChanged(int value) {
    // 页面加载超时时间变化
    addLogEntry(QString("页面加载超时时间已更改为: %1 毫秒").arg(value));
}

void MainWindow::on_recognitionTimeoutSpin_valueChanged(int value) {
    // 识别超时时间变化
    addLogEntry(QString("识别超时时间已更改为: %1 毫秒").arg(value));
}

void MainWindow::on_recognitionTechniqueCombo_currentIndexChanged(int index) {
    // 识别技术变化
    QString technique;
    switch (index) {
    case 0:
        technique = "NCC - 归一化交叉相关匹配 (速度: 中等, 准确率: 高)";
        break;
    case 1:
        technique = "SSD - 平方差和匹配 (速度: 快, 准确率: 中等)";
        break;
    case 2:
        technique = "SAD - 绝对差和匹配 (速度: 非常快, 准确率: 中等)";
        break;
    case 3:
        technique = "ORB - 基于特征的匹配 (速度: 快, 准确率: 高)";
        break;
    default:
        technique = "NCC - 归一化交叉相关匹配 (速度: 中等, 准确率: 高)";
        break;
    }
    addLogEntry(QString("识别技术已更改为: %1").arg(technique));
}

void MainWindow::on_timeoutSpin_valueChanged(int value) {
    // 回答超时时间变化
    addLogEntry(QString("回答超时时间已更改为: %1 秒").arg(value));
}

void MainWindow::on_loopCountSpin_valueChanged(int value) {
    // 循环次数变化时更新进度条的最大值
    updateProgress(0, value);
    addLogEntry(QString("循环次数已更改为: %1").arg(value));
}

void MainWindow::on_saveAdvancedConfigButton_clicked() {
    try {
        // 保存高级配置
        QApplication::setOverrideCursor(Qt::WaitCursor);
        if (saveConfigFromUI()) {
            QApplication::restoreOverrideCursor();
        } else {
            QApplication::restoreOverrideCursor();
        }
    } catch (const std::exception& e) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "错误", QString("保存高级配置时发生异常: %1").arg(e.what()));
        addLogEntry(QString("保存高级配置时发生异常: %1").arg(e.what()));
    } catch (...) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "错误", "保存高级配置时发生未知异常");
        addLogEntry("保存高级配置时发生未知异常");
    }
}

void MainWindow::on_loadAdvancedConfigButton_clicked() {
    try {
        // 加载高级配置
        QApplication::setOverrideCursor(Qt::WaitCursor);
        if (loadConfigToUI()) {
            QApplication::restoreOverrideCursor();
            QMessageBox::information(this, "成功", "高级配置已加载");
        } else {
            QApplication::restoreOverrideCursor();
            QMessageBox::warning(this, "失败", "高级配置加载失败，请检查日志");
        }
    } catch (const std::exception& e) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "错误", QString("加载高级配置时发生异常: %1").arg(e.what()));
        addLogEntry(QString("加载高级配置时发生异常: %1").arg(e.what()));
    } catch (...) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, "错误", "加载高级配置时发生未知异常");
        addLogEntry("加载高级配置时发生未知异常");
    }
}

void MainWindow::on_multiMonitorCheck_toggled(bool checked) {
    // 多显示器支持开关变化
    addLogEntry(QString("多显示器支持已%1").arg(checked ? "启用" : "禁用"));
}

void MainWindow::on_primaryMonitorSpin_valueChanged(int value) {
    // 主显示器索引变化
    addLogEntry(QString("主显示器索引已更改为: %1").arg(value));
}

// 图标管理相关槽函数实现
void MainWindow::on_browseIconButton_clicked() {
    // 浏览图标文件，支持所有常见图片格式
    QString path = QFileDialog::getOpenFileName(this, "选择图标文件", "", "所有图像文件 (*.png *.jpg *.jpeg *.bmp *.svg *.gif *.tif *.tiff *.webp);;PNG文件 (*.png);;JPEG文件 (*.jpg *.jpeg);;BMP文件 (*.bmp);;SVG文件 (*.svg);;GIF文件 (*.gif);;TIFF文件 (*.tif *.tiff);;WebP文件 (*.webp)");
    if (!path.isEmpty()) {
        ui->iconPathEdit->setText(path);
        // 更新预览
        on_iconPathEdit_textChanged(path);
    }
}

void MainWindow::on_saveIconButton_clicked() {
    try {
        // 保存图标配置
        QString displayText = ui->iconNameCombo->currentText();
        // 提取实际图标名称（从"workbench - 工作台图标"中提取"workbench"）
        QString iconName = displayText.split(" - ").first().trimmed();
        QString iconPath = ui->iconPathEdit->text();
        
        if (iconPath.isEmpty()) {
            QMessageBox::warning(this, "配置错误", "请选择图标文件");
            return;
        }
        
        // 保存到配置
        ConfigManager::getInstance()->setIconPath(iconName, iconPath);
        
        // 保存配置文件
        if (ConfigManager::getInstance()->saveConfig()) {
            QMessageBox::information(this, "成功", "图标配置已保存");
            addLogEntry(QString("图标配置已保存: %1 -> %2").arg(iconName, iconPath));
        } else {
            QMessageBox::warning(this, "失败", "图标配置保存失败");
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "错误", QString("保存图标配置时发生异常: %1").arg(e.what()));
        addLogEntry(QString("保存图标配置时发生异常: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "错误", "保存图标配置时发生未知异常");
        addLogEntry("保存图标配置时发生未知异常");
    }
}

void MainWindow::on_resetIconButton_clicked() {
    try {
        // 重置为默认图标
        QString displayText = ui->iconNameCombo->currentText();
        // 提取实际图标名称（从"workbench - 工作台图标"中提取"workbench"）
        QString iconName = displayText.split(" - ").first().trimmed();
        QString defaultPath = QString(":/templates/%1.svg").arg(iconName);
        
        // 保存到配置
        ConfigManager::getInstance()->setIconPath(iconName, defaultPath);
        
        // 更新UI
        ui->iconPathEdit->setText(defaultPath);
        on_iconPathEdit_textChanged(defaultPath);
        
        // 保存配置文件
        if (ConfigManager::getInstance()->saveConfig()) {
            QMessageBox::information(this, "成功", "图标已重置为默认");
            addLogEntry(QString("图标已重置为默认: %1 -> %2").arg(iconName, defaultPath));
        } else {
            QMessageBox::warning(this, "失败", "图标重置失败");
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "错误", QString("重置图标时发生异常: %1").arg(e.what()));
        addLogEntry(QString("重置图标时发生异常: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "错误", "重置图标时发生未知异常");
        addLogEntry("重置图标时发生未知异常");
    }
}

void MainWindow::on_iconNameCombo_currentIndexChanged(int index) {
    // 获取当前选中的显示文本
    QString displayText = ui->iconNameCombo->itemText(index);
    // 提取实际图标名称（从"workbench - 工作台图标"中提取"workbench"）
    QString iconName = displayText.split(" - ").first().trimmed();
    // 图标名称变化时更新路径和预览
    QString iconPath = ConfigManager::getInstance()->getIconPath(iconName);
    addLogEntry(QString("切换图标: %1 -> %2").arg(iconName, iconPath));
    ui->iconPathEdit->setText(iconPath);
    
    // 更新当前图标预览
    updateIconPreview(iconPath, ui->currentIconPreviewLabel);
    // 清除选择的图标预览
    ui->selectedIconPreviewLabel->setText("选择的图标预览");
    ui->selectedIconPreviewLabel->setPixmap(QPixmap());
}

// 辅助函数：更新图标预览
void MainWindow::updateIconPreview(const QString &path, QLabel *previewLabel) {
    if (path.isEmpty()) {
        // 清空预览
        previewLabel->setText("图标预览");
        previewLabel->setPixmap(QPixmap());
        return;
    }
    
    // 加载图像并显示预览
    QPixmap pixmap;
    bool loaded = false;
    
    // 处理其他图像格式
    loaded = pixmap.load(path);
    
    if (loaded) {
        // 调整图像大小以适应预览区域
        QSize previewSize = previewLabel->size();
        QPixmap scaledPixmap = pixmap.scaled(previewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        previewLabel->setPixmap(scaledPixmap);
        previewLabel->setText("");
    } else {
        previewLabel->setText("无法加载图像");
        previewLabel->setPixmap(QPixmap());
    }
}





void MainWindow::on_iconPathEdit_textChanged(const QString &path) {
    // 图标路径变化时更新选择的图标预览
    updateIconPreview(path, ui->selectedIconPreviewLabel);
}

void MainWindow::onTabChanged(int index) {
    try {
        // 页面切换动画
        QWidget* currentWidget = ui->tabWidget->widget(index);
        
        if (currentWidget) {
            // 确保widget可见
            currentWidget->show();
            
            // 创建淡入动画
            QPropertyAnimation* fadeInAnimation = new QPropertyAnimation(currentWidget, "windowOpacity");
            fadeInAnimation->setDuration(500);
            fadeInAnimation->setStartValue(0.0);
            fadeInAnimation->setEndValue(1.0);
            fadeInAnimation->setEasingCurve(QEasingCurve::InOutQuad);
            fadeInAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    } catch (const std::exception& e) {
        LOG_CRITICAL(QString("onTabChanged异常: %1").arg(e.what()));
    } catch (...) {
        LOG_CRITICAL("onTabChanged发生未知异常");
    }
}

void MainWindow::showErrorMessage(const QString &message) {
    // 显示错误弹窗
    QMessageBox::critical(this, "自动化错误", message);
}

// 输入方式相关方法实现
MainWindow::InputMethod MainWindow::getCurrentInputMethod() const {
    ConfigManager* config = ConfigManager::getInstance();
    int method = config->getInputMethod();
    return (method == 0) ? KeyboardInput : PasteInput;
}

void MainWindow::setCurrentInputMethod(InputMethod method) {
    ConfigManager* config = ConfigManager::getInstance();
    config->setInputMethod(method == KeyboardInput ? 0 : 1);
    
    // 更新UI
    if (method == KeyboardInput) {
        ui->keyboardInputRadio->setChecked(true);
    } else {
        ui->pasteInputRadio->setChecked(true);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    // 继续处理其他事件
    return QMainWindow::eventFilter(obj, event);
}



