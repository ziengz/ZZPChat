
#include "textbubble.h"
#include <QTextLayout>
#include <QTextDocument>
#include <QTextBlock>
#include <QFontMetricsF>

TextBubble::TextBubble(ChatRole role, const QString &text, QWidget *parent)
    :BubbleFrame(role,parent)
{
    m_pTextEdit = new QTextEdit();
    m_pTextEdit->setReadOnly(true);
    m_pTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTextEdit->installEventFilter(this);
    QFont font("Microsoft YaHei");
    font.setPointSize(12);
    m_pTextEdit->setFont(font);
    setPlainText(text);
    setWidget(m_pTextEdit);
    initStyleSheet();
}

bool TextBubble::eventFilter(QObject *watched, QEvent *event)
{
    //当拉动窗体时，气泡会因拉动而变化大小，触发Paint事件
    if(watched==m_pTextEdit&&event->type()==QEvent::Paint){
        adjustTextHeight();
    }
    return BubbleFrame::eventFilter(watched,event);
}

void TextBubble::adjustTextHeight()
{
    qreal doc_margin = m_pTextEdit->document()->documentMargin();
    QTextDocument* doc = m_pTextEdit->document();
    qreal text_height = 0;
    for(QTextBlock it = doc->begin();it!=doc->end();it = it.next()){
        QTextLayout* pLayout = it.layout();
        //获取这个段落的边界矩形
        QRectF text_rect = pLayout->boundingRect();
        text_height += text_rect.height();
    }
    //获取布局内容到外边框的距离
    int vMargin = this->layout()->contentsMargins().top();
    this->setFixedHeight(text_height + doc_margin * 2 + vMargin * 2);
}

//设置气泡长度
void TextBubble::setPlainText(const QString &text)
{
    m_pTextEdit->setPlainText(text);

    qreal doc_margin = m_pTextEdit->document()->documentMargin();
    //获取左右边距
    int margin_left = this->layout()->contentsMargins().left();
    int margin_right = this->layout()->contentsMargins().right();
    int max_width = 0;

    //计算给定字体的字符和字符串的大小
    QFontMetricsF fm(m_pTextEdit->font());

    QTextDocument*doc = m_pTextEdit->document();
    for(QTextBlock it = doc->begin();it!=doc->end();it = it.next()){
        int txtw = int(fm.horizontalAdvance(it.text()));
        max_width=max_width<txtw?txtw:max_width;
    }
    this->setMaximumWidth(max_width+doc_margin*2+(margin_left+margin_right));
}

void TextBubble::initStyleSheet()
{
    m_pTextEdit->setStyleSheet("QTextEdit{background:transparent;border:none}");
}




























