#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include "user.h"
#include "table.h"
#include "chat.h"
#include "init_game.h"


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
        void Get_Status_Game();

    public:

        Game(QWidget* parent=0);

};


#endif
