
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"
#include "resetdialog.h"
#include "chatdialog.h"
#include "tcpmgr.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void slotSwitchReg();
    void slotSwitchLogin();
    void slotSwitchReset();
    void slotSwitchLogin2();
    void slotSwitchChat();


private:
    Ui::MainWindow *ui;
    LoginDialog* _log_Dlg;
    RegisterDialog* _reg_dlg;
    ResetDialog* _res_dlg;
    ChatDialog* _chat_dlg;
};

#endif // MAINWINDOW_H
