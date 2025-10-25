#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();
signals:
    void sigSwitchLogin();

private slots:
    void on_getCode_btn_clicked();
    void slot_reg_mod_finish(ReqId id,QString res,ErrorCodes err);

    void on_sure_btn_clicked();

    void on_return_btn_clicked();

    void on_cancel_btn_clicked();

private:
    void AddTirErr(TipErr,QString);
    void DelTipErr(TipErr te);
    void showTip(QString str,bool b_ok);
    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkConfirmValid();
    bool checkVarifyValid();
    void ChangeTipPage();

    QTimer *timer_;
    int counter_;
    QMap<TipErr,QString> _tip_err;
    Ui::RegisterDialog *ui;
                                 //使用引用节省传参开销
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handles;
    void initHttpHandlers();
};

#endif // REGISTERDIALOG_H














