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

// OpenCV相关头文件
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>

// 使用OpenCV命名空间
using namespace cv;
using namespace std;

// MatchResult结构体定义（已删除）

// QImage转Mat的辅助函数
cv::Mat ImageRecognizer::QImageToMat(const QImage &image) {
    Mat mat;
    
    if (image.format() == QImage::Format_Grayscale8) {
        // 灰度图像直接转换
        mat = Mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine());
    } else if (image.format() == QImage::Format_ARGB32 || image.format() == QImage::Format_RGB32) {
        // RGBA或RGB图像转换为灰度
        mat = Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        cvtColor(mat, mat, COLOR_BGRA2GRAY);
    } else {
        // 其他格式图像先转换为灰度
        QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);
        mat = Mat(grayImage.height(), grayImage.width(), CV_8UC1, const_cast<uchar*>(grayImage.bits()), grayImage.bytesPerLine());
    }
    
    return mat.clone(); // 返回克隆的Mat对象，避免内存问题
}


ImageRecognizer::ImageRecognizer(QObject *parent) : QObject(parent) {
    // 初始化默认参数
    threshold = ConfigManager::getInstance()->getImageRecognitionThreshold();
    maxAttempts = ConfigManager::getInstance()->getMaxRecognitionAttempts();
    m_stopRequested = false;

    // 清理旧的调试目录
    QString projectPath = QCoreApplication::applicationDirPath();
    QString debugDir = projectPath + "/debug_tu/";
    QDir dir(debugDir);
    if (dir.exists()) {
        // 清除目录下所有文件
        QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        for (const QString &file : files) {
            bool removed = dir.remove(file);
            emit logMessage(QString("删除旧文件: %1 结果: %2").arg(file).arg(removed ? "成功" : "失败"));
        }
        emit logMessage("旧debug_tu目录已清理");
    } else {
        // 创建新的调试目录
        bool dirCreated = dir.mkpath(debugDir);
        emit logMessage(QString("创建目录: %1 结果: %2").arg(debugDir).arg(dirCreated ? "成功" : "失败"));
    }

    // 连接配置变更信号
    connect(ConfigManager::getInstance(), &ConfigManager::configChanged,
            this, &ImageRecognizer::onConfigChanged);
    
    // 创建工作线程
    workerThread = new QThread(this);
    // 将当前对象移动到工作线程
    this->moveToThread(workerThread);
    // 启动工作线程
    workerThread->start();
}

ImageRecognizer::~ImageRecognizer() {
    // 清理工作线程
    workerThread->quit();
    workerThread->wait();
    delete workerThread;
}

QRect ImageRecognizer::findImageOnScreen(const QString &templatePath, int screenIndex) {
    QImage screenImage = captureScreen(screenIndex);
    if (screenImage.isNull()) {
        emit logMessage("截图失败");
        return QRect();
    }

    // 加载模板图像
    QImage templateImage(templatePath);
    if (templateImage.isNull()) {
        emit logMessage("无法加载模板图像: " + templatePath);
        return QRect();
    }

    // 转换为灰度图像
    screenImage = toGrayscale(screenImage);
    templateImage = toGrayscale(templateImage);

    // 检查模板尺寸是否合适
    if (templateImage.width() > screenImage.width() ||
        templateImage.height() > screenImage.height()) {
        emit logMessage("模板尺寸过大");
        return QRect();
    }

    // 尝试多次识别 - 最多maxAttempts次
    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        // 临时加载模板用于findTemplate函数
        QString templateName = "temp_template";
        loadTemplate(templateName, templatePath);
        
        QVector<QPoint> matches = findTemplate(screenImage, templateName);
        if (!matches.isEmpty()) {
        // 遍历所有匹配点
            for (const QPoint& match : matches) {
                // 处理每个匹配点
                ConfigManager* config = ConfigManager::getInstance();
                QSize templateSize = config->getTemplateSize(templateName);
                
                // 调整模板大小，确保与实际图像匹配
                if (templateSize.width() == 100 && templateSize.height() == 100) {
                    templateSize = templateImage.size();
                }
                
                QRect recognitionArea(match.x(), match.y(),
                             templateSize.width(), templateSize.height());
                
                // 发送识别区域信号
                emit recognitionAreaFound(recognitionArea, templatePath);
                
                // 删除了保存识别区域截图功能
            }
            
            // 查找最佳匹配点
        QPoint bestMatch = findBestMatch(matches, screenImage);
        
        // 检查最佳匹配点是否有效
        if (bestMatch.x() < 0 || bestMatch.y() < 0) {
            emit logMessage("[DEBUG] 未找到有效匹配点");
            return QRect();
        }
        
        QRect bestRecognitionArea(bestMatch.x(), bestMatch.y(),
                     templateImage.width(), templateImage.height());
        
        return bestRecognitionArea;
        }

        emit logMessage(QString("识别失败，第%1次重试").arg(attempt + 1));
        QThread::sleep(1); // 等待1秒后重试
        screenImage = captureScreen(screenIndex); // 重新截图
        if (screenImage.isNull()) {
            emit logMessage("截图失败");
            return QRect();
        }

        // 转换为灰度图像
        screenImage = toGrayscale(screenImage);
    }

    return QRect();
}

bool ImageRecognizer::checkAnswerReceived(HWND hwnd) {
    // 检查回答是否完成的核心逻辑
    // 基于图像比对技术，检测回答区域是否稳定

    if (!hwnd) {
        emit logMessage("无效的窗口句柄");
        return false;
    }
    
    // 等待2秒，确保页面已经成功加载
    emit logMessage("等待2秒确保页面加载完成");
    QThread::sleep(2);
    
    const int maxFailedAttempts = 5; // 最多尝试5次失败后重置
    
    // 初始化窗口状态
    if (!m_inputBoxFound.contains(hwnd)) {
        m_inputBoxFound[hwnd] = false;
        m_failedAttempts[hwnd] = 0;
        m_stableFrameCounts[hwnd] = 0;
        m_hasDetectedChanges[hwnd] = false;
    }
    
    // 检查窗口大小变化
    RECT clientRect;
    if (GetClientRect(hwnd, &clientRect)) {
        int currentWidth = clientRect.right - clientRect.left;
        // 如果窗口宽度变化，重置状态
        if (currentWidth != m_clientWidths[hwnd]) {
            m_clientWidths[hwnd] = currentWidth;
            m_inputBoxFound[hwnd] = false; // 窗口大小变化，重新查找输入框
            emit logMessage(QString("窗口宽度变化: %1像素，重新初始化").arg(currentWidth));
        }
    }
    
    // 检查输入框位置
    // 1. 查找输入框
    // 2. 确定回答区域
    // 3. 截图回答区域
    // 4. 比较前后帧差异
    if (!m_inputBoxFound[hwnd] || m_failedAttempts[hwnd] >= maxFailedAttempts) {
        // 重置失败计数
        m_failedAttempts[hwnd] = 0;
        
        // 1. 查找输入框
        // 尝试多种输入框模板
        QStringList inputBoxTemplates = {"input_box", "input_box_small", "input_box_large"};
        
        m_inputBoxFound[hwnd] = false;
        QPoint inputBoxPos;
        for (const QString& templateName : inputBoxTemplates) {
            if (findTemplateInWindow(hwnd, templateName, inputBoxPos)) {
                m_inputBoxFound[hwnd] = true;
                m_inputBoxPositions[hwnd] = inputBoxPos;
                emit logMessage(QString("输入框位置: (%1, %2)").arg(inputBoxPos.x()).arg(inputBoxPos.y()));
                
                // 更新窗口宽度
                if (GetClientRect(hwnd, &clientRect)) {
                    m_clientWidths[hwnd] = clientRect.right - clientRect.left;
                } else {
                    m_clientWidths[hwnd] = 0;
                    emit logMessage("无法获取窗口尺寸");
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
    
    // 2. 确定回答区域，回答区域是输入框上方的区域
    int answerAreaX = 0;
    int answerAreaY = 0;
    int answerAreaWidth = m_clientWidths[hwnd];
    int answerAreaHeight = m_inputBoxPositions[hwnd].y() - 20; // 减去20像素作为安全距离
    
    // 检查回答区域是否有效
    if (answerAreaHeight <= 0 || m_clientWidths[hwnd] <= 0) {
        emit logMessage("回答区域无效");
        m_failedAttempts[hwnd]++;
        return false;
    }
    
    // 3. 截图回答区域
    QImage windowImage = captureWindow(hwnd);
    if (windowImage.isNull()) {
        emit logMessage("窗口截图失败");
        m_failedAttempts[hwnd]++;
        return false;
    }
    
    // 检查截图尺寸是否足够
    if (windowImage.width() < answerAreaWidth || windowImage.height() < m_inputBoxPositions[hwnd].y()) {
        emit logMessage("截图尺寸不足，无法截取完整回答区域");
        m_failedAttempts[hwnd]++;
        return false;
    }
    
    QImage answerArea = windowImage.copy(answerAreaX, answerAreaY, answerAreaWidth, answerAreaHeight);
    
    // 4. 比较前后帧差异
    
    // 首次执行，保存当前帧
    if (!m_previousAnswerAreas.contains(hwnd) || m_previousAnswerAreas[hwnd].isNull()) {
        m_previousAnswerAreas[hwnd] = answerArea;
        m_stableFrameCounts[hwnd] = 0;
        m_hasDetectedChanges[hwnd] = false;
        emit logMessage("首次执行，保存当前回答区域");
        return false;
    }

    // 检查回答区域大小是否变化
    if (m_previousAnswerAreas[hwnd].size() != answerArea.size()) {
        emit logMessage(QString("回答区域大小变化: %1x%2 -> %3x%4")
                        .arg(m_previousAnswerAreas[hwnd].width()).arg(m_previousAnswerAreas[hwnd].height())
                        .arg(answerArea.width()).arg(answerArea.height()));
        m_previousAnswerAreas[hwnd] = answerArea;
        m_stableFrameCounts[hwnd] = 0;
        m_hasDetectedChanges[hwnd] = true;
        return false;
    }

    // 计算前后帧差异
    int diffCount = 0;
    int totalPixels = answerArea.width() * answerArea.height();
    double diffThreshold = 0.01; // 允许1%的像素差异作为稳定判断标准
    int pixelThreshold = static_cast<int>(totalPixels * diffThreshold);

    // 采样比较，每4个像素采样一次，提高效率
    int sampleStep = 4; // 采样步长，减少计算量
    for (int y = 0; y < answerArea.height(); y += sampleStep) {
        for (int x = 0; x < answerArea.width(); x += sampleStep) {
            QRgb prevPixel = m_previousAnswerAreas[hwnd].pixel(x, y);
            QRgb currPixel = answerArea.pixel(x, y);

            // 计算RGB三个通道的差异
            int rDiff = qAbs(qRed(prevPixel) - qRed(currPixel));
            int gDiff = qAbs(qGreen(prevPixel) - qGreen(currPixel));
            int bDiff = qAbs(qBlue(prevPixel) - qBlue(currPixel));
            
            // 如果任一通道差异超过阈值，计数加1
            if (rDiff > 10 || gDiff > 10 || bDiff > 10) { // RGB通道差异阈值
                diffCount++;
                if (diffCount > pixelThreshold) {
                    emit logMessage(QString("回答区域变化: %1 像素 (阈值: %2)").arg(diffCount * sampleStep * sampleStep).arg(pixelThreshold * sampleStep * sampleStep));
                    m_previousAnswerAreas[hwnd] = answerArea;
                    m_stableFrameCounts[hwnd] = 0;
                    m_hasDetectedChanges[hwnd] = true;
                    return false; // 变化较大，回答未完成
                }
            }
        }
    }

    // 如果之前检测到变化，现在变化稳定，计数加1
    if (m_hasDetectedChanges[hwnd] && diffCount <= pixelThreshold) {
        m_stableFrameCounts[hwnd]++;
        emit logMessage(QString("稳定帧数: %1 差异像素: %2 阈值: %3").arg(m_stableFrameCounts[hwnd]).arg(diffCount * sampleStep * sampleStep).arg(pixelThreshold * sampleStep * sampleStep));
        
        // 连续2帧稳定，认为回答完成
        if (m_stableFrameCounts[hwnd] >= 2) {
            emit logMessage("回答已完成");
            m_previousAnswerAreas[hwnd] = answerArea;
            m_stableFrameCounts[hwnd] = 0;
            m_hasDetectedChanges[hwnd] = false;
            return true; // 回答完成
        }
    } else {
        emit logMessage(QString("回答区域稳定，无明显变化: %1 像素 (阈值: %2)").arg(diffCount * sampleStep * sampleStep).arg(pixelThreshold * sampleStep * sampleStep));
        m_stableFrameCounts[hwnd] = 0; // 重置稳定计数
    }

    m_previousAnswerAreas[hwnd] = answerArea;
    return false; // 回答未完成
}

QImage ImageRecognizer::captureScreen(int screenIndex) {
    // 使用Windows API捕获指定屏幕
    QList<QScreen*> screens = QGuiApplication::screens();
    if (screenIndex < 0 || screenIndex >= screens.size()) {
        emit logMessage(QString("无效的屏幕索引: %1，使用默认屏幕").arg(screenIndex));
        screenIndex = 0;
    }
    
    QScreen* screen = screens[screenIndex];
    QRect screenGeometry = screen->geometry();
    
    // 调用captureScreenArea捕获指定区域
    return captureScreenArea(screenGeometry, screenIndex);
}

QImage ImageRecognizer::captureScreenArea(const QRect &area, int screenIndex) {
    // 使用Windows API捕获指定屏幕区域
    if (area.isEmpty()) {
        emit logMessage("捕获区域为空");
        return QImage();
    }

    // 获取所有屏幕
    QList<QScreen*> screens = QGuiApplication::screens();
    
    // 确定目标屏幕
    QScreen* targetScreen = nullptr;
    if (screenIndex >= 0 && screenIndex < screens.size()) {
        targetScreen = screens[screenIndex];
    } else {
        // 尝试找到包含目标区域的屏幕
        for (QScreen* screen : screens) {
            if (screen->geometry().intersects(area)) {
                targetScreen = screen;
                break;
            }
        }
        
        if (!targetScreen) {
            targetScreen = QGuiApplication::primaryScreen();
            emit logMessage("未找到包含目标区域的屏幕，使用主屏幕");
        }
    }
    
    // 获取屏幕DC
    HDC hScreenDC = GetDC(NULL);
    if (!hScreenDC) {
        emit logMessage("获取屏幕DC失败");
        return QImage();
    }

    // 创建兼容DC
    HDC hMemDC = CreateCompatibleDC(hScreenDC);
    if (!hMemDC) {
        emit logMessage("创建兼容DC失败");
        ReleaseDC(NULL, hScreenDC);
        return QImage();
    }

    // 创建兼容位图
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, area.width(), area.height());
    if (!hBitmap) {
        emit logMessage("创建兼容位图失败");
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hScreenDC);
        return QImage();
    }

    // 将位图选入DC
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

    // 复制屏幕内容到内存DC
    BOOL result = BitBlt(hMemDC, 0, 0, area.width(), area.height(), 
                        hScreenDC, area.x(), area.y(), SRCCOPY);
    if (!result) {
        emit logMessage("复制屏幕内容失败");
        SelectObject(hMemDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hScreenDC);
        return QImage();
    }

    // 准备位图信息
    BITMAPINFO bmpInfo;
    memset(&bmpInfo, 0, sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = area.width();
    bmpInfo.bmiHeader.biHeight = -area.height(); // 负高度表示自上而下的位图
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    // 转换为QImage
    QImage image(area.width(), area.height(), QImage::Format_ARGB32);
    GetDIBits(hMemDC, hBitmap, 0, area.height(), image.bits(), &bmpInfo, DIB_RGB_COLORS);

    // 清理资源
    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(NULL, hScreenDC);

    emit logMessage(QString("成功捕获区域: %1x%2 屏幕: %3").arg(area.width()).arg(area.height()).arg(screenIndex));
    return image;
}

void ImageRecognizer::setRecognitionThreshold(double threshold) {
    this->threshold = threshold;
}

QVector<QPoint> ImageRecognizer::findTemplate(const QImage &sourceImage, const QString &templateName) {
    QVector<QPoint> matches;
    
    // 检查是否请求停止
    if (m_stopRequested) {
        emit logMessage("[INFO] 收到停止请求，退出findTemplate");
        return matches;
    }
    
    // 检查模板是否已加载
    if (!templates.contains(templateName)) {
        emit logMessage("模板未加载: " + templateName);
        return matches;
    }

    QImage templateImage = templates[templateName];
    QImage sourceGray = toGrayscale(sourceImage);
    QImage templateGray = toGrayscale(templateImage);
    
    // 检查模板尺寸是否合适
    if (templateGray.width() > sourceGray.width() || templateGray.height() > sourceGray.height()) {
        emit logMessage(QString("模板 %1x%2 大于源图像 %3x%4").arg(templateGray.width()).arg(templateGray.height()).arg(sourceGray.width()).arg(sourceGray.height()));
        return matches;
    }
    
    // 转换为OpenCV Mat
    Mat sourceMat = QImageToMat(sourceGray);
    Mat templateMat = QImageToMat(templateGray);
    
    // 执行模板匹配
    int matchMethod = TM_CCOEFF_NORMED;
    Mat result;
    matchTemplate(sourceMat, templateMat, result, matchMethod);
    
    // 获取模板尺寸配置
    ConfigManager* config = ConfigManager::getInstance();
    QSize configTemplateSize = config->getTemplateSize(templateName);
    int templateWidth = configTemplateSize.width();
    int templateHeight = configTemplateSize.height();
    
    // 如果模板尺寸未配置（使用默认值100x100），则更新为实际尺寸
    if (templateWidth == 100 && templateHeight == 100) {
        templateWidth = templateGray.width();
        templateHeight = templateGray.height();
        config->setTemplateSize(templateName, QSize(templateWidth, templateHeight));
    }
    
    // 根据模板类型调整匹配阈值
    double adjustedThreshold = 0.95; // 默认阈值
    if (templateName.contains("workbench")) {
        adjustedThreshold = 0.97;
    } else if (templateName.contains("mindspark")) {
        adjustedThreshold = 0.96;
    } else if (templateName.contains("input_box")) {
        adjustedThreshold = 0.94;
    } else if (templateName.contains("send_button")) {
        adjustedThreshold = 0.96;
    }
    
    // 遍历匹配结果，寻找所有满足阈值的匹配点
    for (int y = 0; y < result.rows; y++) {
        for (int x = 0; x < result.cols; x++) {
            float matchVal = result.at<float>(y, x);
            double score = matchVal; // TM_CCOEFF_NORMED返回值范围[-1, 1]，越大越匹配
            
            if (score >= adjustedThreshold) {
                QPoint matchPoint(x, y);
                
                // 非极大值抑制，避免相邻重复匹配
                bool isUnique = true;
                int suppressionRadius = qMax(templateWidth, templateHeight) / 2;
                suppressionRadius = qMin(suppressionRadius, 50); // 最大抑制半径限制为50像素
                
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
                    emit logMessage(QString("匹配点(%1,%2)得分: %3").arg(x).arg(y).arg(score));
                }
            }
        }
    }
    
    // 限制最大匹配点数量为2
    if (matches.size() > 2) {
        matches.resize(2);
    }
    
    emit logMessage(QString("findTemplate完成: %1 找到 %2 个匹配点").arg(templateName).arg(matches.size()));
    
    // 如果没有找到匹配点，发送信号
    if (matches.isEmpty()) {
        emit templateNotFound(templateName);
    }
    
    return matches;
}

QPoint ImageRecognizer::findBestMatch(const QVector<QPoint> &matches, const QImage &sourceImage) {
    // 查找最佳匹配点
    Q_UNUSED(sourceImage); // 忽略未使用的参数
    if (matches.isEmpty()) {
        emit logMessage("[DEBUG] 匹配点列表为空");
        return QPoint(-1, -1); // 返回无效点(0,0)
    }
    
    // 过滤掉无效匹配点
    // 排除靠近(0,0)的匹配点，可能是误匹配
    QVector<QPoint> validMatches;
    for (const QPoint& match : matches) {
        // 过滤掉靠近(0,0)的匹配点
        if (match.x() >= 0 && match.y() >= 0 && 
            !(match.x() <= 10 && match.y() <= 10)) { // 排除靠近(0,0)的匹配点
            validMatches.append(match);
        }
    }
    
    // 返回第一个有效匹配点
    if (!validMatches.isEmpty()) {
        QPoint bestMatch = validMatches.first();
        emit logMessage(QString("[DEBUG] 最佳匹配点: (%1,%2)").arg(bestMatch.x()).arg(bestMatch.y()));
        return bestMatch;
    }
    
    // 没有找到有效匹配点
    emit logMessage("[DEBUG] 没有找到有效匹配点");
    return QPoint(-1, -1);
}

QImage ImageRecognizer::toGrayscale(const QImage &image) {
    // 转换为灰度图像
    return image.convertToFormat(QImage::Format_Grayscale8);
}

double ImageRecognizer::matchTemplateNCC(const QImage &source, const QImage &templateImg, int x, int y) {
    // 使用归一化互相关(NCC)算法匹配模板
    double sumSource = 0.0;
    double sumTemplate = 0.0;
    double sumProduct = 0.0;
    double sumSourceSq = 0.0;
    double sumTemplateSq = 0.0;

    // 预处理模板，计算灰度值和
    QVector<uchar> templateGrays(templateImg.width() * templateImg.height());
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            int index = ty * templateImg.width() + tx;
            templateGrays[index] = qGray(templateImg.pixel(tx, ty));
            sumTemplate += templateGrays[index];
            sumTemplateSq += templateGrays[index] * templateGrays[index];
        }
    }

    // 计算源图像区域与模板的相关值
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            // 检查边界，避免越界
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
    // 使用平方差(SSD)算法匹配模板
    double sumSquaredDiff = 0.0;
    
    // 预处理模板，计算灰度值
    QVector<uchar> templateGrays(templateImg.width() * templateImg.height());
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            int index = ty * templateImg.width() + tx;
            templateGrays[index] = qGray(templateImg.pixel(tx, ty));
        }
    }

    // 计算源图像区域与模板的平方差
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            // 检查边界，避免越界
            if (x + tx >= source.width() || y + ty >= source.height()) {
                return 1.0; // 越界返回最大差异
            }
            
            uchar s = qGray(source.pixel(x + tx, y + ty));
            uchar t = templateGrays[ty * templateImg.width() + tx];
            
            double diff = s - t;
            sumSquaredDiff += diff * diff;
        }
    }
    
    // 归一化到0-1范围，0表示完全匹配
    double maxDiff = templateImg.width() * templateImg.height() * 255.0 * 255.0;
    double normalizedDiff = sumSquaredDiff / maxDiff;
    
    // 返回1-normalizedDiff，使结果范围与其他算法一致（越大越匹配）
    return 1.0 - normalizedDiff;
}

double ImageRecognizer::matchTemplateSAD(const QImage &source, const QImage &templateImg, int x, int y) {
    // 使用绝对差值和(SAD)算法匹配模板
    double sumAbsoluteDiff = 0.0;
    
    // 预处理模板，计算灰度值
    QVector<uchar> templateGrays(templateImg.width() * templateImg.height());
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            int index = ty * templateImg.width() + tx;
            templateGrays[index] = qGray(templateImg.pixel(tx, ty));
        }
    }

    // 计算源图像区域与模板的绝对差值和
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            // 检查边界，避免越界
            if (x + tx >= source.width() || y + ty >= source.height()) {
                return 1.0; // 越界返回最大差异
            }
            
            uchar s = qGray(source.pixel(x + tx, y + ty));
            uchar t = templateGrays[ty * templateImg.width() + tx];
            
            sumAbsoluteDiff += qAbs(s - t);
        }
    }
    
    // 归一化到0-1范围，0表示完全匹配
    double maxDiff = templateImg.width() * templateImg.height() * 255.0;
    double normalizedDiff = sumAbsoluteDiff / maxDiff;
    
    // 返回1-normalizedDiff，使结果范围与其他算法一致（越大越匹配）
    return 1.0 - normalizedDiff;
}

QVector<QPoint> ImageRecognizer::matchTemplateORB(const QImage &source, const QImage &templateImg) {
    // 使用ORB算法匹配模板（简化实现）
    QVector<QPoint> matches;
    
    // 注意：这是一个简化的ORB匹配实现
    // 完整的ORB匹配需要更复杂的特征提取和匹配算法
    // 这里仅作为示例，实际使用OpenCV的ORB实现
    
    // 计算图像中心位置作为匹配点
    int centerX = source.width() / 2 - templateImg.width() / 2;
    int centerY = source.height() / 2 - templateImg.height() / 2;
    
    // 确保匹配点在图像范围内
    if (centerX >= 0 && centerY >= 0) {
        matches.append(QPoint(centerX, centerY));
    }
    
    return matches;
}

void ImageRecognizer::loadTemplates() {
    // 加载所有模板图像
    // 从配置管理器获取模板路径并加载
    ConfigManager* config = ConfigManager::getInstance();
    
    // 加载工作台图标
    QString workbenchPath = config->getIconPath("workbench");
    if (!workbenchPath.isEmpty()) {
        QImage workbenchImage(workbenchPath);
        if (!workbenchImage.isNull()) {
            templates["workbench"] = workbenchImage;
            emit logMessage(QString("已加载模板: workbench 尺寸: %1x%2").arg(workbenchImage.width()).arg(workbenchImage.height()));
        } else {
            emit logMessage("无法加载模板: " + workbenchPath);
        }
    }
    
    // 加载MindSpark图标
    QString mindsparkPath = config->getIconPath("mindspark");
    if (!mindsparkPath.isEmpty()) {
        QImage mindsparkImage(mindsparkPath);
        if (!mindsparkImage.isNull()) {
            templates["mindspark"] = mindsparkImage;
            emit logMessage(QString("已加载模板: mindspark 尺寸: %1x%2").arg(mindsparkImage.width()).arg(mindsparkImage.height()));
        } else {
            emit logMessage("无法加载模板: " + mindsparkPath);
        }
    }
    
    // 加载MindSpark小图标
    QString mindsparkSmallPath = config->getIconPath("mindspark_small");
    if (!mindsparkSmallPath.isEmpty()) {
        QImage mindsparkSmallImage(mindsparkSmallPath);
        if (!mindsparkSmallImage.isNull()) {
            templates["mindspark_small"] = mindsparkSmallImage;
            emit logMessage(QString("已加载模板: mindspark_small 尺寸: %1x%2").arg(mindsparkSmallImage.width()).arg(mindsparkSmallImage.height()));
        } else {
            emit logMessage("无法加载模板: " + mindsparkSmallPath);
        }
    }
    
    // 加载输入框图标
    QString inputBoxPath = config->getIconPath("input_box");
    if (!inputBoxPath.isEmpty()) {
        QImage inputBoxImage(inputBoxPath);
        if (!inputBoxImage.isNull()) {
            templates["input_box"] = inputBoxImage;
            emit logMessage(QString("已加载模板: input_box 尺寸: %1x%2").arg(inputBoxImage.width()).arg(inputBoxImage.height()));
        } else {
            emit logMessage("无法加载模板: " + inputBoxPath);
        }
    }
    
    // 加载发送按钮图标
    QString sendButtonPath = config->getIconPath("send_button");
    if (!sendButtonPath.isEmpty()) {
        QImage sendButtonImage(sendButtonPath);
        if (!sendButtonImage.isNull()) {
            templates["send_button"] = sendButtonImage;
            emit logMessage(QString("已加载模板: send_button 尺寸: %1x%2").arg(sendButtonImage.width()).arg(sendButtonImage.height()));
        } else {
            emit logMessage("无法加载模板: " + sendButtonPath);
        }
    }
    
    // 加载历史对话图标
    QString historyDialogPath = config->getIconPath("history_dialog");
    if (!historyDialogPath.isEmpty()) {
        QImage historyDialogImage(historyDialogPath);
        if (!historyDialogImage.isNull()) {
            templates["history_dialog"] = historyDialogImage;
            emit logMessage(QString("已加载模板: history_dialog 尺寸: %1x%2").arg(historyDialogImage.width()).arg(historyDialogImage.height()));
        } else {
            emit logMessage("无法加载模板: " + historyDialogPath);
        }
    }
    
    emit logMessage(QString("模板加载完成，共加载 %1 个模板").arg(templates.size()));

}

void ImageRecognizer::onConfigChanged() {
    // 配置变更处理
    threshold = ConfigManager::getInstance()->getImageRecognitionThreshold();
    maxAttempts = ConfigManager::getInstance()->getMaxRecognitionAttempts();
    
    // 重新加载模板
    templates.clear();
    loadTemplates();
}

QImage ImageRecognizer::captureWindow(HWND hwnd) {
    // 使用Windows API捕获指定窗口
    if (!hwnd) {
        emit logMessage("无效的窗口句柄");
        return QImage();
    }
    
    // 获取窗口矩形
    RECT windowRect;
    if (!GetWindowRect(hwnd, &windowRect)) {
        emit logMessage("获取窗口矩形失败");
        return QImage();
    }
    
    // 计算窗口尺寸
    int width = windowRect.right - windowRect.left;
    int height = windowRect.bottom - windowRect.top;
    
    // 调用captureScreenArea捕获窗口区域
    return captureScreenArea(QRect(windowRect.left, windowRect.top, width, height));
}

bool ImageRecognizer::findTemplateInWindow(HWND hwnd, const QString &templateName, QPoint &resultPos) {
    // 在指定窗口中查找模板
    QImage windowImage = captureWindow(hwnd);
    if (windowImage.isNull()) {
        emit logMessage("窗口截图失败");
        return false;
    }
    
    // 调用findTemplate查找模板，返回所有匹配点
    QVector<QPoint> matches = findTemplate(windowImage, templateName);
    if (matches.isEmpty()) {
        return false;
    }
    
    // 查找最佳匹配点
    QPoint bestMatch = findBestMatch(matches, windowImage);
    
    // 检查最佳匹配点是否有效
    if (bestMatch.x() < 0 || bestMatch.y() < 0) {
        emit logMessage(QString("[DEBUG] 未找到有效匹配点: %1").arg(templateName));
        return false;
    }
    
    resultPos = bestMatch;
    
    // 发送识别区域信号
    if (templates.contains(templateName)) {
        // 获取模板尺寸配置
        ConfigManager* config = ConfigManager::getInstance();
        QSize templateSize = config->getTemplateSize(templateName);
        
        // 如果模板尺寸未配置（使用默认值100x100），则使用实际尺寸
        QImage templateImage = templates[templateName];
        if (templateSize.width() == 100 && templateSize.height() == 100) {
            templateSize = templateImage.size();
        }
        
        // 计算识别区域
        QRect recognitionArea(bestMatch.x(), bestMatch.y(),
                     templateSize.width(), templateSize.height());
        
        // 调整识别区域，确保在图像范围内
        if (recognitionArea.x() < 0) recognitionArea.moveLeft(0);
        if (recognitionArea.y() < 0) recognitionArea.moveTop(0);
        if (recognitionArea.right() > windowImage.width()) {
            recognitionArea.setWidth(windowImage.width() - recognitionArea.x());
        }
        if (recognitionArea.bottom() > windowImage.height()) {
            recognitionArea.setHeight(windowImage.height() - recognitionArea.y());
        }
        
        emit recognitionAreaFound(recognitionArea, templateName);
        
        // 删除了保存识别区域截图功能
    }
    
    return true;
}

void ImageRecognizer::stopRecognition() {
    // 停止识别
    m_stopRequested = true;
    emit logMessage("已请求停止识别");
}

void ImageRecognizer::resetStopFlag() {
    // 重置停止标志
    m_stopRequested = false;
    emit logMessage("停止标志已重置");
}

QPoint ImageRecognizer::getCurrentMousePosition() {
    // 获取当前鼠标位置
    POINT point;
    GetCursorPos(&point);
    return QPoint(point.x, point.y);
}

bool ImageRecognizer::saveImageForDebug(const QImage &image, const QString &prefix, const QString &subfolder) {
    // 保存图像用于调试
    QString projectPath = QCoreApplication::applicationDirPath();
    QString debugDir = projectPath + "/debug_tu/" + subfolder + "/";
    
    QDir dir;
    bool dirCreated = dir.mkpath(debugDir);
    emit logMessage(QString("创建目录: %1 结果: %2").arg(debugDir).arg(dirCreated ? "成功" : "失败"));
    
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
    QString filename = debugDir + prefix + "_" + timestamp + ".png";
    
    if (image.isNull()) {
        emit logMessage(QString("保存失败: %1 图像为空").arg(filename));
        return false;
    }
    
    if (image.save(filename)) {
        emit logMessage(QString("已保存调试图像: %1").arg(filename));
        return true;
    } else {
        emit logMessage(QString("保存失败: %1").arg(filename));
        return false;
    }
}

// 默认保存到full_screenshots子目录
bool ImageRecognizer::saveImageForDebug(const QImage &image, const QString &prefix) {
    return saveImageForDebug(image, prefix, "full_screenshots");
}


    // 保存图像到日志目录
bool ImageRecognizer::saveImageToLogDir(const QImage &image, const QString &prefix, const QString &subfolder) {
    // 保存图像到日志目录
    ConfigManager* config = ConfigManager::getInstance();
    QString logDir = config->getLogPath() + subfolder + "/";
    
    QDir dir;
    bool dirCreated = dir.mkpath(logDir);
    emit logMessage(QString("创建目录: %1 结果: %2").arg(logDir).arg(dirCreated ? "成功" : "失败"));
    
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
    QString filename = logDir + prefix + "_" + timestamp + ".png";
    
    if (image.isNull()) {
        emit logMessage(QString("保存失败: %1 图像为空").arg(filename));
        return false;
    }
    
    if (image.save(filename)) {
        emit logMessage(QString("已保存图像: %1").arg(filename));
        return true;
    } else {
        emit logMessage(QString("保存失败: %1").arg(filename));
        return false;
    }
}

// 默认保存到full_screenshots子目录
bool ImageRecognizer::saveImageToLogDir(const QImage &image, const QString &prefix) {
    return saveImageToLogDir(image, prefix, "full_screenshots");
}

bool ImageRecognizer::isStopRequested() const {
    return m_stopRequested;
}

void ImageRecognizer::setStopRequested(bool stop) {
    m_stopRequested = stop;
}

void ImageRecognizer::resetState() {
    // 重置所有状态
    m_inputBoxPositions.clear();
    m_inputBoxFound.clear();
    m_clientWidths.clear();
    m_failedAttempts.clear();
    m_previousAnswerAreas.clear();
    m_stableFrameCounts.clear();
    m_hasDetectedChanges.clear();
    emit logMessage("状态已重置");
}

void ImageRecognizer::doFindTemplateInWindow(HWND hwnd, const QString &templateName) {
    // 在窗口中查找模板
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
        emit logMessage(QString("已加载模板: %1 路径: %2 尺寸: %3x%4")
                       .arg(name)
                       .arg(path)
                       .arg(templateImage.width()).arg(templateImage.height()));
        
        // 更新模板尺寸配置
        ConfigManager* config = ConfigManager::getInstance();
        config->setTemplateSize(name, templateImage.size());
        emit logMessage(QString("模板尺寸配置已更新: %1 -> %2x%3")
                       .arg(name)
                       .arg(templateImage.width()).arg(templateImage.height()));
        
        return true;
    } else {
        emit logMessage("无法加载模板: " + path);
        return false;
    }
}

void ImageRecognizer::checkAnswerReceivedAsync(HWND hwnd) {
    // 异步检查回答是否收到
    bool received = checkAnswerReceived(hwnd);
    emit answerReceivedChecked(hwnd, received);
}
