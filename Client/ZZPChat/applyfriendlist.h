
#ifndef APPLYFRIENDLIST_H
#define APPLYFRIENDLIST_H

#include <QListWidget>



class ApplyFriendList : public QListWidget
{
    Q_OBJECT
public:
    ApplyFriendList(QWidget*parent = nullptr);
    bool eventFilter(QObject *object, QEvent *event) override;

signals:
    void sig_show_search(bool);
};

#endif // APPLYFRIENDLIST_H
