#include "resetdialog.h"
#include "ui_resetdialog.h"
#include "httpmgr.h"

ResetDialog::ResetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResetDialog)
{
    ui->setupUi(this);
    connect(ui->user_edit,&QLineEdit::editingFinished,this,[this](){
        checkUserValid();
    });
    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this](){
        checkEmailValid();
    });
    connect(ui->pwd_edit, &QLineEdit::editingFinished, this, [this](){
        checkPassValid();
    });
    connect(ui->varify_edit, &QLineEdit::editingFinished, this, [this](){
         checkVarifyValid();
    });

    initHandlers();
    connect(httpmgr::getInstance().get(),&httpmgr::sig_reset_mod_finish,this,&ResetDialog::slot_reset_mod_finish);


}

ResetDialog::~ResetDialog()
{
    delete ui;
}

bool ResetDialog::checkUserValid()
{
    if(ui->user_edit->text()==""){
        AddTipErr(TipErr::TIP_USER_ERR,"用户名不能为空");
        return false;
    }
    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool ResetDialog::checkPassValid()
{
    auto pass = ui->pwd_edit->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;;
    }
    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

void ResetDialog::showTip(QString str, bool b_ok)
{
    if(b_ok){
        ui->err_tip->setProperty("state","normal");
    }else{
        ui->err_tip->setProperty("state","err");
    }
    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}

bool ResetDialog::checkEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->email_edit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(!match){
        //提示邮箱不正确
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool ResetDialog::checkVarifyValid()
{
    if(ui->varify_edit->text()==""){
        AddTipErr(TipErr::TIP_VARIFY_ERR,"验证码不能为空");
        return false;
    }
    DelTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

void ResetDialog::AddTipErr(TipErr te, QString tips)
{
    tip_err_[te] = tips;
    showTip(tips,false);
}

void ResetDialog::DelTipErr(TipErr te)
{
    tip_err_.remove(te);
    if(tip_err_.empty()){
        ui->err_tip->clear();
        return;
    }
    showTip(tip_err_.first(),false);
}

void ResetDialog::initHandlers()
{
    //注册获取邮箱的回包逻辑
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE,[this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error!=ErrorCodes::SUCCESS){
            showTip("参数错误",false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送到邮箱，注意查收"), true);
        qDebug()<<"email is "<<email;
        return;

    });

    //注册注册用户回包逻辑
    _handlers.insert(ReqId::ID_RESET_PWD,[this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error!=ErrorCodes::SUCCESS)
        {
            showTip("参数错误",false);
            return;
        }
        auto email = jsonObj["email"].toString();
        qDebug()<<"email is "<<email;
        showTip("重置成功，点击返回登陆",true);
        qDebug()<<"uid is "<<jsonObj["uid"].toInt();
        return;
    });
}

void ResetDialog::on_varify_btn_clicked()
{
    auto email = ui->email_edit->text();
    bool match = checkEmailValid();
    if(!match){
        return;
    }
    QJsonObject json_obj;
    json_obj["email"] = email;
    httpmgr::getInstance()->PostHttpReq(QUrl(gate_url_prefix+"/get_varifycode"),
                                    json_obj,ReqId::ID_GET_VARIFY_CODE,Modules::RESETMOD);

}

void ResetDialog::slot_reset_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err!=ErrorCodes::SUCCESS){
        showTip("网络请求错误",false);
        return;
    }
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull()||!jsonDoc.isObject()){
        showTip("JSON解析错误",false);
        return;
    }
    _handlers[id](jsonDoc.object());   //执行回调函数
    return;
}


void ResetDialog::on_sure_btn_clicked()
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
    //发送http重置用户请求
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = xorString(ui->pwd_edit->text());
    json_obj["varifycode"] = ui->varify_edit->text();
    httpmgr::getInstance()->PostHttpReq(QUrl(gate_url_prefix+"/reset_pwd"),
                 json_obj, ReqId::ID_RESET_PWD,Modules::RESETMOD);
}


void ResetDialog::on_return_btn_clicked()
{
    qDebug()<<"return to loginDialog";
    emit switchLogin();
}

