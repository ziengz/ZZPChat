
#include "chatview.h"
#include <QScrollBar>
#include <QHBoxLayout>
#include <QEvent>
#include <QTimer>
#include <QStyleOption>
#include <QPainter>

ChatView::ChatView(QWidget *parent)  : QWidget(parent), isAppended(false)
{
    // 1. 主布局（垂直排列）
    QVBoxLayout *pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout);
    pMainLayout->setContentsMargins(0,0,0,0);  // 去除边缘间隙

    // 2. 创建滚动区域（核心组件，用于承载大量消息）
    m_pScrollArea = new QScrollArea();
    m_pScrollArea->setObjectName("chat_area");  // 用于样式表
    pMainLayout->addWidget(m_pScrollArea);      // 把滚动区域加入主布局

    // 3. 滚动区域内部的内容容器（所有消息都放在这个widget里）
    QWidget *w = new QWidget(this);
    w->setObjectName("chat_bg");  // 用于背景样式
    w->setAutoFillBackground(true);  // 允许设置背景

    // 4. 内容容器的布局（垂直排列消息）
    QVBoxLayout *pVLayout_1 = new QVBoxLayout();
    // 在布局末尾添加一个“弹性占位组件”（高度权重100000），
    // 作用：让所有消息向上顶，最新消息始终靠下（类似微信聊天界面）
    pVLayout_1->addWidget(new QWidget(), 100000);
    w->setLayout(pVLayout_1);

    // 5. 关联滚动区域和内容容器
    m_pScrollArea->setWidget(w);  // 内容容器放入滚动区域
    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 隐藏默认滚动条

    // 6. 处理滚动条逻辑
    QScrollBar *pVScrollBar = m_pScrollArea->verticalScrollBar();
    // 滚动条范围变化时（如添加消息后内容变长），触发自定义处理（通常是滚动到底部）
    connect(pVScrollBar, &QScrollBar::rangeChanged, this, &ChatView::onVScrollBarMoved);

    // 7. 调整滚动条位置（默认在右侧，这里用布局强制右对齐，不改变位置但方便隐藏）
    QHBoxLayout *pHLayout_2 = new QHBoxLayout();
    pHLayout_2->addWidget(pVScrollBar, 0, Qt::AlignRight);
    pHLayout_2->setContentsMargins(0,0,0,0);
    m_pScrollArea->setLayout(pHLayout_2);
    pVScrollBar->setHidden(true);  // 初始隐藏滚动条

    // 8. 其他设置
    m_pScrollArea->setWidgetResizable(true);  // 内容容器自动适应滚动区域大小
    m_pScrollArea->installEventFilter(this);  // 安装事件过滤器（用于控制滚动条显示/隐藏）
    initStyleSheet();  // 初始化样式表（美化界面）
}

void ChatView::appendChatItem(QWidget *item)
{
    QVBoxLayout*v1 = qobject_cast<QVBoxLayout*>(m_pScrollArea->widget()->layout());
    //因为后续是伸缩项往上顶，也算一个item，所以要-1
    v1->insertWidget(v1->count()-1,item);
    isAppended = true;
}

bool ChatView::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == m_pScrollArea&&event->type()==QEvent::Enter)
    {
        //判断一下滚动条的最大位置是不是0，说明内容没有超过滚动区域，当超过了才显示滚动条
        m_pScrollArea->verticalScrollBar()->setHidden(m_pScrollArea->verticalScrollBar()->maximum()==0);
    }else if(watched==m_pScrollArea&&event->type()==QEvent::Leave)
    {
        m_pScrollArea->verticalScrollBar()->setHidden(true);
    }
    return QWidget::eventFilter(watched,event);
}

void ChatView::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    this->style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

void ChatView::onVScrollBarMoved(int min, int max)
{
    if(isAppended){
        QScrollBar*pVScrollBar = m_pScrollArea->verticalScrollBar();
        pVScrollBar->setSliderPosition(pVScrollBar->maximum());

        QTimer::singleShot(500,[this](){
            isAppended = false;
        });
    }
}

void ChatView::initStyleSheet()
{

}

