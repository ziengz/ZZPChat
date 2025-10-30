#include "applyfriend.h"
#include "ui_applyfriend.h"
#include <QEvent>
#include <QScrollBar>
#include "global.h"
#include <QFontMetrics>
#include "usermgr.h"

ApplyFriend::ApplyFriend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApplyFriend)
{
    ui->setupUi(this);

    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
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
    _tip_datas = {"同学","家人","菜鸟教程","C++ Primer","Rust 程序设计",
                 "父与子学Python","nodejs开发指南","go 语言开发指南",
                    "游戏伙伴","金融投资","微信读书","拼多多拼友" };

    connect(ui->more_lb,&ClickedOnceLabel::Clicked,this,&ApplyFriend::ShowMoreLabel);
    //初始化tip位置以及
    InitTipLbs();
    //连接输入标签回车事件
    connect(ui->lb_ed,&CustomizeEdit::returnPressed,this,&ApplyFriend::slotLabelEnter);
    connect(ui->lb_ed,&CustomizeEdit::textChanged,this,&ApplyFriend::slotLabelTextChange);
    connect(ui->lb_ed,&CustomizeEdit::editingFinished,this,&ApplyFriend::SlotLabelEditFinished);
    connect(ui->tip_lb,&ClickedOnceLabel::Clicked,this,&ApplyFriend::SlotAddFirendLabelByClickTip);

    ui->scrollArea->horizontalScrollBar()->setHidden(true);
    ui->scrollArea->verticalScrollBar()->setHidden(true);

    ui->scrollArea->installEventFilter(this);
    ui->sure_btn->SetState("normal","hover","press");
    ui->cancel_btn->SetState("normal","hover","press");
    connect(ui->sure_btn,&QPushButton::clicked,this,&ApplyFriend::SlotApplySure);
    connect(ui->cancel_btn,&QPushButton::clicked,this,&ApplyFriend::SlotApplyCancel);


}

ApplyFriend::~ApplyFriend()
{
    delete ui;
}

void ApplyFriend::InitTipLbs()
{
    int line = 1;  //只显示两行标签
    for(int i = 0;i < _tip_datas.size();i++){
        auto* lb = new ClickedLabel(ui->lb_list);
        lb->SetState("normal","hover","pressed","selected_normal",
                     "selected_hover", "selected_pressed");
        lb->setObjectName("tipslb");
        lb->setText(_tip_datas[i]);

        connect(lb,&ClickedLabel::Click,this,&ApplyFriend::SlotChangeFriendLabelByTip);

        QFontMetrics fontMetrics(lb->font());
        int textWidth = fontMetrics.horizontalAdvance(lb->text());
        int textHeight = fontMetrics.height();
        if(_tip_cur_point.x() + textWidth + tip_offset > ui->lb_list->width()){
            line++;
            if(line > 2){
                delete lb;
                return;
            }
            _tip_cur_point.setX(tip_offset);
            _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);
        }
        auto next_point = _tip_cur_point;
        AddTipLbs(lb,_tip_cur_point,next_point,textWidth,textHeight);
        _tip_cur_point = next_point;
    }


}

void ApplyFriend::AddTipLbs(ClickedLabel *lb, QPoint cur_point, QPoint &next_point, int text_width, int text_height)
{
    lb->move(cur_point);
    lb->show();
    _add_labels.insert(lb->text(),lb);
    _add_label_keys.push_back(lb->text());
    next_point.setX(lb->pos().x() + text_width + 15);
    next_point.setY(lb->pos().y());
}

bool ApplyFriend::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->scrollArea&&event->type()==QEvent::Enter){
        ui->scrollArea->verticalScrollBar()->setHidden(false);
    }
    else if(watched==ui->scrollArea&&event->type()==QEvent::Leave){
        ui->scrollArea->verticalScrollBar()->setHidden(true);
    }
    return QObject::eventFilter(watched,event);
}

void ApplyFriend::SetSearchInfo(std::shared_ptr<SearchInfo> si)
{
    _si = si;
    auto applyname = UserMgr::getInstance()->GetName();
    auto backname = si->_name;
    ui->name_ed->setText(applyname);
    ui->back_ed->setText(backname);
}

void ApplyFriend::addLabel(QString name)
{
    if(_friend_labels.find(name)!=_friend_labels.end()){
        ui->lb_ed->clear();
        return;
    }
    auto tmplabel = new FriendLabel(ui->gridWidget);
    tmplabel->setObjectName("FriendLabel");
    tmplabel->SetText(name);

    auto max_width = ui->gridWidget->width();
    if(_label_point.x() + tmplabel->Width() > max_width){
        _label_point.setX(2);
        _label_point.setY(_label_point.y() + tmplabel->Height() + 6);
    }
    tmplabel->move(_label_point);
    tmplabel->show();
    _friend_labels[tmplabel->Text()] = tmplabel;
    _friend_label_key.push_back(tmplabel->Text());
    connect(tmplabel,&FriendLabel::sig_close,this,&ApplyFriend::SlotRemoveFriendLabel);
    _label_point.setX(_label_point.x()+tmplabel->width()+2);

    if(_label_point.x()+MIN_APPLY_LABEL_ED_LEN > ui->gridWidget->width()){
        ui->lb_ed->move(2,_label_point.y()+tmplabel->height()+2);
    }
    else{
        ui->lb_ed->move(_label_point);
    }
    ui->lb_ed->clear();

    if(ui->gridWidget->height()<_label_point.y()+tmplabel->height()+2){
        ui->gridWidget->setFixedHeight(_label_point.y() + tmplabel->height() * 2 + 2);
    }
}

void ApplyFriend::slotLabelEnter()
{
    if(ui->lb_ed->text().isEmpty()){
        return;
    }
    auto text = ui->lb_ed->text();
    addLabel(text);
    auto find_it = std::find(_tip_datas.begin(),_tip_datas.end(),text);
    if(find_it == _tip_datas.end()){
        _tip_datas.push_back(text);
    }

    //判断标签列表中是否有标签
    auto find_add = _add_labels.find(text);
    if(find_add != _add_labels.end()){
        find_add.value()->SetCurState(ClickLbState::Selected);
        return;
    }
    //如果不在标签列表中，则添加一个新的并将其设置为绿色选中
    auto* lb = new ClickedLabel(ui->lb_list);
    lb->setObjectName("tipslb");
    lb->SetState("normal","hover","pressed","selected_normal",
                 "selected_hover","selected_pressed");
    lb->setText(text);
    connect(lb,&ClickedLabel::Click,this,&ApplyFriend::SlotChangeFriendLabelByTip);

    QFontMetrics fontMetrics(lb->font());
    int textWidth = fontMetrics.horizontalAdvance(lb->text());
    int textHeight = fontMetrics.height();
    if(_tip_cur_point.x() + textWidth + tip_offset + 3 > ui->lb_list->width()){
        _tip_cur_point.setX(tip_offset);
        _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);
    }
    auto next_point = _tip_cur_point;
    AddTipLbs(lb,_tip_cur_point,next_point,textWidth,textHeight);
    _tip_cur_point = next_point;

    //如果标签太多导致标签列表显示不下，则设置
    int diff_height = next_point.y() + textHeight + tip_offset - ui->lb_list->height();
    ui->lb_list->setFixedHeight(next_point.y()+textHeight+tip_offset);
    lb->SetCurState(ClickLbState::Selected);

    ui->scrollcontent->setFixedHeight(ui->scrollcontent->height() + diff_height);

}

void ApplyFriend::ShowMoreLabel()
{
    qDebug()<< "receive more label clicked";
    ui->more_lb_wid->hide();

    ui->lb_list->setFixedWidth(325);
    _tip_cur_point = QPoint(5, 5);
    auto next_point = _tip_cur_point;
    int textWidth;
    int textHeight;
    //重拍现有的label
    for(auto & added_key : _add_label_keys){
        auto added_lb = _add_labels[added_key];

        QFontMetrics fontMetrics(added_lb->font()); // 获取QLabel控件的字体信息
        textWidth = fontMetrics.horizontalAdvance(added_lb->text()); // 获取文本的宽度
        textHeight = fontMetrics.height(); // 获取文本的高度

        if(_tip_cur_point.x() +textWidth + tip_offset > ui->lb_list->width()){
            _tip_cur_point.setX(tip_offset);
            _tip_cur_point.setY(_tip_cur_point.y()+textHeight+15);
        }
        added_lb->move(_tip_cur_point);

        next_point.setX(added_lb->pos().x() + textWidth + 15);
        next_point.setY(_tip_cur_point.y());

        _tip_cur_point = next_point;

    }

    //添加未添加的
    for(int i = 0; i < _tip_datas.size(); i++){
        auto iter = _add_labels.find(_tip_datas[i]);
        if(iter != _add_labels.end()){
            continue;
        }

        auto* lb = new ClickedLabel(ui->lb_list);
        lb->SetState("normal", "hover", "pressed", "selected_normal",
            "selected_hover", "selected_pressed");
        lb->setObjectName("tipslb");
        lb->setText(_tip_datas[i]);
        connect(lb, &ClickedLabel::Click, this, &ApplyFriend::SlotChangeFriendLabelByTip);

        QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
        int textWidth = fontMetrics.horizontalAdvance(lb->text()); // 获取文本的宽度
        int textHeight = fontMetrics.height(); // 获取文本的高度

        if (_tip_cur_point.x() + textWidth + tip_offset > ui->lb_list->width()) {

            _tip_cur_point.setX(tip_offset);
            _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

        }

        next_point = _tip_cur_point;

        AddTipLbs(lb, _tip_cur_point, next_point, textWidth, textHeight);

        _tip_cur_point = next_point;

    }

    int diff_height = next_point.y() + textHeight + tip_offset - ui->lb_list->height();
    ui->lb_list->setFixedHeight(next_point.y() + textHeight + tip_offset);

    //qDebug()<<"after resize ui->lb_list size is " <<  ui->lb_list->size();
    ui->scrollcontent->setFixedHeight(ui->scrollcontent->height()+diff_height);
}

void ApplyFriend::resetLabels()
{
    _label_point = QPoint(2,6);
    auto max_width = ui->gridWidget->width();
    auto label_height = 0;
    for(auto iter = _friend_labels.begin(); iter != _friend_labels.end(); iter++){
        //todo... 添加宽度统计
        if( _label_point.x() + iter.value()->width() > max_width) {
            _label_point.setY(_label_point.y()+iter.value()->height()+6);
            _label_point.setX(2);
        }

        iter.value()->move(_label_point);
        iter.value()->show();

        _label_point.setX(_label_point.x()+iter.value()->width()+2);
        _label_point.setY(_label_point.y());
        label_height = iter.value()->height();
    }

    if(_friend_labels.isEmpty()){
         ui->lb_ed->move(_label_point);
         return;
    }

    if(_label_point.x() + MIN_APPLY_LABEL_ED_LEN > ui->gridWidget->width()){
        ui->lb_ed->move(2,_label_point.y()+label_height+6);
    }else{
         ui->lb_ed->move(_label_point);
    }

    // 更新容器高度
    int new_height = ui->lb_ed->pos().y() + ui->lb_ed->height() + 6;
    if(new_height < ui->gridWidget->height()) {
        ui->gridWidget->setFixedHeight(new_height);
    }
}

void ApplyFriend::SlotApplySure()
{

}

void ApplyFriend::SlotApplyCancel()
{
    qDebug() << "Slot Apply Cancel";
    this->hide();
    deleteLater();
}
//点击已有标签，添加或删除新联系人的标签
void ApplyFriend::SlotChangeFriendLabelByTip(QString text, ClickLbState state)
{
    auto find_iter = _add_labels.find(text);
    if(find_iter==_add_labels.end()){
        return;
    }
    if(state == ClickLbState::Selected){
        //添加逻辑
        addLabel(text);
        return;
    }
    if(state == ClickLbState::Normal){
        //删除逻辑
        SlotRemoveFriendLabel(text);
        return;
    }
}

void ApplyFriend::SlotAddFirendLabelByClickTip(QString text)
{
    int index = text.indexOf(add_prefix);
    if (index != -1) {
        text = text.mid(index + add_prefix.length());
    }
    addLabel(text);

    auto find_it = std::find(_tip_datas.begin(), _tip_datas.end(), text);
    //找到了就只需设置状态为选中即可
    if (find_it == _tip_datas.end()) {
        _tip_datas.push_back(text);
    }

    //判断标签展示栏是否有该标签
    auto find_add = _add_labels.find(text);
    if (find_add != _add_labels.end()) {
        find_add.value()->SetCurState(ClickLbState::Selected);
        return;
    }

    //标签展示栏也增加一个标签, 并设置绿色选中
    auto* lb = new ClickedLabel(ui->lb_list);
    lb->SetState("normal", "hover", "pressed", "selected_normal",
        "selected_hover", "selected_pressed");
    lb->setObjectName("tipslb");
    lb->setText(text);
    connect(lb, &ClickedLabel::Click, this, &ApplyFriend::SlotChangeFriendLabelByTip);
    qDebug() << "ui->lb_list->width() is " << ui->lb_list->width();
    qDebug() << "_tip_cur_point.x() is " << _tip_cur_point.x();

    QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
    int textWidth = fontMetrics.horizontalAdvance(lb->text()); // 获取文本的宽度
    int textHeight = fontMetrics.height(); // 获取文本的高度
    qDebug() << "textWidth is " << textWidth;

    if (_tip_cur_point.x() + textWidth+ tip_offset+3 > ui->lb_list->width()) {

        _tip_cur_point.setX(5);
        _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

    }

    auto next_point = _tip_cur_point;

     AddTipLbs(lb, _tip_cur_point, next_point, textWidth,textHeight);
    _tip_cur_point = next_point;

    int diff_height = next_point.y() + textHeight + tip_offset - ui->lb_list->height();
    ui->lb_list->setFixedHeight(next_point.y() + textHeight + tip_offset);

    lb->SetCurState(ClickLbState::Selected);

    ui->scrollcontent->setFixedHeight(ui->scrollcontent->height()+ diff_height );
}

void ApplyFriend::SlotRemoveFriendLabel(QString name)
{
    qDebug() << "receive close signal";

//    _label_point.setX(2);
//    _label_point.setY(6);

   auto find_iter = _friend_labels.find(name);

   if(find_iter == _friend_labels.end()){
       return;
   }

   auto find_key = _friend_label_key.end();
   for(auto iter = _friend_label_key.begin(); iter != _friend_label_key.end();iter++){
       if(*iter == name){
           find_key = iter;
           break;
       }
   }

   if(find_key != _friend_label_key.end()){
      _friend_label_key.erase(find_key);
   }


   delete find_iter.value();

   _friend_labels.erase(find_iter);

   resetLabels();

   auto find_add = _add_labels.find(name);
   if(find_add == _add_labels.end()){
        return;
   }

   find_add.value()->ResetNormalState();

}

void ApplyFriend::slotLabelTextChange(const QString& text)
{
    if(ui->lb_ed->text().isEmpty()){
        ui->tip_lb->setText("");
        ui->input_tip_wid->hide();
        return;
    }

    auto iter = std::find(_tip_datas.begin(),_tip_datas.end(),text);
    if(iter==_tip_datas.end())
    {
        auto new_text = add_prefix+text;
        ui->tip_lb->setText(new_text);
        ui->input_tip_wid->show();
        return;
    }
    ui->tip_lb->setText(text);
    ui->input_tip_wid->show();
}

void ApplyFriend::SlotLabelEditFinished()
{
    ui->input_tip_wid->hide();
}
