/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
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
    "updateIconPreview",
    "QLabel*",
    "previewLabel",
    "onTabChanged",
    "showErrorMessage",
    "message"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[80];
    char stringdata0[11];
    char stringdata1[23];
    char stringdata2[1];
    char stringdata3[22];
    char stringdata4[30];
    char stringdata5[33];
    char stringdata6[28];
    char stringdata7[28];
    char stringdata8[36];
    char stringdata9[36];
    char stringdata10[27];
    char stringdata11[26];
    char stringdata12[30];
    char stringdata13[6];
    char stringdata14[29];
    char stringdata15[32];
    char stringdata16[39];
    char stringdata17[49];
    char stringdata18[6];
    char stringdata19[28];
    char stringdata20[30];
    char stringdata21[29];
    char stringdata22[8];
    char stringdata23[35];
    char stringdata24[25];
    char stringdata25[17];
    char stringdata26[6];
    char stringdata27[22];
    char stringdata28[28];
    char stringdata29[26];
    char stringdata30[27];
    char stringdata31[37];
    char stringdata32[28];
    char stringdata33[5];
    char stringdata34[18];
    char stringdata35[8];
    char stringdata36[13];
    char stringdata37[13];
    char stringdata38[17];
    char stringdata39[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 22),  // "on_startButton_clicked"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 21),  // "on_stopButton_clicked"
        QT_MOC_LITERAL(57, 29),  // "on_browseWechatButton_clicked"
        QT_MOC_LITERAL(87, 32),  // "on_browseQuestionsButton_clicked"
        QT_MOC_LITERAL(120, 27),  // "on_saveConfigButton_clicked"
        QT_MOC_LITERAL(148, 27),  // "on_loadConfigButton_clicked"
        QT_MOC_LITERAL(176, 35),  // "on_saveAdvancedConfigButton_c..."
        QT_MOC_LITERAL(212, 35),  // "on_loadAdvancedConfigButton_c..."
        QT_MOC_LITERAL(248, 26),  // "on_exportLogButton_clicked"
        QT_MOC_LITERAL(275, 25),  // "on_clearLogButton_clicked"
        QT_MOC_LITERAL(301, 29),  // "on_thresholdSpin_valueChanged"
        QT_MOC_LITERAL(331, 5),  // "value"
        QT_MOC_LITERAL(337, 28),  // "on_attemptsSpin_valueChanged"
        QT_MOC_LITERAL(366, 31),  // "on_pageTimeoutSpin_valueChanged"
        QT_MOC_LITERAL(398, 38),  // "on_recognitionTimeoutSpin_val..."
        QT_MOC_LITERAL(437, 48),  // "on_recognitionTechniqueCombo_..."
        QT_MOC_LITERAL(486, 5),  // "index"
        QT_MOC_LITERAL(492, 27),  // "on_timeoutSpin_valueChanged"
        QT_MOC_LITERAL(520, 29),  // "on_loopCountSpin_valueChanged"
        QT_MOC_LITERAL(550, 28),  // "on_multiMonitorCheck_toggled"
        QT_MOC_LITERAL(579, 7),  // "checked"
        QT_MOC_LITERAL(587, 34),  // "on_primaryMonitorSpin_valueCh..."
        QT_MOC_LITERAL(622, 24),  // "onAutomationStateChanged"
        QT_MOC_LITERAL(647, 16),  // "Automator::State"
        QT_MOC_LITERAL(664, 5),  // "state"
        QT_MOC_LITERAL(670, 21),  // "onAutomationCompleted"
        QT_MOC_LITERAL(692, 27),  // "on_browseIconButton_clicked"
        QT_MOC_LITERAL(720, 25),  // "on_saveIconButton_clicked"
        QT_MOC_LITERAL(746, 26),  // "on_resetIconButton_clicked"
        QT_MOC_LITERAL(773, 36),  // "on_iconNameCombo_currentIndex..."
        QT_MOC_LITERAL(810, 27),  // "on_iconPathEdit_textChanged"
        QT_MOC_LITERAL(838, 4),  // "path"
        QT_MOC_LITERAL(843, 17),  // "updateIconPreview"
        QT_MOC_LITERAL(861, 7),  // "QLabel*"
        QT_MOC_LITERAL(869, 12),  // "previewLabel"
        QT_MOC_LITERAL(882, 12),  // "onTabChanged"
        QT_MOC_LITERAL(895, 16),  // "showErrorMessage"
        QT_MOC_LITERAL(912, 7)   // "message"
    },
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
    "updateIconPreview",
    "QLabel*",
    "previewLabel",
    "onTabChanged",
    "showErrorMessage",
    "message"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      29,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  188,    2, 0x08,    1 /* Private */,
       3,    0,  189,    2, 0x08,    2 /* Private */,
       4,    0,  190,    2, 0x08,    3 /* Private */,
       5,    0,  191,    2, 0x08,    4 /* Private */,
       6,    0,  192,    2, 0x08,    5 /* Private */,
       7,    0,  193,    2, 0x08,    6 /* Private */,
       8,    0,  194,    2, 0x08,    7 /* Private */,
       9,    0,  195,    2, 0x08,    8 /* Private */,
      10,    0,  196,    2, 0x08,    9 /* Private */,
      11,    0,  197,    2, 0x08,   10 /* Private */,
      12,    1,  198,    2, 0x08,   11 /* Private */,
      14,    1,  201,    2, 0x08,   13 /* Private */,
      15,    1,  204,    2, 0x08,   15 /* Private */,
      16,    1,  207,    2, 0x08,   17 /* Private */,
      17,    1,  210,    2, 0x08,   19 /* Private */,
      19,    1,  213,    2, 0x08,   21 /* Private */,
      20,    1,  216,    2, 0x08,   23 /* Private */,
      21,    1,  219,    2, 0x08,   25 /* Private */,
      23,    1,  222,    2, 0x08,   27 /* Private */,
      24,    1,  225,    2, 0x08,   29 /* Private */,
      27,    0,  228,    2, 0x08,   31 /* Private */,
      28,    0,  229,    2, 0x08,   32 /* Private */,
      29,    0,  230,    2, 0x08,   33 /* Private */,
      30,    0,  231,    2, 0x08,   34 /* Private */,
      31,    1,  232,    2, 0x08,   35 /* Private */,
      32,    1,  235,    2, 0x08,   37 /* Private */,
      34,    2,  238,    2, 0x08,   39 /* Private */,
      37,    1,  243,    2, 0x08,   42 /* Private */,
      38,    1,  246,    2, 0x08,   44 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   13,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Bool,   22,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::QString,   33,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 35,   33,   36,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::QString,   39,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'on_startButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_stopButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_browseWechatButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_browseQuestionsButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_saveConfigButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_loadConfigButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_saveAdvancedConfigButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_loadAdvancedConfigButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_exportLogButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_clearLogButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_thresholdSpin_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'on_attemptsSpin_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_pageTimeoutSpin_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_recognitionTimeoutSpin_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_recognitionTechniqueCombo_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_timeoutSpin_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_loopCountSpin_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_multiMonitorCheck_toggled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_primaryMonitorSpin_valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onAutomationStateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Automator::State, std::false_type>,
        // method 'onAutomationCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_browseIconButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_saveIconButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_resetIconButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_iconNameCombo_currentIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_iconPathEdit_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'updateIconPreview'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLabel *, std::false_type>,
        // method 'onTabChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'showErrorMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
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
        case 10: _t->on_thresholdSpin_valueChanged((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 11: _t->on_attemptsSpin_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->on_pageTimeoutSpin_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->on_recognitionTimeoutSpin_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->on_recognitionTechniqueCombo_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->on_timeoutSpin_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->on_loopCountSpin_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->on_multiMonitorCheck_toggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 18: _t->on_primaryMonitorSpin_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->onAutomationStateChanged((*reinterpret_cast< std::add_pointer_t<Automator::State>>(_a[1]))); break;
        case 20: _t->onAutomationCompleted(); break;
        case 21: _t->on_browseIconButton_clicked(); break;
        case 22: _t->on_saveIconButton_clicked(); break;
        case 23: _t->on_resetIconButton_clicked(); break;
        case 24: _t->on_iconNameCombo_currentIndexChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 25: _t->on_iconPathEdit_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 26: _t->updateIconPreview((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QLabel*>>(_a[2]))); break;
        case 27: _t->onTabChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 28: _t->showErrorMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 26:
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
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    }
    return _id;
}
QT_WARNING_POP
