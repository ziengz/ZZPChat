
#include "messagetextedit.h"

MessageTextEdit::MessageTextEdit(QWidget *parent):QTextEdit(parent)
{

}

MessageTextEdit::~MessageTextEdit()
{

}

QVector<MsgInfo> MessageTextEdit::getMsgList()
{
    mGetMsgList.clear();
    QString doc = this->document()->toPlainText();
    QString text = "";
    int indexUrl = 0;
    int count = mMsgList.size();
    for(int index = 0;index < doc.size();index++){
        //表示嵌入对象的特殊字符
        if(doc[index]==QChar::ObjectReplacementCharacter){
            if(!text.isEmpty()){
                QPixmap pix;
                insertMsgList(mGetMsgList,"text",text,pix);
                text.clear();
            }
            while(indexUrl < count){
                MsgInfo msg = mMsgList[indexUrl];
                //如果得到的数据为非文本对象（图片、表情等）
                if(this->document()->toHtml().contains(msg.content,Qt::CaseSensitive)){
                    indexUrl++;
                    mGetMsgList.append(msg);
                    break;
                }
                indexUrl++;
            }
        }
        else{
            text.append(doc[index]);
        }
    }
    if(!text.isEmpty()){
        QPixmap pix;
        insertMsgList(mGetMsgList,"text",text,pix);
        text.clear();
    }
    mMsgList.clear();
    this->clear();
    return mGetMsgList;
}

void MessageTextEdit::insertMsgList(QVector<MsgInfo> &list, QString flag, QString text, QPixmap pix)
{
    MsgInfo msg;
    msg.msgFlag = flag;
    msg.content = text;
    msg.pixmap = pix;
    list.append(msg);
}
