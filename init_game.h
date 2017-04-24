#ifndef INIT_GAME_H
#define INIT_GAME_H

#include <QtWidgets>
#include "style.h"
#include "http.h"


class Init_Game: public QDialog
{
    Q_OBJECT

    private:

        Http_Manager* network;
        QListWidget* first_player;
        QListWidget* second_player;
        QStringList players;

        void Create_Window();
        void Add_User_Widget(QString username);
        QString Get_Selected_User(QListWidget* list);
        bool Validate_Selected();

    private slots:

        void Start_Game_SLOT();

    public:

        Init_Game(QWidget* parent);

        QStringList Get_Players();
        void Load_Online_Users();
};


#endif // INIT_GAME_H
