#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global.h"
#include <QList>
#include "statewidget.h"

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

private:
    void ShowSearch(bool b_search);
    void addChatUserList();
    void AddLBGroup(StateWidget* lb);
    void ClearLabelState(StateWidget *lb);

    bool eventFilter(QObject *watched, QEvent *event)override;
    void handleGlobalMousePress(QMouseEvent* event);
    Ui::ChatDialog *ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
    QList<StateWidget*> _lb_list;

private slots:
    void slot_loading_chat_user();
    void slot_side_chat();
    void slot_side_contect();
    void slot_text_changed(const QString& text);
};

#endif // CHATDIALOG_H
