/****************************************************************************
** Meta object code from reading C++ file 'applyfriend.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../applyfriend.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'applyfriend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.0. It"
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
struct qt_meta_stringdata_CLASSApplyFriendENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSApplyFriendENDCLASS = QtMocHelpers::stringData(
    "ApplyFriend",
    "slotLabelEnter",
    "",
    "ShowMoreLabel",
    "SlotApplySure",
    "SlotApplyCancel",
    "SlotChangeFriendLabelByTip",
    "text",
    "ClickLbState",
    "SlotAddFirendLabelByClickTip",
    "SlotRemoveFriendLabel",
    "name",
    "slotLabelTextChange",
    "SlotLabelEditFinished"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSApplyFriendENDCLASS_t {
    uint offsetsAndSizes[28];
    char stringdata0[12];
    char stringdata1[15];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[14];
    char stringdata5[16];
    char stringdata6[27];
    char stringdata7[5];
    char stringdata8[13];
    char stringdata9[29];
    char stringdata10[22];
    char stringdata11[5];
    char stringdata12[20];
    char stringdata13[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSApplyFriendENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSApplyFriendENDCLASS_t qt_meta_stringdata_CLASSApplyFriendENDCLASS = {
    {
        QT_MOC_LITERAL(0, 11),  // "ApplyFriend"
        QT_MOC_LITERAL(12, 14),  // "slotLabelEnter"
        QT_MOC_LITERAL(27, 0),  // ""
        QT_MOC_LITERAL(28, 13),  // "ShowMoreLabel"
        QT_MOC_LITERAL(42, 13),  // "SlotApplySure"
        QT_MOC_LITERAL(56, 15),  // "SlotApplyCancel"
        QT_MOC_LITERAL(72, 26),  // "SlotChangeFriendLabelByTip"
        QT_MOC_LITERAL(99, 4),  // "text"
        QT_MOC_LITERAL(104, 12),  // "ClickLbState"
        QT_MOC_LITERAL(117, 28),  // "SlotAddFirendLabelByClickTip"
        QT_MOC_LITERAL(146, 21),  // "SlotRemoveFriendLabel"
        QT_MOC_LITERAL(168, 4),  // "name"
        QT_MOC_LITERAL(173, 19),  // "slotLabelTextChange"
        QT_MOC_LITERAL(193, 21)   // "SlotLabelEditFinished"
    },
    "ApplyFriend",
    "slotLabelEnter",
    "",
    "ShowMoreLabel",
    "SlotApplySure",
    "SlotApplyCancel",
    "SlotChangeFriendLabelByTip",
    "text",
    "ClickLbState",
    "SlotAddFirendLabelByClickTip",
    "SlotRemoveFriendLabel",
    "name",
    "slotLabelTextChange",
    "SlotLabelEditFinished"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSApplyFriendENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x0a,    1 /* Public */,
       3,    0,   69,    2, 0x0a,    2 /* Public */,
       4,    0,   70,    2, 0x0a,    3 /* Public */,
       5,    0,   71,    2, 0x0a,    4 /* Public */,
       6,    2,   72,    2, 0x0a,    5 /* Public */,
       9,    1,   77,    2, 0x0a,    8 /* Public */,
      10,    1,   80,    2, 0x0a,   10 /* Public */,
      12,    1,   83,    2, 0x0a,   12 /* Public */,
      13,    0,   86,    2, 0x0a,   14 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8,    7,    2,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ApplyFriend::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSApplyFriendENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSApplyFriendENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSApplyFriendENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ApplyFriend, std::true_type>,
        // method 'slotLabelEnter'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'ShowMoreLabel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SlotApplySure'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SlotApplyCancel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SlotChangeFriendLabelByTip'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<ClickLbState, std::false_type>,
        // method 'SlotAddFirendLabelByClickTip'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'SlotRemoveFriendLabel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'slotLabelTextChange'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'SlotLabelEditFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ApplyFriend::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ApplyFriend *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->slotLabelEnter(); break;
        case 1: _t->ShowMoreLabel(); break;
        case 2: _t->SlotApplySure(); break;
        case 3: _t->SlotApplyCancel(); break;
        case 4: _t->SlotChangeFriendLabelByTip((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<ClickLbState>>(_a[2]))); break;
        case 5: _t->SlotAddFirendLabelByClickTip((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->SlotRemoveFriendLabel((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->slotLabelTextChange((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->SlotLabelEditFinished(); break;
        default: ;
        }
    }
}

const QMetaObject *ApplyFriend::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ApplyFriend::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSApplyFriendENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ApplyFriend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
