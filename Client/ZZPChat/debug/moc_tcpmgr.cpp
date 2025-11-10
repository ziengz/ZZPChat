/****************************************************************************
** Meta object code from reading C++ file 'tcpmgr.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../tcpmgr.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpmgr.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSTcpMgrENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSTcpMgrENDCLASS = QtMocHelpers::stringData(
    "TcpMgr",
    "sig_con_success",
    "",
    "bsuccess",
    "sig_send_data",
    "ReqId",
    "reqid",
    "data",
    "sig_login_failed",
    "sig_switch_chatdlg",
    "sig_user_search",
    "std::shared_ptr<SearchInfo>",
    "sig_auth_rsp",
    "std::shared_ptr<AuthRsp>",
    "sig_friend_apply",
    "std::shared_ptr<AddFriendApply>",
    "slot_tcp_connect",
    "ServerInfo",
    "slot_send_data",
    "id"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSTcpMgrENDCLASS_t {
    uint offsetsAndSizes[40];
    char stringdata0[7];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[14];
    char stringdata5[6];
    char stringdata6[6];
    char stringdata7[5];
    char stringdata8[17];
    char stringdata9[19];
    char stringdata10[16];
    char stringdata11[28];
    char stringdata12[13];
    char stringdata13[25];
    char stringdata14[17];
    char stringdata15[32];
    char stringdata16[17];
    char stringdata17[11];
    char stringdata18[15];
    char stringdata19[3];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSTcpMgrENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSTcpMgrENDCLASS_t qt_meta_stringdata_CLASSTcpMgrENDCLASS = {
    {
        QT_MOC_LITERAL(0, 6),  // "TcpMgr"
        QT_MOC_LITERAL(7, 15),  // "sig_con_success"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 8),  // "bsuccess"
        QT_MOC_LITERAL(33, 13),  // "sig_send_data"
        QT_MOC_LITERAL(47, 5),  // "ReqId"
        QT_MOC_LITERAL(53, 5),  // "reqid"
        QT_MOC_LITERAL(59, 4),  // "data"
        QT_MOC_LITERAL(64, 16),  // "sig_login_failed"
        QT_MOC_LITERAL(81, 18),  // "sig_switch_chatdlg"
        QT_MOC_LITERAL(100, 15),  // "sig_user_search"
        QT_MOC_LITERAL(116, 27),  // "std::shared_ptr<SearchInfo>"
        QT_MOC_LITERAL(144, 12),  // "sig_auth_rsp"
        QT_MOC_LITERAL(157, 24),  // "std::shared_ptr<AuthRsp>"
        QT_MOC_LITERAL(182, 16),  // "sig_friend_apply"
        QT_MOC_LITERAL(199, 31),  // "std::shared_ptr<AddFriendApply>"
        QT_MOC_LITERAL(231, 16),  // "slot_tcp_connect"
        QT_MOC_LITERAL(248, 10),  // "ServerInfo"
        QT_MOC_LITERAL(259, 14),  // "slot_send_data"
        QT_MOC_LITERAL(274, 2)   // "id"
    },
    "TcpMgr",
    "sig_con_success",
    "",
    "bsuccess",
    "sig_send_data",
    "ReqId",
    "reqid",
    "data",
    "sig_login_failed",
    "sig_switch_chatdlg",
    "sig_user_search",
    "std::shared_ptr<SearchInfo>",
    "sig_auth_rsp",
    "std::shared_ptr<AuthRsp>",
    "sig_friend_apply",
    "std::shared_ptr<AddFriendApply>",
    "slot_tcp_connect",
    "ServerInfo",
    "slot_send_data",
    "id"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSTcpMgrENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   68,    2, 0x06,    1 /* Public */,
       4,    2,   71,    2, 0x06,    3 /* Public */,
       8,    1,   76,    2, 0x06,    6 /* Public */,
       9,    0,   79,    2, 0x06,    8 /* Public */,
      10,    1,   80,    2, 0x06,    9 /* Public */,
      12,    1,   83,    2, 0x06,   11 /* Public */,
      14,    1,   86,    2, 0x06,   13 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      16,    1,   89,    2, 0x0a,   15 /* Public */,
      18,    2,   92,    2, 0x0a,   17 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, 0x80000000 | 5, QMetaType::QByteArray,    6,    7,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void, 0x80000000 | 15,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 17,    2,
    QMetaType::Void, 0x80000000 | 5, QMetaType::QByteArray,   19,    7,

       0        // eod
};

Q_CONSTINIT const QMetaObject TcpMgr::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSTcpMgrENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSTcpMgrENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSTcpMgrENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<TcpMgr, std::true_type>,
        // method 'sig_con_success'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'sig_send_data'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<ReqId, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        // method 'sig_login_failed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'sig_switch_chatdlg'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sig_user_search'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::shared_ptr<SearchInfo>, std::false_type>,
        // method 'sig_auth_rsp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::shared_ptr<AuthRsp>, std::false_type>,
        // method 'sig_friend_apply'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::shared_ptr<AddFriendApply>, std::false_type>,
        // method 'slot_tcp_connect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<ServerInfo, std::false_type>,
        // method 'slot_send_data'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<ReqId, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>
    >,
    nullptr
} };

void TcpMgr::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TcpMgr *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sig_con_success((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->sig_send_data((*reinterpret_cast< std::add_pointer_t<ReqId>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[2]))); break;
        case 2: _t->sig_login_failed((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->sig_switch_chatdlg(); break;
        case 4: _t->sig_user_search((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<SearchInfo>>>(_a[1]))); break;
        case 5: _t->sig_auth_rsp((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<AuthRsp>>>(_a[1]))); break;
        case 6: _t->sig_friend_apply((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<AddFriendApply>>>(_a[1]))); break;
        case 7: _t->slot_tcp_connect((*reinterpret_cast< std::add_pointer_t<ServerInfo>>(_a[1]))); break;
        case 8: _t->slot_send_data((*reinterpret_cast< std::add_pointer_t<ReqId>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TcpMgr::*)(bool );
            if (_t _q_method = &TcpMgr::sig_con_success; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TcpMgr::*)(ReqId , QByteArray );
            if (_t _q_method = &TcpMgr::sig_send_data; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TcpMgr::*)(int );
            if (_t _q_method = &TcpMgr::sig_login_failed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TcpMgr::*)();
            if (_t _q_method = &TcpMgr::sig_switch_chatdlg; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TcpMgr::*)(std::shared_ptr<SearchInfo> );
            if (_t _q_method = &TcpMgr::sig_user_search; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TcpMgr::*)(std::shared_ptr<AuthRsp> );
            if (_t _q_method = &TcpMgr::sig_auth_rsp; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TcpMgr::*)(std::shared_ptr<AddFriendApply> );
            if (_t _q_method = &TcpMgr::sig_friend_apply; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject *TcpMgr::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpMgr::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSTcpMgrENDCLASS.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "singleton<TcpMgr>"))
        return static_cast< singleton<TcpMgr>*>(this);
    if (!strcmp(_clname, "std::enable_shared_from_this<TcpMgr>"))
        return static_cast< std::enable_shared_from_this<TcpMgr>*>(this);
    return QObject::qt_metacast(_clname);
}

int TcpMgr::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void TcpMgr::sig_con_success(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TcpMgr::sig_send_data(ReqId _t1, QByteArray _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TcpMgr::sig_login_failed(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TcpMgr::sig_switch_chatdlg()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TcpMgr::sig_user_search(std::shared_ptr<SearchInfo> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TcpMgr::sig_auth_rsp(std::shared_ptr<AuthRsp> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void TcpMgr::sig_friend_apply(std::shared_ptr<AddFriendApply> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
