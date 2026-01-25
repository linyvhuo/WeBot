#include "floatingwindow.h"
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QStyle>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QDebug>

FloatingWindow::FloatingWindow(QWidget *parent)
    : QWidget(parent)
    , m_progressBar(nullptr)
    , m_progressLabel(nullptr)
    , m_percentageLabel(nullptr)
    , m_stopButton(nullptr)
    , m_statusLabel(nullptr)
    , m_circleProgressLabel(nullptr)
    , m_currentTheme(ThemeManager::LightTheme)
    , m_progressValue(0)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_Hover, true);
    setMouseTracking(true);

    setMinimumSize(200, 200);

    setupUI();
    applyStyles();

    resize(300, 300);

    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        move((screenGeometry.width() - width()) / 2,
             (screenGeometry.height() - height()) / 2);
    }
}

FloatingWindow::~FloatingWindow()
{
}

void FloatingWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    m_circleProgressLabel = new QLabel(this);
    m_circleProgressLabel->setAlignment(Qt::AlignCenter);
    m_circleProgressLabel->setFixedSize(120, 120);
    mainLayout->addWidget(m_circleProgressLabel, 0, Qt::AlignCenter);

    m_progressLabel = new QLabel("准备就绪", this);
    m_progressLabel->setAlignment(Qt::AlignCenter);
    m_progressLabel->setWordWrap(true);
    mainLayout->addWidget(m_progressLabel);

    m_percentageLabel = new QLabel("0%", this);
    m_percentageLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_percentageLabel);

    m_statusLabel = new QLabel("等待开始", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_statusLabel);

    m_stopButton = new QPushButton("停止", this);
    m_stopButton->setFixedSize(120, 40);
    connect(m_stopButton, &QPushButton::clicked, this, &FloatingWindow::stopRequested);
    mainLayout->addWidget(m_stopButton, 0, Qt::AlignCenter);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setTextVisible(false);
    mainLayout->addWidget(m_progressBar);
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
    QString buttonStyle;
    QString progressBarStyle;

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
        buttonStyle = R"(
            QPushButton {
                background-color: #007aff;
                color: white;
                border: none;
                border-radius: 0px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #0056b3;
            }
            QPushButton:pressed {
                background-color: #004494;
            }
        )";
        progressBarStyle = R"(
            QProgressBar {
                border: 2px solid #007aff;
                border-radius: 0px;
                text-align: center;
                background-color: rgba(0, 122, 255, 0.1);
            }
            QProgressBar::chunk {
                background-color: #007aff;
                border-radius: 0px;
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
        buttonStyle = R"(
            QPushButton {
                background-color: #0e639c;
                color: white;
                border: none;
                border-radius: 0px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #0a4a7a;
            }
            QPushButton:pressed {
                background-color: #083a62;
            }
        )";
        progressBarStyle = R"(
            QProgressBar {
                border: 2px solid #0e639c;
                border-radius: 0px;
                text-align: center;
                background-color: rgba(14, 99, 156, 0.1);
            }
            QProgressBar::chunk {
                background-color: #0e639c;
                border-radius: 0px;
            }
        )";
        break;

    case ThemeManager::TechTheme:
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
        buttonStyle = R"(
            QPushButton {
                background-color: #00ffff;
                color: #0a0e27;
                border: none;
                border-radius: 0px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #00cccc;
            }
            QPushButton:pressed {
                background-color: #009999;
            }
        )";
        progressBarStyle = R"(
            QProgressBar {
                border: 2px solid #00ffff;
                border-radius: 0px;
                text-align: center;
                background-color: rgba(0, 255, 255, 0.1);
            }
            QProgressBar::chunk {
                background-color: #00ffff;
                border-radius: 0px;
            }
        )";
        break;

    case ThemeManager::SakuraTheme:
        windowStyle = R"(
            FloatingWindow {
                background-color: transparent;
            }
        )";
        percentageStyle = R"(
            QLabel {
                color: #d63384;
                font-size: 20pt;
                font-weight: bold;
                background: transparent;
            }
        )";
        statusStyle = R"(
            QLabel {
                color: #d63384;
                font-size: 10pt;
                font-weight: bold;
                background: transparent;
            }
        )";
        buttonStyle = R"(
            QPushButton {
                background-color: #d63384;
                color: white;
                border: none;
                border-radius: 0px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #b02a6b;
            }
            QPushButton:pressed {
                background-color: #8a2254;
            }
        )";
        progressBarStyle = R"(
            QProgressBar {
                border: 2px solid #d63384;
                border-radius: 0px;
                text-align: center;
                background-color: rgba(214, 51, 132, 0.1);
            }
            QProgressBar::chunk {
                background-color: #d63384;
                border-radius: 0px;
            }
        )";
        break;

    case ThemeManager::WarmTheme:
        windowStyle = R"(
            FloatingWindow {
                background-color: transparent;
            }
        )";
        percentageStyle = R"(
            QLabel {
                color: #fb923c;
                font-size: 20pt;
                font-weight: bold;
                background: transparent;
            }
        )";
        statusStyle = R"(
            QLabel {
                color: #fb923c;
                font-size: 10pt;
                font-weight: bold;
                background: transparent;
            }
        )";
        buttonStyle = R"(
            QPushButton {
                background-color: #fb923c;
                color: white;
                border: none;
                border-radius: 0px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #ea8222;
            }
            QPushButton:pressed {
                background-color: #d9771a;
            }
        )";
        progressBarStyle = R"(
            QProgressBar {
                border: 2px solid #fb923c;
                border-radius: 0px;
                text-align: center;
                background-color: rgba(251, 146, 60, 0.1);
            }
            QProgressBar::chunk {
                background-color: #fb923c;
                border-radius: 0px;
            }
        )";
        break;

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
        buttonStyle = R"(
            QPushButton {
                background-color: #00ffff;
                color: #0a0e27;
                border: none;
                border-radius: 0px;
                padding: 8px 16px;
                font-size: 14px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #00cccc;
            }
            QPushButton:pressed {
                background-color: #009999;
            }
        )";
        progressBarStyle = R"(
            QProgressBar {
                border: 2px solid #00ffff;
                border-radius: 0px;
                text-align: center;
                background-color: rgba(0, 255, 255, 0.1);
            }
            QProgressBar::chunk {
                background-color: #00ffff;
                border-radius: 0px;
            }
        )";
        break;
    }

    setStyleSheet(windowStyle + percentageStyle + statusStyle + buttonStyle + progressBarStyle);
    update();
}

void FloatingWindow::updateProgress(int current, int total)
{
    if (total > 0) {
        m_progressValue = (current * 100) / total;
        m_progressBar->setValue(m_progressValue);
        m_percentageLabel->setText(QString("%1%").arg(m_progressValue));
    }
    update();
}

void FloatingWindow::setProgressText(const QString &text)
{
    m_progressLabel->setText(text);
}

void FloatingWindow::setStatus(const QString &status)
{
    m_statusLabel->setText(status);
}

void FloatingWindow::onThemeChanged(ThemeManager::Theme theme)
{
    updateThemeStyles(theme);
}

void FloatingWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void FloatingWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void FloatingWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
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
        bgColor = QColor(10, 14, 39, 230);
        borderColor = QColor(0, 255, 255, 200);
        progressColor = QColor(0, 255, 255, 150);
        break;
    case ThemeManager::SakuraTheme:
        bgColor = QColor(255, 240, 245, 240);
        borderColor = QColor(214, 51, 132, 200);
        progressColor = QColor(214, 51, 132, 150);
        break;
    case ThemeManager::WarmTheme:
        bgColor = QColor(255, 248, 225, 240);
        borderColor = QColor(251, 146, 60, 200);
        progressColor = QColor(251, 146, 60, 150);
        break;
    default:
        bgColor = QColor(10, 14, 39, 230);
        borderColor = QColor(0, 255, 255, 200);
        progressColor = QColor(0, 255, 255, 150);
        break;
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(bgColor);
    painter.drawRoundedRect(centerX - radius, centerY - radius, radius * 2, radius * 2, 5, 5);

    QPen pen(borderColor);
    pen.setWidth(3);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(centerX - radius, centerY - radius, radius * 2, radius * 2, 5, 5);

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

void FloatingWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        QWidget::keyPressEvent(event);
    }
}
