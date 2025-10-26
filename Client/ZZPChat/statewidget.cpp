
#include "statewidget.h"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>

StateWidget::StateWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setCursor(Qt::PointingHandCursor);
    AddRedPoint();
}

void StateWidget::SetState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{
    _normal = normal;
    _normal_hover = hover;
    _normal_press = press;
    _selected = select;
    _selected_hover = select_hover;
    _selected_press = select_press;
    setProperty("state",normal);
    repolish(this);
}

ClickLbState StateWidget::GetCurState()
{
    return _curstate;
}

void StateWidget::ClearState()
{
    _curstate = ClickLbState::Normal;
    this->setProperty("state",_normal);
    repolish(this);
    update();
}

void StateWidget::SetSelected(bool bselected)
{
    if(bselected){
        _curstate = ClickLbState::Selected;
        this->setProperty("state",_selected);
        repolish(this);
        update();
        return;
    }
    _curstate = ClickLbState::Normal;
    this->setProperty("state",_normal);
    repolish(this);
    update();
    return;
}

void StateWidget::AddRedPoint()
{
    //添加红点
    _red_point = new QLabel();
    _red_point->setObjectName("red_point");
    QVBoxLayout*layout2 = new QVBoxLayout;
    _red_point->setAlignment(Qt::AlignCenter);
    layout2->addWidget(_red_point);
    layout2->setContentsMargins(0,0,0,0);
    this->setLayout(layout2);
    _red_point->setVisible(false);
}

void StateWidget::ShowRedPoint(bool show)
{
    _red_point->setVisible(true);
}

void StateWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

void StateWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(_curstate==ClickLbState::Selected){
            QWidget::mousePressEvent(event);
            return;
        }
        if(_curstate==ClickLbState::Normal){
            _curstate = ClickLbState::Selected;
            this->setProperty("state",_selected);
            repolish(this);
            update();
            return;
        }
    }
    QWidget::mousePressEvent(event);
}

void StateWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(_curstate==ClickLbState::Normal){
            this->setProperty("state",_normal_hover);
            repolish(this);
            update();
            return;
        }
        if(_curstate==ClickLbState::Selected){
            this->setProperty("state",_selected_hover);
            repolish(this);
            update();
        }
        emit Clicked();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}

void StateWidget::enterEvent(QEnterEvent *event)
{
    //鼠标悬停
    if(_curstate==ClickLbState::Normal){
        this->setProperty("state",_normal_hover);
        repolish(this);
        update();
        return;
    }else{
        this->setProperty("state",_selected_hover);
        repolish(this);
        update();
        return;
    }
    QWidget::enterEvent(event);
}

void StateWidget::leaveEvent(QEvent *event)
{
    //处理鼠标离开事件
    if(_curstate==ClickLbState::Normal){
        this->setProperty("state",_normal);
        repolish(this);
        update();
        return;
    }
    else{
        this->setProperty("state",_selected);
        repolish(this);
        update();
        return;
    }
    QWidget::leaveEvent(event);

}

