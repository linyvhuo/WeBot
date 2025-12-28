#include "inputsimulator.h"
#include <windows.h>
#include <QClipboard>
#include <windows.h>
#include <QApplication>
#include <windows.h>
#include <windows.h>
#include <QThread>
#include <QString>
#include <QDebug>

#ifndef VK_0
#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39
#endif

InputSimulator::InputSimulator(QObject *parent) : QObject(parent)
{
    // 设置默认延迟
    clickDelay = 50; // 点击后的延迟(毫秒)
    keyDelay = 20;   // 按键之间的延迟(毫秒)
    m_stopRequested = false;
}

InputSimulator::~InputSimulator()
{
}

void InputSimulator::setDelays(int clickDelayMs, int keyDelayMs)
{
    clickDelay = clickDelayMs;
    keyDelay = keyDelayMs;
    emit logMessage(QString("输入延迟已设置 - 点击: %1ms, 按键: %2ms")
                        .arg(clickDelay).arg(keyDelay));
}

void InputSimulator::moveMouse(int x, int y) {
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消鼠标移动");
        return;
    }
    
    // 移动鼠标到指定位置
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;

    // 使用绝对坐标移动，确保鼠标精确到达目标位置
    // 注意：SM_CXSCREEN和SM_CYSCREEN返回的是主显示器的尺寸，使用GetSystemMetrics(SM_CXVIRTUALSCREEN)和SM_CYVIRTUALSCREEN更准确
    int screenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    
    input.mi.dx = (LONG)((x - GetSystemMetrics(SM_XVIRTUALSCREEN)) * (65535.0 / screenWidth));
    input.mi.dy = (LONG)((y - GetSystemMetrics(SM_YVIRTUALSCREEN)) * (65535.0 / screenHeight));
    input.mi.mouseData = 0;
    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK;
    input.mi.time = 0;
    input.mi.dwExtraInfo = 0;

    SendInput(1, &input, sizeof(INPUT));
    emit logMessage(QString("鼠标移动到 (%1, %2)").arg(x).arg(y));
}

void InputSimulator::clickAt(int x, int y) {
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消点击操作");
        return;
    }
    
    // 移动到位置并点击
    moveMouse(x, y);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消点击操作");
        return;
    }
    
    QThread::msleep(clickDelay);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消点击操作");
        return;
    }
    
    emit logMessage(QString("准备在 (%1, %2) 位置点击").arg(x).arg(y));

    // 左键按下
    INPUT inputDown;
    ZeroMemory(&inputDown, sizeof(INPUT));
    inputDown.type = INPUT_MOUSE;
    inputDown.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &inputDown, sizeof(INPUT));
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消点击操作");
        // 确保释放鼠标按键
        INPUT inputUp;
        ZeroMemory(&inputUp, sizeof(INPUT));
        inputUp.type = INPUT_MOUSE;
        inputUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &inputUp, sizeof(INPUT));
        return;
    }
    
    QThread::msleep(clickDelay);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消点击操作");
        // 确保释放鼠标按键
        INPUT inputUp;
        ZeroMemory(&inputUp, sizeof(INPUT));
        inputUp.type = INPUT_MOUSE;
        inputUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &inputUp, sizeof(INPUT));
        return;
    }

    // 左键释放
    INPUT inputUp;
    ZeroMemory(&inputUp, sizeof(INPUT));
    inputUp.type = INPUT_MOUSE;
    inputUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &inputUp, sizeof(INPUT));

    emit logMessage(QString("在 (%1, %2) 位置点击完成").arg(x).arg(y));
    // 增加点击后的延迟时间，确保输入框有足够时间获得焦点
    QThread::msleep(300);
}

void InputSimulator::doubleClickAt(int x, int y)
{
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消双击操作");
        return;
    }
    
    // 移动到位置并双击
    moveMouse(x, y);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消双击操作");
        return;
    }
    
    QThread::msleep(clickDelay);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消双击操作");
        return;
    }

    // 第一次点击
    INPUT input1Down;
    ZeroMemory(&input1Down, sizeof(INPUT));
    input1Down.type = INPUT_MOUSE;
    input1Down.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input1Down, sizeof(INPUT));
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消双击操作");
        // 确保释放鼠标按键
        INPUT input1Up;
        ZeroMemory(&input1Up, sizeof(INPUT));
        input1Up.type = INPUT_MOUSE;
        input1Up.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &input1Up, sizeof(INPUT));
        return;
    }
    
    QThread::msleep(50);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消双击操作");
        // 确保释放鼠标按键
        INPUT input1Up;
        ZeroMemory(&input1Up, sizeof(INPUT));
        input1Up.type = INPUT_MOUSE;
        input1Up.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &input1Up, sizeof(INPUT));
        return;
    }

    INPUT input1Up;
    ZeroMemory(&input1Up, sizeof(INPUT));
    input1Up.type = INPUT_MOUSE;
    input1Up.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input1Up, sizeof(INPUT));
    QThread::msleep(50);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消双击操作");
        return;
    }

    // 第二次点击
    INPUT input2Down;
    ZeroMemory(&input2Down, sizeof(INPUT));
    input2Down.type = INPUT_MOUSE;
    input2Down.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input2Down, sizeof(INPUT));
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消双击操作");
        // 确保释放鼠标按键
        INPUT input2Up;
        ZeroMemory(&input2Up, sizeof(INPUT));
        input2Up.type = INPUT_MOUSE;
        input2Up.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &input2Up, sizeof(INPUT));
        return;
    }
    
    QThread::msleep(50);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消双击操作");
        // 确保释放鼠标按键
        INPUT input2Up;
        ZeroMemory(&input2Up, sizeof(INPUT));
        input2Up.type = INPUT_MOUSE;
        input2Up.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &input2Up, sizeof(INPUT));
        return;
    }

    INPUT input2Up;
    ZeroMemory(&input2Up, sizeof(INPUT));
    input2Up.type = INPUT_MOUSE;
    input2Up.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input2Up, sizeof(INPUT));

    emit logMessage(QString("在 (%1, %2) 位置双击").arg(x).arg(y));
    QThread::msleep(clickDelay);
}

void InputSimulator::keyPress(WORD keyCode)
{
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消按键按下");
        return;
    }
    
    // 按下按键（不释放）
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = keyCode;
    SendInput(1, &input, sizeof(INPUT));
    emit logMessage(QString("按键按下: %1").arg(keyCode));
    QThread::msleep(keyDelay);
}

void InputSimulator::keyRelease(WORD keyCode)
{
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消按键释放");
        return;
    }
    
    // 释放按键
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = keyCode;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
    emit logMessage(QString("按键释放: %1").arg(keyCode));
    QThread::msleep(keyDelay);
}

void InputSimulator::pressKey(WORD keyCode)
{
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消按键操作");
        return;
    }
    
    // 按下并释放指定按键
    keyPress(keyCode);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消按键释放");
        return;
    }
    
    keyRelease(keyCode);
}

void InputSimulator::pasteText(const QString &text)
{
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消粘贴操作");
        return;
    }
    
    // 使用剪贴板粘贴文本
    emit logMessage(QString("输入文本: %1").arg(text));
    emit logMessage("[DEBUG] 使用剪贴板粘贴方式输入");
    
    // 获取剪贴板
    QClipboard* clipboard = QApplication::clipboard();
    if (!clipboard) {
        emit logMessage("[ERROR] 无法获取剪贴板对象");
        return;
    }
    
    // 保存原始剪贴板内容
    QString originalContent = clipboard->text();
    emit logMessage(QString("[DEBUG] 当前剪贴板内容: %1").arg(originalContent));
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消粘贴操作");
        return;
    }
    
    // 设置剪贴板内容为要输入的文本
    clipboard->setText(text);
    QThread::msleep(200);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消粘贴操作");
        // 恢复原始剪贴板内容
        clipboard->setText(originalContent);
        return;
    }
    
    // 使用Ctrl+V粘贴
    keyPress(VK_CONTROL);
    pressKey('V');
    keyRelease(VK_CONTROL);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消粘贴操作");
        // 恢复原始剪贴板内容
        clipboard->setText(originalContent);
        return;
    }
    
    // 等待粘贴完成
    QThread::msleep(500);
    
    // 恢复原始剪贴板内容
    clipboard->setText(originalContent);
    QThread::msleep(100);
    
    emit logMessage("文本输入完成");
}

bool InputSimulator::testCopyPaste(const QString &testText)
{
    emit logMessage("[DEBUG] 开始测试复制粘贴功能");
    emit logMessage(QString("[DEBUG] 测试文本: %1").arg(testText));
    
    // 保存当前剪贴板内容
    QClipboard* clipboard = QApplication::clipboard();
    if (!clipboard) {
        emit logMessage("[ERROR] 无法获取剪贴板对象");
        return false;
    }
    
    QString originalClipboard = clipboard->text();
    emit logMessage(QString("[DEBUG] 当前剪贴板内容: %1").arg(originalClipboard));
    
    // 1. 测试设置剪贴板内容
    emit logMessage("[DEBUG] 测试1: 设置剪贴板内容");
    clipboard->setText(testText);
    QThread::msleep(300);
    
    QString clipboardContent = clipboard->text();
    if (clipboardContent != testText) {
        emit logMessage(QString("[ERROR] 测试失败: 无法正确设置剪贴板内容")
                        .arg(clipboardContent));
        emit logMessage(QString("[ERROR] 预期: '%1'，实际: '%2'").arg(testText).arg(clipboardContent));
        // 恢复原始剪贴板内容
        clipboard->setText(originalClipboard);
        return false;
    }
    emit logMessage("[DEBUG] 测试1通过: 剪贴板内容设置成功");
    
    // 2. 测试读取剪贴板内容
    emit logMessage("[DEBUG] 测试2: 读取剪贴板内容");
    QString readContent = clipboard->text();
    if (readContent != testText) {
        emit logMessage(QString("[ERROR] 测试失败: 无法正确读取剪贴板内容"));
        emit logMessage(QString("[ERROR] 预期: '%1'，实际: '%2'").arg(testText).arg(readContent));
        // 恢复原始剪贴板内容
        clipboard->setText(originalClipboard);
        return false;
    }
    emit logMessage("[DEBUG] 测试2通过: 剪贴板内容读取成功");
    
    // 3. 测试多次读取一致性
    emit logMessage("[DEBUG] 测试3: 多次读取剪贴板内容一致性");
    bool consistent = true;
    for (int i = 0; i < 3; i++) {
        QString multiReadContent = clipboard->text();
        if (multiReadContent != testText) {
            emit logMessage(QString("[ERROR] 测试失败: 多次读取剪贴板内容不一致，第%1次失败")
                            .arg(i + 1));
            emit logMessage(QString("[ERROR] 预期: '%1'，实际: '%2'").arg(testText).arg(multiReadContent));
            consistent = false;
            break;
        }
        QThread::msleep(100);
    }
    
    if (!consistent) {
        // 恢复原始剪贴板内容
        clipboard->setText(originalClipboard);
        return false;
    }
    emit logMessage("[DEBUG] 测试3通过: 多次读取剪贴板内容一致");
    
    // 恢复原始剪贴板内容
    emit logMessage("[DEBUG] 恢复原始剪贴板内容");
    clipboard->setText(originalClipboard);
    QThread::msleep(200);
    
    QString restoredContent = clipboard->text();
    if (restoredContent != originalClipboard) {
        emit logMessage(QString("[WARNING] 恢复原始剪贴板内容失败，但测试本身通过")
                        .arg(restoredContent));
    }
    
    emit logMessage("[DEBUG] 复制粘贴功能测试通过");
    return true;
}

void InputSimulator::dragMouse(int startX, int startY, int endX, int endY)
{
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消拖拽操作");
        return;
    }
    
    // 移动到起始位置
    moveMouse(startX, startY);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消拖拽操作");
        return;
    }
    
    QThread::msleep(clickDelay);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消拖拽操作");
        return;
    }

    // 按下左键
    INPUT inputDown;
    ZeroMemory(&inputDown, sizeof(INPUT));
    inputDown.type = INPUT_MOUSE;
    inputDown.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &inputDown, sizeof(INPUT));
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消拖拽操作");
        // 确保释放鼠标按键
        INPUT inputUp;
        ZeroMemory(&inputUp, sizeof(INPUT));
        inputUp.type = INPUT_MOUSE;
        inputUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &inputUp, sizeof(INPUT));
        return;
    }
    
    QThread::msleep(clickDelay);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消拖拽操作");
        // 确保释放鼠标按键
        INPUT inputUp;
        ZeroMemory(&inputUp, sizeof(INPUT));
        inputUp.type = INPUT_MOUSE;
        inputUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &inputUp, sizeof(INPUT));
        return;
    }

    // 移动到目标位置
    moveMouse(endX, endY);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消拖拽操作");
        // 确保释放鼠标按键
        INPUT inputUp;
        ZeroMemory(&inputUp, sizeof(INPUT));
        inputUp.type = INPUT_MOUSE;
        inputUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &inputUp, sizeof(INPUT));
        return;
    }
    
    QThread::msleep(clickDelay);
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消拖拽操作");
        // 确保释放鼠标按键
        INPUT inputUp;
        ZeroMemory(&inputUp, sizeof(INPUT));
        inputUp.type = INPUT_MOUSE;
        inputUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1, &inputUp, sizeof(INPUT));
        return;
    }

    // 释放左键
    INPUT inputUp;
    ZeroMemory(&inputUp, sizeof(INPUT));
    inputUp.type = INPUT_MOUSE;
    inputUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &inputUp, sizeof(INPUT));

    emit logMessage(QString("鼠标拖拽从 (%1, %2) 到 (%3, %4)")
                    .arg(startX).arg(startY).arg(endX).arg(endY));
    QThread::msleep(clickDelay);
}

void InputSimulator::typeText(const QString &text) {
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[DEBUG] 停止请求已收到，取消文本输入");
        return;
    }
    
    emit logMessage(QString("输入文本: %1").arg(text));
    
    // 检查文本是否为空
    if (text.isEmpty()) {
        emit logMessage("[WARNING] 输入文本为空，跳过输入操作");
        return;
    }
    
    // 使用直接键盘输入方式
    emit logMessage("[DEBUG] 使用直接键盘输入方式");
    
    // 遍历文本中的每个字符，逐个发送键盘事件
    for (const QChar &ch : text) {
        // 检查是否请求停止
        if (m_stopRequested) {
            emit logMessage("[DEBUG] 停止请求已收到，取消文本输入");
            return;
        }
        
        // 转换为UTF-16字符
        wchar_t wc = ch.unicode();
        
        // 发送字符的键盘事件
        INPUT input[2];
        
        // 初始化第一个输入事件（按下字符）
        ZeroMemory(&input[0], sizeof(INPUT));
        input[0].type = INPUT_KEYBOARD;
        input[0].ki.wVk = 0; // 使用wScan而非VK
        input[0].ki.wScan = wc;
        input[0].ki.dwFlags = KEYEVENTF_UNICODE;
        
        // 初始化第二个输入事件（释放字符）
        ZeroMemory(&input[1], sizeof(INPUT));
        input[1].type = INPUT_KEYBOARD;
        input[1].ki.wVk = 0;
        input[1].ki.wScan = wc;
        input[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        
        // 发送输入事件
        UINT result = SendInput(2, input, sizeof(INPUT));
        if (result != 2) {
            emit logMessage(QString("[WARNING] 发送键盘事件失败，字符: %1，SendInput返回: %2").arg(ch).arg(result));
            DWORD error = GetLastError();
            emit logMessage(QString("[WARNING] 错误代码: %1").arg(error));
        }
        
        // 按键之间添加延迟，确保输入准确，增加延迟时间
        QThread::msleep(100);
    }
    
    // 等待所有输入完成
    QThread::msleep(500);
    
    emit logMessage("文本输入完成");
}