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

// OpenCV???
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>

// ??OpenCV????
using namespace cv;
using namespace std;

// MatchResult???????????

// QImage?Mat???????????
cv::Mat ImageRecognizer::QImageToMat(const QImage &image) {
    Mat mat;
    
    if (image.format() == QImage::Format_Grayscale8) {
        // ??????
        mat = Mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine());
    } else if (image.format() == QImage::Format_ARGB32 || image.format() == QImage::Format_RGB32) {
        // ??????
        mat = Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        cvtColor(mat, mat, COLOR_BGRA2GRAY);
    } else {
        // ??????????
        QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);
        mat = Mat(grayImage.height(), grayImage.width(), CV_8UC1, const_cast<uchar*>(grayImage.bits()), grayImage.bytesPerLine());
    }
    
    return mat.clone(); // ??????????????
}


ImageRecognizer::ImageRecognizer(QObject *parent) : QObject(parent) {
    // ???????
    threshold = ConfigManager::getInstance()->getImageRecognitionThreshold();
    maxAttempts = ConfigManager::getInstance()->getMaxRecognitionAttempts();
    m_stopRequested = false;

    // ??debug_tu??
    QString projectPath = QCoreApplication::applicationDirPath();
    QString debugDir = projectPath + "/debug_tu/";
    QDir dir(debugDir);
    if (dir.exists()) {
        // ?????????
        QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        for (const QString &file : files) {
            bool removed = dir.remove(file);
            emit logMessage(QString("??????: %1???: %2").arg(file).arg(removed ? "??" : "??"));
        }
        emit logMessage("???debug_tu????????");
    } else {
        // ???????????
        bool dirCreated = dir.mkpath(debugDir);
        emit logMessage(QString("??????: %1???: %2").arg(debugDir).arg(dirCreated ? "??" : "??"));
    }

    // ??????
    connect(ConfigManager::getInstance(), &ConfigManager::configChanged,
            this, &ImageRecognizer::onConfigChanged);
    
    // ???????
    workerThread = new QThread(this);
    // ????????????
    this->moveToThread(workerThread);
    // ??????
    workerThread->start();
}

ImageRecognizer::~ImageRecognizer() {
    // ??????
    workerThread->quit();
    workerThread->wait();
    delete workerThread;
}

QRect ImageRecognizer::findImageOnScreen(const QString &templatePath, int screenIndex) {
    QImage screenImage = captureScreen(screenIndex);
    if (screenImage.isNull()) {
        emit logMessage("??????");
        return QRect();
    }

    // ??????
    QImage templateImage(templatePath);
    if (templateImage.isNull()) {
        emit logMessage("??????: " + templatePath);
        return QRect();
    }

    // ??????
    screenImage = toGrayscale(screenImage);
    templateImage = toGrayscale(templateImage);

    // ??????
    if (templateImage.width() > screenImage.width() ||
        templateImage.height() > screenImage.height()) {
        emit logMessage("????????");
        return QRect();
    }

    // ???? - ?????????????
    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        // ????????????findTemplate????
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
                
                // ????????????????
                emit recognitionAreaFound(recognitionArea, templatePath);
                
                // 删除了保存识别区域截图功能
            }
            
            // ????????
        QPoint bestMatch = findBestMatch(matches, screenImage);
        
        // ??????????
        if (bestMatch.x() < 0 || bestMatch.y() < 0) {
            emit logMessage("[DEBUG] ??????????????????");
            return QRect();
        }
        
        QRect bestRecognitionArea(bestMatch.x(), bestMatch.y(),
                     templateImage.width(), templateImage.height());
        
        return bestRecognitionArea;
        }

        emit logMessage(QString("???????????%1").arg(attempt + 1));
        QThread::sleep(1); // ?????????
        screenImage = captureScreen(screenIndex); // ??????
        if (screenImage.isNull()) {
            emit logMessage("??????");
            return QRect();
        }

        // ??????
        screenImage = toGrayscale(screenImage);
    }

    return QRect();
}

bool ImageRecognizer::checkAnswerReceived(HWND hwnd) {
    // ????????
    // ???????????????????????

    if (!hwnd) {
        emit logMessage("???????");
        return false;
    }
    
    // 等待2秒，确保页面已经成功加载
    emit logMessage("??????2???????");
    QThread::sleep(2);
    
    const int maxFailedAttempts = 5; // ????5?????
    
    // ??????????
    if (!m_inputBoxFound.contains(hwnd)) {
        m_inputBoxFound[hwnd] = false;
        m_failedAttempts[hwnd] = 0;
        m_stableFrameCounts[hwnd] = 0;
        m_hasDetectedChanges[hwnd] = false;
    }
    
    // ?????????????
    RECT clientRect;
    if (GetClientRect(hwnd, &clientRect)) {
        int currentWidth = clientRect.right - clientRect.left;
        // ?????????????????
        if (currentWidth != m_clientWidths[hwnd]) {
            m_clientWidths[hwnd] = currentWidth;
            m_inputBoxFound[hwnd] = false; // ?????????????????
            emit logMessage(QString("?????????: %1??????????").arg(currentWidth));
        }
    }
    
    // ???????????
    // 1. ?????
    // 2. ??????
    // 3. ?????
    // 4. ???????
    if (!m_inputBoxFound[hwnd] || m_failedAttempts[hwnd] >= maxFailedAttempts) {
        // ??????
        m_failedAttempts[hwnd] = 0;
        
        // 1. ???????
        // ?????????????
        QStringList inputBoxTemplates = {"input_box", "input_box_small", "input_box_large"};
        
        m_inputBoxFound[hwnd] = false;
        QPoint inputBoxPos;
        for (const QString& templateName : inputBoxTemplates) {
            if (findTemplateInWindow(hwnd, templateName, inputBoxPos)) {
                m_inputBoxFound[hwnd] = true;
                m_inputBoxPositions[hwnd] = inputBoxPos;
                emit logMessage(QString("???????: (%1, %2)").arg(inputBoxPos.x()).arg(inputBoxPos.y()));
                
                // ?????????
                if (GetClientRect(hwnd, &clientRect)) {
                    m_clientWidths[hwnd] = clientRect.right - clientRect.left;
                } else {
                    m_clientWidths[hwnd] = 0;
                    emit logMessage("???????????");
                }
                break;
            }
        }
        
        if (!m_inputBoxFound[hwnd]) {
            emit logMessage("??????");
            m_failedAttempts[hwnd]++;
            return false;
        }
    }
    
    // 2. ?????????????
    int answerAreaX = 0;
    int answerAreaY = 0;
    int answerAreaWidth = m_clientWidths[hwnd];
    int answerAreaHeight = m_inputBoxPositions[hwnd].y() - 20; // ????
    
    // ??????????
    if (answerAreaHeight <= 0 || m_clientWidths[hwnd] <= 0) {
        emit logMessage("?????????");
        m_failedAttempts[hwnd]++;
        return false;
    }
    
    // 3. ??????
    QImage windowImage = captureWindow(hwnd);
    if (windowImage.isNull()) {
        emit logMessage("??????");
        m_failedAttempts[hwnd]++;
        return false;
    }
    
    // ????????????
    if (windowImage.width() < answerAreaWidth || windowImage.height() < m_inputBoxPositions[hwnd].y()) {
        emit logMessage("?????????????????");
        m_failedAttempts[hwnd]++;
        return false;
    }
    
    QImage answerArea = windowImage.copy(answerAreaX, answerAreaY, answerAreaWidth, answerAreaHeight);
    
    // 4. ???????
    
    // ????
    if (!m_previousAnswerAreas.contains(hwnd) || m_previousAnswerAreas[hwnd].isNull()) {
        m_previousAnswerAreas[hwnd] = answerArea;
        m_stableFrameCounts[hwnd] = 0;
        m_hasDetectedChanges[hwnd] = false;
        emit logMessage("??????????");
        return false;
    }

    // ????
    if (m_previousAnswerAreas[hwnd].size() != answerArea.size()) {
        emit logMessage(QString("????????: %1x%2 -> %3x%4")
                        .arg(m_previousAnswerAreas[hwnd].width()).arg(m_previousAnswerAreas[hwnd].height())
                        .arg(answerArea.width()).arg(answerArea.height()));
        m_previousAnswerAreas[hwnd] = answerArea;
        m_stableFrameCounts[hwnd] = 0;
        m_hasDetectedChanges[hwnd] = true;
        return false;
    }

    // ???????
    int diffCount = 0;
    int totalPixels = answerArea.width() * answerArea.height();
    double diffThreshold = 0.01; // ???1%??????????????
    int pixelThreshold = static_cast<int>(totalPixels * diffThreshold);

    // ????????????????????????
    int sampleStep = 4; // ?????????????????
    for (int y = 0; y < answerArea.height(); y += sampleStep) {
        for (int x = 0; x < answerArea.width(); x += sampleStep) {
            QRgb prevPixel = m_previousAnswerAreas[hwnd].pixel(x, y);
            QRgb currPixel = answerArea.pixel(x, y);

            // ??RGB?????????????
            int rDiff = qAbs(qRed(prevPixel) - qRed(currPixel));
            int gDiff = qAbs(qGreen(prevPixel) - qGreen(currPixel));
            int bDiff = qAbs(qBlue(prevPixel) - qBlue(currPixel));
            
            // ???????????????
            if (rDiff > 10 || gDiff > 10 || bDiff > 10) { // ??RGB??????????
                diffCount++;
                if (diffCount > pixelThreshold) {
                    emit logMessage(QString("?????????: %1 ?? (??: %2)").arg(diffCount * sampleStep * sampleStep).arg(pixelThreshold * sampleStep * sampleStep));
                    m_previousAnswerAreas[hwnd] = answerArea;
                    m_stableFrameCounts[hwnd] = 0;
                    m_hasDetectedChanges[hwnd] = true;
                    return false; // ????????????
                }
            }
        }
    }

    // ??????????????????????????????
    if (m_hasDetectedChanges[hwnd] && diffCount <= pixelThreshold) {
        m_stableFrameCounts[hwnd]++;
        emit logMessage(QString("??????: %1 ??????: %2???: %3").arg(m_stableFrameCounts[hwnd]).arg(diffCount * sampleStep * sampleStep).arg(pixelThreshold * sampleStep * sampleStep));
        
        // ??2?????????????????
        if (m_stableFrameCounts[hwnd] >= 2) {
            emit logMessage("????????");
            m_previousAnswerAreas[hwnd] = answerArea;
            m_stableFrameCounts[hwnd] = 0;
            m_hasDetectedChanges[hwnd] = false;
            return true; // ????
        }
    } else {
        emit logMessage(QString("????????????????????: %1???: %2").arg(diffCount * sampleStep * sampleStep).arg(pixelThreshold * sampleStep * sampleStep));
        m_stableFrameCounts[hwnd] = 0; // ???????
    }

    m_previousAnswerAreas[hwnd] = answerArea;
    return false; // ?????
}

QImage ImageRecognizer::captureScreen(int screenIndex) {
    // ??Windows API???????????????
    QList<QScreen*> screens = QGuiApplication::screens();
    if (screenIndex < 0 || screenIndex >= screens.size()) {
        emit logMessage(QString("???????: %1??????").arg(screenIndex));
        screenIndex = 0;
    }
    
    QScreen* screen = screens[screenIndex];
    QRect screenGeometry = screen->geometry();
    
    // ?????????
    return captureScreenArea(screenGeometry, screenIndex);
}

QImage ImageRecognizer::captureScreenArea(const QRect &area, int screenIndex) {
    // ??Windows API?????????????
    if (area.isEmpty()) {
        emit logMessage("???????");
        return QImage();
    }

    // ????????
    QList<QScreen*> screens = QGuiApplication::screens();
    
    // ????????
    QScreen* targetScreen = nullptr;
    if (screenIndex >= 0 && screenIndex < screens.size()) {
        targetScreen = screens[screenIndex];
    } else {
        // ???????????
        for (QScreen* screen : screens) {
            if (screen->geometry().intersects(area)) {
                targetScreen = screen;
                break;
            }
        }
        
        if (!targetScreen) {
            targetScreen = QGuiApplication::primaryScreen();
            emit logMessage("??????????????");
        }
    }
    
    // ????DC
    HDC hScreenDC = GetDC(NULL);
    if (!hScreenDC) {
        emit logMessage("??????DC");
        return QImage();
    }

    // ????DC
    HDC hMemDC = CreateCompatibleDC(hScreenDC);
    if (!hMemDC) {
        emit logMessage("??????DC");
        ReleaseDC(NULL, hScreenDC);
        return QImage();
    }

    // ????
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, area.width(), area.height());
    if (!hBitmap) {
        emit logMessage("??????");
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hScreenDC);
        return QImage();
    }

    // ???????DC
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

    // ?????????DC
    BOOL result = BitBlt(hMemDC, 0, 0, area.width(), area.height(), 
                        hScreenDC, area.x(), area.y(), SRCCOPY);
    if (!result) {
        emit logMessage("????????");
        SelectObject(hMemDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(NULL, hScreenDC);
        return QImage();
    }

    // ??????
    BITMAPINFO bmpInfo;
    memset(&bmpInfo, 0, sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = area.width();
    bmpInfo.bmiHeader.biHeight = -area.height(); // ?????????
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    // ??QImage
    QImage image(area.width(), area.height(), QImage::Format_ARGB32);
    GetDIBits(hMemDC, hBitmap, 0, area.height(), image.bits(), &bmpInfo, DIB_RGB_COLORS);

    // ????
    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(NULL, hScreenDC);

    emit logMessage(QString("????????: %1x%2?????: %3").arg(area.width()).arg(area.height()).arg(screenIndex));
    return image;
}

void ImageRecognizer::setRecognitionThreshold(double threshold) {
    this->threshold = threshold;
}

QVector<QPoint> ImageRecognizer::findTemplate(const QImage &sourceImage, const QString &templateName) {
    QVector<QPoint> matches;
    
    // ????????
    if (m_stopRequested) {
        emit logMessage("[INFO] ?????????findTemplate");
        return matches;
    }
    
    // ????????
    if (!templates.contains(templateName)) {
        emit logMessage("?????: " + templateName);
        return matches;
    }

    QImage templateImage = templates[templateName];
    QImage sourceGray = toGrayscale(sourceImage);
    QImage templateGray = toGrayscale(templateImage);
    
    // ??????????
    if (templateGray.width() > sourceGray.width() || templateGray.height() > sourceGray.height()) {
        emit logMessage(QString("???? %1x%2 ????? %3x%4").arg(templateGray.width()).arg(templateGray.height()).arg(sourceGray.width()).arg(sourceGray.height()));
        return matches;
    }
    
    // ???OpenCV Mat
    Mat sourceMat = QImageToMat(sourceGray);
    Mat templateMat = QImageToMat(templateGray);
    
    // ??????????
    int matchMethod = TM_CCOEFF_NORMED;
    Mat result;
    matchTemplate(sourceMat, templateMat, result, matchMethod);
    
    // ?????????????????????
    ConfigManager* config = ConfigManager::getInstance();
    QSize configTemplateSize = config->getTemplateSize(templateName);
    int templateWidth = configTemplateSize.width();
    int templateHeight = configTemplateSize.height();
    
    // ?????????????????????????
    if (templateWidth == 100 && templateHeight == 100) {
        templateWidth = templateGray.width();
        templateHeight = templateGray.height();
        config->setTemplateSize(templateName, QSize(templateWidth, templateHeight));
    }
    
    // ???????????????????
    double adjustedThreshold = 0.95; // ?????
    if (templateName.contains("workbench")) {
        adjustedThreshold = 0.97;
    } else if (templateName.contains("mindspark")) {
        adjustedThreshold = 0.96;
    } else if (templateName.contains("input_box")) {
        adjustedThreshold = 0.94;
    } else if (templateName.contains("send_button")) {
        adjustedThreshold = 0.96;
    }
    
    // ???????????????
    for (int y = 0; y < result.rows; y++) {
        for (int x = 0; x < result.cols; x++) {
            float matchVal = result.at<float>(y, x);
            double score = matchVal; // TM_CCOEFF_NORMED????[-1, 1]??????
            
            if (score >= adjustedThreshold) {
                QPoint matchPoint(x, y);
                
                // ???????????????????
                bool isUnique = true;
                int suppressionRadius = qMax(templateWidth, templateHeight) / 2;
                suppressionRadius = qMin(suppressionRadius, 50); // ????????
                
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
    
    // ??????????????
    if (matches.size() > 2) {
        matches.resize(2);
    }
    
    emit logMessage(QString("findTemplate?????: %1??? %2 ????").arg(templateName).arg(matches.size()));
    
    // ?????????????
    if (matches.isEmpty()) {
        emit templateNotFound(templateName);
    }
    
    return matches;
}

QPoint ImageRecognizer::findBestMatch(const QVector<QPoint> &matches, const QImage &sourceImage) {
    // ???????
    Q_UNUSED(sourceImage); // ?????????
    if (matches.isEmpty()) {
        emit logMessage("[DEBUG] ????????????");
        return QPoint(-1, -1); // ??????????(0,0)
    }
    
    // ???????????????
    // ?????(0,0)??????
    QVector<QPoint> validMatches;
    for (const QPoint& match : matches) {
        // ??????????
        if (match.x() >= 0 && match.y() >= 0 && 
            !(match.x() <= 10 && match.y() <= 10)) { // ??(0,0)???????
            validMatches.append(match);
        }
    }
    
    // ??????????????
    if (!validMatches.isEmpty()) {
        QPoint bestMatch = validMatches.first();
        emit logMessage(QString("[DEBUG] ???????: (%1,%2)").arg(bestMatch.x()).arg(bestMatch.y()));
        return bestMatch;
    }
    
    // ????????????????
    emit logMessage("[DEBUG] ???????????????");
    return QPoint(-1, -1);
}

QImage ImageRecognizer::toGrayscale(const QImage &image) {
    // ??????
    return image.convertToFormat(QImage::Format_Grayscale8);
}

double ImageRecognizer::matchTemplateNCC(const QImage &source, const QImage &templateImg, int x, int y) {
    // ??????????NCC?
    double sumSource = 0.0;
    double sumTemplate = 0.0;
    double sumProduct = 0.0;
    double sumSourceSq = 0.0;
    double sumTemplateSq = 0.0;

    // ??????????????????qGray
    QVector<uchar> templateGrays(templateImg.width() * templateImg.height());
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            int index = ty * templateImg.width() + tx;
            templateGrays[index] = qGray(templateImg.pixel(tx, ty));
            sumTemplate += templateGrays[index];
            sumTemplateSq += templateGrays[index] * templateGrays[index];
        }
    }

    // ?????????????????
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            // ???????????????
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
    // ??????SSD?
    double sumSquaredDiff = 0.0;
    
    // ??????????????????qGray
    QVector<uchar> templateGrays(templateImg.width() * templateImg.height());
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            int index = ty * templateImg.width() + tx;
            templateGrays[index] = qGray(templateImg.pixel(tx, ty));
        }
    }

    // ?????
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            // ???????????????
            if (x + tx >= source.width() || y + ty >= source.height()) {
                return 1.0; // ??????????
            }
            
            uchar s = qGray(source.pixel(x + tx, y + ty));
            uchar t = templateGrays[ty * templateImg.width() + tx];
            
            double diff = s - t;
            sumSquaredDiff += diff * diff;
        }
    }
    
    // ????0-1???0??????
    double maxDiff = templateImg.width() * templateImg.height() * 255.0 * 255.0;
    double normalizedDiff = sumSquaredDiff / maxDiff;
    
    // ??1-normalizedDiff????????????
    return 1.0 - normalizedDiff;
}

double ImageRecognizer::matchTemplateSAD(const QImage &source, const QImage &templateImg, int x, int y) {
    // ??????SAD?
    double sumAbsoluteDiff = 0.0;
    
    // ??????????????????qGray
    QVector<uchar> templateGrays(templateImg.width() * templateImg.height());
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            int index = ty * templateImg.width() + tx;
            templateGrays[index] = qGray(templateImg.pixel(tx, ty));
        }
    }

    // ?????
    for (int ty = 0; ty < templateImg.height(); ++ty) {
        for (int tx = 0; tx < templateImg.width(); ++tx) {
            // ???????????????
            if (x + tx >= source.width() || y + ty >= source.height()) {
                return 1.0; // ??????????
            }
            
            uchar s = qGray(source.pixel(x + tx, y + ty));
            uchar t = templateGrays[ty * templateImg.width() + tx];
            
            sumAbsoluteDiff += qAbs(s - t);
        }
    }
    
    // ????0-1???0??????
    double maxDiff = templateImg.width() * templateImg.height() * 255.0;
    double normalizedDiff = sumAbsoluteDiff / maxDiff;
    
    // ??1-normalizedDiff????????????
    return 1.0 - normalizedDiff;
}

QVector<QPoint> ImageRecognizer::matchTemplateORB(const QImage &source, const QImage &templateImg) {
    // ORB????
    QVector<QPoint> matches;
    
    // ????????????????
    // ?????ORB??????????????????
    // ?????????OpenCV???????ORB??
    
    // ??????????????
    int centerX = source.width() / 2 - templateImg.width() / 2;
    int centerY = source.height() / 2 - templateImg.height() / 2;
    
    // ??????
    if (centerX >= 0 && centerY >= 0) {
        matches.append(QPoint(centerX, centerY));
    }
    
    return matches;
}

void ImageRecognizer::loadTemplates() {
    // ????????
    // ????????????
    ConfigManager* config = ConfigManager::getInstance();
    
    // ???????
    QString workbenchPath = config->getIconPath("workbench");
    if (!workbenchPath.isEmpty()) {
        QImage workbenchImage(workbenchPath);
        if (!workbenchImage.isNull()) {
            templates["workbench"] = workbenchImage;
            emit logMessage(QString("??????: workbench???: %2x%3").arg(workbenchImage.width()).arg(workbenchImage.height()));
        } else {
            emit logMessage("?????????: " + workbenchPath);
        }
    }
    
    // ???????
    QString mindsparkPath = config->getIconPath("mindspark");
    if (!mindsparkPath.isEmpty()) {
        QImage mindsparkImage(mindsparkPath);
        if (!mindsparkImage.isNull()) {
            templates["mindspark"] = mindsparkImage;
            emit logMessage(QString("??????: mindspark???: %2x%3").arg(mindsparkImage.width()).arg(mindsparkImage.height()));
        } else {
            emit logMessage("?????????: " + mindsparkPath);
        }
    }
    
    // ???????
    QString inputBoxPath = config->getIconPath("input_box");
    if (!inputBoxPath.isEmpty()) {
        QImage inputBoxImage(inputBoxPath);
        if (!inputBoxImage.isNull()) {
            templates["input_box"] = inputBoxImage;
            emit logMessage(QString("??????: input_box???: %2x%3").arg(inputBoxImage.width()).arg(inputBoxImage.height()));
        } else {
            emit logMessage("?????????: " + inputBoxPath);
        }
    }
    
    // ????????
    QString sendButtonPath = config->getIconPath("send_button");
    if (!sendButtonPath.isEmpty()) {
        QImage sendButtonImage(sendButtonPath);
        if (!sendButtonImage.isNull()) {
            templates["send_button"] = sendButtonImage;
            emit logMessage(QString("??????: send_button???: %2x%3").arg(sendButtonImage.width()).arg(sendButtonImage.height()));
        } else {
            emit logMessage("??????????: " + sendButtonPath);
        }
    }
    
    // ????????
    QString historyDialogPath = config->getIconPath("history_dialog");
    if (!historyDialogPath.isEmpty()) {
        QImage historyDialogImage(historyDialogPath);
        if (!historyDialogImage.isNull()) {
            templates["history_dialog"] = historyDialogImage;
            emit logMessage(QString("??????: history_dialog???: %1x%2").arg(historyDialogImage.width()).arg(historyDialogImage.height()));
        } else {
            emit logMessage("????????????: " + historyDialogPath);
        }
    }
    
    emit logMessage(QString("?????????????: %1").arg(templates.size()));

}

void ImageRecognizer::onConfigChanged() {
    // ??????
    threshold = ConfigManager::getInstance()->getImageRecognitionThreshold();
    maxAttempts = ConfigManager::getInstance()->getMaxRecognitionAttempts();
    
    // ??????
    templates.clear();
    loadTemplates();
}

QImage ImageRecognizer::captureWindow(HWND hwnd) {
    // ??Windows API??????
    if (!hwnd) {
        emit logMessage("???????");
        return QImage();
    }
    
    // ??????
    RECT windowRect;
    if (!GetWindowRect(hwnd, &windowRect)) {
        emit logMessage("????????");
        return QImage();
    }
    
    // ??????
    int width = windowRect.right - windowRect.left;
    int height = windowRect.bottom - windowRect.top;
    
    // ??????
    return captureScreenArea(QRect(windowRect.left, windowRect.top, width, height));
}

bool ImageRecognizer::findTemplateInWindow(HWND hwnd, const QString &templateName, QPoint &resultPos) {
    // ??????????
    QImage windowImage = captureWindow(hwnd);
    if (windowImage.isNull()) {
        emit logMessage("????????");
        return false;
    }
    
    // ???? - ?????????????
    QVector<QPoint> matches = findTemplate(windowImage, templateName);
    if (matches.isEmpty()) {
        return false;
    }
    
    // ???????
    QPoint bestMatch = findBestMatch(matches, windowImage);
    
    // ??????????
    if (bestMatch.x() < 0 || bestMatch.y() < 0) {
        emit logMessage(QString("[DEBUG] ???????????????: %1").arg(templateName));
        return false;
    }
    
    resultPos = bestMatch;
    
    // ???????????
    if (templates.contains(templateName)) {
        // ????????????????????
        ConfigManager* config = ConfigManager::getInstance();
        QSize templateSize = config->getTemplateSize(templateName);
        
        // ?????????????????????????
        QImage templateImage = templates[templateName];
        if (templateSize.width() == 100 && templateSize.height() == 100) {
            templateSize = templateImage.size();
        }
        
        // ????????????????
        QRect recognitionArea(bestMatch.x(), bestMatch.y(),
                     templateSize.width(), templateSize.height());
        
        // ????????????????
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
    // ????
    m_stopRequested = true;
    emit logMessage("????????");
}

void ImageRecognizer::resetStopFlag() {
    // ??????
    m_stopRequested = false;
    emit logMessage("??????");
}

QPoint ImageRecognizer::getCurrentMousePosition() {
    // ????????
    POINT point;
    GetCursorPos(&point);
    return QPoint(point.x, point.y);
}

bool ImageRecognizer::saveImageForDebug(const QImage &image, const QString &prefix, const QString &subfolder) {
    // ???????????????
    QString projectPath = QCoreApplication::applicationDirPath();
    QString debugDir = projectPath + "/debug_tu/" + subfolder + "/";
    
    QDir dir;
    bool dirCreated = dir.mkpath(debugDir);
    emit logMessage(QString("??????: %1???: %2").arg(debugDir).arg(dirCreated ? "??" : "??"));
    
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
    QString filename = debugDir + prefix + "_" + timestamp + ".png";
    
    if (image.isNull()) {
        emit logMessage(QString("????????: %1?????").arg(filename));
        return false;
    }
    
    if (image.save(filename)) {
        emit logMessage(QString("???????: %1").arg(filename));
        return true;
    } else {
        emit logMessage(QString("????????: %1").arg(filename));
        return false;
    }
}

// ??????????full_screenshots???
bool ImageRecognizer::saveImageForDebug(const QImage &image, const QString &prefix) {
    return saveImageForDebug(image, prefix, "full_screenshots");
}


    // ?????????????????
bool ImageRecognizer::saveImageToLogDir(const QImage &image, const QString &prefix, const QString &subfolder) {
    // ????????????????
    ConfigManager* config = ConfigManager::getInstance();
    QString logDir = config->getLogPath() + subfolder + "/";
    
    QDir dir;
    bool dirCreated = dir.mkpath(logDir);
    emit logMessage(QString("????????: %1???: %2").arg(logDir).arg(dirCreated ? "??" : "??"));
    
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz");
    QString filename = logDir + prefix + "_" + timestamp + ".png";
    
    if (image.isNull()) {
        emit logMessage(QString("????????: %1?????").arg(filename));
        return false;
    }
    
    if (image.save(filename)) {
        emit logMessage(QString("???????: %1").arg(filename));
        return true;
    } else {
        emit logMessage(QString("????????: %1").arg(filename));
        return false;
    }
}

// ??????????full_screenshots???
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
    // ?????????
    m_inputBoxPositions.clear();
    m_inputBoxFound.clear();
    m_clientWidths.clear();
    m_failedAttempts.clear();
    m_previousAnswerAreas.clear();
    m_stableFrameCounts.clear();
    m_hasDetectedChanges.clear();
    emit logMessage("??????????");
}

void ImageRecognizer::doFindTemplateInWindow(HWND hwnd, const QString &templateName) {
    // ??????
    QPoint resultPos;
    if (findTemplateInWindow(hwnd, templateName, resultPos)) {
        emit templateFound(templateName, resultPos);
    } else {
        emit templateNotFound(templateName);
    }
}

bool ImageRecognizer::loadTemplate(const QString &name, const QString &path) {
    // ??????
    QImage templateImage(path);
    if (!templateImage.isNull()) {
        templates[name] = templateImage;
        emit logMessage(QString("??????: %1???: %2???: %3x%4")
                       .arg(name)
                       .arg(path)
                       .arg(templateImage.width()).arg(templateImage.height()));
        
        // ???????????
        ConfigManager* config = ConfigManager::getInstance();
        config->setTemplateSize(name, templateImage.size());
        emit logMessage(QString("??????????: %1 -> %2x%3")
                       .arg(name)
                       .arg(templateImage.width()).arg(templateImage.height()));
        
        return true;
    } else {
        emit logMessage("??????: " + path);
        return false;
    }
}

void ImageRecognizer::checkAnswerReceivedAsync(HWND hwnd) {
    // ??????????
    bool received = checkAnswerReceived(hwnd);
    emit answerReceivedChecked(hwnd, received);
}
