#include "floatingwindow.h"
#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QConicalGradient>
#include <QKeyEvent>

FloatingWindow::FloatingWindow(QWidget *parent)
    : QWidget(parent)
    , m_progressBar(nullptr)
    , m_progressLabel(nullptr)
    , m_percentageLabel(nullptr)
    , m_stopButton(nullptr)
    , m_statusLabel(nullptr)
    , m_circleProgressLabel(nullptr)
    , m_currentTheme(ThemeManager::TechTheme)
    , m_progressValue(0)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    
    setupUI();
    applyStyles();
    
    resize(120, 140);
}

FloatingWindow::~FloatingWindow()
{
}

void FloatingWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_percentageLabel = new QLabel("0%", this);
    m_percentageLabel->setAlignment(Qt::AlignCenter);
    m_percentageLabel->setGeometry(10, 25, 100, 40);
    
    m_statusLabel = new QLabel("空闲", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setGeometry(10, 70, 100, 25);

    m_stopButton = new QPushButton("", this);
    m_stopButton->setGeometry(0, 0, 120, 140);
    m_stopButton->setCursor(Qt::PointingHandCursor);
    m_stopButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    border: none;"
        "}"
    );
    connect(m_stopButton, &QPushButton::clicked, this, [this]() {
        emit stopRequested();
    });

    setLayout(mainLayout);
    m_stopButton->raise();
}

void FloatingWindow::applyStyles()
{
    updateThemeStyles(m_currentTheme);
}

void FloatingWindow::updateThemeStyles(ThemeManager::Theme theme)
{
    m_currentTheme = theme;
    
    QString windowStyle;
    QString percentageStyle;
    QString statusStyle;

    switch (theme) {
    case ThemeManager::LightTheme:
        windowStyle = R"(
            FloatingWindow {
                background-color: transparent;
            }
        )";
        percentageStyle = R"(
            QLabel {
                color: #007aff;
                font-size: 20pt;
                font-weight: bold;
                background: transparent;
            }
        )";
        statusStyle = R"(
            QLabel {
                color: #007aff;
                font-size: 10pt;
                font-weight: bold;
                background: transparent;
            }
        )";
        break;

    case ThemeManager::DarkTheme:
        windowStyle = R"(
            FloatingWindow {
                background-color: transparent;
            }
        )";
        percentageStyle = R"(
            QLabel {
                color: #0e639c;
                font-size: 20pt;
                font-weight: bold;
                background: transparent;
            }
        )";
        statusStyle = R"(
            QLabel {
                color: #0e639c;
                font-size: 10pt;
                font-weight: bold;
                background: transparent;
            }
        )";
        break;

    case ThemeManager::TechTheme:
    default:
        windowStyle = R"(
            FloatingWindow {
                background-color: transparent;
            }
        )";
        percentageStyle = R"(
            QLabel {
                color: #00ffff;
                font-size: 20pt;
                font-weight: bold;
                background: transparent;
            }
        )";
        statusStyle = R"(
            QLabel {
                color: #00ffff;
                font-size: 10pt;
                font-weight: bold;
                background: transparent;
            }
        )";
        break;
    }

    setStyleSheet(windowStyle + percentageStyle + statusStyle);
    update();
}

void FloatingWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int centerX = width() / 2;
    int centerY = height() / 2;
    int radius = 55;
    
    QColor bgColor, borderColor, progressColor;
    
    switch (m_currentTheme) {
    case ThemeManager::LightTheme:
        bgColor = QColor(255, 255, 255, 240);
        borderColor = QColor(0, 122, 255, 200);
        progressColor = QColor(0, 122, 255, 150);
        break;
    case ThemeManager::DarkTheme:
        bgColor = QColor(30, 30, 30, 240);
        borderColor = QColor(14, 99, 156, 200);
        progressColor = QColor(14, 99, 156, 150);
        break;
    case ThemeManager::TechTheme:
    default:
        bgColor = QColor(10, 14, 39, 230);
        borderColor = QColor(0, 255, 255, 200);
        progressColor = QColor(0, 255, 255, 150);
        break;
    }
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgColor);
    painter.drawEllipse(centerX - radius, centerY - radius, radius * 2, radius * 2);
    
    QPen pen(borderColor);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(centerX - radius, centerY - radius, radius * 2, radius * 2);
    
    if (m_progressValue > 0) {
        QPen progressPen(progressColor);
        progressPen.setWidth(6);
        progressPen.setCapStyle(Qt::RoundCap);
        painter.setPen(progressPen);
        
        QRectF rect(centerX - radius + 3, centerY - radius + 3, (radius - 3) * 2, (radius - 3) * 2);
        int startAngle = 90 * 16;
        int spanAngle = -static_cast<int>(360 * m_progressValue / 100 * 16);
        
        painter.drawArc(rect, startAngle, spanAngle);
    }
}

void FloatingWindow::updateProgress(int current, int total)
{
    if (total > 0) {
        m_progressValue = (current * 100) / total;
        m_percentageLabel->setText(QString("%1%").arg(m_progressValue));
    } else {
        m_progressValue = 0;
        m_percentageLabel->setText("0%");
    }
    update();
}

void FloatingWindow::setProgressText(const QString &text)
{
    m_percentageLabel->setText(text);
}

void FloatingWindow::setStatus(const QString &status)
{
    m_statusLabel->setText(status);
}

void FloatingWindow::onThemeChanged(ThemeManager::Theme theme)
{
    updateThemeStyles(theme);
    updateProgress(0, 100);
}

void FloatingWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    
    QScreen *screen = qApp->primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        int x = screenGeometry.width() - width() - 20;
        int y = screenGeometry.height() - height() - 20;
        move(x, y);
    }
}

void FloatingWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void FloatingWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void FloatingWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        emit stopRequested();
        event->accept();
    } else {
        QWidget::keyPressEvent(event);
    }
}
