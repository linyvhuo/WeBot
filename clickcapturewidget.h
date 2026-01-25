#ifndef CLICKCAPTUREWIDGET_H
#define CLICKCAPTUREWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "thememanager.h"

class ClickCaptureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClickCaptureWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void mouseClicked(const QPointF &pos);

private:
    void onThemeChanged(ThemeManager::Theme theme);
    QColor getThemeColor() const;

    QPointF m_clickedPos;
    ThemeManager::Theme m_currentTheme;
};

#endif // CLICKCAPTUREWIDGET_H
