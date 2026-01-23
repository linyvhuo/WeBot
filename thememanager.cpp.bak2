#include "thememanager.h"
#include <QMutex>
#include <QMutexLocker>

ThemeManager *ThemeManager::instance = nullptr;
QMutex ThemeManager::mutex;

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
    , m_currentTheme(TechTheme)
{
    initThemes();
}

ThemeManager::~ThemeManager()
{
}

ThemeManager *ThemeManager::getInstance()
{
    QMutexLocker locker(&mutex);
    if (!instance) {
        instance = new ThemeManager();
    }
    return instance;
}

void ThemeManager::initThemes()
{
    m_themeStyles[LightTheme] = getLightThemeStyle();
    m_themeStyles[DarkTheme] = getDarkThemeStyle();
    m_themeStyles[TechTheme] = getTechThemeStyle();
}

ThemeManager::Theme ThemeManager::getCurrentTheme() const
{
    return m_currentTheme;
}

void ThemeManager::setTheme(Theme theme)
{
    m_currentTheme = theme;
    applyTheme(theme);
    emit themeChanged(theme);
}

void ThemeManager::applyTheme(Theme theme)
{
    QString style = m_themeStyles.value(theme, getTechThemeStyle());
    qApp->setStyleSheet(style);
}

QString ThemeManager::getThemeName(Theme theme) const
{
    switch (theme) {
    case LightTheme:
        return "浅色主题";
    case DarkTheme:
        return "深色主题";
    case TechTheme:
        return "科技主题";
    default:
        return "科技主题";
    }
}

QStringList ThemeManager::getThemeNames() const
{
    return QStringList() << getThemeName(LightTheme)
                     << getThemeName(DarkTheme)
                     << getThemeName(TechTheme);
}

QString ThemeManager::getLightThemeStyle() const
{
    return R"(
        * {
            background-color: #f5f5f7;
            color: #1d1d1f;
            font-family: -apple-system, BlinkMacSystemFont, sans-serif;
        }

        QMainWindow {
            background-color: #f5f5f7;
            border-radius: 10px;
        }

        QPushButton {
            background-color: #007aff;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 20px;
            font-size: 13px;
            font-weight: 500;
        }

        QPushButton:hover {
            background-color: #0056cc;
        }

        QPushButton:pressed {
            background-color: #0047a3;
        }

        QPushButton:disabled {
            background-color: #c7c7cc;
            color: #ffffff;
        }

        QLineEdit, QTextEdit {
            background-color: white;
            border: 1px solid #c7c7cc;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 13px;
        }

        QLineEdit:focus, QTextEdit:focus {
            border: 2px solid #007aff;
        }

        QTabWidget::pane {
            border: 1px solid #c7c7cc;
            background-color: white;
            border-radius: 6px;
        }

        QTabBar::tab {
            background-color: #e5e5ea;
            color: #1d1d1f;
            padding: 8px 16px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            margin-right: 2px;
        }

        QTabBar::tab:selected {
            background-color: #007aff;
            color: white;
        }

        QTabBar::tab:hover:!selected {
            background-color: #d1d1d6;
        }

        QGroupBox {
            border: 1px solid #c7c7cc;
            border-radius: 6px;
            margin-top: 12px;
            padding-top: 12px;
            font-weight: bold;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
        }

        QLabel {
            color: #1d1d1f;
        }

        QProgressBar {
            border: 2px solid #c7c7cc;
            border-radius: 5px;
            text-align: center;
            background-color: white;
        }

        QProgressBar::chunk {
            background-color: #007aff;
            border-radius: 3px;
        }
    )";
}

QString ThemeManager::getDarkThemeStyle() const
{
    return R"(
        * {
            background-color: #1e1e1e;
            color: #e0e0e0;
            font-family: -apple-system, BlinkMacSystemFont, sans-serif;
        }

        QMainWindow {
            background-color: #1e1e1e;
            border-radius: 10px;
        }

        QPushButton {
            background-color: #0e639c;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 20px;
            font-size: 13px;
            font-weight: 500;
        }

        QPushButton:hover {
            background-color: #0284c7;
        }

        QPushButton:pressed {
            background-color: #015a85;
        }

        QPushButton:disabled {
            background-color: #3a3a3a;
            color: #808080;
        }

        QLineEdit, QTextEdit {
            background-color: #2d2d2d;
            border: 1px solid #3a3a3a;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 13px;
            color: #e0e0e0;
        }

        QLineEdit:focus, QTextEdit:focus {
            border: 2px solid #0e639c;
        }

        QTabWidget::pane {
            border: 1px solid #3a3a3a;
            background-color: #2d2d2d;
            border-radius: 6px;
        }

        QTabBar::tab {
            background-color: #2d2d2d;
            color: #e0e0e0;
            padding: 8px 16px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            margin-right: 2px;
        }

        QTabBar::tab:selected {
            background-color: #0e639c;
            color: white;
        }

        QTabBar::tab:hover:!selected {
            background-color: #3a3a3a;
        }

        QGroupBox {
            border: 1px solid #3a3a3a;
            border-radius: 6px;
            margin-top: 12px;
            padding-top: 12px;
            font-weight: bold;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
        }

        QLabel {
            color: #e0e0e0;
        }

        QProgressBar {
            border: 2px solid #3a3a3a;
            border-radius: 5px;
            text-align: center;
            background-color: #2d2d2d;
        }

        QProgressBar::chunk {
            background-color: #0e639c;
            border-radius: 3px;
        }
    )";
}

QString ThemeManager::getTechThemeStyle() const
{
    return R"(
        * {
            background-color: #0a0e27;
            color: #00ffff;
            font-family: 'Consolas', 'Monaco', monospace;
        }

        QMainWindow {
            background-color: #0a0e27;
            border-radius: 10px;
        }

        QPushButton {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(0, 255, 255, 200), stop:1 rgba(0, 200, 255, 200));
            color: #0a0e27;
            border: 2px solid #00ffff;
            border-radius: 6px;
            padding: 8px 20px;
            font-size: 13px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(0, 200, 255, 200), stop:1 rgba(0, 255, 255, 200));
            border-color: #00cccc;
        }

        QPushButton:pressed {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(0, 150, 200, 200), stop:1 rgba(0, 200, 255, 200));
        }

        QPushButton:disabled {
            background-color: rgba(0, 255, 255, 50);
            color: #008888;
            border-color: #008888;
        }

        QLineEdit, QTextEdit {
            background-color: rgba(0, 20, 40, 200);
            border: 2px solid #00ffff;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 13px;
            color: #00ffff;
        }

        QLineEdit:focus, QTextEdit:focus {
            border-color: #00cccc;
            background-color: rgba(0, 30, 60, 200);
        }

        QTabWidget::pane {
            border: 2px solid #00ffff;
            background-color: rgba(0, 20, 40, 200);
            border-radius: 6px;
        }

        QTabBar::tab {
            background-color: rgba(0, 20, 40, 200);
            color: #00ffff;
            padding: 8px 16px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            margin-right: 2px;
            border: 1px solid #00ffff;
        }

        QTabBar::tab:selected {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(0, 255, 255, 200), stop:1 rgba(0, 200, 255, 200));
            color: #0a0e27;
        }

        QTabBar::tab:hover:!selected {
            background-color: rgba(0, 40, 80, 200);
        }

        QGroupBox {
            border: 2px solid #00ffff;
            border-radius: 6px;
            margin-top: 12px;
            padding-top: 12px;
            font-weight: bold;
            color: #00ffff;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
        }

        QLabel {
            color: #00ffff;
        }

        QProgressBar {
            border: 2px solid #00ffff;
            border-radius: 5px;
            text-align: center;
            background-color: rgba(0, 20, 40, 200);
        }

        QProgressBar::chunk {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(0, 255, 255, 200), stop:1 rgba(0, 200, 255, 200));
            border-radius: 3px;
        }
    )";
}
