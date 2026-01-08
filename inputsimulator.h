#ifndef INPUTSIMULATOR_H
#define INPUTSIMULATOR_H

#include <QObject>
#include <QPoint>
#include <windows.h>

class InputSimulator : public QObject {
    Q_OBJECT
public:
    explicit InputSimulator(QObject *parent = nullptr);
    ~InputSimulator();

    // 设置延迟参数
    void setDelays(int clickDelayMs, int keyDelayMs);

    // 移动鼠标
    void moveMouse(int x, int y);

    // 点击鼠标
    void click();

    // 双击鼠标
    void doubleClick();

    // 按下键盘按键（不释放）
    void keyPress(WORD keyCode);

    // 释放键盘按键
    void keyRelease(WORD keyCode);

    // 按下并释放键盘按键
    void pressKey(WORD keyCode);

    // 输入文本
    void typeText(const QString &text);

    // 使用剪贴板粘贴文本
    void pasteText(const QString &text);

    // 在指定位置点击
    void clickAt(int x, int y);

    // 在指定位置双击
    void doubleClickAt(int x, int y);

    // 拖拽鼠标
    void dragMouse(int startX, int startY, int endX, int endY);
    
    // 测试复制粘贴功能
    bool testCopyPaste(const QString &testText);

signals:
    void logMessage(const QString &message);

private:
    int clickDelay = 100;
    int keyDelay = 50;

    int globalDelay; // 每次操作后的全局延迟，毫秒
    bool m_stopRequested = false; // 停止请求标志

    // 等待指定时间
    void wait(int ms);

    // 转换QString到宽字符
    LPWSTR qStringToLPWSTR(const QString &str);
    
public:
    // 设置停止请求
    void setStopRequested(bool requested) {
        m_stopRequested = requested;
    }
    
    // 检查是否请求停止
    bool isStopRequested() const {
        return m_stopRequested;
    }
};

#endif // INPUTSIMULATOR_H
