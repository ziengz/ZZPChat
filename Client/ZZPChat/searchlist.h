
#ifndef SEARCHLIST_H
#define SEARCHLIST_H

#include <QObject>
#include <QListWidget>
#include <QEvent>
#include <QWheelEvent>
#include <QListWidgetItem>
#include <QScrollBar>
#include "tcpmgr.h"

class SearchList:public QListWidget
{
    Q_OBJECT
public:
    SearchList(QWidget* parent = nullptr);
    void CloseFindDlg();
    void setSeatchEdit(QWidget * edit);
protected:
    bool eventFilter(QObject *object, QEvent *event)override{
        if(object==this->viewport()){
            if(event->type()==QEvent::Enter){
                this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            }else if(event->type()==QEvent::Leave){
                this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            }
        }
        //处理滚轮滚动事件
        if(object==this->viewport()&&event->type()==QEvent::Wheel){
            QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
            int numDegrees = wheelEvent->angleDelta().y()/8;
            int numSteps = numDegrees/15;  //计算滚动步数

            this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()-numSteps);
            return true;
        }
        return QListWidget::eventFilter(object,event);
    }

private:
    void waitPending(bool pending = true);
    bool _send_pending;
    void addTipItem();
    QWidget* _search_edit;
    std::shared_ptr<QDialog> _find_dlg;
private slots:
    void slot_item_clicked(QListWidgetItem*item);
    void slot_user_search(std::shared_ptr<SearchInfo> si);

};

#endif // SEARCHLIST_H
