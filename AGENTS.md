# WeBot Development Guide for Agentic Coding Agents

Guide for coding agents working on WeBot - Qt-based automation tool for WeChat Work.

## Build Commands

```bash
cd D:\QTDownLoads\WeBot

# Build (Release by default)
make

# Build Debug
make debug

# Clean
make clean

# Regenerate Makefile
qmake WeBot.pro
```

**Dependencies:** Qt 6.5.3, OpenCV 4.1.0, MinGW 64-bit, Windows API  
**Build Targets:** release, debug, clean, distclean

## Naming Conventions

- **Classes:** PascalCase (Automator, ConfigManager)
- **Member vars:** camelCase with `m_` prefix (m_configManager, m_state)
- **Functions:** camelCase (start(), stop(), loadConfig())
- **Parameters:** camelCase (questionMode, totalCount)
- **Macros:** UPPER_SNAKE_CASE (Q_OBJECT, LOG_DEBUG)
- **Enums:** PascalCase (Idle, Running, Error)

## File Naming

- Headers: lowercase (automator.h, mainwindow.h)
- Source: lowercase (automator.cpp, mainwindow.cpp)
- UI files: lowercase (mainwindow.ui)

## Includes Order

1. Qt includes (alphabetically grouped)
2. Third-party (OpenCV, Windows API)
3. Custom headers (with quotes)

```cpp
#include <QApplication>
#include <QTimer>
#include <windows.h>
#include "automator.h"
#include "configmanager.h"
```

**Prefer forward declarations** in headers, include in .cpp files.

## Formatting

- **Indentation:** 4 spaces (no tabs)
- **Braces:** Allman style (opening brace on new line)
- **Line length:** Under 120 chars

```cpp
if (condition)
{
    doSomething();
}

class MyClass : public QObject
{
    Q_OBJECT
public:
    explicit MyClass(QObject *parent = nullptr);
};
```

## Type Guidelines

- Prefer Qt types: `QString`, `QVector`, `QMap`, `QPoint`
- Use const references for non-primitive parameters
- Const methods where appropriate

```cpp
void setQuestionList(const QStringList &list);
QString getQuestionPath() const;
```

## Error Handling

- Return `bool` for recoverable errors
- Use `LOG_ERROR()` / `LOG_CRITICAL()` for logging
- Emit signals for async errors

```cpp
bool loadConfig();
LOG_ERROR("Failed to load config");
emit errorMessage("Operation failed");
```

## Qt-Specific Patterns

**QObject hierarchy:**
```cpp
class MyClass : public QObject
{
    Q_OBJECT
public:
    explicit MyClass(QObject *parent = nullptr);
};
```

**Signal/slot connections:**
```cpp
connect(sender, &Sender::signal, receiver, &Receiver::slot);
connect(btn, &QPushButton::clicked, this, [this]() { handleClick(); });
```

**Singleton pattern:**
```cpp
class ConfigManager : public QObject
{
    Q_OBJECT
public:
    static ConfigManager *getInstance();
private:
    static ConfigManager *instance;
    static QMutex mutex;
};
```

**Thread safety:**
```cpp
std::atomic<bool> m_stopRequested = false;
std::atomic<State> m_state = Idle;
mutable QMutex m_mutex;
```

**QThread:**
```cpp
workerObject->moveToThread(m_workerThread);
m_workerThread->start();
QMetaObject::invokeMethod(obj, "method", Qt::QueuedConnection);
```

## Comments

- Language: **Chinese** (standard in this codebase)
- Style: Single-line `//` preferred

```cpp
// 初始化核心模块实例
m_weChatController = new WeChatController();
```

## Logging

```cpp
#include "logger.h"
LOG_DEBUG("Debug message");
LOG_INFO("Info message");
LOG_WARNING("Warning message");
LOG_ERROR("Error message");
LOG_CRITICAL("Critical message");
```

## Architecture

**Core Modules:** Automator (orchestrator), ConfigManager (singleton), Logger (singleton), WeChatController, ImageRecognizer, InputSimulator, QuestionManager, ThemeManager (singleton), MainWindow

**Threading:** Main thread (UI), worker thread (automation). Use `moveToThread()` and Qt signals for communication.

**Memory:** Qt parent-child for automatic cleanup, singletons for managers.

## Common Patterns

**Stop request pattern:**
```cpp
std::atomic<bool> m_stopRequested = false;
void stop() { m_stopRequested = true; }
while (!m_stopRequested && condition) { /* work */ }
```

**Config access:**
```cpp
ConfigManager *config = ConfigManager::getInstance();
int timeout = config->getAnswerTimeout();
```

## Important Notes

- **Chinese UI:** UI strings in Chinese
- **Windows Only:** Windows-specific, uses Windows API
- **OpenCV:** Template matching
- **No Tests:** No automated test framework configured
- **Qt Designer:** UI in `mainwindow.ui`
- **Templates:** Image templates in `templates/` directory

## Testing

**No test framework currently configured.** Manual testing with actual WeChat Work application required.

## Quick Reference

**Build:**
```bash
qmake WeBot.pro && make release
```

**Run:**
```bash
cd dabao && WeBot.exe
```

**Add module:** Create `.h` with Q_OBJECT, `.cpp` with implementation, add to WeBot.pro SOURCES/HEADERS, follow conventions, add LOG_* macros, handle stop requests.
