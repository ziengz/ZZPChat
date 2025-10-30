#include "findsuccessdlg.h"
#include "ui_findsuccessdlg.h"
#include <QDir>
#include "applyfriend.h"

FindSuccessDlg::FindSuccessDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindSuccessDlg),_parent(parent)
{
    ui->setupUi(this);
    this->setWindowTitle("添加");
    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    this->setObjectName("FindSuccessDlg");

    //获得当前应用程序的路径
    QString app_path = QCoreApplication::applicationDirPath();
    QString pix_path = QDir::toNativeSeparators(app_path+
                                                QDir::separator()+"static"+QDir::separator()+"head_1.jpg");
    QPixmap pix(pix_path);
    pix = pix.scaled(ui->head_lb->size(),
               Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->head_lb->setPixmap(pix);
    ui->add_friend_btn->SetState("normal","hover","press");

    //强制用户关注并响应该窗口
    this->setModal(true);

}

FindSuccessDlg::~FindSuccessDlg()
{
    qDebug()<<"FindSuccessDlg destruct";
    delete ui;
}

void FindSuccessDlg::SetSearchInfo(std::shared_ptr<SearchInfo> si)
{
    ui->name_lb->setText(si->_name);
    _si = si;

}

void FindSuccessDlg::on_add_friend_btn_clicked()
{
    //处理添加朋友逻辑
    this->hide();
    auto applyFriend = new ApplyFriend(_parent);
    applyFriend->setModal(true);
    applyFriend->show();
}
