#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QRect>
#include <windows.h>

class WeChatController : public QObject {
    Q_OBJECT
public:
    explicit WeChatController(QObject *parent = nullptr);
    ~WeChatController();

    // 检查企业微信是否运行
    bool isWeChatRunning();

    // 启动企业微信
    bool startWeChat(const QString &path);

    // 从注册表获取企业微信路径
    QString getWeChatPathFromRegistry();

    // 激活企业微信窗口
    bool activateWeChatWindow();

    // 最大化企业微信窗口
    bool maximizeWeChatWindow();

    // 关闭企业微信
    bool closeWeChat();

    // 获取企业微信窗口句柄
    HWND getWeChatWindowHandle();

    // 设置窗口置顶状态
    bool setWindowTopMost(bool topMost);

    // 获取窗口位置
    QRect getWeChatWindowRect();

    // 多显示器支持
    bool isMultiMonitorSupported();
    int getMonitorCount();
    QRect getMonitorRect(int monitorIndex);
    int getWindowMonitorIndex(HWND hwnd);
    bool moveWindowToMonitor(HWND hwnd, int monitorIndex);

signals:
    void logMessage(const QString &message);

private:
    // 查找企业微信窗口
    HWND findWeChatWindow();

    // 获取窗口位置
    QRect getWindowRect(HWND hwnd);


};
