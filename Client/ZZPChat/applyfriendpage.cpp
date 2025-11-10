#include "applyfriendpage.h"
#include "ui_applyfriendpage.h"
#include <QPainter>
#include <QStyleOption>
#include "usermgr.h"
#include "authenfriend.h"


ApplyFriendPage::ApplyFriendPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplyFriendPage)
{
    ui->setupUi(this);
    connect(ui->apply_friend_list,&ApplyFriendList::sig_show_search,this,&ApplyFriendPage::sig_show_search);
    loadApplyList();
    connect(TcpMgr::getInstance().get(),&TcpMgr::sig_auth_rsp,this,&ApplyFriendPage::slot_auth_rsp);

}

ApplyFriendPage::~ApplyFriendPage()
{
    delete ui;
}

void ApplyFriendPage::addNewApply(std::shared_ptr<AddFriendApply> apply)
{
    //生成0-99随机数
    int randomValue = QRandomGenerator::global()->bounded(100);
    int head_i = randomValue%heads.size();
    auto* apply_item = new ApplyFriendItem;
    auto apply_info = std::make_shared<ApplyInfo>(apply->_from_uid,
             apply->_name, apply->_desc,heads[head_i], apply->_name, 0, 0);
    apply_item->SetInfo(apply_info);
    QListWidgetItem* item = new QListWidgetItem;

    item->setSizeHint(apply_item->sizeHint());
    // 设置单元格不可选中，不可选择
    item->setFlags(item->flags()&~Qt::ItemIsEnabled&~Qt::ItemIsSelectable);
    ui->apply_friend_list->insertItem(0,item);
    ui->apply_friend_list->setItemWidget(item,apply_item);
    //新添加的好友需要加上按钮
    apply_item->ShowAddBtn(true);
    //收到审核好友信号
    connect(apply_item, &ApplyFriendItem::sig_auth_friend, [this](std::shared_ptr<ApplyInfo> apply_info){
        auto*authFriend = new AuthenFriend(this);
        authFriend->SetApplyInfo(apply_info);
        authFriend->setModal(true);
        authFriend->show();
    });

}

void ApplyFriendPage::paintEvent(QPaintEvent *event)
{
    QStyleOption ops;
    ops.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&ops,&p,this);
}

void ApplyFriendPage::loadApplyList()
{
    //添加好友申请
    auto apply_list = UserMgr::getInstance()->GetApplyList();
    for(auto&apply : apply_list){
        int randomValue = QRandomGenerator::global()->bounded(100);
        int head_i = randomValue%heads.size();
        auto* apply_item = new ApplyFriendItem;
        apply->SetIcon(heads[head_i]);
        apply_item->SetInfo(apply);
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(apply_item->sizeHint());
        item->setFlags(item->flags()&~Qt::ItemIsEnabled&~Qt::ItemIsSelectable);

        ui->apply_friend_list->addItem(item);
        ui->apply_friend_list->setItemWidget(item,apply_item);

        if(apply->_status){
            apply_item->ShowAddBtn(false);
        }else{
            apply_item->ShowAddBtn(true);
            auto uid = apply_item->GetUid();
            _unauth_items[uid] = apply_item;
        }

        //收到审核好友信号
        connect(apply_item,&ApplyFriendItem::sig_auth_friend,[this](std::shared_ptr<ApplyInfo>apply_info){
//            auto* authFriend = new AuthenFriend(this);
        });
    }

    for(int i=0;i<13;i++){
        int randomValue = QRandomGenerator::global()->bounded(100);
        int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();

        auto* apply_item = new ApplyFriendItem;
        //先将所有人的是否被添加状态设为“已添加”，后面会根据这个判断显示添加按钮还是“已添加”字样
        auto apply_info = std::make_shared<ApplyInfo>(0,names[name_i],strs[str_i],
                                                      heads[head_i],names[name_i],0,1);
        apply_item->SetInfo(apply_info);
        QListWidgetItem* item = new QListWidgetItem;
        item->setFlags(item->flags()&~Qt::ItemIsEnabled&~Qt::ItemIsSelectable);
        item->setSizeHint(apply_item->sizeHint());
        ui->apply_friend_list->addItem(item);
        ui->apply_friend_list->setItemWidget(item,apply_item);

        if(apply_info->_status){
            apply_item->ShowAddBtn(false);
        }else{
            apply_item->ShowAddBtn(true);
            auto uid = apply_item->GetUid();
            _unauth_items[uid] = apply_item;
        }

        connect(apply_item,&ApplyFriendItem::sig_auth_friend,[this](std::shared_ptr<ApplyInfo>apply_info){
            auto* authFriend = new AuthenFriend(this);
            authFriend->setModal(true);
            authFriend->SetApplyInfo(apply_info);
            authFriend->show();
        });

    }
}

void ApplyFriendPage::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp)
{
    auto uid = auth_rsp->_uid;
    auto find_iter = _unauth_items.find(uid);
    if(find_iter == _unauth_items.end()){
        return;
    }
    //int,ApplyFriendItem
    find_iter->second->ShowAddBtn(true);
}
