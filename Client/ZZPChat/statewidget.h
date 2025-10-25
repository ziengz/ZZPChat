
#ifndef STATEWIDGET_H
#define STATEWIDGET_H

#include <QWidget>



class StateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StateWidget(QWidget *parent = nullptr);
    void SetState(QString normal = "",QString hover="",QString press="",
                  QString select="",QString select_hover="",QString select_press="");



signals:

};

#endif // STATEWIDGET_H
