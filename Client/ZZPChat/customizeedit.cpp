
#include "customizeedit.h"


CustomizeEdit::CustomizeEdit(QWidget *parent):QLineEdit(parent)
{
    connect(this,&QLineEdit::textChanged,this,&CustomizeEdit::limitTextLen);
}

void CustomizeEdit::SetMaxLength(int maxLen)
{
    _max_len = maxLen;
}
