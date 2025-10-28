
#include "clickedlabel.h"


ClickedLabel::ClickedLabel(QWidget *parent):QLabel(parent),curstate_(ClickLbState::Normal)
{
    this->setCursor(Qt::PointingHandCursor);
}

void ClickedLabel::mousePressEvent(QMouseEvent *e)
{
    //鼠标点击只会改变两种状态
    if(e->button()==Qt::LeftButton){
        if(curstate_==ClickLbState::Normal){
            qDebug()<<"clicked,change to selected hover:"<<select_press_;
            curstate_=ClickLbState::Selected;
            this->setProperty("state",select_press_);
            repolish(this);
            this->update();
        }
        else{
            qDebug()<<"clicked,Change to normal hover:"<<normal_press_;
            curstate_ = ClickLbState::Normal;
            this->setProperty("state",normal_press_);
            repolish(this);
            this->update();
        }
        emit Click();
        return;
    }
    QLabel::mousePressEvent(e);
}

void ClickedLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(curstate_ == ClickLbState::Normal){
            this->setProperty("state",normal_hover_);
            repolish(this);
            update();
        }
        else{
            this->setProperty("state",select_hover_);
            repolish(this);
            update();
        }
        emit Click(this->text(),curstate_);
        return;
    }
    QLabel::mouseReleaseEvent(event);
}

//处理鼠标悬停进入事件
void ClickedLabel::enterEvent(QEnterEvent *event)
{
    if(curstate_==ClickLbState::Normal)
    {
        qDebug()<<"enter,change to normal hover:"<<normal_hover_;
        this->setProperty("state",normal_hover_);
        repolish(this);
        this->update();
    }
    else{
        qDebug()<<"enter,change to selected hover:"<<select_hover_;
        this->setProperty("state",select_hover_);
        repolish(this);
        this->update();
    }
    QLabel::enterEvent(event);
}

void ClickedLabel::leaveEvent(QEvent *event)
{
    if(curstate_ == ClickLbState::Normal){
            qDebug()<<"leave , change to normal : "<< normal_;
            this->setProperty("state",normal_);
            repolish(this);
            this->update();
        }else{
            qDebug()<<"leave , change to normal hover: "<< select_;
            this->setProperty("state",select_);
            repolish(this);
            this->update();
        }
    QLabel::leaveEvent(event);
}

void ClickedLabel::SetState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{
    normal_ = normal;
    normal_hover_ = hover;
    normal_press_ = press;
    select_ = select;
    select_hover_ = select_hover;
    select_press_ = select_press;

    this->setProperty("state",normal);
    repolish(this);
}

ClickLbState ClickedLabel::GetCurState()
{
    return curstate_;
}






























