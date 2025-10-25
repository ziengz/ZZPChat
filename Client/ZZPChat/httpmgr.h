
#ifndef HTTPMGR_H
#define HTTPMGR_H
#include <singleton.h>
#include <QString>
#include <QUrl>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

//CRTP：一个类X继承自一个模板类Y，并将自身作为模板参数传递给Y
class httpmgr:public QObject,public singleton<httpmgr>,
        public std::enable_shared_from_this<httpmgr>
{
   Q_OBJECT
public:
    ~httpmgr();
private:
    friend class singleton<httpmgr>;
    httpmgr();
    QNetworkAccessManager  _manager;  //类似快递员
public:
    void PostHttpReq(QUrl url,QJsonObject json,ReqId req_id,Modules mod);

public slots:
    void slot_http_finish(ReqId id,QString res,ErrorCodes err,Modules mod);
signals:
    void sig_http_finish(ReqId id,QString res,ErrorCodes err,Modules mod);
    void sig_reg_mod_finish(ReqId id,QString res,ErrorCodes err);
    void sig_reset_mod_finish(ReqId id,QString res,ErrorCodes err);
    void sig_login_mod_finish(ReqId id,QString res,ErrorCodes err);
};

#endif // HTTPMGR_H
