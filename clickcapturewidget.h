#ifndef CLICKCAPTUREWIDGET_H
#define CLICKCAPTUREWIDGET_H

#include <QWidget>
#include <QMouseEvent>

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
    void mouseClicked(const QPoint &pos);
    
private:
    QPoint m_clickedPos;
};

#endif // CLICKCAPTUREWIDGET_H
