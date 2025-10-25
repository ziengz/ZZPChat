
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _log_Dlg = new LoginDialog(this);
    //去掉窗口的标题栏
    _log_Dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    setCentralWidget(_log_Dlg);

    _log_Dlg->show();


    //连接登录界面注册信号
    connect(_log_Dlg, &LoginDialog::switchRegister, this, &MainWindow::slotSwitchReg);
    //连接登录姐买你忘记密码信号
    connect(_log_Dlg,&LoginDialog::switchReset,this,&MainWindow::slotSwitchReset);
    //连接创建聊天界面信号
    connect(TcpMgr::getInstance().get(),&TcpMgr::sig_switch_chatdlg,this,&MainWindow::slotSwitchChat);

    //测试用直接跳到聊天界面
    emit TcpMgr::getInstance()->sig_switch_chatdlg();
}

MainWindow::~MainWindow()
{
    delete ui;
//    if(_log_Dlg)
//    {
//        delete _log_Dlg;
//        _log_Dlg = nullptr;
//    }
//    if(_reg_dlg)
//    {
//        delete _reg_dlg;
//        _reg_dlg = nullptr;
//    }
}
//切换注册页面
void MainWindow::slotSwitchReg()
{
    _reg_dlg = new RegisterDialog(this);
    _reg_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    connect(_reg_dlg,&RegisterDialog::sigSwitchLogin,this,&MainWindow::slotSwitchLogin);
    setCentralWidget(_reg_dlg);
    _log_Dlg->hide();
    _reg_dlg->show();
}

//从注册页面发挥到登陆页面
void MainWindow::slotSwitchLogin()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _log_Dlg = new LoginDialog(this);
    _log_Dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_log_Dlg);

   _reg_dlg->hide();
    _log_Dlg->show();
    //连接登录界面注册信号
    connect(_log_Dlg, &LoginDialog::switchRegister, this, &MainWindow::slotSwitchReg);
    connect(_log_Dlg,&LoginDialog::switchReset,this,&MainWindow::slotSwitchReset);
}

void MainWindow::slotSwitchReset()
{
    _res_dlg = new ResetDialog(this);
    _log_Dlg->hide();
    _res_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    this->setCentralWidget(_res_dlg);
    _res_dlg->show();
    connect(_res_dlg,&ResetDialog::switchLogin,this,&MainWindow::slotSwitchLogin2);

}

void MainWindow::slotSwitchLogin2()
{
    _log_Dlg = new LoginDialog(this);
    _log_Dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    this->setCentralWidget(_log_Dlg);

    _res_dlg->hide();
    _log_Dlg->show();
    connect(_log_Dlg,&LoginDialog::switchRegister,this,&MainWindow::slotSwitchReg);
    connect(_log_Dlg,&LoginDialog::switchReset,this,&MainWindow::slotSwitchReset);
}

void MainWindow::slotSwitchChat()
{
    _chat_dlg = new ChatDialog(this);
    _chat_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    this->setCentralWidget(_chat_dlg);

    _chat_dlg->show();
    _log_Dlg->hide();
    this->setMinimumSize(QSize(1050,900));
    this->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
}




























