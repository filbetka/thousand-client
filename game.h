#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include "user.h"
#include "table.h"
#include "chat.h"
#include "init_game.h"
#include "bidding.h"
#include "dealing_stock.h"
#include "score_board.h"


class Game: public QMainWindow
{
    Q_OBJECT

    public:

        Http_Manager* network;
        User* user;
        Table* table;
        Chat* chat;
        Init_Game* init_game;
        Bidding* bidding;
        Score_Board* scores;

        QString marriage;

        void Create_Window();
        void Launch_Bidding(QJsonObject reply);
        void Winner_Bidding(QJsonObject reply);
        void Check_Marriage(QJsonObject reply);
        void Game_Over(QJsonObject reply);

    private slots:

        void Logout_SLOT();
        void Create_Game_SLOT();
        void Score_Board_SLOT();
        void Leave_Game_SLOT();
        void Get_Status_Game();

    public:

        Game(QWidget* parent=0);

        void closeEvent(QCloseEvent* event);

};


#endif
