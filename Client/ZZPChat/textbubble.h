
#ifndef TEXTBUBBLE_H
#define TEXTBUBBLE_H

#include "bubbleframe.h"
#include "global.h"
#include <QTextEdit>
#include <QHBoxLayout>


class TextBubble : public BubbleFrame
{
    Q_OBJECT
public:
    TextBubble(ChatRole role,const QString& text,QWidget* parent = nullptr);
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    void adjustTextHeight();
    void setPlainText(const QString& text);
    void initStyleSheet();

    QTextEdit* m_pTextEdit;
};

#endif // TEXTBUBBLE_H
