#pragma once

#include <QWidget>
#include <QPoint>
#include <QRect>
#include <QColor>
#include <QMutex>
#include <QList>
#include <QTimer>
#include "thememanager.h"

class RecognitionOverlay : public QWidget {
    Q_OBJECT
public:
    explicit RecognitionOverlay(QWidget *parent = nullptr);
    ~RecognitionOverlay();

    // 绘制识别区域
    void drawRecognitionArea(const QRect &rect, const QColor &color = Qt::red);

    // 清除所有识别区域
    void clearRecognitionAreas();

    // 隐藏识别区域
    void hide();

    // 显示识别区域
    void show();

protected:
    // 重写绘制事件
    void paintEvent(QPaintEvent *event) override;

private:
    void onThemeChanged(ThemeManager::Theme theme);
    QColor getThemeColor() const;

    QList<QRect> m_recognitionAreas; // 识别区域列表
    QList<QColor> m_colors; // 对应区域的颜色列表
    QMutex m_mutex; // 线程安全锁
    QTimer *m_updateTimer; // 更新定时器，用于平滑更新识别框
    ThemeManager::Theme m_currentTheme; // 当前主题
};
