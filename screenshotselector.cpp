#include "screenshotselector.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>

ScreenshotSelector::ScreenshotSelector(const QPixmap &screenshot, QWidget *parent)
    : QWidget(parent)
    , m_screenshot(screenshot)
    , m_selecting(false)
    , m_currentTheme(ThemeManager::LightTheme)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::X11BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);

    // 设置窗口大小为屏幕大小
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        setGeometry(screen->geometry());
    }

    setCursor(Qt::CrossCursor);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    // 连接主题变化信号
    ThemeManager *themeManager = ThemeManager::getInstance();
    connect(themeManager, &ThemeManager::themeChanged, this, &ScreenshotSelector::onThemeChanged);
    m_currentTheme = themeManager->getCurrentTheme();

    // 不使用grabMouse()，因为它会阻塞其他输入
    // grabMouse();
}

ScreenshotSelector::~ScreenshotSelector()
{
}

QRect ScreenshotSelector::getSelectedRect() const
{
    return m_selectedRect;
}

QPixmap ScreenshotSelector::getScreenshot() const
{
    return m_screenshot;
}

void ScreenshotSelector::onThemeChanged(ThemeManager::Theme theme)
{
    m_currentTheme = theme;
    update();
}

QColor ScreenshotSelector::getThemeColor() const
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

void ScreenshotSelector::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制半透明背景
    painter.fillRect(rect(), QColor(0, 0, 0, 100));

    // 绘制截图
    if (!m_screenshot.isNull()) {
        painter.drawPixmap(0, 0, m_screenshot);
    }

    // 绘制选择区域
    if (m_selecting || !m_selectedRect.isNull()) {
        drawSelectionRect(painter);
    }

    // 绘制提示文字
    QColor themeColor = getThemeColor();
    painter.setPen(QColor(themeColor.red(), themeColor.green(), themeColor.blue(), 255));
    painter.setFont(QFont("Arial", 12, QFont::Bold));

    QRect currentRect = m_selecting ? getNormalizedRect(m_startPoint, m_endPoint) : m_selectedRect;
    QString infoText = QString("拖拽选择区域: %1 x %2 | 按ESC取消")
                        .arg(currentRect.width())
                        .arg(currentRect.height());

    QRect textRect = painter.fontMetrics().boundingRect(infoText);
    textRect.moveTopLeft(QPoint(20, 40));

    painter.drawText(20, 40, infoText);

    if (m_selecting) {
        QString hintText = "松开鼠标完成选择";
        painter.drawText(20, 60, hintText);
    }
}

void ScreenshotSelector::drawSelectionRect(QPainter &painter)
{
    QRect rect = m_selecting ? getNormalizedRect(m_startPoint, m_endPoint) : m_selectedRect;

    if (rect.isValid()) {
        QColor themeColor = getThemeColor();
        QPen pen(themeColor, 2);
        pen.setStyle(Qt::DashLine);
        pen.setDashPattern(QVector<qreal>() << 5 << 3);
        painter.setPen(pen);
        painter.setBrush(QBrush(QColor(255, 255, 255, 80)));
        painter.drawRect(rect);

        QPen cornerPen(themeColor, 2);
        cornerPen.setStyle(Qt::SolidLine);
        painter.setPen(cornerPen);
        painter.setBrush(QBrush(themeColor));
        painter.setRenderHint(QPainter::Antialiasing);

        int cornerSize = 8;
        int handleSize = 6;

        painter.drawRect(rect.left() - cornerSize/2, rect.top() - cornerSize/2, cornerSize, cornerSize);
        painter.drawRect(rect.right() - cornerSize/2, rect.top() - cornerSize/2, cornerSize, cornerSize);
        painter.drawRect(rect.left() - cornerSize/2, rect.bottom() - cornerSize/2, cornerSize, cornerSize);
        painter.drawRect(rect.right() - cornerSize/2, rect.bottom() - cornerSize/2, cornerSize, cornerSize);

        painter.setBrush(QBrush(themeColor));
        painter.drawEllipse(rect.left() - handleSize/2, rect.top() - handleSize/2, handleSize, handleSize);
        painter.drawEllipse(rect.right() - handleSize/2, rect.top() - handleSize/2, handleSize, handleSize);
        painter.drawEllipse(rect.left() - handleSize/2, rect.bottom() - handleSize/2, handleSize, handleSize);
        painter.drawEllipse(rect.right() - handleSize/2, rect.bottom() - handleSize/2, handleSize, handleSize);
    }
}

void ScreenshotSelector::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_startPoint = event->pos();
        m_endPoint = event->pos();
        m_selecting = true;
        update();
    }
}

void ScreenshotSelector::mouseMoveEvent(QMouseEvent *event)
{
    if (m_selecting) {
        m_endPoint = event->pos();
        update();
    }
}

void ScreenshotSelector::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_selecting) {
        m_endPoint = event->pos();
        m_selectedRect = getNormalizedRect(m_startPoint, m_endPoint);
        m_selecting = false;

        if (m_selectedRect.isValid() && m_selectedRect.width() > 10 && m_selectedRect.height() > 10) {
            emit regionSelected(m_selectedRect);
        } else {
            // 选择区域太小，取消选择
            m_selectedRect = QRect();
            update();
        }
    }
}

void ScreenshotSelector::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        emit cancelled();
    } else {
        QWidget::keyPressEvent(event);
    }
}

QRect ScreenshotSelector::getNormalizedRect(const QPoint &start, const QPoint &end) const
{
    int left = qMin(start.x(), end.x());
    int top = qMin(start.y(), end.y());
    int width = qAbs(start.x() - end.x());
    int height = qAbs(start.y() - end.y());

    return QRect(left, top, width, height);
}
