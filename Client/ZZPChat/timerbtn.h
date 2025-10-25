
#ifndef TIMERBTN_H
#define TIMERBTN_H
#include <QDebug>
#include <QTimer>
#include <QPushButton>




class TimerBtn:public QPushButton
{
public:
    TimerBtn(QWidget *parent = nullptr);
    ~TimerBtn();
    virtual void mouseReleaseEvent(QMouseEvent *e);

private:
    QTimer*time_;
    int count_;
};

#endif // TIMERBTN_H
