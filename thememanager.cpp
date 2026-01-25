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
    m_themeStyles[SakuraTheme] = getSakuraThemeStyle();
    m_themeStyles[WarmTheme] = getWarmThemeStyle();
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
    case SakuraTheme:
        return "樱花主题";
    case WarmTheme:
        return "暖色主题";
    default:
        return "科技主题";
    }
}

QStringList ThemeManager::getThemeNames() const
{
    return QStringList() << getThemeName(LightTheme)
                     << getThemeName(DarkTheme)
                     << getThemeName(TechTheme)
                     << getThemeName(SakuraTheme)
                     << getThemeName(WarmTheme);
}

QString ThemeManager::getLightThemeStyle() const
{
    return R"(
        * {
            background-color: #f8f9fa;
            color: #1f2937;
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
        }

        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(248, 250, 252, 1),
                stop:0.5 rgba(241, 245, 249, 1),
                stop:1 rgba(235, 240, 245, 1));
            border-radius: 0px;
            border: 1px solid rgba(66, 133, 244, 0.1);
        }

        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(66, 133, 244, 1),
                stop:1 rgba(52, 121, 193, 1));
            color: white;
            border: none;
            border-radius: 0px;
            padding: 10px 20px;
            font-size: 14px;
            font-weight: 600;
            min-height: 16px;
        }

        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(52, 121, 193, 1),
                stop:1 rgba(41, 98, 155, 1));
            transform: translateY(-2px);
            box-shadow: 0 6px 20px rgba(66, 133, 244, 0.3);
        }

        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(41, 98, 155, 1),
                stop:1 rgba(30, 75, 117, 1));
            transform: translateY(0px);
            box-shadow: 0 2px 8px rgba(66, 133, 244, 0.2);
        }

        QPushButton:disabled {
            background: rgba(189, 189, 189, 0.5);
            color: rgba(117, 117, 117, 0.8);
            box-shadow: none;
            transform: none;
        }

        QLineEdit, QTextEdit {
            background-color: white;
            border: 2px solid #e2e8f0;
            border-radius: 0px;
            padding: 10px 14px;
            font-size: 14px;
            color: #1f2937;
        }

        QLineEdit:focus, QTextEdit:focus {
            border: 2px solid #4285f4;
            background-color: #f8fafc;
            box-shadow: 0 0 0 4px rgba(66, 133, 244, 0.1);
        }

        QTabWidget::pane {
            border: 2px solid #e2e8f0;
            background-color: white;
            border-radius: 0px;
            top: -1px;
        }

        QTabBar::tab {
            background-color: rgba(255, 255, 255, 0.8);
            color: #64748b;
            padding: 10px 20px;
            border-top-left-radius: 0px;
            border-top-right-radius: 0px;
            margin-right: 2px;
            font-weight: 500;
            min-width: 80px;
        }

        QTabBar::tab:selected {
            background-color: white;
            color: #4285f4;
            border-bottom: 3px solid #4285f4;
            font-weight: 600;
        }

        QTabBar::tab:hover:!selected {
            background-color: rgba(66, 133, 244, 0.1);
            color: #4285f4;
        }

        QGroupBox {
            border: 2px solid #e2e8f0;
            border-radius: 0px;
            margin-top: 16px;
            padding-top: 16px;
            background-color: rgba(255, 255, 255, 0.6);
            font-weight: 600;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 12px;
            color: #4285f4;
            font-size: 15px;
            font-weight: 700;
            background: rgba(255, 255, 255, 0.9);
            border-radius: 0px;
            margin-top: -8px;
        }

        QLabel {
            color: #1f2937;
        }

        QProgressBar {
            border: 2px solid #e2e8f0;
            border-radius: 0px;
            text-align: center;
            background-color: white;
            height: 20px;
        }

        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(66, 133, 244, 1),
                stop:1 rgba(52, 121, 193, 1));
            border-radius: 0px;
        }

        QComboBox {
            background-color: white;
            border: 2px solid #e2e8f0;
            border-radius: 0px;
            padding: 8px 12px;
            font-size: 14px;
            color: #1f2937;
        }

        QComboBox:hover {
            border: 2px solid #4285f4;
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        QComboBox::down-arrow {
            image: url(none);
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 6px solid #64748b;
            margin-right: 5px;
        }

        QCheckBox {
            spacing: 8px;
            color: #1f2937;
        }

        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #cbd5e1;
            border-radius: 0px;
            background-color: white;
        }

        QCheckBox::indicator:hover {
            border: 2px solid #4285f4;
        }

        QCheckBox::indicator:checked {
            background-color: #4285f4;
            border: 2px solid #4285f4;
            image: url(none);
        }

        QSpinBox, QDoubleSpinBox {
            background-color: white;
            border: 2px solid #e2e8f0;
            border-radius: 0px;
            padding: 8px 12px;
            font-size: 14px;
            color: #1f2937;
        }

        QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #4285f4;
            background-color: #f8fafc;
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button,
        QSpinBox::down-button, QDoubleSpinBox::down-button {
            background-color: #f1f5f9;
            border: none;
            width: 20px;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover,
        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background-color: #e2e8f0;
        }
    )";
}

QString ThemeManager::getDarkThemeStyle() const
{
    return R"(
        * {
            background-color: #1e293b;
            color: #e2e8f0;
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
        }

        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(30, 41, 59, 1),
                stop:0.5 rgba(15, 23, 42, 1),
                stop:1 rgba(10, 15, 30, 1));
            border-radius: 0px;
            border: 1px solid rgba(148, 163, 184, 0.2);
        }

        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(59, 130, 246, 1),
                stop:1 rgba(37, 99, 235, 1));
            color: white;
            border: none;
            border-radius: 0px;
            padding: 10px 20px;
            font-size: 14px;
            font-weight: 600;
            min-height: 16px;
        }

        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(37, 99, 235, 1),
                stop:1 rgba(29, 78, 216, 1));
            transform: translateY(-2px);
            box-shadow: 0 6px 20px rgba(59, 130, 246, 0.3);
        }

        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(29, 78, 216, 1),
                stop:1 rgba(30, 64, 175, 1));
            transform: translateY(0px);
            box-shadow: 0 2px 8px rgba(59, 130, 246, 0.2);
        }

        QPushButton:disabled {
            background: rgba(71, 85, 105, 0.5);
            color: rgba(148, 163, 184, 0.6);
            box-shadow: none;
            transform: none;
        }

        QLineEdit, QTextEdit {
            background-color: #0f172a;
            border: 2px solid #1e293b;
            border-radius: 0px;
            padding: 10px 14px;
            font-size: 14px;
            color: #e2e8f0;
        }

        QLineEdit:focus, QTextEdit:focus {
            border: 2px solid #3b82f6;
            background-color: #1e293b;
            box-shadow: 0 0 0 4px rgba(59, 130, 246, 0.2);
        }

        QTabWidget::pane {
            border: 2px solid #1e293b;
            background-color: #0f172a;
            border-radius: 0px;
            top: -1px;
        }

        QTabBar::tab {
            background-color: rgba(30, 41, 59, 0.8);
            color: #94a3b8;
            padding: 10px 20px;
            border-top-left-radius: 0px;
            border-top-right-radius: 0px;
            margin-right: 2px;
            font-weight: 500;
            min-width: 80px;
        }

        QTabBar::tab:selected {
            background-color: #0f172a;
            color: #60a5fa;
            border-bottom: 3px solid #3b82f6;
            font-weight: 600;
        }

        QTabBar::tab:hover:!selected {
            background-color: rgba(59, 130, 246, 0.15);
            color: #60a5fa;
        }

        QGroupBox {
            border: 2px solid #1e293b;
            border-radius: 0px;
            margin-top: 16px;
            padding-top: 16px;
            background-color: rgba(30, 41, 59, 0.6);
            font-weight: 600;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 12px;
            color: #60a5fa;
            font-size: 15px;
            font-weight: 700;
            background: rgba(15, 23, 42, 0.9);
            border-radius: 0px;
            margin-top: -8px;
        }

        QLabel {
            color: #e2e8f0;
        }

        QProgressBar {
            border: 2px solid #1e293b;
            border-radius: 0px;
            text-align: center;
            background-color: #0f172a;
            height: 20px;
        }

        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(59, 130, 246, 1),
                stop:1 rgba(37, 99, 235, 1));
            border-radius: 0px;
        }

        QComboBox {
            background-color: #0f172a;
            border: 2px solid #1e293b;
            border-radius: 0px;
            padding: 8px 12px;
            font-size: 14px;
            color: #e2e8f0;
        }

        QComboBox:hover {
            border: 2px solid #3b82f6;
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        QComboBox::down-arrow {
            image: url(none);
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 6px solid #94a3b8;
            margin-right: 5px;
        }

        QCheckBox {
            spacing: 8px;
            color: #e2e8f0;
        }

        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #334155;
            border-radius: 0px;
            background-color: #0f172a;
        }

        QCheckBox::indicator:hover {
            border: 2px solid #3b82f6;
        }

        QCheckBox::indicator:checked {
            background-color: #3b82f6;
            border: 2px solid #3b82f6;
            image: url(none);
        }

        QSpinBox, QDoubleSpinBox {
            background-color: #0f172a;
            border: 2px solid #1e293b;
            border-radius: 0px;
            padding: 8px 12px;
            font-size: 14px;
            color: #e2e8f0;
        }

        QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #3b82f6;
            background-color: #1e293b;
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button,
        QSpinBox::down-button, QDoubleSpinBox::down-button {
            background-color: #1e293b;
            border: none;
            width: 20px;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover,
        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background-color: #334155;
        }
    )";
}

QString ThemeManager::getTechThemeStyle() const
{
    return R"(
        * {
            background-color: #0a0e27;
            color: #00ffff;
            font-family: 'Consolas', 'Monaco', 'Courier New', monospace;
        }

        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(10, 14, 39, 1),
                stop:0.5 rgba(5, 10, 30, 1),
                stop:1 rgba(0, 5, 20, 1));
            border-radius: 0px;
            border: 2px solid rgba(0, 255, 255, 0.3);
        }

        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(0, 255, 255, 200),
                stop:1 rgba(0, 200, 255, 200));
            color: #0a0e27;
            border: 2px solid #00ffff;
            border-radius: 0px;
            padding: 10px 20px;
            font-size: 14px;
            font-weight: bold;
            min-height: 16px;
        }

        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(0, 200, 255, 220),
                stop:1 rgba(0, 255, 255, 220));
            border-color: #00cccc;
            box-shadow: 0 0 20px rgba(0, 255, 255, 0.5);
        }

        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(0, 150, 200, 220),
                stop:1 rgba(0, 200, 255, 220));
            border-color: #008888;
            box-shadow: 0 0 10px rgba(0, 255, 255, 0.3);
        }

        QPushButton:disabled {
            background: rgba(0, 255, 255, 30);
            color: rgba(0, 136, 136, 0.6);
            border-color: #008888;
            box-shadow: none;
        }

        QLineEdit, QTextEdit {
            background-color: rgba(0, 20, 40, 220);
            border: 2px solid #00ffff;
            border-radius: 0px;
            padding: 10px 14px;
            font-size: 14px;
            color: #00ffff;
        }

        QLineEdit:focus, QTextEdit:focus {
            border-color: #00cccc;
            background-color: rgba(0, 30, 60, 220);
            box-shadow: 0 0 15px rgba(0, 255, 255, 0.4);
        }

        QLineEdit:hover, QTextEdit:hover {
            border-color: #00ffff;
            box-shadow: 0 0 10px rgba(0, 255, 255, 0.3);
        }

        QTabWidget::pane {
            border: 2px solid #00ffff;
            background-color: rgba(0, 20, 40, 220);
            border-radius: 0px;
            top: -1px;
        }

        QTabBar::tab {
            background-color: rgba(0, 20, 40, 220);
            color: #00ffff;
            padding: 10px 20px;
            border-top-left-radius: 0px;
            border-top-right-radius: 0px;
            margin-right: 2px;
            border: 1px solid #00ffff;
        }

        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(0, 255, 255, 200),
                stop:1 rgba(0, 200, 255, 200));
            color: #0a0e27;
            font-weight: bold;
        }

        QTabBar::tab:hover:!selected {
            background-color: rgba(0, 40, 80, 220);
            box-shadow: 0 0 10px rgba(0, 255, 255, 0.2);
        }

        QGroupBox {
            border: 2px solid #00ffff;
            border-radius: 0px;
            margin-top: 16px;
            padding-top: 16px;
            background-color: rgba(0, 20, 40, 180);
            font-weight: bold;
            color: #00ffff;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 12px;
            color: #00ffff;
            font-size: 15px;
            font-weight: bold;
            background: rgba(0, 40, 80, 200);
            border-radius: 0px;
            margin-top: -8px;
        }

        QLabel {
            color: #00ffff;
        }

        QProgressBar {
            border: 2px solid #00ffff;
            border-radius: 0px;
            text-align: center;
            background-color: rgba(0, 20, 40, 220);
            height: 20px;
            color: #00ffff;
        }

        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(0, 255, 255, 200),
                stop:1 rgba(0, 200, 255, 200));
            border-radius: 0px;
            box-shadow: 0 0 10px rgba(0, 255, 255, 0.5);
        }

        QComboBox {
            background-color: rgba(0, 20, 40, 220);
            border: 2px solid #00ffff;
            border-radius: 0px;
            padding: 8px 12px;
            font-size: 14px;
            color: #00ffff;
        }

        QComboBox:hover {
            border-color: #00cccc;
            background-color: rgba(0, 30, 60, 220);
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        QComboBox::down-arrow {
            image: url(none);
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 6px solid #00ffff;
            margin-right: 5px;
        }

        QCheckBox {
            spacing: 8px;
            color: #00ffff;
        }

        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #00ffff;
            border-radius: 0px;
            background-color: rgba(0, 20, 40, 220);
        }

        QCheckBox::indicator:hover {
            border-color: #00cccc;
            background-color: rgba(0, 30, 60, 220);
        }

        QCheckBox::indicator:checked {
            background-color: #00ffff;
            border: 2px solid #00ffff;
            image: url(none);
        }

        QSpinBox, QDoubleSpinBox {
            background-color: rgba(0, 20, 40, 220);
            border: 2px solid #00ffff;
            border-radius: 0px;
            padding: 8px 12px;
            font-size: 14px;
            color: #00ffff;
        }

        QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: #00cccc;
            background-color: rgba(0, 30, 60, 220);
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button,
        QSpinBox::down-button, QDoubleSpinBox::down-button {
            background-color: rgba(0, 40, 80, 220);
            border: none;
            width: 20px;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover,
        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background-color: rgba(0, 60, 120, 220);
        }
    )";
}

QString ThemeManager::getSakuraThemeStyle() const
{
    return R"(
        * {
            background-color: #fff0f5;
            color: #d63384;
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
        }

        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(255, 240, 245, 1),
                stop:0.3 rgba(255, 230, 235, 1),
                stop:0.6 rgba(255, 220, 230, 1),
                stop:1 rgba(255, 210, 220, 1));
            border-radius: 0px;
            border: 1px solid rgba(214, 51, 132, 0.3);
        }

        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(255, 182, 193, 1),
                stop:1 rgba(255, 209, 220, 1));
            color: white;
            border: none;
            border-radius: 0px;
            padding: 10px 20px;
            font-size: 14px;
            font-weight: 600;
            min-height: 16px;
        }

        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(255, 209, 220, 1),
                stop:1 rgba(255, 182, 193, 1));
            transform: translateY(-2px);
            box-shadow: 0 6px 20px rgba(255, 182, 193, 0.4);
        }

        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(255, 167, 181, 1),
                stop:1 rgba(255, 182, 193, 1));
            transform: translateY(0px);
            box-shadow: 0 2px 8px rgba(255, 182, 193, 0.3);
        }

        QPushButton:disabled {
            background: rgba(255, 182, 193, 0.3);
            color: rgba(214, 51, 132, 0.6);
            box-shadow: none;
            transform: none;
        }

        QLineEdit, QTextEdit {
            background-color: rgba(255, 250, 252, 220);
            border: 2px solid #ffb6c1;
            border-radius: 0px;
            padding: 10px 14px;
            font-size: 14px;
            color: #d63384;
        }

        QLineEdit:focus, QTextEdit:focus {
            border-color: #ff9eb3;
            background-color: rgba(255, 245, 250, 220);
            box-shadow: 0 0 0 4px rgba(255, 182, 193, 0.2);
        }

        QLineEdit:hover, QTextEdit:hover {
            border-color: #ffd1dc;
        }

        QTabWidget::pane {
            border: 2px solid #ffb6c1;
            background-color: rgba(255, 250, 252, 220);
            border-radius: 0px;
            top: -1px;
        }

        QTabBar::tab {
            background-color: rgba(255, 250, 252, 220);
            color: #d63384;
            padding: 10px 20px;
            border-top-left-radius: 0px;
            border-top-right-radius: 0px;
            margin-right: 2px;
            border: 1px solid #ffb6c1;
        }

        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(255, 182, 193, 1),
                stop:1 rgba(255, 209, 220, 1));
            color: white;
            font-weight: 600;
        }

        QTabBar::tab:hover:!selected {
            background-color: rgba(255, 230, 235, 220);
            box-shadow: 0 0 10px rgba(255, 182, 193, 0.2);
        }

        QGroupBox {
            border: 2px solid #ffb6c1;
            border-radius: 0px;
            margin-top: 16px;
            padding-top: 16px;
            background-color: rgba(255, 245, 250, 180);
            font-weight: 600;
            color: #d63384;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 12px;
            color: #d63384;
            font-size: 15px;
            font-weight: 700;
            background: rgba(255, 230, 235, 200);
            border-radius: 0px;
            margin-top: -8px;
        }

        QLabel {
            color: #d63384;
        }

        QProgressBar {
            border: 2px solid #ffb6c1;
            border-radius: 0px;
            text-align: center;
            background-color: rgba(255, 250, 252, 220);
            height: 20px;
            color: #d63384;
        }

        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(255, 182, 193, 1),
                stop:1 rgba(255, 209, 220, 1));
            border-radius: 0px;
            box-shadow: 0 0 10px rgba(255, 182, 193, 0.5);
        }

        QComboBox {
            background-color: rgba(255, 250, 252, 220);
            border: 2px solid #ffb6c1;
            border-radius: 0px;
            padding: 8px 12px;
            font-size: 14px;
            color: #d63384;
        }

        QComboBox:hover {
            border-color: #ff9eb3;
            background-color: rgba(255, 245, 250, 220);
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        QComboBox::down-arrow {
            image: url(none);
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 6px solid #ff9eb3;
            margin-right: 5px;
        }

        QCheckBox {
            spacing: 8px;
            color: #d63384;
        }

        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #d63384;
            border-radius: 0px;
            background-color: rgba(255, 250, 252, 220);
        }

        QCheckBox::indicator:hover {
            border-color: #ff9eb3;
            background-color: rgba(255, 245, 250, 220);
        }

        QCheckBox::indicator:checked {
            background-color: #ffb6c1;
            border: 2px solid #ffb6c1;
            image: url(none);
        }

        QSpinBox, QDoubleSpinBox {
            background-color: rgba(255, 250, 252, 220);
            border: 2px solid #ffb6c1;
            border-radius: 0px;
            padding: 8px 12px;
            font-size: 14px;
            color: #d63384;
        }

        QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: #ff9eb3;
            background-color: rgba(255, 245, 250, 220);
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button,
        QSpinBox::down-button, QDoubleSpinBox::down-button {
            background-color: rgba(255, 230, 235, 220);
            border: none;
            width: 20px;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover,
        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background-color: rgba(255, 220, 230, 220);
        }
    )";
}

QString ThemeManager::getWarmThemeStyle() const
{
    return R"(
        * {
            background-color: #fff8e1;
            color: #5d4037;
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
        }

        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(255, 248, 225, 1),
                stop:0.3 rgba(255, 240, 200, 1),
                stop:0.6 rgba(255, 230, 180, 1),
                stop:1 rgba(255, 215, 160, 1));
            border-radius: 0px;
            border: 1px solid rgba(251, 146, 60, 0.3);
        }

        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(251, 146, 60, 1),
                stop:1 rgba(245, 158, 11, 1));
            color: white;
            border: none;
            border-radius: 0px;
            padding: 10px 20px;
            font-size: 14px;
            font-weight: 600;
            min-height: 16px;
        }

        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(245, 158, 11, 1),
                stop:1 rgba(251, 146, 60, 1));
            transform: translateY(-2px);
            box-shadow: 0 6px 20px rgba(251, 146, 60, 0.4);
        }

        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(217, 119, 6, 1),
                stop:1 rgba(245, 158, 11, 1));
            transform: translateY(0px);
            box-shadow: 0 2px 8px rgba(251, 146, 60, 0.3);
        }

        QPushButton:disabled {
            background: rgba(251, 146, 60, 0.3);
            color: rgba(217, 119, 6, 0.6);
            box-shadow: none;
            transform: none;
        }

        QLineEdit, QTextEdit {
            background-color: rgba(255, 245, 240, 220);
            border: 2px solid #fb923c;
            border-radius: 0px;
            padding: 10px 14px;
            font-size: 14px;
            color: #5d4037;
        }

        QLineEdit:focus, QTextEdit:focus {
            border-color: #f59e0b;
            background-color: rgba(255, 250, 245, 220);
            box-shadow: 0 0 0 4px rgba(251, 146, 60, 0.2);
        }

        QLineEdit:hover, QTextEdit:hover {
            border-color: #fbbf24;
        }

        QTabWidget::pane {
            border: 2px solid #fb923c;
            background-color: rgba(255, 245, 240, 220);
            border-radius: 0px;
            top: -1px;
        }

        QTabBar::tab {
            background-color: rgba(255, 245, 240, 220);
            color: #5d4037;
            padding: 10px 20px;
            border-top-left-radius: 0px;
            border-top-right-radius: 0px;
            margin-right: 2px;
            border: 1px solid #fb923c;
        }

        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(251, 146, 60, 1),
                stop:1 rgba(245, 158, 11, 1));
            color: white;
            font-weight: 600;
        }

        QTabBar::tab:hover:!selected {
            background-color: rgba(255, 230, 200, 220);
            box-shadow: 0 0 10px rgba(251, 146, 60, 0.2);
        }

        QGroupBox {
            border: 2px solid #fb923c;
            border-radius: 0px;
            margin-top: 16px;
            padding-top: 16px;
            background-color: rgba(255, 240, 235, 180);
            font-weight: 600;
            color: #5d4037;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 12px;
            color: #5d4037;
            font-size: 15px;
            font-weight: 700;
            background: rgba(255, 230, 200, 200);
            border-radius: 0px;
            margin-top: -8px;
        }

        QLabel {
            color: #5d4037;
        }

        QProgressBar {
            border: 2px solid #fb923c;
            border-radius: 0px;
            text-align: center;
            background-color: rgba(255, 245, 240, 220);
            height: 20px;
            color: #5d4037;
        }

        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(251, 146, 60, 1),
                stop:1 rgba(245, 158, 11, 1));
            border-radius: 0px;
            box-shadow: 0 0 10px rgba(251, 146, 60, 0.5);
        }

        QComboBox {
            background-color: rgba(255, 245, 240, 220);
            border: 2px solid #fb923c;
            border-radius: 0px;
            padding: 8px 12px;
            font-size: 14px;
            color: #5d4037;
        }

        QComboBox:hover {
            border-color: #f59e0b;
            background-color: rgba(255, 250, 245, 220);
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        QComboBox::down-arrow {
            image: url(none);
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 6px solid #fbbf24;
            margin-right: 5px;
        }

        QCheckBox {
            spacing: 8px;
            color: #5d4037;
        }

        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #92400e;
            border-radius: 0px;
            background-color: rgba(255, 245, 240, 220);
        }

        QCheckBox::indicator:hover {
            border-color: #f59e0b;
            background-color: rgba(255, 250, 245, 220);
        }

        QCheckBox::indicator:checked {
            background-color: #fb923c;
            border: 2px solid #fb923c;
            image: url(none);
        }

        QSpinBox, QDoubleSpinBox {
            background-color: rgba(255, 245, 240, 220);
            border: 2px solid #fb923c;
            border-radius: 0px;
            padding: 8px 12px;
            font-size: 14px;
            color: #5d4037;
        }

        QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: #f59e0b;
            background-color: rgba(255, 250, 245, 220);
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button,
        QSpinBox::down-button, QDoubleSpinBox::down-button {
            background-color: rgba(255, 230, 200, 220);
            border: none;
            width: 20px;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover,
        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background-color: rgba(255, 215, 160, 220);
        }
    )";
}
