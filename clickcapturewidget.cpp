#include "clickcapturewidget.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QCursor>
#include <QApplication>
#include <QScreen>

ClickCaptureWidget::ClickCaptureWidget(QWidget *parent)
    : QWidget(parent)
{
    // 设置窗口属性为全屏、无边框、透明背景
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    
    // 安装事件过滤器
    installEventFilter(this);
    
    // 设置鼠标样式为十字准星
    setCursor(Qt::CrossCursor);
}

void ClickCaptureWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 获取点击位置
        m_clickedPos = event->globalPos();
        emit mouseClicked(m_clickedPos);
    }
    QWidget::mousePressEvent(event);
}

void ClickCaptureWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制半透明背景
    QColor bgColor(0, 0, 0, 50);
    painter.fillRect(rect(), bgColor);
    
    // 绘制十字准星
    QPen pen(Qt::red, 2);
    painter.setPen(pen);
    
    // 获取鼠标当前位置
    QPoint mousePos = mapFromGlobal(QCursor::pos());
    
    // 绘制水平十字线
    painter.drawLine(0, mousePos.y(), width(), mousePos.y());
    
    // 绘制垂直十字线
    painter.drawLine(mousePos.x(), 0, mousePos.x(), height());
    
    // 绘制中心圆圈
    int circleRadius = 10;
    painter.drawEllipse(mousePos.x() - circleRadius, mousePos.y() - circleRadius, circleRadius * 2, circleRadius * 2);
}

bool ClickCaptureWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove) {
        // 鼠标移动时更新绘制
        update();
    }
    return QWidget::eventFilter(obj, event);
}
