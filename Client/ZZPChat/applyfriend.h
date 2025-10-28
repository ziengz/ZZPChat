#ifndef APPLYFRIEND_H
#define APPLYFRIEND_H

#include <QDialog>
#include "userdata.h"
#include "clickedlabel.h"
#include "friendlabel.h"

namespace Ui {
class ApplyFriend;
}

class ApplyFriend : public QDialog
{
    Q_OBJECT

public:
    explicit ApplyFriend(QWidget *parent = nullptr);
    ~ApplyFriend();

    void InitTipLbs();
    void AddTipLbs(ClickedLabel*,QPoint cur_point,QPoint& next_point,int text_width,int text_height);
    bool eventFilter(QObject *watched, QEvent *event)override;

    void SetSearchInfo(std::shared_ptr<SearchInfo> si);

private:
    void resetLabels();
    Ui::ApplyFriend *ui;

    //已经创建的标签
    QMap<QString,ClickedLabel*> _add_labels;
    std::vector<QString>_add_label_keys;
    QPoint _label_point;
    //用来在输入框中显示添加新好友的标签
    QMap<QString,FriendLabel*>_friend_labels;
    std::vector<QString>_friend_label_key;
    void addLabel(QString name);
    //标签内容
    std::vector<QString> _tip_datas;
    QPoint _tip_cur_point;
    std::shared_ptr<SearchInfo>_si;

public slots:
    void ShowMoreLabel();
    void SlotApplySure();
    void SlotApplyCancel();


};

#endif // APPLYFRIEND_H
