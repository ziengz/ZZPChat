
#include "searchlist.h"
#include "tcpmgr.h"
#include "adduseritem.h"
#include "customizeedit.h"
#include "findsuccessdlg.h"
#include "findfaildlg.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "customizeedit.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "usermgr.h"

SearchList::SearchList(QWidget *parent):QListWidget(parent),_send_pending(false),_search_edit(nullptr),_find_dlg(nullptr)
{
    Q_UNUSED(parent);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //安装事件过滤器
    this->viewport()->installEventFilter(this);
    connect(this,&QListWidget::itemClicked,this,&SearchList::slot_item_clicked);
    //添加条目
    addTipItem();
    //连接搜索条目
    connect(TcpMgr::getInstance().get(),&TcpMgr::sig_user_search,this,&SearchList::slot_user_search);
}

void SearchList::CloseFindDlg()
{
    if(_find_dlg){
        _find_dlg->hide();
        _find_dlg = nullptr;
    }
}

void SearchList::setSeatchEdit(QWidget *edit)
{
    _search_edit = edit;
}

void SearchList::waitPending(bool pending)
{
    if(pending){
        _loadingDlg = new LoadingDlg(this);
        _loadingDlg->show();
        _loadingDlg->setModal(true);
        _send_pending = pending;
    }
    else{
        _loadingDlg->hide();
        _loadingDlg->deleteLater();
        _send_pending = pending;
    }
}

void SearchList::addTipItem()
{
    //创建一个分割符
    auto* invalid_item = new QWidget();
    QListWidgetItem*item_tmp = new QListWidgetItem;

    item_tmp->setSizeHint(QSize(250,10));
    this->addItem(item_tmp);
    invalid_item->setObjectName("invalid_item");
    this->setItemWidget(item_tmp,invalid_item);
    item_tmp->setFlags(item_tmp->flags()&~Qt::ItemIsSelectable);

    //把AddUserItem添加进去
    auto*add_user_item = new AddUserItem();
    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(add_user_item->sizeHint());
    this->addItem(item);
    this->setItemWidget(item,add_user_item);
}

void SearchList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget*widget = this->itemWidget(item);
    if(!widget){
        qDebug()<<"slot item clicked widget is nullptr";
        return;
    }
    ListItemBase*customItem = qobject_cast<ListItemBase*>(widget);
    if(customItem==nullptr){
        qDebug()<<"slot item clicked widget is nullptr";
        return;
    }
    ListItemType itemType = customItem->GetItemType();
    if(itemType==ListItemType::INVALID_ITEM){
        qDebug()<<"slot invalid item clicked";
        return;
    }

    if(itemType==ListItemType::ADD_USER_TIP_ITEM){
        if(_send_pending){
           return;
        }
        waitPending(true);
        
        auto search_edit = static_cast<CustomizeEdit*>(_search_edit);
        auto uid_str = search_edit->text();
        QJsonObject jsonObj;
        jsonObj["uid"] = uid_str;
        QJsonDocument jsonDoc(jsonObj);
        //压缩内容发送
        QByteArray jsonData = jsonDoc.toJson(QJsonDocument::Compact);
        emit TcpMgr::getInstance()->sig_send_data(ReqId::ID_SEARCH_USER_REQ,jsonData);
        return;
    }
    CloseFindDlg();
}

void SearchList::slot_user_search(std::shared_ptr<SearchInfo> si)
{
    waitPending(false);
    if(si==nullptr){
        _find_dlg = std::make_shared<FindFailDlg>(this);
    }else{
        //分为两种情况，一种是搜到的已经是自己的
        auto b_exist = UserMgr::getInstance()->CheckFriendById(si->_uid);
        if(b_exist){
            //处理跳转查找用户聊天页面
            emit sig_jump_chat_item(si);
        }


        _find_dlg = std::make_shared<FindSuccessDlg>(this);
        std::dynamic_pointer_cast<FindSuccessDlg>(_find_dlg)->SetSearchInfo(si);
    }
    _find_dlg->show();
}
