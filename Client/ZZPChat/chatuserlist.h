
#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H

#include <QListWidget>
#include <QEvent>
#include <QObject>
#include <QWheelEvent>
#include <QScrollBar>


class ChatUserList : public QListWidget
{
    Q_OBJECT
public:
    ChatUserList(QWidget*parent = nullptr);
    bool eventFilter(QObject *watch, QEvent *event) override;

signals:
    void sig_loading_chat_user();
};

#endif // CHATUSERLIST_H
