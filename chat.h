#ifndef CHAT_H
#define CHAT_H

#include <QtWidgets>
#include "style.h"
#include "http.h"
class Chat;
class Game;


class Message_Edit: public QTextEdit
{
    Q_OBJECT

    private:

        Chat* chat;

    public:

        Message_Edit(Chat* chat);
        void keyPressEvent(QKeyEvent *event);

};



class Chat : public QWidget
{
    Q_OBJECT

    private:

        Http_Manager* network;
        Game* game;

        QListWidget* message_list;
        QJsonArray old_messages;

        void Create_Chat();
        void Create_Message(QString user,
                QString message, QString date);

    public:

        Chat(Game* game);

        void Send_New_Message(QString message);
        void Load_All_Messages(QJsonObject reply);

};


#endif
