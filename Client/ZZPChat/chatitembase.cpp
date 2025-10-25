
#include "chatitembase.h"

ChatItemBase::ChatItemBase(ChatRole role, QWidget *parent):QWidget(parent)
{
    m_pNameLabel = new QLabel();
    m_pNameLabel->setObjectName("chat_user_name");
    QFont font("Microsoft YaHei");
    font.setPointSize(9);
    m_pNameLabel->setFont(font);
    m_pNameLabel->setFixedHeight(20);

    m_pIconLabel = new QLabel();
    //设置图片可缩放
    m_pIconLabel->setScaledContents(true);
    m_pIconLabel->setFixedSize(42,42);
    m_pBubble = new QWidget();

    QGridLayout* pGLayout = new QGridLayout();
    pGLayout->setVerticalSpacing(3);
    pGLayout->setHorizontalSpacing(3);
    pGLayout->setContentsMargins(3, 3, 3, 3);

    QSpacerItem*pSpace = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
    if(role==ChatRole::Self){
        //名字与头像之间间距8px
        m_pNameLabel->setContentsMargins(0,0,8,0);
        m_pNameLabel->setAlignment(Qt::AlignRight);
        pGLayout->addWidget(m_pNameLabel,0,1,1,1);
        pGLayout->addWidget(m_pIconLabel,0,2,2,1,Qt::AlignTop);
        pGLayout->addItem(pSpace,1,0,1,1);
        pGLayout->addWidget(m_pBubble,1,1,1,1);
        //设置可伸缩宫格为第一列和第二列，占比为2：3
        pGLayout->setColumnStretch(0,2);
        pGLayout->setColumnStretch(1,3);
    }else
    {
        m_pNameLabel->setContentsMargins(8,0,0,0);
        m_pNameLabel->setAlignment(Qt::AlignLeft);
        pGLayout->addWidget(m_pNameLabel,0,1,1,1);
        pGLayout->addWidget(m_pIconLabel,0,0,2,1,Qt::AlignTop);
        pGLayout->addWidget(m_pBubble,1,1,1,1);
        pGLayout->addItem(pSpace,2,1,1,1);
        pGLayout->setColumnStretch(1,3);
        pGLayout->setColumnStretch(2,2);
    }
    this->setLayout(pGLayout);
}

void ChatItemBase::setUserName(const QString &name)
{
    m_pNameLabel->setText(name);
}

void ChatItemBase::setUserIcon(const QPixmap &icon)
{
    m_pIconLabel->setPixmap(icon);
}

//需要定制化气泡，所以要写个函数更新
void ChatItemBase::setWidget(QWidget *w)
{
    QGridLayout*pGLayout = qobject_cast<QGridLayout*>(this->layout());
    pGLayout->replaceWidget(m_pBubble,w);
    delete m_pBubble;
    m_pBubble = w;
}









