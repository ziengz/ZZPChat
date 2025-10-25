#include "tcpmgr.h"
#include <QJsonDocument>
#include <QJsonObject>

#include "usermgr.h"

TcpMgr::TcpMgr():host_(""),port_(0),b_recv_pending(false),
    message_id_(0),message_len_(0)
{
    connect(&socket_,&QTcpSocket::connected,[&](){
        qDebug()<<"Connected to server";
        emit sig_con_success(true);
    });

    initHandlers();
    connect(&socket_,&QTcpSocket::readyRead,[&](){
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

}

void TcpMgr::initHandlers()
{
    //处理登陆聊天服务器回包，用于接收用户信息
    handlers_.insert(ReqId::ID_CHAT_LOGIN_RSP,[this](ReqId id,int len,QByteArray data){
        Q_UNUSED(len);
        qDebug()<<"handle id is "<<id;
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
        UserMgr::getInstance()->SetUid(jsonObj["uid"].toInt());
        UserMgr::getInstance()->SetName(jsonObj["name"].toString());
        UserMgr::getInstance()->SetToken(jsonObj["Token"].toString());

        emit sig_switch_chatdlg();

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
    //qDebug()<<"host: "<<host_<<" port: "<<port_;
    socket_.connectToHost(host_,port_);
}

void TcpMgr::slot_send_data(ReqId reqId, QString data)
{
    uint16_t id = reqId;
    QByteArray dataByte = data.toUtf8();

    quint16 len = static_cast<quint16>(data.size());

    QByteArray block;
    QDataStream stream(&block,QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_0);

    //设置数据流使用网络字节序
    stream.setByteOrder(QDataStream::BigEndian);

    stream<<id<<len;
    block.append(dataByte);
    qDebug()<<block;
    socket_.write(block);
}



