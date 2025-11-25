#include "chatdialog.h"
#include "ui_chatdialog.h"
#include <QAction>
#include "chatuserwid.h"
#include "loadingdlg.h"
#include "tcpmgr.h"
#include "usermgr.h"
#include "userdata.h"
#include "conuseritem.h"
#include "friendinfopage.h"

ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog),_b_loading(false),_mode(ChatUIMode::ChatMode),_state(ChatUIMode::ChatMode)
{
    ui->setupUi(this);
    ui->add_btn->SetState("normal","hover","press");
    ui->search_edit->SetMaxLength(15);

    QAction* searchAction = new QAction(ui->search_edit);
    searchAction->setIcon(QIcon(":/res/search.png"));
    ui->search_edit->addAction(searchAction,QLineEdit::LeadingPosition);
    ui->search_edit->setPlaceholderText(QStringLiteral("搜索"));
    ui->search_list->setSeatchEdit(ui->search_edit);

    QString head_icon = UserMgr::getInstance()->GetIcon();
    qDebug()<<"head_icon is "<<head_icon;
    QPixmap pix(head_icon);
    ui->side_head_lb->setPixmap(pix);                         //原比例缩放
    QPixmap scaledPixmap = pix.scaled(ui->side_head_lb->size(),Qt::KeepAspectRatio);
    ui->side_head_lb->setPixmap(scaledPixmap);
    ui->side_head_lb->setScaledContents(true);  //设置QLabel自动缩放图片内容以适应大小

    ui->side_chat_lb->setProperty("state","normal");

    ui->side_chat_lb->SetState("normal","hover","pressed","selected_normal","selected_hover","selected_pressed");
    ui->side_contact_lb->SetState("normal","hover","pressed","selected_normal","selected_hover","selected_pressed");

    AddLBGroup(ui->side_chat_lb);
    AddLBGroup(ui->side_contact_lb);
    //处理点击其中一个图标时，其他图标会变为正常
    connect(ui->side_chat_lb,&StateWidget::Clicked,this,&ChatDialog::slot_side_chat);
    connect(ui->side_contact_lb,&StateWidget::Clicked,this,&ChatDialog::slot_side_contect);

    //创建一个清除动作并设置图标
    QAction* clearAction = new QAction(ui->search_edit);
    clearAction->setIcon(QIcon(":/res/close_transparent.png"));
    //初始化时不显示图标
    //将清除图标添加到搜索框末尾
    ui->search_edit->addAction(clearAction,QLineEdit::TrailingPosition);

    //当填入数据，则将图标换为可见图标
    connect(ui->search_edit,&QLineEdit::textChanged,[clearAction](const QString&text){
        if(!text.isEmpty()){
            clearAction->setIcon(QIcon(":/res/close_search.png"));
        }
        else{
            clearAction->setIcon(QIcon(":/res/close_transparent.png"));
        }
    });
    connect(clearAction,&QAction::triggered,[this,clearAction](){
        ui->search_edit->clear();
        clearAction->setIcon(QIcon(":/res/close_transparent.png"));
        ui->search_edit->clearFocus();
        ShowSearch(false);
    });
    ShowSearch(false);
    connect(ui->chat_user_list,&ChatUserList::sig_loading_chat_user,this,&ChatDialog::slot_loading_chat_user);
    //假造更多contact页面好友
    connect(ui->con_user_list,&ContactUserList::sig_loading_contact_user,this,
            &ChatDialog::slot_loading_con_user);

    addChatUserList();

    //设置选中条目
    SetSelectChatItem();
    //更新聊天界面信息
    SetSelectChatPage();

    connect(ui->search_edit,&QLineEdit::textChanged,this,&ChatDialog::slot_text_changed);
    this->installEventFilter(this);

    ui->side_chat_lb->SetSelected(true);
    //设置中心部件为chatpage
    ui->stackedWidget->setCurrentWidget(ui->chat_page);

    //连接search_list跳转聊天信号
    connect(ui->search_list,&SearchList::sig_jump_chat_item,this,&ChatDialog::slot_jump_chat_item);
    //连接好友信息界面发送的点击事件
    connect(ui->friend_info_page, &FriendInfoPage::sig_jump_chat_item, this,
            &ChatDialog::slot_jump_chat_item_from_infopage);

    connect(ui->con_user_list,&ContactUserList::sig_switch_friend_info_page,this,
            &ChatDialog::slot_friend_info_page);
    //连接联系人页面点击好友申请条目的信号
    connect(ui->con_user_list, &ContactUserList::sig_switch_apply_friend_page,
            this,&ChatDialog::slot_switch_apply_friend_page);

    //连接聊天列表点击信号
    connect(ui->chat_user_list, &QListWidget::itemClicked, this, &ChatDialog::slot_item_clicked);

    //连接申请添加好友的信号
    connect(TcpMgr::getInstance().get(),&TcpMgr::sig_friend_apply,this,&ChatDialog::slot_apply_friend);
    //连接通知添加好友信号
    connect(TcpMgr::getInstance().get(),&TcpMgr::sig_add_auth_friend,this,&ChatDialog::slot_add_auth_friend);
    //连接添加好友信息回包
    connect(TcpMgr::getInstance().get(),&TcpMgr::sig_auth_rsp,this,&ChatDialog::slot_auth_rsp);

    //连接对端消息通知
    connect(TcpMgr::getInstance().get(), &TcpMgr::sig_text_chat_msg,
            this, &ChatDialog::slot_text_chat_msg);
    connect(ui->chat_page,&ChatPage::sig_append_chat_msg,this,&ChatDialog::slot_append_send_chat_msg);

}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::ShowSearch(bool b_search)
{
    if(b_search){
        ui->chat_user_list->hide();
        ui->con_user_list->hide();
        ui->search_list->show();
        _mode = ChatUIMode::SearchMode;
    }
    else if(_state==ChatUIMode::ChatMode){
        ui->chat_user_list->show();
        ui->con_user_list->hide();
        ui->search_list->hide();
        _mode = ChatUIMode::ChatMode;
    }
    else if(_state ==ChatUIMode::ContactMode){
        ui->chat_user_list->hide();
        ui->con_user_list->show();
        ui->search_list->hide();
        _mode = ChatUIMode::ContactMode;
    }
}


void ChatDialog::addChatUserList()
{
    auto friend_list = UserMgr::getInstance()->GetChatListPrePage();
    if(!friend_list.empty()){
        for(auto&friend_ele:friend_list){
            if(_chat_items_added.find(friend_ele->_uid)!=_chat_items_added.end()){
                continue;
            }

            auto* chat_user_wid = new ChatUserWid();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->SetInfo(user_info);
            QListWidgetItem * item = new QListWidgetItem;
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chat_user_list->insertItem(0,item);
            ui->chat_user_list->setItemWidget(item,chat_user_wid);
            _chat_items_added.insert(friend_ele->_uid,item);
        }
        UserMgr::getInstance()->UpdateChatLoadedCount();
    }
    for(int i=0;i<13;i++){
        //生成一个0-99的随机数
        int randomValue = QRandomGenerator::global()->bounded(100);
        int head_i = randomValue % heads.size();
        int str_i = randomValue % strs.size();
        int name_i = randomValue % names.size();

        auto* chat_user_wid = new ChatUserWid;

        auto user_info = std::make_shared<UserInfo>(0,names[name_i],
                                                        names[name_i],heads[head_i],0,strs[str_i]);
        chat_user_wid->SetInfo(user_info);
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(chat_user_wid->sizeHint());
        ui->chat_user_list->addItem(item);
        ui->chat_user_list->setItemWidget(item,chat_user_wid);
    }
}

void ChatDialog::loadMoreChatUser()
{
    auto friend_list = UserMgr::getInstance()->GetChatListPrePage();
    if(!friend_list.empty()){
        for(auto&friend_ele:friend_list){
            if(_chat_items_added.find(friend_ele->_uid)!=_chat_items_added.end()){
                continue;
            }

            auto* chat_user_wid = new ChatUserWid();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->SetInfo(user_info);
            QListWidgetItem * item = new QListWidgetItem;
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chat_user_list->insertItem(0,item);
            ui->chat_user_list->setItemWidget(item,chat_user_wid);
            _chat_items_added.insert(friend_ele->_uid,item);
        }
        UserMgr::getInstance()->UpdateChatLoadedCount();
    }
}

void ChatDialog::loadMoreConUser()
{
    auto friend_list = UserMgr::getInstance()->GetConListPrePage();
    if(!friend_list.empty()){
        for(auto& friend_ele : friend_list){
            auto* con_user_list = new ConUserItem();
            con_user_list->SetInfo(friend_ele->_uid,friend_ele->_name,
                                   friend_ele->_icon);
            QListWidgetItem*item = new QListWidgetItem();
            item->setSizeHint(con_user_list->sizeHint());
            ui->con_user_list->insertItem(0,item);
            ui->con_user_list->setItemWidget(item,con_user_list);
        }
        UserMgr::getInstance()->UpdateChatLoadedCount();
    }
}

void ChatDialog::SetSelectChatItem(int uid)
{
    if(ui->chat_user_list->count()<=0){
        return;
    }
    if(uid == 0){
        ui->chat_user_list->setCurrentRow(0);
        QListWidgetItem * firstItem = ui->chat_user_list->item(0);
        if(!firstItem){
            return;
        }
        QWidget* item = ui->chat_user_list->itemWidget(firstItem);
        if(!item){
            return;
        }
        auto chat_item = qobject_cast<ChatUserWid*>(item);
        cur_chat_uid = chat_item->GetUserInfo()->_uid;
        return;
    }
    auto find_iter = _chat_items_added.find(uid);
    if(find_iter == _chat_items_added.end()){
        qDebug()<<"uid "<<uid<<"is not found";
        ui->chat_user_list->setCurrentRow(0);
        return;
    }
    ui->chat_user_list->setCurrentItem(find_iter.value());
    cur_chat_uid = uid;
}

void ChatDialog::SetSelectChatPage(int uid)
{
    if(ui->chat_user_list->count() <= 0){
        return;
    }
    if(uid == 0){
        auto item = ui->chat_user_list->item(0);

        QWidget* widget = ui->chat_user_list->itemWidget(item);
        if(!widget){
            return;
        }

        auto chat_item = qobject_cast<ChatUserWid*>(widget);
        if(!chat_item){
            return;
        }
        auto user_info = chat_item->GetUserInfo();
        ui->chat_page->SetUserInfo(user_info);
        return;
    }

    auto find_iter = _chat_items_added.find(uid);
    if(find_iter == _chat_items_added.end()){
        return;
    }
    QWidget* widget = ui->chat_user_list->itemWidget(find_iter.value());
    if(!widget){
        return;
    }
    auto chat_item = qobject_cast<ChatUserWid*>(widget);
    auto user_info = chat_item->GetUserInfo();
    ui->chat_page->SetUserInfo(user_info);
    return;
}

void ChatDialog::AddLBGroup(StateWidget *lb)
{
    _lb_list.push_back(lb);
}

void ChatDialog::slot_loading_chat_user()
{
    if(_b_loading){
        return;
    }
    _b_loading = true;
    LoadingDlg* loadingDlg = new LoadingDlg(this);
    loadingDlg->setModal(true);
    loadingDlg->show();
    qDebug()<<"add new data to list...";
    loadMoreChatUser();
    //显示完删除对话框
    loadingDlg->deleteLater();
    _b_loading = false;
}

void ChatDialog::slot_loading_con_user()
{
    if(_b_loading){
        return;
    }
    _b_loading = true;
    LoadingDlg* loadingDlg = new LoadingDlg(this);
    loadingDlg->setModal(true);
    loadingDlg->show();
    qDebug()<<"add new data to list...";
    loadMoreConUser();
    //显示完删除对话框
    loadingDlg->deleteLater();
    _b_loading = false;
}

void ChatDialog::ClearLabelState(StateWidget *lb)
{
    for(auto& elem:_lb_list){
        if(elem==lb){
            continue;
        }
        elem->ClearState();
    }
}

void ChatDialog::UpdateChatMsg(std::vector<std::shared_ptr<TextChatData> > msgdata)
{
    for(auto& msg:msgdata){
        //因为只有当对面发来消息才会
        if(cur_chat_uid!=msg->_from_uid){
            break;
        }
        ui->chat_page->AppendChatMsg(msg);
    }
}

bool ChatDialog::eventFilter(QObject *watched, QEvent *event)
{
    //监听鼠标点击事件
    if(event->type()==QEvent::MouseButtonPress){
        QMouseEvent*mouseEvent = static_cast<QMouseEvent*>(event);
        handleGlobalMousePress(mouseEvent);
        return true;
    }
    return QDialog::eventFilter(watched,event);
}

void ChatDialog::handleGlobalMousePress(QMouseEvent *event)
{
    //计算鼠标点击位置，如果不在searchlist范围内，清空搜索框内容切换回chat_user_list
    if(_mode!=ChatUIMode::SearchMode)
        return;
    QPoint posInSearchList = ui->search_list->mapFromGlobal(event->globalPos());
    //如果点击位置不包含在search_list中
    if(!ui->search_list->rect().contains(posInSearchList)){
        ui->search_edit->clear();
        ShowSearch(false);
    }
}

void ChatDialog::slot_side_chat()
{
    qDebug()<<"receive side_chat clicked";
    ui->search_edit->clear();
    ClearLabelState(ui->side_chat_lb);
    ui->stackedWidget->setCurrentWidget(ui->chat_page);
    _state = ChatUIMode::ChatMode;
    ShowSearch(false);
}

void ChatDialog::slot_side_contect()
{
    qDebug()<<"receive side_contect clicked";
    ui->search_edit->clear();
    ClearLabelState(ui->side_contact_lb);
    ui->stackedWidget->setCurrentWidget(ui->friend_apply_page);
    _state = ChatUIMode::ContactMode;
    ShowSearch(false);
}

void ChatDialog::slot_text_changed(const QString& text)
{
    if(!text.isEmpty()){
        ShowSearch(true);
    }
}

void ChatDialog::slot_apply_friend(std::shared_ptr<AddFriendApply> apply)
{
    qDebug()<<"receive apply uid is "<<apply->_from_uid<<" name is "<<apply->_name
           <<" desc is "<<apply->_desc;
    bool b_already = UserMgr::getInstance()->AlreadyApply(apply->_from_uid);
    if(b_already){
        return;
    }

    UserMgr::getInstance()->AddApplyList(std::make_shared<ApplyInfo>(apply));
    ui->side_contact_lb->ShowRedPoint(true);
    ui->con_user_list->ShowRedPoint(true);
    ui->friend_apply_page->addNewApply(apply);
}

void ChatDialog::slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info)
{
    qDebug() << "receive slot_add_auth_friend uid is " << auth_info->_uid
        << " name is " << auth_info->_name << " nick is " << auth_info->_nick;
    //判断如果已经是好友则跳过
    bool b_friend = UserMgr::getInstance()->CheckFriendById(auth_info->_uid);
    if(b_friend){
        return;
    }
    UserMgr::getInstance()->AddFriend(auth_info);
    auto*chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(auth_info);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0,item);
    ui->chat_user_list->setItemWidget(item,chat_user_wid);

}

void ChatDialog::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp)
{
    qDebug() << "receive slot_auth_friend uid is " << auth_rsp->_uid
        << " name is " << auth_rsp->_name << " nick is " << auth_rsp->_nick;
    //判断如果已经是好友则跳过
    bool b_friend = UserMgr::getInstance()->CheckFriendById(auth_rsp->_uid);
    if(b_friend){
        return;
    }
    UserMgr::getInstance()->AddFriend(auth_rsp);
    auto*chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(auth_rsp);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0,item);
    ui->chat_user_list->setItemWidget(item,chat_user_wid);
}

void ChatDialog::slot_item_clicked(QListWidgetItem *item)
{
    QWidget* widget = ui->chat_user_list->itemWidget(item);//获取系定义widget对象
    if(!widget){
        qDebug()<<"slot item clicked widget is nullptr";
        return;
    }
    ListItemBase* customItem = qobject_cast<ListItemBase*>(widget);

    ListItemType itemType = customItem->GetItemType();
    if(itemType==ListItemType::GROUP_TIP_ITEM||itemType == ListItemType::INVALID_ITEM){
        qDebug()<<"slot invalid item clicked";
        return;
    }
    if(itemType==ListItemType::CHAT_USER_ITEM){
        qDebug()<<"contact user item clicked";
        auto chat_wid = qobject_cast<ChatUserWid*>(customItem);
        if(!chat_wid){
            return;
        }

        auto user_info = chat_wid->GetUserInfo();
        ui->chat_page->SetUserInfo(user_info);
        cur_chat_uid = user_info->_uid;
        return;
    }
}

void ChatDialog::slot_text_chat_msg(std::shared_ptr<TextChatMsg> msg)
{
    //查看是否对应存在listwidget
    auto find_iter = _chat_items_added.find(msg->_from_uid);
    if(find_iter!=_chat_items_added.end()){
        qDebug()<<"set chat item msg,uid is "<<msg->_from_uid;
        QWidget*item = ui->chat_user_list->itemWidget(find_iter.value());
        auto chat_wid = qobject_cast<ChatUserWid*>(item);
        if(!chat_wid){
            return;
        }
        chat_wid->updateLastMsg(msg->_chat_msgs);
        //更新当前聊天页面记录
        UpdateChatMsg(msg->_chat_msgs);

    }
    else{
        //没找到创建一个新的ChatUserWid
        auto* chat_user_wid = new ChatUserWid;
        auto friend_info = UserMgr::getInstance()->GetFriendById(msg->_from_uid);
        chat_user_wid->SetInfo(friend_info);
        chat_user_wid->updateLastMsg(msg->_chat_msgs);
        UserMgr::getInstance()->AppendFriendChatMsg(friend_info->_uid,msg->_chat_msgs);

        QListWidgetItem*item = new QListWidgetItem;
        item->setSizeHint(chat_user_wid->sizeHint());
        ui->chat_user_list->insertItem(0,item);
        ui->chat_user_list->setItemWidget(item,chat_user_wid);
        _chat_items_added.insert(msg->_from_uid,item);
    }
}

void ChatDialog::slot_append_send_chat_msg(std::shared_ptr<TextChatData> msgdata)
{
    if(cur_chat_uid == 0){
        return;
    }
    auto find_iter = _chat_items_added.find(cur_chat_uid);
    if(find_iter == _chat_items_added.end()){
        qDebug()<<"chat item not found";
        return;
    }
    QWidget* item = ui->chat_user_list->itemWidget(find_iter.value());
    if(!item){
        return;
    }
    ListItemBase* customItem = qobject_cast<ListItemBase*>(item);
    if(!customItem){
        qDebug()<<"qobject_cast<ListItemBase> worry";
        return;
    }
    auto itemType = customItem->GetItemType();
    if(itemType==ListItemType::CHAT_USER_ITEM){
        auto con_item = qobject_cast<ChatUserWid*>(customItem);
        auto user_info = con_item->GetUserInfo();

        user_info->_chat_msgs.push_back(msgdata);
        std::vector<std::shared_ptr<TextChatData>>msg_vec;
        msg_vec.push_back(msgdata);
        UserMgr::getInstance()->AppendFriendChatMsg(cur_chat_uid,msg_vec);
        return;
    }

}

void ChatDialog::slot_jump_chat_item(std::shared_ptr<SearchInfo> si)
{
    qDebug()<<"slot jump chat item";
    auto find_iter = _chat_items_added.find(si->_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug()<<"jump to chat item,uid is "<<si->_uid;
        ui->chat_user_list->scrollToItem(find_iter.value());
        ui->side_chat_lb->SetSelected(true);
        SetSelectChatItem(si->_uid);
        SetSelectChatPage(si->_uid);
        slot_side_chat();
        return;
    }
    auto chat_user_wid = new ChatUserWid;
    auto user_info = std::make_shared<UserInfo>(si);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0,item);
    ui->chat_user_list->setItemWidget(item,chat_user_wid);

    _chat_items_added.insert(si->_uid,item);
    ui->side_chat_lb->SetSelected(true);
    SetSelectChatItem(si->_uid);
    SetSelectChatPage(si->_uid);
    slot_side_chat();
    return;
}

void ChatDialog::slot_jump_chat_item_from_infopage(std::shared_ptr<UserInfo> user_info)
{
    auto find_iter = _chat_items_added.find(user_info->_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug()<<"jump to chat item uid is "<<user_info->_uid;
        ui->chat_user_list->scrollToItem(find_iter.value());

        ui->side_chat_lb->SetSelected(true);
        SetSelectChatItem(user_info->_uid);
        SetSelectChatPage(user_info->_uid);
        slot_side_chat();
        return;
    }

    auto* chat_user_wid = new ChatUserWid;
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(chat_user_wid->sizeHint());

    ui->chat_user_list->insertItem(0,item);
    ui->chat_user_list->setItemWidget(item,chat_user_wid);
    _chat_items_added.insert(user_info->_uid,item);
    ui->side_chat_lb->SetSelected(true);

    SetSelectChatItem(user_info->_uid);
    SetSelectChatPage(user_info->_uid);
    slot_side_chat();
    return;
}

void ChatDialog::slot_switch_apply_friend_page()
{
    qDebug()<<"receive switch apply friend page sig";
    _last_widget = ui->friend_apply_page;
    ui->stackedWidget->setCurrentWidget(_last_widget);
}

void ChatDialog::slot_friend_info_page(std::shared_ptr<UserInfo> user_info)
{
    _last_widget = ui->friend_info_page;
    ui->stackedWidget->setCurrentWidget(_last_widget);
    ui->friend_info_page->SetInfo(user_info);
}

