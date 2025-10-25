
#include "clickedbtn.h"
#include "global.h"

ClickedBtn::ClickedBtn(QWidget *parent):QPushButton(parent)
{
    //设置鼠标移动到上面变为手状
    setCursor(Qt::PointingHandCursor);
}

ClickedBtn::~ClickedBtn()
{

}

void ClickedBtn::SetState(QString normal, QString hover, QString press)
{
    normal_ = normal;
    hover_ = hover;
    press_ = press;
    this->setProperty("state",normal);
    repolish(this);
    update();
}

void ClickedBtn::enterEvent(QEnterEvent *event)
{
    this->setProperty("state",hover_);
    repolish(this);
    update();
    QPushButton::enterEvent(event);
}

void ClickedBtn::leaveEvent(QEvent *event)
{
    this->setProperty("state",normal_);
    repolish(this);
    update();
    QPushButton::leaveEvent(event);
}

void ClickedBtn::mousePressEvent(QMouseEvent *event)
{
    setProperty("state",press_);
    repolish(this);
    update();
    QPushButton::mousePressEvent(event);
}
void ClickedBtn::mouseReleaseEvent(QMouseEvent *event)
{
    setProperty("state",hover_);
    repolish(this);
    update();
    QPushButton::mouseReleaseEvent(event);
}
