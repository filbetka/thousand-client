#ifndef TABLE_H
#define TABLE_H

#include "card.h"
class Game;


class Table: public QWidget
{
    Q_OBJECT

    private:

        Http_Manager* network;
        Game* game;
        QTimer* timer;

        QHBoxLayout* table_cards;
        QHBoxLayout* table_stock;
        QLabel* current_user;
        QLabel* first_user;
        QLabel* second_user;

        QJsonArray old_cards;
        QJsonArray old_stock;

        void Create_Table();
        QLabel* Create_User_Details(QString text="");

    private slots:

        void Clear_Stock();

    public:

        Table(Game* game);

        void Load_Cards(QJsonObject reply);
        void Load_Stock(QJsonObject reply);
        void Update_Users(QJsonObject reply);
        void Check_Stock(QJsonObject reply);

};


#endif
