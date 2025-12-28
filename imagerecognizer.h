#ifndef IMAGERECOGNIZER_H
#define IMAGERECOGNIZER_H

#include <QObject>
#include <QImage>
#include <QPoint>
#include <QRect>
#include <QVector>
#include <QMap>
#include <QPainter>

#include <QFile>
#include <QResource>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include <windows.h>

// OpenCV前向声明
namespace cv {
    class Mat;
}

// 定义匹配结果结构体，包含位置和得分
struct MatchResult {
    QPoint point;
    double score;
    
    // 构造函数
    MatchResult(const QPoint& p, double s) : point(p), score(s) {}
    
    // 比较运算符，用于排序（得分高的在前）
    bool operator<(const MatchResult& other) const {
        return score > other.score;
    }
};

class ImageRecognizer : public QObject
{
    Q_OBJECT
public:
    explicit ImageRecognizer(QObject *parent = nullptr);
    ~ImageRecognizer();

    // 设置识别阈值
    void setThreshold(double threshold);
    void setRecognitionThreshold(double threshold); // 新增声明

    // 从窗口捕获图像
    QImage captureWindow(HWND hwnd);

    // 从屏幕捕获图像
    QImage captureScreen(int screenIndex = 0);

    // 只保留一个模板加载方法
    bool loadTemplate(const QString &name, const QString &path); // 新增

    // 模板匹配 - 在源图像中查找模板（支持多分辨率和 DPI 缩放）
    QVector<QPoint> findTemplate(const QImage &sourceImage, const QString &templateName); // 新增方法声明

    // 查找最可能的匹配点
    QPoint findBestMatch(const QVector<QPoint> &matches, const QImage &sourceImage);

    // 转换为灰度图
    QImage toGrayscale(const QImage &image);

    // 保存图像用于调试
    bool saveImageForDebug(const QImage &image, const QString &prefix);
    bool saveImageForDebug(const QImage &image, const QString &prefix, const QString &subfolder);
    
    // 保存图像到日志目录
    bool saveImageToLogDir(const QImage &image, const QString &prefix);
    bool saveImageToLogDir(const QImage &image, const QString &prefix, const QString &subfolder);
    

    
    // 加载所有模板
    void loadTemplates();
    
    // 停止识别
    void stopRecognition();
    
    // 重置停止标志
    void resetStopFlag();

    // 在窗口中查找模板（非阻塞版本）
    void findTemplateInWindowAsync(HWND hwnd, const QString &templateName);
    
    // 在窗口中查找模板（阻塞版本，保留用于兼容旧代码）
    bool findTemplateInWindow(HWND hwnd, const QString &templateName, QPoint &pos); // 新增

    // 检查是否收到回答（同步版本，保留用于兼容）
    bool checkAnswerReceived(HWND hwnd);
    
    // 检查是否收到回答（异步版本）
    void checkAnswerReceivedAsync(HWND hwnd);

    // 捕获屏幕区域 - 线程安全版本
    QImage captureScreenArea(const QRect &area, int screenIndex = 0);

    QRect findImageOnScreen(const QString &templatePath, int screenIndex = 0);
    

    
    // 获取当前鼠标位置
    QPoint getCurrentMousePosition();
    
    // 检查是否请求停止
    bool isStopRequested() const;
    
    // 设置停止请求
    void setStopRequested(bool stop);
    
    // 重置识别器状态，清理窗口特定的缓存
    void resetState();

signals:
    void logMessage(const QString &message);
    // 模板查找完成信号
    void templateFound(const QString &templateName, const QPoint &pos);
    // 模板查找失败信号
    void templateNotFound(const QString &templateName);
    // 异步检查结果信号
    void answerReceivedChecked(HWND hwnd, bool received);
    // 识别区域显示信号
    void recognitionAreaFound(const QRect &area, const QString &description);

private slots:
    // 配置变化处理
    void onConfigChanged();
    // 异步查找模板的槽
    void doFindTemplateInWindow(HWND hwnd, const QString &templateName);

private:
    double threshold;
    int maxAttempts;
    QMap<QString, QImage> templates;
    // 工作线程
    QThread *workerThread;
    // 停止请求标志
    bool m_stopRequested;
    // 互斥锁，保护m_stopRequested变量
    mutable QMutex m_stopMutex;
    
    // 输入框位置缓存（按窗口句柄存储）
    QMap<HWND, QPoint> m_inputBoxPositions;
    QMap<HWND, bool> m_inputBoxFound;
    QMap<HWND, int> m_clientWidths;
    QMap<HWND, int> m_failedAttempts;
    
    // 回答区域状态缓存（按窗口句柄存储）
    QMap<HWND, QImage> m_previousAnswerAreas;
    QMap<HWND, int> m_stableFrameCounts;
    QMap<HWND, bool> m_hasDetectedChanges;

    // 归一化交叉相关匹配
    double matchTemplateNCC(const QImage &source, const QImage &templateImg,
                            int x, int y);
    
    // 平方差匹配
    double matchTemplateSSD(const QImage &source, const QImage &templateImg,
                           int x, int y);
    
    // 绝对差匹配
    double matchTemplateSAD(const QImage &source, const QImage &templateImg,
                           int x, int y);
    
    // ORB特征匹配
    QVector<QPoint> matchTemplateORB(const QImage &source, const QImage &templateImg);
    
    // QImage转Mat函数（内部函数，避免在头文件中暴露OpenCV依赖）
    cv::Mat QImageToMat(const QImage &image);
};

#endif // IMAGERECOGNIZER_H