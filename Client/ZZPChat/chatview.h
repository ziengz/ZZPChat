
#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>


class ChatView : public QWidget
{
    Q_OBJECT
public:
    explicit ChatView(QWidget *parent = nullptr);
    void appendChatItem(QWidget *item);  //尾插
protected:
    bool eventFilter(QObject *watched, QEvent *event)override;
    void paintEvent(QPaintEvent *event) override;
private slots:
    void onVScrollBarMoved(int min,int max);
private:
    void initStyleSheet();

    QVBoxLayout* m_pVl;
    QScrollArea* m_pScrollArea;
    bool isAppended;
};

#endif // CHATVIEW_H
