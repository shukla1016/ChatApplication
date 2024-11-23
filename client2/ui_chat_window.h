/********************************************************************************
** Form generated from reading UI file 'chat_window.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHAT_WINDOW_H
#define UI_CHAT_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatWindow
{
public:
    QVBoxLayout *verticalLayout;
    QTextBrowser *chatHistory;
    QHBoxLayout *horizontalLayout;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QHBoxLayout *connectionLayout;
    QLineEdit *serverIP;
    QLineEdit *serverPort;
    QPushButton *connectButton;

    void setupUi(QWidget *ChatWindow)
    {
        if (ChatWindow->objectName().isEmpty())
            ChatWindow->setObjectName(QString::fromUtf8("ChatWindow"));
        ChatWindow->resize(400, 500);
        verticalLayout = new QVBoxLayout(ChatWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        chatHistory = new QTextBrowser(ChatWindow);
        chatHistory->setObjectName(QString::fromUtf8("chatHistory"));

        verticalLayout->addWidget(chatHistory);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        messageInput = new QLineEdit(ChatWindow);
        messageInput->setObjectName(QString::fromUtf8("messageInput"));

        horizontalLayout->addWidget(messageInput);

        sendButton = new QPushButton(ChatWindow);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));

        horizontalLayout->addWidget(sendButton);


        verticalLayout->addLayout(horizontalLayout);

        connectionLayout = new QHBoxLayout();
        connectionLayout->setObjectName(QString::fromUtf8("connectionLayout"));
        serverIP = new QLineEdit(ChatWindow);
        serverIP->setObjectName(QString::fromUtf8("serverIP"));

        connectionLayout->addWidget(serverIP);

        serverPort = new QLineEdit(ChatWindow);
        serverPort->setObjectName(QString::fromUtf8("serverPort"));

        connectionLayout->addWidget(serverPort);

        connectButton = new QPushButton(ChatWindow);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));

        connectionLayout->addWidget(connectButton);


        verticalLayout->addLayout(connectionLayout);


        retranslateUi(ChatWindow);

        QMetaObject::connectSlotsByName(ChatWindow);
    } // setupUi

    void retranslateUi(QWidget *ChatWindow)
    {
        ChatWindow->setWindowTitle(QCoreApplication::translate("ChatWindow", "Chat Application", nullptr));
        chatHistory->setPlaceholderText(QCoreApplication::translate("ChatWindow", "Chat history will appear here...", nullptr));
        messageInput->setPlaceholderText(QCoreApplication::translate("ChatWindow", "Type your message...", nullptr));
        sendButton->setText(QCoreApplication::translate("ChatWindow", "Send", nullptr));
        serverIP->setPlaceholderText(QCoreApplication::translate("ChatWindow", "Server IP", nullptr));
        serverPort->setPlaceholderText(QCoreApplication::translate("ChatWindow", "Port", nullptr));
        connectButton->setText(QCoreApplication::translate("ChatWindow", "Connect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatWindow: public Ui_ChatWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAT_WINDOW_H
