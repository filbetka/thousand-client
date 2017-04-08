#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include "user.h"
#include "table.h"
#include "chat.h"


class Init_Game: public QDialog
{
    Q_OBJECT

    private:

        Http_Manager* network;
        QListWidget* users;
        QLineEdit* user_1;
        QLineEdit* user_2;

        void Create_Window();
        void Load_Online_Users();
        void Add_User_Widget(QString username);

    private slots:

        void Start_Game_SLOT();

    public:

        Init_Game(QWidget* parent);
};



class Game: public QMainWindow
{
    Q_OBJECT

    private:

        Http_Manager* network;
        User* user;
        Table* table;
        Chat* chat;

        void Create_Window();

    private slots:

        void Logout_SLOT();
        void Create_Game_SLOT();
        void Check_Status_Game();

    public:

        Game(QWidget* parent=0);

};


#endif
