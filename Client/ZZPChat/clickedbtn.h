
#ifndef CLICKEDBTN_H
#define CLICKEDBTN_H
#include <QWidget>
#include <QPushButton>



class ClickedBtn : public QPushButton
{
    Q_OBJECT
public:
    ClickedBtn(QWidget*parent=nullptr);
    ~ClickedBtn();
    void SetState(QString normal,QString hover,QString press);

protected:
    virtual void enterEvent(QEnterEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
private:
    QString normal_;
    QString hover_;
    QString press_;
};

#endif // CLICKEDBTN_H
