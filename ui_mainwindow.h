/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QKeySequenceEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *mainLayout;
    QTabWidget *tabWidget;
    QWidget *basicTab;
    QLabel *titleLabel;
    QGroupBox *wechatGroup;
    QGridLayout *wechatLayout;
    QCheckBox *floatingWindowCheck;
    QPushButton *browseWechatButton;
    QLineEdit *wechatPathEdit;
    QCheckBox *topMostCheck;
    QLabel *wechatPathLabel;
    QGroupBox *questionGroup;
    QGridLayout *questionLayout;
    QLabel *questionPathLabel;
    QLineEdit *questionPathEdit;
    QPushButton *browseQuestionsButton;
    QLabel *questionModeLabel;
    QComboBox *questionModeCombo;
    QLabel *promptLabel;
    QLineEdit *promptEdit;
    QGroupBox *automationGroup;
    QGridLayout *automationLayout;
    QLabel *loopCountLabel;
    QSpinBox *loopCountSpin;
    QLabel *timeoutLabel;
    QSpinBox *timeoutSpin;
    QLabel *delayLabel;
    QSpinBox *delaySpin;
    QCheckBox *continueOnErrorCheck;
    QCheckBox *continueOnTimeoutCheck;
    QCheckBox *keyboardInputCheck;
    QCheckBox *pasteInputCheck;
    QLabel *inputMethodHintLabel;
    QGroupBox *statusGroup;
    QVBoxLayout *statusLayout;
    QLabel *statusLabel;
    QLabel *progressLabel;
    QProgressBar *progressBar;
    QHBoxLayout *themeSelectLayout;
    QLabel *themeSelectLabel;
    QComboBox *themeCombo;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *startButton;
    QPushButton *stopButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *saveConfigButton;
    QPushButton *loadConfigButton;
    QWidget *advancedTab;
    QGroupBox *pathConfigGroup;
    QGridLayout *pathConfigLayout;
    QLabel *configPathLabel;
    QLineEdit *configPathEdit;
    QPushButton *configPathBrowseButton;
    QLabel *keywordPathLabel;
    QLineEdit *keywordPathEdit;
    QPushButton *keywordPathBrowseButton;
    QLabel *logPathLabel;
    QLineEdit *logPathEdit;
    QPushButton *logPathBrowseButton;
    QLabel *templatePathLabel;
    QLineEdit *templatePathEdit;
    QPushButton *templatePathBrowseButton;
    QPushButton *resetAllPathsButton;
    QGroupBox *recognitionGroup;
    QGridLayout *recognitionLayout;
    QLabel *thresholdLabel;
    QDoubleSpinBox *thresholdSpin;
    QLabel *attemptsLabel;
    QSpinBox *attemptsSpin;
    QLabel *pageTimeoutLabel;
    QSpinBox *pageTimeoutSpin;
    QLabel *recognitionTimeoutLabel;
    QSpinBox *recognitionTimeoutSpin;
    QLabel *recognitionTechniqueLabel;
    QGroupBox *monitorGroup;
    QGridLayout *monitorLayout;
    QCheckBox *multiMonitorCheck;
    QLabel *primaryMonitorLabel;
    QSpinBox *primaryMonitorSpin;
    QGroupBox *shortcutGroup;
    QHBoxLayout *shortcutGroupLayout;
    QLabel *screenshotShortcutLabel;
    QKeySequenceEdit *screenshotShortcutEdit;
    QPushButton *applyShortcutButton;
    QLabel *closeBehaviorLabel;
    QComboBox *closeBehaviorCombo;
    QSpacerItem *shortcutSpacer;
    QWidget *layoutWidget1;
    QHBoxLayout *advancedButtonLayout;
    QSpacerItem *advancedButtonSpacer;
    QPushButton *saveAdvancedConfigButton;
    QPushButton *loadAdvancedConfigButton;
    QWidget *logTab;
    QTextEdit *logTextEdit;
    QWidget *layoutWidget2;
    QVBoxLayout *logButtonLayout;
    QHBoxLayout *logButtons;
    QPushButton *exportLogButton;
    QPushButton *clearLogButton;
    QSpacerItem *logSpacer;
    QLabel *logSavePathLabel;
    QWidget *iconTab;
    QGroupBox *iconGroup;
    QGridLayout *iconGridLayout;
    QLabel *iconNameLabel;
    QComboBox *iconNameCombo;
    QLabel *iconPathLabel_2;
    QLineEdit *iconPathEdit;
    QPushButton *browseIconButton;
    QHBoxLayout *iconPreviewLayout;
    QVBoxLayout *currentIconLayout;
    QLabel *currentIconLabel;
    QLabel *currentIconPreviewLabel;
    QVBoxLayout *selectedIconLayout;
    QLabel *selectedIconLabel;
    QLabel *selectedIconPreviewLabel;
    QHBoxLayout *iconButtonLayout;
    QPushButton *saveIconButton;
    QPushButton *resetIconButton;
    QSpacerItem *iconSpacer;
    QWidget *aboutTab;
    QGroupBox *supportGroup;
    QVBoxLayout *supportLayout;
    QLabel *donationTextLabel;
    QHBoxLayout *qrCodeLayout;
    QVBoxLayout *wechatQrLayout;
    QLabel *wechatQrLabel;
    QLabel *wechatQrCodeLabel;
    QVBoxLayout *alipayQrLayout;
    QLabel *alipayQrLabel;
    QLabel *alipayQrCodeLabel;
    QGroupBox *authorGroup;
    QVBoxLayout *authorLayout;
    QLabel *authorNameLabel;
    QLabel *authorEmailLabel;
    QLabel *authorWebsiteLabel;
    QPushButton *visitWebsiteButton;
    QSpacerItem *aboutSpacer;
    QLabel *copyrightLabel;
    QLabel *versionLabel;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(746, 949);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/app_icon.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8("/* macOS\351\243\216\346\240\274\344\270\273\347\252\227\345\217\243\346\240\267\345\274\217 - \347\233\264\350\247\222\350\256\276\350\256\241 */\n"
"QMainWindow {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"        stop:0 rgba(248, 250, 252, 1),\n"
"        stop:0.5 rgba(241, 245, 249, 1),\n"
"        stop:1 rgba(235, 240, 245, 1));\n"
"    border-radius: 0px;\n"
"    border: 1px solid rgba(66, 133, 244, 0.1);\n"
"}\n"
"\n"
"/* \345\210\206\347\273\204\346\241\206\346\240\267\345\274\217 - macOS\351\243\216\346\240\274\347\233\264\350\247\222 */\n"
"QGroupBox {\n"
"    font-weight: 600;\n"
"    border: 1px solid qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 rgba(66, 133, 244, 0.3),\n"
"        stop:1 rgba(52, 168, 83, 0.3));\n"
"    border-radius: 0px;\n"
"    margin-top: 16px;\n"
"    padding-top: 16px;\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 rgba(255,255, 255, 0.9),\n"
"        stop:1 rgba(248, 250, 252, 0.8));\n"
"}\n"
"\n"
"QGroupBox"
                        "::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 12px;\n"
"    padding: 0 12px;\n"
"    color: #1a73e8;\n"
"    font-size: 14px;\n"
"    font-weight: 600;\n"
"    background: rgba(255, 255, 255, 0.9);\n"
"    border-radius: 0px;\n"
"    margin-top: -8px;\n"
"}\n"
"\n"
"/* \346\214\211\351\222\256\346\240\267\345\274\217 - macOS\351\243\216\346\240\274\347\233\264\350\247\222 */\n"
"QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 rgba(66, 133, 244, 1),\n"
"        stop:1 rgba(52, 121, 193, 1));\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 0px;\n"
"    padding: 8px 16px;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    min-height: 28px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 rgba(52, 121, 193, 1),\n"
"        stop:1 rgba(41, 98, 155, 1));\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 "
                        "rgba(41, 98, 155, 1),\n"
"        stop:1 rgba(30, 75, 117, 1));\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"    background: rgba(189, 189, 189, 0.5);\n"
"    color: rgba(117, 117, 117, 0.8);\n"
"}\n"
"\n"
"/* \347\211\271\346\256\212\346\214\211\351\222\256\346\240\267\345\274\217 */\n"
"QPushButton#startButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 rgba(52, 168, 83, 1),\n"
"        stop:1 rgba(25, 135, 84, 1));\n"
"    font-size: 14px;\n"
"    font-weight: 600;\n"
"    padding: 12px 24px;\n"
"    min-width: 180px;\n"
"    min-height: 40px;\n"
"}\n"
"\n"
"QPushButton#startButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 rgba(25, 135, 84, 1),\n"
"        stop:1 rgba(15, 85, 52, 1));\n"
"}\n"
"\n"
"QPushButton#stopButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 rgba(234, 67, 53, 1),\n"
"        stop:1 rgba(197, 57, 41, 1));\n"
"}\n"
"\n"
"QPushButton#stopButton:hover {\n"
"    background: "
                        "qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 rgba(197, 57, 41, 1),\n"
"        stop:1 rgba(160, 47, 35, 1));\n"
"}\n"
"\n"
"/* \346\240\207\347\255\276\351\241\265\346\240\267\345\274\217 - macOS\351\243\216\346\240\274\347\233\264\350\247\222 */\n"
"QTabWidget::pane {\n"
"    border: 1px solid #e1e5e9;\n"
"    border-radius: 0px;\n"
"    background-color: rgba(255,255,255, 0.9);\n"
"    top: -1px;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background-color: rgba(255,255,255, 0.7);\n"
"    border: 1px solid #e1e5e9;\n"
"    border-bottom: none;\n"
"    border-top-left-radius: 0px;\n"
"    border-top-right-radius: 0px;\n"
"    padding: 8px 16px;\n"
"    margin-right: 2px;\n"
"    color: #5f6368;\n"
"    font-weight: 500;\n"
"    min-width: 80px;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background-color: #ffffff;\n"
"    color: #1a73e8;\n"
"    border-bottom: 2px solid #1a73e8;\n"
"}\n"
"\n"
"QTabBar::tab:hover:!selected {\n"
"    background-color: rgba(26, 115, 232, 0.1);\n"
"    color: #1a73e8;\n"
""
                        "}\n"
"\n"
"/* \350\276\223\345\205\245\346\241\206\346\240\267\345\274\217 - macOS\351\243\216\346\240\274\347\233\264\350\247\222 */\n"
"QLineEdit, QTextEdit, QSpinBox, QDoubleSpinBox, QComboBox {\n"
"    border: 1px solid #dadce0;\n"
"    border-radius: 0px;\n"
"    padding: 8px 12px;\n"
"    background-color: white;\n"
"    font-size: 13px;\n"
"    color: #202124;\n"
"}\n"
"\n"
"QLineEdit:focus, QTextEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus {\n"
"    border-color: #1a73e8;\n"
"    background-color: #f8fafc;\n"
"}\n"
"\n"
"/* \346\240\207\347\255\276\346\240\267\345\274\217 */\n"
"QLabel {\n"
"    color: #202124;\n"
"    font-size: 13px;\n"
"}\n"
"\n"
"/* \345\244\215\351\200\211\346\241\206\346\240\267\345\274\217 - macOS\351\243\216\346\240\274\347\233\264\350\247\222 */\n"
"QCheckBox {\n"
"    spacing: 8px;\n"
"    font-size: 13px;\n"
"    color: #202124;\n"
"}\n"
"\n"
"QCheckBox::indicator {\n"
"    width: 18px;\n"
"    height: 18px;\n"
"    border: 2px solid #dadce0;\n"
"    bor"
                        "der-radius: 0px;\n"
"    background-color: white;\n"
"}\n"
"\n"
"QCheckBox::indicator:checked {\n"
"    background-color: #1a73e8;\n"
"    border-color: #1a73e8;\n"
"}\n"
"\n"
"QCheckBox::indicator:hover {\n"
"    border-color: #1a73e8;\n"
"}\n"
"\n"
"/* \346\273\232\345\212\250\346\235\241\346\240\267\345\274\217 */\n"
"QScrollBar:vertical {\n"
"    background-color: rgba(255,255,255, 0.5);\n"
"    width: 12px;\n"
"    border-radius: 0px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"    background-color: rgba(0, 0, 0, 0.3);\n"
"    border-radius: 0px;\n"
"    min-height: 30px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:hover {\n"
"    background-color: rgba(0, 0, 0, 0.5);\n"
"}\n"
"\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
"    height: 0px;\n"
"}\n"
"\n"
"/* \350\277\233\345\272\246\346\235\241\346\240\267\345\274\217 - macOS\351\243\216\346\240\274\347\233\264\350\247\222 */\n"
"QProgressBar {\n"
"    border: 2px solid #ddd;\n"
"    border-radius: 0px;\n"
"    background-colo"
                        "r: rgba(255,255,255, 0.5);\n"
"    text-align: center;\n"
"    font-weight: 500;\n"
"    color: #333;\n"
"    height: 20px;\n"
"}\n"
"\n"
"QProgressBar::chunk {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"        stop:0 rgba(52, 152, 219, 200),\n"
"        stop:1 rgba(41, 128, 185, 200));\n"
"    border-radius: 0px;\n"
"}\n"
"\n"
"/* \346\227\245\345\277\227\346\226\207\346\234\254\346\241\206\346\240\267\345\274\217 */\n"
"QTextEdit#logTextEdit {\n"
"    background-color: rgba(32, 33, 36, 0.05);\n"
"    border: 1px solid #e1e5e9;\n"
"    border-radius: 0px;\n"
"    font-family: 'Consolas', 'Monaco', monospace;\n"
"    font-size: 12px;\n"
"    line-height: 1.4;\n"
"    color: #202124;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setGeometry(QRect(20, 30, 706, 902));
        mainLayout = new QVBoxLayout(centralwidget);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setEnabled(true);
        tabWidget->setMinimumSize(QSize(100, 200));
        basicTab = new QWidget();
        basicTab->setObjectName("basicTab");
        titleLabel = new QLabel(basicTab);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setEnabled(true);
        titleLabel->setGeometry(QRect(9, 9, 681, 56));
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: 600; color: #2c3e50; margin: 10px;"));
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        wechatGroup = new QGroupBox(basicTab);
        wechatGroup->setObjectName("wechatGroup");
        wechatGroup->setEnabled(true);
        wechatGroup->setGeometry(QRect(9, 87, 681, 138));
        wechatLayout = new QGridLayout(wechatGroup);
        wechatLayout->setObjectName("wechatLayout");
        floatingWindowCheck = new QCheckBox(wechatGroup);
        floatingWindowCheck->setObjectName("floatingWindowCheck");

        wechatLayout->addWidget(floatingWindowCheck, 3, 0, 1, 3);

        browseWechatButton = new QPushButton(wechatGroup);
        browseWechatButton->setObjectName("browseWechatButton");
        browseWechatButton->setMinimumSize(QSize(80, 44));

        wechatLayout->addWidget(browseWechatButton, 1, 2, 1, 1);

        wechatPathEdit = new QLineEdit(wechatGroup);
        wechatPathEdit->setObjectName("wechatPathEdit");

        wechatLayout->addWidget(wechatPathEdit, 1, 1, 1, 1);

        topMostCheck = new QCheckBox(wechatGroup);
        topMostCheck->setObjectName("topMostCheck");

        wechatLayout->addWidget(topMostCheck, 2, 0, 1, 3);

        wechatPathLabel = new QLabel(wechatGroup);
        wechatPathLabel->setObjectName("wechatPathLabel");

        wechatLayout->addWidget(wechatPathLabel, 1, 0, 1, 1);

        questionGroup = new QGroupBox(basicTab);
        questionGroup->setObjectName("questionGroup");
        questionGroup->setGeometry(QRect(9, 231, 681, 158));
        questionLayout = new QGridLayout(questionGroup);
        questionLayout->setObjectName("questionLayout");
        questionPathLabel = new QLabel(questionGroup);
        questionPathLabel->setObjectName("questionPathLabel");

        questionLayout->addWidget(questionPathLabel, 0, 0, 1, 1);

        questionPathEdit = new QLineEdit(questionGroup);
        questionPathEdit->setObjectName("questionPathEdit");

        questionLayout->addWidget(questionPathEdit, 0, 1, 1, 1);

        browseQuestionsButton = new QPushButton(questionGroup);
        browseQuestionsButton->setObjectName("browseQuestionsButton");
        browseQuestionsButton->setMinimumSize(QSize(80, 44));

        questionLayout->addWidget(browseQuestionsButton, 0, 2, 1, 1);

        questionModeLabel = new QLabel(questionGroup);
        questionModeLabel->setObjectName("questionModeLabel");

        questionLayout->addWidget(questionModeLabel, 1, 0, 1, 1);

        questionModeCombo = new QComboBox(questionGroup);
        questionModeCombo->addItem(QString());
        questionModeCombo->addItem(QString());
        questionModeCombo->setObjectName("questionModeCombo");

        questionLayout->addWidget(questionModeCombo, 1, 1, 1, 2);

        promptLabel = new QLabel(questionGroup);
        promptLabel->setObjectName("promptLabel");

        questionLayout->addWidget(promptLabel, 2, 0, 1, 1);

        promptEdit = new QLineEdit(questionGroup);
        promptEdit->setObjectName("promptEdit");

        questionLayout->addWidget(promptEdit, 2, 1, 1, 2);

        automationGroup = new QGroupBox(basicTab);
        automationGroup->setObjectName("automationGroup");
        automationGroup->setGeometry(QRect(9, 395, 684, 209));
        automationLayout = new QGridLayout(automationGroup);
        automationLayout->setObjectName("automationLayout");
        loopCountLabel = new QLabel(automationGroup);
        loopCountLabel->setObjectName("loopCountLabel");

        automationLayout->addWidget(loopCountLabel, 0, 0, 1, 1);

        loopCountSpin = new QSpinBox(automationGroup);
        loopCountSpin->setObjectName("loopCountSpin");
        loopCountSpin->setMinimum(1);
        loopCountSpin->setMaximum(1000);
        loopCountSpin->setValue(10);

        automationLayout->addWidget(loopCountSpin, 0, 1, 1, 1);

        timeoutLabel = new QLabel(automationGroup);
        timeoutLabel->setObjectName("timeoutLabel");

        automationLayout->addWidget(timeoutLabel, 0, 2, 1, 1);

        timeoutSpin = new QSpinBox(automationGroup);
        timeoutSpin->setObjectName("timeoutSpin");
        timeoutSpin->setMinimum(10);
        timeoutSpin->setMaximum(300000);
        timeoutSpin->setValue(50);

        automationLayout->addWidget(timeoutSpin, 0, 3, 1, 1);

        delayLabel = new QLabel(automationGroup);
        delayLabel->setObjectName("delayLabel");

        automationLayout->addWidget(delayLabel, 1, 0, 1, 1);

        delaySpin = new QSpinBox(automationGroup);
        delaySpin->setObjectName("delaySpin");
        delaySpin->setMinimum(10);
        delaySpin->setMaximum(60000);
        delaySpin->setValue(50);

        automationLayout->addWidget(delaySpin, 1, 1, 1, 1);

        continueOnErrorCheck = new QCheckBox(automationGroup);
        continueOnErrorCheck->setObjectName("continueOnErrorCheck");
        continueOnErrorCheck->setChecked(true);

        automationLayout->addWidget(continueOnErrorCheck, 2, 0, 1, 1);

        continueOnTimeoutCheck = new QCheckBox(automationGroup);
        continueOnTimeoutCheck->setObjectName("continueOnTimeoutCheck");
        continueOnTimeoutCheck->setChecked(true);

        automationLayout->addWidget(continueOnTimeoutCheck, 2, 1, 1, 1);

        keyboardInputCheck = new QCheckBox(automationGroup);
        keyboardInputCheck->setObjectName("keyboardInputCheck");
        keyboardInputCheck->setChecked(true);

        automationLayout->addWidget(keyboardInputCheck, 3, 0, 1, 2);

        pasteInputCheck = new QCheckBox(automationGroup);
        pasteInputCheck->setObjectName("pasteInputCheck");

        automationLayout->addWidget(pasteInputCheck, 3, 2, 1, 2);

        inputMethodHintLabel = new QLabel(automationGroup);
        inputMethodHintLabel->setObjectName("inputMethodHintLabel");
        inputMethodHintLabel->setStyleSheet(QString::fromUtf8("font-size: 12px; color: #e67e22; font-weight: 500;"));
        inputMethodHintLabel->setWordWrap(true);

        automationLayout->addWidget(inputMethodHintLabel, 4, 0, 1, 4);

        statusGroup = new QGroupBox(basicTab);
        statusGroup->setObjectName("statusGroup");
        statusGroup->setGeometry(QRect(9, 687, 681, 165));
        statusLayout = new QVBoxLayout(statusGroup);
        statusLayout->setObjectName("statusLayout");
        statusLabel = new QLabel(statusGroup);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setStyleSheet(QString::fromUtf8("font-weight: 600; color: #2ecc71;"));

        statusLayout->addWidget(statusLabel);

        progressLabel = new QLabel(statusGroup);
        progressLabel->setObjectName("progressLabel");

        statusLayout->addWidget(progressLabel);

        progressBar = new QProgressBar(statusGroup);
        progressBar->setObjectName("progressBar");
        progressBar->setMinimumSize(QSize(0, 25));
        progressBar->setValue(0);

        statusLayout->addWidget(progressBar);

        themeSelectLayout = new QHBoxLayout();
        themeSelectLayout->setObjectName("themeSelectLayout");
        themeSelectLabel = new QLabel(statusGroup);
        themeSelectLabel->setObjectName("themeSelectLabel");

        themeSelectLayout->addWidget(themeSelectLabel);

        themeCombo = new QComboBox(statusGroup);
        themeCombo->addItem(QString());
        themeCombo->addItem(QString());
        themeCombo->addItem(QString());
        themeCombo->addItem(QString());
        themeCombo->addItem(QString());
        themeCombo->setObjectName("themeCombo");
        themeCombo->setMinimumSize(QSize(120, 0));

        themeSelectLayout->addWidget(themeCombo);


        statusLayout->addLayout(themeSelectLayout);

        layoutWidget = new QWidget(basicTab);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(10, 620, 681, 66));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        startButton = new QPushButton(layoutWidget);
        startButton->setObjectName("startButton");
        startButton->setMinimumSize(QSize(228, 44));

        horizontalLayout->addWidget(startButton);

        stopButton = new QPushButton(layoutWidget);
        stopButton->setObjectName("stopButton");

        horizontalLayout->addWidget(stopButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        saveConfigButton = new QPushButton(layoutWidget);
        saveConfigButton->setObjectName("saveConfigButton");
        saveConfigButton->setMinimumSize(QSize(100, 44));

        horizontalLayout->addWidget(saveConfigButton);

        loadConfigButton = new QPushButton(layoutWidget);
        loadConfigButton->setObjectName("loadConfigButton");
        loadConfigButton->setMinimumSize(QSize(100, 44));

        horizontalLayout->addWidget(loadConfigButton);

        tabWidget->addTab(basicTab, QString());
        advancedTab = new QWidget();
        advancedTab->setObjectName("advancedTab");
        pathConfigGroup = new QGroupBox(advancedTab);
        pathConfigGroup->setObjectName("pathConfigGroup");
        pathConfigGroup->setGeometry(QRect(9, 9, 681, 247));
        pathConfigLayout = new QGridLayout(pathConfigGroup);
        pathConfigLayout->setObjectName("pathConfigLayout");
        configPathLabel = new QLabel(pathConfigGroup);
        configPathLabel->setObjectName("configPathLabel");

        pathConfigLayout->addWidget(configPathLabel, 0, 0, 1, 1);

        configPathEdit = new QLineEdit(pathConfigGroup);
        configPathEdit->setObjectName("configPathEdit");
        configPathEdit->setReadOnly(true);

        pathConfigLayout->addWidget(configPathEdit, 0, 1, 1, 1);

        configPathBrowseButton = new QPushButton(pathConfigGroup);
        configPathBrowseButton->setObjectName("configPathBrowseButton");
        configPathBrowseButton->setMinimumSize(QSize(80, 44));

        pathConfigLayout->addWidget(configPathBrowseButton, 0, 2, 1, 1);

        keywordPathLabel = new QLabel(pathConfigGroup);
        keywordPathLabel->setObjectName("keywordPathLabel");

        pathConfigLayout->addWidget(keywordPathLabel, 1, 0, 1, 1);

        keywordPathEdit = new QLineEdit(pathConfigGroup);
        keywordPathEdit->setObjectName("keywordPathEdit");

        pathConfigLayout->addWidget(keywordPathEdit, 1, 1, 1, 1);

        keywordPathBrowseButton = new QPushButton(pathConfigGroup);
        keywordPathBrowseButton->setObjectName("keywordPathBrowseButton");
        keywordPathBrowseButton->setMinimumSize(QSize(80, 44));

        pathConfigLayout->addWidget(keywordPathBrowseButton, 1, 2, 1, 1);

        logPathLabel = new QLabel(pathConfigGroup);
        logPathLabel->setObjectName("logPathLabel");

        pathConfigLayout->addWidget(logPathLabel, 2, 0, 1, 1);

        logPathEdit = new QLineEdit(pathConfigGroup);
        logPathEdit->setObjectName("logPathEdit");

        pathConfigLayout->addWidget(logPathEdit, 2, 1, 1, 1);

        logPathBrowseButton = new QPushButton(pathConfigGroup);
        logPathBrowseButton->setObjectName("logPathBrowseButton");
        logPathBrowseButton->setMinimumSize(QSize(80, 44));

        pathConfigLayout->addWidget(logPathBrowseButton, 2, 2, 1, 1);

        templatePathLabel = new QLabel(pathConfigGroup);
        templatePathLabel->setObjectName("templatePathLabel");

        pathConfigLayout->addWidget(templatePathLabel, 3, 0, 1, 1);

        templatePathEdit = new QLineEdit(pathConfigGroup);
        templatePathEdit->setObjectName("templatePathEdit");

        pathConfigLayout->addWidget(templatePathEdit, 3, 1, 1, 1);

        templatePathBrowseButton = new QPushButton(pathConfigGroup);
        templatePathBrowseButton->setObjectName("templatePathBrowseButton");
        templatePathBrowseButton->setMinimumSize(QSize(80, 44));

        pathConfigLayout->addWidget(templatePathBrowseButton, 3, 2, 1, 1);

        resetAllPathsButton = new QPushButton(pathConfigGroup);
        resetAllPathsButton->setObjectName("resetAllPathsButton");
        resetAllPathsButton->setMinimumSize(QSize(120, 44));

        pathConfigLayout->addWidget(resetAllPathsButton, 4, 0, 1, 3);

        recognitionGroup = new QGroupBox(advancedTab);
        recognitionGroup->setObjectName("recognitionGroup");
        recognitionGroup->setGeometry(QRect(9, 262, 681, 152));
        recognitionLayout = new QGridLayout(recognitionGroup);
        recognitionLayout->setObjectName("recognitionLayout");
        thresholdLabel = new QLabel(recognitionGroup);
        thresholdLabel->setObjectName("thresholdLabel");

        recognitionLayout->addWidget(thresholdLabel, 0, 0, 1, 1);

        thresholdSpin = new QDoubleSpinBox(recognitionGroup);
        thresholdSpin->setObjectName("thresholdSpin");
        thresholdSpin->setDecimals(2);
        thresholdSpin->setMinimum(0.100000000000000);
        thresholdSpin->setMaximum(1.000000000000000);
        thresholdSpin->setSingleStep(0.050000000000000);
        thresholdSpin->setValue(0.800000000000000);

        recognitionLayout->addWidget(thresholdSpin, 0, 1, 1, 1);

        attemptsLabel = new QLabel(recognitionGroup);
        attemptsLabel->setObjectName("attemptsLabel");

        recognitionLayout->addWidget(attemptsLabel, 0, 2, 1, 1);

        attemptsSpin = new QSpinBox(recognitionGroup);
        attemptsSpin->setObjectName("attemptsSpin");
        attemptsSpin->setMinimum(1);
        attemptsSpin->setMaximum(10);
        attemptsSpin->setValue(3);

        recognitionLayout->addWidget(attemptsSpin, 0, 3, 1, 1);

        pageTimeoutLabel = new QLabel(recognitionGroup);
        pageTimeoutLabel->setObjectName("pageTimeoutLabel");

        recognitionLayout->addWidget(pageTimeoutLabel, 1, 0, 1, 1);

        pageTimeoutSpin = new QSpinBox(recognitionGroup);
        pageTimeoutSpin->setObjectName("pageTimeoutSpin");
        pageTimeoutSpin->setMinimum(10);
        pageTimeoutSpin->setMaximum(300000);
        pageTimeoutSpin->setValue(50);

        recognitionLayout->addWidget(pageTimeoutSpin, 1, 1, 1, 1);

        recognitionTimeoutLabel = new QLabel(recognitionGroup);
        recognitionTimeoutLabel->setObjectName("recognitionTimeoutLabel");

        recognitionLayout->addWidget(recognitionTimeoutLabel, 1, 2, 1, 1);

        recognitionTimeoutSpin = new QSpinBox(recognitionGroup);
        recognitionTimeoutSpin->setObjectName("recognitionTimeoutSpin");
        recognitionTimeoutSpin->setMinimum(10);
        recognitionTimeoutSpin->setMaximum(300000);
        recognitionTimeoutSpin->setValue(50);

        recognitionLayout->addWidget(recognitionTimeoutSpin, 1, 3, 1, 1);

        recognitionTechniqueLabel = new QLabel(recognitionGroup);
        recognitionTechniqueLabel->setObjectName("recognitionTechniqueLabel");

        recognitionLayout->addWidget(recognitionTechniqueLabel, 2, 0, 1, 4);

        monitorGroup = new QGroupBox(advancedTab);
        monitorGroup->setObjectName("monitorGroup");
        monitorGroup->setGeometry(QRect(9, 420, 681, 85));
        monitorLayout = new QGridLayout(monitorGroup);
        monitorLayout->setObjectName("monitorLayout");
        multiMonitorCheck = new QCheckBox(monitorGroup);
        multiMonitorCheck->setObjectName("multiMonitorCheck");

        monitorLayout->addWidget(multiMonitorCheck, 0, 0, 1, 1);

        primaryMonitorLabel = new QLabel(monitorGroup);
        primaryMonitorLabel->setObjectName("primaryMonitorLabel");

        monitorLayout->addWidget(primaryMonitorLabel, 0, 1, 1, 1);

        primaryMonitorSpin = new QSpinBox(monitorGroup);
        primaryMonitorSpin->setObjectName("primaryMonitorSpin");
        primaryMonitorSpin->setMinimum(0);
        primaryMonitorSpin->setMaximum(10);
        primaryMonitorSpin->setValue(0);

        monitorLayout->addWidget(primaryMonitorSpin, 0, 2, 1, 1);

        shortcutGroup = new QGroupBox(advancedTab);
        shortcutGroup->setObjectName("shortcutGroup");
        shortcutGroup->setGeometry(QRect(10, 520, 681, 82));
        shortcutGroupLayout = new QHBoxLayout(shortcutGroup);
        shortcutGroupLayout->setObjectName("shortcutGroupLayout");
        screenshotShortcutLabel = new QLabel(shortcutGroup);
        screenshotShortcutLabel->setObjectName("screenshotShortcutLabel");

        shortcutGroupLayout->addWidget(screenshotShortcutLabel);

        screenshotShortcutEdit = new QKeySequenceEdit(shortcutGroup);
        screenshotShortcutEdit->setObjectName("screenshotShortcutEdit");
        screenshotShortcutEdit->setMaximumSize(QSize(120, 16777215));

        shortcutGroupLayout->addWidget(screenshotShortcutEdit);

        applyShortcutButton = new QPushButton(shortcutGroup);
        applyShortcutButton->setObjectName("applyShortcutButton");
        applyShortcutButton->setMaximumSize(QSize(60, 16777215));

        shortcutGroupLayout->addWidget(applyShortcutButton);

        closeBehaviorLabel = new QLabel(shortcutGroup);
        closeBehaviorLabel->setObjectName("closeBehaviorLabel");

        shortcutGroupLayout->addWidget(closeBehaviorLabel);

        closeBehaviorCombo = new QComboBox(shortcutGroup);
        closeBehaviorCombo->addItem(QString());
        closeBehaviorCombo->addItem(QString());
        closeBehaviorCombo->setObjectName("closeBehaviorCombo");

        shortcutGroupLayout->addWidget(closeBehaviorCombo);

        shortcutSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        shortcutGroupLayout->addItem(shortcutSpacer);

        layoutWidget1 = new QWidget(advancedTab);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(9, 712, 681, 46));
        advancedButtonLayout = new QHBoxLayout(layoutWidget1);
        advancedButtonLayout->setObjectName("advancedButtonLayout");
        advancedButtonLayout->setContentsMargins(0, 0, 0, 0);
        advancedButtonSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        advancedButtonLayout->addItem(advancedButtonSpacer);

        saveAdvancedConfigButton = new QPushButton(layoutWidget1);
        saveAdvancedConfigButton->setObjectName("saveAdvancedConfigButton");
        saveAdvancedConfigButton->setMinimumSize(QSize(100, 44));

        advancedButtonLayout->addWidget(saveAdvancedConfigButton);

        loadAdvancedConfigButton = new QPushButton(layoutWidget1);
        loadAdvancedConfigButton->setObjectName("loadAdvancedConfigButton");
        loadAdvancedConfigButton->setMinimumSize(QSize(100, 44));

        advancedButtonLayout->addWidget(loadAdvancedConfigButton);

        tabWidget->addTab(advancedTab, QString());
        logTab = new QWidget();
        logTab->setObjectName("logTab");
        logTextEdit = new QTextEdit(logTab);
        logTextEdit->setObjectName("logTextEdit");
        logTextEdit->setGeometry(QRect(9, 9, 681, 671));
        logTextEdit->setReadOnly(true);
        layoutWidget2 = new QWidget(logTab);
        layoutWidget2->setObjectName("layoutWidget2");
        layoutWidget2->setGeometry(QRect(10, 690, 681, 91));
        logButtonLayout = new QVBoxLayout(layoutWidget2);
        logButtonLayout->setObjectName("logButtonLayout");
        logButtonLayout->setContentsMargins(0, 0, 0, 0);
        logButtons = new QHBoxLayout();
        logButtons->setObjectName("logButtons");
        exportLogButton = new QPushButton(layoutWidget2);
        exportLogButton->setObjectName("exportLogButton");
        exportLogButton->setMinimumSize(QSize(120, 44));

        logButtons->addWidget(exportLogButton);

        clearLogButton = new QPushButton(layoutWidget2);
        clearLogButton->setObjectName("clearLogButton");
        clearLogButton->setMinimumSize(QSize(120, 44));

        logButtons->addWidget(clearLogButton);

        logSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        logButtons->addItem(logSpacer);


        logButtonLayout->addLayout(logButtons);

        logSavePathLabel = new QLabel(layoutWidget2);
        logSavePathLabel->setObjectName("logSavePathLabel");
        logSavePathLabel->setStyleSheet(QString::fromUtf8("font-size: 12px; color: #666666;"));

        logButtonLayout->addWidget(logSavePathLabel);

        tabWidget->addTab(logTab, QString());
        iconTab = new QWidget();
        iconTab->setObjectName("iconTab");
        iconGroup = new QGroupBox(iconTab);
        iconGroup->setObjectName("iconGroup");
        iconGroup->setGeometry(QRect(9, 9, 681, 501));
        iconGridLayout = new QGridLayout(iconGroup);
        iconGridLayout->setObjectName("iconGridLayout");
        iconNameLabel = new QLabel(iconGroup);
        iconNameLabel->setObjectName("iconNameLabel");

        iconGridLayout->addWidget(iconNameLabel, 0, 0, 1, 1);

        iconNameCombo = new QComboBox(iconGroup);
        iconNameCombo->addItem(QString());
        iconNameCombo->addItem(QString());
        iconNameCombo->addItem(QString());
        iconNameCombo->addItem(QString());
        iconNameCombo->addItem(QString());
        iconNameCombo->addItem(QString());
        iconNameCombo->addItem(QString());
        iconNameCombo->addItem(QString());
        iconNameCombo->addItem(QString());
        iconNameCombo->addItem(QString());
        iconNameCombo->addItem(QString());
        iconNameCombo->addItem(QString());
        iconNameCombo->addItem(QString());
        iconNameCombo->setObjectName("iconNameCombo");

        iconGridLayout->addWidget(iconNameCombo, 0, 1, 1, 2);

        iconPathLabel_2 = new QLabel(iconGroup);
        iconPathLabel_2->setObjectName("iconPathLabel_2");

        iconGridLayout->addWidget(iconPathLabel_2, 1, 0, 1, 1);

        iconPathEdit = new QLineEdit(iconGroup);
        iconPathEdit->setObjectName("iconPathEdit");

        iconGridLayout->addWidget(iconPathEdit, 1, 1, 1, 1);

        browseIconButton = new QPushButton(iconGroup);
        browseIconButton->setObjectName("browseIconButton");
        browseIconButton->setMaximumSize(QSize(80, 16777215));

        iconGridLayout->addWidget(browseIconButton, 1, 2, 1, 1);

        iconPreviewLayout = new QHBoxLayout();
        iconPreviewLayout->setObjectName("iconPreviewLayout");
        currentIconLayout = new QVBoxLayout();
        currentIconLayout->setObjectName("currentIconLayout");
        currentIconLabel = new QLabel(iconGroup);
        currentIconLabel->setObjectName("currentIconLabel");
        currentIconLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        currentIconLayout->addWidget(currentIconLabel);

        currentIconPreviewLabel = new QLabel(iconGroup);
        currentIconPreviewLabel->setObjectName("currentIconPreviewLabel");
        currentIconPreviewLabel->setEnabled(true);
        currentIconPreviewLabel->setMinimumSize(QSize(150, 100));
        currentIconPreviewLabel->setMaximumSize(QSize(150, 100));
        currentIconPreviewLabel->setStyleSheet(QString::fromUtf8("background-color: #f0f0f0; border: 1px solid #ddd; border-radius: 0px;"));
        currentIconPreviewLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        currentIconLayout->addWidget(currentIconPreviewLabel, 0, Qt::AlignmentFlag::AlignHCenter);


        iconPreviewLayout->addLayout(currentIconLayout);

        selectedIconLayout = new QVBoxLayout();
        selectedIconLayout->setObjectName("selectedIconLayout");
        selectedIconLabel = new QLabel(iconGroup);
        selectedIconLabel->setObjectName("selectedIconLabel");
        selectedIconLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        selectedIconLayout->addWidget(selectedIconLabel);

        selectedIconPreviewLabel = new QLabel(iconGroup);
        selectedIconPreviewLabel->setObjectName("selectedIconPreviewLabel");
        selectedIconPreviewLabel->setMinimumSize(QSize(150, 100));
        selectedIconPreviewLabel->setMaximumSize(QSize(150, 100));
        selectedIconPreviewLabel->setStyleSheet(QString::fromUtf8("background-color: #f0f0f0; border: 1px solid #ddd; border-radius: 0px;"));
        selectedIconPreviewLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        selectedIconLayout->addWidget(selectedIconPreviewLabel, 0, Qt::AlignmentFlag::AlignHCenter);


        iconPreviewLayout->addLayout(selectedIconLayout);


        iconGridLayout->addLayout(iconPreviewLayout, 2, 0, 1, 3);

        iconButtonLayout = new QHBoxLayout();
        iconButtonLayout->setObjectName("iconButtonLayout");
        saveIconButton = new QPushButton(iconGroup);
        saveIconButton->setObjectName("saveIconButton");
        saveIconButton->setMinimumSize(QSize(120, 44));

        iconButtonLayout->addWidget(saveIconButton);

        resetIconButton = new QPushButton(iconGroup);
        resetIconButton->setObjectName("resetIconButton");
        resetIconButton->setMinimumSize(QSize(120, 44));

        iconButtonLayout->addWidget(resetIconButton);

        iconSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        iconButtonLayout->addItem(iconSpacer);


        iconGridLayout->addLayout(iconButtonLayout, 3, 0, 1, 3);

        tabWidget->addTab(iconTab, QString());
        aboutTab = new QWidget();
        aboutTab->setObjectName("aboutTab");
        supportGroup = new QGroupBox(aboutTab);
        supportGroup->setObjectName("supportGroup");
        supportGroup->setGeometry(QRect(9, 9, 684, 244));
        supportLayout = new QVBoxLayout(supportGroup);
        supportLayout->setObjectName("supportLayout");
        donationTextLabel = new QLabel(supportGroup);
        donationTextLabel->setObjectName("donationTextLabel");
        donationTextLabel->setWordWrap(true);

        supportLayout->addWidget(donationTextLabel);

        qrCodeLayout = new QHBoxLayout();
        qrCodeLayout->setObjectName("qrCodeLayout");
        wechatQrLayout = new QVBoxLayout();
        wechatQrLayout->setObjectName("wechatQrLayout");
        wechatQrLabel = new QLabel(supportGroup);
        wechatQrLabel->setObjectName("wechatQrLabel");
        wechatQrLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        wechatQrLayout->addWidget(wechatQrLabel);

        wechatQrCodeLabel = new QLabel(supportGroup);
        wechatQrCodeLabel->setObjectName("wechatQrCodeLabel");
        wechatQrCodeLabel->setMinimumSize(QSize(150, 150));
        wechatQrCodeLabel->setMaximumSize(QSize(150, 150));
        wechatQrCodeLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        wechatQrLayout->addWidget(wechatQrCodeLabel);


        qrCodeLayout->addLayout(wechatQrLayout);

        alipayQrLayout = new QVBoxLayout();
        alipayQrLayout->setObjectName("alipayQrLayout");
        alipayQrLabel = new QLabel(supportGroup);
        alipayQrLabel->setObjectName("alipayQrLabel");
        alipayQrLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        alipayQrLayout->addWidget(alipayQrLabel);

        alipayQrCodeLabel = new QLabel(supportGroup);
        alipayQrCodeLabel->setObjectName("alipayQrCodeLabel");
        alipayQrCodeLabel->setMinimumSize(QSize(150, 150));
        alipayQrCodeLabel->setMaximumSize(QSize(150, 150));
        alipayQrCodeLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        alipayQrLayout->addWidget(alipayQrCodeLabel);


        qrCodeLayout->addLayout(alipayQrLayout);


        supportLayout->addLayout(qrCodeLayout);

        authorGroup = new QGroupBox(aboutTab);
        authorGroup->setObjectName("authorGroup");
        authorGroup->setGeometry(QRect(9, 259, 681, 301));
        authorLayout = new QVBoxLayout(authorGroup);
        authorLayout->setObjectName("authorLayout");
        authorNameLabel = new QLabel(authorGroup);
        authorNameLabel->setObjectName("authorNameLabel");

        authorLayout->addWidget(authorNameLabel);

        authorEmailLabel = new QLabel(authorGroup);
        authorEmailLabel->setObjectName("authorEmailLabel");

        authorLayout->addWidget(authorEmailLabel);

        authorWebsiteLabel = new QLabel(authorGroup);
        authorWebsiteLabel->setObjectName("authorWebsiteLabel");
        authorWebsiteLabel->setOpenExternalLinks(true);

        authorLayout->addWidget(authorWebsiteLabel);

        visitWebsiteButton = new QPushButton(authorGroup);
        visitWebsiteButton->setObjectName("visitWebsiteButton");
        visitWebsiteButton->setMinimumSize(QSize(150, 44));

        authorLayout->addWidget(visitWebsiteButton);

        aboutSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        authorLayout->addItem(aboutSpacer);

        copyrightLabel = new QLabel(authorGroup);
        copyrightLabel->setObjectName("copyrightLabel");
        copyrightLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        authorLayout->addWidget(copyrightLabel);

        versionLabel = new QLabel(authorGroup);
        versionLabel->setObjectName("versionLabel");
        versionLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        authorLayout->addWidget(versionLabel);

        tabWidget->addTab(aboutTab, QString());

        mainLayout->addWidget(tabWidget);


        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "WeBot - \344\274\201\344\270\232\345\276\256\344\277\241\350\207\252\345\212\250\351\227\256\347\255\224\345\267\245\345\205\267", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "\360\237\244\226 WeBot - \344\274\201\344\270\232\345\276\256\344\277\241\350\207\252\345\212\250\351\227\256\347\255\224\345\267\245\345\205\267", nullptr));
        wechatGroup->setTitle(QCoreApplication::translate("MainWindow", "\344\274\201\344\270\232\345\276\256\344\277\241\350\256\276\347\275\256", nullptr));
        floatingWindowCheck->setText(QCoreApplication::translate("MainWindow", "\345\220\257\347\224\250\346\202\254\346\265\256\347\252\227", nullptr));
        browseWechatButton->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210...", nullptr));
        wechatPathEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251\344\274\201\344\270\232\345\276\256\344\277\241\345\256\211\350\243\205\350\267\257\345\276\204", nullptr));
        topMostCheck->setText(QCoreApplication::translate("MainWindow", "\347\252\227\345\217\243\347\275\256\351\241\266\346\230\276\347\244\272", nullptr));
        wechatPathLabel->setText(QCoreApplication::translate("MainWindow", "\344\274\201\344\270\232\345\276\256\344\277\241\350\267\257\345\276\204:", nullptr));
        questionGroup->setTitle(QCoreApplication::translate("MainWindow", "\351\227\256\351\242\230\350\256\276\347\275\256", nullptr));
        questionPathLabel->setText(QCoreApplication::translate("MainWindow", "\351\227\256\351\242\230\345\272\223\350\267\257\345\276\204:", nullptr));
        questionPathEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251\351\227\256\351\242\230\345\272\223\346\226\207\344\273\266", nullptr));
        browseQuestionsButton->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210...", nullptr));
        questionModeLabel->setText(QCoreApplication::translate("MainWindow", "\351\227\256\351\242\230\346\250\241\345\274\217:", nullptr));
        questionModeCombo->setItemText(0, QCoreApplication::translate("MainWindow", "\360\237\224\204 \351\241\272\345\272\217\345\276\252\347\216\257 - \346\214\211\351\227\256\351\242\230\345\272\223\351\241\272\345\272\217\351\200\220\344\270\200\344\275\277\347\224\250", nullptr));
        questionModeCombo->setItemText(1, QCoreApplication::translate("MainWindow", "\360\237\216\262 \351\232\217\346\234\272\351\200\211\346\213\251 - \344\273\216\351\227\256\351\242\230\345\272\223\344\270\255\351\232\217\346\234\272\346\214\221\351\200\211", nullptr));

#if QT_CONFIG(tooltip)
        questionModeCombo->setToolTip(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\351\227\256\351\242\230\345\233\236\347\255\224\347\232\204\346\226\271\345\274\217", nullptr));
#endif // QT_CONFIG(tooltip)
        promptLabel->setText(QCoreApplication::translate("MainWindow", "\345\233\236\347\255\224\351\231\220\345\210\266\346\217\220\347\244\272:", nullptr));
        promptEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\345\233\236\347\255\224\350\257\267\345\213\277\350\266\205\350\277\207\345\215\201\344\270\252\345\255\227", nullptr));
        automationGroup->setTitle(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\345\214\226\350\256\276\347\275\256", nullptr));
        loopCountLabel->setText(QCoreApplication::translate("MainWindow", "\346\211\247\350\241\214\346\254\241\346\225\260:", nullptr));
        timeoutLabel->setText(QCoreApplication::translate("MainWindow", "\345\233\236\347\255\224\350\266\205\346\227\266(\346\257\253\347\247\222):", nullptr));
        delayLabel->setText(QCoreApplication::translate("MainWindow", "\350\275\256\346\254\241\351\227\264\351\232\224(\346\257\253\347\247\222):", nullptr));
        continueOnErrorCheck->setText(QCoreApplication::translate("MainWindow", "\345\207\272\351\224\231\346\227\266\347\273\247\347\273\255\346\211\247\350\241\214", nullptr));
        continueOnTimeoutCheck->setText(QCoreApplication::translate("MainWindow", "\350\266\205\346\227\266\345\220\216\347\273\247\347\273\255\346\211\247\350\241\214", nullptr));
        keyboardInputCheck->setText(QCoreApplication::translate("MainWindow", "\346\250\241\346\213\237\351\224\256\347\233\230\351\200\220\344\270\252\350\276\223\345\205\245", nullptr));
        pasteInputCheck->setText(QCoreApplication::translate("MainWindow", "\345\244\215\345\210\266\347\262\230\350\264\264\350\276\223\345\205\245", nullptr));
        inputMethodHintLabel->setText(QCoreApplication::translate("MainWindow", "\360\237\222\241 \346\217\220\347\244\272\357\274\232\346\214\211\344\270\213ESC\346\214\211\351\224\256\345\217\257\345\201\234\346\255\242\350\207\252\345\212\250\345\214\226\346\211\247\350\241\214\357\274\214\351\200\211\346\213\251\345\220\210\351\200\202\347\232\204\350\276\223\345\205\245\346\226\271\345\274\217\357\274\210\345\217\252\350\203\275\351\200\211\346\213\251\344\270\200\347\247\215\357\274\211\343\200\202", nullptr));
        statusGroup->setTitle(QCoreApplication::translate("MainWindow", "\350\277\220\350\241\214\347\212\266\346\200\201", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201: \345\260\261\347\273\252", nullptr));
        progressLabel->setText(QCoreApplication::translate("MainWindow", "\350\277\233\345\272\246: 0/0", nullptr));
        themeSelectLabel->setText(QCoreApplication::translate("MainWindow", "\347\225\214\351\235\242\344\270\273\351\242\230:", nullptr));
        themeCombo->setItemText(0, QCoreApplication::translate("MainWindow", "\346\265\205\350\211\262\344\270\273\351\242\230", nullptr));
        themeCombo->setItemText(1, QCoreApplication::translate("MainWindow", "\346\267\261\350\211\262\344\270\273\351\242\230", nullptr));
        themeCombo->setItemText(2, QCoreApplication::translate("MainWindow", "\346\250\261\350\212\261\344\270\273\351\242\230", nullptr));
        themeCombo->setItemText(3, QCoreApplication::translate("MainWindow", "\347\247\221\346\212\200\344\270\273\351\242\230", nullptr));
        themeCombo->setItemText(4, QCoreApplication::translate("MainWindow", "\346\232\226\350\211\262\344\270\273\351\242\230", nullptr));

        startButton->setText(QCoreApplication::translate("MainWindow", "\360\237\232\200 \345\274\200\345\247\213\350\207\252\345\212\250\351\227\256\347\255\224", nullptr));
        stopButton->setText(QCoreApplication::translate("MainWindow", "  \342\217\271\357\270\217 \345\201\234\346\255\242\346\211\247\350\241\214     ", nullptr));
#if QT_CONFIG(tooltip)
        saveConfigButton->setToolTip(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\345\275\223\345\211\215\351\205\215\347\275\256\345\210\260\346\226\207\344\273\266", nullptr));
#endif // QT_CONFIG(tooltip)
        saveConfigButton->setText(QCoreApplication::translate("MainWindow", "\360\237\222\276 \344\277\235\345\255\230\351\205\215\347\275\256", nullptr));
#if QT_CONFIG(tooltip)
        loadConfigButton->setToolTip(QCoreApplication::translate("MainWindow", "\344\273\216\346\226\207\344\273\266\345\212\240\350\275\275\351\205\215\347\275\256", nullptr));
#endif // QT_CONFIG(tooltip)
        loadConfigButton->setText(QCoreApplication::translate("MainWindow", "\360\237\223\202 \345\212\240\350\275\275\351\205\215\347\275\256", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(basicTab), QCoreApplication::translate("MainWindow", "\345\237\272\347\241\200\350\256\276\347\275\256", nullptr));
        pathConfigGroup->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\350\267\257\345\276\204\351\205\215\347\275\256", nullptr));
        configPathLabel->setText(QCoreApplication::translate("MainWindow", "\351\205\215\347\275\256\346\226\207\344\273\266\350\267\257\345\276\204:", nullptr));
        configPathEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\351\205\215\347\275\256", nullptr));
        configPathBrowseButton->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210...", nullptr));
        keywordPathLabel->setText(QCoreApplication::translate("MainWindow", "\345\205\263\351\224\256\350\257\215\345\272\223\350\267\257\345\276\204:", nullptr));
        keywordPathEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\351\205\215\347\275\256", nullptr));
        keywordPathBrowseButton->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210...", nullptr));
        logPathLabel->setText(QCoreApplication::translate("MainWindow", "\346\227\245\345\277\227\346\226\207\344\273\266\350\267\257\345\276\204:", nullptr));
        logPathEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\351\205\215\347\275\256", nullptr));
        logPathBrowseButton->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210...", nullptr));
        templatePathLabel->setText(QCoreApplication::translate("MainWindow", "\345\233\276\346\240\207\346\250\241\346\235\277\350\267\257\345\276\204:", nullptr));
        templatePathEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\351\205\215\347\275\256", nullptr));
        templatePathBrowseButton->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210...", nullptr));
        resetAllPathsButton->setText(QCoreApplication::translate("MainWindow", "\360\237\224\204 \351\207\215\347\275\256\346\211\200\346\234\211\350\267\257\345\276\204", nullptr));
        recognitionGroup->setTitle(QCoreApplication::translate("MainWindow", "\345\233\276\345\203\217\350\257\206\345\210\253\350\256\276\347\275\256", nullptr));
#if QT_CONFIG(tooltip)
        thresholdLabel->setToolTip(QCoreApplication::translate("MainWindow", "\345\233\276\345\203\217\345\214\271\351\205\215\347\232\204\347\233\270\344\274\274\345\272\246\351\230\210\345\200\274\357\274\214\345\200\274\350\266\212\351\253\230\345\214\271\351\205\215\350\266\212\344\270\245\346\240\274\357\274\214\346\216\250\350\215\2200.7-0.9", nullptr));
#endif // QT_CONFIG(tooltip)
        thresholdLabel->setText(QCoreApplication::translate("MainWindow", "\350\257\206\345\210\253\351\230\210\345\200\274:", nullptr));
#if QT_CONFIG(tooltip)
        thresholdSpin->setToolTip(QCoreApplication::translate("MainWindow", "\345\233\276\345\203\217\345\214\271\351\205\215\347\232\204\347\233\270\344\274\274\345\272\246\351\230\210\345\200\274\357\274\214\345\200\274\350\266\212\351\253\230\345\214\271\351\205\215\350\266\212\344\270\245\346\240\274\357\274\214\346\216\250\350\215\2200.7-0.9", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        attemptsLabel->setToolTip(QCoreApplication::translate("MainWindow", "\346\257\217\346\254\241\350\257\206\345\210\253\345\244\261\350\264\245\345\220\216\347\232\204\351\207\215\350\257\225\346\254\241\346\225\260\357\274\214\345\242\236\345\212\240\346\254\241\346\225\260\345\217\257\346\217\220\351\253\230\350\257\206\345\210\253\346\210\220\345\212\237\347\216\207\357\274\214\344\275\206\344\274\232\345\242\236\345\212\240\350\200\227\346\227\266", nullptr));
#endif // QT_CONFIG(tooltip)
        attemptsLabel->setText(QCoreApplication::translate("MainWindow", "\346\234\200\345\244\247\345\260\235\350\257\225\346\254\241\346\225\260:", nullptr));
#if QT_CONFIG(tooltip)
        attemptsSpin->setToolTip(QCoreApplication::translate("MainWindow", "\346\257\217\346\254\241\350\257\206\345\210\253\345\244\261\350\264\245\345\220\216\347\232\204\351\207\215\350\257\225\346\254\241\346\225\260\357\274\214\345\242\236\345\212\240\346\254\241\346\225\260\345\217\257\346\217\220\351\253\230\350\257\206\345\210\253\346\210\220\345\212\237\347\216\207\357\274\214\344\275\206\344\274\232\345\242\236\345\212\240\350\200\227\346\227\266", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        pageTimeoutLabel->setToolTip(QCoreApplication::translate("MainWindow", "\347\255\211\345\276\205\351\241\265\351\235\242\345\205\203\347\264\240\345\212\240\350\275\275\345\256\214\346\210\220\347\232\204\346\234\200\345\244\247\346\227\266\351\227\264\357\274\214\346\240\271\346\215\256\347\275\221\347\273\234\346\203\205\345\206\265\350\260\203\346\225\264", nullptr));
#endif // QT_CONFIG(tooltip)
        pageTimeoutLabel->setText(QCoreApplication::translate("MainWindow", "\351\241\265\351\235\242\345\212\240\350\275\275\350\266\205\346\227\266(\346\257\253\347\247\222):", nullptr));
#if QT_CONFIG(tooltip)
        pageTimeoutSpin->setToolTip(QCoreApplication::translate("MainWindow", "\347\255\211\345\276\205\351\241\265\351\235\242\345\205\203\347\264\240\345\212\240\350\275\275\345\256\214\346\210\220\347\232\204\346\234\200\345\244\247\346\227\266\351\227\264\357\274\214\346\240\271\346\215\256\347\275\221\347\273\234\346\203\205\345\206\265\350\260\203\346\225\264", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        recognitionTimeoutLabel->setToolTip(QCoreApplication::translate("MainWindow", "\345\215\225\346\254\241\345\233\276\345\203\217\350\257\206\345\210\253\347\232\204\346\234\200\345\244\247\350\200\227\346\227\266\357\274\214\351\230\262\346\255\242\350\257\206\345\210\253\350\277\207\347\250\213\345\215\241\346\255\273", nullptr));
#endif // QT_CONFIG(tooltip)
        recognitionTimeoutLabel->setText(QCoreApplication::translate("MainWindow", "\350\257\206\345\210\253\350\266\205\346\227\266(\346\257\253\347\247\222):", nullptr));
#if QT_CONFIG(tooltip)
        recognitionTimeoutSpin->setToolTip(QCoreApplication::translate("MainWindow", "\345\215\225\346\254\241\345\233\276\345\203\217\350\257\206\345\210\253\347\232\204\346\234\200\345\244\247\350\200\227\346\227\266\357\274\214\351\230\262\346\255\242\350\257\206\345\210\253\350\277\207\347\250\213\345\215\241\346\255\273", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        recognitionTechniqueLabel->setToolTip(QCoreApplication::translate("MainWindow", "\344\275\277\347\224\250NCC\345\275\222\344\270\200\345\214\226\344\272\244\345\217\211\347\233\270\345\205\263\345\214\271\351\205\215\347\256\227\346\263\225\350\277\233\350\241\214\345\233\276\345\203\217\350\257\206\345\210\253", nullptr));
#endif // QT_CONFIG(tooltip)
        recognitionTechniqueLabel->setText(QCoreApplication::translate("MainWindow", "\350\257\206\345\210\253\346\212\200\346\234\257: NCC - \345\275\222\344\270\200\345\214\226\344\272\244\345\217\211\347\233\270\345\205\263\345\214\271\351\205\215", nullptr));
        monitorGroup->setTitle(QCoreApplication::translate("MainWindow", "\345\244\232\346\230\276\347\244\272\345\231\250\350\256\276\347\275\256", nullptr));
#if QT_CONFIG(tooltip)
        multiMonitorCheck->setToolTip(QCoreApplication::translate("MainWindow", "\345\275\223\344\275\277\347\224\250\345\244\232\344\270\252\346\230\276\347\244\272\345\231\250\346\227\266\345\220\257\347\224\250\357\274\214\345\217\257\346\214\207\345\256\232\344\274\201\344\270\232\345\276\256\344\277\241\346\211\200\345\234\250\347\232\204\346\230\276\347\244\272\345\231\250", nullptr));
#endif // QT_CONFIG(tooltip)
        multiMonitorCheck->setText(QCoreApplication::translate("MainWindow", "\345\220\257\347\224\250\345\244\232\346\230\276\347\244\272\345\231\250\346\224\257\346\214\201", nullptr));
#if QT_CONFIG(tooltip)
        primaryMonitorLabel->setToolTip(QCoreApplication::translate("MainWindow", "\344\274\201\344\270\232\345\276\256\344\277\241\346\211\200\345\234\250\347\232\204\346\230\276\347\244\272\345\231\250\347\264\242\345\274\225\357\274\214\344\273\2160\345\274\200\345\247\213\350\256\241\346\225\260", nullptr));
#endif // QT_CONFIG(tooltip)
        primaryMonitorLabel->setText(QCoreApplication::translate("MainWindow", "\344\270\273\346\230\276\347\244\272\345\231\250\347\264\242\345\274\225:", nullptr));
#if QT_CONFIG(tooltip)
        primaryMonitorSpin->setToolTip(QCoreApplication::translate("MainWindow", "\344\274\201\344\270\232\345\276\256\344\277\241\346\211\200\345\234\250\347\232\204\346\230\276\347\244\272\345\231\250\347\264\242\345\274\225\357\274\214\344\273\2160\345\274\200\345\247\213\350\256\241\346\225\260", nullptr));
#endif // QT_CONFIG(tooltip)
        shortcutGroup->setTitle(QCoreApplication::translate("MainWindow", "\345\277\253\346\215\267\351\224\256\350\256\276\347\275\256", nullptr));
        screenshotShortcutLabel->setText(QCoreApplication::translate("MainWindow", "\346\210\252\345\233\276\345\277\253\346\215\267\351\224\256:", nullptr));
        screenshotShortcutEdit->setKeySequence(QCoreApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
        applyShortcutButton->setText(QCoreApplication::translate("MainWindow", "\345\272\224\347\224\250", nullptr));
        closeBehaviorLabel->setText(QCoreApplication::translate("MainWindow", "\347\202\271\345\207\273\345\205\263\351\227\255\346\214\211\351\222\256\346\227\266\357\274\232", nullptr));
        closeBehaviorCombo->setItemText(0, QCoreApplication::translate("MainWindow", "\346\234\200\345\260\217\345\214\226\345\210\260\347\263\273\347\273\237\346\211\230\347\233\230", nullptr));
        closeBehaviorCombo->setItemText(1, QCoreApplication::translate("MainWindow", "\347\233\264\346\216\245\345\205\263\351\227\255", nullptr));

#if QT_CONFIG(tooltip)
        saveAdvancedConfigButton->setToolTip(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\345\275\223\345\211\215\351\253\230\347\272\247\351\205\215\347\275\256\345\210\260\346\226\207\344\273\266", nullptr));
#endif // QT_CONFIG(tooltip)
        saveAdvancedConfigButton->setText(QCoreApplication::translate("MainWindow", "\360\237\222\276 \344\277\235\345\255\230\351\205\215\347\275\256", nullptr));
#if QT_CONFIG(tooltip)
        loadAdvancedConfigButton->setToolTip(QCoreApplication::translate("MainWindow", "\344\273\216\346\226\207\344\273\266\345\212\240\350\275\275\351\253\230\347\272\247\351\205\215\347\275\256", nullptr));
#endif // QT_CONFIG(tooltip)
        loadAdvancedConfigButton->setText(QCoreApplication::translate("MainWindow", "\360\237\223\202 \345\212\240\350\275\275\351\205\215\347\275\256", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(advancedTab), QCoreApplication::translate("MainWindow", "\351\253\230\347\272\247\350\256\276\347\275\256", nullptr));
        exportLogButton->setText(QCoreApplication::translate("MainWindow", "\360\237\223\244 \345\257\274\345\207\272\346\227\245\345\277\227", nullptr));
        clearLogButton->setText(QCoreApplication::translate("MainWindow", "\360\237\227\221\357\270\217 \346\270\205\347\251\272\346\227\245\345\277\227", nullptr));
#if QT_CONFIG(tooltip)
        logSavePathLabel->setToolTip(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\346\227\245\345\277\227\346\226\207\344\273\266\347\232\204\344\277\235\345\255\230\350\267\257\345\276\204", nullptr));
#endif // QT_CONFIG(tooltip)
        logSavePathLabel->setText(QCoreApplication::translate("MainWindow", "\346\227\245\345\277\227\344\277\235\345\255\230\344\275\215\347\275\256: ", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(logTab), QCoreApplication::translate("MainWindow", "\346\227\245\345\277\227\347\233\221\346\216\247", nullptr));
        iconGroup->setTitle(QCoreApplication::translate("MainWindow", "\350\207\252\345\256\232\344\271\211\350\257\206\345\210\253\345\233\276\346\240\207", nullptr));
        iconNameLabel->setText(QCoreApplication::translate("MainWindow", "\345\233\276\346\240\207\345\220\215\347\247\260:", nullptr));
        iconNameCombo->setItemText(0, QCoreApplication::translate("MainWindow", "workbench - \345\267\245\344\275\234\345\217\260\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(1, QCoreApplication::translate("MainWindow", "mindspark - MindSpark\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(2, QCoreApplication::translate("MainWindow", "mindspark_small - MindSpark\345\260\217\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(3, QCoreApplication::translate("MainWindow", "mindspark_large - MindSpark\345\244\247\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(4, QCoreApplication::translate("MainWindow", "input_box - \350\276\223\345\205\245\346\241\206\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(5, QCoreApplication::translate("MainWindow", "input_box_small - \345\260\217\345\260\272\345\257\270\350\276\223\345\205\245\346\241\206\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(6, QCoreApplication::translate("MainWindow", "input_box_large - \345\244\247\345\260\272\345\257\270\350\276\223\345\205\245\346\241\206\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(7, QCoreApplication::translate("MainWindow", "send_button - \345\217\221\351\200\201\346\214\211\351\222\256\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(8, QCoreApplication::translate("MainWindow", "send_button_small - \345\217\221\351\200\201\346\214\211\351\222\256\345\260\217\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(9, QCoreApplication::translate("MainWindow", "send_button_large - \345\217\221\351\200\201\346\214\211\351\222\256\345\244\247\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(10, QCoreApplication::translate("MainWindow", "history_dialog - \345\216\206\345\217\262\345\257\271\350\257\235\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(11, QCoreApplication::translate("MainWindow", "history_dialog_small - \345\260\217\345\260\272\345\257\270\345\216\206\345\217\262\345\257\271\350\257\235\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(12, QCoreApplication::translate("MainWindow", "history_dialog_large - \345\244\247\345\260\272\345\257\270\345\216\206\345\217\262\345\257\271\350\257\235\345\233\276\346\240\207", nullptr));

        iconPathLabel_2->setText(QCoreApplication::translate("MainWindow", "\345\233\276\346\240\207\350\267\257\345\276\204:", nullptr));
        iconPathEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251\345\233\276\346\240\207\346\226\207\344\273\266", nullptr));
        browseIconButton->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210...", nullptr));
        currentIconLabel->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\345\233\276\346\240\207", nullptr));
        currentIconPreviewLabel->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\345\233\276\346\240\207\351\242\204\350\247\210", nullptr));
        selectedIconLabel->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\347\232\204\345\233\276\346\240\207", nullptr));
        selectedIconPreviewLabel->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\347\232\204\345\233\276\346\240\207\351\242\204\350\247\210", nullptr));
        saveIconButton->setText(QCoreApplication::translate("MainWindow", "\360\237\222\276 \344\277\235\345\255\230\345\233\276\346\240\207\351\205\215\347\275\256", nullptr));
        resetIconButton->setText(QCoreApplication::translate("MainWindow", "\360\237\224\204 \351\207\215\347\275\256\344\270\272\351\273\230\350\256\244", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(iconTab), QCoreApplication::translate("MainWindow", "\345\233\276\346\240\207\347\256\241\347\220\206", nullptr));
        supportGroup->setTitle(QCoreApplication::translate("MainWindow", "\346\224\257\346\214\201\346\210\221\344\273\254", nullptr));
        donationTextLabel->setText(QCoreApplication::translate("MainWindow", "\345\246\202\346\236\234\346\202\250\350\247\211\345\276\227WeBot\345\257\271\346\202\250\346\234\211\345\270\256\345\212\251\357\274\214\346\254\242\350\277\216\351\200\232\350\277\207\344\273\245\344\270\213\346\226\271\345\274\217\346\224\257\346\214\201\346\210\221\344\273\254\347\232\204\345\274\200\345\217\221\357\274\232", nullptr));
        wechatQrLabel->setText(QCoreApplication::translate("MainWindow", "\345\276\256\344\277\241\346\224\257\344\273\230", nullptr));
        wechatQrCodeLabel->setText(QCoreApplication::translate("MainWindow", "\345\276\256\344\277\241\344\272\214\347\273\264\347\240\201", nullptr));
        alipayQrLabel->setText(QCoreApplication::translate("MainWindow", "\346\224\257\344\273\230\345\256\235", nullptr));
        alipayQrCodeLabel->setText(QCoreApplication::translate("MainWindow", "\346\224\257\344\273\230\345\256\235\344\272\214\347\273\264\347\240\201", nullptr));
        authorGroup->setTitle(QCoreApplication::translate("MainWindow", "\344\275\234\350\200\205\344\277\241\346\201\257", nullptr));
        authorNameLabel->setText(QCoreApplication::translate("MainWindow", "\344\275\234\350\200\205: SLin", nullptr));
        authorEmailLabel->setText(QCoreApplication::translate("MainWindow", "\351\202\256\347\256\261: 905658840@qq.com", nullptr));
        authorWebsiteLabel->setText(QCoreApplication::translate("MainWindow", "\347\275\221\347\253\231: https://github.com/linyvhuo/WeBot", nullptr));
        visitWebsiteButton->setText(QCoreApplication::translate("MainWindow", "\360\237\214\220 \350\256\277\351\227\256\345\256\230\347\275\221", nullptr));
        copyrightLabel->setText(QCoreApplication::translate("MainWindow", "\302\251 2025 WeBot\345\274\200\345\217\221\350\200\205 \347\211\210\346\235\203\346\211\200\346\234\211", nullptr));
        versionLabel->setText(QCoreApplication::translate("MainWindow", "\347\211\210\346\234\254: v1.1.2", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(aboutTab), QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216\344\275\234\350\200\205", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
