
#include "contactuserlist.h"
#include <QScrollBar>
#include "conuseritem.h"
#include "grouptipitem.h"
#include <QRandomGenerator>

ContactUserList::ContactUserList(QWidget*parent)
{
    Q_UNUSED(parent);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //安装过滤器
    this->viewport()->installEventFilter(this);
    addContactUserList();
    connect(this,&QListWidget::itemClicked,this,&ContactUserList::slot_item_clicked);


}

void ContactUserList::ShowRedPoint(bool bshow)
{
    _add_friend_item->ShowRedPoint(bshow);
    repolish(this);
    update();
}

bool ContactUserList::eventFilter(QObject *object, QEvent *event)
{
    //鼠标进入离开事件
    if(object==this->viewport()){
        if(event->type()==QEvent::Enter){
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }else if(event->type()==QEvent::Leave){
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }
    //滚轮事件
    if(object==this->viewport()&&event->type()==QEvent::Wheel){
        QWheelEvent*wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y()/8;
        int numSteps = numDegrees/15;

        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()-numSteps);
        QScrollBar* scrollBar = this->verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();
        int currentValue = scrollBar->value();

        if(maxScrollValue-currentValue<=0){
            //加载新的联系人
            qDebug()<<"load more contact user";
            emit sig_loading_contact_user();
        }
        return true;
    }
    return QListWidget::eventFilter(object,event);
}

void ContactUserList::addContactUserList()
{
    auto * groupTip = new GroupTipItem;
    QListWidgetItem*item = new QListWidgetItem;
    item->setSizeHint(groupTip->sizeHint());
    this->addItem(item);
    this->setItemWidget(item,groupTip);
    item->setFlags(item->flags()&~Qt::ItemIsSelectable);

    _add_friend_item = new ConUserItem();
    _add_friend_item->setObjectName("new_user_item");
    _add_friend_item->SetInfo(0,tr("新的朋友"),":/res/add_friend.png");
    _add_friend_item->SetItemType(ListItemType::APPLY_FRIEND_ITEM);

    QListWidgetItem* add_item = new QListWidgetItem;
    add_item->setSizeHint(_add_friend_item->sizeHint());
    this->addItem(add_item);
    this->setItemWidget(add_item,_add_friend_item);
    //设置默认选中
    this->setCurrentItem(add_item);

    auto*groupCon = new GroupTipItem;
    groupCon->SetGroupTip(tr("联系人"));
    _groupitem = new QListWidgetItem;
    _groupitem->setSizeHint(groupCon->sizeHint());
    this->addItem(_groupitem);
    this->setItemWidget(_groupitem,groupCon);
    _groupitem->setFlags(_groupitem->flags()&~Qt::ItemIsSelectable);

    for(int i=0;i<13;i++){
        int randomValue = QRandomGenerator::global()->bounded(100);  //生成0-99随机数字
        int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();

        auto* con_user_wid = new ConUserItem();
        con_user_wid->SetInfo(0,names[name_i],heads[head_i]);
        QListWidgetItem*item = new QListWidgetItem;
        item->setSizeHint(con_user_wid->sizeHint());
        this->addItem(item);
        this->setItemWidget(item,con_user_wid);
    }
}

void ContactUserList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget * widget = this->itemWidget(item);
    if(!widget){
        qDebug()<<"slot item clicked widget is nullptr";
        return;
    }
    ListItemBase*customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        qDebug()<<"slot item clicked widget is nullptr";
        return;
    }

    ListItemType itemType = customItem->GetItemType();
    if(itemType==ListItemType::INVALID_ITEM||itemType==ListItemType::GROUP_TIP_ITEM){
        qDebug()<<"slot invalid item clicked";
        return;
    }
    if(itemType==ListItemType::APPLY_FRIEND_ITEM){
        qDebug()<<"apply friend item clicked";
        emit sig_switch_apply_friend_page();
        return;
    }

    if(itemType==ListItemType::CONTACT_USER_ITEM){
        qDebug()<<"contact user item clicked";
        emit sig_switch_friend_info_page();
        return;
    }
}


















