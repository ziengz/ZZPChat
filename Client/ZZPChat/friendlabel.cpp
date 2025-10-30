#include "friendlabel.h"
#include "ui_friendlabel.h"

FriendLabel::FriendLabel(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FriendLabel)
{
    ui->setupUi(this);
    ui->close_lb->SetState("normal","hover","pressed","selected_normal",
                           "selected_hover", "selected_pressed");
    connect(ui->close_lb,&ClickedLabel::Click,this,&FriendLabel::slot_close);
}

void FriendLabel::SetText(QString text)
{
    _text = text;
    ui->tip_lb->setText(text);
    ui->tip_lb->adjustSize();

    QFontMetrics fontMetrics(ui->tip_lb->text());
    int textWidth = fontMetrics.horizontalAdvance(ui->tip_lb->text());
    int textHeight = fontMetrics.height();
    this->setFixedWidth(ui->tip_lb->width()+ui->close_lb->width()+5);
    this->setFixedHeight(textHeight+2);
    qDebug()<<"this->setFixHeight "<<this->height();
    _width = this->width();
    _height = this->height();
}

FriendLabel::~FriendLabel()
{
    delete ui;
}

int FriendLabel::Width()
{
    return _width;
}

int FriendLabel::Height()
{
    return _height;
}

QString FriendLabel::Text()
{
    return _text;
}

void FriendLabel::slot_close()
{
    emit sig_close(_text);
}
