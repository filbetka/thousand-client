#ifndef CARD_H
#define CARD_H

#include <QtWidgets>
#include <QStylePainter>
#include "style.h"
#include "http.h"
class Table;


enum COLOR
{
    HEART, // serce
    DIAMOND, // karo
    CLOVER, // trefl
    SPADES, // pik
    NONE_COLOR
};

enum RANK
{
    ACE,
    _10,
    KING,
    QUEEN,
    JACK,
    _9,
    NONE_RANK
};


class Rotated_Text: public QLabel
{

    private:

        qreal rotate;
        void paintEvent(QPaintEvent* event);

    public:

        Rotated_Text(QString text);
        void Set_Rotate(qreal rotate);

};


class Card: public QPushButton
{
    Q_OBJECT

    private:

        Http_Manager* network;
        Table* table;
        COLOR color;
        RANK rank;

        void Create_Card();
        void Set_Style();

        COLOR Get_Color(QString color);
        RANK Get_Rank(QString rank);

    public:

        Card(COLOR color, RANK rank, Table* table=0);
        Card(QString color, QString rank, Table* table=0);

        static QString Get_Rank_Name(RANK rank);
        static QString Get_Rank_Full_Name(RANK rank);
        static QString Get_Color_Name(COLOR rank);
        static QString Get_Color_Path(COLOR color);

        void mouseDoubleClickEvent(QMouseEvent* event);

};


#endif
