QT += core gui widgets network

TARGET = WeBot
TEMPLATE = app

CONFIG += c++17 console windows

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050000

# Windows specific configuration
win32 {
    LIBS += -luser32 -lkernel32 -lshell32 -ladvapi32 -lgdi32
    QMAKE_CXXFLAGS += -D_USE_MATH_DEFINES
    DEFINES += NOMINMAX
    
    # OpenCV configuration
    OPENCV_DIR = C:/opencv/OpenCV-MinGW-Build-OpenCV-4.1.0-x64
    INCLUDEPATH += $${OPENCV_DIR}/include
    LIBS += -L$${OPENCV_DIR}/x64/mingw/lib
    LIBS += -lopencv_core410 -lopencv_imgproc410 -lopencv_highgui410 -lopencv_imgcodecs410 -lopencv_features2d410
}

SOURCES = main.cpp mainwindow.cpp automator.cpp configmanager.cpp logger.cpp wechatcontroller.cpp imagerecognizer.cpp inputsimulator.cpp questionmanager.cpp recognitionoverlay.cpp clickcapturewidget.cpp floatingwindow.cpp thememanager.cpp screenshotselector.cpp framelesswindow.cpp

HEADERS = mainwindow.h automator.h configmanager.h logger.h wechatcontroller.h imagerecognizer.h inputsimulator.h questionmanager.h recognitionoverlay.h clickcapturewidget.h floatingwindow.h thememanager.h screenshotselector.h framelesswindow.h

FORMS = mainwindow.ui

RESOURCES = resources.qrc

RC_ICONS = app_icon.ico
