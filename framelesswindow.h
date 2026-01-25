#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QPoint>
#include "thememanager.h"

class FramelessWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FramelessWindow(QWidget *parent = nullptr);
    ~FramelessWindow();

    // 设置窗口标题
    void setWindowTitle(const QString &title);

    // 设置中央widget
    void setCentralWidget(QWidget *widget);

    // 设置窗口图标
    void setWindowIcon(const QIcon &icon);

    // 窗口控制
    void showMinimized();
    void showMaximized();
    void showNormal();
    void close();

    // 阴影设置
    void setShadowEnabled(bool enabled);
    bool isShadowEnabled() const;

protected:
    // 鼠标事件处理（用于窗口移动和拉伸）
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    // 绘制阴影
    void paintEvent(QPaintEvent *event) override;

    // 大小改变事件
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onMinimizeClicked();
    void onMaximizeClicked();
    void onCloseClicked();

private:
    void createTitleBar();
    QPushButton* createTitleButton(const QString &text, const QString &tooltip);
    void onThemeChanged(ThemeManager::Theme theme);
    void updateTitleBarStyle();

    enum ResizeDirection {
        None = 0,
        Left = 1,
        Right = 2,
        Top = 4,
        Bottom = 8,
        TopLeft = Top | Left,
        TopRight = Top | Right,
        BottomLeft = Bottom | Left,
        BottomRight = Bottom | Right
    };

    // UI组件
    QWidget *m_titleBar;
    QLabel *m_titleLabel;
    QLabel *m_iconLabel;
    QPushButton *m_minimizeButton;
    QPushButton *m_maximizeButton;
    QPushButton *m_closeButton;
    QWidget *m_centralWidget;

    // 布局
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_titleLayout;

    // 窗口状态
    bool m_isMaximized;
    bool m_shadowEnabled;

    // 鼠标拖拽相关
    bool m_dragging;
    QPoint m_dragStartPos;
    QRect m_normalGeometry;

    // 窗口拉伸相关
    bool m_resizing;
    ResizeDirection m_resizeDirection;
    QPoint m_resizeStartPos;
    QRect m_resizeStartGeometry;

    // 边框检测
    ResizeDirection getResizeDirection(const QPoint &pos) const;
    void updateCursor(ResizeDirection direction);
    void performResize(const QPoint &currentPos);

    // 阴影绘制
    void drawShadow(QPainter &painter);

    // 常量定义
    static const int BORDER_WIDTH = 8;
    static const int TITLE_BAR_HEIGHT = 40;

    // 主题支持
    ThemeManager::Theme m_currentTheme;
};

#endif // FRAMELESSWINDOW_H