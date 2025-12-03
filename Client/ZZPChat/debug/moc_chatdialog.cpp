/****************************************************************************
** Meta object code from reading C++ file 'chatdialog.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chatdialog.h"
#include <QtNetwork/QSslError>
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatdialog.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSChatDialogENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSChatDialogENDCLASS = QtMocHelpers::stringData(
    "ChatDialog",
    "slot_loading_chat_user",
    "",
    "slot_loading_con_user",
    "slot_side_chat",
    "slot_side_contect",
    "slot_side_setting",
    "slot_text_changed",
    "text",
    "slot_apply_friend",
    "std::shared_ptr<AddFriendApply>",
    "slot_add_auth_friend",
    "std::shared_ptr<AuthInfo>",
    "auth_info",
    "slot_auth_rsp",
    "std::shared_ptr<AuthRsp>",
    "auth_rsp",
    "slot_item_clicked",
    "QListWidgetItem*",
    "item",
    "slot_text_chat_msg",
    "std::shared_ptr<TextChatMsg>",
    "msg",
    "slot_append_send_chat_msg",
    "std::shared_ptr<TextChatData>",
    "msgdata",
    "slot_jump_chat_item",
    "std::shared_ptr<SearchInfo>",
    "si",
    "slot_jump_chat_item_from_infopage",
    "std::shared_ptr<UserInfo>",
    "user_info",
    "slot_switch_apply_friend_page",
    "slot_friend_info_page"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSChatDialogENDCLASS_t {
    uint offsetsAndSizes[68];
    char stringdata0[11];
    char stringdata1[23];
    char stringdata2[1];
    char stringdata3[22];
    char stringdata4[15];
    char stringdata5[18];
    char stringdata6[18];
    char stringdata7[18];
    char stringdata8[5];
    char stringdata9[18];
    char stringdata10[32];
    char stringdata11[21];
    char stringdata12[26];
    char stringdata13[10];
    char stringdata14[14];
    char stringdata15[25];
    char stringdata16[9];
    char stringdata17[18];
    char stringdata18[17];
    char stringdata19[5];
    char stringdata20[19];
    char stringdata21[29];
    char stringdata22[4];
    char stringdata23[26];
    char stringdata24[30];
    char stringdata25[8];
    char stringdata26[20];
    char stringdata27[28];
    char stringdata28[3];
    char stringdata29[34];
    char stringdata30[26];
    char stringdata31[10];
    char stringdata32[30];
    char stringdata33[22];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSChatDialogENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSChatDialogENDCLASS_t qt_meta_stringdata_CLASSChatDialogENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "ChatDialog"
        QT_MOC_LITERAL(11, 22),  // "slot_loading_chat_user"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 21),  // "slot_loading_con_user"
        QT_MOC_LITERAL(57, 14),  // "slot_side_chat"
        QT_MOC_LITERAL(72, 17),  // "slot_side_contect"
        QT_MOC_LITERAL(90, 17),  // "slot_side_setting"
        QT_MOC_LITERAL(108, 17),  // "slot_text_changed"
        QT_MOC_LITERAL(126, 4),  // "text"
        QT_MOC_LITERAL(131, 17),  // "slot_apply_friend"
        QT_MOC_LITERAL(149, 31),  // "std::shared_ptr<AddFriendApply>"
        QT_MOC_LITERAL(181, 20),  // "slot_add_auth_friend"
        QT_MOC_LITERAL(202, 25),  // "std::shared_ptr<AuthInfo>"
        QT_MOC_LITERAL(228, 9),  // "auth_info"
        QT_MOC_LITERAL(238, 13),  // "slot_auth_rsp"
        QT_MOC_LITERAL(252, 24),  // "std::shared_ptr<AuthRsp>"
        QT_MOC_LITERAL(277, 8),  // "auth_rsp"
        QT_MOC_LITERAL(286, 17),  // "slot_item_clicked"
        QT_MOC_LITERAL(304, 16),  // "QListWidgetItem*"
        QT_MOC_LITERAL(321, 4),  // "item"
        QT_MOC_LITERAL(326, 18),  // "slot_text_chat_msg"
        QT_MOC_LITERAL(345, 28),  // "std::shared_ptr<TextChatMsg>"
        QT_MOC_LITERAL(374, 3),  // "msg"
        QT_MOC_LITERAL(378, 25),  // "slot_append_send_chat_msg"
        QT_MOC_LITERAL(404, 29),  // "std::shared_ptr<TextChatData>"
        QT_MOC_LITERAL(434, 7),  // "msgdata"
        QT_MOC_LITERAL(442, 19),  // "slot_jump_chat_item"
        QT_MOC_LITERAL(462, 27),  // "std::shared_ptr<SearchInfo>"
        QT_MOC_LITERAL(490, 2),  // "si"
        QT_MOC_LITERAL(493, 33),  // "slot_jump_chat_item_from_info..."
        QT_MOC_LITERAL(527, 25),  // "std::shared_ptr<UserInfo>"
        QT_MOC_LITERAL(553, 9),  // "user_info"
        QT_MOC_LITERAL(563, 29),  // "slot_switch_apply_friend_page"
        QT_MOC_LITERAL(593, 21)   // "slot_friend_info_page"
    },
    "ChatDialog",
    "slot_loading_chat_user",
    "",
    "slot_loading_con_user",
    "slot_side_chat",
    "slot_side_contect",
    "slot_side_setting",
    "slot_text_changed",
    "text",
    "slot_apply_friend",
    "std::shared_ptr<AddFriendApply>",
    "slot_add_auth_friend",
    "std::shared_ptr<AuthInfo>",
    "auth_info",
    "slot_auth_rsp",
    "std::shared_ptr<AuthRsp>",
    "auth_rsp",
    "slot_item_clicked",
    "QListWidgetItem*",
    "item",
    "slot_text_chat_msg",
    "std::shared_ptr<TextChatMsg>",
    "msg",
    "slot_append_send_chat_msg",
    "std::shared_ptr<TextChatData>",
    "msgdata",
    "slot_jump_chat_item",
    "std::shared_ptr<SearchInfo>",
    "si",
    "slot_jump_chat_item_from_infopage",
    "std::shared_ptr<UserInfo>",
    "user_info",
    "slot_switch_apply_friend_page",
    "slot_friend_info_page"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSChatDialogENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  110,    2, 0x08,    1 /* Private */,
       3,    0,  111,    2, 0x08,    2 /* Private */,
       4,    0,  112,    2, 0x08,    3 /* Private */,
       5,    0,  113,    2, 0x08,    4 /* Private */,
       6,    0,  114,    2, 0x08,    5 /* Private */,
       7,    1,  115,    2, 0x08,    6 /* Private */,
       9,    1,  118,    2, 0x08,    8 /* Private */,
      11,    1,  121,    2, 0x08,   10 /* Private */,
      14,    1,  124,    2, 0x08,   12 /* Private */,
      17,    1,  127,    2, 0x08,   14 /* Private */,
      20,    1,  130,    2, 0x08,   16 /* Private */,
      23,    1,  133,    2, 0x08,   18 /* Private */,
      26,    1,  136,    2, 0x08,   20 /* Private */,
      29,    1,  139,    2, 0x08,   22 /* Private */,
      32,    0,  142,    2, 0x08,   24 /* Private */,
      33,    1,  143,    2, 0x08,   25 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, 0x80000000 | 24,   25,
    QMetaType::Void, 0x80000000 | 27,   28,
    QMetaType::Void, 0x80000000 | 30,   31,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 30,   31,

       0        // eod
};

Q_CONSTINIT const QMetaObject ChatDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSChatDialogENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSChatDialogENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSChatDialogENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ChatDialog, std::true_type>,
        // method 'slot_loading_chat_user'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slot_loading_con_user'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slot_side_chat'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slot_side_contect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slot_side_setting'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slot_text_changed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'slot_apply_friend'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::shared_ptr<AddFriendApply>, std::false_type>,
        // method 'slot_add_auth_friend'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::shared_ptr<AuthInfo>, std::false_type>,
        // method 'slot_auth_rsp'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::shared_ptr<AuthRsp>, std::false_type>,
        // method 'slot_item_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'slot_text_chat_msg'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::shared_ptr<TextChatMsg>, std::false_type>,
        // method 'slot_append_send_chat_msg'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::shared_ptr<TextChatData>, std::false_type>,
        // method 'slot_jump_chat_item'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::shared_ptr<SearchInfo>, std::false_type>,
        // method 'slot_jump_chat_item_from_infopage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::shared_ptr<UserInfo>, std::false_type>,
        // method 'slot_switch_apply_friend_page'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'slot_friend_info_page'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::shared_ptr<UserInfo>, std::false_type>
    >,
    nullptr
} };

void ChatDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChatDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->slot_loading_chat_user(); break;
        case 1: _t->slot_loading_con_user(); break;
        case 2: _t->slot_side_chat(); break;
        case 3: _t->slot_side_contect(); break;
        case 4: _t->slot_side_setting(); break;
        case 5: _t->slot_text_changed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->slot_apply_friend((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<AddFriendApply>>>(_a[1]))); break;
        case 7: _t->slot_add_auth_friend((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<AuthInfo>>>(_a[1]))); break;
        case 8: _t->slot_auth_rsp((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<AuthRsp>>>(_a[1]))); break;
        case 9: _t->slot_item_clicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 10: _t->slot_text_chat_msg((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<TextChatMsg>>>(_a[1]))); break;
        case 11: _t->slot_append_send_chat_msg((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<TextChatData>>>(_a[1]))); break;
        case 12: _t->slot_jump_chat_item((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<SearchInfo>>>(_a[1]))); break;
        case 13: _t->slot_jump_chat_item_from_infopage((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<UserInfo>>>(_a[1]))); break;
        case 14: _t->slot_switch_apply_friend_page(); break;
        case 15: _t->slot_friend_info_page((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<UserInfo>>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *ChatDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSChatDialogENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ChatDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
