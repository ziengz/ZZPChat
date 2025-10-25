
#ifndef MESSAGETEXTEDIT_H
#define MESSAGETEXTEDIT_H

#include <QTextEdit>
#include "global.h"
#include <QWidget>
#include <QVector>


class MessageTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit MessageTextEdit(QWidget* parent = nullptr);
    ~MessageTextEdit();

    QVector<MsgInfo> getMsgList();

private:
    void insertMsgList(QVector<MsgInfo>&list,QString flag,QString text,QPixmap pix);

    QVector<MsgInfo> mMsgList;
    QVector<MsgInfo> mGetMsgList;
};

#endif // MESSAGETEXTEDIT_H
