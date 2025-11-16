#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global.h"
#include <QList>
#include "statewidget.h"
#include "userdata.h"
#include <QListWidgetItem>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

private:
    void ShowSearch(bool b_search);
    void addChatUserList();
    void loadMoreChatUser();
    void loadMoreConUser();
    void AddLBGroup(StateWidget* lb);
    void ClearLabelState(StateWidget *lb);

    void UpdateChatMsg(std::vector<std::shared_ptr<TextChatData>>msgdata);
    bool eventFilter(QObject *watched, QEvent *event)override;
    void handleGlobalMousePress(QMouseEvent* event);
    Ui::ChatDialog *ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
    QList<StateWidget*> _lb_list;
    int cur_chat_uid;   //当前聊天的uid
    QMap<int,QListWidgetItem*>_chat_items_added;

private slots:
    void slot_loading_chat_user();
    void slot_loading_con_user();
    void slot_side_chat();
    void slot_side_contect();
    void slot_text_changed(const QString& text);
    void slot_apply_friend(std::shared_ptr<AddFriendApply>);
    void slot_add_auth_friend(std::shared_ptr<AuthInfo>auth_info);
    void slot_auth_rsp(std::shared_ptr<AuthRsp>auth_rsp);
    void slot_item_clicked(QListWidgetItem* item);
    void slot_text_chat_msg(std::shared_ptr<TextChatMsg> msg);
};

#endif // CHATDIALOG_H
