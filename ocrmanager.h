#ifndef OCRMANAGER_H
#define OCRMANAGER_H

#include <QObject>
#include <QString>
#include <QPixmap>
#include <QRect>
#include <QMutex>

class OCRManager : public QObject
{
    Q_OBJECT

public:
    enum OCREngine {
        WindowsOCR,    // Windows 内置 OCR
        TesseractOCR,  // Tesseract OCR
        BaiduOCR       // 百度 OCR
    };

    enum Language {
        ChineseSimplified,
        ChineseTraditional,
        English,
        Japanese,
        Korean
    };

    explicit OCRManager(QObject *parent = nullptr);
    ~OCRManager();

    static OCRManager *getInstance();

    // 识别图片中的文字
    QString recognizeText(const QPixmap &pixmap, const QRect &region = QRect());

    // 设置OCR引擎
    void setOCREngine(OCREngine engine);

    // 设置识别语言
    void setLanguage(Language language);

    // 检查OCR功能是否可用
    bool isAvailable() const;

    // 获取支持的语言列表
    QStringList getSupportedLanguages() const;

signals:
    void recognitionCompleted(const QString &text);
    void recognitionError(const QString &error);

private:
    bool initializeOCR();
    void cleanupOCR();
    bool checkWindowsOCRAvailability();
    bool checkTesseractAvailability();
    QString recognizeWithWindowsOCR(const QString &imagePath);
    QString recognizeWithTesseract(const QString &imagePath);
    QString recognizeWithBaiduOCR(const QString &imagePath);
    QString getLanguageCode(Language language) const;
    QString getTesseractLanguageCode(Language language) const;

    OCREngine m_currentEngine;
    Language m_currentLanguage;
    bool m_isAvailable;
    // QNetworkAccessManager *m_networkManager; // 暂时注释，未来用于百度OCR
    QMutex m_mutex;

    static OCRManager *instance;
    static QMutex instanceMutex;
};

#endif // OCRMANAGER_H