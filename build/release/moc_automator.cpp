/****************************************************************************
** Meta object code from reading C++ file 'automator.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../automator.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'automator.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSAutomatorENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSAutomatorENDCLASS = QtMocHelpers::stringData(
    "Automator",
    "logMessage",
    "",
    "message",
    "progressUpdated",
    "currentCount",
    "totalCount",
    "stateChanged",
    "State",
    "state",
    "automationCompleted",
    "errorMessage",
    "runAutomation",
    "onFinished",
    "Idle",
    "Starting",
    "Running",
    "Completed",
    "Error"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSAutomatorENDCLASS_t {
    uint offsetsAndSizes[38];
    char stringdata0[10];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[16];
    char stringdata5[13];
    char stringdata6[11];
    char stringdata7[13];
    char stringdata8[6];
    char stringdata9[6];
    char stringdata10[20];
    char stringdata11[13];
    char stringdata12[14];
    char stringdata13[11];
    char stringdata14[5];
    char stringdata15[9];
    char stringdata16[8];
    char stringdata17[10];
    char stringdata18[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSAutomatorENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSAutomatorENDCLASS_t qt_meta_stringdata_CLASSAutomatorENDCLASS = {
    {
        QT_MOC_LITERAL(0, 9),  // "Automator"
        QT_MOC_LITERAL(10, 10),  // "logMessage"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 7),  // "message"
        QT_MOC_LITERAL(30, 15),  // "progressUpdated"
        QT_MOC_LITERAL(46, 12),  // "currentCount"
        QT_MOC_LITERAL(59, 10),  // "totalCount"
        QT_MOC_LITERAL(70, 12),  // "stateChanged"
        QT_MOC_LITERAL(83, 5),  // "State"
        QT_MOC_LITERAL(89, 5),  // "state"
        QT_MOC_LITERAL(95, 19),  // "automationCompleted"
        QT_MOC_LITERAL(115, 12),  // "errorMessage"
        QT_MOC_LITERAL(128, 13),  // "runAutomation"
        QT_MOC_LITERAL(142, 10),  // "onFinished"
        QT_MOC_LITERAL(153, 4),  // "Idle"
        QT_MOC_LITERAL(158, 8),  // "Starting"
        QT_MOC_LITERAL(167, 7),  // "Running"
        QT_MOC_LITERAL(175, 9),  // "Completed"
        QT_MOC_LITERAL(185, 5)   // "Error"
    },
    "Automator",
    "logMessage",
    "",
    "message",
    "progressUpdated",
    "currentCount",
    "totalCount",
    "stateChanged",
    "State",
    "state",
    "automationCompleted",
    "errorMessage",
    "runAutomation",
    "onFinished",
    "Idle",
    "Starting",
    "Running",
    "Completed",
    "Error"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSAutomatorENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       1,   73, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x06,    1 /* Public */,
       4,    2,   59,    2, 0x06,    3 /* Public */,
       7,    1,   64,    2, 0x06,    6 /* Public */,
      10,    0,   67,    2, 0x06,    8 /* Public */,
      11,    1,   68,    2, 0x06,    9 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      12,    0,   71,    2, 0x08,   11 /* Private */,
      13,    0,   72,    2, 0x08,   12 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    5,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

 // enums: name, alias, flags, count, data
       8,    8, 0x0,    5,   78,

 // enum data: key, value
      14, uint(Automator::Idle),
      15, uint(Automator::Starting),
      16, uint(Automator::Running),
      17, uint(Automator::Completed),
      18, uint(Automator::Error),

       0        // eod
};

Q_CONSTINIT const QMetaObject Automator::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSAutomatorENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSAutomatorENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSAutomatorENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Automator, std::true_type>,
        // method 'logMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'progressUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'stateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<State, std::false_type>,
        // method 'automationCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'errorMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'runAutomation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Automator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Automator *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->logMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->progressUpdated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 2: _t->stateChanged((*reinterpret_cast< std::add_pointer_t<State>>(_a[1]))); break;
        case 3: _t->automationCompleted(); break;
        case 4: _t->errorMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->runAutomation(); break;
        case 6: _t->onFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Automator::*)(const QString & );
            if (_t _q_method = &Automator::logMessage; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Automator::*)(int , int );
            if (_t _q_method = &Automator::progressUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Automator::*)(State );
            if (_t _q_method = &Automator::stateChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Automator::*)();
            if (_t _q_method = &Automator::automationCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Automator::*)(const QString & );
            if (_t _q_method = &Automator::errorMessage; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *Automator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Automator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSAutomatorENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Automator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Automator::logMessage(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Automator::progressUpdated(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Automator::stateChanged(State _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Automator::automationCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Automator::errorMessage(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
