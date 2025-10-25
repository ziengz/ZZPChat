/********************************************************************************
** Form generated from reading UI file 'chatuserwid.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATUSERWID_H
#define UI_CHATUSERWID_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatUserWid
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *icon_wid;
    QLabel *icon_lb;
    QLabel *red_point;
    QWidget *user_info_wid;
    QVBoxLayout *verticalLayout;
    QLabel *user_name_lb;
    QLabel *user_chat_lb;
    QWidget *time_wid;
    QLabel *time_lb;

    void setupUi(QWidget *ChatUserWid)
    {
        if (ChatUserWid->objectName().isEmpty())
            ChatUserWid->setObjectName("ChatUserWid");
        ChatUserWid->resize(418, 75);
        ChatUserWid->setMinimumSize(QSize(0, 75));
        ChatUserWid->setMaximumSize(QSize(16777215, 75));
        horizontalLayout = new QHBoxLayout(ChatUserWid);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(6, 2, 6, 2);
        icon_wid = new QWidget(ChatUserWid);
        icon_wid->setObjectName("icon_wid");
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(icon_wid->sizePolicy().hasHeightForWidth());
        icon_wid->setSizePolicy(sizePolicy);
        icon_wid->setMinimumSize(QSize(60, 60));
        icon_wid->setMaximumSize(QSize(60, 60));
        icon_lb = new QLabel(icon_wid);
        icon_lb->setObjectName("icon_lb");
        icon_lb->setGeometry(QRect(7, 12, 45, 45));
        icon_lb->setMinimumSize(QSize(45, 45));
        icon_lb->setMaximumSize(QSize(45, 45));
        icon_lb->setAlignment(Qt::AlignCenter);
        red_point = new QLabel(icon_wid);
        red_point->setObjectName("red_point");
        red_point->setGeometry(QRect(27, 0, 30, 30));
        red_point->setMinimumSize(QSize(30, 30));
        red_point->setMaximumSize(QSize(30, 30));

        horizontalLayout->addWidget(icon_wid);

        user_info_wid = new QWidget(ChatUserWid);
        user_info_wid->setObjectName("user_info_wid");
        user_info_wid->setMinimumSize(QSize(120, 0));
        user_info_wid->setMaximumSize(QSize(120, 16777215));
        verticalLayout = new QVBoxLayout(user_info_wid);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(2, 5, 2, 5);
        user_name_lb = new QLabel(user_info_wid);
        user_name_lb->setObjectName("user_name_lb");
        user_name_lb->setMinimumSize(QSize(0, 30));
        user_name_lb->setMaximumSize(QSize(16777215, 30));
        QFont font;
        font.setPointSize(10);
        user_name_lb->setFont(font);

        verticalLayout->addWidget(user_name_lb);

        user_chat_lb = new QLabel(user_info_wid);
        user_chat_lb->setObjectName("user_chat_lb");
        user_chat_lb->setMinimumSize(QSize(0, 30));
        user_chat_lb->setMaximumSize(QSize(130, 30));
        QFont font1;
        font1.setPointSize(8);
        user_chat_lb->setFont(font1);

        verticalLayout->addWidget(user_chat_lb);


        horizontalLayout->addWidget(user_info_wid);

        time_wid = new QWidget(ChatUserWid);
        time_wid->setObjectName("time_wid");
        sizePolicy.setHeightForWidth(time_wid->sizePolicy().hasHeightForWidth());
        time_wid->setSizePolicy(sizePolicy);
        time_wid->setMinimumSize(QSize(50, 50));
        time_wid->setMaximumSize(QSize(50, 50));
        time_lb = new QLabel(time_wid);
        time_lb->setObjectName("time_lb");
        time_lb->setGeometry(QRect(0, 20, 54, 12));
        time_lb->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(time_wid);


        retranslateUi(ChatUserWid);

        QMetaObject::connectSlotsByName(ChatUserWid);
    } // setupUi

    void retranslateUi(QWidget *ChatUserWid)
    {
        ChatUserWid->setWindowTitle(QCoreApplication::translate("ChatUserWid", "Form", nullptr));
        icon_lb->setText(QString());
        red_point->setText(QString());
        user_name_lb->setText(QCoreApplication::translate("ChatUserWid", "llfc", nullptr));
        user_chat_lb->setText(QCoreApplication::translate("ChatUserWid", "Hello World!", nullptr));
        time_lb->setText(QCoreApplication::translate("ChatUserWid", "13:54", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatUserWid: public Ui_ChatUserWid {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATUSERWID_H
