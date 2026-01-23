/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
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
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
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
    QLabel *titleLabel;
    QTabWidget *tabWidget;
    QWidget *basicTab;
    QVBoxLayout *basicLayout;
    QGroupBox *wechatGroup;
    QGridLayout *wechatLayout;
    QLabel *wechatPathLabel;
    QLineEdit *wechatPathEdit;
    QPushButton *browseWechatButton;
    QCheckBox *topMostCheck;
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
    QHBoxLayout *buttonLayout;
    QPushButton *startButton;
    QPushButton *stopButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *saveConfigButton;
    QPushButton *loadConfigButton;
    QWidget *advancedTab;
    QVBoxLayout *advancedLayout;
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
    QGroupBox *fixedCoordinatesGroup;
    QGridLayout *fixedCoordinatesLayout;
    QCheckBox *useImageRecognitionCheck;
    QCheckBox *useFixedCoordinatesCheck;
    QLabel *workbenchXLabel;
    QSpinBox *workbenchXSpin;
    QSpinBox *workbenchYSpin;
    QPushButton *selectWorkbenchPosButton;
    QLabel *mindSparkXLabel;
    QSpinBox *mindSparkXSpin;
    QSpinBox *mindSparkYSpin;
    QPushButton *selectMindSparkPosButton;
    QGroupBox *monitorGroup;
    QGridLayout *monitorLayout;
    QCheckBox *multiMonitorCheck;
    QLabel *primaryMonitorLabel;
    QSpinBox *primaryMonitorSpin;
    QHBoxLayout *advancedButtonLayout;
    QSpacerItem *advancedButtonSpacer;
    QPushButton *saveAdvancedConfigButton;
    QPushButton *loadAdvancedConfigButton;
    QSpacerItem *advancedSpacer;
    QWidget *logTab;
    QVBoxLayout *logLayout;
    QHBoxLayout *logButtonLayout;
    QPushButton *exportLogButton;
    QPushButton *clearLogButton;
    QPushButton *testCopyPasteButton;
    QSpacerItem *logSpacer;
    QTextEdit *logTextEdit;
    QWidget *iconTab;
    QVBoxLayout *iconLayout;
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
    QSpacerItem *iconTabSpacer;
    QWidget *aboutTab;
    QVBoxLayout *aboutLayout;
    QLabel *aboutTitleLabel;
    QGroupBox *authorGroup;
    QVBoxLayout *authorLayout;
    QLabel *authorNameLabel;
    QLabel *authorEmailLabel;
    QLabel *authorWebsiteLabel;
    QLabel *copyrightLabel;
    QGroupBox *donationGroup;
    QVBoxLayout *donationLayout;
    QLabel *donationTextLabel;
    QHBoxLayout *qrCodeLayout;
    QVBoxLayout *wechatQrLayout;
    QLabel *wechatQrLabel;
    QLabel *wechatQrCodeLabel;
    QVBoxLayout *alipayQrLayout;
    QLabel *alipayQrLabel;
    QLabel *alipayQrCodeLabel;
    QGroupBox *versionGroup;
    QVBoxLayout *versionLayout;
    QLabel *versionLabel;
    QLabel *descriptionLabel;
    QSpacerItem *aboutSpacer;
    QHBoxLayout *statusLayout;
    QProgressBar *progressBar;
    QLabel *progressLabel;
    QLabel *statusLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 884);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/app_icon.svg"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QVBoxLayout(centralwidget);
        mainLayout->setObjectName("mainLayout");
        titleLabel = new QLabel(centralwidget);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 20pt; font-weight: bold; color: #2c3e50; margin: 10px;"));
        titleLabel->setAlignment(Qt::AlignCenter);

        mainLayout->addWidget(titleLabel);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        basicTab = new QWidget();
        basicTab->setObjectName("basicTab");
        basicLayout = new QVBoxLayout(basicTab);
        basicLayout->setObjectName("basicLayout");
        wechatGroup = new QGroupBox(basicTab);
        wechatGroup->setObjectName("wechatGroup");
        wechatLayout = new QGridLayout(wechatGroup);
        wechatLayout->setObjectName("wechatLayout");
        wechatPathLabel = new QLabel(wechatGroup);
        wechatPathLabel->setObjectName("wechatPathLabel");

        wechatLayout->addWidget(wechatPathLabel, 0, 0, 1, 1);

        wechatPathEdit = new QLineEdit(wechatGroup);
        wechatPathEdit->setObjectName("wechatPathEdit");

        wechatLayout->addWidget(wechatPathEdit, 0, 1, 1, 1);

        browseWechatButton = new QPushButton(wechatGroup);
        browseWechatButton->setObjectName("browseWechatButton");
        browseWechatButton->setMaximumSize(QSize(80, 16777215));

        wechatLayout->addWidget(browseWechatButton, 0, 2, 1, 1);

        topMostCheck = new QCheckBox(wechatGroup);
        topMostCheck->setObjectName("topMostCheck");

        wechatLayout->addWidget(topMostCheck, 1, 0, 1, 3);


        basicLayout->addWidget(wechatGroup);

        questionGroup = new QGroupBox(basicTab);
        questionGroup->setObjectName("questionGroup");
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
        browseQuestionsButton->setMaximumSize(QSize(80, 16777215));
        browseQuestionsButton->setStyleSheet(QString::fromUtf8("font-size: 10pt; background-color: #607D8B; color: white; border-radius: 4px; padding: 4px 8px; border: none;\n"
"QPushButton:pressed { background-color: #455A64; transform: translateY(1px); }"));

        questionLayout->addWidget(browseQuestionsButton, 0, 2, 1, 1);

        questionModeLabel = new QLabel(questionGroup);
        questionModeLabel->setObjectName("questionModeLabel");

        questionLayout->addWidget(questionModeLabel, 1, 0, 1, 1);

        questionModeCombo = new QComboBox(questionGroup);
        questionModeCombo->addItem(QString());
        questionModeCombo->addItem(QString());
        questionModeCombo->addItem(QString());
        questionModeCombo->setObjectName("questionModeCombo");

        questionLayout->addWidget(questionModeCombo, 1, 1, 1, 1);

        promptLabel = new QLabel(questionGroup);
        promptLabel->setObjectName("promptLabel");

        questionLayout->addWidget(promptLabel, 2, 0, 1, 1);

        promptEdit = new QLineEdit(questionGroup);
        promptEdit->setObjectName("promptEdit");

        questionLayout->addWidget(promptEdit, 2, 1, 1, 2);


        basicLayout->addWidget(questionGroup);

        automationGroup = new QGroupBox(basicTab);
        automationGroup->setObjectName("automationGroup");
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
        timeoutSpin->setMinimum(5);
        timeoutSpin->setMaximum(300);
        timeoutSpin->setValue(30);

        automationLayout->addWidget(timeoutSpin, 0, 3, 1, 1);

        delayLabel = new QLabel(automationGroup);
        delayLabel->setObjectName("delayLabel");

        automationLayout->addWidget(delayLabel, 1, 0, 1, 1);

        delaySpin = new QSpinBox(automationGroup);
        delaySpin->setObjectName("delaySpin");
        delaySpin->setMinimum(1);
        delaySpin->setMaximum(60);
        delaySpin->setValue(5);

        automationLayout->addWidget(delaySpin, 1, 1, 1, 1);

        continueOnErrorCheck = new QCheckBox(automationGroup);
        continueOnErrorCheck->setObjectName("continueOnErrorCheck");
        continueOnErrorCheck->setChecked(true);

        automationLayout->addWidget(continueOnErrorCheck, 1, 2, 1, 2);

        continueOnTimeoutCheck = new QCheckBox(automationGroup);
        continueOnTimeoutCheck->setObjectName("continueOnTimeoutCheck");
        continueOnTimeoutCheck->setChecked(true);

        automationLayout->addWidget(continueOnTimeoutCheck, 2, 0, 1, 4);


        basicLayout->addWidget(automationGroup);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        startButton = new QPushButton(basicTab);
        startButton->setObjectName("startButton");
        startButton->setMinimumSize(QSize(150, 40));

        buttonLayout->addWidget(startButton);

        stopButton = new QPushButton(basicTab);
        stopButton->setObjectName("stopButton");
        stopButton->setEnabled(false);
        stopButton->setMinimumSize(QSize(120, 40));

        buttonLayout->addWidget(stopButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        saveConfigButton = new QPushButton(basicTab);
        saveConfigButton->setObjectName("saveConfigButton");
        saveConfigButton->setMinimumSize(QSize(100, 35));

        buttonLayout->addWidget(saveConfigButton);

        loadConfigButton = new QPushButton(basicTab);
        loadConfigButton->setObjectName("loadConfigButton");
        loadConfigButton->setMinimumSize(QSize(100, 35));

        buttonLayout->addWidget(loadConfigButton);


        basicLayout->addLayout(buttonLayout);

        tabWidget->addTab(basicTab, QString());
        advancedTab = new QWidget();
        advancedTab->setObjectName("advancedTab");
        advancedLayout = new QVBoxLayout(advancedTab);
        advancedLayout->setObjectName("advancedLayout");
        recognitionGroup = new QGroupBox(advancedTab);
        recognitionGroup->setObjectName("recognitionGroup");
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
        pageTimeoutSpin->setMinimum(1000);
        pageTimeoutSpin->setMaximum(10000);
        pageTimeoutSpin->setValue(2000);

        recognitionLayout->addWidget(pageTimeoutSpin, 1, 1, 1, 1);

        recognitionTimeoutLabel = new QLabel(recognitionGroup);
        recognitionTimeoutLabel->setObjectName("recognitionTimeoutLabel");

        recognitionLayout->addWidget(recognitionTimeoutLabel, 1, 2, 1, 1);

        recognitionTimeoutSpin = new QSpinBox(recognitionGroup);
        recognitionTimeoutSpin->setObjectName("recognitionTimeoutSpin");
        recognitionTimeoutSpin->setMinimum(1000);
        recognitionTimeoutSpin->setMaximum(10000);
        recognitionTimeoutSpin->setValue(3000);

        recognitionLayout->addWidget(recognitionTimeoutSpin, 1, 3, 1, 1);

        recognitionTechniqueLabel = new QLabel(recognitionGroup);
        recognitionTechniqueLabel->setObjectName("recognitionTechniqueLabel");

        recognitionLayout->addWidget(recognitionTechniqueLabel, 2, 0, 1, 4);


        advancedLayout->addWidget(recognitionGroup);

        fixedCoordinatesGroup = new QGroupBox(advancedTab);
        fixedCoordinatesGroup->setObjectName("fixedCoordinatesGroup");
        fixedCoordinatesLayout = new QGridLayout(fixedCoordinatesGroup);
        fixedCoordinatesLayout->setObjectName("fixedCoordinatesLayout");
        useImageRecognitionCheck = new QCheckBox(fixedCoordinatesGroup);
        useImageRecognitionCheck->setObjectName("useImageRecognitionCheck");
        useImageRecognitionCheck->setChecked(true);

        fixedCoordinatesLayout->addWidget(useImageRecognitionCheck, 0, 0, 1, 3);

        useFixedCoordinatesCheck = new QCheckBox(fixedCoordinatesGroup);
        useFixedCoordinatesCheck->setObjectName("useFixedCoordinatesCheck");

        fixedCoordinatesLayout->addWidget(useFixedCoordinatesCheck, 1, 0, 1, 3);

        workbenchXLabel = new QLabel(fixedCoordinatesGroup);
        workbenchXLabel->setObjectName("workbenchXLabel");

        fixedCoordinatesLayout->addWidget(workbenchXLabel, 2, 0, 1, 1);

        workbenchXSpin = new QSpinBox(fixedCoordinatesGroup);
        workbenchXSpin->setObjectName("workbenchXSpin");
        workbenchXSpin->setMinimum(0);
        workbenchXSpin->setMaximum(3840);
        workbenchXSpin->setValue(100);

        fixedCoordinatesLayout->addWidget(workbenchXSpin, 2, 1, 1, 1);

        workbenchYSpin = new QSpinBox(fixedCoordinatesGroup);
        workbenchYSpin->setObjectName("workbenchYSpin");
        workbenchYSpin->setMinimum(0);
        workbenchYSpin->setMaximum(2160);
        workbenchYSpin->setValue(100);

        fixedCoordinatesLayout->addWidget(workbenchYSpin, 2, 2, 1, 1);

        selectWorkbenchPosButton = new QPushButton(fixedCoordinatesGroup);
        selectWorkbenchPosButton->setObjectName("selectWorkbenchPosButton");

        fixedCoordinatesLayout->addWidget(selectWorkbenchPosButton, 3, 0, 1, 3);

        mindSparkXLabel = new QLabel(fixedCoordinatesGroup);
        mindSparkXLabel->setObjectName("mindSparkXLabel");

        fixedCoordinatesLayout->addWidget(mindSparkXLabel, 4, 0, 1, 1);

        mindSparkXSpin = new QSpinBox(fixedCoordinatesGroup);
        mindSparkXSpin->setObjectName("mindSparkXSpin");
        mindSparkXSpin->setMinimum(0);
        mindSparkXSpin->setMaximum(3840);
        mindSparkXSpin->setValue(500);

        fixedCoordinatesLayout->addWidget(mindSparkXSpin, 4, 1, 1, 1);

        mindSparkYSpin = new QSpinBox(fixedCoordinatesGroup);
        mindSparkYSpin->setObjectName("mindSparkYSpin");
        mindSparkYSpin->setMinimum(0);
        mindSparkYSpin->setMaximum(2160);
        mindSparkYSpin->setValue(200);

        fixedCoordinatesLayout->addWidget(mindSparkYSpin, 4, 2, 1, 1);

        selectMindSparkPosButton = new QPushButton(fixedCoordinatesGroup);
        selectMindSparkPosButton->setObjectName("selectMindSparkPosButton");

        fixedCoordinatesLayout->addWidget(selectMindSparkPosButton, 5, 0, 1, 3);


        advancedLayout->addWidget(fixedCoordinatesGroup);

        monitorGroup = new QGroupBox(advancedTab);
        monitorGroup->setObjectName("monitorGroup");
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


        advancedLayout->addWidget(monitorGroup);

        advancedButtonLayout = new QHBoxLayout();
        advancedButtonLayout->setObjectName("advancedButtonLayout");
        advancedButtonSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        advancedButtonLayout->addItem(advancedButtonSpacer);

        saveAdvancedConfigButton = new QPushButton(advancedTab);
        saveAdvancedConfigButton->setObjectName("saveAdvancedConfigButton");
        saveAdvancedConfigButton->setMinimumSize(QSize(100, 35));

        advancedButtonLayout->addWidget(saveAdvancedConfigButton);

        loadAdvancedConfigButton = new QPushButton(advancedTab);
        loadAdvancedConfigButton->setObjectName("loadAdvancedConfigButton");
        loadAdvancedConfigButton->setMinimumSize(QSize(100, 35));

        advancedButtonLayout->addWidget(loadAdvancedConfigButton);


        advancedLayout->addLayout(advancedButtonLayout);

        advancedSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        advancedLayout->addItem(advancedSpacer);

        tabWidget->addTab(advancedTab, QString());
        logTab = new QWidget();
        logTab->setObjectName("logTab");
        logLayout = new QVBoxLayout(logTab);
        logLayout->setObjectName("logLayout");
        logButtonLayout = new QHBoxLayout();
        logButtonLayout->setObjectName("logButtonLayout");
        exportLogButton = new QPushButton(logTab);
        exportLogButton->setObjectName("exportLogButton");
        exportLogButton->setMinimumSize(QSize(120, 35));

        logButtonLayout->addWidget(exportLogButton);

        clearLogButton = new QPushButton(logTab);
        clearLogButton->setObjectName("clearLogButton");
        clearLogButton->setMinimumSize(QSize(120, 35));

        logButtonLayout->addWidget(clearLogButton);

        testCopyPasteButton = new QPushButton(logTab);
        testCopyPasteButton->setObjectName("testCopyPasteButton");
        testCopyPasteButton->setMinimumSize(QSize(150, 35));

        logButtonLayout->addWidget(testCopyPasteButton);

        logSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        logButtonLayout->addItem(logSpacer);


        logLayout->addLayout(logButtonLayout);

        logTextEdit = new QTextEdit(logTab);
        logTextEdit->setObjectName("logTextEdit");
        logTextEdit->setStyleSheet(QString::fromUtf8("font-family: 'Consolas', 'Monaco', monospace; font-size: 9pt; background-color: #2c3e50; color: #ecf0f1;"));
        logTextEdit->setReadOnly(true);

        logLayout->addWidget(logTextEdit);

        tabWidget->addTab(logTab, QString());
        iconTab = new QWidget();
        iconTab->setObjectName("iconTab");
        iconLayout = new QVBoxLayout(iconTab);
        iconLayout->setObjectName("iconLayout");
        iconGroup = new QGroupBox(iconTab);
        iconGroup->setObjectName("iconGroup");
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
        iconNameCombo->setObjectName("iconNameCombo");

        iconGridLayout->addWidget(iconNameCombo, 0, 1, 1, 1);

        iconPathLabel_2 = new QLabel(iconGroup);
        iconPathLabel_2->setObjectName("iconPathLabel_2");

        iconGridLayout->addWidget(iconPathLabel_2, 1, 0, 1, 1);

        iconPathEdit = new QLineEdit(iconGroup);
        iconPathEdit->setObjectName("iconPathEdit");

        iconGridLayout->addWidget(iconPathEdit, 1, 1, 1, 1);

        browseIconButton = new QPushButton(iconGroup);
        browseIconButton->setObjectName("browseIconButton");
        browseIconButton->setMaximumSize(QSize(80, 16777215));
        browseIconButton->setStyleSheet(QString::fromUtf8("font-size: 10pt; background-color: #607D8B; color: white; border-radius: 4px; padding: 4px 8px; border: none;\n"
"QPushButton:pressed { background-color: #455A64; transform: translateY(1px); }"));

        iconGridLayout->addWidget(browseIconButton, 1, 2, 1, 1);

        iconPreviewLayout = new QHBoxLayout();
        iconPreviewLayout->setObjectName("iconPreviewLayout");
        currentIconLayout = new QVBoxLayout();
        currentIconLayout->setObjectName("currentIconLayout");
        currentIconLabel = new QLabel(iconGroup);
        currentIconLabel->setObjectName("currentIconLabel");
        currentIconLabel->setAlignment(Qt::AlignCenter);

        currentIconLayout->addWidget(currentIconLabel);

        currentIconPreviewLabel = new QLabel(iconGroup);
        currentIconPreviewLabel->setObjectName("currentIconPreviewLabel");
        currentIconPreviewLabel->setEnabled(true);
        currentIconPreviewLabel->setMinimumSize(QSize(150, 100));
        currentIconPreviewLabel->setMaximumSize(QSize(150, 100));
        currentIconPreviewLabel->setStyleSheet(QString::fromUtf8("background-color: #f0f0f0; border: 1px solid #ddd; border-radius: 4px;"));
        currentIconPreviewLabel->setAlignment(Qt::AlignCenter);

        currentIconLayout->addWidget(currentIconPreviewLabel, 0, Qt::AlignHCenter);


        iconPreviewLayout->addLayout(currentIconLayout);

        selectedIconLayout = new QVBoxLayout();
        selectedIconLayout->setObjectName("selectedIconLayout");
        selectedIconLabel = new QLabel(iconGroup);
        selectedIconLabel->setObjectName("selectedIconLabel");
        selectedIconLabel->setAlignment(Qt::AlignCenter);

        selectedIconLayout->addWidget(selectedIconLabel);

        selectedIconPreviewLabel = new QLabel(iconGroup);
        selectedIconPreviewLabel->setObjectName("selectedIconPreviewLabel");
        selectedIconPreviewLabel->setMinimumSize(QSize(150, 100));
        selectedIconPreviewLabel->setMaximumSize(QSize(150, 100));
        selectedIconPreviewLabel->setStyleSheet(QString::fromUtf8("background-color: #f0f0f0; border: 1px solid #ddd; border-radius: 4px;"));
        selectedIconPreviewLabel->setAlignment(Qt::AlignCenter);

        selectedIconLayout->addWidget(selectedIconPreviewLabel, 0, Qt::AlignHCenter);


        iconPreviewLayout->addLayout(selectedIconLayout);


        iconGridLayout->addLayout(iconPreviewLayout, 2, 0, 1, 3);

        iconButtonLayout = new QHBoxLayout();
        iconButtonLayout->setObjectName("iconButtonLayout");
        saveIconButton = new QPushButton(iconGroup);
        saveIconButton->setObjectName("saveIconButton");
        saveIconButton->setMinimumSize(QSize(120, 35));

        iconButtonLayout->addWidget(saveIconButton);

        resetIconButton = new QPushButton(iconGroup);
        resetIconButton->setObjectName("resetIconButton");
        resetIconButton->setMinimumSize(QSize(120, 35));

        iconButtonLayout->addWidget(resetIconButton);

        iconSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        iconButtonLayout->addItem(iconSpacer);


        iconGridLayout->addLayout(iconButtonLayout, 3, 0, 1, 3);


        iconLayout->addWidget(iconGroup);

        iconTabSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        iconLayout->addItem(iconTabSpacer);

        tabWidget->addTab(iconTab, QString());
        aboutTab = new QWidget();
        aboutTab->setObjectName("aboutTab");
        aboutLayout = new QVBoxLayout(aboutTab);
        aboutLayout->setObjectName("aboutLayout");
        aboutTitleLabel = new QLabel(aboutTab);
        aboutTitleLabel->setObjectName("aboutTitleLabel");
        aboutTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 16pt; font-weight: bold; color: #00ffff; margin: 20px;"));
        aboutTitleLabel->setAlignment(Qt::AlignCenter);

        aboutLayout->addWidget(aboutTitleLabel);

        authorGroup = new QGroupBox(aboutTab);
        authorGroup->setObjectName("authorGroup");
        authorLayout = new QVBoxLayout(authorGroup);
        authorLayout->setObjectName("authorLayout");
        authorNameLabel = new QLabel(authorGroup);
        authorNameLabel->setObjectName("authorNameLabel");
        authorNameLabel->setStyleSheet(QString::fromUtf8("font-size: 11pt; margin: 5px;"));

        authorLayout->addWidget(authorNameLabel);

        authorEmailLabel = new QLabel(authorGroup);
        authorEmailLabel->setObjectName("authorEmailLabel");
        authorEmailLabel->setStyleSheet(QString::fromUtf8("font-size: 11pt; margin: 5px;"));

        authorLayout->addWidget(authorEmailLabel);

        authorWebsiteLabel = new QLabel(authorGroup);
        authorWebsiteLabel->setObjectName("authorWebsiteLabel");
        authorWebsiteLabel->setStyleSheet(QString::fromUtf8("font-size: 11pt; margin: 5px; color: #00ffff;"));
        authorWebsiteLabel->setOpenExternalLinks(true);

        authorLayout->addWidget(authorWebsiteLabel);

        copyrightLabel = new QLabel(authorGroup);
        copyrightLabel->setObjectName("copyrightLabel");
        copyrightLabel->setStyleSheet(QString::fromUtf8("font-size: 10pt; margin: 10px; color: #888;"));
        copyrightLabel->setAlignment(Qt::AlignCenter);

        authorLayout->addWidget(copyrightLabel);


        aboutLayout->addWidget(authorGroup);

        donationGroup = new QGroupBox(aboutTab);
        donationGroup->setObjectName("donationGroup");
        donationLayout = new QVBoxLayout(donationGroup);
        donationLayout->setObjectName("donationLayout");
        donationTextLabel = new QLabel(donationGroup);
        donationTextLabel->setObjectName("donationTextLabel");
        donationTextLabel->setStyleSheet(QString::fromUtf8("font-size: 11pt; margin: 10px;"));
        donationTextLabel->setWordWrap(true);

        donationLayout->addWidget(donationTextLabel);

        qrCodeLayout = new QHBoxLayout();
        qrCodeLayout->setObjectName("qrCodeLayout");
        wechatQrLayout = new QVBoxLayout();
        wechatQrLayout->setObjectName("wechatQrLayout");
        wechatQrLabel = new QLabel(donationGroup);
        wechatQrLabel->setObjectName("wechatQrLabel");
        wechatQrLabel->setStyleSheet(QString::fromUtf8("font-size: 11pt; margin: 5px; font-weight: bold;"));
        wechatQrLabel->setAlignment(Qt::AlignCenter);

        wechatQrLayout->addWidget(wechatQrLabel);

        wechatQrCodeLabel = new QLabel(donationGroup);
        wechatQrCodeLabel->setObjectName("wechatQrCodeLabel");
        wechatQrCodeLabel->setMinimumSize(QSize(150, 150));
        wechatQrCodeLabel->setMaximumSize(QSize(150, 150));
        wechatQrCodeLabel->setStyleSheet(QString::fromUtf8("background-color: #ffffff; border: 2px solid #00ffff; border-radius: 8px; padding: 5px;"));
        wechatQrCodeLabel->setAlignment(Qt::AlignCenter);

        wechatQrLayout->addWidget(wechatQrCodeLabel);


        qrCodeLayout->addLayout(wechatQrLayout);

        alipayQrLayout = new QVBoxLayout();
        alipayQrLayout->setObjectName("alipayQrLayout");
        alipayQrLabel = new QLabel(donationGroup);
        alipayQrLabel->setObjectName("alipayQrLabel");
        alipayQrLabel->setStyleSheet(QString::fromUtf8("font-size: 11pt; margin: 5px; font-weight: bold;"));
        alipayQrLabel->setAlignment(Qt::AlignCenter);

        alipayQrLayout->addWidget(alipayQrLabel);

        alipayQrCodeLabel = new QLabel(donationGroup);
        alipayQrCodeLabel->setObjectName("alipayQrCodeLabel");
        alipayQrCodeLabel->setMinimumSize(QSize(150, 150));
        alipayQrCodeLabel->setMaximumSize(QSize(150, 150));
        alipayQrCodeLabel->setStyleSheet(QString::fromUtf8("background-color: #ffffff; border: 2px solid #00ffff; border-radius: 8px; padding: 5px;"));
        alipayQrCodeLabel->setAlignment(Qt::AlignCenter);

        alipayQrLayout->addWidget(alipayQrCodeLabel);


        qrCodeLayout->addLayout(alipayQrLayout);


        donationLayout->addLayout(qrCodeLayout);


        aboutLayout->addWidget(donationGroup);

        versionGroup = new QGroupBox(aboutTab);
        versionGroup->setObjectName("versionGroup");
        versionLayout = new QVBoxLayout(versionGroup);
        versionLayout->setObjectName("versionLayout");
        versionLabel = new QLabel(versionGroup);
        versionLabel->setObjectName("versionLabel");
        versionLabel->setStyleSheet(QString::fromUtf8("font-size: 11pt; margin: 5px;"));

        versionLayout->addWidget(versionLabel);

        descriptionLabel = new QLabel(versionGroup);
        descriptionLabel->setObjectName("descriptionLabel");
        descriptionLabel->setStyleSheet(QString::fromUtf8("font-size: 10pt; margin: 5px; color: #aaaaaa;"));
        descriptionLabel->setWordWrap(true);

        versionLayout->addWidget(descriptionLabel);


        aboutLayout->addWidget(versionGroup);

        aboutSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        aboutLayout->addItem(aboutSpacer);

        tabWidget->addTab(aboutTab, QString());

        mainLayout->addWidget(tabWidget);

        statusLayout = new QHBoxLayout();
        statusLayout->setObjectName("statusLayout");
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName("progressBar");
        progressBar->setValue(0);
        progressBar->setTextVisible(true);

        statusLayout->addWidget(progressBar);

        progressLabel = new QLabel(centralwidget);
        progressLabel->setObjectName("progressLabel");
        progressLabel->setMinimumSize(QSize(120, 0));
        progressLabel->setAlignment(Qt::AlignCenter);

        statusLayout->addWidget(progressLabel);

        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setMinimumSize(QSize(150, 0));
        statusLabel->setStyleSheet(QString::fromUtf8("font-weight: bold; color: #27ae60; background-color: rgba(39, 174, 96, 0.1); border: 1px solid #27ae60; border-radius: 4px; padding: 4px 8px;"));
        statusLabel->setAlignment(Qt::AlignCenter);

        statusLayout->addWidget(statusLabel);


        mainLayout->addLayout(statusLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 24));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "WeBot - \344\274\201\344\270\232\345\276\256\344\277\241\350\207\252\345\212\250\351\227\256\347\255\224\345\267\245\345\205\267", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "\360\237\244\226 WeBot - \344\274\201\344\270\232\345\276\256\344\277\241\350\207\252\345\212\250\351\227\256\347\255\224\345\267\245\345\205\267", nullptr));
        wechatGroup->setTitle(QCoreApplication::translate("MainWindow", "\344\274\201\344\270\232\345\276\256\344\277\241\350\256\276\347\275\256", nullptr));
        wechatPathLabel->setText(QCoreApplication::translate("MainWindow", "\344\274\201\344\270\232\345\276\256\344\277\241\350\267\257\345\276\204:", nullptr));
        wechatPathEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251\344\274\201\344\270\232\345\276\256\344\277\241\345\256\211\350\243\205\350\267\257\345\276\204", nullptr));
        browseWechatButton->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210...", nullptr));
        topMostCheck->setText(QCoreApplication::translate("MainWindow", "\347\252\227\345\217\243\347\275\256\351\241\266\346\230\276\347\244\272", nullptr));
        questionGroup->setTitle(QCoreApplication::translate("MainWindow", "\351\227\256\351\242\230\350\256\276\347\275\256", nullptr));
        questionPathLabel->setText(QCoreApplication::translate("MainWindow", "\351\227\256\351\242\230\345\272\223\350\267\257\345\276\204:", nullptr));
        questionPathEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251\351\227\256\351\242\230\345\272\223\346\226\207\344\273\266", nullptr));
        browseQuestionsButton->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210...", nullptr));
        questionModeLabel->setText(QCoreApplication::translate("MainWindow", "\351\227\256\351\242\230\346\250\241\345\274\217:", nullptr));
        questionModeCombo->setItemText(0, QCoreApplication::translate("MainWindow", "\345\276\252\347\216\257\344\275\277\347\224\250\351\242\204\350\256\276\351\227\256\351\242\230", nullptr));
        questionModeCombo->setItemText(1, QCoreApplication::translate("MainWindow", "\351\232\217\346\234\272\344\275\277\347\224\250\351\242\204\350\256\276\351\227\256\351\242\230", nullptr));
        questionModeCombo->setItemText(2, QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\347\224\237\346\210\220\351\227\256\351\242\230", nullptr));

        promptLabel->setText(QCoreApplication::translate("MainWindow", "\345\233\236\347\255\224\351\231\220\345\210\266\346\217\220\347\244\272:", nullptr));
        promptEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\345\233\236\347\255\224\350\257\267\345\213\277\350\266\205\350\277\207\345\215\201\344\270\252\345\255\227", nullptr));
        automationGroup->setTitle(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\345\214\226\350\256\276\347\275\256", nullptr));
        loopCountLabel->setText(QCoreApplication::translate("MainWindow", "\345\276\252\347\216\257\346\254\241\346\225\260:", nullptr));
        timeoutLabel->setText(QCoreApplication::translate("MainWindow", "\345\233\236\347\255\224\350\266\205\346\227\266(\347\247\222):", nullptr));
        delayLabel->setText(QCoreApplication::translate("MainWindow", "\350\275\256\346\254\241\351\227\264\351\232\224(\347\247\222):", nullptr));
        continueOnErrorCheck->setText(QCoreApplication::translate("MainWindow", "\345\207\272\351\224\231\346\227\266\347\273\247\347\273\255\346\211\247\350\241\214", nullptr));
        continueOnTimeoutCheck->setText(QCoreApplication::translate("MainWindow", "\350\266\205\346\227\266\345\220\216\347\273\247\347\273\255\346\211\247\350\241\214", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "\360\237\232\200 \345\274\200\345\247\213\350\207\252\345\212\250\351\227\256\347\255\224", nullptr));
        stopButton->setText(QCoreApplication::translate("MainWindow", "\342\217\271\357\270\217 \345\201\234\346\255\242\346\211\247\350\241\214", nullptr));
#if QT_CONFIG(tooltip)
        saveConfigButton->setToolTip(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\345\275\223\345\211\215\351\205\215\347\275\256\345\210\260\346\226\207\344\273\266", nullptr));
#endif // QT_CONFIG(tooltip)
        saveConfigButton->setText(QCoreApplication::translate("MainWindow", "\360\237\222\276 \344\277\235\345\255\230\351\205\215\347\275\256", nullptr));
#if QT_CONFIG(tooltip)
        loadConfigButton->setToolTip(QCoreApplication::translate("MainWindow", "\344\273\216\346\226\207\344\273\266\345\212\240\350\275\275\351\205\215\347\275\256", nullptr));
#endif // QT_CONFIG(tooltip)
        loadConfigButton->setText(QCoreApplication::translate("MainWindow", "\360\237\223\202 \345\212\240\350\275\275\351\205\215\347\275\256", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(basicTab), QCoreApplication::translate("MainWindow", "\345\237\272\347\241\200\350\256\276\347\275\256", nullptr));
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
        fixedCoordinatesGroup->setTitle(QCoreApplication::translate("MainWindow", "\350\257\206\345\210\253\346\226\271\345\274\217\350\256\276\347\275\256", nullptr));
#if QT_CONFIG(tooltip)
        useImageRecognitionCheck->setToolTip(QCoreApplication::translate("MainWindow", "\344\275\277\347\224\250\345\233\276\345\203\217\350\257\206\345\210\253\346\237\245\346\211\276\345\205\203\347\264\240\357\274\214\351\200\202\347\224\250\344\272\216\345\244\247\345\244\232\346\225\260\346\203\205\345\206\265", nullptr));
#endif // QT_CONFIG(tooltip)
        useImageRecognitionCheck->setText(QCoreApplication::translate("MainWindow", "\345\220\257\347\224\250\345\233\276\345\203\217\350\257\206\345\210\253", nullptr));
#if QT_CONFIG(tooltip)
        useFixedCoordinatesCheck->setToolTip(QCoreApplication::translate("MainWindow", "\344\275\277\347\224\250\345\233\272\345\256\232\345\235\220\346\240\207\347\202\271\345\207\273\344\273\243\346\233\277\345\233\276\345\203\217\350\257\206\345\210\253\357\274\214\346\217\220\351\253\230\346\211\247\350\241\214\351\200\237\345\272\246", nullptr));
#endif // QT_CONFIG(tooltip)
        useFixedCoordinatesCheck->setText(QCoreApplication::translate("MainWindow", "\345\220\257\347\224\250\345\233\272\345\256\232\345\235\220\346\240\207\347\202\271\345\207\273", nullptr));
#if QT_CONFIG(tooltip)
        workbenchXLabel->setToolTip(QCoreApplication::translate("MainWindow", "\345\267\245\344\275\234\345\217\260\345\233\276\346\240\207\347\232\204\345\233\272\345\256\232X\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
        workbenchXLabel->setText(QCoreApplication::translate("MainWindow", "\345\267\245\344\275\234\345\217\260X\345\235\220\346\240\207:", nullptr));
#if QT_CONFIG(tooltip)
        workbenchXSpin->setToolTip(QCoreApplication::translate("MainWindow", "\345\267\245\344\275\234\345\217\260\345\233\276\346\240\207\347\232\204\345\233\272\345\256\232X\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        workbenchYSpin->setToolTip(QCoreApplication::translate("MainWindow", "\345\267\245\344\275\234\345\217\260\345\233\276\346\240\207\347\232\204\345\233\272\345\256\232Y\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        selectWorkbenchPosButton->setToolTip(QCoreApplication::translate("MainWindow", "\347\202\271\345\207\273\345\261\217\345\271\225\344\270\212\347\232\204\344\275\215\347\275\256\346\235\245\350\216\267\345\217\226\345\267\245\344\275\234\345\217\260\345\233\276\346\240\207\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
        selectWorkbenchPosButton->setText(QCoreApplication::translate("MainWindow", "\347\202\271\345\207\273\351\200\211\346\213\251\345\267\245\344\275\234\345\217\260\344\275\215\347\275\256", nullptr));
#if QT_CONFIG(tooltip)
        mindSparkXLabel->setToolTip(QCoreApplication::translate("MainWindow", "MindSpark\345\233\276\346\240\207\347\232\204\345\233\272\345\256\232X\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
        mindSparkXLabel->setText(QCoreApplication::translate("MainWindow", "MindSparkX\345\235\220\346\240\207:", nullptr));
#if QT_CONFIG(tooltip)
        mindSparkXSpin->setToolTip(QCoreApplication::translate("MainWindow", "MindSpark\345\233\276\346\240\207\347\232\204\345\233\272\345\256\232X\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        mindSparkYSpin->setToolTip(QCoreApplication::translate("MainWindow", "MindSpark\345\233\276\346\240\207\347\232\204\345\233\272\345\256\232Y\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        selectMindSparkPosButton->setToolTip(QCoreApplication::translate("MainWindow", "\347\202\271\345\207\273\345\261\217\345\271\225\344\270\212\347\232\204\344\275\215\347\275\256\346\235\245\350\216\267\345\217\226MindSpark\345\233\276\346\240\207\345\235\220\346\240\207", nullptr));
#endif // QT_CONFIG(tooltip)
        selectMindSparkPosButton->setText(QCoreApplication::translate("MainWindow", "\347\202\271\345\207\273\351\200\211\346\213\251MindSpark\344\275\215\347\275\256", nullptr));
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
        testCopyPasteButton->setToolTip(QCoreApplication::translate("MainWindow", "\346\265\213\350\257\225\345\244\215\345\210\266\347\262\230\350\264\264\345\212\237\350\203\275\346\230\257\345\220\246\346\255\243\345\270\270\345\267\245\344\275\234", nullptr));
#endif // QT_CONFIG(tooltip)
        testCopyPasteButton->setText(QCoreApplication::translate("MainWindow", "\360\237\247\252 \346\265\213\350\257\225\345\244\215\345\210\266\347\262\230\350\264\264", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(logTab), QCoreApplication::translate("MainWindow", "\346\227\245\345\277\227\347\233\221\346\216\247", nullptr));
        iconGroup->setTitle(QCoreApplication::translate("MainWindow", "\350\207\252\345\256\232\344\271\211\350\257\206\345\210\253\345\233\276\346\240\207", nullptr));
        iconNameLabel->setText(QCoreApplication::translate("MainWindow", "\345\233\276\346\240\207\345\220\215\347\247\260:", nullptr));
        iconNameCombo->setItemText(0, QCoreApplication::translate("MainWindow", "workbench - \345\267\245\344\275\234\345\217\260\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(1, QCoreApplication::translate("MainWindow", "weiweisou - MindSpark\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(2, QCoreApplication::translate("MainWindow", "input_box - \350\276\223\345\205\245\346\241\206\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(3, QCoreApplication::translate("MainWindow", "input_box_small - \345\260\217\345\260\272\345\257\270\350\276\223\345\205\245\346\241\206\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(4, QCoreApplication::translate("MainWindow", "input_box_large - \345\244\247\345\260\272\345\257\270\350\276\223\345\205\245\346\241\206\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(5, QCoreApplication::translate("MainWindow", "send_button - \345\217\221\351\200\201\346\214\211\351\222\256\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(6, QCoreApplication::translate("MainWindow", "history_dialog - \345\216\206\345\217\262\345\257\271\350\257\235\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(7, QCoreApplication::translate("MainWindow", "history_dialog_small - \345\260\217\345\260\272\345\257\270\345\216\206\345\217\262\345\257\271\350\257\235\345\233\276\346\240\207", nullptr));
        iconNameCombo->setItemText(8, QCoreApplication::translate("MainWindow", "history_dialog_large - \345\244\247\345\260\272\345\257\270\345\216\206\345\217\262\345\257\271\350\257\235\345\233\276\346\240\207", nullptr));

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
        aboutTitleLabel->setText(QCoreApplication::translate("MainWindow", "\360\237\244\226 WeBot - \344\274\201\344\270\232\345\276\256\344\277\241\350\207\252\345\212\250\351\227\256\347\255\224\345\267\245\345\205\267", nullptr));
        authorGroup->setTitle(QCoreApplication::translate("MainWindow", "\344\275\234\350\200\205\344\277\241\346\201\257", nullptr));
        authorNameLabel->setText(QCoreApplication::translate("MainWindow", "\344\275\234\350\200\205: SLin", nullptr));
        authorEmailLabel->setText(QCoreApplication::translate("MainWindow", "\351\202\256\347\256\261: 905658840@qq.com", nullptr));
        authorWebsiteLabel->setText(QCoreApplication::translate("MainWindow", "\347\275\221\347\253\231: https://github.com/linyvhuo/WeBot", nullptr));
        copyrightLabel->setText(QCoreApplication::translate("MainWindow", "\302\251 2025 WeBot\345\274\200\345\217\221\350\200\205 \347\211\210\346\235\203\346\211\200\346\234\211", nullptr));
        donationGroup->setTitle(QCoreApplication::translate("MainWindow", "\346\224\257\346\214\201\346\210\221\344\273\254", nullptr));
        donationTextLabel->setText(QCoreApplication::translate("MainWindow", "\345\246\202\346\236\234\346\202\250\350\247\211\345\276\227WeBot\345\257\271\346\202\250\346\234\211\345\270\256\345\212\251\357\274\214\346\254\242\350\277\216\351\200\232\350\277\207\344\273\245\344\270\213\346\226\271\345\274\217\346\224\257\346\214\201\346\210\221\344\273\254\347\232\204\345\274\200\345\217\221\357\274\232", nullptr));
        wechatQrLabel->setText(QCoreApplication::translate("MainWindow", "\345\276\256\344\277\241\346\224\257\344\273\230", nullptr));
        wechatQrCodeLabel->setText(QCoreApplication::translate("MainWindow", "\345\276\256\344\277\241\344\272\214\347\273\264\347\240\201", nullptr));
        alipayQrLabel->setText(QCoreApplication::translate("MainWindow", "\346\224\257\344\273\230\345\256\235", nullptr));
        alipayQrCodeLabel->setText(QCoreApplication::translate("MainWindow", "\346\224\257\344\273\230\345\256\235\344\272\214\347\273\264\347\240\201", nullptr));
        versionGroup->setTitle(QCoreApplication::translate("MainWindow", "\347\211\210\346\234\254\344\277\241\346\201\257", nullptr));
        versionLabel->setText(QCoreApplication::translate("MainWindow", "\347\211\210\346\234\254: v1.0.0", nullptr));
        descriptionLabel->setText(QCoreApplication::translate("MainWindow", "WeBot\346\230\257\344\270\200\346\254\276\345\237\272\344\272\216Qt\345\222\214OpenCV\345\274\200\345\217\221\347\232\204\344\274\201\344\270\232\345\276\256\344\277\241\350\207\252\345\212\250\351\227\256\347\255\224\345\267\245\345\205\267\357\274\214\346\224\257\346\214\201\345\233\276\345\203\217\350\257\206\345\210\253\343\200\201\350\207\252\345\212\250\347\202\271\345\207\273\343\200\201\350\207\252\345\212\250\350\276\223\345\205\245\347\255\211\345\212\237\350\203\275\357\274\214\345\217\257\347\224\250\344\272\216\350\207\252\345\212\250\345\233\236\347\255\224\344\274\201\344\270\232\345\276\256\344\277\241\344\270\255\347\232\204\351\227\256\351\242\230\343\200\202", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(aboutTab), QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216\344\275\234\350\200\205", nullptr));
        progressLabel->setText(QCoreApplication::translate("MainWindow", "\350\277\233\345\272\246: 0/0", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201: \347\251\272\351\227\262", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
