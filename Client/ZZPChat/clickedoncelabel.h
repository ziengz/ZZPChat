
#ifndef CLICKEDONCELABEL_H
#define CLICKEDONCELABEL_H

#include <QLabel>
#include <QMouseEvent>


class ClickedOnceLabel : public QLabel
{
    Q_OBJECT
public:
    ClickedOnceLabel(QWidget* parent=nullptr);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void Clicked(QString);
};

#endif // CLICKEDONCELABEL_H
