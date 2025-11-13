#include "conuseritem.h"
#include "ui_conuseritem.h"
#include <QRandomGenerator>

ConUserItem::ConUserItem(QWidget *parent) :
    ListItemBase(parent),
    ui(new Ui::ConUserItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::CONTACT_USER_ITEM);
    ui->red_point->raise();
    ShowRedPoint(false);
}

ConUserItem::~ConUserItem()
{
    delete ui;
}

QSize ConUserItem::sizeHint() const
{
    return QSize(250,70);
}

void ConUserItem::SetInfo(std::shared_ptr<AuthInfo> auth_info)
{
    _info = std::make_shared<UserInfo>(auth_info);
    int random = QRandomGenerator::global()->bounded(100);
    auto head_i = random % heads.size();
    if(_info->_icon.isEmpty()){
        _info->_icon = heads[head_i];
    }
    QPixmap pixmap(_info->_icon);
    //设置图片自动缩放
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);
    ui->user_name_lb->setText(_info->_name);

}

void ConUserItem::SetInfo(std::shared_ptr<AuthRsp> auth_info)
{
    _info = std::make_shared<UserInfo>(auth_info);

    int random = QRandomGenerator::global()->bounded(100);
    auto head_i = random % heads.size();
    if(_info->_icon.isEmpty()){
        _info->_icon = heads[head_i];
    }

    QPixmap pixmap(_info->_icon);
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);

    ui->user_name_lb->setText(_info->_name);
}

void ConUserItem::SetInfo(int uid, QString name, QString icon)
{
    _info = std::make_shared<UserInfo>(uid,name,icon);
    int random = QRandomGenerator::global()->bounded(100);
    auto head_i = random % heads.size();
    if(_info->_icon.isEmpty()){
        _info->_icon = heads[head_i];
    }
    QPixmap pixmap(_info->_icon);
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);
    ui->user_name_lb->setText(_info->_name);
}

void ConUserItem::ShowRedPoint(bool show)
{
    if(show){
        ui->red_point->show();
    }else{
        ui->red_point->hide();
    }
}
