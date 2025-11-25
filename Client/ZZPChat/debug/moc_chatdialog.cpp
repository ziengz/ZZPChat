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
    uint offsetsAndSizes[66];
    char stringdata0[11];
    char stringdata1[23];
    char stringdata2[1];
    char stringdata3[22];
    char stringdata4[15];
    char stringdata5[18];
    char stringdata6[18];
    char stringdata7[5];
    char stringdata8[18];
    char stringdata9[32];
    char stringdata10[21];
    char stringdata11[26];
    char stringdata12[10];
    char stringdata13[14];
    char stringdata14[25];
    char stringdata15[9];
    char stringdata16[18];
    char stringdata17[17];
    char stringdata18[5];
    char stringdata19[19];
    char stringdata20[29];
    char stringdata21[4];
    char stringdata22[26];
    char stringdata23[30];
    char stringdata24[8];
    char stringdata25[20];
    char stringdata26[28];
    char stringdata27[3];
    char stringdata28[34];
    char stringdata29[26];
    char stringdata30[10];
    char stringdata31[30];
    char stringdata32[22];
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
        QT_MOC_LITERAL(90, 17),  // "slot_text_changed"
        QT_MOC_LITERAL(108, 4),  // "text"
        QT_MOC_LITERAL(113, 17),  // "slot_apply_friend"
        QT_MOC_LITERAL(131, 31),  // "std::shared_ptr<AddFriendApply>"
        QT_MOC_LITERAL(163, 20),  // "slot_add_auth_friend"
        QT_MOC_LITERAL(184, 25),  // "std::shared_ptr<AuthInfo>"
        QT_MOC_LITERAL(210, 9),  // "auth_info"
        QT_MOC_LITERAL(220, 13),  // "slot_auth_rsp"
        QT_MOC_LITERAL(234, 24),  // "std::shared_ptr<AuthRsp>"
        QT_MOC_LITERAL(259, 8),  // "auth_rsp"
        QT_MOC_LITERAL(268, 17),  // "slot_item_clicked"
        QT_MOC_LITERAL(286, 16),  // "QListWidgetItem*"
        QT_MOC_LITERAL(303, 4),  // "item"
        QT_MOC_LITERAL(308, 18),  // "slot_text_chat_msg"
        QT_MOC_LITERAL(327, 28),  // "std::shared_ptr<TextChatMsg>"
        QT_MOC_LITERAL(356, 3),  // "msg"
        QT_MOC_LITERAL(360, 25),  // "slot_append_send_chat_msg"
        QT_MOC_LITERAL(386, 29),  // "std::shared_ptr<TextChatData>"
        QT_MOC_LITERAL(416, 7),  // "msgdata"
        QT_MOC_LITERAL(424, 19),  // "slot_jump_chat_item"
        QT_MOC_LITERAL(444, 27),  // "std::shared_ptr<SearchInfo>"
        QT_MOC_LITERAL(472, 2),  // "si"
        QT_MOC_LITERAL(475, 33),  // "slot_jump_chat_item_from_info..."
        QT_MOC_LITERAL(509, 25),  // "std::shared_ptr<UserInfo>"
        QT_MOC_LITERAL(535, 9),  // "user_info"
        QT_MOC_LITERAL(545, 29),  // "slot_switch_apply_friend_page"
        QT_MOC_LITERAL(575, 21)   // "slot_friend_info_page"
    },
    "ChatDialog",
    "slot_loading_chat_user",
    "",
    "slot_loading_con_user",
    "slot_side_chat",
    "slot_side_contect",
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
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  104,    2, 0x08,    1 /* Private */,
       3,    0,  105,    2, 0x08,    2 /* Private */,
       4,    0,  106,    2, 0x08,    3 /* Private */,
       5,    0,  107,    2, 0x08,    4 /* Private */,
       6,    1,  108,    2, 0x08,    5 /* Private */,
       8,    1,  111,    2, 0x08,    7 /* Private */,
      10,    1,  114,    2, 0x08,    9 /* Private */,
      13,    1,  117,    2, 0x08,   11 /* Private */,
      16,    1,  120,    2, 0x08,   13 /* Private */,
      19,    1,  123,    2, 0x08,   15 /* Private */,
      22,    1,  126,    2, 0x08,   17 /* Private */,
      25,    1,  129,    2, 0x08,   19 /* Private */,
      28,    1,  132,    2, 0x08,   21 /* Private */,
      31,    0,  135,    2, 0x08,   23 /* Private */,
      32,    1,  136,    2, 0x08,   24 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, 0x80000000 | 23,   24,
    QMetaType::Void, 0x80000000 | 26,   27,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 29,   30,

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
        case 4: _t->slot_text_changed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->slot_apply_friend((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<AddFriendApply>>>(_a[1]))); break;
        case 6: _t->slot_add_auth_friend((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<AuthInfo>>>(_a[1]))); break;
        case 7: _t->slot_auth_rsp((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<AuthRsp>>>(_a[1]))); break;
        case 8: _t->slot_item_clicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 9: _t->slot_text_chat_msg((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<TextChatMsg>>>(_a[1]))); break;
        case 10: _t->slot_append_send_chat_msg((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<TextChatData>>>(_a[1]))); break;
        case 11: _t->slot_jump_chat_item((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<SearchInfo>>>(_a[1]))); break;
        case 12: _t->slot_jump_chat_item_from_infopage((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<UserInfo>>>(_a[1]))); break;
        case 13: _t->slot_switch_apply_friend_page(); break;
        case 14: _t->slot_friend_info_page((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<UserInfo>>>(_a[1]))); break;
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
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
