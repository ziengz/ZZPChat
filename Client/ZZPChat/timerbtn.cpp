
#include "timerbtn.h"
#include <QMouseEvent>

TimerBtn::TimerBtn(QWidget *parent):QPushButton(parent),count_(10)
{
    time_ = new QTimer(this);
    connect(time_,&QTimer::timeout,[this](){
        count_--;
        if(count_<=0){
            time_->stop();
            this->setEnabled(true);
            count_=10;
            this->setText("获取");
            return;
        }
        this->setEnabled(false);
        this->setText(QString::number(count_));

    });
}

TimerBtn::~TimerBtn()
{
    time_->stop();
}

//处理鼠标左键松开时间
void TimerBtn::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton){
        qDebug()<<"get code button is released";
        this->setEnabled(false);
        this->setText(QString::number(count_));
        time_->start(1000);     //隔一秒触发一次
        emit clicked();
    }
    QPushButton::mouseReleaseEvent(e);
}

