#include "chatpage.h"
#include "ui_chatpage.h"
#include <QStyleOption>
#include <QPainter>
#include "chatitembase.h"
#include "textbubble.h"
#include "picturebubble.h"
#include "usermgr.h"
#include "tcpmgr.h"
#include <QUuid>
#include <QJsonDocument>

ChatPage::ChatPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatPage)
{
    ui->setupUi(this);
    ui->receive_btn->SetState("normal","hover","press");
    ui->send_btn->SetState("normal","hover","press");

    //设置图标样式
    ui->emo_lb->SetState("normal","hover","press","normal","hover","press");
    ui->file_lb->SetState("normal","hover","press","normal","hover","press");
}

void ChatPage::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    this->style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

ChatPage::~ChatPage()
{
    delete ui;
}

void ChatPage::SetUserInfo(std::shared_ptr<UserInfo> user_info)
{
    //对方的用户信息
    _user_info = user_info;
    ui->title_lb->setText(_user_info->_name);
    ui->chat_data_list->removeAllItem();
    //追加之前聊天的消息
    for(auto & msg : user_info->_chat_msgs){
        AppendChatMsg(msg);
    }
}

void ChatPage::AppendChatMsg(std::shared_ptr<TextChatData> msg)
{
    auto self_info = UserMgr::getInstance()->GetUserInfo();
    ChatRole role;
    if(self_info->_uid == msg->_from_uid){
        role = ChatRole::Self;
        ChatItemBase*pChatItem = new ChatItemBase(role);
        pChatItem->setUserName(self_info->_name);
        pChatItem->setUserIcon(QPixmap(self_info->_icon));

        QWidget*pBubble = new TextBubble(role,msg->_msg_content);
        pChatItem->setWidget(pBubble);
        ui->chat_data_list->appendChatItem(pChatItem);
    }else{
        role = ChatRole::Other;
        ChatItemBase*pChatItem = new ChatItemBase(role);
        auto friend_info = UserMgr::getInstance()->GetFriendById(msg->_from_uid);
        if(friend_info==nullptr){
            return;
        }

        pChatItem->setUserName(friend_info->_name);
        pChatItem->setUserIcon(friend_info->_icon);

        QWidget*pBubble = new TextBubble(role,msg->_msg_content);
        pChatItem->setWidget(pBubble);
        ui->chat_data_list->appendChatItem(pChatItem);
    }
}

void ChatPage::on_send_btn_clicked()
{
    if(_user_info==nullptr){
        qDebug()<<"_user_info is empty";
        return;
    }

    //自己的用户信息
    auto user_info = UserMgr::getInstance()->GetUserInfo();
    auto pTextEdit = ui->chatEdit;
    ChatRole role = ChatRole::Self;
    QString userName = user_info->_name;
    QString userIcon = user_info->_icon;

    const QVector<MsgInfo>& msgList = pTextEdit->getMsgList();
    QJsonObject textObj;
    QJsonArray textArray;
    int text_size = 0;

    for(int i=0;i<msgList.size();i++){
        //单条消息过长 跳过
        if(msgList[i].content.length()>1024){
            continue;
        }

        QString type = msgList[i].msgFlag;
        ChatItemBase *pChatItem = new ChatItemBase(role);
        pChatItem->setUserName(userName);
        pChatItem->setUserIcon(QPixmap(userIcon));
        QWidget* pBubble = nullptr;
        if(type=="text"){
            //生成唯一消息id
            QUuid uuid = QUuid::createUuid();
            QString uuidString = uuid.toString();
            pBubble = new TextBubble(role,msgList[i].content);
            if(text_size+msgList[i].content.length()>1024){
                textObj["fromuid"] = user_info->_uid;
                textObj["touid"] = _user_info->_uid;
                textObj["text_array"] = textArray;
                QJsonDocument doc(textObj);
                QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
                //发送并清空之前累计的文本列表
                text_size = 0;
                textArray = QJsonArray();
                textObj = QJsonObject();
                //发送tcp请求给chat server
                emit TcpMgr::getInstance()->sig_send_data(ReqId::ID_TEXT_CHAT_MSG_REQ,jsonData);
            }
            //将bubble和uid绑定，以后可以等网络返回消息后设置是否送达
            text_size+=msgList[i].content.length();
            QJsonObject obj;
            QByteArray utf8Message = msgList[i].content.toUtf8();
            obj["content"] = QString::fromUtf8(utf8Message);
            obj["msgid"] = uuidString;
            textArray.append(obj);
            auto text_msg = std::make_shared<TextChatData>(uuidString,obj["content"].toString(),
                                user_info->_uid,_user_info->_uid);
            emit sig_append_chat_msg(text_msg);
        }
        else if(type == "image"){
            pBubble = new PictureBubble(QPixmap(msgList[i].content),role);
        }
        else if(type=="file"){

        }

        if(pBubble!=nullptr){
            pChatItem->setWidget(pBubble);
            ui->chat_data_list->appendChatItem(pChatItem);
        }
    }

    qDebug()<<"textArray is "<<textArray;
    //最后消息长度不足1024，跳出循环后直接发送
    textObj["text_array"] = textArray;
    textObj["fromuid"] = user_info->_uid;
    textObj["touid"] = _user_info->_uid;
    QJsonDocument Doc(textObj);
    QByteArray jsonData = Doc.toJson(QJsonDocument::Compact);
    //发送之前清理掉累计的文本列表
    text_size = 0;
    textArray = QJsonArray();
    textObj = QJsonObject();
    emit TcpMgr::getInstance()->sig_send_data(ReqId::ID_TEXT_CHAT_MSG_REQ,jsonData);
}


























