
#include "httpmgr.h"

httpmgr::~httpmgr()
{

}

httpmgr::httpmgr()
{
    connect(this,&httpmgr::sig_http_finish,this,&httpmgr::slot_http_finish);
}

void httpmgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    //1、准备请求数据
    QByteArray data = QJsonDocument(json).toJson();
    //2、配置网络
    QNetworkRequest request(url);
    //声明json格式
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    //设置内容长度
    request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(data.length()));

    //3、发送请求 发送请求得到一个回复，可理解为发送快递后得到一个回执单
    QNetworkReply* reply = _manager.post(request,data);

    //关键点：获取当前对象的智能指针，确保回调时仍存在
    auto self = shared_from_this();

    //4、链接信号与槽，当收到回执单后，判断该回执单是否有错
    connect(reply,&QNetworkReply::finished,[self,reply,req_id,mod](){
        //处理错误情况
        if(reply->error()!=QNetworkReply::NoError){
          qDebug()<<"network error:"<<reply->errorString();
          //发送失败信号
          emit self->sig_http_finish(req_id,"",ErrorCodes::ERR_NETWORK,mod);
          //清空资源
          reply->deleteLater();
          return;
        }
        //如果没有错误，获得回复信息
        QString response = reply->readAll();
        emit self->sig_http_finish(req_id,response,ErrorCodes::SUCCESS,mod);
        reply->deleteLater();
        return;
    });

}

void httpmgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    if(mod == Modules::REGISTERMOD)
    {
        emit sig_reg_mod_finish(id,res,err);
    }
    if(mod == Modules::RESETMOD){
        emit sig_reset_mod_finish(id,res,err);
    }
    if(mod==Modules::LOGINMOD){
        emit sig_login_mod_finish(id,res,err);
    }
}

