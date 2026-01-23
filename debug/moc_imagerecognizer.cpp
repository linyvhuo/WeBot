/****************************************************************************
** Meta object code from reading C++ file 'imagerecognizer.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../imagerecognizer.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagerecognizer.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN15ImageRecognizerE_t {};
} // unnamed namespace

template <> constexpr inline auto ImageRecognizer::qt_create_metaobjectdata<qt_meta_tag_ZN15ImageRecognizerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ImageRecognizer",
        "logMessage",
        "",
        "message",
        "templateFound",
        "templateName",
        "QPoint",
        "pos",
        "templateNotFound",
        "answerReceivedChecked",
        "HWND",
        "hwnd",
        "received",
        "recognitionAreaFound",
        "QRect",
        "area",
        "description",
        "onConfigChanged",
        "doFindTemplateInWindow"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'logMessage'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'templateFound'
        QtMocHelpers::SignalData<void(const QString &, const QPoint &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 }, { 0x80000000 | 6, 7 },
        }}),
        // Signal 'templateNotFound'
        QtMocHelpers::SignalData<void(const QString &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Signal 'answerReceivedChecked'
        QtMocHelpers::SignalData<void(HWND, bool)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 10, 11 }, { QMetaType::Bool, 12 },
        }}),
        // Signal 'recognitionAreaFound'
        QtMocHelpers::SignalData<void(const QRect &, const QString &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 14, 15 }, { QMetaType::QString, 16 },
        }}),
        // Slot 'onConfigChanged'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'doFindTemplateInWindow'
        QtMocHelpers::SlotData<void(HWND, const QString &)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 10, 11 }, { QMetaType::QString, 5 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ImageRecognizer, qt_meta_tag_ZN15ImageRecognizerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ImageRecognizer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ImageRecognizerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ImageRecognizerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15ImageRecognizerE_t>.metaTypes,
    nullptr
} };

void ImageRecognizer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ImageRecognizer *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->logMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->templateFound((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[2]))); break;
        case 2: _t->templateNotFound((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->answerReceivedChecked((*reinterpret_cast<std::add_pointer_t<HWND>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 4: _t->recognitionAreaFound((*reinterpret_cast<std::add_pointer_t<QRect>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->onConfigChanged(); break;
        case 6: _t->doFindTemplateInWindow((*reinterpret_cast<std::add_pointer_t<HWND>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ImageRecognizer::*)(const QString & )>(_a, &ImageRecognizer::logMessage, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ImageRecognizer::*)(const QString & , const QPoint & )>(_a, &ImageRecognizer::templateFound, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ImageRecognizer::*)(const QString & )>(_a, &ImageRecognizer::templateNotFound, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ImageRecognizer::*)(HWND , bool )>(_a, &ImageRecognizer::answerReceivedChecked, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ImageRecognizer::*)(const QRect & , const QString & )>(_a, &ImageRecognizer::recognitionAreaFound, 4))
            return;
    }
}

const QMetaObject *ImageRecognizer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageRecognizer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ImageRecognizerE_t>.strings))
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
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void ImageRecognizer::logMessage(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void ImageRecognizer::templateFound(const QString & _t1, const QPoint & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void ImageRecognizer::templateNotFound(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void ImageRecognizer::answerReceivedChecked(HWND _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void ImageRecognizer::recognitionAreaFound(const QRect & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2);
}
QT_WARNING_POP
