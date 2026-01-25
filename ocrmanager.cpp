#include "ocrmanager.h"
#include <QDebug>
#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QDir>
#include <QStandardPaths>
#include <QtGlobal>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QUrlQuery>
#include "logger.h"

OCRManager *OCRManager::instance = nullptr;
QMutex OCRManager::instanceMutex;

OCRManager::OCRManager(QObject *parent)
    : QObject(parent)
    , m_currentEngine(WindowsOCR)
    , m_currentLanguage(ChineseSimplified)
    , m_isAvailable(false)
    // , m_networkManager(nullptr)
{
    m_isAvailable = initializeOCR();
}

OCRManager::~OCRManager()
{
    cleanupOCR();
}

OCRManager *OCRManager::getInstance()
{
    QMutexLocker locker(&instanceMutex);
    if (!instance) {
        instance = new OCRManager();
    }
    return instance;
}

bool OCRManager::initializeOCR()
{
    // 检查Windows OCR是否可用
    bool windowsOCRAvailable = checkWindowsOCRAvailability();

    // 检查Tesseract是否可用
    bool tesseractAvailable = checkTesseractAvailability();

    LOG_INFO(QString("OCR管理器初始化完成 - Windows OCR: %1, Tesseract: %2")
             .arg(windowsOCRAvailable ? "可用" : "不可用")
             .arg(tesseractAvailable ? "可用" : "不可用"));

    return windowsOCRAvailable || tesseractAvailable;
}

void OCRManager::cleanupOCR()
{
    // if (m_networkManager) {
    //     delete m_networkManager;
    //     m_networkManager = nullptr;
    // }
    LOG_INFO("OCR管理器清理完成");
}

void OCRManager::setOCREngine(OCREngine engine)
{
    m_currentEngine = engine;
    LOG_INFO(QString("OCR引擎已切换为: %1").arg(engine == WindowsOCR ? "Windows OCR" :
                                                 engine == TesseractOCR ? "Tesseract OCR" : "百度 OCR"));
}

void OCRManager::setLanguage(Language language)
{
    m_currentLanguage = language;
    LOG_INFO(QString("OCR语言已设置为: %1").arg(getLanguageCode(language)));
}

QString OCRManager::recognizeText(const QPixmap &pixmap, const QRect &region)
{
    Q_UNUSED(region);

    if (!m_isAvailable) {
        emit recognitionError("OCR功能不可用");
        return QString();
    }

    try {
        // 保存图片到临时文件
        QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        QString imagePath = tempPath + "/ocr_temp_" + QString::number(QDateTime::currentMSecsSinceEpoch()) + ".png";

        QPixmap pixmapToProcess = pixmap;
        if (!region.isNull() && region.isValid()) {
            pixmapToProcess = pixmap.copy(region);
        }

        if (!pixmapToProcess.save(imagePath, "PNG")) {
            emit recognitionError("无法保存临时图片文件");
            return QString();
        }

        QString result;

        switch (m_currentEngine) {
        case WindowsOCR:
            result = recognizeWithWindowsOCR(imagePath);
            break;
        case TesseractOCR:
            result = recognizeWithTesseract(imagePath);
            break;
        case BaiduOCR:
            result = recognizeWithBaiduOCR(imagePath);
            break;
        default:
            result = "未知OCR引擎";
            break;
        }

        // 删除临时文件
        QFile::remove(imagePath);

        if (!result.isEmpty()) {
            LOG_INFO(QString("OCR识别完成，结果长度: %1").arg(result.length()));
            emit recognitionCompleted(result);
        } else {
            emit recognitionError("OCR识别结果为空");
        }

        return result;

    } catch (const std::exception& e) {
        QString errorMsg = QString("OCR识别失败: %1").arg(e.what());
        LOG_ERROR(errorMsg);
        emit recognitionError(errorMsg);
        return QString();
    } catch (...) {
        QString errorMsg = "OCR识别失败：未知错误";
        LOG_ERROR(errorMsg);
        emit recognitionError(errorMsg);
        return QString();
    }
}

QString OCRManager::recognizeWithWindowsOCR(const QString &imagePath)
{
    Q_UNUSED(imagePath);

    // Windows OCR实现较为复杂，暂时返回模拟结果
    // 实际实现需要调用Windows.Media.OCR API
    LOG_WARNING("Windows OCR功能暂未完全实现，使用模拟结果");
    return "Windows OCR识别结果：\n这是模拟的文字识别结果。\n实际需要集成Windows.Media.OCR API。";
}

QString OCRManager::recognizeWithTesseract(const QString &imagePath)
{
    Q_UNUSED(imagePath);

    // Tesseract OCR实现，暂时返回模拟结果
    LOG_WARNING("Tesseract OCR功能暂未完全实现，使用模拟结果");
    return "Tesseract OCR识别结果：\n这是Tesseract模拟的文字识别结果。\n实际需要安装并调用Tesseract程序。";
}

QString OCRManager::recognizeWithBaiduOCR(const QString &imagePath)
{
    Q_UNUSED(imagePath);

    // TODO: 实现百度OCR API调用
    LOG_WARNING("百度OCR功能尚未实现");
    return "百度OCR功能开发中...\n需要配置百度OCR API密钥。";
}

bool OCRManager::checkWindowsOCRAvailability()
{
    // 检查是否支持Windows OCR (Windows 10 版本 1703+)
    // 简化检查，暂时返回true表示支持
    LOG_INFO("Windows OCR可用性检查：暂时返回true");
    return true;
}

bool OCRManager::checkTesseractAvailability()
{
    QProcess process;
    process.start("tesseract", QStringList() << "--version");
    process.waitForFinished(5000);

    return process.exitCode() == 0;
}

QString OCRManager::getLanguageCode(Language language) const
{
    switch (language) {
    case ChineseSimplified: return "zh-CN";
    case ChineseTraditional: return "zh-TW";
    case English: return "en-US";
    case Japanese: return "ja-JP";
    case Korean: return "ko-KR";
    default: return "zh-CN";
    }
}

QString OCRManager::getTesseractLanguageCode(Language language) const
{
    switch (language) {
    case ChineseSimplified: return "chi_sim";
    case ChineseTraditional: return "chi_tra";
    case English: return "eng";
    case Japanese: return "jpn";
    case Korean: return "kor";
    default: return "chi_sim";
    }
}

bool OCRManager::isAvailable() const
{
    return m_isAvailable;
}

QStringList OCRManager::getSupportedLanguages() const
{
    return QStringList() << "中文简体" << "中文繁体" << "英文" << "日文" << "韩文";
}