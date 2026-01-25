#include "framelesswindow.h"
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QStyle>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QDebug>

FramelessWindow::FramelessWindow(QWidget *parent)
    : QWidget(parent)
    , m_titleBar(nullptr)
    , m_titleLabel(nullptr)
    , m_iconLabel(nullptr)
    , m_minimizeButton(nullptr)
    , m_maximizeButton(nullptr)
    , m_closeButton(nullptr)
    , m_centralWidget(nullptr)
    , m_isMaximized(false)
    , m_shadowEnabled(true)
    , m_dragging(false)
    , m_resizing(false)
    , m_resizeDirection(None)
    , m_currentTheme(ThemeManager::LightTheme)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_Hover, true);
    setMouseTracking(true);

    // 设置窗口最小尺寸
    setMinimumSize(600, 400);

    // 创建主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(1, 1, 1, 1); // 为阴影留出空间
    m_mainLayout->setSpacing(0);

    // 创建标题栏
    createTitleBar();

    // 设置窗口大小
    resize(1000, 800);

    // 居中显示
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        move((screenGeometry.width() - width()) / 2,
             (screenGeometry.height() - height()) / 2);
    }

    // 连接主题变化信号
    ThemeManager *themeManager = ThemeManager::getInstance();
    connect(themeManager, &ThemeManager::themeChanged, this, &FramelessWindow::onThemeChanged);
    m_currentTheme = themeManager->getCurrentTheme();
}

FramelessWindow::~FramelessWindow()
{
}

void FramelessWindow::createTitleBar()
{
    // 创建标题栏
    m_titleBar = new QWidget(this);
    m_titleBar->setFixedHeight(TITLE_BAR_HEIGHT);
    m_titleBar->setMouseTracking(true);

    // 创建标题栏布局
    m_titleLayout = new QHBoxLayout(m_titleBar);
    m_titleLayout->setContentsMargins(12, 0, 12, 0);
    m_titleLayout->setSpacing(8);

    // 窗口图标
    m_iconLabel = new QLabel(m_titleBar);
    m_iconLabel->setFixedSize(20, 20);
    m_iconLabel->setScaledContents(true);
    m_titleLayout->addWidget(m_iconLabel);

    // 窗口标题
    m_titleLabel = new QLabel("WeBot", m_titleBar);
    m_titleLayout->addWidget(m_titleLabel);

    // 弹簧
    m_titleLayout->addStretch();

    // 控制按钮
    m_minimizeButton = createTitleButton("−", "最小化");
    connect(m_minimizeButton, &QPushButton::clicked, this, &FramelessWindow::onMinimizeClicked);

    m_maximizeButton = createTitleButton("□", "最大化");
    connect(m_maximizeButton, &QPushButton::clicked, this, &FramelessWindow::onMaximizeClicked);

    m_closeButton = createTitleButton("×", "关闭");
    connect(m_closeButton, &QPushButton::clicked, this, &FramelessWindow::onCloseClicked);

    m_titleLayout->addWidget(m_minimizeButton);
    m_titleLayout->addWidget(m_maximizeButton);
    m_titleLayout->addWidget(m_closeButton);

    m_mainLayout->addWidget(m_titleBar);

    // 应用主题样式
    updateTitleBarStyle();
}

QPushButton* FramelessWindow::createTitleButton(const QString &text, const QString &tooltip)
{
    QPushButton *button = new QPushButton(text, m_titleBar);
    button->setFixedSize(36, 28);
    button->setToolTip(tooltip);
    button->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            color: white;
            border: none;
            font-size: 14px;
            padding: 4px;
            border-radius: 5px;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.1);
        }
        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 0.2);
        }
    )");
    return button;
}

void FramelessWindow::setWindowTitle(const QString &title)
{
    if (m_titleLabel) {
        m_titleLabel->setText(title);
    }
    QWidget::setWindowTitle(title);
}

void FramelessWindow::setCentralWidget(QWidget *widget)
{
    if (m_centralWidget) {
        m_mainLayout->removeWidget(m_centralWidget);
        m_centralWidget->hide();
    }

    m_centralWidget = widget;
    if (m_centralWidget) {
        m_centralWidget->setParent(this);
        m_mainLayout->addWidget(m_centralWidget);
        m_centralWidget->show();
    }
}

void FramelessWindow::setWindowIcon(const QIcon &icon)
{
    if (m_iconLabel) {
        m_iconLabel->setPixmap(icon.pixmap(20, 20));
    }
    QWidget::setWindowIcon(icon);
}

void FramelessWindow::onThemeChanged(ThemeManager::Theme theme)
{
    m_currentTheme = theme;
    updateTitleBarStyle();
    update();
}

void FramelessWindow::updateTitleBarStyle()
{
    QString titleBarStyle;
    QString buttonStyle;
    QString closeButtonStyle;

    switch (m_currentTheme) {
    case ThemeManager::LightTheme:
        titleBarStyle = R"(
            QWidget {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 rgba(248, 250, 252, 0.98),
                    stop:1 rgba(241, 243, 245, 0.95));
                border-bottom: 1px solid rgba(218, 220, 224, 0.5);
                border-radius: 5px 5px 0 0;
            }
        )";
        buttonStyle = R"(
            QPushButton {
                background-color: transparent;
                color: #333;
                border: none;
                font-size: 14px;
                padding: 4px;
                border-radius: 5px;
            }
            QPushButton:hover {
                background-color: rgba(0, 122, 255, 0.1);
            }
            QPushButton:pressed {
                background-color: rgba(0, 122, 255, 0.2);
            }
        )";
        closeButtonStyle = R"(
            QPushButton {
                background-color: transparent;
                color: #333;
                border: none;
                font-size: 16px;
                font-weight: bold;
                padding: 8px 12px;
                border-radius: 5px;
            }
            QPushButton:hover {
                background-color: rgba(248, 113, 113, 0.1);
            }
            QPushButton:pressed {
                background-color: rgba(220, 38, 38, 0.2);
            }
        )";
        break;

    case ThemeManager::DarkTheme:
        titleBarStyle = R"(
            QWidget {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 rgba(30, 30, 30, 0.98),
                    stop:1 rgba(25, 25, 25, 0.95));
                border-bottom: 1px solid rgba(100, 100, 100, 0.5);
                border-radius: 5px 5px 0 0;
            }
        )";
        buttonStyle = R"(
            QPushButton {
                background-color: transparent;
                color: #ccc;
                border: none;
                font-size: 14px;
                padding: 4px;
                border-radius: 5px;
            }
            QPushButton:hover {
                background-color: rgba(14, 99, 156, 0.1);
            }
            QPushButton:pressed {
                background-color: rgba(14, 99, 156, 0.2);
            }
        )";
        closeButtonStyle = R"(
            QPushButton {
                background-color: transparent;
                color: #ccc;
                border: none;
                font-size: 16px;
                font-weight: bold;
                padding: 8px 12px;
                border-radius: 5px;
            }
            QPushButton:hover {
                background-color: rgba(248, 113, 113, 0.1);
            }
            QPushButton:pressed {
                background-color: rgba(220, 38, 38, 0.2);
            }
        )";
        break;

    case ThemeManager::TechTheme:
        titleBarStyle = R"(
            QWidget {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 rgba(10, 14, 39, 0.98),
                    stop:1 rgba(5, 8, 20, 0.95));
                border-bottom: 1px solid rgba(0, 255, 255, 0.5);
                border-radius: 5px 5px 0 0;
            }
        )";
        buttonStyle = R"(
            QPushButton {
                background-color: transparent;
                color: #00ffff;
                border: none;
                font-size: 14px;
                padding: 4px;
                border-radius: 5px;
            }
            QPushButton:hover {
                background-color: rgba(0, 255, 255, 0.1);
            }
            QPushButton:pressed {
                background-color: rgba(0, 255, 255, 0.2);
            }
        )";
        closeButtonStyle = R"(
            QPushButton {
                background-color: transparent;
                color: #00ffff;
                border: none;
                font-size: 16px;
                font-weight: bold;
                padding: 8px 12px;
                border-radius: 5px;
            }
            QPushButton:hover {
                background-color: rgba(0, 255, 255, 0.1);
            }
            QPushButton:pressed {
                background-color: rgba(0, 255, 255, 0.2);
            }
        )";
        break;

    case ThemeManager::SakuraTheme:
        titleBarStyle = R"(
            QWidget {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 rgba(255, 240, 245, 0.98),
                    stop:1 rgba(255, 228, 225, 0.95));
                border-bottom: 1px solid rgba(214, 51, 132, 0.5);
                border-radius: 5px 5px 0 0;
            }
        )";
        buttonStyle = R"(
            QPushButton {
                background-color: transparent;
                color: #d63384;
                border: none;
                font-size: 14px;
                padding: 4px;
                border-radius: 5px;
            }
            QPushButton:hover {
                background-color: rgba(214, 51, 132, 0.1);
            }
            QPushButton:pressed {
                background-color: rgba(214, 51, 132, 0.2);
            }
        )";
        closeButtonStyle = R"(
            QPushButton {
                background-color: transparent;
                color: #d63384;
                border: none;
                font-size: 16px;
                font-weight: bold;
                padding: 8px 12px;
                border-radius: 5px;
            }
            QPushButton:hover {
                background-color: rgba(214, 51, 132, 0.1);
            }
            QPushButton:pressed {
                background-color: rgba(214, 51, 132, 0.2);
            }
        )";
        break;

    case ThemeManager::WarmTheme:
        titleBarStyle = R"(
            QWidget {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 rgba(255, 248, 225, 0.98),
                    stop:1 rgba(255, 235, 205, 0.95));
                border-bottom: 1px solid rgba(251, 146, 60, 0.5);
                border-radius: 5px 5px 0 0;
            }
        )";
        buttonStyle = R"(
            QPushButton {
                background-color: transparent;
                color: #fb923c;
                border: none;
                font-size: 14px;
                padding: 4px;
                border-radius: 5px;
            }
            QPushButton:hover {
                background-color: rgba(251, 146, 60, 0.1);
            }
            QPushButton:pressed {
                background-color: rgba(251, 146, 60, 0.2);
            }
        )";
        closeButtonStyle = R"(
            QPushButton {
                background-color: transparent;
                color: #fb923c;
                border: none;
                font-size: 16px;
                font-weight: bold;
                padding: 8px 12px;
                border-radius: 5px;
            }
            QPushButton:hover {
                background-color: rgba(251, 146, 60, 0.1);
            }
            QPushButton:pressed {
                background-color: rgba(251, 146, 60, 0.2);
            }
        )";
        break;

    default:
        titleBarStyle = R"(
            QWidget {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 rgba(10, 14, 39, 0.98),
                    stop:1 rgba(5, 8, 20, 0.95));
                border-bottom: 1px solid rgba(0, 255, 255, 0.5);
                border-radius: 5px 5px 0 0;
            }
        )";
        buttonStyle = R"(
            QPushButton {
                background-color: transparent;
                color: #00ffff;
                border: none;
                font-size: 14px;
                padding: 4px;
                border-radius: 5px;
            }
            QPushButton:hover {
                background-color: rgba(0, 255, 255, 0.1);
            }
            QPushButton:pressed {
                background-color: rgba(0, 255, 255, 0.2);
            }
        )";
        closeButtonStyle = R"(
            QPushButton {
                background-color: transparent;
                color: #00ffff;
                border: none;
                font-size: 16px;
                font-weight: bold;
                padding: 8px 12px;
                border-radius: 5px;
            }
            QPushButton:hover {
                background-color: rgba(0, 255, 255, 0.1);
            }
            QPushButton:pressed {
                background-color: rgba(0, 255, 255, 0.2);
            }
        )";
        break;
    }

    if (m_titleBar) {
        m_titleBar->setStyleSheet(titleBarStyle);
    }

    if (m_minimizeButton) {
        m_minimizeButton->setStyleSheet(buttonStyle);
    }

    if (m_maximizeButton) {
        m_maximizeButton->setStyleSheet(buttonStyle);
    }

    if (m_closeButton) {
        m_closeButton->setStyleSheet(closeButtonStyle);
    }

    if (m_titleLabel) {
        QString labelColor;
        switch (m_currentTheme) {
        case ThemeManager::LightTheme:
            labelColor = "#333";
            break;
        case ThemeManager::DarkTheme:
            labelColor = "#ccc";
            break;
        case ThemeManager::TechTheme:
            labelColor = "#00ffff";
            break;
        case ThemeManager::SakuraTheme:
            labelColor = "#d63384";
            break;
        case ThemeManager::WarmTheme:
            labelColor = "#fb923c";
            break;
        default:
            labelColor = "#00ffff";
            break;
        }
        m_titleLabel->setStyleSheet(QString("color: %1; font-size: 14px; font-weight: 500;").arg(labelColor));
    }
}

void FramelessWindow::showMinimized()
{
    QWidget::showMinimized();
}

void FramelessWindow::showMaximized()
{
    if (!m_isMaximized) {
        m_normalGeometry = geometry();
        QWidget::showMaximized();
        m_isMaximized = true;
        m_maximizeButton->setText("❐");
        m_maximizeButton->setToolTip("还原");
    } else {
        showNormal();
    }
}

void FramelessWindow::showNormal()
{
    if (m_isMaximized) {
        setGeometry(m_normalGeometry);
        m_isMaximized = false;
        m_maximizeButton->setText("□");
        m_maximizeButton->setToolTip("最大化");
    }
}

void FramelessWindow::close()
{
    QWidget::close();
}

void FramelessWindow::setShadowEnabled(bool enabled)
{
    m_shadowEnabled = enabled;
    update();
}

bool FramelessWindow::isShadowEnabled() const
{
    return m_shadowEnabled;
}

void FramelessWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->pos();

        // 检查是否在标题栏
        if (m_titleBar && m_titleBar->geometry().contains(pos)) {
            // 检查是否点击了控制按钮
            if (!m_minimizeButton->geometry().contains(pos) &&
                !m_maximizeButton->geometry().contains(pos) &&
                !m_closeButton->geometry().contains(pos)) {
                // 开始拖拽
                m_dragging = true;
                m_dragStartPos = event->globalPosition().toPoint() - frameGeometry().topLeft();
                setCursor(Qt::ClosedHandCursor);
            }
        } else {
            // 检查是否在边框上（用于拉伸）
            ResizeDirection direction = getResizeDirection(pos);
            if (direction != None) {
                m_resizing = true;
                m_resizeDirection = direction;
                m_resizeStartPos = event->globalPosition().toPoint();
                m_resizeStartGeometry = geometry();
                updateCursor(direction);
            }
        }
    }

    QWidget::mousePressEvent(event);
}

void FramelessWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && !m_isMaximized) {
        // 拖拽窗口
        QPoint newPos = event->globalPosition().toPoint() - m_dragStartPos;
        move(newPos);
    } else if (m_resizing) {
        // 拉伸窗口
        performResize(event->globalPosition().toPoint());
    } else {
        // 更新鼠标光标
        ResizeDirection direction = getResizeDirection(event->pos());
        updateCursor(direction);
    }

    QWidget::mouseMoveEvent(event);
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        m_resizing = false;
        setCursor(Qt::ArrowCursor);
    }

    QWidget::mouseReleaseEvent(event);
}

void FramelessWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton &&
        m_titleBar && m_titleBar->geometry().contains(event->pos())) {
        // 双击标题栏最大化/还原
        showMaximized();
    }

    QWidget::mouseDoubleClickEvent(event);
}

FramelessWindow::ResizeDirection FramelessWindow::getResizeDirection(const QPoint &pos) const
{
    int x = pos.x();
    int y = pos.y();
    int w = width();
    int h = height();

    ResizeDirection direction = None;

    // 检查左右边框
    if (x <= BORDER_WIDTH) {
        direction = (ResizeDirection)(direction | Left);
    } else if (x >= w - BORDER_WIDTH) {
        direction = (ResizeDirection)(direction | Right);
    }

    // 检查上下边框
    if (y <= BORDER_WIDTH) {
        direction = (ResizeDirection)(direction | Top);
    } else if (y >= h - BORDER_WIDTH) {
        direction = (ResizeDirection)(direction | Bottom);
    }

    return direction;
}

void FramelessWindow::updateCursor(ResizeDirection direction)
{
    switch (direction) {
    case Left:
    case Right:
        setCursor(Qt::SizeHorCursor);
        break;
    case Top:
    case Bottom:
        setCursor(Qt::SizeVerCursor);
        break;
    case TopLeft:
    case BottomRight:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case TopRight:
    case BottomLeft:
        setCursor(Qt::SizeBDiagCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}

void FramelessWindow::performResize(const QPoint &currentPos)
{
    QPoint delta = currentPos - m_resizeStartPos;
    QRect newGeometry = m_resizeStartGeometry;

    if (m_resizeDirection & Left) {
        newGeometry.setLeft(newGeometry.left() + delta.x());
    }
    if (m_resizeDirection & Right) {
        newGeometry.setRight(newGeometry.right() + delta.x());
    }
    if (m_resizeDirection & Top) {
        newGeometry.setTop(newGeometry.top() + delta.y());
    }
    if (m_resizeDirection & Bottom) {
        newGeometry.setBottom(newGeometry.bottom() + delta.y());
    }

    // 限制最小尺寸
    if (newGeometry.width() < 400) {
        if (m_resizeDirection & Left) {
            newGeometry.setLeft(newGeometry.right() - 400);
        } else {
            newGeometry.setRight(newGeometry.left() + 400);
        }
    }
    if (newGeometry.height() < 300) {
        if (m_resizeDirection & Top) {
            newGeometry.setTop(newGeometry.bottom() - 300);
        } else {
            newGeometry.setBottom(newGeometry.top() + 300);
        }
    }

    setGeometry(newGeometry);
}

void FramelessWindow::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (m_shadowEnabled) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // macOS风格：更柔和、更扩散的阴影
        QColor shadowColor(0, 0, 0, 60);
        int shadowWidth = 25;

        // 绘制整个窗口的阴影（统一使用直角）
        for (int i = 0; i < shadowWidth; i++) {
            // 使用更平滑的衰减曲线
            double alpha = 60.0 * exp(-pow(i / 10.0, 1.5));
            QColor color = shadowColor;
            color.setAlpha(qMax(0, (int)alpha));

            painter.setPen(QPen(color, 1));
            painter.setBrush(Qt::NoBrush);

            // 整个窗口使用直角矩形
            QRect shadowRect = rect().adjusted(-i, -i, i, i);
            painter.drawRect(shadowRect);
        }

        // 添加内部高光效果（macOS风格）
        QColor highlightColor(255, 255, 255, 20);
        painter.setPen(QPen(highlightColor, 1));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(rect().adjusted(1, 1, -1, -1));

        // 添加顶部高光条（macOS风格）
        QColor topHighlight(255, 255, 255, 40);
        painter.setPen(QPen(topHighlight, 1));
        painter.setBrush(Qt::NoBrush);
        QRect topBarRect = rect().adjusted(1, 1, -1, 1);
        topBarRect.setHeight(2);
        painter.drawRect(topBarRect);
    }
}

void FramelessWindow::drawShadow(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing);

    // 创建更柔和的阴影效果
    QColor shadowColor(0, 0, 0, 80);
    int shadowWidth = 20;

    for (int i = 0; i < shadowWidth; i++) {
        // 使用高斯分布计算透明度
        double alpha = 80.0 * exp(-pow(i / 8.0, 2));
        QColor color = shadowColor;
        color.setAlpha(qMax(0, (int)alpha));

        painter.setPen(QPen(color, 1));
        painter.setBrush(Qt::NoBrush);

        QRect shadowRect = rect().adjusted(-i, -i, i, i);
        painter.drawRect(shadowRect);
    }

    // 添加内部高光效果
    QColor highlightColor(255, 255, 255, 30);
    painter.setPen(QPen(highlightColor, 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(rect().adjusted(1, 1, -1, -1));
}

void FramelessWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    update();
}

void FramelessWindow::onMinimizeClicked()
{
    showMinimized();
}

void FramelessWindow::onMaximizeClicked()
{
    showMaximized();
}

void FramelessWindow::onCloseClicked()
{
    close();
}