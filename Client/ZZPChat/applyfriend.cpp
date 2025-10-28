#include "applyfriend.h"
#include "ui_applyfriend.h"
#include <QEvent>
#include <QScrollBar>
#include "global.h"
#include <QFontMetrics>

ApplyFriend::ApplyFriend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApplyFriend)
{
    ui->setupUi(this);

    this->setWindowFlag(windowFlags()|Qt::FramelessWindowHint);
    this->setObjectName("ApplyFriend");
    this->setModal(true);
    ui->name_ed->setPlaceholderText(tr("zzp"));
    ui->lb_ed->setPlaceholderText("搜索、添加标签");
    ui->back_ed->setPlaceholderText("燃烧的胸毛");

    ui->lb_ed->SetMaxLength(21);
    ui->lb_ed->move(2,2);
    ui->lb_ed->setFixedHeight(20);
    ui->lb_ed->setMaxLength(10);
    ui->input_tip_wid->hide();

    _tip_cur_point = QPoint(5,5);
    _tip_data = {"同学","家人","菜鸟教程","C++ Primer","Rust 程序设计",
                 "父与子学Python","nodejs开发指南","go 语言开发指南",
                    "游戏伙伴","金融投资","微信读书","拼多多拼友" };

    connect(ui->more_lb,&ClickedOnceLabel::Clicked,this,&ApplyFriend::ShowMoreLabel);
    //初始化tip位置以及
    InitTipLbs();

    ui->scrollArea->horizontalScrollBar()->setHidden(true);
    ui->scrollArea->verticalScrollBar()->setHidden(true);

    ui->scrollArea->installEventFilter(this);
    ui->sure_btn->SetState("normal","hover","press");
    ui->cancel_btn->SetState("normal","hover","press");
    connect(ui->sure_btn,&ClickedBtn::clicked,this,&ApplyFriend::SlotApplySure);
    connect(ui->cancel_btn,&ClickedBtn::clicked,this,&ApplyFriend::SlotApplyCancel);
}

ApplyFriend::~ApplyFriend()
{
    delete ui;
}

void ApplyFriend::InitTipLbs()
{
    qDebug()<<"receive more label clicked";
    ui->more_lb->hide();

    ui->lb_list->setFixedWidth(325);
    _tip_cur_point = QPoint(5,5);
    auto next_point = _tip_cur_point;
    int textWidth;
    int textHeight;

    //重排现有label
    for(auto& added_key:_add_label_keys){
        auto added_lb = _add_labels[added_key];

        QFontMetrics
    }
}

bool ApplyFriend::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->scrollArea&&event->type()==QEvent::Enter){
        ui->scrollArea->verticalScrollBar()->setHidden(falses);
    }
    else if(watched==ui->scrollArea&&event->type()==QEvent::Leave){
        ui->scrollArea->verticalScrollBar()->setHidden(true);
    }
    return QObject::eventFilter(watched,event);
}

void ApplyFriend::addLabel(QString name)
{

}

void ApplyFriend::ShowMoreLabel()
{

}

void ApplyFriend::SlotApplySure()
{

}

void ApplyFriend::SlotApplyCancel()
{

}
