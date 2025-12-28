#include "imagerecognizer.h"
#include "configmanager.h"
#include <QScreen>
#include <QPixmap>
#include <QImage>
#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QPainter>
#include <QDateTime>
#include <QDir>
#include <QMutexLocker>
#include <tuple>

// OpenCV头文�?#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>

// 使用OpenCV命名空间
using namespace cv;
using namespace std;

// MatchResult结构体已在头文件中定�?
// QImage转Mat函数（类成员函数定义�?cv::Mat ImageRecognizer::QImageToMat(const QImage &image) {
    Mat mat;
    
    if (image.format() == QImage::Format_Grayscale8) {
        // 灰度图像转换
        mat = Mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine());
    } else if (image.format() == QImage::Format_ARGB32 || image.format() == QImage::Format_RGB32) {
        // 彩色图像转换
        mat = Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        cvtColor(mat, mat, COLOR_BGRA2GRAY);
    } else {
        // 其他格式转换为灰度图
        QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);
        mat = Mat(grayImage.height(), grayImage.width(), CV_8UC1, const_cast<uchar*>(grayImage.bits()), grayImage.bytesPerLine());
    }
    
    return mat.clone(); // 返回克隆，避免原始数据被修改
}


ImageRecognizer::ImageRecognizer(QObject *parent) : QObject(parent) {
    // 初始化识别阈�?    threshold = ConfigManager::getInstance()->getImageRecognitionThreshold();
    maxAttempts = ConfigManager::getInstance()->getMaxRecognitionAttempts();
    m_stopRequested = false;

    // 清理debug_tu目录
    QString projectPath = QCoreApplication::applicationDirPath();
    QString debugDir = projectPath + "/debug_tu/";
    QDir dir(debugDir);
    if (dir.exists()) {
        // 删除目录下所有文�?        QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        for (const QString &file : files) {
            bool removed = dir.remove(file);
            emit logMessage(QString("尝试删除文件: %1，结�? %2").arg(file).arg(removed ? "成功" : "失败"));
        }
        emit logMessage("已清理debug_tu目录下的所有文�?);
    } else {
        // 如果目录不存在，创建�?        bool dirCreated = dir.mkpath(debugDir);
        emit logMessage(QString("尝试创建目录: %1，结�? %2").arg(debugDir).arg(dirCreated ? "成功" : "失败"));
    }

    // 监听配置变化
    connect(ConfigManager::getInstance(), &ConfigManager::configChanged,
            this, &ImageRecognizer::onConfigChanged);
    
    // 初始化工作线�?    workerThread = new QThread(this);
    // 将当前对象移动到工作线程
    this->moveToThread(workerThread);
    // 启动工作线程
    workerThread->start();
}

ImageRecognizer::~ImageRecognizer() {
    // 停止工作线程
    workerThread->quit();
    workerThread->wait();
    delete workerThread;
}

QRect ImageRecognizer::findImageOnScreen(const QString &templatePath, int screenIndex) {
    QImage screenImage = captureScreen(screenIndex);
    if (screenImage.isNull()) {
        emit logMessage("无法捕获屏幕");
        return QRect();
    }

    // 加载模板图像
    QImage templateImage(templatePath);
    if (templateImage.isNull()) {
        emit logMessage("无法加载模板: " + templatePath);
        return QRect();
    }

    // 转换为灰度图
    screenImage = toGrayscale(screenImage);
    templateImage = toGrayscale(templateImage);

    // 检查模板尺�?    if (templateImage.width() > screenImage.width() ||
        templateImage.height() > screenImage.height()) {
        emit logMessage("模板尺寸大于屏幕");
        return QRect();
    }

    // 模板匹配 - 确保对所有区域进行完整识�?    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        // 加载模板文件到内存，以便findTemplate方法使用
        QString templateName = "temp_template";
        loadTemplate(templateName, templatePath);
        
        QVector<QPoint> matches = findTemplate(screenImage, templateName);
        if (!matches.isEmpty()) {
            // 对所有匹配点进行处理，而不是只返回第一�?            for (const QPoint& match : matches) {
                // 使用配置文件中保存的模板尺寸计算识别区域
                ConfigManager* config = ConfigManager::getInstance();
                QSize templateSize = config->getTemplateSize(templateName);
                
                // 如果配置文件中没有保存尺寸，使用模板图像的实际尺�?                if (templateSize.width() == 100 && templateSize.height() == 100) {
                    templateSize = templateImage.size();
                }
                
                QRect recognitionArea(match.x(), match.y(),
                             templateSize.width(), templateSize.height());
                
                // 发射识别区域信号，用于显示识别框
                emit recognitionAreaFound(recognitionArea, templatePath);
                
                // 保存识别区域截图
                saveRecognitionAreaImage(screenImage, match, templateName, "recognition");
            }
            
            // 返回最佳匹配区�?            QPoint bestMatch = findBestMatch(matches, screenImage);
            QRect bestRecognitionArea(bestMatch.x(), bestMatch.y(),
                         templateImage.width(), templateImage.height());
            
            return bestRecognitionArea;
        }

        emit logMessage(QString("未找到匹配，尝试次数�?1").arg(attempt + 1));
        QThread::sleep(1); // 等待一段时间后重试
        screenImage = captureScreen(screenIndex); // 重新捕获屏幕
        if (screenImage.isNull()) {
            emit logMessage("无法捕获屏幕");
            return QRect();
        }

        // 转换为灰度图
        screenImage = toGrayscale(screenImage);
    }

    return QRect();
}

bool ImageRecognizer::checkAnswerReceived(HWND hwnd) {
    // 检查是否收到回�?    // 通过比较输入框上方区域的变化来判断是否有新内�?
    if (!hwnd) {
        emit logMessage("无效的窗口句�?);
        return false;
    }
    
    const int maxFailedAttempts = 5; // 最多允�?次失败尝�?    
    // 初始化窗口特定的缓存
    if (!m_inputBoxFound.contains(hwnd)) {
        m_inputBoxFound[hwnd] = false;
        m_failedAttempts[hwnd] = 0;
        m_stableFrameCounts[hwnd] = 0;
        m_hasDetectedChanges[hwnd] = false;
    }
    
    // 检查窗口客户区大小是否变化
    RECT clientRect;
    if (GetClientRect(hwnd, &clientRect)) {
        int currentWidth = clientRect.right - clientRect.left;
        // 如果客户区宽度变化，重新查找输入�?        if (currentWidth != m_clientWidths[hwnd]) {
            m_clientWidths[hwnd] = currentWidth;
            m_inputBoxFound[hwnd] = false; // 客户区大小变化，输入框位置可能变�?            emit logMessage(QString("窗口客户区大小变�? %1，需要重新查找输入框").arg(currentWidth));
        }
    }
    
    // 查找输入框位置的条件�?    // 1. 第一次调�?    // 2. 输入框未找到
    // 3. 多次失败�?    // 4. 客户区大小变�?    if (!m_inputBoxFound[hwnd] || m_failedAttempts[hwnd] >= maxFailedAttempts) {
        // 重置失败计数
        m_failedAttempts[hwnd] = 0;
        
        // 1. 找到输入框位�?        // 尝试多个模板变体查找输入�?        QStringList inputBoxTemplates = {"input_box", "input_box_small", "input_box_large"};
        
        m_inputBoxFound[hwnd] = false;
        QPoint inputBoxPos;
        for (const QString& templateName : inputBoxTemplates) {
            if (findTemplateInWindow(hwnd, templateName, inputBoxPos)) {
                m_inputBoxFound[hwnd] = true;
                m_inputBoxPositions[hwnd] = inputBoxPos;
                emit logMessage(QString("找到输入框位�? (%1, %2)").arg(inputBoxPos.x()).arg(inputBoxPos.y()));
                
                // 更新窗口客户区大�?                if (GetClientRect(hwnd, &clientRect)) {
                    m_clientWidths[hwnd] = clientRect.right - clientRect.left;
                } else {
                    m_clientWidths[hwnd] = 0;
                    emit logMessage("无法获取窗口客户区大�?);
                }
                break;
            }
        }
        
        if (!m_inputBoxFound[hwnd]) {
            emit logMessage("未找到输入框");
            m_failedAttempts[hwnd]++;
            return false;
        }
    }
    
    // 2. 定义回答区域（输入框上方�?    int answerAreaX = 0;
    int answerAreaY = 0;
    int answerAreaWidth = m_clientWidths[hwnd];
    int answerAreaHeight = m_inputBoxPositions[hwnd].y() - 20; // 预留空间
    
    // 检查回答区域是否有�?    if (answerAreaHeight <= 0 || m_clientWidths[hwnd] <= 0) {
        emit logMessage("无效的回答区域尺�?);
        m_failedAttempts[hwnd]++;
        return false;
    }
    
    // 3. 捕获回答区域
    QImage windowImage = captureWindow(hwnd);
    if (windowImage.isNull()) {
        emit logMessage("无法捕获窗口");
        m_failedAttempts[hwnd]++;
        return false;
    }
    
    // 检查窗口图像尺寸是否足�?    if (windowImage.width() < answerAreaWidth || windowImage.height() < m_inputBoxPositions[hwnd].y()) {
        emit logMessage("窗口图像尺寸不足，无法捕获回答区�?);
        m_failedAttempts[hwnd]++;
        return false;
    }
    
    QImage answerArea = windowImage.copy(answerAreaX, answerAreaY, answerAreaWidth, answerAreaHeight);
    
    // 4. 比较前后帧差�?    
    // 首次捕获
    if (!m_previousAnswerAreas.contains(hwnd) || m_previousAnswerAreas[hwnd].isNull()) {
        m_previousAnswerAreas[hwnd] = answerArea;
        m_stableFrameCounts[hwnd] = 0;
        m_hasDetectedChanges[hwnd] = false;
        emit logMessage("首次捕获回答区域图像");
        return false;
    }

    // 尺寸检�?    if (m_previousAnswerAreas[hwnd].size() != answerArea.size()) {
        emit logMessage(QString("回答区域尺寸变化: %1x%2 -> %3x%4")
                        .arg(m_previousAnswerAreas[hwnd].width()).arg(m_previousAnswerAreas[hwnd].height())
                        .arg(answerArea.width()).arg(answerArea.height()));
        m_previousAnswerAreas[hwnd] = answerArea;
        m_stableFrameCounts[hwnd] = 0;
        m_hasDetectedChanges[hwnd] = true;
        return false;
    }

    // 计算差异像素�?    int diffCount = 0;
    int totalPixels = answerArea.width() * answerArea.height();
    double diffThreshold = 0.005; // 0.5%差异视为有效变化
    int pixelThreshold = static_cast<int>(totalPixels * diffThreshold);

    // 采样计算差异（为提高性能，每隔几个像素采样一次）
    int sampleStep = 3;
    for (int y = 0; y < answerArea.height(); y += sampleStep) {
        for (int x = 0; x < answerArea.width(); x += sampleStep) {
            QRgb prevPixel = m_previousAnswerAreas[hwnd].pixel(x, y);
            QRgb currPixel = answerArea.pixel(x, y);

            // 比较RGB差异，使用阈值避免轻微噪�?            int rDiff = qAbs(qRed(prevPixel) - qRed(currPixel));
            int gDiff = qAbs(qGreen(prevPixel) - qGreen(currPixel));
            int bDiff = qAbs(qBlue(prevPixel) - qBlue(currPixel));
            
            // 只有当差异超过一定阈值时才计�?            if (rDiff > 5 || gDiff > 5 || bDiff > 5) {
                diffCount++;
                if (diffCount > pixelThreshold) {
                    emit logMessage(QString("检测到回答区域变化: %1 像素 (阈�? %2)")
                                .arg(diffCount * sampleStep * sampleStep)
                                .arg(pixelThreshold * sampleStep * sampleStep));
                    m_previousAnswerAreas[hwnd] = answerArea;
                    m_stableFrameCounts[hwnd] = 0;
                    m_hasDetectedChanges[hwnd] = true;
                    return false; // 检测到变化，但还不算完�?                }
            }
        }
    }

    // 如果之前检测到过变化，且当前帧稳定（无变化），增加稳定帧计�?    if (m_hasDetectedChanges[hwnd] && diffCount <= pixelThreshold) {
        m_stableFrameCounts[hwnd]++;
        emit logMessage(QString("回答区域稳定: %1 �?).arg(m_stableFrameCounts[hwnd]));
        
        // 连续3帧稳定，认为回答已完�?        if (m_stableFrameCounts[hwnd] >= 3) {
            emit logMessage("检测到回答已完�?);
            m_previousAnswerAreas[hwnd] = answerArea;
            m_stableFrameCounts[hwnd] = 0;
            m_hasDetectedChanges[hwnd] = false;
            return true; // 回答完成
        }
    } else {
        m_stableFrameCounts[hwnd] = 0; // 重置稳定帧计�?    }

    m_previousAnswerAreas[hwnd] = answerArea;
    return false; // 回答未完�?}

QImage ImageRecognizer::captureScreen(int screenIndex) {
    // 使用Windows API直接捕获指定屏幕，支持多显示�?    QList<QScreen*> screens = QGuiApplication::screens();
    if (screenIndex < 0 || screenIndex >= screens.size()) {
        emit logMessage(QString("无效的屏幕索�? %1，使用主屏幕").arg(screenIndex));
        screenIndex = 0;
    }
    
    QScreen* screen = screens[screenIndex];
    QRect screenGeometry = screen->geometry();
    
    // 捕获指定屏幕的全�?    return captureScreenArea(screenGeometry, screenIndex);
}

QImage ImageRecognizer::captureScreenArea(const QRect &area, int screenIndex) {
    // 使用Windows API直接捕获屏幕，支持多显示�?    if (area.isEmpty()) {
        emit logMessage("无效的捕获区�?);
        return QImage();
    }

    // 获取所有屏幕信�?    QList<QScreen*> screens = QGuiApplication::screens();
    
    // 确定要捕获的屏幕
    QScreen* targetScreen = nullptr;
    if (screenIndex >= 0 && screenIndex < screens.size()) {
        targetScreen = screens[screenIndex];
    } else {
        // 查找包含指定区域的屏�?        for (QScreen* screen : screens) {
            if (screen->geometry().intersects(area)) {
                targetScreen = screen;
                break;
            }
        }
        
        if (!targetScreen) {
            targetScreen = QGuiApplication::primaryScreen();
            emit logMessage("无法确定目标屏幕，使用主屏幕");
        }
    }
    
    // 获取屏幕DC
    HDC hScreenDC = GetDC(NULL);
    if (!hScreenDC) {
        emit logMessage("无法获取屏幕DC");
        return QImage();
    }

    // 创建内存DC
    HDC hMemDC = CreateCompatibleDC(hScreenDC);
    if (!hMemDC) {
        emit logMessage("无法创建内存DC");
        ReleaseDC(NULL, hScreenDC);
        return QImage();
    }

    // 创建位图
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, area.width(), area.height());
    if (!hBitmap) {
        emit logMessage("无法创建位图");
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hScreenDC);
        return QImage();
    }

    // 选择位图到内存DC
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

    // 复制屏幕内容到内存DC
    BOOL result = BitBlt(hMemDC, 0, 0, area.width(), area.height(), 
                        hScreenDC, area.x(), area.y(), SRCCOPY);
    if (!result) {
        emit logMessage("无法复制屏幕内容");
        SelectObject(hMemDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hScreenDC);
        return QImage();
    }

    // 获取位图信息
    BITMAPINFO bmpInfo;
    memset(&bmpInfo, 0, sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = area.width();
    bmpInfo.bmiHeader.biHeight = -area.height(); // 负高度表示从上到�?    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    // 创建QImage
    QImage image(area.width(), area.height(), QImage::Format_ARGB32);
    GetDIBits(hMemDC, hBitmap, 0, area.height(), image.bits(), &bmpInfo, DIB_RGB_COLORS);

    // 恢复资源
    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(NULL, hScreenDC);

    emit logMessage(QString("成功捕获屏幕区域: %1x%2，屏幕索�? %3").arg(area.width()).arg(area.height()).arg(screenIndex));
    return image;
}

void ImageRecognizer::setRecognitionThreshold(double threshold) {
    this->threshold = threshold;
}

QVector<QPoint> ImageRecognizer::findTemplate(const QImage &sourceImage, const QString &templateName) {
    QVector<QPoint> matches;
    
    // 检查是否请求停�?    if (m_stopRequested) {
        emit logMessage("[INFO] 收到停止请求，退出findTemplate");
        return matches;
    }
    
    // 检查模板是否存�?    if (!templates.contains(templateName)) {
        emit logMessage("模板未加�? " + templateName);
        return matches;
    }

    QImage templateImage = templates[templateName];
    QImage sourceGray = toGrayscale(sourceImage);
    QImage templateGray = toGrayscale(templateImage);
    
    // 检查模板尺寸是否有�?    if (templateGray.width() > sourceGray.width() || templateGray.height() > sourceGray.height()) {
        emit logMessage(QString("模板尺寸 %1x%2 大于源图�?%3x%4").arg(templateGray.width()).arg(templateGray.height()).arg(sourceGray.width()).arg(sourceGray.height()));
        return matches;
    }
    
    // 转换为OpenCV Mat
    Mat sourceMat = QImageToMat(sourceGray);
    Mat templateMat = QImageToMat(templateGray);
    
    // 使用最有效的匹配方�?    int matchMethod = TM_CCOEFF_NORMED;
    Mat result;
    matchTemplate(sourceMat, templateMat, result, matchMethod);
    
    // 获取模板尺寸，优先使用配置文件中保存的尺�?    ConfigManager* config = ConfigManager::getInstance();
    QSize configTemplateSize = config->getTemplateSize(templateName);
    int templateWidth = configTemplateSize.width();
    int templateHeight = configTemplateSize.height();
    
    // 如果配置文件中没有保存尺寸，使用模板图像的实际尺�?    if (templateWidth == 100 && templateHeight == 100) {
        templateWidth = templateGray.width();
        templateHeight = templateGray.height();
        config->setTemplateSize(templateName, QSize(templateWidth, templateHeight));
    }
    
    // 为不同模板设置合适的高阈值，减少误匹�?    double adjustedThreshold = 0.95; // 默认高阈�?    if (templateName.contains("workbench")) {
        adjustedThreshold = 0.97;
    } else if (templateName.contains("mindspark")) {
        adjustedThreshold = 0.96;
    } else if (templateName.contains("input_box")) {
        adjustedThreshold = 0.94;
    } else if (templateName.contains("send_button")) {
        adjustedThreshold = 0.96;
    }
    
    // 遍历匹配结果，寻找高得分匹配�?    for (int y = 0; y < result.rows; y++) {
        for (int x = 0; x < result.cols; x++) {
            float matchVal = result.at<float>(y, x);
            double score = matchVal; // TM_CCOEFF_NORMED得分范围[-1, 1]，值越大越�?            
            if (score >= adjustedThreshold) {
                QPoint matchPoint(x, y);
                
                // 简单的非极大值抑制，去除邻近的重复匹�?                bool isUnique = true;
                int suppressionRadius = qMax(templateWidth, templateHeight) / 2;
                suppressionRadius = qMin(suppressionRadius, 50); // 限制最大抑制半�?                
                for (const QPoint& existingMatch : matches) {
                    int dx = abs(existingMatch.x() - x);
                    int dy = abs(existingMatch.y() - y);
                    if (dx <= suppressionRadius && dy <= suppressionRadius) {
                        isUnique = false;
                        break;
                    }
                }
                
                if (isUnique) {
                    matches.append(matchPoint);
                    emit logMessage(QString("找到匹配，位�?%1,%2)，得�? %3").arg(x).arg(y).arg(score));
                }
            }
        }
    }
    
    // 限制最大匹配数量，避免误匹�?    if (matches.size() > 2) {
        matches.resize(2);
    }
    
    emit logMessage(QString("findTemplate完成，模�? %1，找�?%2 个匹配点").arg(templateName).arg(matches.size()));
    
    // 如果没有找到匹配，发出信�?    if (matches.isEmpty()) {
        emit templateNotFound(templateName);
    }
    
    return matches;
}

QPoint ImageRecognizer::findBestMatch(const QVector<QPoint> &matches, const QImage &sourceImage) {
    // 查找最佳匹配点
    if (matches.isEmpty()) {
        return QPoint();
    }
    
    // 遍历所有匹配点，找到最佳匹配点
    // 优先选择�?0,0)位置的匹配点
    for (const QPoint& match : matches) {
        // 跳过(0,0)附近的匹配点
        if (match.x() > 10 || match.y() > 10) {
            return match;
        }
    }
    
    // 如果所有匹配点都在(0,0)附近，返回距离图像中心最近的匹配�?    QPoint center(sourceImage.width() / 2, sourceImage.height() / 2);
    QPoint bestMatch = matches.first();
    double bestDistance = QLineF(bestMatch, center).length();
    
    for (const QPoint& match : matches) {
        double distance = QLineF(match, center).length();
        if (distance < bestDistance) {
            bestMatch = match;
            bestDistance = distance;
        }
    }
    
    emit logMessage(QString("[DEBUG] 选择最佳匹配点: (%1,%2)，距离中�? %3").arg(bestMatch.x()).arg(bestMatch.y()).arg(bestDistance));
    return bestMatch;
}

QImage ImageRecognizer::toGrayscale(const QImage &image) {
    // 转换为灰度图
    return image.convertToFormat(QImage::Format_Grayscale8);
}

double ImageRecognizer::matchTemplateNCC(const QImage &source, const QImage &templateImg, int x, int y) {
    // 归一化交叉相关匹配（NCC�?    double sumSource = 0.0;
    double sumTemplate = 0.0;
    double sumProduct = 0.0;
    double sumSourceSq = 0.0;
    double sumTemplateSq = 0.0;

    // 预计算模板图像的灰度值，避免重复调用qGray
    QVector<uchar> templateGrays(templateImg.width() * templateImg.height());
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            int index = ty * templateImg.width() + tx;
            templateGrays[index] = qGray(templateImg.pixel(tx, ty));
            sumTemplate += templateGrays[index];
            sumTemplateSq += templateGrays[index] * templateGrays[index];
        }
    }

    // 优化匹配计算，使用更高效的像素访�?    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            // 检查坐标是否超出范围，避免崩溃
            if (x + tx >= source.width() || y + ty >= source.height()) {
                return 0.0;
            }
            
            uchar s = qGray(source.pixel(x + tx, y + ty));
            uchar t = templateGrays[ty * templateImg.width() + tx];

            sumSource += s;
            sumProduct += s * t;
            sumSourceSq += s * s;
        }
    }

    int n = templateImg.width() * templateImg.height();
    double numerator = sumProduct - (sumSource * sumTemplate / n);
    double denominator = sqrt(
        (sumSourceSq - (sumSource * sumSource / n)) *
        (sumTemplateSq - (sumTemplate * sumTemplate / n))
    );

    return (denominator == 0.0) ? 0.0 : numerator / denominator;
}

double ImageRecognizer::matchTemplateSSD(const QImage &source, const QImage &templateImg, int x, int y) {
    // 平方差匹配（SSD�?    double sumSquaredDiff = 0.0;
    
    // 预计算模板图像的灰度值，避免重复调用qGray
    QVector<uchar> templateGrays(templateImg.width() * templateImg.height());
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            int index = ty * templateImg.width() + tx;
            templateGrays[index] = qGray(templateImg.pixel(tx, ty));
        }
    }

    // 计算平方�?    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            // 检查坐标是否超出范围，避免崩溃
            if (x + tx >= source.width() || y + ty >= source.height()) {
                return 1.0; // 返回最大值表示不匹配
            }
            
            uchar s = qGray(source.pixel(x + tx, y + ty));
            uchar t = templateGrays[ty * templateImg.width() + tx];
            
            double diff = s - t;
            sumSquaredDiff += diff * diff;
        }
    }
    
    // 归一化到0-1范围�?表示完全匹配
    double maxDiff = templateImg.width() * templateImg.height() * 255.0 * 255.0;
    double normalizedDiff = sumSquaredDiff / maxDiff;
    
    // 返回1-normalizedDiff，使得值越大表示匹配越�?    return 1.0 - normalizedDiff;
}

double ImageRecognizer::matchTemplateSAD(const QImage &source, const QImage &templateImg, int x, int y) {
    // 绝对差匹配（SAD�?    double sumAbsoluteDiff = 0.0;
    
    // 预计算模板图像的灰度值，避免重复调用qGray
    QVector<uchar> templateGrays(templateImg.width() * templateImg.height());
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            int index = ty * templateImg.width() + tx;
            templateGrays[index] = qGray(templateImg.pixel(tx, ty));
        }
    }

    // 计算绝对�?    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            // 检查坐标是否超出范围，避免崩溃
            if (x + tx >= source.width() || y + ty >= source.height()) {
                return 1.0; // 返回最大值表示不匹配
            }
            
            uchar s = qGray(source.pixel(x + tx, y + ty));
            uchar t = templateGrays[ty * templateImg.width() + tx];
            
            sumAbsoluteDiff += qAbs(s - t);
        }
    }
    
    // 归一化到0-1范围�?表示完全匹配
    double maxDiff = templateImg.width() * templateImg.height() * 255.0;
    double normalizedDiff = sumAbsoluteDiff / maxDiff;
    
    // 返回1-normalizedDiff，使得值越大表示匹配越�?    return 1.0 - normalizedDiff;
}

QVector<QPoint> ImageRecognizer::matchTemplateORB(const QImage &source, const QImage &templateImg) {
    // ORB特征匹配
    QVector<QPoint> matches;
    
    // 简单实现：使用模板中心作为匹配�?    // 注意：实际ORB匹配需要更复杂的实现，这里仅作为示�?    // 实际项目中可以使用OpenCV库来实现完整的ORB匹配
    
    // 计算模板在源图像中的中心位置
    int centerX = source.width() / 2 - templateImg.width() / 2;
    int centerY = source.height() / 2 - templateImg.height() / 2;
    
    // 确保位置有效
    if (centerX >= 0 && centerY >= 0) {
        matches.append(QPoint(centerX, centerY));
    }
    
    return matches;
}

void ImageRecognizer::loadTemplates() {
    // 加载所有图像模�?    // 从配置文件中获取模板路径
    ConfigManager* config = ConfigManager::getInstance();
    
    // 加载工作台图�?    QString workbenchPath = config->getIconPath("workbench");
    if (!workbenchPath.isEmpty()) {
        QImage workbenchImage(workbenchPath);
        if (!workbenchImage.isNull()) {
            templates["workbench"] = workbenchImage;
            emit logMessage(QString("成功加载模板: workbench，路�? %1，尺�? %2x%3")
                           .arg(workbenchPath)
                           .arg(workbenchImage.width()).arg(workbenchImage.height()));
        } else {
            emit logMessage("无法加载工作台图�? " + workbenchPath);
        }
    }
    
    // 加载微搜索图�?    QString mindsparkPath = config->getIconPath("mindspark");
    if (!mindsparkPath.isEmpty()) {
        QImage mindsparkImage(mindsparkPath);
        if (!mindsparkImage.isNull()) {
            templates["mindspark"] = mindsparkImage;
            emit logMessage(QString("成功加载模板: mindspark，路�? %1，尺�? %2x%3")
                           .arg(mindsparkPath)
                           .arg(mindsparkImage.width()).arg(mindsparkImage.height()));
        } else {
            emit logMessage("无法加载微搜索图�? " + mindsparkPath);
        }
    }
    
    // 加载输入框模�?    QString inputBoxPath = config->getIconPath("input_box");
    if (!inputBoxPath.isEmpty()) {
        QImage inputBoxImage(inputBoxPath);
        if (!inputBoxImage.isNull()) {
            templates["input_box"] = inputBoxImage;
            emit logMessage(QString("成功加载模板: input_box，路�? %1，尺�? %2x%3")
                           .arg(inputBoxPath)
                           .arg(inputBoxImage.width()).arg(inputBoxImage.height()));
        } else {
            emit logMessage("无法加载输入框模�? " + inputBoxPath);
        }
    }
    
    // 加载发送按钮模�?    QString sendButtonPath = config->getIconPath("send_button");
    if (!sendButtonPath.isEmpty()) {
        QImage sendButtonImage(sendButtonPath);
        if (!sendButtonImage.isNull()) {
            templates["send_button"] = sendButtonImage;
            emit logMessage(QString("成功加载模板: send_button，路�? %1，尺�? %2x%3")
                           .arg(sendButtonPath)
                           .arg(sendButtonImage.width()).arg(sendButtonImage.height()));
        } else {
            emit logMessage("无法加载发送按钮模�? " + sendButtonPath);
        }
    }
    
    emit logMessage(QString("模板加载完成，当前模板数�? %1").arg(templates.size()));
}

void ImageRecognizer::onConfigChanged() {
    // 更新识别阈�?    threshold = ConfigManager::getInstance()->getImageRecognitionThreshold();
    maxAttempts = ConfigManager::getInstance()->getMaxRecognitionAttempts();
    
    // 重新加载模板
    templates.clear();
    loadTemplates();
}

QImage ImageRecognizer::captureWindow(HWND hwnd) {
    // 使用Windows API捕获指定窗口
    if (!hwnd) {
        emit logMessage("无效的窗口句�?);
        return QImage();
    }
    
    // 获取窗口矩形
    RECT windowRect;
    if (!GetWindowRect(hwnd, &windowRect)) {
        emit logMessage("无法获取窗口矩形");
        return QImage();
    }
    
    // 计算窗口尺寸
    int width = windowRect.right - windowRect.left;
    int height = windowRect.bottom - windowRect.top;
    
    // 捕获窗口区域
    return captureScreenArea(QRect(windowRect.left, windowRect.top, width, height));
}

bool ImageRecognizer::findTemplateInWindow(HWND hwnd, const QString &templateName, QPoint &resultPos) {
    // 在指定窗口中查找模板
    QImage windowImage = captureWindow(hwnd);
    if (windowImage.isNull()) {
        emit logMessage("无法捕获窗口图像");
        return false;
    }
    
    // 保存全屏截图用于调试
    saveImageForDebug(windowImage, "window_full");
    
    // 查找模板 - 确保对所有区域进行完整识�?    QVector<QPoint> matches = findTemplate(windowImage, templateName);
    if (matches.isEmpty()) {
        return false;
    }
    
    // 对所有匹配点进行处理，而不是只返回第一�?        for (const QPoint& match : matches) {
            // 发射识别区域信号，用于显示识别框
            if (templates.contains(templateName)) {
                // 使用配置文件中保存的模板尺寸计算识别区域
                ConfigManager* config = ConfigManager::getInstance();
                QSize templateSize = config->getTemplateSize(templateName);
                
                // 如果配置文件中没有保存尺寸，使用模板图像的实际尺�?                QImage templateImage = templates[templateName];
                if (templateSize.width() == 100 && templateSize.height() == 100) {
                    templateSize = templateImage.size();
                }
                
                QRect recognitionArea(match.x(), match.y(),
                             templateSize.width(), templateSize.height());
                emit recognitionAreaFound(recognitionArea, templateName);
                
                // 保存识别区域截图
                saveRecognitionAreaImage(windowImage, match, templateName, "window_recognition");
            }
        }
    
    // 选择最佳匹配点
    QPoint bestMatch = findBestMatch(matches, windowImage);
    resultPos = bestMatch;
    
    return true;
}

void ImageRecognizer::stopRecognition() {
    // 停止识别
    m_stopRequested = true;
    emit logMessage("收到停止识别请求");
}

void ImageRecognizer::resetStopFlag() {
    // 重置停止标志
    m_stopRequested = false;
    emit logMessage("重置停止标志");
}

QPoint ImageRecognizer::getCurrentMousePosition() {
    // 获取当前鼠标位置
    POINT point;
    GetCursorPos(&point);
    return QPoint(point.x, point.y);
}

bool ImageRecognizer::saveImageForDebug(const QImage &image, const QString &prefix, const QString &subfolder) {
    // 保存图像用于调试，支持子文件�?    QString projectPath = QCoreApplication::applicationDirPath();
    QString debugDir = projectPath + "/debug_tu/" + subfolder + "/";
    
    QDir dir;
    bool dirCreated = dir.mkpath(debugDir);
    emit logMessage(QString("调试创建目录: %1，结�? %2").arg(debugDir).arg(dirCreated ? "成功" : "失败"));
    
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
    QString filename = debugDir + prefix + "_" + timestamp + ".png";
    
    if (image.isNull()) {
        emit logMessage(QString("无法保存调试图像: %1，图像为�?).arg(filename));
        return false;
    }
    
    if (image.save(filename)) {
        emit logMessage(QString("调试图像已保�? %1").arg(filename));
        return true;
    } else {
        emit logMessage(QString("无法保存调试图像: %1").arg(filename));
        return false;
    }
}

// 重载版本，默认保存到full_screenshots文件�?bool ImageRecognizer::saveImageForDebug(const QImage &image, const QString &prefix) {
    return saveImageForDebug(image, prefix, "full_screenshots");
}

bool ImageRecognizer::saveRecognitionAreaImage(const QImage &sourceImage, const QPoint &matchPoint, const QString &templateName, const QString &prefix) {
    // 保存识别区域截图，只截取识别框区�?    if (!templates.contains(templateName)) {
        emit logMessage("模板未加�? " + templateName);
        return false;
    }
    
    // 优先使用配置文件中保存的模板尺寸
    ConfigManager* config = ConfigManager::getInstance();
    QSize templateSize = config->getTemplateSize(templateName);
    
    // 如果配置文件中没有保存尺寸，使用模板图像的实际尺�?    if (templateSize.width() == 100 && templateSize.height() == 100) { // 默认�?        QImage templateImage = templates[templateName];
        templateSize = templateImage.size();
    }
    
    QRect recognitionRect(matchPoint.x(), matchPoint.y(), templateSize.width(), templateSize.height());
    
    // 确保识别区域在源图像范围�?    if (!sourceImage.rect().contains(recognitionRect)) {
        emit logMessage("识别区域超出源图像范�?);
        return false;
    }
    
    // 截取识别区域
    QImage recognitionArea = sourceImage.copy(recognitionRect);
    
    // 保存识别区域图像到日志目录的recognition_areas子文件夹
    return saveImageToLogDir(recognitionArea, prefix + "_recognition_area", "recognition_areas");
}

bool ImageRecognizer::saveImageToLogDir(const QImage &image, const QString &prefix, const QString &subfolder) {
    // 保存图像到日志目录，支持子文件夹
    ConfigManager* config = ConfigManager::getInstance();
    QString logDir = config->getLogPath() + subfolder + "/";
    
    QDir dir;
    bool dirCreated = dir.mkpath(logDir);
    emit logMessage(QString("调试创建日志目录: %1，结�? %2").arg(logDir).arg(dirCreated ? "成功" : "失败"));
    
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
    QString filename = logDir + prefix + "_" + timestamp + ".png";
    
    if (image.isNull()) {
        emit logMessage(QString("无法保存日志图像: %1，图像为�?).arg(filename));
        return false;
    }
    
    if (image.save(filename)) {
        emit logMessage(QString("日志图像已保�? %1").arg(filename));
        return true;
    } else {
        emit logMessage(QString("无法保存日志图像: %1").arg(filename));
        return false;
    }
}

// 重载版本，默认保存到full_screenshots文件�?bool ImageRecognizer::saveImageToLogDir(const QImage &image, const QString &prefix) {
    return saveImageToLogDir(image, prefix, "full_screenshots");
}

bool ImageRecognizer::isStopRequested() const {
    return m_stopRequested;
}

void ImageRecognizer::setStopRequested(bool stop) {
    m_stopRequested = stop;
}

HWND ImageRecognizer::findChildWindowWithText(HWND parentHwnd, const QString &text, bool exactMatch) {
    // 查找包含指定文本的子窗口
    HWND foundHwnd = nullptr;
    
    // 简单实现：直接使用FindWindowEx查找
    // 注意：这个实现可能无法找到所有子窗口，因为FindWindowEx只查找直接子窗口
    // 但对于当前需求来说，这个实现应该足够用了
    HWND childHwnd = FindWindowExW(parentHwnd, nullptr, nullptr, nullptr);
    while (childHwnd != nullptr) {
        // 获取窗口文本
        wchar_t buffer[256];
        GetWindowTextW(childHwnd, buffer, sizeof(buffer) / sizeof(wchar_t));
        QString windowText = QString::fromWCharArray(buffer);
        
        // 检查文本是否匹�?        if (exactMatch) {
            if (windowText == text) {
                foundHwnd = childHwnd;
                break;
            }
        } else {
            if (windowText.contains(text, Qt::CaseInsensitive)) {
                foundHwnd = childHwnd;
                break;
            }
        }
        
        // 查找下一个子窗口
        childHwnd = FindWindowExW(parentHwnd, childHwnd, nullptr, nullptr);
    }
    
    return foundHwnd;
}

HWND ImageRecognizer::findChildWindowWithClass(HWND parentHwnd, const QString &className) {
    // 查找包含指定类名的子窗口
    return FindWindowExW(parentHwnd, nullptr, reinterpret_cast<LPCWSTR>(className.utf16()), nullptr);
}

QRect ImageRecognizer::getWindowRect(HWND hwnd) {
    // 获取窗口矩形
    RECT rect;
    if (GetWindowRect(hwnd, &rect)) {
        return QRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    }
    return QRect();
}

void ImageRecognizer::doFindTemplateInWindow(HWND hwnd, const QString &templateName) {
    // 异步查找模板
    QPoint resultPos;
    if (findTemplateInWindow(hwnd, templateName, resultPos)) {
        emit templateFound(templateName, resultPos);
    } else {
        emit templateNotFound(templateName);
    }
}

bool ImageRecognizer::loadTemplate(const QString &name, const QString &path) {
    // 加载单个模板
    QImage templateImage(path);
    if (!templateImage.isNull()) {
        templates[name] = templateImage;
        emit logMessage(QString("成功加载模板: %1，路�? %2，尺�? %3x%4")
                       .arg(name)
                       .arg(path)
                       .arg(templateImage.width()).arg(templateImage.height()));
        
        // 保存模板尺寸到配置文�?        ConfigManager* config = ConfigManager::getInstance();
        config->setTemplateSize(name, templateImage.size());
        emit logMessage(QString("已保存模板尺寸到配置: %1 -> %2x%3")
                       .arg(name)
                       .arg(templateImage.width()).arg(templateImage.height()));
        
        return true;
    } else {
        emit logMessage("无法加载模板: " + path);
        return false;
    }
}

void ImageRecognizer::checkAnswerReceivedAsync(HWND hwnd) {
    // 异步检查是否收到回�?    bool received = checkAnswerReceived(hwnd);
    emit answerReceivedChecked(hwnd, received);
}

