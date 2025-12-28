#include "qt_compat.h"
#include "mainwindow.h"
#include "logger.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <windows.h>

// 全局变量，用于存储崩溃信息
LONG WINAPI exceptionFilter(EXCEPTION_POINTERS* exceptionInfo)
{
    QString errorMsg;
    switch (exceptionInfo->ExceptionRecord->ExceptionCode) {
    case EXCEPTION_ACCESS_VIOLATION:
        errorMsg = "访问冲突异常";
        break;
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        errorMsg = "数据类型未对齐异常";
        break;
    case EXCEPTION_BREAKPOINT:
        errorMsg = "断点异常";
        break;
    case EXCEPTION_SINGLE_STEP:
        errorMsg = "单步执行异常";
        break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        errorMsg = "数组越界异常";
        break;
    case EXCEPTION_FLT_DENORMAL_OPERAND:
        errorMsg = "浮点非正规操作数异常";
        break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        errorMsg = "浮点除以零异常";
        break;
    case EXCEPTION_FLT_INEXACT_RESULT:
        errorMsg = "浮点不精确结果异常";
        break;
    case EXCEPTION_FLT_INVALID_OPERATION:
        errorMsg = "浮点无效操作异常";
        break;
    case EXCEPTION_FLT_OVERFLOW:
        errorMsg = "浮点溢出异常";
        break;
    case EXCEPTION_FLT_STACK_CHECK:
        errorMsg = "浮点栈检查异常";
        break;
    case EXCEPTION_FLT_UNDERFLOW:
        errorMsg = "浮点下溢异常";
        break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        errorMsg = "整数除以零异常";
        break;
    case EXCEPTION_INT_OVERFLOW:
        errorMsg = "整数溢出异常";
        break;
    case EXCEPTION_PRIV_INSTRUCTION:
        errorMsg = "特权指令异常";
        break;
    case EXCEPTION_IN_PAGE_ERROR:
        errorMsg = "页错误异常";
        break;
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        errorMsg = "非法指令异常";
        break;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        errorMsg = "不可继续异常";
        break;
    case EXCEPTION_STACK_OVERFLOW:
        errorMsg = "栈溢出异常";
        break;
    case EXCEPTION_INVALID_DISPOSITION:
        errorMsg = "无效处置异常";
        break;
    case EXCEPTION_GUARD_PAGE:
        errorMsg = "保护页异常";
        break;
    case EXCEPTION_INVALID_HANDLE:
        errorMsg = "无效句柄异常";
        break;
    default:
        errorMsg = QString("未知异常，代码: 0x%1").arg(exceptionInfo->ExceptionRecord->ExceptionCode, 8, 16, QLatin1Char('0'));
        break;
    }
    
    // 记录崩溃信息到日志文件，使用指定的D盘路径
    QString crashLogPath = "D:/OneDrive - linl/文档/WeBot/Logs/crash.log";
    QDir().mkpath("D:/OneDrive - linl/文档/WeBot/Logs");
    QFile crashFile(crashLogPath);
    if (crashFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&crashFile);
        stream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << " - " << errorMsg << Qt::endl;
        stream << "异常地址: 0x" << QString::number((quintptr)exceptionInfo->ExceptionRecord->ExceptionAddress, 16) << Qt::endl;
        stream << "线程ID: " << GetCurrentThreadId() << Qt::endl;
        stream << "进程ID: " << GetCurrentProcessId() << Qt::endl;
        stream << "----------------------------------------" << Qt::endl;
        crashFile.close();
    }
    
    // 显示崩溃信息
    QMessageBox::critical(nullptr, "程序崩溃", QString("程序发生崩溃: %1\n\n详细信息已记录到: %2").arg(errorMsg).arg(crashLogPath));
    
    // 继续默认处理
    return EXCEPTION_EXECUTE_HANDLER;
}

// Qt 6入口点函数
int qMain(int argc, char *argv[])
{
    // 安装全局异常过滤器
    SetUnhandledExceptionFilter(exceptionFilter);
    
    QApplication a(argc, argv);

    // 检查是否以管理员权限运行
    // 在实际实现中需要添加Windows权限检查代码

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "WeBot_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // 确保中文显示正常
    QFont font = a.font();
    font.setFamily("SimHei");
    a.setFont(font);

    MainWindow w;
    w.show();

    int result = a.exec();
    
    // 程序退出日志由Logger类自动处理
    
    return result;
}
