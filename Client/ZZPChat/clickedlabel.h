
#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QMouseEvent>
#include <QLabel>
#include "global.h"
#include <QEnterEvent>



class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    ClickedLabel(QWidget *parent=nullptr);

    virtual void mousePressEvent(QMouseEvent* e) override;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
    virtual void enterEvent(QEnterEvent* event);
    virtual void leaveEvent(QEvent* event);
    void SetState(QString normal,QString hover,QString press,
                  QString select,QString select_hover,QString select_press);

    ClickLbState GetCurState();

private:
    QString normal_;
    QString normal_hover_;
    QString normal_press_;

    QString select_;
    QString select_hover_;
    QString select_press_;

    ClickLbState curstate_;
signals:
    void Click(void);

};

#endif // CLICKEDLABEL_H
