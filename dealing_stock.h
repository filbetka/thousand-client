#ifndef DEALING_STOCK_H
#define DEALING_STOCK_H

#include <QtWidgets>
#include "http.h"
#include "style.h"
class Game;


class Row_Card: public QWidget
{
    Q_OBJECT

    private:

        void Create();

    public:

        QString color;
        QString rank;

        Row_Card(QString color, QString rank);

};



class Dealing_Stock: public QDialog
{
    Q_OBJECT

    private:

        Http_Manager* network;
        QListWidget* first_player_card;
        QListWidget* second_player_card;
        Game* game;

        void Create_Window();
        void Load_Cards();
        QStringList Get_Players();

        void Add_Line_Widget(QString color, QString rank);
        bool Validate_Selected();
        QString Get_Card_Information(QListWidget* list);
        QStringList Get_Other_Players();

    private slots:

        void Accept();

    public:

        Dealing_Stock(Game* game);

};


#endif
