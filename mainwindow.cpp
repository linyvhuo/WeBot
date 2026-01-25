#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logger.h"
#include "configmanager.h"
#include "automator.h"
#include "clickcapturewidget.h"
#include "floatingwindow.h"
#include "thememanager.h"
#include "screenshotselector.h"

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
#include <QDesktopServices>
#include <QUrl>
#include <QScreen>
#include <QPixmap>
#include <QGuiApplication>
#include <QClipboard>
#include <QTextEdit>
#include <QResizeEvent>


MainWindow::MainWindow(QWidget *parent)
    : FramelessWindow(parent)
    , ui(new Ui::MainWindow)
    , automator(new Automator(this))
    , recognitionOverlay(nullptr)
    , floatingWindow(new FloatingWindow(this))
{
    // 安装事件过滤器，拦截样式表解析错误
    qApp->installEventFilter(this);
    
    ui->setupUi(this);
    
    // 设置窗口为可接收键盘事件
    this->setFocusPolicy(Qt::StrongFocus);
    this->activateWindow();
    this->setFocus();

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

    // 连接悬浮窗停止信号
    connect(floatingWindow, &FloatingWindow::stopRequested, this, &MainWindow::on_stopButton_clicked);

    // 连接主题变化信号
    ThemeManager *themeManager = ThemeManager::getInstance();
    connect(themeManager, &ThemeManager::themeChanged, floatingWindow, &FloatingWindow::onThemeChanged);

    // 连接主题选择下拉框
    connect(ui->themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_themeCombo_currentIndexChanged);

    // 连接关闭行为选择下拉框
    connect(ui->closeBehaviorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                ConfigManager* config = ConfigManager::getInstance();
                config->setCloseBehavior(index);
                config->saveConfig();
            });

    // 注册截图热键 (Ctrl+Shift+S)
    registerScreenshotHotkey();

    // 设置系统托盘
    setupSystemTray();

    // 加载配置到UI
    loadConfigToUI();

        // 初始化UI状态
        updateUIState(false);

        // 初始化主题选择下拉框
        ui->themeCombo->setCurrentIndex((int)ThemeManager::getInstance()->getCurrentTheme());

        // 初始化截图快捷键设置
        ConfigManager *configManager = ConfigManager::getInstance();
        ui->screenshotShortcutEdit->setKeySequence(QKeySequence(configManager->getScreenshotShortcut()));

        // 设置中央widget
        QWidget *centralContainer = new QWidget();
        centralContainer->setLayout(ui->mainLayout);
        setCentralWidget(centralContainer);

        // 应用当前主题（确保主题正确应用）
        ThemeManager::getInstance()->applyTheme(ThemeManager::getInstance()->getCurrentTheme());
    
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
    // 注销截图热键
    unregisterScreenshotHotkey();

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
    
    // 加载路径配置
    ui->configPathEdit->setText(config->getConfigFilePath());
    ui->questionPathEdit->setText(config->getQuestionFilePath());
    ui->keywordPathEdit->setText(config->getKeywordFilePath());
    ui->logPathEdit->setText(config->getLogFilePath());
    ui->templatePathEdit->setText(config->getTemplateFilePath());

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

        // 加载选项设置
        ui->topMostCheck->setChecked(config->getWindowTopMost());
        ui->continueOnErrorCheck->setChecked(config->getContinueOnError());
        ui->continueOnTimeoutCheck->setChecked(config->getContinueOnTimeout());

        // 加载悬浮窗设置
        ui->floatingWindowCheck->setChecked(config->getFloatingWindowVisible());

        // 加载问题模式
    ui->questionModeCombo->clear(); // 先清除现有选项，避免重复
    ui->questionModeCombo->addItem("循环使用预设问题", 0); // Automator::CycleMode
    ui->questionModeCombo->addItem("随机使用预设问题", 1); // Automator::RandomMode
    ui->questionModeCombo->addItem("自动生成问题", 2);   // Automator::GenerateMode
    ui->questionModeCombo->setCurrentIndex(config->getQuestionMode());
    
    // 加载输入方式设置
    int inputMethod = config->getInputMethod();
    if (inputMethod == 0) {
        ui->keyboardInputCheck->setChecked(true);
    } else {
        ui->pasteInputCheck->setChecked(true);
    }

    // 加载主题设置
    ui->themeCombo->setCurrentIndex(config->getTheme());

    // 加载关闭行为设置
    ui->closeBehaviorCombo->setCurrentIndex(config->getCloseBehavior());

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
    
    // 保存路径配置
    config->setConfigFilePath(ui->configPathEdit->text());
    config->setQuestionFilePath(ui->questionPathEdit->text());
    config->setKeywordFilePath(ui->keywordPathEdit->text());
    config->setLogFilePath(ui->logPathEdit->text());
    config->setTemplateFilePath(ui->templatePathEdit->text());

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
        int inputMethod = ui->keyboardInputCheck->isChecked() ? 0 : 1;
        config->setInputMethod(inputMethod);

        // 保存主题设置
        config->setTheme(ui->themeCombo->currentIndex());

        // 保存关闭行为设置
        config->setCloseBehavior(ui->closeBehaviorCombo->currentIndex());

        // 保存悬浮窗设置
        config->setFloatingWindowVisible(ui->floatingWindowCheck->isChecked());

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
        config->setPrimaryMonitorIndex(ui->primaryMonitorSpin->value());
        
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

    // 更新悬浮窗进度
    if (floatingWindow->isVisible()) {
        floatingWindow->updateProgress(current, total);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    // ESC键停止自动化
    if (event->key() == Qt::Key_Escape) {
        on_stopButton_clicked();
        return;
    }

    QWidget::keyPressEvent(event);
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

        // 显示悬浮窗
        floatingWindow->show();
        floatingWindow->setStatus("运行中");

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
    case Automator::Idle:
        stateStr = "空闲";
        floatingWindow->hide();
        break;
    case Automator::Starting: stateStr = "启动中"; break;
    case Automator::Running: stateStr = "运行中"; break;
    case Automator::Completed:
        stateStr = "已完成";
        floatingWindow->hide();
        break;
    case Automator::Error:
        stateStr = "错误";
        floatingWindow->hide();
        break;
    default: stateStr = "未知状态"; break;
    }

    ui->statusLabel->setText("状态: " + stateStr);
    if (floatingWindow->isVisible()) {
        floatingWindow->setStatus(stateStr);
    }
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
                int maxLines = 200; // 最多显示200行
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
        addLogEntry(QString("问题库路径已保存: %1").arg(path));
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

void MainWindow::on_configPathBrowseButton_clicked() {
    // 浏览配置文件路径
    QString path = QFileDialog::getOpenFileName(this, "选择配置文件", "", "配置文件 (*.ini);;所有文件 (*.*)");
    if (!path.isEmpty()) {
        ui->configPathEdit->setText(path);
        ConfigManager* config = ConfigManager::getInstance();
        config->setConfigFilePath(path);
        config->saveConfig();
        addLogEntry(QString("配置文件路径已设置: %1").arg(path));
    }
}

void MainWindow::on_questionPathBrowseButton_clicked() {
    // 浏览问题库路径
    QString path = QFileDialog::getOpenFileName(this, "选择问题库路径", "", "问题库文件 (*.txt *.csv);;所有文件 (*.*)");
    if (!path.isEmpty()) {
        ui->questionPathEdit->setText(path);
        ConfigManager* config = ConfigManager::getInstance();
        config->setQuestionFilePath(path);
        config->saveConfig();
        addLogEntry(QString("问题库路径已设置: %1").arg(path));
    }
}

void MainWindow::on_keywordPathBrowseButton_clicked() {
    // 浏览关键词库路径
    QString path = QFileDialog::getOpenFileName(this, "选择关键词库路径", "", "关键词库文件 (*.txt);;所有文件 (*.*)");
    if (!path.isEmpty()) {
        ui->keywordPathEdit->setText(path);
        ConfigManager* config = ConfigManager::getInstance();
        config->setKeywordFilePath(path);
        config->saveConfig();
        addLogEntry(QString("关键词库路径已设置: %1").arg(path));
    }
}

void MainWindow::on_logPathBrowseButton_clicked() {
    // 浏览日志文件路径
    QString path = QFileDialog::getExistingDirectory(this, "选择日志文件路径", "");
    if (!path.isEmpty()) {
        ui->logPathEdit->setText(path);
        ConfigManager* config = ConfigManager::getInstance();
        config->setLogFilePath(path + "/");
        config->saveConfig();
        addLogEntry(QString("日志文件路径已设置: %1").arg(path));
    }
}

void MainWindow::on_templatePathBrowseButton_clicked() {
    // 浏览图标模板路径
    QString path = QFileDialog::getExistingDirectory(this, "选择图标模板路径", "");
    if (!path.isEmpty()) {
        ui->templatePathEdit->setText(path);
        ConfigManager* config = ConfigManager::getInstance();
        config->setTemplateFilePath(path + "/");
        config->saveConfig();
        addLogEntry(QString("图标模板路径已设置: %1").arg(path));
    }
}

void MainWindow::on_resetAllPathsButton_clicked() {
    // 重置所有路径为默认值
    QMessageBox::StandardButton reply = QMessageBox::question(this, "确认重置", 
        "确定要重置所有路径为默认值吗？", 
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        ConfigManager* config = ConfigManager::getInstance();
        
        // 获取默认路径
        QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        QString weBotPath = documentsPath + "/WeBot";
        
        // 重置所有路径
        config->setConfigFilePath(weBotPath + "/config.ini");
        config->setQuestionFilePath(weBotPath + "/Questions.txt");
        config->setKeywordFilePath(weBotPath + "/Keywords.txt");
        config->setLogFilePath(weBotPath + "/logs/");
        config->setTemplateFilePath(weBotPath + "/templates/");
        
        // 保存配置
        config->saveConfig();
        
        // 更新UI显示
        loadConfigToUI();
        
        addLogEntry("所有路径已重置为默认值");
        QMessageBox::information(this, "重置完成", "所有路径已重置为默认值");
    }
}

void MainWindow::on_visitWebsiteButton_clicked() {
    // 打开官方网站
    QString websiteUrl = "https://github.com/linyvhuo/WeBot";
    
    try {
        QDesktopServices::openUrl(QUrl(websiteUrl));
        addLogEntry(QString("正在访问官网: %1").arg(websiteUrl));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "错误", QString("无法打开官网: %1").arg(e.what()));
        addLogEntry(QString("无法打开官网: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "错误", "无法打开官网");
        addLogEntry("无法打开官网");
    }
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
        ui->keyboardInputCheck->setChecked(true);
    } else {
        ui->pasteInputCheck->setChecked(true);
    }
}

void MainWindow::on_keyboardInputCheck_toggled(bool checked) {
    if (checked) {
        // 确保只有一个输入方式被选中
        if (ui->pasteInputCheck->isChecked()) {
            ui->pasteInputCheck->setChecked(false);
        }
        setCurrentInputMethod(KeyboardInput);
        addLogEntry("输入方式已切换为：模拟键盘逐个输入");
    }
}

void MainWindow::on_pasteInputCheck_toggled(bool checked) {
    if (checked) {
        // 确保只有一个输入方式被选中
        if (ui->keyboardInputCheck->isChecked()) {
            ui->keyboardInputCheck->setChecked(false);
        }
        setCurrentInputMethod(PasteInput);
        addLogEntry("输入方式已切换为：复制粘贴输入");
    }
}



bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    // 继续处理其他事件
    return QWidget::eventFilter(obj, event);
}

// 截图热键相关方法实现
void MainWindow::registerScreenshotHotkey() {
    // 注册全局热键 Ctrl+Shift+S (截图)
    if (!RegisterHotKey((HWND)winId(), 1, MOD_CONTROL | MOD_SHIFT, 'S')) {
        LOG_WARNING("注册截图热键失败");
        addLogEntry("警告：无法注册截图热键 (Ctrl+Shift+S)");
    } else {
        LOG_INFO("截图热键已注册: Ctrl+Shift+S");
        addLogEntry("截图热键已注册: Ctrl+Shift+S");
    }
}

void MainWindow::unregisterScreenshotHotkey() {
    // 注销全局热键
    UnregisterHotKey((HWND)winId(), 1);
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result) {
    Q_UNUSED(result);

    if (eventType == "windows_generic_MSG") {
        MSG *msg = static_cast<MSG *>(message);
        if (msg->message == WM_HOTKEY && msg->wParam == 1) {
            // 截图热键被触发
            takeScreenshot();
            return true;
        }
    }

    return false;
}

void MainWindow::takeScreenshot() {
    try {
        // 获取屏幕信息
        QScreen *screen = QGuiApplication::primaryScreen();
        if (!screen) {
            LOG_ERROR("无法获取屏幕信息");
            addLogEntry("截图失败：无法获取屏幕信息");
            return;
        }

        // 隐藏主窗口以便截图
        bool wasVisible = isVisible();
        if (wasVisible) {
            hide();
            QApplication::processEvents(); // 确保窗口隐藏
        }

        // 捕获屏幕截图
        QPixmap screenshot = screen->grabWindow(0);

        // 如果主窗口之前是可见的，恢复显示
        if (wasVisible) {
            show();
        }

        if (screenshot.isNull()) {
            LOG_ERROR("截图失败：无法捕获屏幕内容");
            addLogEntry("截图失败：无法捕获屏幕内容");
            return;
        }

        // 创建区域选择器
        if (screenshotSelector) {
            delete screenshotSelector;
        }

        screenshotSelector = new ScreenshotSelector(screenshot, this);

        // 连接信号
        connect(screenshotSelector, &ScreenshotSelector::regionSelected, this, [this](const QRect &rect) {
            // 先隐藏选择器
            if (screenshotSelector) {
                screenshotSelector->hide();
            }
            processSelectedScreenshot(rect);
        });

        connect(screenshotSelector, &ScreenshotSelector::cancelled, this, [this]() {
            addLogEntry("截图已取消");
            if (screenshotSelector) {
                screenshotSelector->deleteLater();
                screenshotSelector = nullptr;
            }
        });

        // 显示选择器（非模态）
        screenshotSelector->show();
        screenshotSelector->activateWindow();
        screenshotSelector->raise();

        LOG_INFO("显示截图区域选择器");
        addLogEntry("请拖拽鼠标选择要截图的区域，按ESC取消");

    } catch (const std::exception& e) {
        QString errorMsg = QString("截图时发生异常: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        addLogEntry(errorMsg);
    } catch (...) {
        LOG_ERROR("截图时发生未知异常");
        addLogEntry("截图时发生未知异常");
    }
}

void MainWindow::processSelectedScreenshot(const QRect &selectedRect) {
    try {
        if (selectedRect.isEmpty()) {
            addLogEntry("截图区域无效");
            return;
        }

        // 获取选择的截图区域
        QPixmap selectedScreenshot;

        // 从选择器获取原始截图，然后复制选择的区域
        if (screenshotSelector) {
            QPixmap fullScreenshot = screenshotSelector->getScreenshot();
            if (!fullScreenshot.isNull()) {
                selectedScreenshot = fullScreenshot.copy(selectedRect);
            }
        }

        // 如果获取失败，重新截取全屏
        if (selectedScreenshot.isNull()) {
            QScreen *screen = QGuiApplication::primaryScreen();
            if (screen) {
                QPixmap fullScreenShot = screen->grabWindow(0);
                selectedScreenshot = fullScreenShot.copy(selectedRect);
            }
        }

        if (selectedScreenshot.isNull()) {
            LOG_ERROR("无法获取选择的截图区域");
            addLogEntry("截图失败：无法获取选择的区域");
            return;
        }

        // 生成临时文件名用于预览
        QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        QString tempScreenshotPath = tempPath + "/webot_temp_screenshot_" +
                                    QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".png";

        // 临时保存用于预览
        if (selectedScreenshot.save(tempScreenshotPath, "PNG")) {
            LOG_INFO(QString("截图已捕获，尺寸: %1x%2")
                    .arg(selectedRect.width())
                    .arg(selectedRect.height()));
            addLogEntry(QString("截图已捕获，尺寸: %1x%2")
                       .arg(selectedRect.width())
                       .arg(selectedRect.height()));

            // 显示截图预览和操作窗口（不自动保存）
            showScreenshotPreview(selectedScreenshot, tempScreenshotPath);
        } else {
            LOG_ERROR("截图临时保存失败");
            addLogEntry("截图失败：临时保存出错");
        }

        // 清理截图选择器
        if (screenshotSelector) {
            screenshotSelector->deleteLater();
            screenshotSelector = nullptr;
        }
    } catch (const std::exception& e) {
        QString errorMsg = QString("处理截图区域时发生异常: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        addLogEntry(errorMsg);

        // 清理截图选择器
        if (screenshotSelector) {
            screenshotSelector->deleteLater();
            screenshotSelector = nullptr;
        }
    } catch (...) {
        LOG_ERROR("处理截图区域时发生未知异常");
        addLogEntry("处理截图区域时发生未知异常");

        // 清理截图选择器
        if (screenshotSelector) {
            screenshotSelector->deleteLater();
            screenshotSelector = nullptr;
        }
    }
}



// 系统托盘相关方法实现
void MainWindow::setupSystemTray() {
    try {
        // 检查系统托盘是否可用
        if (!QSystemTrayIcon::isSystemTrayAvailable()) {
            LOG_WARNING("系统托盘不可用");
            addLogEntry("警告：系统托盘不可用，最小化到托盘功能将被禁用");
            return;
        }

        // 创建托盘菜单
        trayMenu = new QMenu(this);

        QAction *showAction = trayMenu->addAction("显示主窗口");
        connect(showAction, &QAction::triggered, this, &MainWindow::showFromTray);

        QAction *hideAction = trayMenu->addAction("隐藏到托盘");
        connect(hideAction, &QAction::triggered, this, &MainWindow::hideToTray);

        trayMenu->addSeparator();

        QAction *quitAction = trayMenu->addAction("退出");
        connect(quitAction, &QAction::triggered, this, &QWidget::close);

        // 创建托盘图标
        trayIcon = new QSystemTrayIcon(this);
        trayIcon->setIcon(QIcon(":/icons/app_icon.svg"));
        trayIcon->setToolTip("WeBot - 企业微信自动问答工具");
        trayIcon->setContextMenu(trayMenu);

        // 连接托盘图标激活信号
        connect(trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::DoubleClick || reason == QSystemTrayIcon::Trigger) {
                showFromTray();
            }
        });

        // 显示托盘图标
        trayIcon->show();

        LOG_INFO("系统托盘已设置");
        addLogEntry("系统托盘功能已启用");

    } catch (const std::exception& e) {
        QString errorMsg = QString("设置系统托盘时发生异常: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        addLogEntry(errorMsg);
    } catch (...) {
        LOG_ERROR("设置系统托盘时发生未知异常");
        addLogEntry("设置系统托盘时发生未知异常");
    }
}

void MainWindow::showFromTray() {
    try {
        show();
        activateWindow();
        raise();

        if (trayIcon) {
            trayIcon->showMessage("WeBot", "应用程序已恢复", QSystemTrayIcon::Information, 2000);
        }

        LOG_INFO("从托盘显示主窗口");
        addLogEntry("从托盘恢复显示主窗口");

    } catch (const std::exception& e) {
        QString errorMsg = QString("从托盘显示窗口时发生异常: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        addLogEntry(errorMsg);
    } catch (...) {
        LOG_ERROR("从托盘显示窗口时发生未知异常");
        addLogEntry("从托盘显示窗口时发生未知异常");
    }
}

void MainWindow::hideToTray() {
    try {
        hide();

        if (trayIcon) {
            trayIcon->showMessage("WeBot", "应用程序已最小化到托盘", QSystemTrayIcon::Information, 2000);
        }

        LOG_INFO("隐藏主窗口到托盘");
        addLogEntry("主窗口已隐藏到系统托盘");

    } catch (const std::exception& e) {
        QString errorMsg = QString("隐藏窗口到托盘时发生异常: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        addLogEntry(errorMsg);
    } catch (...) {
        LOG_ERROR("隐藏窗口到托盘时发生未知异常");
        addLogEntry("隐藏窗口到托盘时发生未知异常");
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    try {
        // 根据配置的关闭行为决定是隐藏到托盘还是直接关闭
        ConfigManager* config = ConfigManager::getInstance();
        int closeBehavior = config->getCloseBehavior();

        if (closeBehavior == ConfigManager::MinimizeToTray) {
            // 最小化到系统托盘
            if (trayIcon && trayIcon->isVisible()) {
                event->ignore();
                hideToTray();
                return;
            }
        }

        // 直接关闭或托盘不可用
        event->accept();

    } catch (const std::exception& e) {
        QString errorMsg = QString("关闭事件处理异常: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        // 确保应用程序能正常关闭
        event->accept();
    } catch (...) {
        LOG_ERROR("关闭事件处理未知异常");
        event->accept();
    }
}

void MainWindow::openImageEditor(const QString &filePath, const QPixmap &) {
    try {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        addLogEntry("已使用系统默认程序打开截图文件");
        addLogEntry("您可以使用系统自带的画图工具或其他图片编辑软件对截图进行编辑和标注");
        
        QTimer::singleShot(2000, this, [this]() {
            QMessageBox::information(this, "编辑提示", 
                "截图文件已在默认程序中打开\n\n"
                "您可以使用以下工具进行编辑：\n"
                "• Windows画图 - 基础编辑和标注\n"
                "• Paint 3D - 3D绘图工具\n"
                "• Photoshop - 专业图片编辑\n"
                "• GIMP - 免费开源编辑器\n\n"
                "• 在线工具 - 如Canva、Fotor等");
        });
        
    } catch (const std::exception& e) {
        QString errorMsg = QString("打开图片编辑器时发生异常: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        addLogEntry(errorMsg);
    } catch (...) {
        LOG_ERROR("打开图片编辑器时发生未知异常");
        addLogEntry("打开图片编辑器时发生未知异常");
    }
}

void MainWindow::showScreenshotPreview(const QPixmap &screenshot, const QString &filePath) {
    try {
        QDialog *previewDialog = new QDialog(this);
        previewDialog->setWindowTitle("截图预览");
        previewDialog->setModal(true);
        previewDialog->resize(900, 700);

        QVBoxLayout *layout = new QVBoxLayout(previewDialog);

        QLabel *imageLabel = new QLabel(previewDialog);
        imageLabel->setPixmap(screenshot.scaled(previewDialog->size() * 0.7, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imageLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(imageLabel);

        QLabel *infoLabel = new QLabel(QString("截图预览\n尺寸: %1x%2\n\n请选择操作：")
                                      .arg(screenshot.width())
                                      .arg(screenshot.height()), previewDialog);
        infoLabel->setWordWrap(true);
        infoLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(infoLabel);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(10);

        QPushButton *copyButton = new QPushButton("📋 复制到剪贴板", previewDialog);
        copyButton->setMinimumSize(120, 40);
        connect(copyButton, &QPushButton::clicked, this, [this, screenshot]() {
            QApplication::clipboard()->setPixmap(screenshot);
            QMessageBox::information(this, "成功", "截图已复制到剪贴板");
        });
        buttonLayout->addWidget(copyButton);

        QPushButton *saveButton = new QPushButton("💾 保存截图", previewDialog);
        saveButton->setMinimumSize(120, 40);
        connect(saveButton, &QPushButton::clicked, this, [this, filePath]() {
            QString savePath = QFileDialog::getSaveFileName(this, "保存截图", "",
                                                           "PNG文件 (*.png);;JPEG文件 (*.jpg);;BMP文件 (*.bmp);;所有文件 (*.*)");
            if (!savePath.isEmpty()) {
                QFile::copy(filePath, savePath);
                QMessageBox::information(this, "保存成功", "截图已保存到: " + savePath);
            }
        });
        buttonLayout->addWidget(saveButton);

        QPushButton *editButton = new QPushButton("🎨 编辑截图", previewDialog);
        editButton->setMinimumSize(120, 40);
        connect(editButton, &QPushButton::clicked, this, [this, filePath, screenshot]() {
            openImageEditor(filePath, screenshot);
        });
        buttonLayout->addWidget(editButton);

        QPushButton *newScreenshotButton = new QPushButton("✂️ 重新截图", previewDialog);
        newScreenshotButton->setMinimumSize(120, 40);
        connect(newScreenshotButton, &QPushButton::clicked, this, [this, previewDialog]() {
            previewDialog->accept();
            takeScreenshot();
        });
        buttonLayout->addWidget(newScreenshotButton);

        QPushButton *closeButton = new QPushButton("关闭", previewDialog);
        closeButton->setMinimumSize(100, 40);
        connect(closeButton, &QPushButton::clicked, previewDialog, &QDialog::accept);
        buttonLayout->addWidget(closeButton);

        layout->addLayout(buttonLayout);

        previewDialog->exec();
        delete previewDialog;

    } catch (const std::exception& e) {
        QString errorMsg = QString("显示截图预览时发生异常: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        addLogEntry(errorMsg);
    } catch (...) {
        LOG_ERROR("显示截图预览时发生未知异常");
        addLogEntry("显示截图预览时发生未知异常");
    }
}

void MainWindow::on_themeCombo_currentIndexChanged(int index) {
    try {
        ThemeManager *themeManager = ThemeManager::getInstance();

        switch (index) {
        case 0: // 浅色主题
            themeManager->setTheme(ThemeManager::LightTheme);
            break;
        case 1: // 深色主题
            themeManager->setTheme(ThemeManager::DarkTheme);
            break;
        case 2: // 科技主题
            themeManager->setTheme(ThemeManager::TechTheme);
            break;
        case 3: // 樱花主题
            themeManager->setTheme(ThemeManager::SakuraTheme);
            break;
        case 4: // 暖色主题
            themeManager->setTheme(ThemeManager::WarmTheme);
            break;
        default:
            themeManager->setTheme(ThemeManager::TechTheme);
            break;
        }

        addLogEntry(QString("界面主题已切换为: %1").arg(themeManager->getThemeName(themeManager->getCurrentTheme())));

    } catch (const std::exception& e) {
        QString errorMsg = QString("切换主题时发生异常: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        addLogEntry(errorMsg);
    } catch (...) {
        LOG_ERROR("切换主题时发生未知异常");
        addLogEntry("切换主题时发生未知异常");
    }
}

void MainWindow::on_applyShortcutButton_clicked() {
    try {
        if (ui->screenshotShortcutEdit) {
            QKeySequence keySequence = ui->screenshotShortcutEdit->keySequence();
            if (!keySequence.isEmpty()) {
                // 保存到配置管理器
                ConfigManager *config = ConfigManager::getInstance();
                config->setScreenshotShortcut(keySequence.toString());

                // 注销旧的快捷键
                unregisterScreenshotHotkey();

                // 注册新的快捷键
                QString shortcutStr = keySequence.toString();
                addLogEntry(QString("正在设置截图快捷键: %1").arg(shortcutStr));

                // 这里需要解析快捷键并注册
                // 暂时只支持Ctrl+Shift+S，未来可以扩展支持更多组合键
                if (shortcutStr == "Ctrl+Shift+S") {
                    registerScreenshotHotkey();
                    addLogEntry("截图快捷键设置成功: Ctrl+Shift+S");
                } else {
                    addLogEntry("当前只支持 Ctrl+Shift+S 快捷键，其他组合键将在后续版本中支持");
                }
            } else {
                addLogEntry("快捷键设置失败：请输入有效的快捷键组合");
            }
        }
    } catch (const std::exception& e) {
        QString errorMsg = QString("设置快捷键时发生异常: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        addLogEntry(errorMsg);
    } catch (...) {
        LOG_ERROR("设置快捷键时发生未知异常");
        addLogEntry("设置快捷键时发生未知异常");
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    
    // 调整控件大小以适应窗口大小
    if (ui->tabWidget) {
        int margin = 10;
        int newWidth = width() - margin * 2;
        
        // 遍历所有标签页，调整GroupBox宽度
        for (int i = 0; i < ui->tabWidget->count(); ++i) {
            QWidget *tab = ui->tabWidget->widget(i);
            if (tab) {
                // 查找所有GroupBox并调整大小
                QList<QGroupBox*> groupBoxes = tab->findChildren<QGroupBox*>();
                for (QGroupBox *groupBox : groupBoxes) {
                    QRect geometry = groupBox->geometry();
                    geometry.setWidth(newWidth);
                    groupBox->setGeometry(geometry);
                }
                
                // 调整titleLabel
                QLabel *titleLabel = tab->findChild<QLabel*>("titleLabel");
                if (titleLabel) {
                    QRect geometry = titleLabel->geometry();
                    geometry.setWidth(newWidth);
                    titleLabel->setGeometry(geometry);
                }
            }
        }
    }
}
