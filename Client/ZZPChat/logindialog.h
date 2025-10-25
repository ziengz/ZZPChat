#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "httpmgr.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

signals:
    void switchRegister();
    void switchReset();
    void sig_connect_tcp(ServerInfo);

public slots:
    void slot_forget_pwd();
    void slot_login_mod_finish(ReqId id,QString res,ErrorCodes err);
    void slot_tcp_con_finish(bool success);
    void slot_login_failed(int err);

private slots:
    void on_login_btn_clicked();

private:
    void initHead();
    bool checkUserValid();
    bool checkPwdValid();
    void initHttpHandlers();
    void showTip(QString str,bool b_ok);
    void AddTipErr(TipErr,QString);
    void DelTipErr(TipErr te);
    int _uid;
    QString _token;

    QMap<TipErr,QString> _tip_err;
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handles;
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
