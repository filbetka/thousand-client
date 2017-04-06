#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include "http_request.h"


class Game: public QMainWindow
{
    Q_OBJECT

    private:

        Form_Manager form;

    public:

        Game(QWidget* parent=0);

};

#endif
