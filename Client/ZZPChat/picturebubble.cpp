
#include "picturebubble.h"

#define PIC_MAX_WIDTH 160
#define PIC_MAX_LENGTH 90
PictureBubble::PictureBubble(const QPixmap &picture, ChatRole role, QWidget *parent):BubbleFrame(role,parent)
{
    QLabel* label = new QLabel();
    //设置可伸缩
    label->setScaledContents(true);                                 //保持原比例
    QPixmap pix = picture.scaled(QSize(PIC_MAX_WIDTH,PIC_MAX_LENGTH),Qt::KeepAspectRatio);
    label->setPixmap(pix);
    this->setWidget(label);

    int margin_left = this->layout()->contentsMargins().left();
    int margin_right = this->layout()->contentsMargins().right();
    int margin_v = this->layout()->contentsMargins().bottom();
    this->setFixedSize(pix.width() + margin_left + margin_right , pix.height() + margin_v * 2);
}
