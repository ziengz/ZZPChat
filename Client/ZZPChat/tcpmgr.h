
#ifndef TCPMGR_H
#define TCPMGR_H
#include <QDataStream>
#include <QTcpSocket>
#include "singleton.h"
#include "global.h"
#include "userdata.h"


class TcpMgr:public QObject,public singleton<TcpMgr>,
        public std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
    friend class singleton<TcpMgr>;
public:
    ~TcpMgr();
    void CloseConnection();
private:
    TcpMgr();
    void initHandlers();
    void handleMsg(ReqId id ,int len,QByteArray data);
    QTcpSocket socket_;
    QString host_;
    quint16 port_;
    QByteArray buffer_;
    bool b_recv_pending;
    quint16 message_id_;
    quint16 message_len_;
    QMap<ReqId, std::function<void(ReqId id,int len,QByteArray data)>> handlers_;


public slots:
    void slot_tcp_connect(ServerInfo);
    void slot_send_data(ReqId id,QByteArray data);

signals:
    void sig_con_success(bool bsuccess);
    void sig_send_data(ReqId reqid,QByteArray data);
    void sig_login_failed(int);
    void sig_switch_chatdlg();
    void sig_user_search(std::shared_ptr<SearchInfo>);
    void sig_friend_apply(std::shared_ptr<AddFriendApply>);
    void sig_add_auth_friend(std::shared_ptr<AuthInfo>);
    void sig_auth_rsp(std::shared_ptr<AuthRsp>);
    void sig_text_chat_msg(std::shared_ptr<TextChatMsg>);
    void sig_offline();
};

#endif // TCPMGR_H
