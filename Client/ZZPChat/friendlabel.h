#ifndef FRIENDLABEL_H
#define FRIENDLABEL_H

#include <QFrame>

namespace Ui {
class FriendLabel;
}

class FriendLabel : public QFrame
{
    Q_OBJECT

public:
    explicit FriendLabel(QWidget *parent = nullptr);
    ~FriendLabel();

private:
    Ui::FriendLabel *ui;
};

#endif // FRIENDLABEL_H
