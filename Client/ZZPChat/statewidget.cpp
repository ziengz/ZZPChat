
#include "statewidget.h"

StateWidget::StateWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setCursor(Qt::PointingHandCursor);
    AddRedPoint();
}

void StateWidget::SetState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{

}

ClickLbState StateWidget::GetCurState()
{
    return _curstate;
}

void StateWidget::ClearState()
{
    _curstate = _normal;
    this->setProperty("normal",_normal);
    repolish(this);
    update();
}

void StateWidget::SetSelected(bool bselected)
{

}

void StateWidget::AddRedPoint()
{

}

void StateWidget::ShowRedPoint(bool show)
{

}

void StateWidget::mousePressEvent(QMouseEvent *event)
{

}

void StateWidget::mouseReleaseEvent(QMouseEvent *event)
{

}

void StateWidget::enterEvent(QEnterEvent *event)
{

}

void StateWidget::leaveEvent(QEvent *event)
{

}

