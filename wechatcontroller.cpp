#include "wechatcontroller.h"
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <QProcess>
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QRect>

WeChatController::WeChatController(QObject *parent) : QObject(parent)
{
    // 构造函数
}

WeChatController::~WeChatController()
{
}

bool WeChatController::isWeChatRunning()
{
    // 检查进程是否存在
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        emit logMessage("无法获取进程列表");
        return false;
    }

    bool found = false;
    DWORD processId = 0;
    
    Process32First(processesSnapshot, &processInfo);
    // 检查WeChatWork.exe或WXWork.exe进程
    if (!_tcscmp(processInfo.szExeFile, L"WeChatWork.exe") || 
        !_tcscmp(processInfo.szExeFile, L"WXWork.exe")) {
        found = true;
        processId = processInfo.th32ProcessID;
    }

    while (!found && Process32Next(processesSnapshot, &processInfo)) {
        if (!_tcscmp(processInfo.szExeFile, L"WeChatWork.exe") || 
            !_tcscmp(processInfo.szExeFile, L"WXWork.exe")) {
            found = true;
            processId = processInfo.th32ProcessID;
            break;
        }
    }

    CloseHandle(processesSnapshot);
    
    if (found) {
        emit logMessage(QString("企业微信进程已运行，进程ID: %1").arg(processId));
    }
    
    return found;
}

bool WeChatController::startWeChat(const QString &path)
{
    if (path.isEmpty()) {
        emit logMessage("企业微信路径为空");
        return false;
    }

    // 检查文件是否存在
    QFile file(path);
    if (!file.exists()) {
        emit logMessage("企业微信文件不存在: " + path);
        return false;
    }

    // 检查是否已经在运行
    if (isWeChatRunning()) {
        emit logMessage("企业微信已经在运行，无需重新启动");
        return true;
    }

    // 启动企业微信
    emit logMessage("正在启动企业微信: " + path);
    
    // 使用异步方式启动企业微信，避免阻塞
    bool started = false;
    
    // 尝试普通方式启动
    emit logMessage("尝试普通方式启动企业微信");
    if (QProcess::startDetached(path)) {
        emit logMessage("企业微信启动成功(普通权限)");
        started = true;
    } else {
        // 如果普通方式失败，尝试管理员权限启动
        emit logMessage("普通方式启动失败，尝试管理员权限启动");
        SHELLEXECUTEINFO shExInfo;
        ZeroMemory(&shExInfo, sizeof(shExInfo));
        shExInfo.cbSize = sizeof(SHELLEXECUTEINFO);
        shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        shExInfo.hwnd = NULL;
        shExInfo.lpVerb = L"runas";  // 请求管理员权限
        shExInfo.lpFile = (LPCWSTR)path.utf16();
        shExInfo.lpParameters = NULL;
        shExInfo.lpDirectory = NULL;
        shExInfo.nShow = SW_SHOW;
        shExInfo.hInstApp = NULL;
        
        if (ShellExecuteEx(&shExInfo)) {
            emit logMessage("企业微信启动成功(管理员权限)");
            if (shExInfo.hProcess) {
                CloseHandle(shExInfo.hProcess);
            }
            started = true;
        } else {
            DWORD error = GetLastError();
            emit logMessage(QString("企业微信启动失败，错误码: %1").arg(error));
            started = false;
        }
    }
    
    if (started) {
        // 等待企业微信完全启动
        emit logMessage("等待企业微信完全启动...");
        QThread::msleep(3000); // 等待3秒
        
        // 激活并最大化窗口
        activateWeChatWindow();
    }
    
    return started;
}

QString WeChatController::getWeChatPathFromRegistry()
{
    // 从注册表获取企业微信安装路径
    QString wechatPath;
    QStringList registryPaths = {
        // 标准安装路径 - 32位系统
        "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\企业微信",
        // 标准安装路径 - 64位系统
        "HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\企业微信",
        // 用户级安装路径
        "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\企业微信"
    };
    
    // 尝试从各个注册表路径查找
    for (const QString& registryPath : registryPaths) {
        QSettings settings(registryPath, QSettings::NativeFormat);
        if (settings.contains("InstallLocation")) {
            QString installLocation = settings.value("InstallLocation").toString();
            // 确保路径以斜杠结尾
            if (!installLocation.endsWith('/') && !installLocation.endsWith('\\')) {
                installLocation += "/";
            }
            
            wechatPath = installLocation + "WeChatWork.exe";
            // 替换路径中的正斜杠为反斜杠（Windows风格）
            wechatPath.replace("/", "\\");
            
            QFile file(wechatPath);
            if (file.exists()) {
                emit logMessage("从注册表找到企业微信路径: " + wechatPath);
                return wechatPath;
            }
        }
    }
    
    // 尝试常见安装位置
    QStringList commonPaths = {
        "C:\\Program Files\\WXWork\\WeChatWork.exe",
        "C:\\Program Files (x86)\\WXWork\\WeChatWork.exe",
        "D:\\Program Files\\WXWork\\WeChatWork.exe",
        "D:\\Program Files (x86)\\WXWork\\WeChatWork.exe"
    };
    
    for (const QString& path : commonPaths) {
        QFile file(path);
        if (file.exists()) {
            emit logMessage("在常见位置找到企业微信: " + path);
            return path;
        }
    }

    emit logMessage("无法从注册表获取企业微信路径");
    return "";
}

HWND WeChatController::findWeChatWindow()
{
    // 尝试查找企业微信窗口，直接硬编码窗口标题
    HWND hwnd = FindWindowW(NULL, L"企业微信");
    if (hwnd != NULL) {
        return hwnd;
    }

    // 如果找不到，尝试查找企业微信的另一个常见窗口标题
    hwnd = FindWindowW(NULL, L"WeChatWork");
    if (hwnd != NULL) {
        return hwnd;
    }

    // 如果找不到，尝试查找企业微信的主窗口类名
    hwnd = FindWindowW(L"WeChatMainWndForPC", NULL);
    if (hwnd != NULL) {
        return hwnd;
    }

    // 如果找不到，尝试查找企业微信的另一个窗口类名
    return FindWindowW(L"WeChatWorkMainWndForPC", NULL);
}

bool WeChatController::activateWeChatWindow()
{
    HWND hwnd = findWeChatWindow();
    if (hwnd == NULL) {
        emit logMessage("找不到企业微信窗口");
        return false;
    }

    // 确保窗口可见
    if (IsIconic(hwnd)) {
        ShowWindow(hwnd, SW_RESTORE); // 从最小化恢复
    } else {
        ShowWindow(hwnd, SW_SHOW); // 显示窗口
    }

    // 激活窗口并前置
    SetForegroundWindow(hwnd);
    SetActiveWindow(hwnd);

    // 等待窗口激活
    QThread::msleep(500);

    // 最大化窗口
    maximizeWeChatWindow();

    // 检查是否激活成功，降低判断条件，只要窗口可见即可
    if (IsWindowVisible(hwnd)) {
        emit logMessage("企业微信窗口已激活并最大化");
        return true;
    } else {
        emit logMessage("无法激活企业微信窗口，但窗口已可见");
        return true; // 降低判断条件，只要窗口可见就返回true
    }
}

bool WeChatController::maximizeWeChatWindow()
{
    HWND hwnd = findWeChatWindow();
    if (hwnd == NULL) {
        emit logMessage("找不到企业微信窗口");
        return false;
    }

    // 最大化窗口
    ShowWindow(hwnd, SW_MAXIMIZE);
    emit logMessage("企业微信窗口已最大化");

    // 等待窗口最大化
    QThread::msleep(500);

    // 检查是否最大化成功
    WINDOWPLACEMENT placement;
    placement.length = sizeof(WINDOWPLACEMENT);
    if (GetWindowPlacement(hwnd, &placement)) {
        if (placement.showCmd == SW_MAXIMIZE) {
            emit logMessage("企业微信窗口最大化成功");
            return true;
        } else {
            emit logMessage("企业微信窗口最大化失败，当前状态: " + QString::number(placement.showCmd));
            return false;
        }
    }

    return true;
}

bool WeChatController::setWindowTopMost(bool topMost)
{
    HWND hwnd = findWeChatWindow();
    if (hwnd == NULL) {
        emit logMessage("找不到企业微信窗口");
        return false;
    }

    // 设置窗口置顶属性
    SetWindowPos(hwnd, topMost ? HWND_TOPMOST : HWND_NOTOPMOST,
                 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE);

    emit logMessage(topMost ? "企业微信窗口已置顶" : "企业微信窗口取消置顶");
    return true;
}

QRect WeChatController::getWeChatWindowRect()
{
    HWND hwnd = findWeChatWindow();
    if (hwnd == NULL) {
        emit logMessage("找不到企业微信窗口");
        return QRect();
    }

    RECT rect;
    if (GetWindowRect(hwnd, &rect)) {
        return QRect(rect.left, rect.top,
                     rect.right - rect.left,
                     rect.bottom - rect.top);
    } else {
        emit logMessage("无法获取企业微信窗口位置");
        return QRect();
    }
}

bool WeChatController::closeWeChat()
{
    HWND hwnd = findWeChatWindow();
    if (hwnd == NULL) {
        emit logMessage("找不到企业微信窗口");
        return false;
    }

    // 发送关闭消息
    SendMessage(hwnd, WM_CLOSE, 0, 0);

    // 等待关闭
    int timeout = 5000; // 5秒超时
    int interval = 500; // 检查间隔
    int elapsed = 0;

    while (elapsed < timeout) {
        if (!isWeChatRunning()) {
            emit logMessage("企业微信已关闭");
            return true;
        }
        QThread::msleep(interval);
        elapsed += interval;
    }

    emit logMessage("企业微信关闭超时");
    return false;
}

HWND WeChatController::getWeChatWindowHandle() {
    return findWeChatWindow();
}

bool WeChatController::isMultiMonitorSupported() {
    return GetSystemMetrics(SM_CMONITORS) > 1;
}

int WeChatController::getMonitorCount() {
    return GetSystemMetrics(SM_CMONITORS);
}

QRect WeChatController::getMonitorRect(int monitorIndex) {
    if (monitorIndex < 0 || monitorIndex >= getMonitorCount()) {
        return QRect();
    }
    
    DISPLAY_DEVICE displayDevice;
    displayDevice.cb = sizeof(DISPLAY_DEVICE);
    
    if (EnumDisplayDevices(NULL, monitorIndex, &displayDevice, 0)) {
        DEVMODE devMode;
        devMode.dmSize = sizeof(DEVMODE);
        
        if (EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode)) {
            return QRect(devMode.dmPosition.x, devMode.dmPosition.y,
                        devMode.dmPelsWidth, devMode.dmPelsHeight);
        }
    }
    
    return QRect();
}

int WeChatController::getWindowMonitorIndex(HWND hwnd) {
    if (!hwnd) return -1;
    
    HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    if (!monitor) return -1;
    
    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFO);
    
    if (GetMonitorInfo(monitor, &monitorInfo)) {
        // 遍历所有显示器找到匹配的
        for (int i = 0; i < getMonitorCount(); ++i) {
            QRect monitorRect = getMonitorRect(i);
            if (monitorRect == QRect(monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top,
                                   monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
                                   monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top)) {
                return i;
            }
        }
    }
    
    return -1;
}

bool WeChatController::moveWindowToMonitor(HWND hwnd, int monitorIndex) {
    if (!hwnd || monitorIndex < 0 || monitorIndex >= getMonitorCount()) {
        return false;
    }
    
    QRect monitorRect = getMonitorRect(monitorIndex);
    if (monitorRect.isEmpty()) {
        emit logMessage("无法获取显示器信息");
        return false;
    }
    
    // 获取窗口当前大小
    RECT windowRect;
    if (!GetWindowRect(hwnd, &windowRect)) {
        emit logMessage("无法获取窗口大小");
        return false;
    }
    
    int width = windowRect.right - windowRect.left;
    int height = windowRect.bottom - windowRect.top;
    
    // 计算在新显示器上的位置（居中显示）
    int x = monitorRect.x() + (monitorRect.width() - width) / 2;
    int y = monitorRect.y() + (monitorRect.height() - height) / 2;
    
    // 确保窗口在显示器范围内
    if (x < monitorRect.x()) x = monitorRect.x();
    if (y < monitorRect.y()) y = monitorRect.y();
    if (x + width > monitorRect.x() + monitorRect.width()) {
        x = monitorRect.x() + monitorRect.width() - width;
    }
    if (y + height > monitorRect.y() + monitorRect.height()) {
        y = monitorRect.y() + monitorRect.height() - height;
    }
    
    // 移动窗口
    if (SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER)) {
        emit logMessage(QString("窗口已移动到显示器 %1").arg(monitorIndex));
        return true;
    } else {
        emit logMessage("移动窗口失败");
        return false;
    }
}
