#ifndef CARD_H
#define CARD_H

#include <QtWidgets>
#include <QStylePainter>
#include "style.h"

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

        COLOR color;
        RANK rank;

        void Create_Card();
        void Set_Style();

    public:

        Card(COLOR color, RANK rank);

        static QString Get_Rank_Name(RANK rank);
        static QString Get_Color_Path(COLOR color);

};


#endif
