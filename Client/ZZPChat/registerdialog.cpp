#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmgr.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),counter_(5),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

    //设置密码框为密码模式
    ui->pass_lineEdit->setEchoMode(QLineEdit::Password);
    ui->again_lineEdit->setEchoMode(QLineEdit::Password);

    ui->err_tip->setProperty("state","normal");
    repolish(ui->err_tip);
    initHttpHandlers();

    connect(httpmgr::getInstance().get(),&httpmgr::sig_reg_mod_finish,this,&RegisterDialog::slot_reg_mod_finish);

    ui->err_tip->clear();
    connect(ui->user_lineEdit,&QLineEdit::editingFinished,this,[this](){
        checkUserValid();
    });
    connect(ui->email_lineEdit, &QLineEdit::editingFinished, this, [this](){
        checkEmailValid();
    });
    connect(ui->pass_lineEdit, &QLineEdit::editingFinished, this, [this](){
        checkPassValid();
    });
    connect(ui->again_lineEdit, &QLineEdit::editingFinished, this, [this](){
        checkConfirmValid();
    });
    connect(ui->code_lineEdit, &QLineEdit::editingFinished, this, [this](){
        checkVarifyValid();
    });

    ui->pass_visible->setCursor(Qt::PointingHandCursor);
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);

    ui->pass_visible->SetState("unvisible","unvisible_hover","","visible",
                                "visible_hover","");
    ui->confirm_visible->SetState("unvisible","unvisible_hover","","visible",
                                    "visible_hover","");


    connect(ui->pass_visible,&ClickedLabel::Click,[this](){
        ClickLbState state = ui->pass_visible->GetCurState();
        if(state==ClickLbState::Normal)
        {
            ui->pass_lineEdit->setEchoMode(QLineEdit::Password);
        }else{
            ui->pass_lineEdit->setEchoMode(QLineEdit::Normal);
        }
        qDebug()<<"label was clicked";
    });

    connect(ui->confirm_visible,&ClickedLabel::Click,[this](){
        ClickLbState state = ui->confirm_visible->GetCurState();
        if(state==ClickLbState::Normal)
        {
            ui->again_lineEdit->setEchoMode(QLineEdit::Password);
        }else{
            ui->again_lineEdit->setEchoMode(QLineEdit::Normal);
        }
        qDebug()<<"label was clicked";

    });

    timer_ = new QTimer(this);
    connect(timer_,&QTimer::timeout,[this](){
        counter_--;
        if(counter_==0){
            timer_->stop();
            emit sigSwitchLogin();
            return;
        }
        auto str = QString("注册成功，%1 s后返回登录").arg(counter_);
        ui->tip_lb->setText(str);
    });

}

RegisterDialog::~RegisterDialog()
{
    qDebug()<<"registerDialog is destoryed";
    delete ui;
}

void RegisterDialog::on_getCode_btn_clicked()
{
    QString email = ui->email_lineEdit->text();
    bool match = checkEmailValid();
    if(match)
    {
        //发送验证码
        QJsonObject json_obj;
        json_obj["email"] = email;
        httpmgr::getInstance()->PostHttpReq(QUrl(gate_url_prefix+"/get_varifycode"),
                                json_obj,ReqId::ID_GET_VARIFY_CODE,Modules::REGISTERMOD);
    }
    else{
        showTip(tr("邮箱地址不正确"),false);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS)
    {
        showTip(tr("网络请求错误"),false);
        return;
    }
    //解析JSON字符串，res需转换为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    //如果json解析错误
    if(jsonDoc.isNull() || !jsonDoc.isObject())
    {
       showTip(tr("json解析错误"),false);
       return;
    }
    //使用map类型 值为函数
    _handles[id](jsonDoc.object());
    return;

}

//初始化map对象
void RegisterDialog::initHttpHandlers()
{
    //注册获取验证码回包逻辑
    _handles.insert(ReqId::ID_GET_VARIFY_CODE,[this](const QJsonObject& jsonObj){
        auto error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS)
        {
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送，请注意查收"),true);
        qDebug()<<"1、email is "<<email;
        return;
    });

    _handles.insert(ReqId::ID_REG_USER,[this](const QJsonObject&jsonObj){
        auto error = jsonObj["error"].toInt();
        if(error!=ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            return;
        }
        auto uid = jsonObj["uid"].toInt();
        auto email = jsonObj["email"].toString();
        showTip(tr("注册成功"),true);
        qDebug()<<"uid = "<<uid<<"email: "<<email;
        ChangeTipPage();
        return;
    });
}

void RegisterDialog::showTip(QString str, bool b_ok)
{
    if(b_ok)
    {
        ui->err_tip->setProperty("state","normal");
    }else{
        ui->err_tip->setProperty("state","err");
    }
    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}

void RegisterDialog::AddTirErr(TipErr te, QString str)
{
    _tip_err[te] = str;
    showTip(str,false);
}

void RegisterDialog::DelTipErr(TipErr te)
{
    _tip_err.remove(te);
    if(_tip_err.empty()){
        ui->err_tip->clear();
        return;
    }
    showTip(_tip_err.first(),false);
}



bool RegisterDialog::checkUserValid()
{
    QString user = ui->user_lineEdit->text();
    if(user==""){
        AddTirErr(TipErr::TIP_USER_ERR,"用户名不能为空");
        return false;
    }
    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialog::checkEmailValid()
{
    QString email = ui->email_lineEdit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(!match){
        AddTirErr(TipErr::TIP_EMAIL_ERR,"邮箱地址不正确");
        return false;
    }
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool RegisterDialog::checkPassValid()
{
    auto pass = ui->pass_lineEdit->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        AddTirErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        AddTirErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;;
    }
    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

bool RegisterDialog::checkConfirmValid()
{
    auto pass = ui->pass_lineEdit->text();
    auto confirm = ui->again_lineEdit->text();

    if(confirm.length() < 6 || confirm.length() > 15 ){
        //提示长度不准确
        AddTirErr(TipErr::TIP_CONFIRM_ERR, tr("密码长度应为6~15"));
        return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{6,15}$");
    bool match = regExp.match(confirm).hasMatch();
    if(!match){
        //提示字符非法
        AddTirErr(TipErr::TIP_CONFIRM_ERR, tr("不能包含非法字符"));
        return false;
    }

    DelTipErr(TipErr::TIP_CONFIRM_ERR);

    if(pass != confirm){
        //提示密码不匹配
        AddTirErr(TipErr::TIP_PWD_CONFIRM, tr("确认密码和密码不匹配"));
        return false;
    }else{
        DelTipErr(TipErr::TIP_PWD_CONFIRM);
    }
    return true;
}

bool RegisterDialog::checkVarifyValid()
{
    auto varify_code = ui->code_lineEdit->text();
    if(varify_code==""){
        AddTirErr(TipErr::TIP_VARIFY_ERR,"验证码不能为空");
        return false;
    }
    DelTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

void RegisterDialog::ChangeTipPage()
{
//    timer->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    timer_->start(1000);
}



void RegisterDialog::on_sure_btn_clicked()
{
    bool valid = checkUserValid();
    if(!valid){
        return;
    }
    valid = checkEmailValid();
    if(!valid){
        return;
    }
    valid = checkPassValid();
    if(!valid){
        return;
    }
    valid = checkVarifyValid();
    if(!valid){
        return;
    }
        //day11 发送http请求注册用户
        QJsonObject json_obj;
        json_obj["user"] = ui->user_lineEdit->text();
        json_obj["email"] = ui->email_lineEdit->text();
        json_obj["passwd"] = xorString(ui->pass_lineEdit->text());
        json_obj["confirm"] = xorString(ui->again_lineEdit->text());
        json_obj["varifycode"] = ui->code_lineEdit->text();
        httpmgr::getInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                            json_obj, ReqId::ID_REG_USER,Modules::REGISTERMOD);
}




void RegisterDialog::on_return_btn_clicked()
{
    timer_->stop();
    emit sigSwitchLogin();
}


void RegisterDialog::on_cancel_btn_clicked()
{
    qDebug()<<"return to loginDialog";
    emit sigSwitchLogin();
}

