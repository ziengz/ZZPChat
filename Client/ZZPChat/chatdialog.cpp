#include "chatdialog.h"
#include "ui_chatdialog.h"
#include <QAction>
#include "chatuserwid.h"
#include "loadingdlg.h"

ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog),_b_loading(false),_mode(ChatUIMode::ChatMode),_state(ChatUIMode::ChatMode)
{
    ui->setupUi(this);
    ui->add_btn->SetState("normal","hover","press");
    ui->search_edit->SetMaxLength(15);

    QAction* searchAction = new QAction(ui->search_edit);
    searchAction->setIcon(QIcon(":/res/search.png"));
    ui->search_edit->addAction(searchAction,QLineEdit::LeadingPosition);
    ui->search_edit->setPlaceholderText(QStringLiteral("搜索"));

    QPixmap pix(":/res/head_1.jpg");
    ui->side_head_lb->setPixmap(pix);                         //原比例缩放
    QPixmap scaledPixmap = pix.scaled(ui->side_head_lb->size(),Qt::KeepAspectRatio);
    ui->side_head_lb->setPixmap(scaledPixmap);
    ui->side_head_lb->setScaledContents(true);  //设置QLabel自动缩放图片内容以适应大小

    ui->side_chat_lb->setProperty("state","normal");

    ui->side_chat_lb->SetState("normal","hover","pressed","selected_normal","selected_hover","selected_pressed");
    ui->side_contact_lb->SetState("normal","hover","pressed","selected_normal","selected_hover","selected_pressed");

    AddLBGroup(ui->side_chat_lb);
    AddLBGroup(ui->side_contact_lb);
    //处理点击其中一个图标时，其他图标会变为正常
    connect(ui->side_chat_lb,&StateWidget::Clicked,this,&ChatDialog::slot_side_chat);
    connect(ui->side_contact_lb,&StateWidget::Clicked,this,&ChatDialog::slot_side_contect);

    //创建一个清除动作并设置图标
    QAction* clearAction = new QAction(ui->search_edit);
    clearAction->setIcon(QIcon(":/res/close_transparent.png"));
    //初始化时不显示图标
    //将清除图标添加到搜索框末尾
    ui->search_edit->addAction(clearAction,QLineEdit::TrailingPosition);

    //当填入数据，则将图标换为可见图标
    connect(ui->search_edit,&QLineEdit::textChanged,[clearAction](const QString&text){
        if(!text.isEmpty()){
            clearAction->setIcon(QIcon(":/res/close_search.png"));
        }
        else{
            clearAction->setIcon(QIcon(":/res/close_transparent.png"));
        }
    });
    connect(clearAction,&QAction::triggered,[this,clearAction](){
        ui->search_edit->clear();
        clearAction->setIcon(QIcon(":/res/close_transparent.png"));
        ui->search_edit->clearFocus();
        ShowSearch(false);
    });
    ShowSearch(false);
    connect(ui->chat_user_list,&ChatUserList::sig_loading_chat_user,this,&ChatDialog::slot_loading_chat_user);

    addChatUserList();

    connect(ui->search_edit,&QLineEdit::textChanged,this,&ChatDialog::slot_text_changed);
    this->installEventFilter(this);

    ui->side_chat_lb->SetSelected(true);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::ShowSearch(bool b_search)
{
    if(b_search){
        ui->chat_user_list->hide();
        ui->con_user_list->hide();
        ui->search_list->show();
        _mode = ChatUIMode::SearchMode;
    }
    else if(_state==ChatUIMode::ChatMode){
        ui->chat_user_list->show();
        ui->con_user_list->hide();
        ui->search_list->hide();
        _mode = ChatUIMode::ChatMode;
    }
    else if(_state ==ChatUIMode::ContactMode){
        ui->chat_user_list->hide();
        ui->con_user_list->show();
        ui->search_list->hide();
        _mode = ChatUIMode::ContactMode;
    }
}

std::vector<QString>  strs ={"hello world !",
                             "nice to meet u",
                             "New year，new life",
                            "You have to love yourself",
                            "My love is written in the wind ever since the whole world is you"};
std::vector<QString> heads = {
    ":/res/head_1.jpg",
    ":/res/head_2.jpg",
    ":/res/head_3.jpg",
    ":/res/head_4.jpg",
    ":/res/head_5.jpg"
};
std::vector<QString> names = {
    "llfc",
    "zack",
    "golang",
    "cpp",
    "java",
    "nodejs",
    "python",
    "rust"
};

void ChatDialog::addChatUserList()
{
    for(int i=0;i<13;i++){
        //生成一个0-99的随机数
        int randomValue = QRandomGenerator::global()->bounded(100);
        int head_i = randomValue % heads.size();
        int str_i = randomValue % strs.size();
        int name_i = randomValue % names.size();

        auto* chat_user_wid = new ChatUserWid;

        chat_user_wid->SetInfo(names[name_i],heads[head_i],strs[str_i]);
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(chat_user_wid->sizeHint());
        ui->chat_user_list->addItem(item);
        ui->chat_user_list->setItemWidget(item,chat_user_wid);
    }
}

void ChatDialog::AddLBGroup(StateWidget *lb)
{
    _lb_list.push_back(lb);
}

void ChatDialog::slot_loading_chat_user()
{
    if(_b_loading){
        return;
    }
    _b_loading = true;
    LoadingDlg* loadingDlg = new LoadingDlg(this);
    loadingDlg->setModal(true);
    loadingDlg->show();
    qDebug()<<"add new data to list...";
    addChatUserList();
    //显示完删除对话框
    loadingDlg->deleteLater();
    _b_loading = false;
}

void ChatDialog::ClearLabelState(StateWidget *lb)
{
    for(auto& elem:_lb_list){
        if(elem==lb){
            continue;
        }
        elem->ClearState();
    }
}

bool ChatDialog::eventFilter(QObject *watched, QEvent *event)
{
    //监听鼠标点击事件
    if(event->type()==QEvent::MouseButtonPress){
        QMouseEvent*mouseEvent = static_cast<QMouseEvent*>(event);
        handleGlobalMousePress(mouseEvent);
        return true;
    }
    return QDialog::eventFilter(watched,event);
}

void ChatDialog::handleGlobalMousePress(QMouseEvent *event)
{
    //计算鼠标点击位置，如果不在searchlist范围内，清空搜索框内容切换回chat_user_list
    if(_mode!=ChatUIMode::SearchMode)
        return;
    QPoint posInSearchList = ui->search_list->mapFromGlobal(event->globalPos());
    //如果点击位置不包含在search_list中
    if(!ui->search_list->rect().contains(posInSearchList)){
        ui->search_edit->clear();
        ShowSearch(false);
    }
}

void ChatDialog::slot_side_chat()
{
    qDebug()<<"receive side_chat clicked";
    ui->search_edit->clear();
    ClearLabelState(ui->side_chat_lb);
    ui->stackedWidget->setCurrentWidget(ui->chat_page);
    _state = ChatUIMode::ChatMode;
    ShowSearch(false);
}

void ChatDialog::slot_side_contect()
{
    qDebug()<<"receive side_contect clicked";
    ui->search_edit->clear();
    ClearLabelState(ui->side_contact_lb);
    ui->stackedWidget->setCurrentWidget(ui->friend_apply_page);
    _state = ChatUIMode::ContactMode;
    ShowSearch(false);
}

void ChatDialog::slot_text_changed(const QString& text)
{
    if(!text.isEmpty()){
        ShowSearch(true);
    }
}

