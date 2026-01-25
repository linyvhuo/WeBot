/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "on_startButton_clicked",
        "",
        "on_stopButton_clicked",
        "on_browseWechatButton_clicked",
        "on_browseQuestionsButton_clicked",
        "on_saveConfigButton_clicked",
        "on_loadConfigButton_clicked",
        "on_saveAdvancedConfigButton_clicked",
        "on_loadAdvancedConfigButton_clicked",
        "on_exportLogButton_clicked",
        "on_clearLogButton_clicked",
        "on_thresholdSpin_valueChanged",
        "value",
        "on_attemptsSpin_valueChanged",
        "on_pageTimeoutSpin_valueChanged",
        "on_recognitionTimeoutSpin_valueChanged",
        "on_recognitionTechniqueCombo_currentIndexChanged",
        "index",
        "on_timeoutSpin_valueChanged",
        "on_loopCountSpin_valueChanged",
        "on_multiMonitorCheck_toggled",
        "checked",
        "on_primaryMonitorSpin_valueChanged",
        "on_keyboardInputCheck_toggled",
        "on_pasteInputCheck_toggled",
        "on_themeCombo_currentIndexChanged",
        "on_applyShortcutButton_clicked",
        "onAutomationStateChanged",
        "Automator::State",
        "state",
        "onAutomationCompleted",
        "on_browseIconButton_clicked",
        "on_saveIconButton_clicked",
        "on_resetIconButton_clicked",
        "on_iconNameCombo_currentIndexChanged",
        "on_iconPathEdit_textChanged",
        "path",
        "on_configPathBrowseButton_clicked",
        "on_questionPathBrowseButton_clicked",
        "on_keywordPathBrowseButton_clicked",
        "on_logPathBrowseButton_clicked",
        "on_templatePathBrowseButton_clicked",
        "on_resetAllPathsButton_clicked",
        "on_visitWebsiteButton_clicked",
        "updateIconPreview",
        "QLabel*",
        "previewLabel",
        "onTabChanged",
        "showErrorMessage",
        "message",
        "registerScreenshotHotkey",
        "unregisterScreenshotHotkey",
        "takeScreenshot",
        "processSelectedScreenshot",
        "QRect",
        "selectedRect",
        "showScreenshotPreview",
        "QPixmap",
        "screenshot",
        "filePath",
        "openImageEditor",
        "setupSystemTray",
        "showFromTray",
        "hideToTray"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_startButton_clicked'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_stopButton_clicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_browseWechatButton_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_browseQuestionsButton_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_saveConfigButton_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_loadConfigButton_clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_saveAdvancedConfigButton_clicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_loadAdvancedConfigButton_clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_exportLogButton_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_clearLogButton_clicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_thresholdSpin_valueChanged'
        QtMocHelpers::SlotData<void(double)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 13 },
        }}),
        // Slot 'on_attemptsSpin_valueChanged'
        QtMocHelpers::SlotData<void(int)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Slot 'on_pageTimeoutSpin_valueChanged'
        QtMocHelpers::SlotData<void(int)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Slot 'on_recognitionTimeoutSpin_valueChanged'
        QtMocHelpers::SlotData<void(int)>(16, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Slot 'on_recognitionTechniqueCombo_currentIndexChanged'
        QtMocHelpers::SlotData<void(int)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'on_timeoutSpin_valueChanged'
        QtMocHelpers::SlotData<void(int)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Slot 'on_loopCountSpin_valueChanged'
        QtMocHelpers::SlotData<void(int)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Slot 'on_multiMonitorCheck_toggled'
        QtMocHelpers::SlotData<void(bool)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 22 },
        }}),
        // Slot 'on_primaryMonitorSpin_valueChanged'
        QtMocHelpers::SlotData<void(int)>(23, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Slot 'on_keyboardInputCheck_toggled'
        QtMocHelpers::SlotData<void(bool)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 22 },
        }}),
        // Slot 'on_pasteInputCheck_toggled'
        QtMocHelpers::SlotData<void(bool)>(25, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 22 },
        }}),
        // Slot 'on_themeCombo_currentIndexChanged'
        QtMocHelpers::SlotData<void(int)>(26, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'on_applyShortcutButton_clicked'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onAutomationStateChanged'
        QtMocHelpers::SlotData<void(Automator::State)>(28, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 29, 30 },
        }}),
        // Slot 'onAutomationCompleted'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_browseIconButton_clicked'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_saveIconButton_clicked'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_resetIconButton_clicked'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_iconNameCombo_currentIndexChanged'
        QtMocHelpers::SlotData<void(int)>(35, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'on_iconPathEdit_textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(36, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 37 },
        }}),
        // Slot 'on_configPathBrowseButton_clicked'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_questionPathBrowseButton_clicked'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_keywordPathBrowseButton_clicked'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_logPathBrowseButton_clicked'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_templatePathBrowseButton_clicked'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_resetAllPathsButton_clicked'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_visitWebsiteButton_clicked'
        QtMocHelpers::SlotData<void()>(44, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateIconPreview'
        QtMocHelpers::SlotData<void(const QString &, QLabel *)>(45, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 37 }, { 0x80000000 | 46, 47 },
        }}),
        // Slot 'onTabChanged'
        QtMocHelpers::SlotData<void(int)>(48, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'showErrorMessage'
        QtMocHelpers::SlotData<void(const QString &)>(49, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 50 },
        }}),
        // Slot 'registerScreenshotHotkey'
        QtMocHelpers::SlotData<void()>(51, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'unregisterScreenshotHotkey'
        QtMocHelpers::SlotData<void()>(52, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'takeScreenshot'
        QtMocHelpers::SlotData<void()>(53, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'processSelectedScreenshot'
        QtMocHelpers::SlotData<void(const QRect &)>(54, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 55, 56 },
        }}),
        // Slot 'showScreenshotPreview'
        QtMocHelpers::SlotData<void(const QPixmap &, const QString &)>(57, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 58, 59 }, { QMetaType::QString, 60 },
        }}),
        // Slot 'openImageEditor'
        QtMocHelpers::SlotData<void(const QString &, const QPixmap &)>(61, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 60 }, { 0x80000000 | 58, 59 },
        }}),
        // Slot 'setupSystemTray'
        QtMocHelpers::SlotData<void()>(62, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showFromTray'
        QtMocHelpers::SlotData<void()>(63, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'hideToTray'
        QtMocHelpers::SlotData<void()>(64, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<FramelessWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_startButton_clicked(); break;
        case 1: _t->on_stopButton_clicked(); break;
        case 2: _t->on_browseWechatButton_clicked(); break;
        case 3: _t->on_browseQuestionsButton_clicked(); break;
        case 4: _t->on_saveConfigButton_clicked(); break;
        case 5: _t->on_loadConfigButton_clicked(); break;
        case 6: _t->on_saveAdvancedConfigButton_clicked(); break;
        case 7: _t->on_loadAdvancedConfigButton_clicked(); break;
        case 8: _t->on_exportLogButton_clicked(); break;
        case 9: _t->on_clearLogButton_clicked(); break;
        case 10: _t->on_thresholdSpin_valueChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 11: _t->on_attemptsSpin_valueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->on_pageTimeoutSpin_valueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->on_recognitionTimeoutSpin_valueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->on_recognitionTechniqueCombo_currentIndexChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->on_timeoutSpin_valueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->on_loopCountSpin_valueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->on_multiMonitorCheck_toggled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 18: _t->on_primaryMonitorSpin_valueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->on_keyboardInputCheck_toggled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 20: _t->on_pasteInputCheck_toggled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 21: _t->on_themeCombo_currentIndexChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 22: _t->on_applyShortcutButton_clicked(); break;
        case 23: _t->onAutomationStateChanged((*reinterpret_cast<std::add_pointer_t<Automator::State>>(_a[1]))); break;
        case 24: _t->onAutomationCompleted(); break;
        case 25: _t->on_browseIconButton_clicked(); break;
        case 26: _t->on_saveIconButton_clicked(); break;
        case 27: _t->on_resetIconButton_clicked(); break;
        case 28: _t->on_iconNameCombo_currentIndexChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 29: _t->on_iconPathEdit_textChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 30: _t->on_configPathBrowseButton_clicked(); break;
        case 31: _t->on_questionPathBrowseButton_clicked(); break;
        case 32: _t->on_keywordPathBrowseButton_clicked(); break;
        case 33: _t->on_logPathBrowseButton_clicked(); break;
        case 34: _t->on_templatePathBrowseButton_clicked(); break;
        case 35: _t->on_resetAllPathsButton_clicked(); break;
        case 36: _t->on_visitWebsiteButton_clicked(); break;
        case 37: _t->updateIconPreview((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QLabel*>>(_a[2]))); break;
        case 38: _t->onTabChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 39: _t->showErrorMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 40: _t->registerScreenshotHotkey(); break;
        case 41: _t->unregisterScreenshotHotkey(); break;
        case 42: _t->takeScreenshot(); break;
        case 43: _t->processSelectedScreenshot((*reinterpret_cast<std::add_pointer_t<QRect>>(_a[1]))); break;
        case 44: _t->showScreenshotPreview((*reinterpret_cast<std::add_pointer_t<QPixmap>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 45: _t->openImageEditor((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QPixmap>>(_a[2]))); break;
        case 46: _t->setupSystemTray(); break;
        case 47: _t->showFromTray(); break;
        case 48: _t->hideToTray(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 37:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLabel* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return FramelessWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = FramelessWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 49)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 49;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 49)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 49;
    }
    return _id;
}
QT_WARNING_POP
