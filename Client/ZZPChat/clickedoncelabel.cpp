
#include "clickedoncelabel.h"

ClickedOnceLabel::ClickedOnceLabel(QWidget *parent)
{
    this->setCursor(Qt::PointingHandCursor);
}

void ClickedOnceLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        emit Clicked(this->text());
        return;
    }
    QLabel::mouseReleaseEvent(event);
}
