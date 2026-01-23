#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QApplication>
#include <QMutex>

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    // 主题枚举
    enum Theme {
        LightTheme,    // 浅色主题
        DarkTheme,     // 深色主题
        TechTheme      // 科技主题
    };
    Q_ENUM(Theme)

    explicit ThemeManager(QObject *parent = nullptr);
    ~ThemeManager();

    static ThemeManager *getInstance();

    // 获取当前主题
    Theme getCurrentTheme() const;

    // 设置主题
    void setTheme(Theme theme);

    // 应用主题到应用程序
    void applyTheme(Theme theme);

    // 获取主题名称
    QString getThemeName(Theme theme) const;

    // 获取所有主题名称列表
    QStringList getThemeNames() const;

signals:
    void themeChanged(Theme theme);

private:
    void initThemes();
    QString getLightThemeStyle() const;
    QString getDarkThemeStyle() const;
    QString getTechThemeStyle() const;

    Theme m_currentTheme;
    QMap<Theme, QString> m_themeStyles;

    static ThemeManager *instance;
    static QMutex mutex;
};

#endif // THEMEMANAGER_H
