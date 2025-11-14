#ifndef APPLYFRIENDPAGE_H
#define APPLYFRIENDPAGE_H

#include <QWidget>
#include "userdata.h"
#include "applyfrienditem.h"
#include "applyfriendlist.h"
#include "tcpmgr.h"
#include <QRandomGenerator>

namespace Ui {
class ApplyFriendPage;
}

class ApplyFriendPage : public QWidget
{
    Q_OBJECT

public:
    explicit ApplyFriendPage(QWidget *parent = nullptr);
    ~ApplyFriendPage();
    void addNewApply(std::shared_ptr<AddFriendApply>apply);
protected:
    void paintEvent(QPaintEvent *event);
private:
    void loadApplyList();
    Ui::ApplyFriendPage *ui;
    std::unordered_map<int,ApplyFriendItem*>_unauth_items;
public slots:
    void slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp);
signals:
    void sig_show_search(bool);
};

#endif // APPLYFRIENDPAGE_H
