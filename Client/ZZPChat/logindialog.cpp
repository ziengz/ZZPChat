#include "logindialog.h"
#include "ui_logindialog.h"
#include <QPainter>
#include "tcpmgr.h"
#include <QPainterPath>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->reg_btn,&QPushButton::clicked,this,&LoginDialog::switchRegister);
    ui->forget_label->SetState("normal","hover","","selected","selected_hover","");

    connect(ui->forget_label,&ClickedLabel::Click,this,&LoginDialog::slot_forget_pwd);
    initHead();
    initHttpHandlers();
    connect(ui->email_lineEdit,&QLineEdit::editingFinished,this,&LoginDialog::checkUserValid);
    connect(ui->pass_lineEdit,&QLineEdit::editingFinished,this,&LoginDialog::checkPwdValid);

    connect(httpmgr::getInstance().get(),&httpmgr::sig_login_mod_finish,this,&LoginDialog::slot_login_mod_finish);

    connect(this,&LoginDialog::sig_connect_tcp,TcpMgr::getInstance().get(),&TcpMgr::slot_tcp_connect);
    connect(TcpMgr::getInstance().get(),&TcpMgr::sig_con_success,this,&LoginDialog::slot_tcp_con_finish);
    connect(TcpMgr::getInstance().get(),&TcpMgr::sig_login_failed,this,&LoginDialog::slot_login_failed);
}

void LoginDialog::initHead()
{
    // 加载图片
    QPixmap originalPixmap(":/res/head_1.jpg");
      // 设置图片自动缩放
    qDebug()<< originalPixmap.size() << ui->head_label->size();
    originalPixmap = originalPixmap.scaled(ui->head_label->size(),
            Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 创建一个和原始图片相同大小的QPixmap，用于绘制圆角图片
    QPixmap roundedPixmap(originalPixmap.size());
    roundedPixmap.fill(Qt::transparent); // 用透明色填充

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing); // 设置抗锯齿，使圆角更平滑
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // 使用QPainterPath设置圆角
    QPainterPath path;
    path.addRoundedRect(0, 0, originalPixmap.width(), originalPixmap.height(), 10, 10); // 最后两个参数分别是x和y方向的圆角半径
    painter.setClipPath(path);

    // 将原始图片绘制到roundedPixmap上
    painter.drawPixmap(0, 0, originalPixmap);

    // 设置绘制好的圆角图片到QLabel上
    ui->head_label->setPixmap(roundedPixmap);

}

LoginDialog::~LoginDialog()
{
    qDebug()<<"loginDialog is destoryed";
    delete ui;
}

void LoginDialog::slot_forget_pwd()
{
    qDebug()<<"slot forget pwd";
    emit switchReset();
}

void LoginDialog::showTip(QString str, bool b_ok)
{
    if(b_ok){
         ui->err_tip->setProperty("state","normal");
    }else{
        ui->err_tip->setProperty("state","err");
    }

    ui->err_tip->setText(str);

    repolish(ui->err_tip);
}

void LoginDialog::AddTipErr(TipErr err, QString str)
{
    _tip_err[err] = str;
    showTip(str,false);
}

void LoginDialog::DelTipErr(TipErr te)
{
    _tip_err.remove(te);
    if(_tip_err.isEmpty()){
        ui->err_tip->clear();
        return;
    }
    showTip(_tip_err.first(),false);
}

void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err!=ErrorCodes::SUCCESS){
        showTip("网络请求错误",false);
        return;
    }
    QJsonDocument json_Doc = QJsonDocument::fromJson(res.toUtf8());
    if(json_Doc.isNull()||!json_Doc.isObject()){
        showTip("json解析错误",false);
        return;
    }
    _handles[id](json_Doc.object());
    return;
}

void LoginDialog::slot_tcp_con_finish(bool success)
{
    if(success){
        showTip("聊天服务器连接成功，正在登陆...",true);
        QJsonObject jsonObj;
        jsonObj["uid"] = _uid;
        jsonObj["token"] = _token;

        QJsonDocument jsonDoc(jsonObj);
        QString json_str = jsonDoc.toJson(QJsonDocument::Indented);

        emit TcpMgr::getInstance()->sig_send_data(ReqId::ID_CHAT_LOGIN,json_str);
    }else{
        showTip("网络错误",false);
    }
}

void LoginDialog::slot_login_failed(int err)
{
    QString result = QString("登录失败，err is %1").arg(err);
    showTip(result,false);

}

void LoginDialog::initHttpHandlers()
{
    _handles.insert(ReqId::ID_LOGIN_USER,[this](QJsonObject jsonObj){
        auto error = jsonObj["error"].toInt();
        if(error!=ErrorCodes::SUCCESS){
            showTip("参数错误",false);
            return;
        }
        auto email = jsonObj["email"].toString();

        ServerInfo si;
        si.Uid = jsonObj["uid"].toInt();
        si.Host = jsonObj["host"].toString();
        si.Port = jsonObj["port"].toString();
        si.Token = jsonObj["token"].toString();

        _uid = si.Uid;
        _token = si.Token;
        qDebug()<< "email is " << email << " uid is " << si.Uid <<" host is "
                << si.Host << " Port is " << si.Port << " Token is " << si.Token;
        showTip(tr("登录成功"), true);
        emit sig_connect_tcp(si);
    });
}

bool LoginDialog::checkUserValid()
{
    auto user = ui->email_lineEdit->text();
    if(user.isEmpty()){
        AddTipErr(TipErr::TIP_USER_ERR,"用户名不能为空");
        return false;
    }
    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool LoginDialog::checkPwdValid()
{
    auto pwd = ui->pass_lineEdit->text();
    if(pwd.length()<6||pwd.length()>15){
        qDebug()<<"pwd length invalid";
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{6,15}$");
    bool match = regExp.match(pwd).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符且长度为(6~15)"));
        return false;;
    }

    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

void LoginDialog::on_login_btn_clicked()
{
    qDebug()<<"login button is clicked";
    if(checkPwdValid()==false){
        return;
    }
    if(checkUserValid()==false){
        return;
    }
    auto email = ui->email_lineEdit->text();
    auto pwd = ui->pass_lineEdit->text();
    QJsonObject json_obj;
    json_obj["email"] = email;
    json_obj["passwd"] = xorString(pwd);
    httpmgr::getInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_login"),json_obj,
                ReqId::ID_LOGIN_USER,Modules::LOGINMOD);
}






