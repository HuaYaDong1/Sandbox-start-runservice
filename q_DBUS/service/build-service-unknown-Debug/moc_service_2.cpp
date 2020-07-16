/****************************************************************************
** Meta object code from reading C++ file 'service_2.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../service/service_2.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'service_2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_service_2_t {
    QByteArrayData data[8];
    char stringdata0[86];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_service_2_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_service_2_t qt_meta_stringdata_service_2 = {
    {
QT_MOC_LITERAL(0, 0, 9), // "service_2"
QT_MOC_LITERAL(1, 10, 15), // "D-Bus Interface"
QT_MOC_LITERAL(2, 26, 15), // "com.client.test"
QT_MOC_LITERAL(3, 42, 14), // "send_to_client"
QT_MOC_LITERAL(4, 57, 0), // ""
QT_MOC_LITERAL(5, 58, 2), // "st"
QT_MOC_LITERAL(6, 61, 11), // "service_get"
QT_MOC_LITERAL(7, 73, 12) // "appname_base"

    },
    "service_2\0D-Bus Interface\0com.client.test\0"
    "send_to_client\0\0st\0service_get\0"
    "appname_base"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_service_2[] = {

 // content:
       8,       // revision
       0,       // classname
       1,   14, // classinfo
       2,   16, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags
       3,    1,   26,    4, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   29,    4, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    7,

       0        // eod
};

void service_2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<service_2 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->send_to_client((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->service_get((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (service_2::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&service_2::send_to_client)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject service_2::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_service_2.data,
    qt_meta_data_service_2,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *service_2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *service_2::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_service_2.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int service_2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void service_2::send_to_client(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
