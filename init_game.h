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


#endif // INIT_GAME_H
