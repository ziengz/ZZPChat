
#include "applyfriendlist.h"
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>

ApplyFriendList::ApplyFriendList(QWidget *parent):QListWidget(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->viewport()->installEventFilter(this);
}

bool ApplyFriendList::eventFilter(QObject *object, QEvent *event)
{
    //鼠标进入离开事件
    if(object==this->viewport()){
        if(event->type() == QEvent::Enter){
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }
        else if(event->type() == QEvent::Leave){
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }
    if(object == this->viewport()){
        if(event->type() == QEvent::MouseButtonPress){
            emit sig_show_search(false);
        }
    }
    //处理滚轮事件
    if(object == this->viewport()&&event->type()==QEvent::Wheel){
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegress = wheelEvent->angleDelta().y()/8;
        int numSteps = numDegress/15;

        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()-numSteps);
        return true;
    }
    return QListWidget::eventFilter(object,event);
}

