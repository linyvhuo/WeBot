/****************************************************************************
** Meta object code from reading C++ file 'imagerecognizer.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../imagerecognizer.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagerecognizer.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSImageRecognizerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSImageRecognizerENDCLASS = QtMocHelpers::stringData(
    "ImageRecognizer",
    "logMessage",
    "",
    "message",
    "templateFound",
    "templateName",
    "pos",
    "templateNotFound",
    "answerReceivedChecked",
    "HWND",
    "hwnd",
    "received",
    "recognitionAreaFound",
    "area",
    "description",
    "onConfigChanged",
    "doFindTemplateInWindow"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSImageRecognizerENDCLASS_t {
    uint offsetsAndSizes[34];
    char stringdata0[16];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[14];
    char stringdata5[13];
    char stringdata6[4];
    char stringdata7[17];
    char stringdata8[22];
    char stringdata9[5];
    char stringdata10[5];
    char stringdata11[9];
    char stringdata12[21];
    char stringdata13[5];
    char stringdata14[12];
    char stringdata15[16];
    char stringdata16[23];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSImageRecognizerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSImageRecognizerENDCLASS_t qt_meta_stringdata_CLASSImageRecognizerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 15),  // "ImageRecognizer"
        QT_MOC_LITERAL(16, 10),  // "logMessage"
        QT_MOC_LITERAL(27, 0),  // ""
        QT_MOC_LITERAL(28, 7),  // "message"
        QT_MOC_LITERAL(36, 13),  // "templateFound"
        QT_MOC_LITERAL(50, 12),  // "templateName"
        QT_MOC_LITERAL(63, 3),  // "pos"
        QT_MOC_LITERAL(67, 16),  // "templateNotFound"
        QT_MOC_LITERAL(84, 21),  // "answerReceivedChecked"
        QT_MOC_LITERAL(106, 4),  // "HWND"
        QT_MOC_LITERAL(111, 4),  // "hwnd"
        QT_MOC_LITERAL(116, 8),  // "received"
        QT_MOC_LITERAL(125, 20),  // "recognitionAreaFound"
        QT_MOC_LITERAL(146, 4),  // "area"
        QT_MOC_LITERAL(151, 11),  // "description"
        QT_MOC_LITERAL(163, 15),  // "onConfigChanged"
        QT_MOC_LITERAL(179, 22)   // "doFindTemplateInWindow"
    },
    "ImageRecognizer",
    "logMessage",
    "",
    "message",
    "templateFound",
    "templateName",
    "pos",
    "templateNotFound",
    "answerReceivedChecked",
    "HWND",
    "hwnd",
    "received",
    "recognitionAreaFound",
    "area",
    "description",
    "onConfigChanged",
    "doFindTemplateInWindow"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSImageRecognizerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   56,    2, 0x06,    1 /* Public */,
       4,    2,   59,    2, 0x06,    3 /* Public */,
       7,    1,   64,    2, 0x06,    6 /* Public */,
       8,    2,   67,    2, 0x06,    8 /* Public */,
      12,    2,   72,    2, 0x06,   11 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      15,    0,   77,    2, 0x08,   14 /* Private */,
      16,    2,   78,    2, 0x08,   15 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QPoint,    5,    6,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, 0x80000000 | 9, QMetaType::Bool,   10,   11,
    QMetaType::Void, QMetaType::QRect, QMetaType::QString,   13,   14,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, QMetaType::QString,   10,    5,

       0        // eod
};

Q_CONSTINIT const QMetaObject ImageRecognizer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSImageRecognizerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSImageRecognizerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSImageRecognizerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ImageRecognizer, std::true_type>,
        // method 'logMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'templateFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'templateNotFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'answerReceivedChecked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<HWND, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'recognitionAreaFound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QRect &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onConfigChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'doFindTemplateInWindow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<HWND, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void ImageRecognizer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImageRecognizer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->logMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->templateFound((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[2]))); break;
        case 2: _t->templateNotFound((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->answerReceivedChecked((*reinterpret_cast< std::add_pointer_t<HWND>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 4: _t->recognitionAreaFound((*reinterpret_cast< std::add_pointer_t<QRect>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->onConfigChanged(); break;
        case 6: _t->doFindTemplateInWindow((*reinterpret_cast< std::add_pointer_t<HWND>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ImageRecognizer::*)(const QString & );
            if (_t _q_method = &ImageRecognizer::logMessage; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ImageRecognizer::*)(const QString & , const QPoint & );
            if (_t _q_method = &ImageRecognizer::templateFound; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ImageRecognizer::*)(const QString & );
            if (_t _q_method = &ImageRecognizer::templateNotFound; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ImageRecognizer::*)(HWND , bool );
            if (_t _q_method = &ImageRecognizer::answerReceivedChecked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ImageRecognizer::*)(const QRect & , const QString & );
            if (_t _q_method = &ImageRecognizer::recognitionAreaFound; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *ImageRecognizer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageRecognizer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSImageRecognizerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ImageRecognizer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void ImageRecognizer::logMessage(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ImageRecognizer::templateFound(const QString & _t1, const QPoint & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ImageRecognizer::templateNotFound(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ImageRecognizer::answerReceivedChecked(HWND _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ImageRecognizer::recognitionAreaFound(const QRect & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
