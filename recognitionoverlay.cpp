#include "recognitionoverlay.h"
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QTimer>
#include <QDebug>
#include <windows.h>

RecognitionOverlay::RecognitionOverlay(QWidget *parent)
    : QWidget(parent)
    , m_currentTheme(ThemeManager::LightTheme)
{
    // 设置窗口属性
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_PaintOnScreen);

    // 获取所有屏幕的物理尺寸
    int screenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    int screenX = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int screenY = GetSystemMetrics(SM_YVIRTUALSCREEN);

    // 设置窗口大小为物理屏幕尺寸
    setGeometry(screenX, screenY, screenWidth, screenHeight);

    // 初始隐藏
    hide();

    // 添加定时器，用于平滑更新识别框
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, [this]() {
        update();
    });
    m_updateTimer->setInterval(8); // 约120fps，提高更新速度
    m_updateTimer->start();

    // 连接主题变化信号
    ThemeManager *themeManager = ThemeManager::getInstance();
    connect(themeManager, &ThemeManager::themeChanged, this, &RecognitionOverlay::onThemeChanged);
    m_currentTheme = themeManager->getCurrentTheme();
}

RecognitionOverlay::~RecognitionOverlay() {
    if (m_updateTimer) {
        m_updateTimer->stop();
        delete m_updateTimer;
    }
}

void RecognitionOverlay::onThemeChanged(ThemeManager::Theme theme)
{
    m_currentTheme = theme;
    update();
}

QColor RecognitionOverlay::getThemeColor() const
{
    switch (m_currentTheme) {
    case ThemeManager::LightTheme:
        return QColor(0, 122, 255);
    case ThemeManager::DarkTheme:
        return QColor(14, 99, 156);
    case ThemeManager::TechTheme:
        return QColor(0, 255, 255);
    case ThemeManager::SakuraTheme:
        return QColor(214, 51, 132);
    case ThemeManager::WarmTheme:
        return QColor(251, 146, 60);
    default:
        return QColor(0, 255, 255);
    }
}

void RecognitionOverlay::drawRecognitionArea(const QRect &rect, const QColor &color) {
    QMutexLocker locker(&m_mutex);

    // 清除之前的识别区域，确保每次只显示最新的识别区域
    m_recognitionAreas.clear();
    m_colors.clear();

    // 获取屏幕尺寸，确保识别区域在屏幕范围内
    QRect desktopRect;
    for (QScreen *screen : QApplication::screens()) {
        desktopRect = desktopRect.united(screen->geometry());
    }

    // 验证并修复识别区域，确保位置准确
    QRect validRect = rect;

    // 确保识别区域在屏幕范围内
    validRect = validRect.intersected(desktopRect);

    // 确保识别区域有有效尺寸
    if (validRect.width() <= 0 || validRect.height() <= 0) {
        qDebug() << "无效的识别区域尺寸，跳过绘制:" << rect;
        return;
    }

    // 记录原始区域和修复后的区域，用于调试
    qDebug() << "原始识别区域:" << rect << "，修复后:" << validRect;

    // 添加新的识别区域，确保位置准确
    m_recognitionAreas.append(validRect);
    m_colors.append(color);

    // 显示并置顶窗口
    if (!isVisible()) {
        show();
        raise();
    }

    // 更新整个窗口，确保识别框正确显示
    update();
}

void RecognitionOverlay::clearRecognitionAreas() {
    QMutexLocker locker(&m_mutex);
    m_recognitionAreas.clear();
    m_colors.clear();
    hide();
}

void RecognitionOverlay::hide() {
    QWidget::hide();
}

void RecognitionOverlay::show() {
    QWidget::show();
    raise();
}

void RecognitionOverlay::paintEvent(QPaintEvent *event) {
    QMutexLocker locker(&m_mutex);

    // 如果没有识别区域，直接返回
    if (m_recognitionAreas.isEmpty()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // 绘制所有识别区域
    for (int i = 0; i < m_recognitionAreas.size(); ++i) {
        const QRect &rect = m_recognitionAreas[i];
        QColor color = m_colors[i];
        if (color == QColor()) {
            color = getThemeColor(); // 使用主题颜色作为默认颜色
        }

        // 设置画笔，使用更醒目的科技风格
        QPen pen(color);
        pen.setWidth(2);
        pen.setStyle(Qt::SolidLine);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(pen);

        // 绘制矩形边框，使用科技风格的虚线
        painter.drawRect(rect);

        // 绘制填充半透明矩形，使用更科技感的透明度
        QBrush brush(color);
        brush.setStyle(Qt::SolidPattern);
        painter.setBrush(brush);
        painter.setOpacity(0.1);
        painter.drawRect(rect);
        painter.setOpacity(1.0);

        // 绘制四角的标记，增强科技感
        int cornerSize = 15;
        int cornerLineWidth = 3;

        // 左上角
        painter.setPen(QPen(color, cornerLineWidth));
        painter.drawLine(rect.left(), rect.top(), rect.left() + cornerSize, rect.top());
        painter.drawLine(rect.left(), rect.top(), rect.left(), rect.top() + cornerSize);

        // 右上角
        painter.drawLine(rect.right() - cornerSize, rect.top(), rect.right(), rect.top());
        painter.drawLine(rect.right(), rect.top(), rect.right(), rect.top() + cornerSize);

        // 左下角
        painter.drawLine(rect.left(), rect.bottom() - cornerSize, rect.left(), rect.bottom());
        painter.drawLine(rect.left(), rect.bottom(), rect.left() + cornerSize, rect.bottom());

        // 右下角
        painter.drawLine(rect.right() - cornerSize, rect.bottom(), rect.right(), rect.bottom());
        painter.drawLine(rect.right(), rect.bottom() - cornerSize, rect.right(), rect.bottom());
    }

    QWidget::paintEvent(event);
}
