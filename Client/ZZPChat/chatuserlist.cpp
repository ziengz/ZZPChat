
#include "chatuserlist.h"

ChatUserList::ChatUserList(QWidget *parent):QListWidget(parent)
{
    Q_UNUSED(parent);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->viewport()->installEventFilter(this);
}

bool ChatUserList::eventFilter(QObject *watch, QEvent *event)
{
    //检测鼠标是否进入离开
    if(watch==this->viewport()){
        if(event->type()==QEvent::Enter){
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }
        else if(event->type()==QEvent::Leave){
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    //检测滚轮是否在listWidget区域滚动事件
    if(watch==this->viewport()&&event->type()==QEvent::Wheel){
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        //angleDelta().y() 返回垂直方向的滚轮角度变化（以1/8度为单位）
        int numDegrees = wheelEvent->angleDelta().y()/8;
        //除以15是因为标准鼠标滚轮每格为15度
        int numSteps = numDegrees/15;

        //设置滚动幅度
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()-numSteps);
        QScrollBar*scrollBar = this->verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();
        int currentValue = scrollBar->value();

        if(maxScrollValue-currentValue<=0){
            qDebug()<<"load more chat user";
            emit sig_loading_chat_user();
        }
        return true;
    }
    return QListWidget::eventFilter(watch,event);
}
