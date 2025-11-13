#ifndef CHATUSERWID_H
#define CHATUSERWID_H

#include <QWidget>
#include "listitembase.h"
#include "userdata.h"
#include <QRandomGenerator>

namespace Ui {
class ChatUserWid;
}

class ChatUserWid : public ListItemBase
{
    Q_OBJECT

public:
    explicit ChatUserWid(QWidget *parent = nullptr);
    ~ChatUserWid();

    QSize sizeHint() const override{
        return QSize(250,70);
    }

    void SetInfo(std::shared_ptr<UserInfo>user_info);
    void SetInfo(QString name, QString head, QString msg);
    void ShowRedPoint(bool bshow);

private:

    Ui::ChatUserWid *ui;
    QString _name;
    QString _head;
    QString _msg;
    std::shared_ptr<UserInfo>_user_info;
};

#endif // CHATUSERWID_H
