#ifndef SCREENSHOTSELECTOR_H
#define SCREENSHOTSELECTOR_H

#include <QWidget>
#include <QPixmap>
#include <QRect>
#include <QPoint>
#include "thememanager.h"

class ScreenshotSelector : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenshotSelector(const QPixmap &screenshot, QWidget *parent = nullptr);
    ~ScreenshotSelector();

    QRect getSelectedRect() const;
    QPixmap getScreenshot() const;

signals:
    void regionSelected(const QRect &rect);
    void cancelled();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void drawSelectionRect(QPainter &painter);
    QRect getNormalizedRect(const QPoint &start, const QPoint &end) const;
    void onThemeChanged(ThemeManager::Theme theme);
    QColor getThemeColor() const;

    QPixmap m_screenshot;
    QPoint m_startPoint;
    QPoint m_endPoint;
    bool m_selecting;
    QRect m_selectedRect;
    ThemeManager::Theme m_currentTheme;
};

#endif // SCREENSHOTSELECTOR_H