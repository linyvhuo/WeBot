#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QMouseEvent>
#include <QTimer>
#include <QPoint>
#include "automator.h"
#include "recognitionoverlay.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 输入方式枚举
    enum InputMethod {
        KeyboardInput,  // 模拟键盘逐个输入
        PasteInput      // 复制粘贴输入
    };
    
    // 获取当前输入方式
    InputMethod getCurrentInputMethod() const;
    
    // 设置当前输入方式
    void setCurrentInputMethod(InputMethod method);

private slots:
        void on_startButton_clicked();
        void on_stopButton_clicked();
        void on_browseWechatButton_clicked();
        void on_browseQuestionsButton_clicked();
        void on_saveConfigButton_clicked();
        void on_loadConfigButton_clicked();
        void on_saveAdvancedConfigButton_clicked();
        void on_loadAdvancedConfigButton_clicked();
        void on_exportLogButton_clicked();
        void on_clearLogButton_clicked();

        void on_thresholdSpin_valueChanged(double value);
        void on_attemptsSpin_valueChanged(int value);
        void on_pageTimeoutSpin_valueChanged(int value);
        void on_recognitionTimeoutSpin_valueChanged(int value);
        void on_recognitionTechniqueCombo_currentIndexChanged(int index);
        void on_timeoutSpin_valueChanged(int value);
        void on_loopCountSpin_valueChanged(int value);
        void on_multiMonitorCheck_toggled(bool checked);
        void on_primaryMonitorSpin_valueChanged(int value);

    // 自动化状态变化
    void onAutomationStateChanged(Automator::State state);

    // 自动化完成
    void onAutomationCompleted();
    
    // 图标管理相关槽函数
    void on_browseIconButton_clicked();
    void on_saveIconButton_clicked();
    void on_resetIconButton_clicked();
    void on_iconNameCombo_currentIndexChanged(int index);
    void on_iconPathEdit_textChanged(const QString &path);
    
    // 路径配置相关槽函数
    void on_configPathBrowseButton_clicked();
    void on_questionPathBrowseButton_clicked();
    void on_keywordPathBrowseButton_clicked();
    void on_logPathBrowseButton_clicked();
    void on_templatePathBrowseButton_clicked();
    void on_resetAllPathsButton_clicked();
    
    // 官网跳转相关槽函数
    void on_visitWebsiteButton_clicked();
    
    // 辅助函数：更新图标预览
    void updateIconPreview(const QString &path, QLabel *previewLabel);
    
    // 页面切换槽函数
    void onTabChanged(int index);
    
    // 错误提示槽函数
    void showErrorMessage(const QString &message);



protected:
    // 重写键盘事件处理函数，用于处理ESC按键
    void keyPressEvent(QKeyEvent *event) override;
    // 重写事件过滤器
    bool eventFilter(QObject *obj, QEvent *event) override;
    
public:
    // 获取日志内容
    QString getLogContent();

private:
    Ui::MainWindow *ui;
    Automator *automator = nullptr;
    RecognitionOverlay *recognitionOverlay = nullptr;
    


    // 加载配置到UI
    bool loadConfigToUI();

    // 从UI保存配置
    bool saveConfigFromUI();

    // 添加日志条目
    void addLogEntry(const QString &logEntry);
    void addLogEntry(const QString &logEntry, bool logToFile);

    // 更新进度
    void updateProgress(int current, int total);

    // 更新UI状态
    void updateUIState(bool isRunning);
};
#endif // MAINWINDOW_H
