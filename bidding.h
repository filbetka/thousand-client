#ifndef BIDDING_H
#define BIDDING_H

#include <QtWidgets>
#include "style.h"
#include "http.h"
class Game;


class Bidding: public QDialog
{
    Q_OBJECT

    private:

        Http_Manager* network;
        Game* game;

        QListWidget* biddings;
        QLineEdit* points;
        QJsonArray old_biddings;
        QLabel* current_user;

            // buttons
        QPushButton* finish;
        QPushButton* boom;
        QPushButton* pass;
        QPushButton* bid;

        void Create_Window();
        void Add_Bidding_Widget(QString points, QString user);
        void Manage_Butttons(QJsonObject reply);

    private slots:

        void Pass();
        void Bid();
        void Finish();
        void Boom();

    public:

        Bidding(Game* game);

        void Load_Biddings(QJsonObject reply);
        void Show_Message(QJsonObject reply);

};


#endif
