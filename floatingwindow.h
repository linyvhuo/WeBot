#ifndef FLOATINGWINDOW_H
#define FLOATINGWINDOW_H

#include <QWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPoint>
#include "thememanager.h"

class FloatingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FloatingWindow(QWidget *parent = nullptr);
    ~FloatingWindow();

    void updateProgress(int current, int total);
    void setProgressText(const QString &text);
    void setStatus(const QString &status);
    void onThemeChanged(ThemeManager::Theme theme);

signals:
    void stopRequested();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setupUI();
    void applyStyles();
    void updateThemeStyles(ThemeManager::Theme theme);

    QProgressBar *m_progressBar;
    QLabel *m_progressLabel;
    QLabel *m_percentageLabel;
    QPushButton *m_stopButton;
    QLabel *m_statusLabel;
    QLabel *m_circleProgressLabel;

    QPoint m_dragPosition;
    ThemeManager::Theme m_currentTheme;
    int m_progressValue;
};

#endif // FLOATINGWINDOW_H
