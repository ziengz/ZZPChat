
#ifndef LISTITEMBASE_H
#define LISTITEMBASE_H

#include "global.h"
#include <QWidget>


class ListItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ListItemBase(QWidget *parent = nullptr);
    void SetItemType(ListItemType itemType);

    ListItemType GetItemType();
protected:
    //virtual void paintEvent(QPaintEvent *event) override;
private:
    ListItemType _itemType;
signals:

};

#endif // LISTITEMBASE_H
