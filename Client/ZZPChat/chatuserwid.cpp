#include "chatuserwid.h"
#include "ui_chatuserwid.h"

ChatUserWid::ChatUserWid(QWidget *parent) :
    ListItemBase(parent),
    ui(new Ui::ChatUserWid)
{
    ui->setupUi(this);
    SetItemType(ListItemType::CHAT_USER_ITEM);
    ShowRedPoint(false);
}

ChatUserWid::~ChatUserWid()
{
    delete ui;
}

void ChatUserWid::SetInfo(std::shared_ptr<UserInfo>user_info)
{
    _user_info = user_info;

    int random = QRandomGenerator::global()->bounded(100);
    auto head_i = random % heads.size();
    if(_user_info->_icon.isEmpty()){
        _user_info->_icon = heads[head_i];
    }
    QPixmap pixmap(_user_info->_icon);

    //头像图片自动缩放
    //缩放模式是Qt::KeepAspectRatio，表示保持宽高比，这样图片不会变形。
    //转换模式是Qt::SmoothTransformation，表示使用平滑的转换方式（抗锯齿）
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);

    ui->user_name_lb->setText(_user_info->_name);
    ui->user_chat_lb->setText(_user_info->_last_msg);
}

void ChatUserWid::SetInfo(QString name, QString head, QString msg)
{
    _head = head;
    _name = name;
    _msg = msg;
    QPixmap pixmap(_head);

    //头像图片自动缩放
    //缩放模式是Qt::KeepAspectRatio，表示保持宽高比，这样图片不会变形。
    //转换模式是Qt::SmoothTransformation，表示使用平滑的转换方式（抗锯齿）
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);

    ui->user_name_lb->setText(_name);
    ui->user_chat_lb->setText(_msg);
}

void ChatUserWid::SetInfo(std::shared_ptr<FriendInfo> friend_info)
{
    _user_info = std::make_shared<UserInfo>(friend_info);
    int random = QRandomGenerator::global()->bounded(100);
    int head_i = heads.size() % random;
    if(_user_info->_icon.isEmpty()){
        _user_info->_icon = heads[head_i];
    }
    QPixmap pix(_user_info->_icon);
    ui->icon_lb->setPixmap(pix.scaled(ui->icon_lb->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);
}

void ChatUserWid::ShowRedPoint(bool bshow)
{
    if(bshow){
        ui->red_point->show();
    }else{
        ui->red_point->hide();
    }
}

void ChatUserWid::updateLastMsg(std::vector<std::shared_ptr<TextChatData>> msgs)
{
    //更新最新消息
    QString last_msg = "";
    for(auto& msg : msgs){
        last_msg = msg->_msg_content;
        _user_info->_chat_msgs.push_back(msg);
    }
    _user_info->_last_msg = last_msg;
    ui->user_chat_lb->setText(last_msg);
}

std::shared_ptr<UserInfo> ChatUserWid::GetUserInfo()
{
    return _user_info;
}

