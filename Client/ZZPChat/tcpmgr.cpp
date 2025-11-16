#include "tcpmgr.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "usermgr.h"

TcpMgr::~TcpMgr()
{

}

void TcpMgr::CloseCOnnection()
{
    socket_.close();
}

TcpMgr::TcpMgr():host_(""),port_(0),b_recv_pending(false),
    message_id_(0),message_len_(0)
{
    connect(&socket_,&QTcpSocket::connected,[&](){
        qDebug()<<"Connected to server";
        emit sig_con_success(true);
    });


    connect(&socket_,&QTcpSocket::readyRead,[&](){
        // 当有数据可读时，读取所有数据
        // 读取所有数据并追加到缓冲区
        buffer_.append(socket_.readAll());
        QDataStream stream(&buffer_,QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_6_0);

        forever{
            //先解析头部
            if(!b_recv_pending){
                if(buffer_.size() < static_cast<int>(sizeof(quint16)*2)){
                    return;  //说明头部数据没取完全
                }
                stream >> message_id_ >> message_len_;

                //把头部数据截取掉
                buffer_ = buffer_.mid(sizeof(quint16)*2);
                qDebug()<<"Message ID:"<<message_id_<<"Message length:"<<message_len_;
            }

            if(buffer_.size()<message_len_){
                b_recv_pending = true;
                return;
            }

            b_recv_pending = false;
            QByteArray messageBody = buffer_.mid(0,message_len_);
            qDebug()<<"receive body msg is "<<messageBody;
            buffer_ = buffer_.mid(message_len_);

            //读取到回包后就去handles_的map中查询并执行回调函数
            handleMsg(ReqId(message_id_),message_len_,messageBody);
        }

    });

    QObject::connect(&socket_, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), [&](QAbstractSocket::SocketError socketError) {
        Q_UNUSED(socketError)
        qDebug() << "Error:" << socket_.errorString();
    });

    //QTcpSocket::error 信号有多个重载版本：有参和无参两种，需要显式指定
    //void (QTcpSocket::*) - 指向 QTcpSocket 类成员函数的指针
    //(QTcpSocket::SocketError) - 该函数接受一个 QTcpSocket::SocketError 参数
//    connect(&socket_,static_cast<void (QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error),[&](QTcpSocket&socketError){
//        qDebug()<<"socket Error is "<<socket_.errorString();
//        switch(socketError){
//        case QTcpSocket::HostNotFoundError:
//            qDebug()<<"host not found";
//            emit sig_con_success(false);
//            break;
//        case QTcpSocket::RemoteHostClosedError:
//            qDebug()<<"remote host closed";
//            break;
//        case QTcpSocket::SocketTimeoutError:
//            qDebug()<<"Connection Timeout!";
//            emit sig_con_success(false);
//            break;
//        case QTcpSocket::NetworkError:
//            qDebug()<<"Network Error";
//            break;
//        default:
//            qDebug()<<"Other error";
//            break;
//        }
//    });

    connect(&socket_,&QTcpSocket::disconnected,[&](){
        qDebug()<<"Disconnect from server";
    });
    connect(this,&TcpMgr::sig_send_data,this,&TcpMgr::slot_send_data);
    //注册消息
    initHandlers();

}

void TcpMgr::initHandlers()
{
    //处理登陆聊天服务器回包，用于接收用户信息
    handlers_.insert(ReqId::ID_CHAT_LOGIN_RSP,[this](ReqId id,int len,QByteArray data){
        Q_UNUSED(len);
        qDebug()<<"handle id is "<<id<<"data is "<<data;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        //检查转换是否成功
        if(jsonDoc.isNull()){
            qDebug()<<"Failed to create QJsonDocument";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error")){
            int err = ErrorCodes::ERR_JSON;
            qDebug()<<"Login Failed,err is Json Parse err"<<err;
            emit sig_login_failed(err);
            return;
        }
        int err = jsonObj["error"].toInt();
        if(jsonObj["error"]!=ErrorCodes::SUCCESS){
            qDebug()<<"Login failed,err is "<<jsonObj["err"];
            emit sig_login_failed(err);
            return;
        }

        auto uid = jsonObj["uid"].toInt();
        auto name = jsonObj["name"].toString();
        auto nick = jsonObj["nick"].toString();
        auto icon = jsonObj["icon"].toString();
        auto sex = jsonObj["sex"].toInt();
        auto desc = jsonObj["desc"].toString();
        auto user_info = std::make_shared<UserInfo>(uid, name, nick, icon, sex,"",desc);

        UserMgr::getInstance()->setUserInfo(user_info);
        UserMgr::getInstance()->SetToken(jsonObj["Token"].toString());
        if(jsonObj.contains("apply_list")){
            UserMgr::getInstance()->AppendApplyList(jsonObj["apply_list"].toArray());
        }
        if(jsonObj.contains("friend_list")){
            UserMgr::getInstance()->AppendFriendList(jsonObj["friend_list"].toArray());
        }
        emit sig_switch_chatdlg();

    });

    handlers_.insert(ReqId::ID_SEARCH_USER_RSP,[this](ReqId id,int len,QByteArray data){
        Q_UNUSED(len);

        qDebug()<<"handle id is"<<id<<"data is" <<data;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonObject jsonObj(jsonDoc.object());

        if(!jsonObj.contains("error"))
        {
            int err = ErrorCodes::ERR_JSON;
            qDebug()<<"Search failed,err is json parse err,err is "<<err;
            emit sig_user_search(nullptr);
            return;
        }
        int err = jsonObj["error"].toInt();
        if(err!=ErrorCodes::SUCCESS)
        {
            qDebug()<<"Search failed,err is "<<err;
            emit sig_user_search(nullptr);
            return;
        }
        auto searchinfo = std::make_shared<SearchInfo>(jsonObj["uid"].toInt(),
                jsonObj["name"].toString(),jsonObj["nick"].toString(),jsonObj["desc"].toString(),
                jsonObj["sex"].toInt(),jsonObj["icon"].toString());
        emit sig_user_search(searchinfo);
    });
    handlers_.insert(ReqId::ID_NOTIFY_ADD_FRIEND_REQ,[this](ReqId id,int len,QByteArray data){
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;

        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj(jsonDoc.object());
        if(!jsonObj.contains("error")){
            int err = ErrorCodes::ERR_JSON;
            qDebug()<<"Notify Failed,err is Json parse err"<<err;
            emit sig_user_search(nullptr);
            return;
        }
        int err = jsonObj["error"].toInt();
        if(jsonObj["error"]!=ErrorCodes::SUCCESS){
            qDebug()<<"Notify failed,error is "<<err;
            emit sig_user_search(nullptr);
            return;
        }
        //int from_uid, QString name, QString desc,QString icon, QString nick, int sex
        int from_uid = jsonObj["applyuid"].toInt();
        QString name = jsonObj["name"].toString();
        QString desc = jsonObj["desc"].toString();
        QString icon = jsonObj["icon"].toString();
        QString nick = jsonObj["nick"].toString();
        int sex = jsonObj["sex"].toInt();

       auto apply_info = std::make_shared<AddFriendApply>(
                   from_uid, name, desc,
                     icon, nick, sex);
       qDebug()<<"Notify sig is received";

       emit sig_friend_apply(apply_info);
    });
    handlers_.insert(ReqId::ID_NOTIFY_AUTH_FRIEND_REQ,[this](ReqId id,int len,QByteArray data){
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isNull()){
            qDebug()<<"Failed to parse json data";
            return;
        }
        QJsonObject jsonObj(jsonDoc.object());
        if(!jsonObj.contains("error")){
            int err = ErrorCodes::ERR_JSON;
            qDebug()<<"Auth failed,err is "<<err;
            return;
        }
        int err = jsonObj["error"].toInt();
        if(err!=ErrorCodes::SUCCESS){
            qDebug()<<"Auth friend failed,err is "<<err;
            return;
        }
        int from_uid = jsonObj["fromuid"].toInt();
        QString name = jsonObj["name"].toString();
        QString nick = jsonObj["nick"].toString();
        QString icon = jsonObj["icon"].toString();
        int sex = jsonObj["sex"].toInt();

        //int uid, QString name,QString nick, QString icon, int sex
        auto auth_info = std::make_shared<AuthInfo>(from_uid,name,nick,icon,sex);
        emit sig_add_auth_friend(auth_info);
    });
    handlers_.insert(ReqId::ID_AUTH_FRIEND_RSP,[this](ReqId id,int len,QByteArray data){
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isNull()){
            qDebug()<<"Failed to parse json data";
            return;
        }
        QJsonObject jsonObj(jsonDoc.object());
        if(!jsonObj.contains("error")){
            int err = ErrorCodes::ERR_JSON;
            qDebug()<<"Auth failed,err is "<<err;
            return;
        }
        int err = jsonObj["error"].toInt();
        if(err!=ErrorCodes::SUCCESS){
            qDebug()<<"Auth friend failed,err is "<<err;
            return;
        }

        auto name = jsonObj["name"].toString();
        auto nick = jsonObj["nick"].toString();
        auto icon = jsonObj["icon"].toString();
        auto sex = jsonObj["sex"].toInt();
        auto uid = jsonObj["uid"].toInt();
        auto rsp = std::make_shared<AuthRsp>(uid, name, nick, icon, sex);
        emit sig_auth_rsp(rsp);
    });

    //B所在的服务器会通知B，告诉B有来自A的消息，通知消息为ID_NOTIFY_TEXT_CHAT_MSG_REQ
    handlers_.insert(ReqId::ID_NOTIFY_TEXT_CHAT_MSG_REQ,[this](ReqId id,int len,QByteArray data){
        Q_UNUSED(len);
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isNull()){
            qDebug()<<"Failed to create QJsonDocument.";
            return;
        }
        QJsonObject jsonObj(jsonDoc.object());
        int err = jsonObj["error"].toInt();
        if(!jsonObj.contains("error")){
            qDebug()<<"Chat Msg Rsp Failed, err is Json Parse Err"<<err;
            return;
        }
        if(err!=ErrorCodes::SUCCESS){
            qDebug()<<"Chat Msg Rsp Failed, err is Json Parse Err"<<err;
            return;
        }
        qDebug()<<"receive text chat Notify Success";
        auto msg_ptr = std::make_shared<TextChatMsg>(jsonObj["fromuid"].toInt(),jsonObj["touid"].toInt(),
                                                     jsonObj["text_array"].toArray());
        emit sig_text_chat_msg(msg_ptr);

    });

    //A给B发送文本消息，A所在的服务器会给A发送ID_TEXT_CHAT_MSG_RSP消息。
    handlers_.insert(ReqId::ID_TEXT_CHAT_MSG_RSP,[this](ReqId id,int len,QByteArray data){
        Q_UNUSED(len);
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isNull()){
            qDebug()<<"Failed to create QJsonDocument.";
            return;
        }
        QJsonObject jsonObj(jsonDoc.object());
        int err = jsonObj["error"].toInt();
        if(!jsonObj.contains("error")){
            qDebug()<<"Chat Msg Rsp Failed, err is Json Parse Err"<<err;
            return;
        }
        if(err!=ErrorCodes::SUCCESS){
            qDebug()<<"Chat Msg Rsp Failed, err is Json Parse Err"<<err;
            return;
        }
        qDebug() << "Receive Text Chat Rsp Success " ;
        //ui上做处理，显示已送达之类的
    });
}

void TcpMgr::handleMsg(ReqId id, int len, QByteArray data)
{
    auto iter = handlers_.find(id);
    if(iter == handlers_.end()){
        qDebug()<<"not found id"<<"["<<id<<"]to handle";
        return;
    }
    //执行回调函数
    handlers_[id](id,len,data);
}

void TcpMgr::slot_tcp_connect(ServerInfo si)
{
    qDebug()<<"receive tcp connect signal";
    qDebug()<<"Connection to Server...";
    host_ = si.Host;
    port_ = static_cast<quint16>(si.Port.toUInt());
    qDebug()<<"host: "<<host_<<" port: "<<port_;
    socket_.connectToHost(host_,port_);
}

void TcpMgr::slot_send_data(ReqId reqId, QByteArray data)
{
    uint16_t id = reqId;
    quint16 len = static_cast<quint16>(data.size());

    QByteArray block;
    QDataStream stream(&block,QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_0);

    //设置数据流使用网络字节序
    stream.setByteOrder(QDataStream::BigEndian);

    stream<<id<<len;
    block.append(data);
    socket_.write(block);
}



