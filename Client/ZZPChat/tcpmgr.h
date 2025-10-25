
#ifndef TCPMGR_H
#define TCPMGR_H
#include <QDataStream>
#include <QTcpSocket>
#include "singleton.h"
#include "global.h"


class TcpMgr:public QObject,public singleton<TcpMgr>,
        public std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
public:
    TcpMgr();
private:
    friend class singleton<TcpMgr>;
    QTcpSocket socket_;
    QString host_;
    quint16 port_;
    QByteArray buffer_;
    bool b_recv_pending;
    quint16 message_id_;
    quint16 message_len_;
    void initHandlers();
    void handleMsg(ReqId id ,int len,QByteArray data);
    QMap<ReqId, std::function<void(ReqId id,int len,QByteArray data)>> handlers_;


public slots:
    void slot_tcp_connect(ServerInfo);
    void slot_send_data(ReqId id,QString data);

signals:
    void sig_con_success(bool bsuccess);
    void sig_send_data(ReqId reqid,QString data);
    void sig_login_failed(int);
    void sig_switch_chatdlg();
};

#endif // TCPMGR_H
